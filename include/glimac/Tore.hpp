#pragma once

#include <vector>

#include "common.hpp"

#include <c3ga/Mvec.hpp>

namespace glimac {

    // Représente une sphère discrétisée centrée en (0, 0, 0) (dans son repère local)
    // Son axe vertical est (0, 1, 0) et ses axes transversaux sont (1, 0, 0) et (0, 0, 1)
    class Tore {

        public:
            // Constructeur
            Tore(GLfloat ri, GLfloat re, GLsizei nbi, GLsizei nbe);

            // Destructeur
            ~Tore();

            // Alloue et construit les données (implantation dans le .cpp)
            void build(GLfloat ri, GLfloat re, GLsizei nbi, GLsizei nbe);

            // Renvoit le pointeur vers les données
            const ShapeVertex* getDataPointer() const;
            
            // Renvoit le nombre de vertex
            GLsizei getVertexCount() const;

        private:
            std::vector<ShapeVertex> vertices;
            GLsizei vertexCount; // Nombre de sommets
            GLsizei latitude;
            GLsizei longitude;
      
    };
    
}