//FirebaseSnippits.cpp, 8/26/19
//**********************************************************
//FirebaseESP8266.cpp
String FirebaseData::errorReason() //Actual line 5400
{
    Serial << "FirebaseData::errorReason() begin" << endl;
    std::string buff = "";

    if (_httpCode == _HTTP_CODE_OK)
    {
        if (_bufferOverflow)
            _httpCode = FIREBASE_ERROR_BUFFER_OVERFLOW;
        else if (_mismatchDataType)
            _httpCode = FIREBASE_ERROR_DATA_TYPE_MISMATCH;
        else if (_pathNotExist)
            _httpCode = FIREBASE_ERROR_PATH_NOT_EXIST;
    }

    Serial << "FirebaseData::errorReason(): call Firebase.errorToString()" << endl;
    Firebase.errorToString(_httpCode, buff);

    if (_firebaseError != "")
    {
        Serial << "FirebaseData::errorReason(): _firebaseError != quote quote" << endl;
        char *str = new char[100];
        memset(str, 0, 100);
        strcpy_P(str, ESP8266_FIREBASE_STR_132);
        buff += str;
        memset(str, 0, 100);
        strcpy_P(str, ESP8266_FIREBASE_STR_6);
        buff += str;
        delete[] str;
        buff += _firebaseError;
    }

    Serial << "FirebaseData::errorReason(): Return" << endl;
    return buff.c_str();
} //errorReason


