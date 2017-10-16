#pragma warning(push)
#pragma warning(disable: 4244)
#pragma warning(pop)

#include "HRTFContainer.h"



HRTFContainer::HRTFContainer()
	:hrir_(),
     sampleRate (44100),
     diffused (0)
{
}

HRTFContainer::~HRTFContainer()
{
}

void HRTFContainer::updateHRIR(double azimuth, double elevation)
{
    int nAzimuth = (int) azimuth;
    int nElevation = (int) elevation;
    int taps = mit_hrtf_availability (nAzimuth, nElevation, sampleRate, diffused);
    
    if (taps)
    {
        leftEar.setSize (1, taps);
        rightEar.setSize (1, taps);
        
        AudioBuffer<short> leftShort (1, taps);
        AudioBuffer<short> rightShort (1, taps);
        
        short* const writeLeft = leftShort.getWritePointer (0);
        short* const writeRight = rightShort.getWritePointer (0);
        
        taps = mit_hrtf_get (&nAzimuth, &nElevation, sampleRate, diffused, writeLeft, writeRight);
        
        float* const leftEarWrite = leftEar.getWritePointer (0);
        float* const rightEarWrite = rightEar.getWritePointer (0);
        
        const short* const readLeft = leftShort.getReadPointer (0);
        const short* const readRight = rightShort.getReadPointer (0);
        
        for (int n = 0; n < taps; ++n)
        {
            leftEarWrite[n] = readLeft[n] / 32768.0f;
            rightEarWrite[n] = readRight[n] / 32768.0f;
        }
        
        for (size_t i = 0; i < hrir_[0].size(); ++i)
        {
            hrir_[0][i] = leftEarWrite[i];
            hrir_[1][i] = rightEarWrite[i];
        }
    }
}

const HrirBuffer& HRTFContainer::hrir() const
{
	return hrir_;
}



