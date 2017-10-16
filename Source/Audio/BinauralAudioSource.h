//
//  BinauralAudioSource.hpp
//  Binaural
//
//  Created by Alex on 19/7/16.
//
//

#ifndef BinauralAudioSource_h
#define BinauralAudioSource_h

#include "../JuceLibraryCode/JuceHeader.h"
#include "HRTFContainer.h"
#include "FFTConvolver/FFTConvolver.h"
#include "../Definitions.h"


#include <atomic>
#include <math.h>

class BinauralAudioSource  : public AudioIODeviceCallback
{
public:
    BinauralAudioSource();
    ~BinauralAudioSource();
    
    void audioDeviceIOCallback(const float** inputChannelData,
                               int totalNumInputChannels,
                               float** outputChannelData,
                               int totalNumOutputChannels,
                               int numSamples);
    void audioDeviceAboutToStart (AudioIODevice* device);
    void audioDeviceStopped();
    
    void setFile(File audioFile);
    void play();
    void setAzimuth(int azimuth);
    void setElevation(int elevation);
    void updateHRIR(double azimuth,double elevation );
    void bypassAudio(bool isBypassed);
    void setDistance(int distance);

private:
    
    
    AudioSourcePlayer audioSourcePlayer;
    AudioTransportSource transportSource;
    
    AudioFormatReaderSource* currentAudioFileSource;
    File currentFile;
    
    std::atomic<int> azimuth;
    std::atomic<int> elevation;
    std::atomic<int> distance;
    
    /*----------------------------*/
    
    HRTFContainer hrtfContainer_;
    
    bool crossfading_;
    bool bypassed_;

    
    struct Crossover
    {
        IIRFilter loPass;
        IIRFilter hiPass;
        double f0 = 150.;
        double fs = 44100.;
        void set(double fs, double f0)
        {
            loPass.setCoefficients(IIRCoefficients::makeLowPass(fs, f0));
            hiPass.setCoefficients(IIRCoefficients::makeHighPass(fs, f0));
            this->fs = fs;
            this->f0 = f0;
        }
    } crossover_;
    FFTConvolver filters_[2];
    HrirBuffer currentHrir_;
    std::vector<float> loPassIn_;
    std::vector<float> hiPassIn_;
    std::vector<float> buffers_[2];
    
    float crossfadeRate;
    float wetPercent_;
    
    float *convolutionResult;
    
    float hasChanged=false;
    bool isBypassed=false;
    
    
    
    
    

    
};

#endif /* BinauralAudioSource_hpp */
