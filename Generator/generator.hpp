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

	float getX() {
		return x;
	}

	float getY() {
		return y;
	}

	float getZ() {
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
};