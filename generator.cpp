#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>
//0- 0.5*length, 0, 0.5*length ----->>>>
void plane(float length,int divisions, char* file){
	//nrvertices
	//spamar os vertices todos
	//comprimento do lado
	//nr divisões
	FILE *fd = fopen(file,"w+");
	int vertices = divisions * divisions * 2 * 3;
	fprintf(fd,"%d\n",vertices);
	float espacamento = length/((float)divisions+1);
	for(int i=0; i < divisions;i++){
		float ponto1[3];
		ponto1[0] = 0- 0.5*length;
		ponto1[1] = 0;
		ponto1[2] = 0 + 0.5*length-espacamento*i;
		for(int j=0;j<divisions;j++){
			fprintf(fd, "%f %f %f\n",ponto1[0],ponto1[1], ponto1[2]);
			fprintf(fd, "%f %f %f\n",ponto1[0]+espacamento,ponto1[1],ponto1[2]);
			fprintf(fd, "%f %f %f\n",ponto1[0]+espacamento,ponto1[1],ponto1[2]-espacamento);

			fprintf(fd, "%f %f %f\n",ponto1[0],ponto1[1],ponto1[2]);
			fprintf(fd, "%f %f %f\n",ponto1[0],ponto1[1],ponto1[2]-espacamento);
			fprintf(fd, "%f %f %f\n",ponto1[0]+espacamento,ponto1[1],ponto1[2]-espacamento);
		}	
	}
	fprintf(fd, "%f\n %d\n", length, divisions);

}

void sphere(float radius, int slices, int stacks, char* file){
	//nrvertices
	//spamar os vertices
	//radius
	//slices
	//stacks
	//Cartesian Coordinates
	//𝑝𝑧 = 𝑟 × cos 𝛽 × cos(𝛼);
	//𝑝𝑥 = 𝑟 × cos 𝛽 × sin(𝛼);
	//𝑝𝑦 = 𝑟 × sin(𝛽);
	FILE *fd = fopen(file,"w+");
	int vertices = slices * stacks * 6;
	fprintf(fd, "%d\n", vertices );
	float lat = 2 * M_PI / slices;
	float lon = 2 * M_PI / stacks;
	for(int i=0;i<stacks;i++){
		for(int j=0;j<slices;j++){
			float currentlat = j * lat;
			float currentlon = (-1/2) * M_PI + i * lon;
			fprintf(fd, "%f %f %f\n",radius * cos(currentlat) * sin(currentlon),radius * sin(currentlon),radius * cos(currentlat) * cos(currentlon));
			fprintf(fd, "%f %f %f\n",radius * cos(currentlat) * sin(currentlon),radius * sin(currentlon + lon),radius * cos(currentlat) * cos(currentlon));
			fprintf(fd, "%f %f %f\n",radius * cos(currentlat+lat) * sin(currentlon+lon),radius * sin(currentlon + lon),radius * cos(currentlat+lat) * cos(currentlon+lon));

			fprintf(fd, "%f %f %f\n",radius * cos(currentlat) * sin(currentlon),radius * sin(currentlon),radius * cos(currentlat) * cos(currentlon));
			fprintf(fd, "%f %f %f\n",radius * cos(currentlat+lat) * sin(currentlon),radius * sin(currentlon),radius * cos(currentlat+lat) * cos(currentlon));
			fprintf(fd, "%f %f %f\n",radius * cos(currentlat+lat) * sin(currentlon+lon),radius * sin(currentlon + lon),radius * cos(currentlat+lat) * cos(currentlon+lon));
		}	
	}
	fprintf(fd, "%f\n%d\n%d\n", radius,slices,stacks);
}

int main(int argc, char** argv){
	if(strcmp(argv[1],"plane")){
		plane(atof(argv[2]),atoi(argv[3]), argv[4]);
	}

	return 0;
}