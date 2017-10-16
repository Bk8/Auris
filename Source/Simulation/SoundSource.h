//
//  SoundSource.hpp
//  Auris
//
//  Created by Alex on 11/1/17.
//
//

#ifndef SoundSource_hpp
#define SoundSource_hpp

#include "../JuceLibraryCode/JuceHeader.h"

#if JUCE_WINDOWS
#pragma comment(lib, "glu32.lib")
#include <Windows.h>
#include <gl/glu.h>
#endif

#if JUCE_MAC
#include <GLUT/glut.h>
#endif

#include "Room.h"
#include "../ObjLoader/tiny_obj_loader.h"
#include "../Primitives/Triangle.h"
#include "../Math/Vectors.h"
#include "../Audio/BinauralAudioSource.h"
#include "../Camera/Camera.h"
#include "../Math/Matrices.h"
#include <string>
#include <atomic>

class SoundSource
{
public:
    SoundSource();
    ~SoundSource();
    SoundSource(std::string objFileName,std::string audioFileName,Vec3f position);
    void load(std::string objFileName,std::string audioFileName);
    void setPosition(Vec3f newPosition){position=newPosition;}
    void draw();
    Vec3f getPosition(){return position;}
    void updateListenerPosition(FPSCamera camera,Room *room);
    
    void setAudioDeviceManager(AudioDeviceManager* newAudioDeviceM){
        audioDeviceManager=newAudioDeviceM;
        audioDeviceManager->addAudioCallback(&binauralAudioSource);
    }
    float getAzimuth(){return azimuth;}
    float getElevation(){return elevation;}
    
private:
    //Obj elements
    std::vector<tinyobj::shape_t> shapes;				//stores shapes of the room obj
    std::vector<tinyobj::material_t> materials;			//this will be null because it only draws wireframe
    Vec3f position;
    BinauralAudioSource binauralAudioSource;
    
     std::atomic<float>elevation;			//variable to store elevation of the head
     std::atomic<float>azimuth;				//variable to store azimuth of the head
    
    AudioDeviceManager* audioDeviceManager;
    
    

};

#endif /* SoundSource_hpp */
