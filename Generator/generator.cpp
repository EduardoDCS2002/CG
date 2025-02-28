#include "generator.hpp"
//0- 0.5*length, 0, 0.5*length ----->>>>
void plane(float length,int divisions, char* file){
	//nrvertices
	//spamar os vertices todos
	//comprimento do lado
	//nr divisões
	FILE *fd = fopen(file,"w+");
	int vertices = divisions * divisions * 2 * 3 * 2;
	fprintf(fd,"%d\n",vertices);
	float espacamento = length/((float)divisions);
	float ponto1[3];
	for(int i=0; i < divisions;i++){
		for(int j=0;j<divisions;j++){
			ponto1[0] = -0.5 * length + j * espacamento;
			ponto1[1] = 0;
			ponto1[2] = 0.5 * length - i * espacamento;
			fprintf(fd, "%f %f %f\n",ponto1[0],ponto1[1], ponto1[2]);
			fprintf(fd, "%f %f %f\n",ponto1[0]+espacamento,ponto1[1],ponto1[2]);
			fprintf(fd, "%f %f %f\n",ponto1[0]+espacamento,ponto1[1],ponto1[2]-espacamento);

			fprintf(fd, "%f %f %f\n",ponto1[0]+espacamento,ponto1[1],ponto1[2]-espacamento);
			fprintf(fd, "%f %f %f\n",ponto1[0],ponto1[1],ponto1[2]-espacamento);
			fprintf(fd, "%f %f %f\n",ponto1[0],ponto1[1],ponto1[2]);

			fprintf(fd, "%f %f %f\n",ponto1[0]+espacamento,ponto1[1],ponto1[2]-espacamento);
			fprintf(fd, "%f %f %f\n",ponto1[0]+espacamento,ponto1[1],ponto1[2]);
			fprintf(fd, "%f %f %f\n",ponto1[0],ponto1[1], ponto1[2]);

			fprintf(fd, "%f %f %f\n",ponto1[0],ponto1[1],ponto1[2]);
			fprintf(fd, "%f %f %f\n",ponto1[0],ponto1[1],ponto1[2]-espacamento);
			fprintf(fd, "%f %f %f\n",ponto1[0]+espacamento,ponto1[1],ponto1[2]-espacamento);
		}	
	}
	fprintf(fd, "%f\n %d\n", length, divisions);
	fclose(fd);
}

void sphere(float radius, int slices, int stacks, char* file){
	FILE *fd = fopen(file,"w+");
	int vertices = 6 * (slices * stacks + slices);
	fprintf(fd, "%d\n", vertices );

	float lat = 2 * M_PI / slices;
	float lon = M_PI / stacks;

	for(int j=0;j<slices;j++){
		float currentlat = j * lat;
		
		//cima
		fprintf(fd, "%f %f %f\n",radius * cos(M_PI / 2) * sin(currentlat),					radius * sin(M_PI / 2),				radius * cos(M_PI /2) * cos(currentlat));
		fprintf(fd, "%f %f %f\n",radius * cos(M_PI / 2 - lon) * sin(currentlat),			radius * sin((M_PI / 2) - lon),		radius * cos((M_PI / 2) - lon) * cos(currentlat));
		fprintf(fd, "%f %f %f\n",radius * cos(M_PI / 2 - lon) * sin(currentlat + lat),		radius * sin((M_PI / 2) - lon),		radius * cos((M_PI / 2) - lon) * cos(currentlat + lat));

		//baixo
		fprintf(fd, "%f %f %f\n",radius * cos(-M_PI / 2) * sin(currentlat),					radius * sin(-M_PI / 2),			radius * cos(-M_PI /2) * cos(currentlat));
		fprintf(fd, "%f %f %f\n",radius * cos((-M_PI / 2) + lon) * sin(currentlat + lat),	radius * sin((-M_PI / 2) + lon),	radius * cos((-M_PI / 2) + lon) * cos(currentlat + lat));
		fprintf(fd, "%f %f %f\n",radius * cos((-M_PI / 2) + lon) * sin(currentlat),			radius * sin((-M_PI / 2) + lon),	radius * cos((-M_PI / 2) + lon) * cos(currentlat));
		for(int i=0;i<stacks;i++){
			float currentlon = M_PI / 2 - (i * lon);	
			
			fprintf(fd, "%f %f %f\n",radius * cos(currentlon) * sin(currentlat),			radius * sin(currentlon),			radius * cos(currentlon) * cos(currentlat));
			fprintf(fd, "%f %f %f\n",radius * cos(currentlon - lon) * sin(currentlat),		radius * sin(currentlon - lon),		radius * cos(currentlon - lon) * cos(currentlat));
			fprintf(fd, "%f %f %f\n",radius * cos(currentlon) * sin(currentlat + lat),		radius * sin(currentlon),			radius * cos(currentlon) * cos(currentlat + lat));

			fprintf(fd, "%f %f %f\n",radius * cos(currentlon - lon) * sin(currentlat),		radius * sin(currentlon - lon),		radius * cos(currentlon-lon) * cos(currentlat));
			fprintf(fd, "%f %f %f\n",radius * cos(currentlon - lon) * sin(currentlat+lat),	radius * sin(currentlon - lon),		radius * cos(currentlon - lon) * cos(currentlat + lat));
			fprintf(fd, "%f %f %f\n",radius * cos(currentlon) * sin(currentlat + lat),		radius * sin(currentlon),			radius * cos(currentlon) * cos(currentlat+lat));
		}
	}
	fprintf(fd, "%f\n%d\n%d\n", radius,slices,stacks);
	fclose(fd);
}

