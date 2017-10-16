
 /*
 * This file is part of the AURIS distribution
 * Copyright (c) 2017 Alex Armendáriz
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#define PI 3.14159265358979323846
#include "../JuceLibraryCode/JuceHeader.h"

#if JUCE_WINDOWS
#pragma comment(lib, "glu32.lib")
#include <Windows.h>
#include <gl/glu.h>
#endif

#if JUCE_MAC
#include <GLUT/glut.h>
#endif

#include "Camera/Camera.h"
#include "Primitives/Triangle.h"


#include"Simulation/Room.h"
#include"Simulation/SoundSource.h"
#include"Math/Matrices.h"

#include "Component/MainDisplay.h"




//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public OpenGLAppComponent, public KeyListener, public Timer
{
public:
    //==============================================================================
    MainContentComponent()
    {
        //-------------------------------------------------------------------------------
        int numOutputChannels = 2, numInputChannels = 0;
        
        //initialize to default audio device
        audioDeviceManager.initialise (numInputChannels, /* number of input channels */
                                       numOutputChannels, /* number of output channels */
                                       0, /* no XML settings.. */
                                       true  /* select default device on failure */);
        //-------------------------------------------------------------------------------
        setSize (800, 600);
        
        //Centers mouse cursor
        Point<float> newPosition(getWidth()/2, getHeight()/2);
        Desktop::getInstance().getMouseSource(0)->setScreenPosition(newPosition);
        
        setWantsKeyboardFocus(true);
        getTopLevelComponent()->addKeyListener(this);
        
        // First position of the camera
        camera.setPosition(0, 1.5f, 30, 0, 1.5f, 0, 0, 1, 0);
        
        room.load();
        

        sources.push_back(new SoundSource("Speaker.obj","BK8 - HuacaySiqui.mp3",Vec3f(0,0,-4)));
        sources.push_back(new SoundSource("Speaker.obj","BK8 - Lullaby.mp3",Vec3f(10,0,0)));
        
        for(int i=0;i<sources.size();i++)
            sources[i]->setAudioDeviceManager(&audioDeviceManager);
        

        
        setMouseCursor (MouseCursor::NoCursor);
        mainDisplay.setSources(&sources);
        
        addAndMakeVisible(mainDisplay);
        mainDisplay.setBounds(getWidth()*(2/3),getHeight()*(2/3), 300, 300);
        
        startTimer(100);
        

        
    }

    ~MainContentComponent()
    {
        shutdownOpenGL();
    }

    void initialise() override
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0,1.0);
        glLineWidth(1);								//opengl line width
        glPointSize(30);
        sizeOpenGLScreen(getWidth(), getHeight());
    }

    void shutdown() override
    {
    }
    bool keyPressed (const KeyPress &key, Component *originatingComponent) override
    {

        //Close program
        if(key == KeyPress::escapeKey) {
            JUCEApplicationBase::quit();
        }
        return true;
        
    }

    void render() override
    {
        OpenGLHelpers::clear(Colours::black);
        //Takes cursor postion into the screen
        Point<float> point =Desktop::getInstance().getMouseSource(0)->getScreenPosition();
        
        //Updates the camera view and data
        camera.update(point.x, point.y, getWidth(), getHeight());
        
        //Checks camera-room collisions
        camera.checkCameraCollision(&room.getRoomTriangles()[0], room.getTriangleCount());
        
        glLoadIdentity();
        
        camera.look();
        
        for(int i=0;i<sources.size();i++)
            sources[i]->updateListenerPosition(camera, &room);

        glColor3f(1, 0, 0.8);
        room.draw();
        
        glColor3f(0, 0.75, 0.94);
        for(int i=0;i<sources.size();i++)
            sources[i]->draw();
        
        

    }
    


    void paint (Graphics& g) override
    {
        // You can add your component specific drawing code here!
        // This will draw over the top of the openGL background.
        g.setColour(Colours::magenta);
        
        int height=100;
        g.drawText("Auris v0", 0, getHeight()-height, height, height, Justification::centredLeft);
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }
    
    void sizeOpenGLScreen(int width, int height)			// Initialize The GL Window
    {

        glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
        glLoadIdentity();									// Reset The Projection Matrix
        
        // Calculate The Aspect Ratio Of The Window
        // The parameters are:
        // (view angle, aspect ration of the width to the height,
        //  the closest distance to the camera before it clips,
        // FOV		// Ratio				//  the farthest distance before it stops drawing).
        
        gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 150.0f);
        
        glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
        glLoadIdentity();									// Reset The Modelview Matrix
    }
    
    void timerCallback() override
    {
        //Set azimuth and elevation into head display component and repaint it
        mainDisplay.repaint();

    }


private:
    //==============================================================================

    //Camera
    FPSCamera camera;
    Room room;
    AudioDeviceManager audioDeviceManager;
    std::vector<ScopedPointer<SoundSource>> sources;
    MainDisplay mainDisplay;
    
    
    //------------------------------------------------------------------------------
    



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()    { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
