#ifndef __ESP32HTTP_UPDATE_SERVER_H
#define __ESP32HTTP_UPDATE_SERVER_H

class WebServer;

class ESP32HTTPUpdateServer
{
  public:
		ESP32HTTPUpdateServer(bool serial_debug=false);

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

  protected:
    void _setUpdaterError();

  private:
    bool _serial_output;
    WebServer *_server;
    char * _username;
    char * _password;
    bool _authenticated;
    String _updaterError;
};
#endif	//__ESP32HTTP_UPDATE_SERVER_H
