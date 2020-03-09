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
            Sphere(GLfloat radius, GLsizei discLat, GLsizei discLong);

            // Destructeur
            ~Sphere();

            // Alloue et construit les données (implantation dans le .cpp)
            void build(GLfloat radius, GLsizei discLat, GLsizei discLong);

            // Alloue et construit les données (implantation dans le .cpp)
            void buildC3GA();

            // Renvoit le pointeur vers les données
            const ShapeVertex* getDataPointer() const;
            
            // Renvoit le nombre de vertex
            GLsizei getVertexCount() const;

            c3ga::Mvec<double> sphere(float Rsphere);

        private:
        	c3ga::Mvec<double> s;
        	std::list<c3ga::Mvec<double>> coordsphere;
            std::vector<ShapeVertex> vertices;
            c3ga::Mvec<GLfloat> radiusVector;
            GLsizei vertexCount; // Nombre de sommets
            GLsizei latitude;
            GLsizei longitude;
      
    };
    
}