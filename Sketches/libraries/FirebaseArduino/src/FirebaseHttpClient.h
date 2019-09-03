#ifndef FIREBASE_HTTP_CLIENT_H
#define FIREBASE_HTTP_CLIENT_H

#include <string>

#include "Arduino.h"
#include "Stream.h"

struct HttpStatus {
  static const int TEMPORARY_REDIRECT = 307;
};

class FirebaseHttpClient {
 public:
  static FirebaseHttpClient* create();

  virtual void setReuseConnection(bool reuse) = 0;
  virtual void begin(const std::string& url) = 0;
  virtual void begin(const std::string& host, const std::string& path) = 0;

  virtual void end() = 0;

  virtual void addHeader(const std::string& name, const std::string& value) = 0;
  virtual void collectHeaders(const char* header_keys[],
                              const int header_key_count) = 0;
  virtual std::string header(const std::string& name) = 0;

  virtual int sendRequest(const std::string& method, const std::string& data) = 0;

  virtual std::string getString() = 0;

  virtual Stream* getStreamPtr() = 0;

  virtual std::string errorToString(int error_code) = 0;

  virtual bool connected() = 0;

 protected:
  static const uint16_t kFirebasePort = 443;
};
//Beck 9/02/19, This must agree with setting in .ino , like Beck_FirebaseNodeMCU.ino
// Fingerprint for firebaseio.com as in thermo-2b830.firebaseio.com
static const char kFirebaseFingerprint[] =
        "B6 F5 80 C8 B1 DA 61 C1 07 9D 80 42 D8 A9 1F AF 9F C8 96 7D"; // 2019-04 and 9/2/19

// Fingerprint for web.app as in thermo-2b830.web.app
//static const char kFirebaseFingerprint[] =
//        "7D:F4:C4:BB:81:2F:EE:72:29:BD:36:42:6D:80:A7:A7:B3:F9:28:B9";

#endif  // FIREBASE_HTTP_CLIENT_H