//**********************************************************
void FirebaseESP8266::errorToString(int httpCode, std::string &buff) //Actual line 3793
{
    buff.clear();
    switch (httpCode)
    {
    case HTTPC_ERROR_CONNECTION_REFUSED:
        p_memCopy(buff, ESP8266_FIREBASE_STR_39);
        return;
    case HTTPC_ERROR_SEND_HEADER_FAILED:
        p_memCopy(buff, ESP8266_FIREBASE_STR_40);
        return;
    case HTTPC_ERROR_SEND_PAYLOAD_FAILED:
        p_memCopy(buff, ESP8266_FIREBASE_STR_41);
        return;
    case HTTPC_ERROR_NOT_CONNECTED:
        p_memCopy(buff, ESP8266_FIREBASE_STR_42);
        return;
    case HTTPC_ERROR_CONNECTION_LOST:
        p_memCopy(buff, ESP8266_FIREBASE_STR_43);
        return;
    case HTTPC_ERROR_NO_HTTP_SERVER:
        p_memCopy(buff, ESP8266_FIREBASE_STR_44);
        return;
    case _HTTP_CODE_BAD_REQUEST:
        p_memCopy(buff, ESP8266_FIREBASE_STR_45);
        return;
    case _HTTP_CODE_NON_AUTHORITATIVE_INFORMATION:
        p_memCopy(buff, ESP8266_FIREBASE_STR_46);
        return;
    case _HTTP_CODE_NO_CONTENT:
        p_memCopy(buff, ESP8266_FIREBASE_STR_47);
        return;
    case _HTTP_CODE_MOVED_PERMANENTLY:
        p_memCopy(buff, ESP8266_FIREBASE_STR_48);
        return;
    case _HTTP_CODE_USE_PROXY:
        p_memCopy(buff, ESP8266_FIREBASE_STR_49);
        return;
    case _HTTP_CODE_TEMPORARY_REDIRECT:
        p_memCopy(buff, ESP8266_FIREBASE_STR_50);
        return;
    case _HTTP_CODE_PERMANENT_REDIRECT:
        p_memCopy(buff, ESP8266_FIREBASE_STR_51);
        return;
    case _HTTP_CODE_UNAUTHORIZED:
        p_memCopy(buff, ESP8266_FIREBASE_STR_52);
        return;
    case _HTTP_CODE_FORBIDDEN:
        p_memCopy(buff, ESP8266_FIREBASE_STR_53);
        return;
    case _HTTP_CODE_NOT_FOUND:
        p_memCopy(buff, ESP8266_FIREBASE_STR_54);
        return;
    case _HTTP_CODE_METHOD_NOT_ALLOWED:
        p_memCopy(buff, ESP8266_FIREBASE_STR_55);
        return;
    case _HTTP_CODE_NOT_ACCEPTABLE:
        p_memCopy(buff, ESP8266_FIREBASE_STR_56);
        return;
    case _HTTP_CODE_PROXY_AUTHENTICATION_REQUIRED:
        p_memCopy(buff, ESP8266_FIREBASE_STR_57);
        return;
    case _HTTP_CODE_REQUEST_TIMEOUT:
        p_memCopy(buff, ESP8266_FIREBASE_STR_58);
        return;
    case _HTTP_CODE_LENGTH_REQUIRED:
        p_memCopy(buff, ESP8266_FIREBASE_STR_59);
        return;
    case _HTTP_CODE_TOO_MANY_REQUESTS:
        p_memCopy(buff, ESP8266_FIREBASE_STR_60);
        return;
    case _HTTP_CODE_REQUEST_HEADER_FIELDS_TOO_LARGE:
        p_memCopy(buff, ESP8266_FIREBASE_STR_61);
        return;
    case _HTTP_CODE_INTERNAL_SERVER_ERROR:
        p_memCopy(buff, ESP8266_FIREBASE_STR_62);
        return;
    case _HTTP_CODE_BAD_GATEWAY:
        p_memCopy(buff, ESP8266_FIREBASE_STR_63);
        return;
    case _HTTP_CODE_SERVICE_UNAVAILABLE:
        p_memCopy(buff, ESP8266_FIREBASE_STR_64);
        return;
    case _HTTP_CODE_GATEWAY_TIMEOUT:
        p_memCopy(buff, ESP8266_FIREBASE_STR_65);
        return;
    case _HTTP_CODE_HTTP_VERSION_NOT_SUPPORTED:
        p_memCopy(buff, ESP8266_FIREBASE_STR_66);
        return;
    case _HTTP_CODE_NETWORK_AUTHENTICATION_REQUIRED:
        p_memCopy(buff, ESP8266_FIREBASE_STR_67);
        return;
    case _HTTP_CODE_PRECONDITION_FAILED:
        p_memCopy(buff, ESP8266_FIREBASE_STR_152);
        return;
    case HTTPC_ERROR_READ_TIMEOUT:
        p_memCopy(buff, ESP8266_FIREBASE_STR_69);
        return;
    case FIREBASE_ERROR_DATA_TYPE_MISMATCH:
        p_memCopy(buff, ESP8266_FIREBASE_STR_70);
        return;
    case FIREBASE_ERROR_PATH_NOT_EXIST:
        p_memCopy(buff, ESP8266_FIREBASE_STR_71);
        return;
    case HTTPC_ERROR_CONNECTION_INUSED:
        p_memCopy(buff, ESP8266_FIREBASE_STR_94);
        return;
    case FIREBASE_ERROR_BUFFER_OVERFLOW:
        p_memCopy(buff, ESP8266_FIREBASE_STR_68);
        return;
    case HTTPC_NO_FCM_TOPIC_PROVIDED:
        p_memCopy(buff, ESP8266_FIREBASE_STR_144);
        return;
    case HTTPC_NO_FCM_DEVICE_TOKEN_PROVIDED:
        p_memCopy(buff, ESP8266_FIREBASE_STR_145);
        return;
    case HTTPC_NO_FCM_SERVER_KEY_PROVIDED:
        p_memCopy(buff, ESP8266_FIREBASE_STR_146);
        return;
    case HTTPC_NO_FCM_INDEX_NOT_FOUND_IN_DEVICE_TOKEN_PROVIDED:
        p_memCopy(buff, ESP8266_FIREBASE_STR_147);
        return;

    default:
        return;
    }
}


void FirebaseESP8266::p_memCopy(std::string &buff, const char *p, bool empty) //4434
{
    if (empty)
        buff.clear();
    size_t len = strlen_P(p) + 10;
    char *b = new char[len];
    memset(b, 0, len);
    strcpy(b, p);
    buff += b;
    delete[] b;
} //p_memCopy


