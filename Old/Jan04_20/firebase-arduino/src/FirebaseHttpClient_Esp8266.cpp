//Beck FirebaseHttpClient.h, 1/3/20b
#include "FirebaseHttpClient.h"
#include <stdio.h>

//The ordering of these includes matters greatly.
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Streaming.h>
#include <BeckLogLib.h>

// Detect whether stable version of HTTP library is installed instead of
// master branch and patch in missing status and methods.
#ifndef HTTP_CODE_TEMPORARY_REDIRECT
#define HTTP_CODE_TEMPORARY_REDIRECT 307
#define USE_ESP_ARDUINO_CORE_2_0_0
#endif

// Firebase now returns `Connection: close` after REST streaming redirection.
//
// Override the built-in ESP8266HTTPClient to *not* close the
// connection if forceReuse it set to `true`.
class ForceReuseHTTPClient : public HTTPClient {
public:
  void end() {
    if (_forceReuse) {
      _canReuse = true;
    }
    HTTPClient::end();
  } //end


  void forceReuse(bool forceReuse) {
    _forceReuse = forceReuse;
  } //forceReuse


protected:
  bool _forceReuse = false;
};  //ForceReuseHTTPClient class


class FirebaseHttpClientEsp8266 : public FirebaseHttpClient {
 public:
  FirebaseHttpClientEsp8266() {}

  void setReuseConnection(bool reuse) override {
    http_.setReuse(reuse);
    http_.forceReuse(reuse);
  } //setReuseConnection


  void begin(const std::string& url) override {
    http_.begin(url.c_str(), kFirebaseFingerprint);
  } //begin


  void begin(const std::string& host, const std::string& path) override {
    //Serial << "FirebaseHttpClientEsp8266:begin(" << host << ", " << path << ") Begin" << endl;
/*
    Serial << "FirebaseHttpClientEsp8266:begin(): Call http_.begin(" << host <<
        ", " << kFirebasePort << ", " << path << ", " << kFirebaseFingerprint << ")" << endl;
*/
    Serial << LOG0 << "FirebaseHttpClientEsp8266:begin(): " << endl <<
        "Call http_.begin(" << host.c_str() <<
        ", " << kFirebasePort << ", " << endl << "    " << path.c_str() <<
        ", " << endl << "    " << kFirebaseFingerprint << ")" << endl;
    http_.begin(host.c_str(), kFirebasePort, path.c_str(), kFirebaseFingerprint);
  } //begin


  void end() override {
    http_.end();
  } //end


  void addHeader(const std::string& name, const std::string& value) override {
    http_.addHeader(name.c_str(), value.c_str());
  } //addHeader


  void collectHeaders(const char* header_keys[], const int count) override {
    http_.collectHeaders(header_keys, count);
  } //collectHeaders


  std::string header(const std::string& name) override {
    return http_.header(name.c_str()).c_str();
  } //header


  int sendRequest(const std::string& method, const std::string& data) override {
    Serial << LOG0 << "FirebaseHttpClientEsp8266:sendRequest(): Call http_.sendRequest()" << endl;
    return http_.sendRequest(method.c_str(), (uint8_t*)data.c_str(), data.length());
  } //sendRequest


  std::string getString() override {
    return http_.getString().c_str();
  } //getString


  Stream* getStreamPtr() override {
    return http_.getStreamPtr();
  } //getStreamPtr


  std::string errorToString(int error_code) override {
   return HTTPClient::errorToString(error_code).c_str();
  } //errorToString


  bool connected() override {
    return http_.connected();
  } //connected

 private:
  ForceReuseHTTPClient http_;
};  //FirebaseHttpClientEsp8266 class


//The only non-virtual FirebaseHttpClient class method
FirebaseHttpClient* FirebaseHttpClient::create() {
  return new FirebaseHttpClientEsp8266();
} //create
//Last line.
