#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/Sphere.hpp"

namespace glimac {
    Sphere::Sphere(const c3ga::Mvec<GLfloat> &radiusVector, GLuint64 latitude, GLuint64 longitude) : 
    radiusVector(radiusVector), latitude(latitude), longitude(longitude) {}
            
    Sphere::~Sphere(){}

    // void Sphere:build2(const c3ga::Mvec<GLfloat> &center, float radius) {
    //     3ga::Mvec<double> sphere = center - (radius * radius) / 2 * c3ga::ei<double>();
    //     sphere = sphere.dual();
    // }


    // Creation des vertex
    void Sphere::build() {
        std::cout << radiusVector << std::endl;
        c3ga::Mvec<GLfloat> dual = radiusVector.dual();
        dual /= dual[c3ga::E0];

        GLfloat radius = std::sqrt(dual | dual);
        std::cout << "radius " << radius << " " << dual << " " << (dual | dual) << std::endl;
        std::cout << "radius " << dual[0] << " " << dual[1] << " " << dual[2] << std::endl;

        GLfloat rcpLat = 1.f / latitude, rcpLong = 1.f / longitude;
        GLfloat dPhi = 2 * glm::pi<float>() * rcpLat, dTheta = glm::pi<float>() * rcpLong;
        
        std::vector<ShapeVertex> data;
        
        // Construit l'ensemble des vertex
        for(GLsizei j = 0; j <= longitude; ++j) {
            GLfloat cosTheta = cos(-glm::pi<float>() / 2 + j * dTheta);
            GLfloat sinTheta = sin(-glm::pi<float>() / 2 + j * dTheta);
            
            for(GLsizei i = 0; i <= latitude; ++i) {
                ShapeVertex vertex;
                
                vertex.texCoords.x = i * rcpLat;
                vertex.texCoords.y = 1.f - j * rcpLong;

                vertex.normal.x = sin(i * dPhi) * cosTheta;
                vertex.normal.y = sinTheta;
                vertex.normal.z = cos(i * dPhi) * cosTheta;
                
                vertex.position = radius * vertex.normal;
                
                data.push_back(vertex);
            }
        }

        vertexCount = latitude * longitude * 6;
        
        GLuint idx = 0;
        for(GLsizei j = 0; j < longitude; ++j) {
            GLsizei offset = j * (latitude + 1);
            for(GLsizei i = 0; i < latitude; ++i) {
                vertices.push_back(data[offset + i]);
                vertices.push_back(data[offset + (i + 1)]);
                vertices.push_back(data[offset + latitude + 1 + (i + 1)]);
                vertices.push_back(data[offset + i]);
                vertices.push_back(data[offset + latitude + 1 + (i + 1)]);
                vertices.push_back(data[offset + i + latitude + 1]);
            }
        }
    }

    const ShapeVertex* Sphere::getDataPointer() const {
        return vertices.data();
    }
            
    GLsizei Sphere::getVertexCount() const {
        //return vertexCount;
        return vertices.size();
    }

}
