/**
Talking Plush Toy
@author Andromeda Kepecs

Parts:
- Arduino Pro Mini
- 5V Adafruit Micro-SD Breakout Board
- 32 GB Micro SD Card
- 8 ohm 0.25 Watt speaker
- Push button

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
- From Arduino to Speaker:
  - 9 to +
  - GND to GND
- Buttons connected to 10k ohm resistors in series. Connected to digital pins 3 and 4
- Electret Microphone
 - See Electret Microphone with Arduino example circuits. Connected to A0
- LEDs connected to digital pins 7 and 8


TODO Force sensing and conditional sound playing
TODO microphone recording, storing to SD and playback
**/

#include "SD.h"
#include "SPI.h"
#include "TMRpcm.h"

// Digital
#define BUTTON_PIN_RECORD_HIGH 2
#define BUTTON_PIN_RECORD 3
#define BUTTON_PIN_PLAY 4
#define BUTTON_PIN_MUSIC 5
#define RED_LED_PIN 7
#define YELLOW_LED_PIN 8
#define SPEAKER_PIN 9
#define CHIP_SELECT_PIN 10

// Analog
const int ELECTRET_PIN = A0;

#define BAUD_RATE 19200 // View monitor at 9600 baud

const int VOLUME = 5;
const int FREQUENCY = 16000;
const int NUM_TRACKS = 4;

// Sd2Card card;
TMRpcm audio;

void setup() {
  pinMode(CHIP_SELECT_PIN, OUTPUT); // SS
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);

  audio.CSPin = CHIP_SELECT_PIN;
  audio.speakerPin = SPEAKER_PIN;
  audio.setVolume(VOLUME);

  Serial.begin(BAUD_RATE);

  Serial.println("Initializing SD card");
  if (!SD.begin(CHIP_SELECT_PIN)) {
    Serial.println("Initialization failed");
    digitalWrite(RED_LED_PIN, HIGH);
    delay(1000);
    digitalWrite(RED_LED_PIN, LOW);
    return;
  } 
  else {
    Serial.println("Wiring is correct and a card is present");
    digitalWrite(YELLOW_LED_PIN, HIGH);
    delay(1000);
    digitalWrite(YELLOW_LED_PIN, LOW);
    return;
  }
  
}

// Variables for recording and playback
int record_count = 1;
int play_count = 1;
bool started = false;
bool stopped = false;
bool play = false;
bool paused = false;

int record_high_count = 1;
bool record_high = false;

unsigned long record_length = 0;
unsigned long play_length = 0;

unsigned long rec_high_length = 0;

int track_count = 0;

void loop() { 

  if (digitalRead(BUTTON_PIN_RECORD_HIGH) == 1) {
    if (millis() - rec_high_length > 200) {
      record_high_count++;
      rec_low_length = millis();
      if (record_high_count % 2 == 0) {
        record_high = true;
        Serial.println(record_high);
      }
      else {
        record_high = false;
        Serial.println(record_high);
      }
    }
  }

  if (digitalRead(BUTTON_PIN_RECORD) == 1) {
    if (millis() - record_length > 200) {
      record_length = millis();
      record_count++;
      started = true;
    }
  }
  else if (digitalRead(BUTTON_PIN_PLAY) == 1) {
    if (millis() - play_length > 200) {
      play_length = millis();
      play_count++;
        play = true;
      }
  }

  if (record_count % 2 == 0 && started) {
    Serial.println("Recording");
    digitalWrite(RED_LED_PIN, HIGH);
    if (record_high) {
      audio.startRecording("MIC.WAV", FREQUENCY * 4, ELECTRET_PIN);
    }
    else {
      audio.startRecording("MIC.WAV", FREQUENCY, ELECTRET_PIN);
    }
   
    started = false;
    stopped = true;
  }
  else if (record_count % 2 != 0 && stopped) {
    Serial.println("Recording stopped");
    digitalWrite(RED_LED_PIN, LOW);
    audio.stopRecording("MIC.WAV");
    started = true;
    stopped = false;
  }
  else if (play_count % 2 == 0 && play) {
    Serial.println("Playing file");
    digitalWrite(YELLOW_LED_PIN, HIGH);
    audio.play("MIC.WAV");
    play = false;
    paused = true;
  }
  else if (play_count % 2 != 0 && paused) {
    Serial.println("Paused file");
    digitalWrite(YELLOW_LED_PIN, LOW);
    audio.pause();
    digitalWrite(SPEAKER_PIN, LOW); // Prevent excess speaker noise
    play = true;
    paused = false;
  }

  

 }