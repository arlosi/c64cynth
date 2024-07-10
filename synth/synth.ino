#include "MIDIUSB.h"

// RV                   1  2  3  4  5  6  7  8  9  10 11  12
const int POT_PINS[] = {2, 3, 4, 5, 6, 1, 0, 7, 8, 9, 10, 11};
const int POT_COUNT = 12;

const int ANALOG_PIN = 10;
const int MUX_PINS[] = {2,3,4,5};
const int LED_PIN = 17;

void setup() {
  Serial.begin(115200);
  pinMode(ANALOG_PIN, INPUT);
  pinMode(MUX_PINS[0], OUTPUT);
  pinMode(MUX_PINS[1], OUTPUT);
  pinMode(MUX_PINS[2], OUTPUT);
  pinMode(MUX_PINS[3], OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void loop() {
  int values[POT_COUNT] = {};

  for (int pot = 0; pot < POT_COUNT; pot++) {
    int adj = POT_PINS[pot];
    digitalWrite(MUX_PINS[0], (adj >> 0) & 1);
    digitalWrite(MUX_PINS[1], (adj >> 1) & 1);
    digitalWrite(MUX_PINS[2], (adj >> 2) & 1);
    digitalWrite(MUX_PINS[3], (adj >> 3) & 1);
    delay(1);
    values[pot] = 1023 - analogRead(ANALOG_PIN);
  }
  
  byte filter_res     = values[7] / 8;
  byte filter_cutoff  = values[1] / 8;
  byte attack         = values[0] / 8;
  byte release        = values[6] / 8;
  
  byte trem_depth     = values[9] / 16;
  byte trem_speed     = values[10] / 16;
  byte pulse_width    = values[11] / 8;
  
  byte voice_mode     = values[2] / 8;
  byte mod_mode       = values[8] / 8;

  byte osc_all        = values[3] / 8;
  byte osc_2          = values[4] / 8;
  byte osc_3          = values[5] / 8;
  
  controlChange(0, 0, filter_res);
  controlChange(0, 1, filter_cutoff);
  controlChange(0, 4, attack);
  controlChange(0, 5, release);
  controlChange(0, 6, pulse_width);
  controlChange(0, 8, trem_depth);
  controlChange(0, 9, trem_speed);
  controlChange(0, 13, osc_all);
  controlChange(0, 14, osc_2);
  controlChange(0, 15, osc_3);
  controlChange(0, 2, voice_mode);
  controlChange(0, 3, mod_mode);
  
  MidiUSB.flush();
  delay(100);
}
