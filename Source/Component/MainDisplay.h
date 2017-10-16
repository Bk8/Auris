#pragma once
#include <cmath>
#include "../JuceLibraryCode/JuceHeader.h"
#include "Util.h"
#include "images/head_top.h"
#include "images/source_icon.h"
#include "../Simulation/SoundSource.h"



class MainDisplay :
	public Component
{
public:
	MainDisplay();
	~MainDisplay();

	void paint(Graphics& g) override;
    void setSources(std::vector<ScopedPointer<SoundSource>>* sources);

private:
	void drawGridLines(Graphics& g);
    void drawSource(Graphics& g, int i);

	Image headImage_;
	Image sourceImage_;
    std::vector<ScopedPointer<SoundSource>>* sources;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainDisplay)
};
