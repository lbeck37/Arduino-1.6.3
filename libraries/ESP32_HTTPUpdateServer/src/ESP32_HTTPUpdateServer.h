#ifndef ESP32_HTTP_UPDATE_SERVER_H
#define ESP32_HTTP_UPDATE_SERVER_H

class WebServer;

class ESP32_HTTPUpdateServer
{
  private:
    bool _serial_output;
    WebServer *_server;
    static const char *_serverIndex;
    static const char *_failedResponse;
    static const char *_successResponse;
    char * _username;
    char * _password;
    bool _authenticated;
  public:
    ESP32_HTTPUpdateServer(bool serial_debug=false);

    void setup(WebServer *server)
    {
      setup(server, NULL, NULL);
    }

    void setup(WebServer *server, const char * path)
    {
      setup(server, path, NULL, NULL);
    }

    void setup(WebServer *server, const char * username, const char * password)
    {
      setup(server, "/update", username, password);
    }

    void setup(WebServer *server, const char * path, const char * username, const char * password);
};
#endif	//ESP32_HTTP_UPDATE_SERVER_H
