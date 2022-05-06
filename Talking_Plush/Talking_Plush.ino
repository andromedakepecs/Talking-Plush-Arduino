/**
Talking Plush Toy
@author Andromeda Kepecs

Parts:
- Arduino Pro Mini
- 5V Adafruit Micro-SD Breakout Board
- 32 GB Micro SD Card
- 8 ohm 0.25 Watt speaker

Library References:
- https://www.arduino.cc/en/reference/SD
- https://www.arduino.cc/en/reference/SPI 
- https://www.arduino.cc/reference/en/libraries/tmrpcm/ 

Hookup:
- From Arduino to CP2102 USB to TTL Module:
  - VCC to +5V
  - GND to GND
  - TXO to RXD
  - RXI to TXD
- From Arduino to Micro-SD Breakout:
  - 10 to CS
  - 11 to DI
  - 12 to DO
  - 13 to CLK
  - GND to GND
  - VCC to 5V
- From Arduino to Speaker: TODO speaker for some reason doesn't play sound
  - 9 to +
  - GND to GND
- Force Sensor
  - One end to 5V
  - Other end to analog in, 10k ohm resistor, then ground


TODO Force sensing and conditional sound playing
TODO microphone recording, storing to SD and playback
**/

#include "SD.h"
#include "SPI.h"
#include "TMRpcm.h"

#define CHIP_SELECT 8
#define SPEAKER_PIN 9
#define FORCE_SENSOR_PIN 0
#define BAUD_RATE 19200 // View monitor at 9600 baud

const int VOLUME = 6;

Sd2Card card;
TMRpcm tmrpcm;

void setup() {
  tmrpcm.speakerPin = SPEAKER_PIN;
  Serial.begin(BAUD_RATE);

  Serial.println("Initializing SD card");

  pinMode(10, OUTPUT);

  if (!card.init(SPI_HALF_SPEED, CHIP_SELECT)) {
    Serial.println("Initialization failed");
    return;
  } 
  else {
    Serial.println("Wiring is correct and a card is present");
  }

  tmrpcm.setVolume(VOLUME);
  tmrpcm.play("0000.wav");
  Serial.println(tmrpcm.isPlaying());
}

void loop() { 
  delay(1000);
  int analog_read_force = analogRead(FORCE_SENSOR_PIN);
  Serial.println(analog_read_force);
 }