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
	fclose(fd);
}

void sphere(float radius, int slices, int stacks, char* file){
	FILE *fd = fopen(file,"w+");
	int vertices = slices * (6 + (stacks * 6));  
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
	fclose(fd);
}

void box(float dimension, int divisions, char* file){
	//requires dimension, and the number of divisions per edge, centred in the origin
	//nrvertices
	//vertices
	FILE *fd = fopen(file, "w+");
	int vertices = divisions * divisions * 2 * 3 * 6;
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

void cone(float radius, float height, int slices, int stacks, char *file){
	//nrvertices
	int vertices = 6 * stacks * slices + 3 * slices;
	FILE *fd = fopen(file, "w+");
	fprintf(fd,"%d\n", vertices);
	float angle = 2 * M_PI / slices;
	float height_change = height / stacks;
    float radius_change = radius / stacks;
	//faces
	for (int i = 0; i < stacks; i++) {
        float current_height = height - height_change * i;
        float current_radius = i * radius_change;
        for (int j = 0; j < slices; j++) {
			fprintf(fd, "%f %f %f\n",current_radius * sin(angle * j),current_height,current_radius * cos(angle * j));
			fprintf(fd, "%f %f %f\n",(current_radius+radius_change) * sin(angle * j),current_height-height_change,(current_radius+radius_change) * cos(angle * j));
			fprintf(fd, "%f %f %f\n",(current_radius+radius_change) * sin(angle * (j+1)),current_height-height_change,(current_radius+radius_change) * cos(angle * (j+1)));

			fprintf(fd, "%f %f %f\n",current_radius * sin(angle * j),current_height,current_radius * cos(angle * j));
			fprintf(fd, "%f %f %f\n",(current_radius+radius_change) * sin(angle * (j+1)),current_height-height_change,(current_radius+radius_change) * cos(angle * (j+1)));
			fprintf(fd, "%f %f %f\n",current_radius * sin(angle * (j+1)),current_height,current_radius * cos(angle * (j+1)));
        }
    }
	//circunferencia
	for(int i = 0;i < slices; i++){
		fprintf(fd, "%f %f %f\n",radius * sin(angle * i),0.0,radius * cos(angle * i));
		fprintf(fd, "%f %f %f\n",0.0,0.0,0.0);
		fprintf(fd, "%f %f %f\n",radius * sin(angle * (i+1)),0.0,radius * cos(angle * (i+1)));
	}
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
	else if(strcmp(argv[1],"cone") == 0){
        cone(atof(argv[2]),atof(argv[3]), atof(argv[4]), atof(argv[5]), argv[6]);
    }
	return 0;
}