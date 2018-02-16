#include <pion/http/plugin_server.hpp>
#include <cstdlib>


PION_DECLARE_PLUGIN(EchoService);

int main(int, char**) {

    pion::http::plugin_server_ptr web_server;
    web_server.reset(new pion::http::plugin_server(8080u));
    web_server->load_service("/echo", "EchoService");
    
    return EXIT_SUCCESS;
}
