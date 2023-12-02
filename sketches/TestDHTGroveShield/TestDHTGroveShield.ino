#include "DHT20.h"
DHT20 DHT;

void setup() {
  Serial.begin(115200);
  delay(2000);
  if (!DHT.begin()) {
    Serial.println("Failed to initialize I2C communication with DHT20");
    while (1);
  }
  Serial.println("DHT20 initialized");

}

void loop() {
  delay(1000);
  int status = DHT.read();
   float temperature = DHT.getTemperature();
   Serial.println(temperature);
   switch (status)
    {
      case DHT20_OK:
        break;
      case DHT20_ERROR_CHECKSUM:
        Serial.print("Checksum error");
        break;
      case DHT20_ERROR_CONNECT:
        Serial.print("Connect error");
        break;
      case DHT20_MISSING_BYTES:
        Serial.print("Missing bytes");
        break;
      case DHT20_ERROR_BYTES_ALL_ZERO:
        Serial.print("All bytes read zero");
        break;
      case DHT20_ERROR_READ_TIMEOUT:
        Serial.print("Read time out");
        break;
      case DHT20_ERROR_LASTREAD:
        Serial.print("Error read too fast");
        break;
      default:
        Serial.print("Unknown error");
        break;
    }

}
