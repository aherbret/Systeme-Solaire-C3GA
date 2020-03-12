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
#include <glimac/Tore.hpp>
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


void drawPlanet(Sphere & sphere, TexProgram & program, Texture & tex, GLuint tex_planet, SDLWindowManager & windowManager, glm::mat4 & globalMVMatrix, glm::mat4 & ProjMatrix, glm::vec3 & rotateGlobal,
    glm::vec3 & translate, glm::vec3 & scale, glm::vec3 & rotate, float speed) {
    program.m_Program.use();
    glUniform1i(program.uTexture, 0);

    //glm::mat4 MVMatrix = glm::rotate(globalMVMatrix, windowManager.getTime(), glm::vec3(0, 1, 0)); // Translation * Rotation
    glm::mat4 MVMatrix = glm::rotate(globalMVMatrix, windowManager.getTime()*speed, rotateGlobal);

    MVMatrix = glm::translate(MVMatrix, translate);
    MVMatrix = glm::scale(MVMatrix, scale);
    MVMatrix = glm::rotate(MVMatrix, windowManager.getTime(), rotate);
    
    // Specify the value of a uniform variable for the current program object
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(MVMatrix))));
    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
    tex.activeAndBindTexture(GL_TEXTURE0, tex_planet);
    glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
    glActiveTexture(GL_TEXTURE0);
    tex.activeAndBindTexture(GL_TEXTURE0, 0); // la texture earthTexture est bindée sur l'unité GL_TEXTURE2
    glUniform1i(program.uTexture, 0);
}

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
    TexProgram earthProgram(applicationPath);
    TexProgram sunProgram(applicationPath);
    TexProgram moonProgram(applicationPath);
    TexProgram mercureProgram(applicationPath);
    TexProgram venusProgram(applicationPath);
    TexProgram marsProgram(applicationPath);
    TexProgram jupiterProgram(applicationPath);
    TexProgram saturneProgram(applicationPath);
    TexProgram uranusProgram(applicationPath);
    TexProgram neptuneProgram(applicationPath);
    Skytext skytex(applicationPath);

    Sphere sphere(1, 32, 16); // rayon = 1, latitude = 32, longitude = 16
    Tore tore(0.1, 1, 72, 36);

    Transformation transfo;
    // std::cout << sphere.getSphere() << std::endl;
    // //transfo.scale(sphere, 15);
    // sphere.setSphere(transfo.scale2(sphere.getSphere(), 5));
    // //transfo.applyScale(sphere);
    // //sphere.setSphere(transfo.translate2(sphere.getSphere()));
    // std::cout << sphere.getSphere() << std::endl;

    // std::cout << transfo.applyScale(sphere) << std::endl;
    // glm::vec3 scalescale = transfo.applyScale(sphere);

    //exit(EXIT_FAILURE);
    
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
    std::unique_ptr<Image> MercureMap = loadImage("../assets/textures/Mercure.jpg");
    std::unique_ptr<Image> VenusMap = loadImage("../assets/textures/Venus.jpg");
    std::unique_ptr<Image> MarsMap = loadImage("../assets/textures/Mars.jpg");
    std::unique_ptr<Image> JupiterMap = loadImage("../assets/textures/Jupiter.jpg");
    std::unique_ptr<Image> SaturneMap = loadImage("../assets/textures/Saturne.jpg");
    std::unique_ptr<Image> UranusMap = loadImage("../assets/textures/Uranus.jpg");
    std::unique_ptr<Image> NeptuneMap = loadImage("../assets/textures/Neptune.jpg");

    if (SunMap == NULL || MoonMap == NULL || CloudMap == NULL || EarthMap == NULL
        || MercureMap == NULL || VenusMap == NULL || MarsMap == NULL || JupiterMap == NULL
        || SaturneMap == NULL || UranusMap == NULL || NeptuneMap == NULL) {
        std::cerr << "Une des textures n'a pas pu etre chargée. \n" << std::endl;
        exit(0);
    }
    GLuint texture[11];
    glGenTextures(10, texture);
    tex.firstBindTexture(SunMap, texture[0]); //Binding de la texture SunMap
    tex.firstBindTexture(MoonMap, texture[1]); //Binding de la texture MoonMap
    tex.firstBindTexture(CloudMap, texture[2]); //Binding de la texture CloudMap
    tex.firstBindTexture(EarthMap, texture[3]); //Binding de la texture EarthMap
    tex.firstBindTexture(MercureMap, texture[4]); //Binding de la texture MercureMap
    tex.firstBindTexture(VenusMap, texture[5]); //Binding de la texture VenusMap
    tex.firstBindTexture(MarsMap, texture[6]); //Binding de la texture MarsMap
    tex.firstBindTexture(JupiterMap, texture[7]); //Binding de la texture JupiterMap
    tex.firstBindTexture(SaturneMap, texture[8]); //Binding de la texture SaturneMap
    tex.firstBindTexture(UranusMap, texture[9]); //Binding de la texture UranusMap
    tex.firstBindTexture(NeptuneMap, texture[10]); //Binding de la texture NeptuneMap
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
    

    /* Tore */

    GLuint vbo_tore;
    glGenBuffers(1, &vbo_tore);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_tore);

    glEnable(GL_DEPTH_TEST);
    glm::mat4 ProjMatrixTore = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 100.f);

    glBufferData(GL_ARRAY_BUFFER, tore.getVertexCount() * sizeof(ShapeVertex), tore.getDataPointer(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao_tore;
    glGenVertexArrays(1, &vao_tore);
    glBindVertexArray(vao_tore);
    
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORD);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_tore);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,  sizeof(ShapeVertex), (const GLvoid *)(offsetof(ShapeVertex, position)));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,  sizeof(ShapeVertex), (const GLvoid *)(offsetof(ShapeVertex, normal)));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORD, 2, GL_FLOAT, GL_FALSE,  sizeof(ShapeVertex), (const GLvoid *)(offsetof(ShapeVertex, texCoords)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);





    glm::ivec2 lastmousePos;
    bool flag = false;
    FreeflyCamera Camera;
    float defaultspeed = 0.5;
    float speedcam = 0.5;
    float boost_speed = 0.5;
    // Application loop:
    bool done = false;

    
    //sphere.setSphere(transfo.rotate(sphere.getSphere(), 0, c3ga::e23<double>()));
    //glm::vec3 rotateGlobal = transfo.applyRotation(sphere);
    glm::vec3 rotateGlobal = glm::vec3(0, 1, 0);
    /* Mercure */
        /* Translation */
        sphere.setSphere(transfo.translate(sphere.getSphere()));
        glm::vec3 translateMercure = transfo.applyTranslationX(sphere);
        /* Scale */
        sphere.setSphere(transfo.scale(sphere.getSphere(), 5));
        glm::vec3 scaleMercure = transfo.applyScale(sphere);
        /* Rotation */
        sphere.setSphere(transfo.rotate(sphere.getSphere()));
        glm::vec3 rotateMercure = transfo.applyRotation(sphere);
    /* Venus */
        /* Translation */
        //std::cout << sphere.getSphere() << std::endl;
        sphere.setSphere(transfo.translate(sphere.getSphere(), 0.5));
        glm::vec3 translateVenus = transfo.applyTranslationX(sphere);
        //std::cout << sphere.getSphere() << std::endl;
        /* Scale */
        glm::vec3 scaleVenus = scaleMercure;
        /* Rotation */
        glm::vec3 rotateVenus = rotateMercure;
    /* Terre */
        /* Translation */
        //std::cout << sphere.getSphere() << std::endl;
        sphere.setSphere(transfo.translate(sphere.getSphere(), 8.5));
        glm::vec3 translateEarth = transfo.applyTranslationX(sphere);
        //std::cout << sphere.getSphere() << std::endl;
        /* Scale */
        glm::vec3 scaleEarth = scaleMercure;
        /* Rotation */
        glm::vec3 rotateEarth = rotateMercure;
    /* Mars */
        /* Translation */
        sphere.setSphere(transfo.translate(sphere.getSphere(), 1));
        glm::vec3 translateMars = transfo.applyTranslationX(sphere);
        /* Scale */
        glm::vec3 scaleMars = scaleMercure;
        /* Rotation */
        glm::vec3 rotateMars = rotateMercure;
    /* Jupiter */
        /* Translation */
        sphere.setSphere(transfo.translate(sphere.getSphere(), 1.5));
        glm::vec3 translateJupiter = transfo.applyTranslationX(sphere);
        /* Scale */
        sphere.setSphere(transfo.scale(sphere.getSphere(), 0.5));
        glm::vec3 scaleJupiter = transfo.applyScale(sphere);
        /* Rotation */
        glm::vec3 rotateJupiter = rotateMercure;
    /* Saturne */
        /* Translation */
        sphere.setSphere(transfo.translate(sphere.getSphere(), 1));
        glm::vec3 translateSaturne = transfo.applyTranslationX(sphere);
        /* Scale */
        glm::vec3 scaleSaturne = scaleMercure;
        /* Rotation */
        glm::vec3 rotateSaturne = rotateMercure;
    /* Uranus */
        /* Translation */
        sphere.setSphere(transfo.translate(sphere.getSphere(), 1.5));
        glm::vec3 translateUranus = transfo.applyTranslationX(sphere);
        /* Scale */
        glm::vec3 scaleUranus = scaleMercure;
        /* Rotation */
        glm::vec3 rotateUranus = rotateMercure;
    /* Neptune */
        /* Translation */
        sphere.setSphere(transfo.translate(sphere.getSphere(), 1));
        glm::vec3 translateNeptune = transfo.applyTranslationX(sphere);
        /* Scale */
        glm::vec3 scaleNeptune = scaleMercure;
        /* Rotation */
        glm::vec3 rotateNeptune = rotateMercure;


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
        if (windowManager.isKeyPressed(SDLK_LCTRL) == true) { speedcam = defaultspeed * boost_speed; }
        else { speedcam = defaultspeed; }

        if (windowManager.isKeyPressed(SDLK_z) == true) { Camera.moveFront(speedcam); }
        if (windowManager.isKeyPressed(SDLK_q) == true) { Camera.moveLeft(speedcam); }

        if (windowManager.isKeyPressed(SDLK_s) == true) { Camera.moveFront(-speedcam); }

        if (windowManager.isKeyPressed(SDLK_d) == true) { Camera.moveLeft(-speedcam); }
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

        glm::mat4 globalMVMatrix = Camera.getViewMatrix();

        // Soleil (grande) avec OPENGL
        sunProgram.m_Program.use();
        glUniform1i(sunProgram.uTexture, 0);
        glm::mat4 sunMVMatrix = glm::rotate(globalMVMatrix, windowManager.getTime(), rotateGlobal);
        // Specify the value of a uniform variable for the current program object
        glUniformMatrix4fv(sunProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(sunMVMatrix));
        glUniformMatrix4fv(sunProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(sunMVMatrix))));
        glUniformMatrix4fv(sunProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * sunMVMatrix));
        tex.activeAndBindTexture(GL_TEXTURE0, texture[0]);
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glActiveTexture(GL_TEXTURE0);
        tex.activeAndBindTexture(GL_TEXTURE0, 0); // la texture earthTexture est bindée sur l'unité GL_TEXTURE0
        glUniform1i(sunProgram.uTexture, 0);

        /*
        // Mercure avec C3GA
        mercureProgram.m_Program.use();
        glUniform1i(mercureProgram.uTexture, 0);

        glm::mat4 mercureMVMatrix = glm::rotate(globalMVMatrix, windowManager.getTime(), glm::vec3(0, 1, 0)); // Translation * Rotation

        mercureMVMatrix = glm::translate(mercureMVMatrix, translateMercure);
        mercureMVMatrix = glm::scale(mercureMVMatrix, scaleMercure);
        mercureMVMatrix = glm::rotate(mercureMVMatrix, windowManager.getTime(), rotateMercure);
        
        // Specify the value of a uniform variable for the current program object
        glUniformMatrix4fv(mercureProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(mercureMVMatrix));
        glUniformMatrix4fv(mercureProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(mercureMVMatrix))));
        glUniformMatrix4fv(mercureProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * mercureMVMatrix));
		tex.activeAndBindTexture(GL_TEXTURE0, texture[4]);
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glActiveTexture(GL_TEXTURE0);
        tex.activeAndBindTexture(GL_TEXTURE0, 0); // la texture earthTexture est bindée sur l'unité GL_TEXTURE2
        glUniform1i(mercureProgram.uTexture, 0);


        // Venus avec C3GA
        venusProgram.m_Program.use();
        glUniform1i(venusProgram.uTexture, 0);

        glm::mat4 venusMVMatrix = glm::rotate(globalMVMatrix, windowManager.getTime(), glm::vec3(0, 1, 0)); // Translation * Rotation

        venusMVMatrix = glm::translate(venusMVMatrix, translateVenus);
        venusMVMatrix = glm::scale(venusMVMatrix, scaleVenus);
        venusMVMatrix = glm::rotate(venusMVMatrix, windowManager.getTime(), rotateVenus);
        
        // Specify the value of a uniform variable for the current program object
        glUniformMatrix4fv(venusProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(venusMVMatrix));
        glUniformMatrix4fv(venusProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(venusMVMatrix))));
        glUniformMatrix4fv(venusProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * venusMVMatrix));
        tex.activeAndBindTexture(GL_TEXTURE0, texture[5]);
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glActiveTexture(GL_TEXTURE0);
        tex.activeAndBindTexture(GL_TEXTURE0, 0); // la texture earthTexture est bindée sur l'unité GL_TEXTURE2
        glUniform1i(venusProgram.uTexture, 0);

        glBindVertexArray(0);
        */

        drawPlanet(sphere, mercureProgram, tex, texture[4], windowManager,
            globalMVMatrix, ProjMatrix, rotateGlobal, translateMercure, scaleMercure, rotateMercure, 1);

        drawPlanet(sphere, venusProgram, tex, texture[5], windowManager,
            globalMVMatrix, ProjMatrix, rotateGlobal, translateVenus, scaleVenus, rotateVenus, 2);


        // Terre avec C3GA
        drawPlanet(sphere, earthProgram, tex, texture[3], windowManager,
            globalMVMatrix, ProjMatrix, rotateGlobal, translateEarth, scaleEarth, rotateEarth, 1);
        // earthProgram.m_Program.use();
        // glUniform1i(earthProgram.uEarthTexture, 0);
        // glUniform1i(earthProgram.uCloudTexture, 1);

        // glm::mat4 earthMVMatrix = glm::rotate(globalMVMatrix, windowManager.getTime(), rotateGlobal); // Translation * Rotation

        // earthMVMatrix = glm::translate(earthMVMatrix, translateEarth);
        // earthMVMatrix = glm::scale(earthMVMatrix, scaleEarth);
        // earthMVMatrix = glm::rotate(earthMVMatrix, windowManager.getTime(), rotateEarth);
        
        // // Specify the value of a uniform variable for the current program object
        // glUniformMatrix4fv(earthProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(earthMVMatrix));
        // glUniformMatrix4fv(earthProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(earthMVMatrix))));
        // glUniformMatrix4fv(earthProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * earthMVMatrix));
        // tex.activeAndBindTexture(GL_TEXTURE0, texture[3]);
        // tex.activeAndBindTexture(GL_TEXTURE1, texture[2]);
        // glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        // tex.activeAndBindTexture(GL_TEXTURE0, 0);
        // tex.activeAndBindTexture(GL_TEXTURE1, 0);
        // //glUniform1i(earthProgram.uEarthTexture, 0);

        drawPlanet(sphere, marsProgram, tex, texture[6], windowManager,
            globalMVMatrix, ProjMatrix, rotateGlobal, translateMars, scaleMars, rotateMars, 0.2);

        drawPlanet(sphere, jupiterProgram, tex, texture[7], windowManager,
            globalMVMatrix, ProjMatrix, rotateGlobal, translateJupiter, scaleJupiter, rotateJupiter, 1);

        drawPlanet(sphere, saturneProgram, tex, texture[8], windowManager,
            globalMVMatrix, ProjMatrix, rotateGlobal, translateSaturne, scaleSaturne, rotateSaturne, 0.5);

        drawPlanet(sphere, uranusProgram, tex, texture[9], windowManager,
            globalMVMatrix, ProjMatrix, rotateGlobal, translateUranus, scaleUranus, rotateUranus, 1);

        drawPlanet(sphere, neptuneProgram, tex, texture[10], windowManager,
            globalMVMatrix, ProjMatrix, rotateGlobal, translateNeptune, scaleNeptune, rotateNeptune, 1.5);

        glBindVertexArray(0);

        /*
        // Tore avec C3GA
        glBindVertexArray(vao_tore);
        venusProgram.m_Program.use();
        glUniform1i(venusProgram.uTexture, 0);

        venusMVMatrix = glm::rotate(globalMVMatrix, windowManager.getTime(), glm::vec3(0, 1, 0)); // Translation * Rotation

        venusMVMatrix = glm::rotate(venusMVMatrix, windowManager.getTime(), glm::vec3(0, 1, 0));
        
        // Specify the value of a uniform variable for the current program object
        glUniformMatrix4fv(venusProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(venusMVMatrix));
        glUniformMatrix4fv(venusProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(venusMVMatrix))));
        glUniformMatrix4fv(venusProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * venusMVMatrix));
        tex.activeAndBindTexture(GL_TEXTURE0, texture[5]);
        glDrawArrays(GL_TRIANGLES, 0, tore.getVertexCount());
        glActiveTexture(GL_TEXTURE0);
        tex.activeAndBindTexture(GL_TEXTURE0, 0); // la texture earthTexture est bindée sur l'unité GL_TEXTURE2
        glUniform1i(venusProgram.uTexture, 0);


        glBindVertexArray(0);
        tex.activeAndBindTexture(GL_TEXTURE0, 0); // la texture moonTexture est bindée sur l'unité GL_TEXTURE0
        */

        // Update the display
        windowManager.swapBuffers();
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    return EXIT_SUCCESS;
}
