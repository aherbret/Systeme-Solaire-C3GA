#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Sphere.hpp>
#include <glimac/common.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <glimac/Image.hpp>
#include <glimac/Cube.hpp>
#include <glimac/Geometry.hpp>
#include <cstddef>
#include <vector>
#include <glimac/TrackballCamera.hpp>
#include <../include/glimac/FreeflyCamera.hpp>
#include <../include/space/Texture.hpp>
#include <../include/space/SkyBox.hpp>
#include <../include/space/Transformation.hpp>

#include <c3ga/Mvec.hpp>

using namespace glimac;
using namespace glm;

const std::string TEXTURE_DIR = "../assets/textures";
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
	int width_windows = 1350;
    int height_windows = 700;
    float ratio_h_w = (float)width_windows / (float)height_windows;
    // Initialize SDL and open a window
    SDLWindowManager windowManager(width_windows, height_windows, "GLImac");

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
    Skytext skytex(applicationPath);

    Sphere sphere(1, 32, 16); // rayon = 1, latitude = 32, longitude = 16

    Sphere sphere2(1, 32, 16); // rayon = 1, latitude = 32, longitude = 16

    Transformation t;

    t.scale(sphere2.getSphere());

    //Sphere sphere(point(1.0,2.0,0.0), 32, 16); // rayon = 1, latitude = 32, longitude = 16
    //sphere.buildC3GA();
    
    // SkyBox
    float size_cube = 1;
    Cube cubeSkybox(size_cube);
    GLsizei count_vertex_skybox = cubeSkybox.getVertexCount();
    const ShapeVertex*  Datapointeur_skybox = cubeSkybox.getDataPointer();
    ShapeVertex verticesSkybox[count_vertex_skybox];
    for (auto i = 0; i < count_vertex_skybox; i++) { // Skybox
        verticesSkybox[i] = *Datapointeur_skybox;
        verticesSkybox[i].position.x -= 0.5;
        verticesSkybox[i].position.y -= 0.5;
        verticesSkybox[i].position.z += 0.5;
        Datapointeur_skybox++;
    }
    SkyBox skybox(count_vertex_skybox, verticesSkybox);

    GLuint texSpatial;
    // Texture Spatial Skybox
    std::vector<std::string> facesGalaxy {
        TEXTURE_DIR + "/etoiles/right.png",
        TEXTURE_DIR + "/etoiles/left.png",
        TEXTURE_DIR + "/etoiles/top1.png",
        TEXTURE_DIR + "/etoiles/bottom.png",
        TEXTURE_DIR + "/etoiles/front.png",
        TEXTURE_DIR + "/etoiles/back.png"
    };
    //Binding de la texture Spatial
    texSpatial = skybox.loadCubemap(facesGalaxy);
    float distRendu = 2500.0f;
    //// Fin SkyBox

    // Textures planetes
    Texture tex;
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
    tex.firstBindTexture(SunMap, texture[0]); //Binding de la texture SunMap
    tex.firstBindTexture(MoonMap, texture[1]); //Binding de la texture MoonMap
    tex.firstBindTexture(CloudMap, texture[2]); //Binding de la texture CloudMap
    tex.firstBindTexture(EarthMap, texture[3]); //Binding de la texture EarthMap
    //// Fin Textures planetes

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnable(GL_DEPTH_TEST);
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 100.f);

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
    
    glm::ivec2 lastmousePos;
    bool flag = false;
    FreeflyCamera Camera;
    float defaultspeed = 0.5;
    float speedcam = 0.5;
    float boost_speed = 0.5;
    // Application loop:
    bool done = false;

    while (!done) {
        //glm::mat4 ViewMatrix = Camera.getViewMatrix();
        // Event loop:
        SDL_Event e;
        while (windowManager.pollEvent(e)) {
            if (e.type == SDL_QUIT || windowManager.isKeyPressed(SDLK_ESCAPE)) {
                done = true; // Leave the loop after this iteration
            }
            if (e.type == SDL_VIDEORESIZE) {
                width_windows = e.resize.w;
                height_windows = e.resize.h;
                glViewport(0, 0, (GLsizei) width_windows, (GLsizei) height_windows);
                ratio_h_w = (float)width_windows / (float)height_windows;
                ProjMatrix = glm::perspective(glm::radians(70.f), ratio_h_w, 0.1f, distRendu);
            }
        }

        // Gestion de la camera
        if (windowManager.isMouseButtonPressed(SDL_BUTTON_LEFT) == true) {
            SDL_GetRelativeMouseState(&lastmousePos.x, &lastmousePos.y);
            if (flag == true) {
                Camera.rotateLeft(lastmousePos.x / 5);
                Camera.rotateUp(lastmousePos.y / 5);
            }
            flag = true;
        }
        else {
            flag = false;
        }
        //Ici on récupère les touches du clavier
        if (windowManager.isKeyPressed(SDLK_LCTRL) == true) {
            speedcam = defaultspeed * boost_speed;
        }
        else {
            speedcam = defaultspeed;
        }

        if (windowManager.isKeyPressed(SDLK_z) == true) {
            Camera.moveFront(speedcam);
        }
        if (windowManager.isKeyPressed(SDLK_q) == true) {
            Camera.moveLeft(speedcam);
        }

        if (windowManager.isKeyPressed(SDLK_s) == true) {
            Camera.moveFront(-speedcam);
        }

        if (windowManager.isKeyPressed(SDLK_d) == true) {
            Camera.moveLeft(-speedcam);
        }
        //// Fin Gestion de la camera

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
        // Nettoyage de la fenêtre
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Affichage de la skybox
        glm::mat4  VMatrix = Camera.getViewMatrix();
        skybox.activeSkyBox(skytex, texSpatial, distRendu, ratio_h_w, VMatrix);





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
        glUniform1i(sunProgram.uTexture, 0);

        // Specify the value of a uniform variable for the current program object
        glUniformMatrix4fv(sunProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(sunMVMatrix));
        glUniformMatrix4fv(sunProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(sunMVMatrix))));
        glUniformMatrix4fv(sunProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * sunMVMatrix));
        tex.activeAndBindTexture(GL_TEXTURE0, texture[1]);
        glDrawArrays(GL_TRIANGLES, 0, sphere2.getVertexCount());
        glActiveTexture(GL_TEXTURE0);
        tex.activeAndBindTexture(GL_TEXTURE0, 0); // la texture earthTexture est bindée sur l'unité GL_TEXTURE0
        tex.activeAndBindTexture(GL_TEXTURE1, 0); // la texture cloudTexture est bindée sur l'unité GL_TEXTURE1
        glUniform1i(sunProgram.uTexture, 0);

        // Soleil (grande) avec C3GA
        moonProgram.m_Program.use();
        glUniform1i(moonProgram.uTexture, 0);

        glm::mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5)); // Translation
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        MVMatrix = glm::rotate(MVMatrix, windowManager.getTime(), AxesRotation.at(1)); // Translation * Rotation
        MVMatrix = glm::translate(MVMatrix, Translations.at(1)); // Translation * Rotation * Translation
        MVMatrix = glm::scale(MVMatrix, glm::vec3(0.2, 0.2, 0.2)); // Translation * Rotation * Translation * Scale
       
        glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(moonProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        glUniformMatrix4fv(moonProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
		tex.activeAndBindTexture(GL_TEXTURE0, texture[1]);
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glActiveTexture(GL_TEXTURE0);
        tex.activeAndBindTexture(GL_TEXTURE0, 0); // la texture earthTexture est bindée sur l'unité GL_TEXTURE2
        glUniform1i(moonProgram.uTexture, 0);


        glBindVertexArray(0);
        tex.activeAndBindTexture(GL_TEXTURE0, 0); // la texture moonTexture est bindée sur l'unité GL_TEXTURE0

        // Update the display
        windowManager.swapBuffers();
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    return EXIT_SUCCESS;
}
