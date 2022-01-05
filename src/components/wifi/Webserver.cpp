#include <WebServer.h>

WebServer server(80);

void handle_root()
{
    server.send(200, "text/html", "html hier ofzo");
}

void InitWebserver()
{

    server.on("/", handle_root);
    server.begin();
}

void handleWebserver()
{
    server.handleClient();
}