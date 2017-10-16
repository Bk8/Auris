//
//  BinauralAudioSource.cpp
//  Binaural
//
//  Created by Alex on 19/7/16.
//
//

#include "BinauralAudioSource.h"

BinauralAudioSource::BinauralAudioSource()
{
   
    azimuth=0;
    elevation=0;
    
    
    currentAudioFileSource = NULL;
    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    
    
    
    
    // load HRIR
    //auto thisDir = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory();

    bypassed_ = false;
    crossfadeRate=0.2f;
    hrtfContainer_.updateHRIR(0, 0);
    crossfading_ = true;
    wetPercent_=100;
    

    

}

BinauralAudioSource::~BinauralAudioSource()
{
    
    if (transportSource.isPlaying())
        transportSource.stop();
    transportSource.setSource (NULL);
    
    if (currentAudioFileSource)
        deleteAndZero (currentAudioFileSource);
    
    audioSourcePlayer.setSource (NULL);
}

void BinauralAudioSource::audioDeviceIOCallback(const float** inputChannelData,
                                                int totalNumInputChannels,
                                                float** outputChannelData,
                                                int totalNumOutputChannels,
                                                int numSamples)
{
    // Create a buffer that points to outputChannelData
    AudioSampleBuffer buffer (outputChannelData, totalNumOutputChannels, numSamples);
    
    AudioSourceChannelInfo info;
    info.buffer = &buffer;
    info.startSample = 0;
    info.numSamples = numSamples;
    
    

    info.clearActiveBufferRegion();
    
    // Gets the audio buffer from AudioSource and stores it to outputBuffer
    // So, outputChannelData now stores the audio from the file transport stream
    if(!isBypassed)
        transportSource.getNextAudioBlock (info);
    
    // do audio processing here, for example gain is applied to audio samples
    
    if (transportSource.isPlaying())
    {
        
        //hrtfContainer_.updateHRIR(azimuth, elevation);
        if (bypassed_)
            return;
        
        hrtfContainer_.updateHRIR(azimuth.load(),elevation.load());
        
        if (crossfading_)
        {
            auto& targetHrir = hrtfContainer_.hrir();
            float diff[2], totalDiff = 0.f;
            
            // linear interpolation, sample by sample
            for (auto i = 0u; i < targetHrir[0].size(); ++i)
            {
                diff[0] = targetHrir[0][i] - currentHrir_[0][i];
                diff[1] = targetHrir[1][i] - currentHrir_[1][i];
                currentHrir_[0][i] += diff[0] * crossfadeRate;
                currentHrir_[1][i] += diff[1] * crossfadeRate;
                
                totalDiff += std::fabsf(diff[0]) + std::fabsf(diff[1]);
            }
            
            // update impule response
            filters_[0].setIR(currentHrir_[0].data());
            filters_[1].setIR(currentHrir_[1].data());
            
            if (totalDiff < 1.f)
                crossfading_ = false;
        }
        
        // get a pointer to the left channel data
        auto inL = buffer.getWritePointer(0);
        auto inR = buffer.getWritePointer(1);
        auto bufferLength = buffer.getNumSamples();
        
        // downmix to mono in case of a stereo input
        // by adding from the right channel to left channel
        /*if (getTotalNumInputChannels() == 2) //NEEEDS FIX
        {*/
            buffer.addFrom(0, 0, inR, bufferLength);
            buffer.applyGain(0.5f);
        
            if(distance<0.0288)
                distance=0.0288;
        
            //buffer.applyGain(20/(4*PI*pow(distance,2.0f)));
            float gain=100/(4*PI*pow(distance,2.0f));
        
            if(gain>1)
                gain=1;
            buffer.applyGain(gain);
        /*}*/
        
        // split the input signal into two bands, only freqs above crossover's f0
        // will be spatialized
        memcpy(loPassIn_.data(), inL, bufferLength * sizeof(float));
        memcpy(hiPassIn_.data(), inL, bufferLength * sizeof(float));
        crossover_.loPass.processSamples(loPassIn_.data(), bufferLength);
        crossover_.hiPass.processSamples(hiPassIn_.data(), bufferLength);
        
        
        
        // we need to copy the hi-pass input to buffers
        memcpy(buffers_[0].data(), hiPassIn_.data(), bufferLength * sizeof(float));
        memcpy(buffers_[1].data(), hiPassIn_.data(), bufferLength * sizeof(float));
        
        // actual hrir filtering
        filters_[0].process(buffers_[0].data(), convolutionResult, numSamples);
        
        for(int i=0;i<buffers_[0].size();i++)
            buffers_[0].data()[i]=convolutionResult[i];
        
        filters_[1].process(buffers_[1].data(), convolutionResult, numSamples);
        
        for(int i=0;i<buffers_[0].size();i++)
            buffers_[1].data()[i]=convolutionResult[i];
        
        
        // copy to output
        auto outL = inL;
        auto outR = buffer.getWritePointer(1);
        
        // fill stereo output
        const auto wet = wetPercent_ / 100;
        const auto dry = 1 - wet;
        float monoIn;
        for (int i = 0; i < bufferLength; i++)
        {
            monoIn = inL[i];
            outL[i] = wet * (loPassIn_[i] + buffers_[0][i]) + dry * monoIn;
            outR[i] = wet * (loPassIn_[i] + buffers_[1][i]) + dry * monoIn;
        }
    }
}


