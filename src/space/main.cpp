#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Sphere.hpp>
#include <glimac/common.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <glimac/Image.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Geometry.hpp>
#include <cstddef>
#include <vector>
#include <glimac/TrackballCamera.hpp>
#include <../include/glimac/FreeflyCamera.hpp>
#include <../include/space/Texture.hpp>

#include <c3ga/Mvec.hpp>

using namespace glimac;
using namespace glm;

const GLuint VERTEX_ATTR_POSITION = 0;
const GLuint VERTEX_ATTR_NORMAL = 1;
const GLuint VERTEX_ATTR_TEXCOORD = 2;

c3ga::Mvec<GLfloat> point(const GLfloat &x, const GLfloat &y, const GLfloat &z){
    c3ga::Mvec<GLfloat> mv;
    mv[c3ga::E1] = x;
    mv[c3ga::E2] = y;
    mv[c3ga::E3] = z;
    mv[c3ga::Ei] = 0.5 * (x*x + y*y + z*z);
    mv[c3ga::E0] = 1.0;

    return mv;
}

// void drawSphere() {
//     c3ga::Mvec<double> pt1 = point(1.0, 0.0, 0.0);
//     c3ga::Mvec<double> pt2 = point(-1.0, 0.0, 0.0);
//     c3ga::Mvec<double> pt3 = point(0.0,0.0,-1.0);
//     c3ga::Mvec<double> pt4 = point(0.0,1.0,0.0);
//     c3ga::Mvec<double> s = pt1 ^ pt2 ^ pt3 ^ pt4;

// }

int main(int argc, char** argv) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(800, 600, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    FilePath applicationPath(argv[0]);
    MultiTexProgram earthProgram(applicationPath);
    TexProgram sunProgram(applicationPath);
    TexProgram moonProgram(applicationPath);
    Program program = loadProgram(applicationPath.dirPath() + "../shaders/3D.vs.glsl",
                                  applicationPath.dirPath() + "../shaders/tex3D.fs.glsl");

    Sphere sphere(1, 32, 16); // rayon = 1, latitude = 32, longitude = 16
    //Sphere sphere(point(1.0,2.0,0.0), 32, 16); // rayon = 1, latitude = 32, longitude = 16
    //sphere.buildC3GA();
    Texture tex;
    std::cout << sphere.getVertexCount() << std::endl;

    glEnable(GL_DEPTH_TEST);
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 100.f);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //Création d'une texture
    std::unique_ptr<Image> SunMap = loadImage("../assets/textures/SunMap.jpg");
    std::unique_ptr<Image> MoonMap = loadImage("../assets/textures/MoonMap.jpg");
    std::unique_ptr<Image> CloudMap = loadImage("../assets/textures/CloudMap.jpg");
    std::unique_ptr<Image> EarthMap = loadImage("../assets/textures/EarthMap.jpg");

    if (SunMap == NULL || MoonMap == NULL || CloudMap == NULL || EarthMap == NULL) {
        std::cerr << "Une des textures n'a pas pu etre chargée. \n" << std::endl;
        exit(0);
    }

    GLuint texture[4];
    glGenTextures(3, texture);
    //Binding de la texture SunMap
    tex.firstBindTexture(SunMap, texture[0]);
    //Binding de la texture MoonMap
    tex.firstBindTexture(MoonMap, texture[1]);
    //Binding de la texture CloudMap
    tex.firstBindTexture(CloudMap, texture[2]);
    //Binding de la texture EarthMap
    tex.firstBindTexture(EarthMap, texture[3]);

    glBufferData(GL_ARRAY_BUFFER, sphere.getVertexCount() * sizeof(ShapeVertex), sphere.getDataPointer(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORD);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,  sizeof(ShapeVertex), (const GLvoid *)(offsetof(ShapeVertex, position)));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,  sizeof(ShapeVertex), (const GLvoid *)(offsetof(ShapeVertex, normal)));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORD, 2, GL_FLOAT, GL_FALSE,  sizeof(ShapeVertex), (const GLvoid *)(offsetof(ShapeVertex, texCoords)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::vector<glm::vec3> AxesRotation;
    std::vector<glm::vec3> Translations;
    for (int i = 0; i < 32; ++i) { 
        AxesRotation.push_back(glm::sphericalRand(1.0f));
        Translations.push_back(glm::sphericalRand(2.0f));
    }

    FreeflyCamera Camera;
    // Application loop:
    bool done = false;
    while (!done) {
        //glm::mat4 ViewMatrix = Camera.getViewMatrix();
        // Event loop:
        SDL_Event e;
        while (windowManager.pollEvent(e)) {
            if (e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
        //Ici on récupère les touches du clavier
        if (windowManager.isKeyPressed(SDLK_s)) { Camera.moveFront(-0.1); }
        if (windowManager.isKeyPressed(SDLK_z)) { Camera.moveFront(0.1); }
        if (windowManager.isKeyPressed(SDLK_q)) { Camera.moveLeft(0.1); }
        if (windowManager.isKeyPressed(SDLK_d)) { Camera.moveLeft(-0.1); }

        glm::ivec2 mousePos = glm::ivec2(0.0);
        if (windowManager.isMouseButtonPressed(SDL_BUTTON_LEFT)) {
            mousePos = windowManager.getMousePosition();
            float mousePosX = mousePos.x / 800.0f - 0.5;
            float mousePosY = mousePos.y / 600.0f - 0.5;
            Camera.rotateLeft(-2 * mousePosX);
            Camera.rotateUp(-2 * mousePosY);
        }

        // Nettoyage de la fenêtre
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);



        // Soleil (grande) avec OPENGL
        sunProgram.m_Program.use();
        glUniform1i(sunProgram.uTexture, 0);
        glm::mat4 globalMVMatrix = Camera.getViewMatrix();
        glm::mat4 sunMVMatrix = glm::rotate(globalMVMatrix, windowManager.getTime(), glm::vec3(0, 1, 0));
        // Specify the value of a uniform variable for the current program object
        glUniformMatrix4fv(sunProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(sunMVMatrix));
        glUniformMatrix4fv(sunProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(sunMVMatrix))));
        glUniformMatrix4fv(sunProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * sunMVMatrix));
        tex.activeAndBindTexture(GL_TEXTURE0, texture[0]);
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glActiveTexture(GL_TEXTURE0);
        tex.activeAndBindTexture(GL_TEXTURE0, 0); // la texture earthTexture est bindée sur l'unité GL_TEXTURE0
        tex.activeAndBindTexture(GL_TEXTURE1, 0); // la texture cloudTexture est bindée sur l'unité GL_TEXTURE1
        glUniform1i(sunProgram.uTexture, 0);

        // Soleil (grande) avec C3GA


        glBindVertexArray(0);
        tex.activeAndBindTexture(GL_TEXTURE0, 0); // la texture moonTexture est bindée sur l'unité GL_TEXTURE0

        // Update the display
        windowManager.swapBuffers();
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    return EXIT_SUCCESS;
}