bool FirebaseESP8266::sdTest()
{
    File file;
    std::string filepath = "";
    p_memCopy(filepath, ESP8266_FIREBASE_STR_73, true);

    SD.begin(SD_CS_PIN);
    _sdPin = SD_CS_PIN;

    file = SD.open(filepath.c_str(), FILE_WRITE);
    if (!file)
        goto EXIT_5;

    if (!file.write(32))
        goto EXIT_5;
    file.close();

    file = SD.open(filepath.c_str());
    if (!file)
        goto EXIT_5;

    while (file.available())
    {
        if (file.read() != 32)
            goto EXIT_5;
    }
    file.close();

    SD.remove(filepath.c_str());

    std::string().swap(filepath);

    return true;

EXIT_5:
    std::string().swap(filepath);
    return false;
} //sdTest


//**********************************************************
//FirebaseESP8266.h
static const char ESP8266_FIREBASE_STR_1[] PROGMEM = "/";
static const char ESP8266_FIREBASE_STR_2[] PROGMEM = ".json?auth=";
static const char ESP8266_FIREBASE_STR_3[] PROGMEM = "\"";
static const char ESP8266_FIREBASE_STR_4[] PROGMEM = ".";
static const char ESP8266_FIREBASE_STR_5[] PROGMEM = "HTTP/1.1 ";
static const char ESP8266_FIREBASE_STR_6[] PROGMEM = " ";
static const char ESP8266_FIREBASE_STR_7[] PROGMEM = ":";
static const char ESP8266_FIREBASE_STR_8[] PROGMEM = "Content-Type: ";
static const char ESP8266_FIREBASE_STR_9[] PROGMEM = "text/event-stream";
static const char ESP8266_FIREBASE_STR_10[] PROGMEM = "Connection: ";
static const char ESP8266_FIREBASE_STR_11[] PROGMEM = "keep-alive";
static const char ESP8266_FIREBASE_STR_12[] PROGMEM = "Content-Length: ";
static const char ESP8266_FIREBASE_STR_13[] PROGMEM = "event: ";
static const char ESP8266_FIREBASE_STR_14[] PROGMEM = "data: ";
static const char ESP8266_FIREBASE_STR_15[] PROGMEM = "put";
static const char ESP8266_FIREBASE_STR_16[] PROGMEM = "patch";
static const char ESP8266_FIREBASE_STR_17[] PROGMEM = "\"path\":\"";
static const char ESP8266_FIREBASE_STR_18[] PROGMEM = "\"data\":";
static const char ESP8266_FIREBASE_STR_19[] PROGMEM = "null";
static const char ESP8266_FIREBASE_STR_20[] PROGMEM = "{\"name\":\"";
static const char ESP8266_FIREBASE_STR_21[] PROGMEM = "\r\n";
static const char ESP8266_FIREBASE_STR_22[] PROGMEM = "GET ";
static const char ESP8266_FIREBASE_STR_23[] PROGMEM = "PUT";
static const char ESP8266_FIREBASE_STR_24[] PROGMEM = "POST";
static const char ESP8266_FIREBASE_STR_25[] PROGMEM = "GET";
static const char ESP8266_FIREBASE_STR_26[] PROGMEM = "PATCH";
static const char ESP8266_FIREBASE_STR_27[] PROGMEM = "DELETE";
static const char ESP8266_FIREBASE_STR_28[] PROGMEM = "&download=";
static const char ESP8266_FIREBASE_STR_29[] PROGMEM = "&print=silent";
static const char ESP8266_FIREBASE_STR_30[] PROGMEM = " HTTP/1.1\r\n";
static const char ESP8266_FIREBASE_STR_31[] PROGMEM = "Host: ";
static const char ESP8266_FIREBASE_STR_32[] PROGMEM = "User-Agent: ESP8266\r\n";
static const char ESP8266_FIREBASE_STR_33[] PROGMEM = "X-Firebase-Decoding: 1\r\n";
static const char ESP8266_FIREBASE_STR_34[] PROGMEM = "Connection: close\r\n";
static const char ESP8266_FIREBASE_STR_35[] PROGMEM = "Accept: text/event-stream\r\n";
static const char ESP8266_FIREBASE_STR_36[] PROGMEM = "Connection: keep-alive\r\n";
static const char ESP8266_FIREBASE_STR_37[] PROGMEM = "Keep-Alive: timeout=30, max=100\r\n";
static const char ESP8266_FIREBASE_STR_38[] PROGMEM = "Accept-Encoding: identity;q=1,chunked;q=0.1,*;q=0\r\n";
static const char ESP8266_FIREBASE_STR_39[] PROGMEM = "connection refused";
static const char ESP8266_FIREBASE_STR_40[] PROGMEM = "send header failed";
static const char ESP8266_FIREBASE_STR_41[] PROGMEM = "send payload failed";
static const char ESP8266_FIREBASE_STR_42[] PROGMEM = "not connected";
static const char ESP8266_FIREBASE_STR_43[] PROGMEM = "connection lost";
static const char ESP8266_FIREBASE_STR_44[] PROGMEM = "no HTTP server";
static const char ESP8266_FIREBASE_STR_45[] PROGMEM = "bad request";
static const char ESP8266_FIREBASE_STR_46[] PROGMEM = "non-authoriative information";
static const char ESP8266_FIREBASE_STR_47[] PROGMEM = "no content";
static const char ESP8266_FIREBASE_STR_48[] PROGMEM = "moved permanently";
static const char ESP8266_FIREBASE_STR_49[] PROGMEM = "use proxy";
static const char ESP8266_FIREBASE_STR_50[] PROGMEM = "temporary redirect";
static const char ESP8266_FIREBASE_STR_51[] PROGMEM = "permanent redirect";
static const char ESP8266_FIREBASE_STR_52[] PROGMEM = "unauthorized";
static const char ESP8266_FIREBASE_STR_53[] PROGMEM = "forbidden";
static const char ESP8266_FIREBASE_STR_54[] PROGMEM = "not found";
static const char ESP8266_FIREBASE_STR_55[] PROGMEM = "method not allow";
static const char ESP8266_FIREBASE_STR_56[] PROGMEM = "not acceptable";
static const char ESP8266_FIREBASE_STR_57[] PROGMEM = "proxy authentication required";
static const char ESP8266_FIREBASE_STR_58[] PROGMEM = "request timeout";
static const char ESP8266_FIREBASE_STR_59[] PROGMEM = "length required";
static const char ESP8266_FIREBASE_STR_60[] PROGMEM = "too many requests";
static const char ESP8266_FIREBASE_STR_61[] PROGMEM = "request header fields too larg";
static const char ESP8266_FIREBASE_STR_62[] PROGMEM = "internal server error";
static const char ESP8266_FIREBASE_STR_63[] PROGMEM = "bad gateway";
static const char ESP8266_FIREBASE_STR_64[] PROGMEM = "service unavailable";
static const char ESP8266_FIREBASE_STR_65[] PROGMEM = "gateway timeout";
static const char ESP8266_FIREBASE_STR_66[] PROGMEM = "http version not support";
static const char ESP8266_FIREBASE_STR_67[] PROGMEM = "network authentication required";
static const char ESP8266_FIREBASE_STR_68[] PROGMEM = "data buffer overflow";
static const char ESP8266_FIREBASE_STR_69[] PROGMEM = "read Timeout";
static const char ESP8266_FIREBASE_STR_70[] PROGMEM = "data type mismatch";
static const char ESP8266_FIREBASE_STR_71[] PROGMEM = "path not exist";
static const char ESP8266_FIREBASE_STR_72[] PROGMEM = "task";
static const char ESP8266_FIREBASE_STR_73[] PROGMEM = "/esp.32";
static const char ESP8266_FIREBASE_STR_74[] PROGMEM = "json";
static const char ESP8266_FIREBASE_STR_75[] PROGMEM = "string";
static const char ESP8266_FIREBASE_STR_76[] PROGMEM = "float";
static const char ESP8266_FIREBASE_STR_77[] PROGMEM = "int";
static const char ESP8266_FIREBASE_STR_78[] PROGMEM = "null";
static const char ESP8266_FIREBASE_STR_79[] PROGMEM = ";";
static const char ESP8266_FIREBASE_STR_80[] PROGMEM = "Content-Disposition: ";
static const char ESP8266_FIREBASE_STR_81[] PROGMEM = "application/octet-stream";
static const char ESP8266_FIREBASE_STR_82[] PROGMEM = "attachment";
static const char ESP8266_FIREBASE_STR_83[] PROGMEM = "Backup file was not exist";
static const char ESP8266_FIREBASE_STR_84[] PROGMEM = "SD card is in used";
static const char ESP8266_FIREBASE_STR_85[] PROGMEM = "SD card was not available";
static const char ESP8266_FIREBASE_STR_86[] PROGMEM = "Could not read/write backup file";
static const char ESP8266_FIREBASE_STR_87[] PROGMEM = "Transmission error, ";
static const char ESP8266_FIREBASE_STR_88[] PROGMEM = "Node path is not exist";
static const char ESP8266_FIREBASE_STR_89[] PROGMEM = ".json";
static const char ESP8266_FIREBASE_STR_90[] PROGMEM = "/root.json";
static const char ESP8266_FIREBASE_STR_91[] PROGMEM = "blob";
static const char ESP8266_FIREBASE_STR_92[] PROGMEM = "\"blob,base64,";
static const char ESP8266_FIREBASE_STR_93[] PROGMEM = "\"file,base64,";
static const char ESP8266_FIREBASE_STR_94[] PROGMEM = "http connection was used by other processes";
static const char ESP8266_FIREBASE_STR_95[] PROGMEM = "Location: ";
static const char ESP8266_FIREBASE_STR_96[] PROGMEM = "&orderBy=";
static const char ESP8266_FIREBASE_STR_97[] PROGMEM = "&limitToFirst=";
static const char ESP8266_FIREBASE_STR_98[] PROGMEM = "&limitToLast=";
static const char ESP8266_FIREBASE_STR_99[] PROGMEM = "&startAt=";
static const char ESP8266_FIREBASE_STR_100[] PROGMEM = "&endAt=";
static const char ESP8266_FIREBASE_STR_101[] PROGMEM = "&equalTo=";
static const char ESP8266_FIREBASE_STR_102[] PROGMEM = "\"error\" : ";
static const char ESP8266_FIREBASE_STR_103[] PROGMEM = "/.settings/rules";
static const char ESP8266_FIREBASE_STR_104[] PROGMEM = "{\"status\":\"ok\"}";
static const char ESP8266_FIREBASE_STR_105[] PROGMEM = "boolean";
static const char ESP8266_FIREBASE_STR_106[] PROGMEM = "false";
static const char ESP8266_FIREBASE_STR_107[] PROGMEM = "true";
static const char ESP8266_FIREBASE_STR_108[] PROGMEM = "double";
static const char ESP8266_FIREBASE_STR_109[] PROGMEM = "cancel";
static const char ESP8266_FIREBASE_STR_110[] PROGMEM = "auth_revoked";
static const char ESP8266_FIREBASE_STR_111[] PROGMEM = "http://";
static const char ESP8266_FIREBASE_STR_112[] PROGMEM = "https://";

