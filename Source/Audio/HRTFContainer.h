#pragma once
#include <array>
#include <vector>
#include <memory>
#include <map>
#include "../JuceLibraryCode/JuceHeader.h"
#include "mit-hrtf-lib/mit_hrtf_lib.h"




#define HRIR_LENGTH 128
using HrirBuffer = std::array < std::array<float, HRIR_LENGTH>, 2 >;

class HRTFContainer
{
public:
	HRTFContainer();
	~HRTFContainer();

	void updateHRIR(double azimuth, double elevation);
	const HrirBuffer& hrir() const;
    
    AudioSampleBuffer leftEar;
    AudioSampleBuffer rightEar;
    HrirBuffer hrir_;

private:

    
    unsigned int sampleRate;
    unsigned int diffused;
};
