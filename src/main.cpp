#include <SoftwareSerial.h>

#define OPEN_SQUELCH false
#define ADC_REFERENCE REF_5V

#include "LibAPRS.h"
#include "KISS.h"

extern Afsk modem;
extern AX25Ctx AX25;

char incomingByte;

#ifdef TNC_CAR
SoftwareSerial bluetooth(10, 11); // RX, TX (car)
#pragma message("Building for /M TNC")
#else
SoftwareSerial bluetooth(12, 11); // RX, TX (tnc05, tnc06)
#pragma message("Building for /P TNC")
#endif

void aprs_msg_callback(struct AX25Ctx *ctx) {
  kiss_messageCallback(ctx);
}

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);

  APRS_init(ADC_REFERENCE, OPEN_SQUELCH);
  //APRS_setTail(1000U);
  kiss_init(&AX25, &modem, &Serial, &bluetooth);
  //Serial.println("started");
}

void loop() {
  APRS_poll();
  while (bluetooth.available() > 0) {
    incomingByte = bluetooth.read();
    //Serial.write(incomingByte);
    kiss_serialCallback(incomingByte);
  }
  while (Serial.available() > 0) {
    incomingByte = Serial.read();
    kiss_serialCallback(incomingByte);
  }
}
