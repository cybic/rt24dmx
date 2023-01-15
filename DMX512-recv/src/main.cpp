#include <Arduino.h>

#include <DMXSerial.h>

// nRF24 deps
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

struct DMXFrame {
  uint16_t address;
  uint8_t  value;
};

RF24 radio(7, 8); // CE, CSN

// Radio TX channel address
const byte address[6] = "01001";

void setup () {
  //DMXSerial.init(DMXReceiver);
  DMXSerial.init(DMXController);
  //Serial.begin(0);

  //while(!Serial) {}

  //Serial.println("OK");

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_2MBPS);
  //radio.enableDynamicPayloads();
  //radio.disableAckPayload();
  //radio.setAutoAck(false);
  radio.startListening();
}

static constexpr const uint16_t channels = 512;
DMXFrame frame;

void loop() {
  while ( radio.available()) {

    radio.read(&frame, sizeof(frame));

    /*
    Serial.print(" ");
    Serial.print(frame.address);
    Serial.print(" ");
    Serial.println(frame.value);
    */

    DMXSerial.write(frame.address , frame.value);

  }
}

