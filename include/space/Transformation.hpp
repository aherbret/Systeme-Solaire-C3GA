#ifndef TRANSFORMATION
#define TRANSFORMATION
#include <GL/glew.h>
#include <iostream>
#include <glimac/common.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <glimac/Sphere.hpp>
#include <c3ga/Mvec.hpp>

using namespace glimac;
using namespace glm;


class Transformation {
	public :
		c3ga::Mvec<double> translate(c3ga::Mvec<double> vect, double facteur = 0.2, c3ga::Mvec<double> translation = c3ga::e1<double>());
		c3ga::Mvec<double> rotate(c3ga::Mvec<double> vect, c3ga::Mvec<double> biVect = c3ga::e12<double>(), double angle = 180);
		c3ga::Mvec<double> scale(c3ga::Mvec<double> vect, double scale = 2.);

		glm::vec3 applyTranslationX(Sphere sphere);
		glm::vec3 applyRotation(Sphere sphere);
		glm::vec3 applyScale(Sphere sphere);
};


#endif // TRANSFORMATION