void box(float dimension, int divisions, char* file){
	//requires dimension, and the number of divisions per edge, centred in the origin
	//nrvertices
	//vertices
	FILE *fd = fopen(file, "w+");
	int vertices = divisions * divisions * divisions * 6 * 2;
	fprintf(fd,"%d\n",vertices);
	float espacamento = dimension/((float)divisions);
	float ponto[3];
	//planos no xy
	for(int i=0; i<divisions;i++){
		for(int j=0;j<divisions;j++){
			ponto[0] = -0.5 * dimension + j * espacamento;
			ponto[1] = 0.5 * dimension - i * espacamento;
			ponto[2] = -(dimension/2);
			
			fprintf(fd, "%f %f %f\n",ponto[0]+espacamento,ponto[1]-espacamento,ponto[2]);
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1], ponto[2]);
			fprintf(fd, "%f %f %f\n",ponto[0]+espacamento,ponto[1],ponto[2]);

			fprintf(fd, "%f %f %f\n",ponto[0]+espacamento,ponto[1]-espacamento,ponto[2]);
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1]-espacamento,ponto[2]);
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1],ponto[2]);
			
		}
	}
	for(int i=0; i<divisions;i++){
		for(int j=0;j<divisions;j++){
			ponto[0] = -0.5 * dimension + j * espacamento;
			ponto[1] = 0.5 * dimension - i * espacamento;
			ponto[2] = (dimension/2);

			fprintf(fd, "%f %f %f\n",ponto[0]+espacamento,ponto[1],ponto[2]);
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1], ponto[2]);
			fprintf(fd, "%f %f %f\n",ponto[0]+espacamento,ponto[1]-espacamento,ponto[2]);

			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1],ponto[2]);
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1]-espacamento,ponto[2]);
			fprintf(fd, "%f %f %f\n",ponto[0]+espacamento,ponto[1]-espacamento,ponto[2]);
		}
	}
	
	//planos no xz
	for(int i=0; i < divisions;i++){	//RIGHT
		for(int j=0;j<divisions;j++){
			ponto[0] = -0.5 * dimension + j * espacamento;
			ponto[1] = -(dimension/2);
			ponto[2] = 0.5 * dimension - i * espacamento;
			fprintf(fd, "%f %f %f\n",ponto[0]+espacamento,ponto[1],ponto[2]);
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1], ponto[2]);
			fprintf(fd, "%f %f %f\n",ponto[0]+espacamento,ponto[1],ponto[2]-espacamento);

			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1],ponto[2]);
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1],ponto[2]-espacamento);
			fprintf(fd, "%f %f %f\n",ponto[0]+espacamento,ponto[1],ponto[2]-espacamento);
		}
	}
	for(int i=0; i < divisions;i++){
		for(int j=0;j<divisions;j++){
			ponto[0] = -0.5 * dimension + j * espacamento;
			ponto[1] = dimension/2;
			ponto[2] = 0.5 * dimension - i * espacamento;
			fprintf(fd, "%f %f %f\n",ponto[0]+espacamento,ponto[1],ponto[2]-espacamento);
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1], ponto[2]);
			fprintf(fd, "%f %f %f\n",ponto[0]+espacamento,ponto[1],ponto[2]);

			fprintf(fd, "%f %f %f\n",ponto[0]+espacamento,ponto[1],ponto[2]-espacamento);
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1],ponto[2]-espacamento);
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1],ponto[2]);

		}	
	}
	//planos no yz
	for(int i=0; i < divisions;i++){
		for(int j=0;j<divisions;j++){
			ponto[0] = -(dimension/2);
			ponto[1] = -0.5 * dimension + j * espacamento;
			ponto[2] = 0.5 * dimension - i * espacamento;
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1]+espacamento,ponto[2]-espacamento);
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1], ponto[2]);
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1]+espacamento,ponto[2]);

			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1]+espacamento,ponto[2]-espacamento);
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1],ponto[2]-espacamento);
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1],ponto[2]);

		}	
	}
	for(int i=0; i < divisions;i++){
		for(int j=0;j<divisions;j++){
			ponto[0] = (dimension/2);
			ponto[1] = -0.5 * dimension + j * espacamento;
			ponto[2] = 0.5 * dimension - i * espacamento;
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1]+espacamento,ponto[2]);
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1], ponto[2]);
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1]+espacamento,ponto[2]-espacamento);

			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1],ponto[2]);
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1],ponto[2]-espacamento);
			fprintf(fd, "%f %f %f\n",ponto[0],ponto[1]+espacamento,ponto[2]-espacamento);

		}	
	}
	fclose(fd);
}

int main(int argc, char** argv){
	if(strcmp(argv[1],"plane")==0){
		plane(atof(argv[2]),atof(argv[3]), argv[4]);
	}
    else if(strcmp(argv[1],"sphere") == 0){
        sphere(atof(argv[2]),atof(argv[3]),atof(argv[4]),argv[5]);
    }
	else if(strcmp(argv[1],"box") == 0){
        box(atof(argv[2]),atof(argv[3]), argv[4]);
    }

	return 0;
}