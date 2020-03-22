#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/Tore.hpp"

namespace glimac {
    // Constructeur: alloue le tableau de données et construit les attributs des vertex
    Tore::Tore(GLfloat ri, GLfloat re, GLsizei nbi, GLsizei nbe):
        vertexCount(0) {
        build(ri, re, nbi, nbe); // Construction (voir le .cpp)
    }

    Tore::~Tore(){}

    /*
    ri : rayon interieur 
    re : rayon exterieur
    */
    void Tore::build(GLfloat ri, GLfloat re, GLsizei nbi, GLsizei nbe) {
        
        std::vector<ShapeVertex> data;
        float x1, y1, z1;
        float alphai;
        float cosalphai;
        float sinalphai;
        float beta, cosbeta, sinbeta;
        float fact = 0.1;
        // Construit l'ensemble des vertex
        for (int i = 0; i < nbi + 1; i++) {
            alphai = 2 * M_PI * i / nbi;
            cosalphai = cos(alphai);
            sinalphai = sin(alphai);
            for (int j = 0; j <= nbe; j++) {
                beta = 2 * M_PI * j / nbe;
                cosbeta = cos(beta);
                sinbeta = sin(beta);

                ShapeVertex vertex;
                x1 = (re + ri * cosbeta) * cosalphai;
                y1 = (re + ri * cosbeta) * sinalphai;
                z1 = ri + 2;
                vertex.texCoords.x = i / nbi * fact;
                vertex.texCoords.y = j / nbe * fact;
                vertex.normal.x = cosbeta * cosalphai;
                vertex.normal.y = cosbeta * sinalphai;
                vertex.normal.z = sinbeta;
                vertex.position.x = x1;
                vertex.position.y = y1;
                vertex.position.z = z1;
                data.push_back(vertex);
            }
        }

        vertexCount = nbi * nbe * 6;
        for(GLsizei j = 0; j < nbe; ++j) {
            GLsizei offset = j * (nbi + 1);
            for(GLsizei i = 0; i < nbi; ++i) {
                vertices.push_back(data[offset + i]);
                vertices.push_back(data[offset + (i + 1)]);
                vertices.push_back(data[offset + nbi + 1 + (i + 1)]);
                vertices.push_back(data[offset + i]);
                vertices.push_back(data[offset + nbi + 1 + (i + 1)]);
                vertices.push_back(data[offset + i + nbi + 1]);
            }
        }
    }

    const ShapeVertex* Tore::getDataPointer() const {
        return &vertices[0];
    }
            
    GLsizei Tore::getVertexCount() const {
        return vertexCount;
    }
}
