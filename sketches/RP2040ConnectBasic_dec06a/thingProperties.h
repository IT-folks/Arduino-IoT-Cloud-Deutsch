// Code generated by Arduino IoT Cloud, DO NOT EDIT.

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char SSID[]     = SECRET_SSID;    // Network SSID (name)
const char PASS[]     = SECRET_OPTIONAL_PASS;    // Network password (use for WPA, or use as key for WEP)

void onGreenChange();

float a_x;
float a_y;
float a_z;
float temperature;
bool green;

void initProperties(){

  ArduinoCloud.addProperty(a_x, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(a_y, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(a_z, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(temperature, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(green, READWRITE, 1 * SECONDS, onGreenChange);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
