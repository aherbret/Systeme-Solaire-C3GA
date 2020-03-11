#include <cmath>
#include <list>
#include <iostream>
#include "glimac/common.hpp"
#include "space/Transformation.hpp"

#include "c3ga/c3gaTools.hpp"


void Transformation::translate(Sphere sphere, c3ga::Mvec<double> translation, double facteur) {
	//c3ga::Mvec<double> translation = c3ga::e1<double>();
    //double facteur = 1;
    c3ga::Mvec<double> s = sphere.getSphere();
    c3ga::Mvec<double> translator = 1 - 0.5 * translation * c3ga::ei<double>() * facteur;    
	s = translator * s * translator.inv();
	sphere.setSphere(s);
}

glm::vec3 Transformation::applyTranslation(Sphere sphere) {
	/*
	std::cout << vect[c3ga::E0123] << std::endl;
	std::cout << vect[c3ga::E123i] << std::endl;
	std::cout << vect[c3ga::E0123i] << std::endl;
	*/
	translate(sphere);
	c3ga::Mvec<double> s = sphere.getSphere();
	return glm::vec3(s[c3ga::E0123], s[c3ga::E123i] + 0.5, s[c3ga::E0123i]);
}

c3ga::Mvec<double> Transformation::rotate(c3ga::Mvec<double> vect, c3ga::Mvec<double> biVect, double angle) {
	//c3ga::Mvec<double> biVect = c3ga::e12<double>();
    //double angle = 2.;
    c3ga::Mvec<double> rotor = cos(0.5 * angle * M_PI/180) - biVect * sin(0.5 * angle * M_PI/180);    
	vect = rotor * vect * rotor.inv();
	vect.roundZero(1.0e-10);
    return vect;
}

void Transformation::scale(Sphere sphere, double scale) {
	//double scale = 2.;
	c3ga::Mvec<double> s = sphere.getSphere();
    c3ga::Mvec<double> dilator = 1. - ((1. - scale) / (1. + scale)) * c3ga::e0i<double>();  
	s = dilator * s * dilator.inv();
	//vect.roundZero(1.0e-10);
	sphere.setSphere(s);
}

glm::vec3 Transformation::applyScale(Sphere sphere) {
	
	scale(sphere, 10);
	/*
	std::cout << vect[c3ga::E0123] << std::endl;
	std::cout << vect[c3ga::E123i] << std::endl;
	std::cout << vect[c3ga::E0123i] << std::endl;
	*/
	c3ga::Mvec<double> s = sphere.getSphere();
	return glm::vec3(s[c3ga::E0123], s[c3ga::E123i], 0);
}