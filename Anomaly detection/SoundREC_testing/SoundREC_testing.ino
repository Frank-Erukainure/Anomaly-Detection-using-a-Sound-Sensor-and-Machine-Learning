
#include <arduinoFFT.h>
#include "Window.h"
#include "model.h"

#define MIC A0
#define NUM_SAMPLES 31
#define SAMPLING_FREQUENCY 104
#define INTERVAL 5
#define SOUND_THRESHOLD 3

Window window;
Eloquent::ML::Port::RandomForest clf;
//Eloquent::ML::Port::SVM clf;

//unsigned int samplingPeriod;
//unsigned long microSeconds;
//
//int32_t backgroundSound;
//double features[NUM_SAMPLES];
//arduinoFFT fft;

//No sound recorded

void setup() {
    Serial.begin(115200);
    pinMode(MIC, INPUT);
     calibrate();
}

void loop() {
  if (!soundDetected()) {
        //delay(10);
        return;
    }
    captureWord();
    print_features();
    //delay(1000);
    Serial.println(analogRead(MIC));
    //delay(INTERVAL);
}

int16_t readMic() {
    // this translated the analog value to a proper interval
    return  (analogRead(MIC) - 512) >> 2;
}

// Detect sound

float backgroundSound = 0;
void calibrate() {
    for (int i = 0; i < 200; i++)
        backgroundSound += readMic();

    backgroundSound /= 200;

    Serial.print("Background sound level is ");
    Serial.println(backgroundSound);
}
bool soundDetected() {
    return abs(readMic() - backgroundSound) >= SOUND_THRESHOLD;
}

// Start Recording
arduinoFFT fft;
float features[NUM_SAMPLES];
double featuresForFFT[NUM_SAMPLES];
void captureWord() {
    for (uint16_t i = 0; i < NUM_SAMPLES; i++) {
        features[i] = readMic();
        featuresForFFT[i] = readMic();
        //delay(INTERVAL);
    }
     fft.Windowing(featuresForFFT, NUM_SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);

    for (int i = 0; i < NUM_SAMPLES; i++)
        features[i] = featuresForFFT[i];
}

void print_features() {
        const uint16_t numfeatures = sizeof(features) / sizeof(float);

        for (int i = 0; i < numfeatures; i++) {
        Serial.print(features[i]);
        Serial.print(i == numfeatures - 1 ? 'n' : ',');
        
        float x_1sample[] = {features[i]};
        if (!window.transform(x_1sample))
          return;
        Serial.print("Predicted Status: ");
        Serial.println(clf.predictLabel(x_1sample));
        
        delay(300);

    }
}
