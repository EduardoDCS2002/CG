#include <math.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include "tinyxml2.hpp"
#include <iterator>
#include <map>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <IL/il.h>
//#include "../toolkits/glut/GL/glut.h"
//#include "../toolkits/glew/GL/glew.h"
//#include "../toolkits/devil/IL/il.h"
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

class Color {
	public:
	float diffuse[3];
	float ambient[3];
	float specular[3];
	float emissive[3];
	float shininess;

	Color() {
		diffuse[0] = diffuse[1] = diffuse[2] = 200.0f / 255.0f;
		ambient[0] = ambient[1] = ambient[2] = 50.0f / 255.0f;
		specular[0] = specular[1] = specular[2] = 0.0f;
		emissive[0] = emissive[1] = emissive[2] = 0.0f;
		shininess = 0.0f;
	}

	void getDiffuse(float outDiffuse[3]){
		for(int i = 0; i < 3; i++){
			outDiffuse[i] = diffuse[i];
		}
	}

	void getAmbient(float outAmbient[3]){
		for(int i = 0; i < 3; i++){
			outAmbient[i] = ambient[i];
		}
	}

	void getSpecular(float outSpecular[3]){
		for(int i = 0; i < 3; i++){
			outSpecular[i] = specular[i];
		}
	}

	void getEmissive(float outEmissive[3]){
		for(int i = 0; i < 3; i++){
			outEmissive[i] = emissive[i];
		}
	}

	void getShininess(float outShininess){
		shininess = outShininess;
	}

	void setDiffuse(float newDiffuse[3]){
		for(int i = 0; i < 3; i++)
			diffuse[i] = newDiffuse[i];
	}

	void setAmbient(float newAmbient[3]){
		for(int i = 0; i < 3; i++)
			ambient[i] = newAmbient[i];
	}

	void setSpecular(float newSpecular[3]){
		for(int i = 0; i < 3; i++)
			specular[i] = newSpecular[i];
	}

	void setEmissive(float newEmissive[3]){
		for(int i = 0; i < 3; i++)
			emissive[i] = newEmissive[i];
	}

	void setShininess(float newShininess){
		shininess = newShininess;
	}
};

#define TRANSLATE 0
#define ROTATE 1
#define SCALE 2

class Models {
	public:
    GLuint vboVertices;
    GLuint vboNormals;
    GLuint vboTexCoords;
    int vertexCount;
    string texture;
    Color color;

    Models() {
        vboVertices = 0;
        vboNormals = 0;
        vboTexCoords = 0;
        vertexCount = 0;
        texture = "";
    }

    void setVertices(GLuint newVertices) { 
		vboVertices = newVertices; 
	}
    void setNormals(GLuint newNormals) {
		 vboNormals = newNormals; 
	}
    void setTexCoords(GLuint newTextCoords) {
		 vboTexCoords = newTextCoords; 
	}
    void setVertexCount(int newCount) {
		 vertexCount = newCount; 
	}
    void setTexture(string newTexture) {
		 texture = newTexture; 
	}
    void setColor(const Color& c) {
		 color = c; 
	}

    GLuint getVertices() const {
		 return vboVertices; 
	}
    GLuint getNormals() const {
		 return vboNormals; 
	}
    GLuint getTexCoords() const {
		 return vboTexCoords; 
	}
    int getVertexCount() const {
		 return vertexCount; 
	}
    string getTexture() const {
		 return texture; 
	}
    Color getColor() const {
		 return color; 
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
		vector<Models> modelos;
		int order[3];

		Group(int nr){
			this->nr = nr;
			for(int i = 0;i<4;i++){
				this->rotation[i] = 0.0f;
				this->rotationTime[i] = 0.0f;
			}
			for(int i = 0;i<3;i++){
				this->scale[i] = 1.0f;
				this->translation[i] = 0.0f;
				this->order[i] = -1;
			}
			this->translationTime = -1;
			this->align = 0;
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

		void getOrder(int outOrder[3]){
			for(int i = 0; i < 3; i++)
				outOrder[i] = order[i];
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

		void setOrder(int newOrder[3]){
			for (int i = 0; i < 3; i++){
				order[i] = newOrder[i];
			}
		}

		void setAlign(float newAlign){
			align = newAlign;
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


class Light {
	public:
	string type;
	float pos[3];
    float dir[3];
    float cutoff;

	Light(){
		this->type = "";
		for(int i=0; i < 3; i++){
			this->pos[i] = 0;
			this->dir[i] = 0;
		}
		this->cutoff = 0;
	}

	// Getters
    std::string getType(){ 
		return type; 
	}

    void getPos(float outPos[3]){ 
		for(int i = 0; i < 3; i++)
			outPos[i] = pos[i];
	}

    void getDir(float outDir[3]){ 
		for(int i = 0; i < 3; i++)
			outDir[i] = dir[i];
	}

    float getCutoff(){ 
		return cutoff;
	}

    // Setters
    void setType(std::string newType){
		type = newType; 
	}

    void setPos(float newPos[3]) {
        for(int i = 0; i < 3; i++){
			pos[i] = newPos[i];
		}
    }

    void setDir(float newDir[3]) {
        for(int i = 0; i < 3; i++){
			dir[i] = newDir[i];
		}
    }

    void setCutoff(float newCutoff){ 
		cutoff = newCutoff;
	}
};
