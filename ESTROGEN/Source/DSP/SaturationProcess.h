/*
  ==============================================================================

    SaturationProcess.h
    Created: 22 Feb 2024 3:05:46pm
    Author:  audrey dawson

  ==============================================================================
*/

#pragma once

#include <math.h>
#include <cmath>
#include "CompressorProcess.h"

class SaturationProcess {
  
public:
    
    enum SaturationType {
        
        ARCTAN,
        EXP_SOFTCLIP,
        DIODE
        
    };
    
    
    void prepareToPlay(double sampleRate);
    
    float processSample(float x);
    float processSample(float &xL, float &xR);
    void process(float * buffer, int numSamples, int channel);
    
    void setDriveAmount(float drive);
    void setOutputTrim(float gain);
    
    
private:
    
    float Fs;

    float driveValue = 0.f;
    float outputTrim_dB = 0.f;
    
    const float _twoOverPi = 2.f/M_PI;
    
};
