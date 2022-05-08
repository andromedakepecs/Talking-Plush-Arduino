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
  - Note: uncommented "#define DISABLE_SPEAKER2" in pcmConfig.h to play sound correctly

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
- Button connected to 10k ohm resistor. Connected to digital pin 2


TODO Force sensing and conditional sound playing
TODO microphone recording, storing to SD and playback
**/

#include "SD.h"
#include "SPI.h"
#include "TMRpcm.h"

// Digital
#define BUTTON_PIN 2
#define CHIP_SELECT 8
#define SPEAKER_PIN 9

// Analog
#define FORCE_SENSOR_PIN 0

#define BAUD_RATE 19200 // View monitor at 9600 baud

const int VOLUME = 6;

Sd2Card card;

TMRpcm tmrpcm;

void setup() {
  tmrpcm.speakerPin = SPEAKER_PIN;
  Serial.begin(BAUD_RATE);

  Serial.println("Initializing SD card");

  pinMode(10, OUTPUT); // SS
  pinMode(BUTTON_PIN, INPUT);

  if (!SD.begin(CHIP_SELECT)) {
    Serial.println("Initialization failed");
    return;
  } 
  else {
    Serial.println("Wiring is correct and a card is present");
  }

  // File my_file = SD.open("1.wav");
  // if (my_file) {
  //   while (my_file.available()) {
  //     Serial.write(my_file.read());
  //   }

  //   my_file.close();
  // }
  // else {
  //   Serial.println("Error");
  // }

  tmrpcm.setVolume(VOLUME);
  tmrpcm.play("1.wav");
  delay(3000);
  tmrpcm.play("2.wav");
  delay(3000);
  tmrpcm.play("3.wav");
  delay(3000);
  tmrpcm.play("4.wav");
  delay(3000);
  tmrpcm.play("5.wav");
  delay(3000);
  tmrpcm.play("6.wav");
  delay(3000);
  Serial.println(tmrpcm.isPlaying());

  
}

void loop() { 
  delay(1000);
  Serial.println();

  int button_state = digitalRead(BUTTON_PIN);
  if (button_state == HIGH) {
    Serial.println("Button pressed");
  }
  else {
    Serial.println("Button not pressed");
  }

  int analog_force = analogRead(FORCE_SENSOR_PIN);
  Serial.println(analog_force);
  // TODO play different sounds based on amount of pressure
  if (analog_force < 50) {
    Serial.println("Do nothing");
  }
  else if (analog_force < 500) {
    Serial.println("React to light touch");
  }
  else if (analog_force < 900) {
    Serial.println("React to stronger touch");
  }
  else {
    Serial.println("Ow");
  }

 }