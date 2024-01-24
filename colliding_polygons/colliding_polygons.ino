#include "config.h"

#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include "Voice.h"

int _inputValues[NUMBER_OF_INPUTS];
Voice _voices[NUMBER_OF_INPUTS];


AudioMixer4 _mixer;
AudioOutputI2S _audioOut;
AudioConnection _mixer2Output(_mixer, 0, _audioOut, 0);


void setup() {
  //  setup serial
  Serial.begin(115200);
  //  setup inputs
  for (auto i = 0; i < NUMBER_OF_INPUTS; i++) {
    pinMode(INPUT_PINS[i], INPUT_PULLUP);
  }
  //  setup audio
  AudioMemory(15);
  AudioNoInterrupts();

  for (auto i = 0; i < NUMBER_OF_INPUTS; i++) {
    AudioConnection(_voices[i]._amp, 0,  _mixer, i);//TODO: add more mixers for >4 voices
    _mixer.gain(i, 0.25);//set each channel to 1/4
  }
}

void loop() {
  //  read inputs
  int inputValues[NUMBER_OF_INPUTS];
  for (auto i = 0; i < NUMBER_OF_INPUTS; i++) {
    inputValues[i] = digitalRead(INPUT_PINS[i]);

    //    if collision detected, but not at previous frame
    if (inputValues[i] && _inputValues[i] != inputValues[i]) {
      Serial.print("trigger note on: ");
      Serial.println(_notes[i]);
      _voices[i].noteOn(_notes[i], 100);
    }

    //    if no collision detected, but there was a collision at previous frame
    if (!inputValues[i] && _inputValues[i] != inputValues[i]) {
      Serial.print("trigger note off: ");
      Serial.println(_notes[i]);
      _voices[i].noteOff();
    }
  }
}
