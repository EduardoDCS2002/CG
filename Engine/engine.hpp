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
//#include <GL/glew.h>
//#include <GL/glut.h>
#include "../toolkits/glut/GL/glut.h"
#include "../toolkits/glew/GL/glew.h"

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
		
		list<Group*> subgroups;
		//list<Ponto> pontos;
		int nr;
		float rotation[4];
		float rotationTime[4];
		float scale[3];
		float translation[3];
		float translationTime;
		bool align;
		std::list<Ponto> pontosTranslacao;
		GLuint vertices;
		int verticeCount;

		Group(int nr){
			this->nr = nr;
			for(int i = 0;i<4;i++){
				this->rotation[i] = 0.0f;
				this->rotationTime[i] = 0.0f;
			}
			for(int i = 0;i<3;i++){
				this->scale[i] = 1.0f;
				this->translation[i] = 0.0f;
			}
			this->translationTime = -1;
			this->align = 0;
			this->vertices = 0;
			this->verticeCount = 0;
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

		float getTranslationTime(){
			return translationTime;
		}

		void getRotationTime(float outRotationTime[4]){
			for (int i = 0; i < 4; i++){
				outRotationTime[i] = rotationTime[i];
			}
		}

		bool getAlign(){
			return align;
		}

		GLuint getVertices(){
			return vertices;
		}

		int getVerticeCount(){
			return verticeCount;
		}
		
		list<Group*> getSubgroups(){       
			return subgroups;
		}
		list<Ponto> getPontosTranslacao(){
			return this->pontosTranslacao;
		}
		/*
		list<Ponto> getPontos(){
			return this->pontos;
		}

		//SETS
		void setPontos(list<Ponto> npontos){
			this->pontos = npontos;
		}
		*/

		void setnr(int newnr){
			nr = newnr;
		}

		void setRotation(float newRotation[4]){
			for (int i = 0; i < 4; i++){
				rotation[i] = newRotation[i];
			}
		}

		void setRotationTime(float newRotationTime[4]){
			for (int i = 0; i < 4; i++){
				rotationTime[i] = newRotationTime[i];
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

		void setTranslationTime(float newTranslationTime){
			translationTime = newTranslationTime;
		}

		void setAlign(float newAlign){
			align = newAlign;
		}

		void setVertices(GLuint newVertices){
			vertices = newVertices;
		}

		void setVerticeCount(int newVerticeCount){
			verticeCount = newVerticeCount;
		}

		// métodos das listas
		void addSubgroup(Group* subgroup) {
			this->subgroups.push_back(subgroup);
		}
		/*
		void addPonto(Ponto ponto) {
			this->pontos.push_back(ponto);
		}
		*/
		void addPontoTranslacao(Ponto ponto){
			this->pontosTranslacao.push_back(ponto);
		}
};
