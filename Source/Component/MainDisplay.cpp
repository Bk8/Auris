#include "MainDisplay.h"
//#include "PluginEditor.h"


MainDisplay::MainDisplay()
{
	sourceImage_ = ImageFileFormat::loadFrom(source_icon_png, source_icon_png_size);
	headImage_ = ImageFileFormat::loadFrom(head_top_png, head_top_png_size);
}

MainDisplay::~MainDisplay()
{
}

void MainDisplay::paint(Graphics& g)
{
	g.fillAll(Colours::transparentBlack);

	g.setOpacity(1.0f);
	g.setColour(Colours::black);
	auto w = getWidth();
	auto h = getHeight();
	g.fillEllipse(0.f, 0.f, static_cast<float>(w), static_cast<float>(h));

	for(int i=0;i<sources->size();i++)
        if ((*sources)[i]->getElevation() < 0)
            drawSource(g,i);
	
    g.setOpacity(1.f);
	g.drawImageWithin(headImage_, 0, 0, w, h, RectanglePlacement::centred | RectanglePlacement::doNotResize);
	drawGridLines(g);
	
    for(int i=0;i<sources->size();i++)
        if ((*sources)[i]->getElevation() >= 0)
            drawSource(g,i);
}


void MainDisplay::drawGridLines(Graphics& g)
{
	g.setColour(Colours::white);
	g.setOpacity(0.5f);
	auto w = static_cast<float>(getWidth());
	auto h = static_cast<float>(getHeight());
	auto lineHorizontal = Line<float>(0.f, h * 0.5f, w, h * 0.5f);
	auto lineVertical = Line<float>(w * 0.5f, 0.f, w * 0.5f, h);
	float dashes[] = {3, 2};
	g.drawDashedLine(lineHorizontal, dashes, 2);
	g.drawDashedLine(lineVertical, dashes, 2);
}

void MainDisplay::drawSource(Graphics& g, int i)
{
	auto w = static_cast<float>(getWidth());
	auto h = static_cast<float>(getHeight());
	auto radius = w * 0.5f - 30;
    auto azimuth=((*sources)[i]->getAzimuth()*PI)/180;
    auto elevation=((*sources)[i]->getElevation()*PI)/180;
	
    
        auto x = radius * std::sin(azimuth) * std::cos(elevation);
        auto y = radius * std::cos(azimuth) * std::cos(elevation);
        x = w * 0.5f + x;
        y = h * 0.5f - y;
        auto color = Colours::cyan;
        if (elevation < 0)
        {
            color = color.darker();
            g.setOpacity(0.8f);
        }
        ColourGradient grad(color, x, y, Colours::transparentBlack, x + w * 0.25f, y + h * 0.25f, true);
        g.setGradientFill(grad);
        g.fillEllipse(0.f, 0.f, w, h);
        auto scaleFactor = 0.75f * (std::sin(elevation) * 0.25f + 1);
        auto sw = sourceImage_.getWidth();
        auto sh = sourceImage_.getHeight();
        g.drawImageWithin(sourceImage_,
                          static_cast<int>(x - sw * 0.5f * scaleFactor),
                          static_cast<int>(y - sh * 0.5f * scaleFactor),
                          static_cast<int>(sw * scaleFactor),
                          static_cast<int>(sh * scaleFactor),
                          RectanglePlacement::centred,
                          true);
}

void MainDisplay::setSources(std::vector<ScopedPointer<SoundSource>>* newSources)
{
    sources=newSources;
}
