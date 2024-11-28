#include "config.h"

#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include "Voice.h"


int _inputValues[NUMBER_OF_INPUTS];
Voice _voices[NUMBER_OF_INPUTS];

AudioControlSGTL5000 _codec;
AudioMixer4 _mixer1;      // Mischer für die ersten 4 Stimmen
AudioMixer4 _mixer2;      // Mischer für die nächsten 4 Stimmen
AudioMixer4 _finalMixer;  // Endmischer für alle Stimmen
AudioOutputI2S _audioOut;
AudioConnection _mixerToOutputLeft(_finalMixer, 0, _audioOut, 0);
AudioConnection _mixerToOutputRight(_finalMixer, 0, _audioOut, 1);

// AudioConnection _osc0toFinalMixer0(_voices[0]._env, 0, _finalMixer, 0);  //TODO: add more mixers for >4 voices

AudioConnection _osc0toFinalMixer0(_voices[0]._amp, 0, _finalMixer, 0);  //TODO: add more mixers for >4 voices
AudioConnection _osc1toFinalMixer0(_voices[1]._amp, 0, _finalMixer, 1);  //TODO: add more mixers for >4 voices
AudioConnection _osc2toFinalMixer0(_voices[2]._amp, 0, _finalMixer, 2);  //TODO: add more mixers for >4 voices


void onNoteOn(byte channel, byte note, byte velocity) {
  Serial.println(note);

  switch (note) {
    case 60:
      {
        _voices[0].noteOn(note, velocity);
        break;
      }
    case 61:
      {
        _voices[1].noteOn(note, velocity);
        break;
      }
    case 62:
      {
        _voices[2].noteOn(note, velocity);
        break;
      }
  }
}
void onNoteOff(byte channel, byte note, byte velocity) {
  switch (note) {
    case 60:
      {
        _voices[0].noteOff();
        break;
      }
    case 61:
      {
        _voices[1].noteOff();
        break;
      }
    case 62:
      {
        _voices[2].noteOff();
        break;
      }
  }
}

void setup() {
  //  setup serial
  Serial.begin(115200);
  //  setup inputs
  for (auto i = 0; i < NUMBER_OF_INPUTS; i++) {
    pinMode(INPUT_PINS[i], INPUT_PULLUP);
  }
  // midi
  Serial.println("setup midi engine");
  usbMIDI.setHandleNoteOff(onNoteOff);
  usbMIDI.setHandleNoteOn(onNoteOn);

  //  setup audio
  AudioMemory(15);
  AudioNoInterrupts();
  _codec.enable();
  _codec.volume(0.45);

  _finalMixer.gain(0, 0.5);
  _finalMixer.gain(1, 0.5);
}

void loop() {
  //  read inputs
  int inputValues[NUMBER_OF_INPUTS];
  // for (auto i = 0; i < NUMBER_OF_INPUTS; i++) {
  //   inputValues[i] = digitalRead(INPUT_PINS[i]);


  //   //    if collision detected, but not at previous frame
  //   if (inputValues[i] && _inputValues[i] != inputValues[i]) {
  //     Serial.print("trigger note on: ");
  //     Serial.println(_notes[i]);
  //     _voices[i].noteOn(_notes[i], 100);
  //   }

  //   //    if no collision detected, but there was a collision at previous frame
  //   if (!inputValues[i] && _inputValues[i] != inputValues[i]) {
  //     Serial.print("trigger note off: ");
  //     Serial.println(_notes[i]);
  //     _voices[i].noteOff();
  //   }
  // }
  usbMIDI.read();
}
