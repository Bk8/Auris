//
//  SoundSource.cpp
//  Auris
//
//  Created by Alex on 11/1/17.
//
//

#include "SoundSource.h"

SoundSource::SoundSource()
{
    elevation=0;
    azimuth=0;
}

SoundSource::SoundSource(std::string objFileName,std::string audioFileName,Vec3f position)
{
    load(objFileName,audioFileName);
    setPosition(position);
    elevation=0;
    azimuth=0;
}

SoundSource::~SoundSource()
{
    audioDeviceManager->removeAudioCallback(&binauralAudioSource);
}

void SoundSource::load(std::string objFileName,std::string audioFileName){
    //Objs obstacles
    File objFile = File::getCurrentWorkingDirectory().getChildFile(objFileName.c_str());

    
    std::string err;
    bool ret = tinyobj::LoadObj(shapes, materials, err, objFile.getFullPathName().getCharPointer());
    
    
    if (!err.empty()) { // `err` may contain warning message.
        std::cerr << err << std::endl;
    }
    
    if (!ret) {
        std::cerr << "No obj source found" << std::endl;
        exit(1);
    }
    if(File::getCurrentWorkingDirectory().getChildFile(audioFileName.c_str()).exists())
    {
        binauralAudioSource.setFile(File::getCurrentWorkingDirectory().getChildFile(audioFileName.c_str()));
        binauralAudioSource.bypassAudio(true);
        binauralAudioSource.play();
    }
        else
            std::cerr << "No sound file found" << std::endl;
}

void SoundSource::draw()
{
    //Draw the models (obstacles)
    
        for (size_t i = 0; i < shapes.size(); i++) {
            size_t indexOffset = 0;
            for (size_t n = 0; n <  shapes[i].mesh.num_vertices.size(); n++) {
                int ngon = shapes[i].mesh.num_vertices[n];
                
                Triangle aux;
                glBegin(GL_LINE_LOOP);
                for (size_t f = 0; f < ngon; f++) {
                    unsigned int v = shapes[i].mesh.indices[indexOffset + f];
                    glVertex3f(shapes[i].mesh.positions[3 * v + 0]+position.x,
                               shapes[i].mesh.positions[3 * v + 1]+position.y,
                               shapes[i].mesh.positions[3 * v + 2]+position.z);
                }
                glEnd();
                indexOffset += ngon;
            }
            
        }
    
}

void SoundSource::updateListenerPosition(FPSCamera camera,Room *room)
{
    float listenerXRotation;	//rotation of the head in x axis
    float listenerYRotation;	//rotation of the head in y axis // check!!
    bool intersectionRaySphere;
    IntersectionInfo info;	//info of the intersection btw ray and sphere (one per ray)
    Vec3f translatedIntersection;	//intersection translated to head coordinate system
    
    listenerXRotation = (-camera.getXrotation()*180)/PI;
    listenerYRotation = (-camera.getYrotation()*180)/PI;
    
    Ray *ray=new Ray(position,camera.sphere.center - position);
    
    intersectionRaySphere = camera.sphere.getIntersection(ray, info);
    
    //Checks all ray triangle collisions
    for (size_t i = 0; i < room->getTriangleCount(); i++)
    {
        double dist;
        Vec3f intPoint;
        
        if (room->getRoomTriangles()[i].intersectsTriangle(ray, dist,intPoint) && dist < info.distance)
            intersectionRaySphere = false;	//If ray there is collision btw ray and triangle it wont be supoused to listen so make the boolean false
    }
    
    //These will be the rotation matrix
    Matrix4 m1;
    Matrix4 m2;
    
    if (intersectionRaySphere) {
        
        //do not bypass audio
        binauralAudioSource.bypassAudio(false);
        
        //Translate intersection to listener coordinate system
        translatedIntersection = camera.sphere.center- ray->startPos;
        m1.rotateY(listenerXRotation);
        translatedIntersection = m1*translatedIntersection;
        m2.rotateX(listenerYRotation);
        translatedIntersection = m2*translatedIntersection;
        
        
        //http://www.antenna-theory.com/definitions/sphericalCoordinates.php
        //Transform to polar coordinates
        
        azimuth = acos(translatedIntersection.x / sqrt(pow(translatedIntersection.x, 2) + pow(translatedIntersection.z, 2)));
        
        elevation = atan(sqrt(pow(translatedIntersection.x, 2) + pow(translatedIntersection.z, 2)) / translatedIntersection.y);
        
        // Transforms azimuth and elevation to degrees
        elevation = (elevation * 180) / PI;
        azimuth = (azimuth * 180) / PI;
        
        //Centers azimuth
        azimuth = azimuth-90;
        
        //Transformations depending of the quadrant
        if (elevation < 0)
            elevation = elevation+90;
        else
            elevation = elevation-90;
        
        if (translatedIntersection.z < 0 && translatedIntersection.x > 0)
            azimuth = -90-(90+azimuth);
        
        if (translatedIntersection.z < 0 && translatedIntersection.x < 0)
            azimuth = 180-azimuth;
        
        
        binauralAudioSource.setDistance((camera.sphere.center - ray->startPos).length());
        
        //Sets azimuth and elevation to appropriate audioPlayer
        binauralAudioSource.updateHRIR(azimuth, elevation);
        
    }
    else
    {
        //If it loses intersection azimuth and elevation will be nan
        azimuth = std::numeric_limits<float>::quiet_NaN();
        elevation = std::numeric_limits<float>::quiet_NaN();
        
        //Makes appropriate audioPlayer bypass the audio
        binauralAudioSource.bypassAudio(true);
    }
    
}