void BinauralAudioSource::audioDeviceAboutToStart (AudioIODevice* device)
{
    audioSourcePlayer.audioDeviceAboutToStart (device);
    int samplesPerBlock=device->getCurrentBufferSizeSamples();
    int sampleRate=device->getCurrentSampleRate();
    
    
    crossover_.set(sampleRate, crossover_.f0);
    loPassIn_.resize(samplesPerBlock);
    hiPassIn_.resize(samplesPerBlock);
    buffers_[0].resize(samplesPerBlock);
    buffers_[1].resize(samplesPerBlock);
    
    int bufferSize=device->getCurrentBufferSizeSamples();
    
    convolutionResult= new float[bufferSize];
    filters_[0].init(samplesPerBlock,currentHrir_[0].data(), HRIR_LENGTH);
    filters_[1].init(samplesPerBlock,currentHrir_[0].data(), HRIR_LENGTH);

}

void BinauralAudioSource::audioDeviceStopped()
{
    audioSourcePlayer.audioDeviceStopped();
}


void BinauralAudioSource::setFile(File audioFile)
{
    if (transportSource.isPlaying()) {
        transportSource.stop();
        transportSource.setSource (NULL);
    }
    
    if (currentAudioFileSource)
        deleteAndZero (currentAudioFileSource);
    
    // get a format manager and set it up with the basic types (wav and aiff).
    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    
    AudioFormatReader* reader = formatManager.createReaderFor (audioFile);
    
    if (reader != NULL) {
        currentFile = audioFile;
        
        currentAudioFileSource = new AudioFormatReaderSource (reader, true);
        
        // ..and plug it into our transport source
        transportSource.setSource(currentAudioFileSource);
        
        
    }
}

void BinauralAudioSource::play()
{
    transportSource.setPosition (0.0);
    transportSource.start();
    
    // check if audio file is set, or else it will crash
    if (currentAudioFileSource)
        currentAudioFileSource->setLooping(true);
}

void BinauralAudioSource::bypassAudio(bool isBypassed)
{
    this->isBypassed=isBypassed;
}

void BinauralAudioSource::setAzimuth(int azimuth)
{
    this->azimuth=azimuth;
}

void BinauralAudioSource::setElevation(int elevation)
{
    this->elevation=elevation;
}

void BinauralAudioSource::updateHRIR(double azimuth,double elevation )
{
    setAzimuth(azimuth);
    setElevation(elevation);
    crossfading_ = true;

}

void BinauralAudioSource::setDistance(int distance)
{
    this->distance=distance;
}



