#include <pion/plugin.hpp>
#include <pion/http/plugin_server.hpp>
#include <pion/http/plugin_service.hpp>
#include <pion/http/response_writer.hpp>
#include <cstdlib>


namespace pion {
namespace plugins {

PION_DECLARE_PLUGIN(TestPlugin);

class TestPlugin :
    public ::pion::http::plugin_service
{
public:
    TestPlugin() = default;
    virtual ~TestPlugin() = default;
    virtual void operator()(const ::pion::http::request_ptr& http_request_ptr,
                            const ::pion::tcp::connection_ptr& tcp_conn) override;
};

void TestPlugin::operator()(const ::pion::http::request_ptr& http_request_ptr, const ::pion::tcp::connection_ptr& tcp_conn) {
    
    ::pion::http::response_writer_ptr writer(::pion::http::response_writer::create(tcp_conn, *http_request_ptr,
                                     boost::bind(&tcp::connection::finish, tcp_conn)));
    
    writer->get_response().set_status_code(::pion::http::types::RESPONSE_CODE_NOT_FOUND);
    writer->get_response().set_status_message(::pion::http::types::RESPONSE_MESSAGE_NOT_FOUND);

    // send the writer
    writer->send();
}

}
}

/// creates new TestPlugin objects
extern "C" PION_PLUGIN pion::plugins::TestPlugin* pion_create_TestPlugin(void)
{
    return new pion::plugins::TestPlugin();
}

/// destroys TestPlugin objects
extern "C" PION_PLUGIN void pion_destroy_TestPlugin(pion::plugins::TestPlugin* service_ptr)
{
    delete service_ptr;
}


int main(int, char**) {

    pion::http::plugin_server_ptr web_server;
    web_server.reset(new pion::http::plugin_server(8080u));
    web_server->load_service("/robots.txt", "TestPlugin");
    
    return EXIT_SUCCESS;
}
