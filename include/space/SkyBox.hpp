#ifndef SKYBOX
#define SKYBOX
    #include <iostream>
    #include <GL/glew.h>
    #include <glimac/Image.hpp>
    #include <glimac/common.hpp>
    #include <space/Texture.hpp>
    #include <glimac/Program.hpp>
    #include <glimac/FilePath.hpp>

    using namespace glimac;
    using namespace glm;

    class SkyBox {
    	public :
            /*
             * Constructeur de la skybox.
             * @param count_vertex_skybox : le nombre de vertex.
             * @param verticesSkybox : les vertices de la skybox.
             */
            SkyBox(const GLsizei count_vertex_skybox, const ShapeVertex *verticesSkybox);

            /*
             * Destructeur
             */
            ~SkyBox();

            /*
             * Creation de la skybox.
             * @param count_vertex_skybox : le nombre de vertex.
             * @param verticesSkybox : les vertices de la skybox.
             */
            void buildSkyBox(const GLsizei count_vertex_skybox, const ShapeVertex *verticesSkybox);

            /*
             * Chargement de la cubemap.
             * @param faces : un vecteur contenant les faces de la skybox.
             */
            unsigned int loadCubemap(std::vector<std::string> faces);

            /*
             * Permet l'activation et l'affichage de la skybox.
             * @param skytext : le programme de la skybox qui permet le chargement des textures.
             * @param cubemapTexture : l'identifiant de la texture souhaitée.
             * @param distRendu : la distance de rendu.
             * @param ratio_h_w : le ratio en fonction de la taille de la fenetre.
             * @param VMatrix : la view matrice.
             */
            void activeSkyBox(const Skytext &skytext, const GLuint &cubemapTexture, float distRendu, float ratio_h_w, glm::mat4 VMatrix);

        private :
        	GLuint vbo;
        	GLuint vao;
    };

#endif // SKYBOX