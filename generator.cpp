#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
//0- 0.5*length, 0, 0.5*length ----->>>>
void plane(float length,int divisions, char* file){
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
			fprintf(fd, "%f %f %f\n",ponto1[0],ponto1[1],ponto1[2]);
			fprintf(fd, "%f %f %f\n",ponto1[0]+espacamento,ponto1[1],ponto1[2]);
			fprintf(fd, "%f %f %f\n",ponto1[0]+espacamento,ponto1[1],ponto1[2]-espacamento);

			fprintf(fd, "%f %f %f\n",ponto1[0],ponto1[1],ponto1[2]);
			fprintf(fd, "%f %f %f\n",ponto1[0],ponto1[1],ponto1[2]-espacamento);
			fprintf(fd, "%f %f %f\n",ponto1[0]+espacamento,ponto1[1],ponto1[2]-espacamento);
		}	
	}
	fprintf(fd, "%f\n %d\n", length, divisions);

}
int main(int argc, char** argv){
	if(argv[1]=="plane"){
		plane(atof(argv[2]),atoi(argv[3]), argv[4]);
	}

	return 0;
}