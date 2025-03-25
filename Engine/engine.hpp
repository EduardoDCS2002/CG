#include <math.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include "tinyxml2.hpp"
#include <iterator>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES

using namespace std;
using namespace tinyxml2;

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

class Group{
	public:
		
		list<Group> subgroups;
		list<Ponto> pontos;
		int nr;
		float rotation[4];
		float scale[3];
		float translation[3];
	
		Group(int nr){
			this->nr = nr;
			for(int i = 0;i<4;i++){
				this->rotation[i] = 0.0f;
			}
			for(int i = 0;i<3;i++){
				this->scale[i] = 1.0f;
				this->translation[i] = 0.0f;
			}
			
		}

		//GETS
		int getnr(){
			return nr;
		}
		
		void getRotation(float outRotation[4]){
			for(int i = 0; i < 4; i++)
				outRotation[i] = rotation[i];
		}

		void getScale(float outScale[3]){
			for(int i = 0; i < 3; i++)
				outScale[i] = scale[i];
		}

		void getTranslation(float outTranslation[3]){
			for(int i = 0; i < 3; i++)
				outTranslation[i] = translation[i];
		}
		
		list<Group> getSubgroups(){       
			return subgroups;
		}
		list<Ponto> getPontos(){
			return this->pontos;
		}

		//SETS
		void setPontos(list<Ponto> npontos){
			this->pontos = npontos;
		}

		void setnr(int newnr){
			nr = newnr;
		}

		void setRotation(float newRotation[4]){
			for (int i = 0; i < 4; i++){
				rotation[i] = newRotation[i];
			}
		}

		void setScale(float newScale[3]){
			for (int i = 0; i < 3; i++){
				scale[i] = newScale[i];
			}
		}

		void setTranslation(float newTranslation[3]){
			for (int i = 0; i < 3; i++){
				translation[i] = newTranslation[i];
			}
			
		}

		// métodos das listas
		void addSubgroup(Group subgroup) {
			this->subgroups.push_back(subgroup);
		}
		void addPonto(Ponto ponto) {
			this->pontos.push_back(ponto);
		}
};