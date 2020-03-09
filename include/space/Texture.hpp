#ifndef TEXTURE
#define TEXTURE
#include <GL/glew.h>
#include <iostream>
#include <glimac/common.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>

using namespace glimac;
using namespace glm;

struct MultiTexProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uEarthTexture;
    GLint uCloudTexture;

    MultiTexProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "../shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "../shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uEarthTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
        uCloudTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture2");
    }
};

struct TexProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uTexture;

    TexProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "../shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "../shaders/tex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
    }
};

class Texture {
	public :
		void firstBindTexture(std::unique_ptr<Image> &texLoad, GLuint texture);
		void activeAndBindTexture(GLenum tex, GLuint texture);
};


#endif // TEXTURE