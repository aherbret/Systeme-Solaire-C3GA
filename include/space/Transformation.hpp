#ifndef TRANSFORMATION
#define TRANSFORMATION
	#include <iostream>
	#include <GL/glew.h>
	#include <c3ga/Mvec.hpp>
	#include <glimac/Image.hpp>
	#include <glimac/Sphere.hpp>
	#include <glimac/common.hpp>
	#include <glimac/Program.hpp>
	#include <glimac/FilePath.hpp>

	using namespace glimac;
	using namespace glm;

	class Transformation {
		public :
			/*
	         * Calcul de la translation avec C3GA
	         * @param vect : le vecteur de la shpere C3GA.
	         * @param facteur : le facteur de translation.
	         * @param translation : l'axe de translation.
	         */
			c3ga::Mvec<double> translate(c3ga::Mvec<double> vect, double facteur = 0.2, c3ga::Mvec<double> translation = c3ga::e1<double>());

			/*
	         * Calcul de la rotation avec C3GA
	         * @param vect : le vecteur de la shpere C3GA.
	         * @param angle : l'angle de rotation.
	         * @param biVect : l'axe de rotation.
	         */
			c3ga::Mvec<double> rotate(c3ga::Mvec<double> vect, double angle = 180, c3ga::Mvec<double> biVect = c3ga::e12<double>());

			/*
	         * Calcul du scale avec C3GA
	         * @param vect : le vecteur de la shpere C3GA.
	         * @param scale : coefficient de scale.
	         */
			c3ga::Mvec<double> scale(c3ga::Mvec<double> vect, double scale = 2.);

			/*
	         * Application de la translation sur l'axe x.
	         * @param sphere : la sphere qui faut appliquer la transformation.
	         */
			glm::vec3 applyTranslationX(Sphere sphere);

			/*
	         * Application de la rotation.
	         * @param sphere : la sphere qui faut appliquer la transformation.
	         */
			glm::vec3 applyRotation(Sphere sphere);

			/*
	         * Application du scale.
	         * @param sphere : la sphere qui faut appliquer la transformation.
	         */
			glm::vec3 applyScale(Sphere sphere);
	};

#endif // TRANSFORMATION