#include "arduino_secrets.h"
/*
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/08347614-3a47-432f-9060-7b725f7eb974

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  float a_x;
  float a_y;
  float a_z;
  float temperature;
  bool green;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "DHT20.h"
DHT20 DHT;

#include "thingProperties.h"
#include <Arduino_LSM6DSOX.h>

// Oled, see https://github.com/olikraus/u8g2/issues/1850
#define U8X8_DO_NOT_SET_WIRE_CLOCK
#include "U8x8lib.h"  // OLED Display
U8X8_SSD1306_128X64_NONAME_HW_I2C Oled(/* reset=*/U8X8_PIN_NONE);

unsigned long last_calc = 0;  // last time we took the time
float humidity;               // not a cloud variable because free plan only supports
                              // up to five variables

void setup() {
  pinMode(LEDG, OUTPUT);

  // Initialize serial and wait for port to open:
  Serial.begin(115200);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }

  if (!DHT.begin()) {
    Serial.println("Failed to initialize I2C communication with DHT20");
    while (1)
      ;
  }
  Serial.println("DHT20 initialized");

  //initialize OLED
  // Oled.setBusClock(400000u);  // set bus clock to same speed as other i2c devices integrated on RP2040 Connect board
  if (!Oled.begin()) {
    Serial.println("Failed to initialize I2C communication with Oled display");
    while (1)
      ;
  }
  Oled.setFlipMode(true);
  Serial.println("Oled initialized");

  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection, true);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(4);
  ArduinoCloud.printDebugInfo();
  last_calc = millis();
}

void loop() {
  ArduinoCloud.update();

  // Your code here
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(a_x, a_y, a_z);
    Serial.print("a");
  } else {
    Serial.print(".");
  }

  // Alternative with internal temperature
  // if no DHT20 sensor is available
  /*if (IMU.temperatureAvailable())
    {
    IMU.readTemperatureFloat(temperature);
    }*/

  // read temperature every second
  if ((millis() - last_calc) > 1000) {
    int status = DHT.read();
    temperature = DHT.getTemperature();
    humidity = DHT.getHumidity();
    last_calc = millis();
    switch (status) {
      case DHT20_OK:
        oledDisplayTemperature(temperature, humidity);
        Serial.print("d");
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
}

void oledDisplayTemperature(float temperature, float humidity) {
  Oled.setFont(u8x8_font_amstrad_cpc_extended_r);
  // print temperature
  Oled.setCursor(0, 1);
  Oled.print("Temperatur: ");
  Oled.setCursor(3, 2);
  Oled.print(temperature);
  Oled.print(" C   ");  // The unit for  Celsius because        original arduino don't support speical symbols
  // // print humidity data
  Oled.setCursor(0, 3);
  Oled.print("Luftfeuchtigk.:");
  Oled.setCursor(3, 4);
  Oled.print(humidity);
  Oled.print(" %  ");

  Oled.refreshDisplay();  // Update the Display
}


/*
  Since Green is READ_WRITE variable, onGreenChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onGreenChange() {
  Serial.println(".");
  Serial.print("green: ");
  Serial.println(green);
  if (green) {
    digitalWrite(LEDG, HIGH);  //turn on RED
  } else {
    digitalWrite(LEDG, LOW);  //turn off RED
  }
}