static const char ESP8266_FIREBASE_STR_120[] PROGMEM = "fcm.googleapis.com";
static const char ESP8266_FIREBASE_STR_121[] PROGMEM = "/fcm/send";
static const char ESP8266_FIREBASE_STR_122[] PROGMEM = "\"notification\":{";
static const char ESP8266_FIREBASE_STR_123[] PROGMEM = "\"title\":\"";
static const char ESP8266_FIREBASE_STR_124[] PROGMEM = "\"body\":\"";
static const char ESP8266_FIREBASE_STR_125[] PROGMEM = "\"icon\":\"";
static const char ESP8266_FIREBASE_STR_126[] PROGMEM = "\"click_action\":\"";
static const char ESP8266_FIREBASE_STR_127[] PROGMEM = "}";
static const char ESP8266_FIREBASE_STR_128[] PROGMEM = "\"to\":\"";
static const char ESP8266_FIREBASE_STR_129[] PROGMEM = "application/json";
static const char ESP8266_FIREBASE_STR_130[] PROGMEM = "\"registration_ids\":[";
static const char ESP8266_FIREBASE_STR_131[] PROGMEM = "Authorization: key=";
static const char ESP8266_FIREBASE_STR_132[] PROGMEM = ",";
static const char ESP8266_FIREBASE_STR_133[] PROGMEM = "]";
static const char ESP8266_FIREBASE_STR_134[] PROGMEM = "/topics/";
static const char ESP8266_FIREBASE_STR_135[] PROGMEM = "\"data\":";
static const char ESP8266_FIREBASE_STR_136[] PROGMEM = "\"priority\":\"";
static const char ESP8266_FIREBASE_STR_137[] PROGMEM = "\"time_to_live\":";
static const char ESP8266_FIREBASE_STR_138[] PROGMEM = "\"collapse_key\":\"";
static const char ESP8266_FIREBASE_STR_139[] PROGMEM = "\"multicast_id\":";
static const char ESP8266_FIREBASE_STR_140[] PROGMEM = "\"success\":";
static const char ESP8266_FIREBASE_STR_141[] PROGMEM = "\"failure\":";
static const char ESP8266_FIREBASE_STR_142[] PROGMEM = "\"canonical_ids\":";
static const char ESP8266_FIREBASE_STR_143[] PROGMEM = "\"results\":";
static const char ESP8266_FIREBASE_STR_144[] PROGMEM = "No topic provided";
static const char ESP8266_FIREBASE_STR_145[] PROGMEM = "No device token provided";
static const char ESP8266_FIREBASE_STR_146[] PROGMEM = "No server key provided";
static const char ESP8266_FIREBASE_STR_147[] PROGMEM = "The index of recipient device registered token not found";
static const char ESP8266_FIREBASE_STR_148[] PROGMEM = "X-Firebase-ETag: true\r\n";
static const char ESP8266_FIREBASE_STR_149[] PROGMEM = "if-match: ";
static const char ESP8266_FIREBASE_STR_150[] PROGMEM = "ETag: ";
static const char ESP8266_FIREBASE_STR_151[] PROGMEM = "null_etag";
static const char ESP8266_FIREBASE_STR_152[] PROGMEM = "Precondition Failed (ETag is not match)";
static const char ESP8266_FIREBASE_STR_153[] PROGMEM = "X-HTTP-Method-Override: ";
static const char ESP8266_FIREBASE_STR_154[] PROGMEM = "{\".sv\": \"timestamp\"}";
static const char ESP8266_FIREBASE_STR_155[] PROGMEM = "&shallow=true";
static const char ESP8266_FIREBASE_STR_156[] PROGMEM = "/.priority";
static const char ESP8266_FIREBASE_STR_157[] PROGMEM = ",\".priority\":";
static const char ESP8266_FIREBASE_STR_158[] PROGMEM = "&timeout=";
static const char ESP8266_FIREBASE_STR_159[] PROGMEM = "ms";
static const char ESP8266_FIREBASE_STR_160[] PROGMEM = "&writeSizeLimit=";
static const char ESP8266_FIREBASE_STR_161[] PROGMEM = "{\".value\":";
static const char ESP8266_FIREBASE_STR_162[] PROGMEM = "&format=export";
static const char ESP8266_FIREBASE_STR_163[] PROGMEM = "Flash memory was not ready";

static const char ESP8266_FIREBASE_STR_164[] PROGMEM = "get";
static const char ESP8266_FIREBASE_STR_165[] PROGMEM = "set";
static const char ESP8266_FIREBASE_STR_166[] PROGMEM = "push";
static const char ESP8266_FIREBASE_STR_167[] PROGMEM = "update";
static const char ESP8266_FIREBASE_STR_168[] PROGMEM = "delete";

static const char ESP8266_FIREBASE_STR_169[] PROGMEM = "{";
static const char ESP8266_FIREBASE_STR_170[] PROGMEM = "%d";
static const char ESP8266_FIREBASE_STR_171[] PROGMEM = "%f";
static const char ESP8266_FIREBASE_STR_172[] PROGMEM = "[";
static const char ESP8266_FIREBASE_STR_173[] PROGMEM = "]";

static const unsigned char ESP8266_FIREBASE_base64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

class FirebaseData;
class StreamData;
class QueueInfo;
class FirebaseESP8266;
class FCMObject;

