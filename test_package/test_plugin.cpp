#include <pion/http/plugin_service.hpp>
#include <cstdlib>


namespace pion {        // begin namespace pion
namespace plugins {     // begin namespace plugins

class TestPlugin :
    public pion::http::plugin_service
{
public:
    ODAFavicon() = default;
    virtual ~ODAFavicon() = default;
    virtual void operator()(const pion::http::request_ptr& http_request_ptr,
                            const pion::tcp::connection_ptr& tcp_conn);
};

void ODAFavicon::operator()(const http::request_ptr& http_request_ptr, const tcp::connection_ptr& tcp_conn) {
    
    http::response_writer_ptr writer(http::response_writer::create(tcp_conn, *http_request_ptr,
                                     boost::bind(&tcp::connection::finish, tcp_conn)));
    
    writer->get_response().set_status_code(http::types::RESPONSE_CODE_NOT_FOUND);
    writer->get_response().set_status_message(http::types::RESPONSE_MESSAGE_NOT_FOUND);

    // send the writer
    writer->send();
}

}   // end namespace plugins
}   // end namespace pion

/// creates new ODAFavicon objects
extern "C" PION_PLUGIN pion::plugins::ODAFavicon *pion_create_ODAFavicon(void)
{
    return new pion::plugins::ODAFavicon();
}

/// destroys ODAFavicon objects
extern "C" PION_PLUGIN void pion_destroy_ODAFavicon(pion::plugins::ODAFavicon *service_ptr)
{
    delete service_ptr;
}


int main(int, char**) {
    //websocketpp::server<websocketpp::config::asio> server;
    return EXIT_SUCCESS;
}
