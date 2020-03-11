#include <cmath>
#include <list>
#include <iostream>
#include "glimac/common.hpp"
#include "space/Transformation.hpp"

#include "c3ga/c3gaTools.hpp"


c3ga::Mvec<double> Transformation::translate(c3ga::Mvec<double> vect, c3ga::Mvec<double> translation, double facteur) {
	//c3ga::Mvec<double> translation = c3ga::e1<double>();
    //double facteur = 1;
    c3ga::Mvec<double> translator = 1 - 0.5 * translation * c3ga::ei<double>() * facteur;    
	vect = translator * vect * translator.inv();
    return vect;
}

glm::vec3 Transformation::applyTranslation(c3ga::Mvec<double> vect) {
	/*
	std::cout << vect[c3ga::E0123] << std::endl;
	std::cout << vect[c3ga::E123i] << std::endl;
	std::cout << vect[c3ga::E0123i] << std::endl;
	*/
	translate(vect);
	return glm::vec3(vect[c3ga::E0123], vect[c3ga::E123i] + 0.5, vect[c3ga::E0123i]);
}

c3ga::Mvec<double> Transformation::rotate(c3ga::Mvec<double> vect, c3ga::Mvec<double> biVect, double angle) {
	//c3ga::Mvec<double> biVect = c3ga::e12<double>();
    //double angle = 2.;
    c3ga::Mvec<double> rotor = cos(0.5 * angle * M_PI/180) - biVect * sin(0.5 * angle * M_PI/180);    
	vect = rotor * vect * rotor.inv();
	vect.roundZero(1.0e-10);
    return vect;
}

c3ga::Mvec<double> Transformation::scale(c3ga::Mvec<double> vect, double scale) {
	//double scale = 2.;
    c3ga::Mvec<double> dilator = 1. - ((1. - scale) / (1. + scale)) * c3ga::e0i<double>();  
	vect = dilator * vect * dilator.inv();
	vect.roundZero(1.0e-10);
    return vect;
}

glm::vec3 Transformation::applyScale(c3ga::Mvec<double> vect) {
	std::cout << vect[c3ga::E0123] << std::endl;
	std::cout << vect[c3ga::E123i] << std::endl;
	std::cout << vect[c3ga::E0123i] << std::endl;
	scale(vect, 0.2);
	return glm::vec3(vect[c3ga::E0123], vect[c3ga::E123i], 0);
}