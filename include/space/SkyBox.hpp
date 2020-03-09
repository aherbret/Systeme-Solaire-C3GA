#ifndef SKYBOX
#define SKYBOX
#include <GL/glew.h>
#include <iostream>
#include <glimac/common.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <space/Texture.hpp>

using namespace glimac;
using namespace glm;

class SkyBox {
	public :

		// Constructeur
        SkyBox(const GLsizei count_vertex_skybox, const ShapeVertex *verticesSkybox);

        // Destructeur
        ~SkyBox();

        void buildSkyBox(const GLsizei count_vertex_skybox, const ShapeVertex *verticesSkybox);
        unsigned int loadCubemap(std::vector<std::string> faces);
        void activeSkyBox(const Skytext &skytext, const GLuint &cubemapTexture, float distRendu, float ratio_h_w, glm::mat4 VMatrix);

    private :
    	GLuint vbo;
    	GLuint vao;

};


#endif // SKYBOX