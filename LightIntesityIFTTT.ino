#include <BH1750.h>
#include <Wire.h>

#include <WiFiNINA.h>
#include "Secrets.h";

// SENSITIVE DATA - Secrets.h
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

WiFiClient client;

char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME   = "https://maker.ifttt.com/trigger/sunlight_hits_terrarium/with/key/bDG8MglfO1X49_dr0Vax1Y";
String queryString = "?value1=57";

BH1750 lightMeter;

void setup() {
  // initialize WiFi connection
  WiFi.begin(ssid, pass);

  Serial.begin(9600);

  // Initialise the I2C bus
  Wire.begin();

  while (!Serial);

  // connect to web server on port 80:
  if (client.connect(HOST_NAME, 80)) {
    // if connected:
    Serial.println("Connected to server");
  }
  else {// if not connected:
    Serial.println("connection failed");
  }
  lightMeter.begin();
  Serial.println("BH1750 Test begin");
}

void loop() {
  float lx = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lx);
  Serial.println(" lx");
  delay(1000);

  if (lx >= 55) {
    queryString = "?Light_Intensity=" + String(lx);
    // make a HTTP request:
    // send HTTP header
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header


    while (client.connected()) {
      if (client.available()) {
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }

    // the server's disconnected, stop the client:
    client.stop();
    Serial.println();
    Serial.println("disconnected");
  }
}