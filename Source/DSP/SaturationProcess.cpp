/*
 ==============================================================================
 
 SaturationProcess.cpp
 Created: 22 Feb 2024 3:05:46pm
 Author:  audrey dawson
 
 ==============================================================================
 */

#include "SaturationProcess.h"

void SaturationProcess::prepareToPlay(double sampleRate) {
    
    if (Fs != (float) sampleRate) {
        Fs = (float) sampleRate;
    }
    
}

// –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

float SaturationProcess::processSample(float x) {
    
    y = x;
    y = convert_dB(y) + inputGainTrimVal;
    y = convert_lin(y);
    
    if (y != 0.0f) {
        
        // apply drive amount
        y *= driveValue;
        
        // saturate the signal
        // (2/pi) * atan(drive * x) // << THE EQUATION
        
        y = _twoOverPi * atan(y);
        
        // phase check to make sure output signal is the correct sign (+/-) based off of the input signal
        y *= (x/abs(x));
        
    }
    
    // return processed signal
    return y;
    
}

void SaturationProcess::process(float * buffer, int numSamples, int channel) {
    
    for (int n = 0; n < numSamples; n++) {
        
        float x = buffer[n];
        buffer[n] = processSample(x);
        
    }
    
}

float SaturationProcess::convert_dB(float sample_lin) {
    
    // takes a linear audio signal sample and converts it to the dB scale
    
    float sample_dB = 20.f * log10(abs(sample_lin) / 1.f);
    
    // no negative infinity values
    if (sample_dB < -96.f) {
        sample_dB = -96.f;
    }
    
    return sample_dB;
}

float SaturationProcess::convert_lin(float sample_dB) {
    
    // takes a dB audio signal sample and converts it to the linear scale for output
    
    float sample_lin = pow(10.0f, sample_dB * 0.05f);
    
    return sample_lin;
}

// –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

void SaturationProcess::setDriveAmount(float drive) {
    
    driveValue = drive;
    CompressorProcess compress;
    compress.setKneeWidth( (drive - 1) * 0.55f + 0.5f);
}

void SaturationProcess::setInputGain(float gainTrim) {
    
    inputGainTrimVal = gainTrim;
    
}


