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

uint8_t buffer[512] = {0};

DMXFrame frame;

void setup () {
  DMXSerial.init(DMXReceiver);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_2MBPS);
  //radio.enableDynamicPayloads();
  //radio.disableAckPayload();
  //radio.setAutoAck(false);
  radio.stopListening();

  delay(100);
  frame.address = 0;
  frame.value = 0;
  radio.write(&frame, sizeof(frame));

}

static constexpr const uint16_t channels = 512;
static constexpr const uint16_t channels_per_line = 32;
static constexpr const uint16_t channels_per_group = 8;

void printByte(uint8_t b) {
}


void loop() {
  if (DMXSerial.dataUpdated()) {
    DMXSerial.resetUpdated();

    //uint8_t *buf = (uint8_t *)DMXSerial.getBuffer();

    /*
    for (uint16_t i = 0; i < channels; i++) {
      int c = i+1;
      DMXFrame frame;
      frame.address = c;
      frame.value = DMXSerial.read(c);

      radio.write(&frame, sizeof(frame));
      //radio.flush_tx();

      //delay(2);
      //radio.write(buf + i, 32);
    }
    */

    uint8_t *dmxbuf = DMXSerial.getBuffer();

    //bool dirty = false;

    for (uint16_t i = 0; i < channels; i++ ) {
      if ( *(dmxbuf + i) != *(buffer + i)) {
        frame.address = i;
        frame.value = dmxbuf[i];

        radio.write(&frame, sizeof(frame));
        buffer[i] = dmxbuf[i];
        //dirty = true;
      }
    }
  /*
    if ( dirty ) {
      memcpy(buffer, dmxbuf, 512);
    }
    */
  }
}