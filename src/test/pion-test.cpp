// pion-test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <websocketpp/config/boost_config.hpp>

#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <pion/http/server.hpp>
#include <pion/algorithm.hpp>
#include <pion/http/response_writer.hpp>
#include <pion/user.hpp>

#include <boost/bind/bind.hpp>

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <../services/FileService.hpp>

using namespace pion;
namespace fs = boost::filesystem;

int main(int argc, char** argv)
{
    //pion::logger logger(PION_GET_LOGGER("pion.main.app"));
    //logger.m_priority = pion::logger::LOG_LEVEL_DEBUG;

	pion::http::server server(8585);

	server.add_redirect("/", "/echo");
	server.add_resource("/echo", [](const pion::http::request_ptr& http_request_ptr,
									const pion::tcp::connection_ptr& tcp_conn) {
		// this web service uses static text to test the mixture of "copied" with
		// "static" (no-copy) text
		static const std::string REQUEST_ECHO_TEXT("[Request Echo]");
		static const std::string REQUEST_HEADERS_TEXT("[Request Headers]");
		static const std::string QUERY_PARAMS_TEXT("[Query Parameters]");
		static const std::string COOKIE_PARAMS_TEXT("[Cookie Parameters]");
		static const std::string POST_CONTENT_TEXT("[POST Content]");
		static const std::string USER_INFO_TEXT("[USER Info]");

		// Set Content-type to "text/plain" (plain ascii text)
		http::response_writer_ptr writer(http::response_writer::create(tcp_conn, *http_request_ptr,
			boost::bind(&tcp::connection::finish, tcp_conn)));
		writer->get_response().set_content_type(http::types::CONTENT_TYPE_TEXT);

		// write request information
		writer->write_no_copy(REQUEST_ECHO_TEXT);
		writer->write_no_copy(http::types::STRING_CRLF);
		writer->write_no_copy(http::types::STRING_CRLF);
		writer
			<< "Request method: "
			<< http_request_ptr->get_method()
			<< http::types::STRING_CRLF
			<< "Resource originally requested: "
			<< http_request_ptr->get_original_resource()
			<< http::types::STRING_CRLF
			<< "Resource delivered: "
			<< http_request_ptr->get_resource()
			<< http::types::STRING_CRLF
			<< "Query string: "
			<< http_request_ptr->get_query_string()
			<< http::types::STRING_CRLF
			<< "HTTP version: "
			<< http_request_ptr->get_version_major() << '.' << http_request_ptr->get_version_minor()
			<< http::types::STRING_CRLF
			<< "Content length: "
			<< (unsigned long)http_request_ptr->get_content_length()
			<< http::types::STRING_CRLF
			<< http::types::STRING_CRLF;

		// write request headers
		writer->write_no_copy(REQUEST_HEADERS_TEXT);
		writer->write_no_copy(http::types::STRING_CRLF);
		writer->write_no_copy(http::types::STRING_CRLF);
		std::for_each(http_request_ptr->get_headers().begin(), http_request_ptr->get_headers().end(),
			[writer](const ihash_multimap::value_type& val)
		{
			// text is copied into writer text cache
			writer << val.first << http::types::HEADER_NAME_VALUE_DELIMITER
				<< val.second
				<< http::types::STRING_CRLF;
		});
		writer->write_no_copy(http::types::STRING_CRLF);

		// write query parameters
		writer->write_no_copy(QUERY_PARAMS_TEXT);
		writer->write_no_copy(http::types::STRING_CRLF);
		writer->write_no_copy(http::types::STRING_CRLF);
		std::for_each(http_request_ptr->get_queries().begin(), http_request_ptr->get_queries().end(),
			[writer](const ihash_multimap::value_type& val)
		{
			// text is copied into writer text cache
			writer << val.first << http::types::HEADER_NAME_VALUE_DELIMITER
				<< val.second
				<< http::types::STRING_CRLF;
		});
		writer->write_no_copy(http::types::STRING_CRLF);

		// write cookie parameters
		writer->write_no_copy(COOKIE_PARAMS_TEXT);
		writer->write_no_copy(http::types::STRING_CRLF);
		writer->write_no_copy(http::types::STRING_CRLF);
		std::for_each(http_request_ptr->get_cookies().begin(), http_request_ptr->get_cookies().end(),
			[writer](const ihash_multimap::value_type& val)
		{
			// text is copied into writer text cache
			writer << val.first << http::types::HEADER_NAME_VALUE_DELIMITER
				<< val.second
				<< http::types::STRING_CRLF;
		});
		writer->write_no_copy(http::types::STRING_CRLF);

		// write POST content
		writer->write_no_copy(POST_CONTENT_TEXT);
		writer->write_no_copy(http::types::STRING_CRLF);
		writer->write_no_copy(http::types::STRING_CRLF);
		if (http_request_ptr->get_content_length() != 0) {
			writer->write(http_request_ptr->get_content(), http_request_ptr->get_content_length());
			writer->write_no_copy(http::types::STRING_CRLF);
			writer->write_no_copy(http::types::STRING_CRLF);
		}

		// if authenticated, write user info
		user_ptr user = http_request_ptr->get_user();
		if (user) {
			writer->write_no_copy(USER_INFO_TEXT);
			writer->write_no_copy(http::types::STRING_CRLF);
			writer->write_no_copy(http::types::STRING_CRLF);
			writer << "User authenticated, username: " << user->get_username();
			writer->write_no_copy(http::types::STRING_CRLF);
		}

		// send the writer
		writer->send();
	});


	typedef websocketpp::server<websocketpp::config::asio> websocket_server_t;

	websocket_server_t websocket_server;

	server.add_resource("/websocket", [&websocket_server](const pion::http::request_ptr& http_request_ptr,
		                                                  const pion::tcp::connection_ptr& tcp_conn) {
		std::string request = http_request_ptr->get_method() + " " + http_request_ptr->get_resource() + " HTTP/1.1\r\n";
		for (const auto& quiery : http_request_ptr->get_headers())
			request += quiery.first + ": " + quiery.second + "\r\n";
		request += "\r\n";

		websocket_server.adopt_socket(request, std::move(const_cast<pion::tcp::connection_ptr&>(tcp_conn)->get_ssl_socket().next_layer()));

		const_cast<pion::tcp::connection_ptr&>(tcp_conn)->close();
	});

    pion::plugins::FileService fileService;

    boost::system::error_code ec;
    auto path = fs::initial_path(ec);
    if (!ec && !path.empty() && fs::is_directory(path)) {
        //PION_LOG_DEBUG(logger, "Found path : " << path.string());

        fileService.set_option("directory", path.string());
        fileService.set_option("cache", "2");
        fileService.set_option("scan", "0");

        server.add_resource("/files", boost::bind(&pion::plugins::FileService::operator(), &fileService, boost::placeholders::_1, boost::placeholders::_2));
    }

	server.start();
    

	typedef websocket_server_t::message_ptr message_ptr;
	

	// Set logging settings
	websocket_server.set_access_channels(websocketpp::log::alevel::all);
	websocket_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

	// Initialize Asio
	websocket_server.init_asio();

	// Register our message handler
	websocket_server.set_message_handler([&websocket_server](websocketpp::connection_hdl hdl, message_ptr msg) {
		std::cout << "on_message called with hdl: " << hdl.lock().get()
			<< " and message: " << msg->get_payload()
			<< std::endl;

		// check for a special command to instruct the server to stop listening so
		// it can be cleanly exited.
		if (msg->get_payload() == "stop-listening") {
			websocket_server.stop_listening();
			return;
		}

		try {
			websocket_server.send(hdl, msg->get_payload(), msg->get_opcode());
		}
		catch (const websocketpp::lib::error_code& e) {
			std::cout << "Echo failed because: " << e
				<< "(" << e.message() << ")" << std::endl;
		}
	});

	// Listen on port 9002
	websocket_server.listen(9002);

	// Start the server accept loop
	websocket_server.start_accept();

	// Start the ASIO io_service run loop
	websocket_server.run();


	//server.join();

    //::Sleep(5000);

    //server.stop();

    return 0;
}

