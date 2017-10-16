//
//  Room.cpp
//  Auris
//
//  Created by Alex on 11/1/17.
//
//

#include "Room.h"

void Room::load(){

    //Open obj room
    File objFile = File::getCurrentWorkingDirectory().getChildFile ("laberinto.obj");
    
    std::string err;
    bool ret = tinyobj::LoadObj(shapes, materials, err, objFile.getFullPathName().getCharPointer());
    
    
    if (!err.empty()) { // `err` may contain warning message.
        std::cerr << err << std::endl;
    }
    
    if (!ret) {
        std::cerr << "No room found" << std::endl;
        exit(1);
    }
    
    //loads room triangles into roomTriangles array
    for (size_t i = 0; i < shapes.size(); i++) {
        
        size_t indexOffset = 0;
        for (size_t n = 0; n < shapes[i].mesh.num_vertices.size(); n++) {
            int ngon = shapes[i].mesh.num_vertices[n];
            
            Triangle aux;
            
            for (size_t f = 0; f < ngon; f++) {
                unsigned int v = shapes[i].mesh.indices[indexOffset + f];
                aux.vertexes[f].x = shapes[i].mesh.positions[3 * v + 0];
                aux.vertexes[f].y = shapes[i].mesh.positions[3 * v + 1];
                aux.vertexes[f].z = shapes[i].mesh.positions[3 * v + 2];
                
            }
            
            roomTriangles.push_back(aux);
            indexOffset += ngon;
        }
        
    }
}

void Room::draw(){
    //Draw the room
    for(int i=0;i<roomTriangles.size();i++)
    {
        
        
        glBegin(GL_LINE_LOOP);
        for(int j=0;j<3;j++)
            glVertex3f(roomTriangles[i].vertexes[j].x,
                       roomTriangles[i].vertexes[j].y,
                       roomTriangles[i].vertexes[j].z);
        
        glEnd();
    }
    
}
