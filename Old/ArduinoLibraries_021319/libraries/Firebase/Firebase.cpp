// Beck 4/24/16
#include <BeckLib.h>
#include "Firebase.h"
//#define DEBUG_LOGGING

// Detect whether stable version of HTTP library is installed instead of
// master branch and patch in missing status and methods.
#ifndef HTTP_CODE_TEMPORARY_REDIRECT
#define HTTP_CODE_TEMPORARY_REDIRECT 307
#define USE_ESP_ARDUINO_CORE_2_0_0
#endif

namespace {
const char* kFirebaseFingerprint = "7A 54 06 9B DC 7A 25 B3 86 8D 66 53 48 2C 0B 96 42 C7 B3 0A";
const uint16_t kFirebasePort = 443;

String makeFirebaseURL(const String& path, const String& auth) {
  String url;
  if (path[0] != '/') {
    url = "/";
  }
  url += path + ".json";
  if (auth.length() > 0) {
    url += "?auth=" + auth;
  }
  return url;
}

}  // namespace

/*
//Beck 4/16/16
Firebase::Firebase() {
  http_.setReuse(true);
}	//default ctor
*/


Firebase::Firebase(const String& host) : host_(host) {
#ifdef DEBUG_LOGGING
  Serial << LOG0 << " Firebase ctor  Current Value: host_= " << host_ << endl;
  Serial << LOG0 << "   host_= " << host_ << ", auth_= " << auth_ << endl;
#endif
  http_.setReuse(true);
}


Firebase& Firebase::auth(const String& auth) {
  auth_ = auth;
#ifdef DEBUG_LOGGING
  Serial << LOG0 << " Firebase::auth()  Current Value: host_= " << host_ << endl;
  Serial << LOG0 << "   auth_= " << auth_ << endl;
#endif
  return *this;
}


void Firebase::PrintPrivates(void) {
#ifdef DEBUG_LOGGING
  Serial << LOG0 << " Firebase::PrintPrivates()  Current Values: host_= " << host_ << endl;
  Serial << LOG0 << "   auth_= " << auth_ << endl;
#endif
  return;
}	//PrintPrivates


FirebaseGet Firebase::get(const String& path) {
#ifdef DEBUG_LOGGING
  Serial << LOG0 << " Firebase::get()  Current Values: path= " << path << endl;
  Serial << LOG0 << "   host_= " << host_ << ", auth_= " << auth_ << endl;
#endif
  return FirebaseGet(host_, auth_, path, &http_);
}

FirebaseSet Firebase::set(const String& path, const String& value) {
  return FirebaseSet(host_, auth_, path, value, &http_);
}

FirebasePush Firebase::push(const String& path, const String& value) {
  return FirebasePush(host_, auth_, path, value, &http_);
}

FirebaseRemove Firebase::remove(const String& path) {
  return FirebaseRemove(host_, auth_, path, &http_);
}

FirebaseStream Firebase::stream(const String& path) {
  // TODO: create new client dedicated to stream.
  return FirebaseStream(host_, auth_, path, &http_);
}

// FirebaseCall
FirebaseCall::FirebaseCall(const String& host, const String& auth,
                           const char* method, const String& path,
                           const String& data, HTTPClient* http) : http_(http) {
  String url = makeFirebaseURL(path, auth);
#ifdef DEBUG_LOGGING
  Serial << LOG0 << " FirebaseCall() Ctor  host= " << host << ", auth= " << auth << endl;
  Serial << LOG0 << "  url= " << url << "   method= " << method << ", path= " << path << ", data= " << data << endl;
#endif

  http_->setReuse(true);
  http_->begin(host, kFirebasePort, url, true, kFirebaseFingerprint);

  bool followRedirect = false;
  if (method == "STREAM") {
    method = "GET";
    http_->addHeader("Accept", "text/event-stream");
    followRedirect = true;
  }

  if (followRedirect) {
    const char* headers[] = {"Location"};
    http_->collectHeaders(headers, 1);
  }

#ifdef DEBUG_LOGGING
  Serial << LOG0 << " FirebaseCall() Ctor: Calling http_->sendRequest(method,  data, length)" << endl;
  Serial << LOG0 << "   method= " << method << ", data= " << data.c_str() << ", length= " << data.length() << endl;
#endif
  int status = http_->sendRequest(method, (uint8_t*)data.c_str(), data.length());

  // TODO: Add a max redirect check
  if (followRedirect) {
    while (status == HTTP_CODE_TEMPORARY_REDIRECT) {
      String location = http_->header("Location");
      http_->setReuse(false);
      http_->end();
      http_->setReuse(true);
      http_->begin(location, kFirebaseFingerprint);
      status = http_->sendRequest("GET", (uint8_t*)NULL, 0);
    }
  }

  if (status != 200) {
#ifdef USE_ESP_ARDUINO_CORE_2_0_0
    error_ = FirebaseError(status, String(method) + " " + url + ": " + status);
#else
    error_ = FirebaseError(status, String(method) + " " + url + ": " + HTTPClient::errorToString(status));
#endif
  }

  // if not streaming.
  if (!followRedirect) {
    response_ = http_->getString();
  }
}

// FirebaseGet
FirebaseGet::FirebaseGet(const String& host, const String& auth,
                         const String& path,
                         HTTPClient* http)
  : FirebaseCall(host, auth, "GET", path, "", http) {
#ifdef DEBUG_LOGGING
  Serial << LOG0 << " FirebaseGet() Ctor:  host= " << host << ", auth= " << auth << ", path= " << path << endl;
#endif
  if (!error()) {
    // TODO: parse json
    json_ = response();
  }
}

// FirebaseSet
FirebaseSet::FirebaseSet(const String& host, const String& auth,
       const String& path, const String& value,
       HTTPClient* http)
  : FirebaseCall(host, auth, "PUT", path, value, http) {
  if (!error()) {
    // TODO: parse json
    json_ = response();
  }
}
// FirebasePush
FirebasePush::FirebasePush(const String& host, const String& auth,
                           const String& path, const String& value,
                           HTTPClient* http)
  : FirebaseCall(host, auth, "POST", path, value, http) {
  if (!error()) {
    // TODO: parse name
    name_ = response();
  }
}

// FirebasePush
FirebaseRemove::FirebaseRemove(const String& host, const String& auth,
                               const String& path,
                               HTTPClient* http)
  : FirebaseCall(host, auth, "DELETE", path, "", http) {
}

// FirebaseStream
FirebaseStream::FirebaseStream(const String& host, const String& auth,
                               const String& path,
                               HTTPClient* http)
  : FirebaseCall(host, auth, "STREAM", path, "", http) {
}

bool FirebaseStream::available() {
  return http_->getStreamPtr()->available();
}

FirebaseStream::Event FirebaseStream::read(String& event) {
  auto client = http_->getStreamPtr();
  Event type;
  String typeStr = client->readStringUntil('\n').substring(7);
  if (typeStr == "put") {
    type = Event::PUT;
  } else if (typeStr == "patch") {
    type = Event::PATCH;
  } else {
    type = Event::UNKNOWN;
  }
  event = client->readStringUntil('\n').substring(6);
  client->readStringUntil('\n'); // consume separator
  return type;
}
