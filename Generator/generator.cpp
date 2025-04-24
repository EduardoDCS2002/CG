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

void torus(float inner_radius, float outer_radius, int slices, int rings, char *file) {
    int vertices = 6 * slices * rings;
    FILE *fd = fopen(file, "w+");
    fprintf(fd, "%d\n", vertices);

    float ring_step = 2 * M_PI / rings;
    float slice_step = 2 * M_PI / slices;

    for (int i = 0; i < rings; i++) {
        float ring_angle = i * ring_step;
        float next_ring_angle = (i + 1) * ring_step;

        for (int j = 0; j < slices; j++) {
            float slice_angle = j * slice_step;
            float next_slice_angle = (j + 1) * slice_step;

            // First triangle
            float x1 = (outer_radius + inner_radius * cos(slice_angle)) * cos(ring_angle);
            float y1 = inner_radius * sin(slice_angle);
            float z1 = (outer_radius + inner_radius * cos(slice_angle)) * sin(ring_angle);

            float x2 = (outer_radius + inner_radius * cos(slice_angle)) * cos(next_ring_angle);
            float y2 = inner_radius * sin(slice_angle);
            float z2 = (outer_radius + inner_radius * cos(slice_angle)) * sin(next_ring_angle);

            float x3 = (outer_radius + inner_radius * cos(next_slice_angle)) * cos(next_ring_angle);
            float y3 = inner_radius * sin(next_slice_angle);
            float z3 = (outer_radius + inner_radius * cos(next_slice_angle)) * sin(next_ring_angle);

            // Second triangle
            float x4 = (outer_radius + inner_radius * cos(next_slice_angle)) * cos(ring_angle);
            float y4 = inner_radius * sin(next_slice_angle);
            float z4 = (outer_radius + inner_radius * cos(next_slice_angle)) * sin(ring_angle);

            
            fprintf(fd, "%f %f %f\n", x1, y1, z1);
            fprintf(fd, "%f %f %f\n", x3, y3, z3);
            fprintf(fd, "%f %f %f\n", x2, y2, z2);

            fprintf(fd, "%f %f %f\n", x1, y1, z1);
            fprintf(fd, "%f %f %f\n", x4, y4, z4);
            fprintf(fd, "%f %f %f\n", x3, y3, z3);
        }
    }
}

float bernstein(int i, float t) {
    switch(i) {
        case 0: return (1 - t) * (1 - t) * (1 - t);
        case 1: return 3 * t * (1 - t) * (1 - t);
        case 2: return 3 * t * t * (1 - t);
        case 3: return t * t * t;
    }
    return 0;
}

Ponto bezierPatchEval(const std::vector<Ponto>& cp, float u, float v) {
    Ponto p;
    for (int i = 0; i < 4; ++i) {
        float bu = bernstein(i, u);
        for (int j = 0; j < 4; ++j) {
            float bv = bernstein(j, v);
            Ponto temp = cp[i * 4 + j];
            float x = p.getX() + bu * bv * temp.getX();
            float y = p.getY() + bu * bv * temp.getY();
            float z = p.getZ() + bu * bv * temp.getZ();
            p.setX(x);
            p.setY(y);
            p.setZ(z);
        }
    }
    return p;
}

void bezier(char* file_in, int tessellation, char* file_out) {
    std::ifstream infile(file_in);
    std::ofstream outfile(file_out);
    std::vector<std::vector<int>> patches;
    std::vector<Ponto> controlPoints;

    int patchCount;
    infile >> patchCount;

    for (int i = 0; i < patchCount; ++i) {
		std::string line;
		std::getline(infile >> std::ws, line); // lê linha, ignorando espaços em branco
		for (char& c : line) {
			if (c == ',') c = ' ';
		}
		std::stringstream ss(line);
		std::vector<int> patch(16);
		for (int j = 0; j < 16; ++j) {
			ss >> patch[j];
		}
		patches.push_back(patch);
	}
	

    std::string line;
	while (std::getline(infile, line)) {
		for (char& c : line) {
			if (c == ',') c = ' ';
		}
		std::stringstream ss(line);
		float x, y, z;
		if (ss >> x >> y >> z) {
			controlPoints.emplace_back(x, y, z);
		}
	}

    std::vector<Ponto> vertices;

    for (const auto& patch : patches) {
        std::vector<Ponto> patchPoints;
        for (int idx : patch) {
			if (idx < 0 || idx >= (int)controlPoints.size()) {
				std::cerr << " Erro: índice fora do intervalo: " << idx
						  << " (tamanho dos pontos: " << controlPoints.size() << ")\n";
				exit(EXIT_FAILURE);
			}
			patchPoints.push_back(controlPoints[idx]);
		}
		

        for (int i = 0; i < tessellation; ++i) {
            float u = (float)i / tessellation;
            float u_next = (float)(i + 1) / tessellation;
            for (int j = 0; j < tessellation; ++j) {
                float v = (float)j / tessellation;
                float v_next = (float)(j + 1) / tessellation;

                Ponto p1 = bezierPatchEval(patchPoints, u, v);
                Ponto p2 = bezierPatchEval(patchPoints, u_next, v);
                Ponto p3 = bezierPatchEval(patchPoints, u, v_next);
                Ponto p4 = bezierPatchEval(patchPoints, u_next, v_next);

                // Triângulo 1
                vertices.push_back(p1);
                vertices.push_back(p4);
                vertices.push_back(p2);

                // Triângulo 2
                vertices.push_back(p1);
                vertices.push_back(p3);
                vertices.push_back(p4);
            }
        }
    }

    outfile << vertices.size() << "\n";
    for (Ponto p : vertices) {
        outfile << p.getX() << " " << p.getY() << " " << p.getZ() << "\n";
    }

    infile.close();
    outfile.close();
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
	else if(strcmp(argv[1],"torus") == 0){
        torus(atof(argv[2]),atof(argv[3]), atof(argv[4]), atof(argv[5]), argv[6]);
    }
	else if (strcmp(argv[1],"bezier") == 0){
		bezier(argv[2], atoi(argv[3]), argv[4]);
	}
	return 0;
}