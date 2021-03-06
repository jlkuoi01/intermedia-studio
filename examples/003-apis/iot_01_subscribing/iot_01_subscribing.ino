// Adafruit IO Subscription Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the secrets.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "secrets.h"

/************************ Example Starts Here *******************************/

// set up the 'words' feed
AdafruitIO_Feed *words = io.feed("words");
AdafruitIO_Feed *messages = io.feed("messages");

#include <Adafruit_NeoPixel.h>
#define PIN            4
#define NUMPIXELS      12
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  // while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // start MQTT connection to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  words->onMessage(handleMessage);

  // wait for an MQTT connection
  // NOTE: when blending the HTTP and MQTT API, always use the mqttStatus
  // method to check on MQTT connection status specifically
  while(io.mqttStatus() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // Because Adafruit IO doesn't support the MQTT retain flag, we can use the
  // get() function to ask IO to resend the last value for this feed to just
  // this MQTT client after the io client is connected.
  words->get();

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  pixels.begin(); // This initializes the NeoPixel library.
  allColor(pixels.Color(10, 10, 10));
  delay(500);
  allColor(pixels.Color(0,0,0));
}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  // Because this sketch isn't publishing, we don't need
  // a delay() in the main program loop.

}

// this function is called whenever a 'words' message
// is received from Adafruit IO. it was attached to
// the words feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");
  Serial.println(data->value());

  messages->save("received a word on Arduino");
  String color = data->toString();
  if (color == "red") {
    allColor(pixels.Color(100, 0, 0));
  } else if (color == "green") { 
    allColor(pixels.Color(0, 100, 0));
  } else if (color == "blue") {
    allColor(pixels.Color(0, 0, 100));
  } else {
    allColor(pixels.Color(0, 0, 0));
  }

}

void allColor(uint32_t color) {
  for(int i=0;i<pixels.numPixels();i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, color); // Moderately bright green color.
    pixels.show();
    delay(25);
  }
}

