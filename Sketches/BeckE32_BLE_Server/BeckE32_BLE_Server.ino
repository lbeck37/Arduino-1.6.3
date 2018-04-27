const char szSketchName[]  = "BeckE32_BLE_Server.ino";
const char szFileDate[]    = "Apr 27, 2018-c";
#include <BeckE32_BLE_ServerLib.h>

void setup() {
  Serial.begin(115200);
  Serial << endl << endl<< endl<< endl;
  Serial << "setup(): Begin " << szSketchName << ", " << szFileDate << endl;

  //Slow down CPU since HX711 needs to work with ESP32, verify BTE works
  Serial.println("setup(): Call rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M)");
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  pinMode(LED, OUTPUT);

  //Create the  Device with a name
  //BLEDevice::init("BeckE32 BLE Server"); // Give it a name
  BLEDevice::init("RightPedal_BeckE32");

  // Create the Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a Characteristic for Notify
  pNotifyCharact = pService->createCharacteristic(NOTIFY_CHARACT_UUID,
                                                  BLECharacteristic::PROPERTY_NOTIFY);
  //Add Descriptor to Characteristic for Notify
  pNotifyCharact->addDescriptor(new BLE2902());

  //Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting for a client connection to notify...");
} //setup


void loop() {
  //static int  sCount= 1;
  char szNotifyString[16]; // make sure this is big enough
  if (deviceConnected){
    dNotifyValue += 2.0;
    //Convert the value to a string:
    dtostrf(dNotifyValue, 1, 4, szNotifyString);  // float_val, min_width, digits_after_decimal, char_buffer
    //pCharacteristic->setValue(&txValue, 1); // To send the integer value
    //pCharacteristic->setValue("Hello!");    // Sending a test message
    pNotifyCharact->setValue(szNotifyString);
    pNotifyCharact->notify();                 //Send the value to the client
    Serial << "Server: loop(): Sent Notify Value= " << szNotifyString << endl;
  } //if (deviceConnected)
  //delay(1000);
  //delay(300);	//~One loop per second with 80MHz CPU
  delay(10);    //32 samples/sec with 80MHz CPU to Android app
  return;
} //Loop
//Last line.
