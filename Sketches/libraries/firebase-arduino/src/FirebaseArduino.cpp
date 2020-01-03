//Beck, 1/2/20a

#include "FirebaseArduino.h"
#include <SoftwareSerial.h>
#include <BeckLogLib.h>
#include <Streaming.h>

// This is needed to compile std::string on esp8266.
template class std::basic_string<char>;

void FirebaseArduino::begin(const String& host, const String& auth) {
  host_ = host.c_str();
  auth_ = auth.c_str();
}

void FirebaseArduino::initStream() {
  if (stream_http_.get() == nullptr) {
    stream_http_.reset(FirebaseHttpClient::create());
    stream_http_->setReuseConnection(true);
    stream_.reset(new FirebaseStream(stream_http_));
  }
}

void FirebaseArduino::initRequest() {
  if (req_http_.get() == nullptr) {
    req_http_.reset(FirebaseHttpClient::create());
    req_http_->setReuseConnection(true);
    req_.reset(new FirebaseRequest(req_http_));
  }
}

String FirebaseArduino::pushInt(const String& path, int value) {
  return push(path, value);
}

String FirebaseArduino::pushFloat(const String& path, float value) {
  return push(path, value);
}

String FirebaseArduino::pushBool(const String& path, bool value) {
  return push(path, value);
}

String FirebaseArduino::pushString(const String& path, const String& value) {
  JsonVariant json(value.c_str());
  return push(path, json);
}

String FirebaseArduino::push(const String& path, const JsonVariant& value) {
  int size = value.measureLength()+1;
  char * buf = new char[size];
  value.printTo(buf, size);
  initRequest();
  int status = req_.get()->sendRequest(host_, auth_, "POST", path.c_str(), buf);
  error_ = req_.get()->error();
  const char* name = req_.get()->json()["name"].as<const char*>();
  delete buf;
  return name;
}

void FirebaseArduino::setInt(const String& path, int value) {
  set(path, value);
}

void FirebaseArduino::setFloat(const String& path, float value) {
  set(path, value);
}

void FirebaseArduino::setBool(const String& path, bool value) {
  set(path, value);
}

void FirebaseArduino::setString(const String& path, const String& value) {
  JsonVariant json(value.c_str());
  set(path, json);
}

void FirebaseArduino::set(const String& path, const JsonVariant& value) {
  int size = value.measureLength()+1;
  char* buf= new char[size];
  value.printTo(buf, size);
  Serial << LOG0 << "FirebaseArduino::set(): Call initRequest()" << endl;
  initRequest();

  Serial << LOG0 <<"FirebaseArduino::set(): Call req_.get()->sendRequest()" << endl;
  req_.get()->sendRequest(host_, auth_, "PUT", path.c_str(), buf);

  error_ = req_.get()->error();
  Serial << LOG0 <<"FirebaseArduino::set(): error_= |" << error_ << "|" << endl;
  delete buf;
}

void FirebaseArduino::getRequest(const String& path) {
  initRequest();
  req_.get()->sendRequest(host_, auth_, "GET", path.c_str());
  error_ = req_.get()->error();
}

FirebaseObject FirebaseArduino::get(const String& path) {
  getRequest(path);
  if (failed()) {
    return FirebaseObject{""};
  }
  return FirebaseObject(req_.get()->response().c_str());
}

int FirebaseArduino::getInt(const String& path) {
  getRequest(path);
  if (failed()) {
    return 0;
  }
  return FirebaseObject(req_.get()->response().c_str()).getInt();
}


float FirebaseArduino::getFloat(const String& path) {
  getRequest(path);
  if (failed()) {
    return 0.0f;
  }
  return FirebaseObject(req_.get()->response().c_str()).getFloat();
}

String FirebaseArduino::getString(const String& path) {
  getRequest(path);
  if (failed()) {
    return "";
  }
  return FirebaseObject(req_.get()->response().c_str()).getString();
}

bool FirebaseArduino::getBool(const String& path) {
  getRequest(path);
  if (failed()) {
    return "";
  }
  return FirebaseObject(req_.get()->response().c_str()).getBool();
}
void FirebaseArduino::remove(const String& path) {
  initRequest();
  req_.get()->sendRequest(host_, auth_, "DELETE", path.c_str());
  error_ = req_.get()->error();
}

void FirebaseArduino::stream(const String& path) {
  initStream();
  stream_.get()->startStreaming(host_, auth_, path.c_str());
  error_ = stream_.get()->error();
}

bool FirebaseArduino::available() {
  if (stream_http_.get() == nullptr) {
    error_ = FirebaseError(FIREBASE_ERROR_CODES::STREAM_NOT_INITIALIZED, "HTTP stream is not initialized");
    return 0;
  }
  if (!stream_http_.get()->connected()) {
    error_ = FirebaseError(FIREBASE_ERROR_CODES::HTTP_CONNECTION_LOST, "Connection Lost");
    return 0;
  }
  auto client = stream_http_.get()->getStreamPtr();
  return (client == nullptr) ? false : client->available();
}

FirebaseObject FirebaseArduino::readEvent() {
  if (stream_http_.get() == nullptr) {
    return FirebaseObject("");
  }
  auto client = stream_http_.get()->getStreamPtr();
  if (client == nullptr) {
      return FirebaseObject("");
  }
  String type = client->readStringUntil('\n').substring(7);;
  String event = client->readStringUntil('\n').substring(6);
  client->readStringUntil('\n'); // consume separator
  FirebaseObject obj = FirebaseObject(event.c_str());

  // required to have a copy of the string but use a char[] format which is
  // the only supported format for JsonObject#set (it does not like the std::string of the test env)
  char *cstr = new char[type.length() + 1];
  strncpy(cstr, type.c_str(), type.length() + 1);
  obj.getJsonVariant().as<JsonObject&>().set("type", cstr);
  delete[] cstr;
  return obj;
}

bool FirebaseArduino::success() {
  return error_.code() == 0;
}

bool FirebaseArduino::failed() {
  return error_.code() != 0;
}

const String& FirebaseArduino::error() {
  Serial << LOG0 << "FirebaseArduino::error(): error_.message().c_str()= |" << error_.message().c_str() << "|" << endl;
  return error_.message().c_str();
}

FirebaseArduino Firebase;

//Last line.
