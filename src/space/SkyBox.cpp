#include <cmath>
#include <vector>
#include <iostream>
#include <glimac/Image.hpp>
#include "glimac/common.hpp"
#include "../src/glimac/stb_image.h"
#include "../include/glimac/Cube.hpp"
#include <../include/space/SkyBox.hpp>

SkyBox::SkyBox(const GLsizei count_vertex_skybox, const ShapeVertex *verticesSkybox) {
    buildSkyBox(count_vertex_skybox, verticesSkybox);
}

SkyBox::~SkyBox() {}

void SkyBox::buildSkyBox(const GLsizei count_vertex_skybox, const ShapeVertex *verticesSkybox) {
	/// Bind VBO for skybox
    //GLuint vbo;
    glGenBuffers(1, &vbo);
    // Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, count_vertex_skybox * sizeof(ShapeVertex), verticesSkybox, GL_STATIC_DRAW); // Envoi des données
    //Après avoir modifié le VBO, on le débind de la cible pour éviter de le remodifier par erreur
    glBindBuffer(GL_ARRAY_BUFFER, 0); // debind
    /// Bind VAO for skybox
    //GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0); // VERTEX_ATTR_POSITION
    glEnableVertexAttribArray(1); // VERTEX_ATTR_NORMAL
    glEnableVertexAttribArray(2); // VERTEX_ATTR_TEXT
    // VERTEX_ATTR_POSITION
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)(offsetof(ShapeVertex, position)));
    // VERTEX_ATTR_NORMAL
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)(offsetof(ShapeVertex, normal)));
    // VERTEX_ATTR_TEXT
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)(offsetof(ShapeVertex, texCoords)));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

unsigned int SkyBox::loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return textureID;
}

void SkyBox::activeSkyBox(const Skytext &skytext, const GLuint &cubemapTexture, float distRendu, float ratio_h_w, glm::mat4 VMatrix) {
    glBindVertexArray(vao);

    glm::mat4 MVMatrix, ProjMatrix, MVPMatrix, NormalMatrix;
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    skytext.m_Program.use();
    // ... définir la matrice de vue et projection
    ProjMatrix = glm::perspective(glm::radians(70.f), ratio_h_w, 0.1f, distRendu);
    MVMatrix = glm::mat4(glm::mat3(VMatrix)); // Remove translation from the MV
    MVPMatrix = ProjMatrix * MVMatrix;
    glUniformMatrix4fv(skytext.uMVP, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
    //debindage de la texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    glBindVertexArray(0);
}