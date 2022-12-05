#include <arduinoFFT.h>
#define MIC A0
#define INTERVAL 5
#define SOUND_THRESHOLD 3
#define NUM_SAMPLES 32

//No sound recorded

void setup() {
    Serial.begin(115200);
    pinMode(MIC, INPUT);
     calibrate();
}

void loop() {
  if (!soundDetected()) {
        delay(10);
        return;
    }
    captureWord();
    printFeatures();
    delay(1000);
    Serial.println(analogRead(MIC));
    delay(INTERVAL);
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
        delay(INTERVAL);
    }
     fft.Windowing(featuresForFFT, NUM_SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);

    for (int i = 0; i < NUM_SAMPLES; i++)
        features[i] = featuresForFFT[i];
}

void printFeatures() {
    const uint16_t numFeatures = sizeof(features) / sizeof(float);
    
    for (int i = 0; i < numFeatures; i++) {
        Serial.print(features[i]);
        Serial.print(i == numFeatures - 1 ? 'n' : ',');
    }
}

//arduinoFFT fft;
//
//void captureWord() {
//    for (uint16_t i = 0; i < NUM_SAMPLES; i++) {
//        featuresForFFT[i] = readMic();
//        delay(INTERVAL);
//    }
//
//    fft.Windowing(featuresForFFT, NUM_SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
//
//    for (int i = 0; i < NUM_SAMPLES; i++)
//        features[i] = featuresForFFT[i];
//}
