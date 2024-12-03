#pragma once
#include <Audio.h>
#define AMPLITUDE (0.2)
const float velocity2amplitude[127] = {
  0.01778, 0.01966, 0.02164, 0.02371, 0.02588, 0.02814, 0.03049, 0.03294, 0.03549, 0.03812,
  0.04086, 0.04369, 0.04661, 0.04963, 0.05274, 0.05594, 0.05924, 0.06264, 0.06613, 0.06972,
  0.07340, 0.07717, 0.08104, 0.08500, 0.08906, 0.09321, 0.09746, 0.10180, 0.10624, 0.11077,
  0.11539, 0.12011, 0.12493, 0.12984, 0.13484, 0.13994, 0.14513, 0.15042, 0.15581, 0.16128,
  0.16685, 0.17252, 0.17828, 0.18414, 0.19009, 0.19613, 0.20227, 0.20851, 0.21484, 0.22126,
  0.22778, 0.23439, 0.24110, 0.24790, 0.25480, 0.26179, 0.26887, 0.27605, 0.28333, 0.29070,
  0.29816, 0.30572, 0.31337, 0.32112, 0.32896, 0.33690, 0.34493, 0.35306, 0.36128, 0.36960,
  0.37801, 0.38651, 0.39511, 0.40381, 0.41260, 0.42148, 0.43046, 0.43953, 0.44870, 0.45796,
  0.46732, 0.47677, 0.48631, 0.49595, 0.50569, 0.51552, 0.52544, 0.53546, 0.54557, 0.55578,
  0.56609, 0.57648, 0.58697, 0.59756, 0.60824, 0.61902, 0.62989, 0.64085, 0.65191, 0.66307,
  0.67432, 0.68566, 0.69710, 0.70863, 0.72026, 0.73198, 0.74380, 0.75571, 0.76771, 0.77981,
  0.79201, 0.80430, 0.81668, 0.82916, 0.84174, 0.85440, 0.86717, 0.88003, 0.89298, 0.90602,
  0.91917, 0.93240, 0.94573, 0.95916, 0.97268, 0.98629, 1.00000
};

// from Pure Data x_acoustics.c
float mtof(float note) {
  if (note <= -1500) return (0);
  else if (note > 1499) return (mtof(1499));
  else return (8.17579891564 * exp(.0577622650 * note));
}

class Voice {
public:
  Voice()
    : _oscillatorToEnv(_oscillator, _env),
      _envToFilterLop(_env, _filterLop),
      _filterLopToFilterHip(_filterLop, _filterHip),
      _filterHipToAmp(_filterHip, _amp) {

    _env.attack(50);
    _env.decay(50);
    _env.release(250);
    _amp.gain(1);
    _filterLop.setLowpass(0, 440, 0.5);
    _filterHip.setHighpass(0, 220, 0.5);
  }

  void noteOn(int note, int velocity = 127) {
    AudioNoInterrupts();
    // TODO: what to do with velocity? we could use an a gain value from the table above and set the amps gain or somehow apply the velocity to the env parameters

    _oscillator.begin(1, mtof(note), WAVEFORM_SINE);
    _env.noteOn();
    AudioInterrupts();
  }
  void noteOff() {
    _env.noteOff();
    _oscillator.amplitude(0);
  }
  AudioSynthWaveform _oscillator;
  AudioEffectEnvelope _env;
  AudioAmplifier _amp;
  AudioConnection _oscillatorToEnv;
  AudioConnection _envToFilterLop;
  AudioConnection _filterLopToFilterHip;
  AudioConnection _filterHipToAmp;
  AudioFilterBiquad _filterLop;
  AudioFilterBiquad _filterHip;
};
