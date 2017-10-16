//
//  Room.hpp
//  Auris
//
//  Created by Alex on 11/1/17.
//
//

#ifndef Room_hpp
#define Room_hpp

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

class Room{
public:
    
    void load();
    void draw();
    
    int getTriangleCount(){return roomTriangles.size();}
    
    std::vector<Triangle> getRoomTriangles(){return roomTriangles;}

    
private:
    // private member variables
    //Obj elements
    std::vector<tinyobj::shape_t> shapes;				//stores shapes of the room obj
    std::vector<tinyobj::material_t> materials;			//this will be null because it only draws wireframe
    std::vector<tinyobj::shape_t> model;                //obj model
    
    std::vector<Triangle> roomTriangles;				//stores all room triangles
};

#endif /* Room_hpp */
