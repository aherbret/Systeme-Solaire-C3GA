#pragma once

#include <vector>

#include "common.hpp"

namespace glimac {

class TrackballCamera {

public:
    // Constructeur
    TrackballCamera();

    // Destructeur
    ~TrackballCamera();

    // permettant d'avancer / reculer la caméra. 
    // Lorsque delta est positif la caméra doit avancer, sinon elle doit reculer. 
    void moveFront(float delta);

    // permettant de tourner latéralement autour du centre de vision. 
    void rotateLeft(float degrees);

    // permettant de tourner verticalement autour du centre de vision. 
    void rotateUp(float degrees);

    glm::mat4 getViewMatrix() const;

private:
	float m_fDistance;  //la distance par rapport au centre de la scène
	float m_fAngleX; //l'angle effectuée par la caméra autour de l'axe x de la scène (rotation vers le haut ou vers le bas)
	float m_fAngleY; //l'angle effectuée par la caméra autour de l'axe y de la scène (rotation vers la gauche ou vers la droite)
};
    
}