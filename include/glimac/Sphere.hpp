#pragma once

#include <vector>

#include "common.hpp"

#include <c3ga/Mvec.hpp>

namespace glimac {

    // Représente une sphère discrétisée centrée en (0, 0, 0) (dans son repère local)
    // Son axe vertical est (0, 1, 0) et ses axes transversaux sont (1, 0, 0) et (0, 0, 1)
    class Sphere {

        public:
            // Constructeur
            Sphere(const c3ga::Mvec<GLfloat> &radiusVector, GLuint64 longitude, GLuint64 latitude);

            // Destructeur
            ~Sphere();

            // Alloue et construit les données (implantation dans le .cpp)
            void build();

            // Renvoit le pointeur vers les données
            const ShapeVertex* getDataPointer() const;
            
            // Renvoit le nombre de vertex
            GLsizei getVertexCount() const;

        private:
            std::vector<ShapeVertex> vertices;
            c3ga::Mvec<GLfloat> radiusVector;
            GLsizei vertexCount; // Nombre de sommets
            GLsizei latitude;
            GLsizei longitude;
      
    };
    
}