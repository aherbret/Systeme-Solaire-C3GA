#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include <../include/space/Texture.hpp>

void Texture::firstBindTexture(std::unique_ptr<Image> &texLoad, GLuint texture) {
    //Binding de la texture 
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texLoad->getWidth(), texLoad->getHeight(), 0, GL_RGBA, GL_FLOAT, texLoad->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //debindage de la texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::activeAndBindTexture(GLenum tex, GLuint texture) {
	glActiveTexture(tex);
    glBindTexture(GL_TEXTURE_2D, texture);
}
