#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glut.h>
//#include "../toolkits/glut/GL/glut.h"

#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <cmath>

class Ponto {
	float x;
	float y;
	float z;

public:
	Ponto() {
		x = 0;
		y = 0;
		z = 0;
	}

	Ponto(float a, float b, float c) {
		x = a;
		y = b;
		z = c;
	}

	float getX() const {
		return x;
	}

	float getY() const {
		return y;
	}

	float getZ() const {
		return z;
	}

	void setX(float a) {
		x = a;
	}

	void setY(float b) {
		y = b;
	}

	void setZ(float c) {
		z = c;
	}

	//comprimento do vetor
	float length() {
		return sqrt(x * x + y * y + z * z);
	}

	//
	Ponto cross(Ponto p) {
		return Ponto(
			y * p.getZ() - z * p.getY(),
			z * p.getX() - x * p.getZ(),
			x * p.getY() - y * p.getX()
		);	
	}

	Ponto normalize() const {
    float len = sqrt(x*x + y*y + z*z);
    return (len == 0) ? Ponto(0, 0, 0) : Ponto(x/len, y/len, z/len);
}

	Ponto operator-(const Ponto& p) const {
		return Ponto(x - p.x, y - p.y, z - p.z);
	}
};