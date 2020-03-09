#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/Sphere.hpp"

#include "c3ga/c3gaTools.hpp"

namespace glimac {
    Sphere::Sphere(const c3ga::Mvec<GLfloat> &radiusVector, GLuint64 latitude, GLuint64 longitude) : 
    radiusVector(radiusVector), latitude(latitude), longitude(longitude) {}
        
    // Constructeur: alloue le tableau de données et construit les attributs des vertex
    Sphere::Sphere(GLfloat radius, GLsizei discLat, GLsizei discLong):
        vertexCount(0) {
        build(radius, discLat, discLong); // Construction (voir le .cpp)
        //buildC3GA();
    }

    Sphere::~Sphere(){}

    void Sphere::build(GLfloat r, GLsizei discLat, GLsizei discLong) {
        /////
        radiusVector = c3ga::point<double>(0, 0, 0);
        std::cout << "Point C3GA initial : " << radiusVector << std::endl;
        ////

        GLfloat rcpLat = 1.f / discLat, rcpLong = 1.f / discLong;
        GLfloat dPhi = 2 * glm::pi<float>() * rcpLat, dTheta = glm::pi<float>() * rcpLong;
        
        std::vector<ShapeVertex> data;
        
        // Construit l'ensemble des vertex
        for(GLsizei j = 0; j <= discLong; ++j) {
            GLfloat cosTheta = cos(-glm::pi<float>() / 2 + j * dTheta);
            GLfloat sinTheta = sin(-glm::pi<float>() / 2 + j * dTheta);
            
            for(GLsizei i = 0; i <= discLat; ++i) {
                ShapeVertex vertex;
                
                vertex.texCoords.x = i * rcpLat;
                vertex.texCoords.y = 1.f - j * rcpLong;

                vertex.normal.x = sin(i * dPhi) * cosTheta;
                vertex.normal.y = sinTheta;
                vertex.normal.z = cos(i * dPhi) * cosTheta;
                
                vertex.position = r * vertex.normal;
                
                data.push_back(vertex);
            }
        }

        vertexCount = discLat * discLong * 6;
        for(GLsizei j = 0; j < discLong; ++j) {
            GLsizei offset = j * (discLat + 1);
            for(GLsizei i = 0; i < discLat; ++i) {
                vertices.push_back(data[offset + i]);
                vertices.push_back(data[offset + (i + 1)]);
                vertices.push_back(data[offset + discLat + 1 + (i + 1)]);
                vertices.push_back(data[offset + i]);
                vertices.push_back(data[offset + discLat + 1 + (i + 1)]);
                vertices.push_back(data[offset + i + discLat + 1]);
            }
        }
    }


    // Creation des vertex
    void Sphere::buildC3GA() {
        /*
        std::cout << radiusVector << std::endl;
        c3ga::Mvec<GLfloat> dual = radiusVector.dual();

        dual /= dual[c3ga::E0];

        GLfloat radius = std::sqrt(dual | dual);
        std::cout << "radius " << radius << " " << dual << " " << (dual | dual) << std::endl;
        std::cout << "radius " << dual[0] << " " << dual[1] << " " << dual[2] << std::endl;

        dual.display();
        */

        GLfloat radius = 10;
        c3ga::Mvec<double> sphere = c3ga::point<double>(0,1,0)
                            ^ c3ga::point<double>(0,0,-1)
                            ^ c3ga::point<double>(0,0,1)
                            ^ c3ga::point<double>(1,0,0);

        std::cout << "test " << sphere << " test" << std::endl;
        sphere.display();

        GLfloat rcpLat = 1.f / latitude, rcpLong = 1.f / longitude;
        GLfloat dPhi = 2 * glm::pi<float>() * rcpLat, dTheta = glm::pi<float>() * rcpLong;
        
        std::vector<ShapeVertex> data;

        // Construit l'ensemble des vertex
        for(GLsizei j = 0; j <= 10; ++j) {
            
        }
    }

    const ShapeVertex* Sphere::getDataPointer() const {
        return &vertices[0];
    }
            
    GLsizei Sphere::getVertexCount() const {
        return vertexCount;
    }

}
