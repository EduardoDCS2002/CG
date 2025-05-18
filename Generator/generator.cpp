#include "generator.hpp"
//0- 0.5*length, 0, 0.5*length ----->>>>
void plane(float length,int divisions, char* file){
	FILE *fd = fopen(file,"w+");
	int vertices = divisions * divisions * 2 * 3 * 2;
	fprintf(fd,"%d\n",vertices);
	float espacamento = length/((float)divisions);
	float ponto1[3];
	std::vector<Ponto> vertex;
    std::vector<Ponto> normals;
    std::vector<Ponto> texture;
	for(int i=0; i < divisions;i++){
		for(int j=0;j<divisions;j++){
			ponto1[0] = -0.5 * length + j * espacamento;
			ponto1[1] = 0;
			ponto1[2] = 0.5 * length - i * espacamento;

			vertex.emplace_back(ponto1[0], ponto1[1], ponto1[2]);
			vertex.emplace_back(ponto1[0]+espacamento,ponto1[1],ponto1[2]);
			vertex.emplace_back(ponto1[0]+espacamento,ponto1[1],ponto1[2]-espacamento);

			texture.emplace_back((float)j / divisions, 1.0f - (float)i / divisions, 0);
			texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)i / divisions, 0);
			texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)(i + 1) / divisions, 0);

			vertex.emplace_back(ponto1[0]+espacamento,ponto1[1],ponto1[2]-espacamento);
			vertex.emplace_back(ponto1[0],ponto1[1],ponto1[2]-espacamento);
			vertex.emplace_back(ponto1[0],ponto1[1],ponto1[2]);

			texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)(i + 1) / divisions, 0);
			texture.emplace_back((float)j / divisions, 1.0f - (float)(i + 1) / divisions, 0);
			texture.emplace_back((float)j / divisions, 1.0f - (float)i / divisions, 0);

			for (int k = 0; k < 6; ++k)
                normals.emplace_back(0.0f, 1.0f, 0.0f);

			//parte de baixo
			vertex.emplace_back(ponto1[0]+espacamento,ponto1[1],ponto1[2]-espacamento);
			vertex.emplace_back(ponto1[0]+espacamento,ponto1[1],ponto1[2]);
			vertex.emplace_back(ponto1[0],ponto1[1],ponto1[2]);

			texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)(i + 1) / divisions, 0);
			texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)i / divisions, 0);
			texture.emplace_back((float)j / divisions, 1.0f - (float)i / divisions, 0);

			vertex.emplace_back(ponto1[0],ponto1[1],ponto1[2]);
			vertex.emplace_back(ponto1[0],ponto1[1],ponto1[2]-espacamento);
			vertex.emplace_back(ponto1[0]+espacamento,ponto1[1],ponto1[2]-espacamento);

			texture.emplace_back((float)j / divisions, 1.0f - (float)i / divisions, 0);
			texture.emplace_back((float)j / divisions, 1.0f - (float)(i + 1) / divisions, 0);
			texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)(i + 1) / divisions, 0);

			for (int k = 0; k < 6; ++k)
                normals.emplace_back(0.0f, -1.0f, 0.0f);
		}	
	}

    for (auto& v : vertex)
        fprintf(fd, "%f %f %f\n", v.getX(), v.getY(), v.getZ());

    for (auto& n : normals)
        fprintf(fd, "%f %f %f\n", n.getX(), n.getY(), n.getZ());

    for (auto& t : texture)
        fprintf(fd, "%f %f\n", t.getX(), t.getY());
	fclose(fd);
}


void sphere(float radius, int slices, int stacks, char* file) {
    FILE* fd = fopen(file, "w+");
    int vertices = slices * (6 + (stacks * 6));
    fprintf(fd, "%d\n", vertices);

    std::vector<Ponto> vertex;
    std::vector<Ponto> normals;
    std::vector<Ponto> texture;

    float lat = 2 * M_PI / slices;
    float lon = M_PI / stacks;

    for (int j = 0; j < slices; j++) {
        float currentlat = j * lat;

        // cima
        vertex.emplace_back(radius * cos(M_PI / 2) * sin(currentlat), radius * sin(M_PI / 2), radius * cos(M_PI / 2) * cos(currentlat));
        normals.emplace_back(cos(M_PI / 2) * sin(currentlat), sin(M_PI / 2), cos(M_PI / 2) * cos(currentlat));
        texture.emplace_back((float)j / slices, 1.0f - 0.0f, 0);

        vertex.emplace_back(radius * cos(M_PI / 2 - lon) * sin(currentlat), radius * sin(M_PI / 2 - lon), radius * cos(M_PI / 2 - lon) * cos(currentlat));
        normals.emplace_back(cos(M_PI / 2 - lon) * sin(currentlat), sin(M_PI / 2 - lon), cos(M_PI / 2 - lon) * cos(currentlat));
        texture.emplace_back((float)j / slices, 1.0f - (1.0f / stacks), 0);

        vertex.emplace_back(radius * cos(M_PI / 2 - lon) * sin(currentlat + lat), radius * sin(M_PI / 2 - lon), radius * cos(M_PI / 2 - lon) * cos(currentlat + lat));
        normals.emplace_back(cos(M_PI / 2 - lon) * sin(currentlat + lat), sin(M_PI / 2 - lon), cos(M_PI / 2 - lon) * cos(currentlat + lat));
        texture.emplace_back((float)(j + 1) / slices, 1.0f - (1.0f / stacks), 0);

        // baixo
        vertex.emplace_back(radius * cos(-M_PI / 2) * sin(currentlat), radius * sin(-M_PI / 2), radius * cos(-M_PI / 2) * cos(currentlat));
        normals.emplace_back(cos(-M_PI / 2) * sin(currentlat), sin(-M_PI / 2), cos(-M_PI / 2) * cos(currentlat));
        texture.emplace_back((float)j / slices, 1.0f - 1.0f, 0);

        vertex.emplace_back(radius * cos(-M_PI / 2 + lon) * sin(currentlat + lat), radius * sin(-M_PI / 2 + lon), radius * cos(-M_PI / 2 + lon) * cos(currentlat + lat));
        normals.emplace_back(cos(-M_PI / 2 + lon) * sin(currentlat + lat), sin(-M_PI / 2 + lon), cos(-M_PI / 2 + lon) * cos(currentlat + lat));
        texture.emplace_back((float)(j + 1) / slices, 1.0f - (1.0f - 1.0f / stacks), 0);

        vertex.emplace_back(radius * cos(-M_PI / 2 + lon) * sin(currentlat), radius * sin(-M_PI / 2 + lon), radius * cos(-M_PI / 2 + lon) * cos(currentlat));
        normals.emplace_back(cos(-M_PI / 2 + lon) * sin(currentlat), sin(-M_PI / 2 + lon), cos(-M_PI / 2 + lon) * cos(currentlat));
        texture.emplace_back((float)j / slices, 1.0f - (1.0f - 1.0f / stacks), 0);

        // stacks do meio
        for (int i = 0; i < stacks; i++) {
            float currentlon = M_PI / 2 - (i * lon);

            vertex.emplace_back(radius * cos(currentlon) * sin(currentlat), radius * sin(currentlon), radius * cos(currentlon) * cos(currentlat));
            normals.emplace_back(cos(currentlon) * sin(currentlat), sin(currentlon), cos(currentlon) * cos(currentlat));
            texture.emplace_back((float)j / slices, 1.0f - (float)i / stacks, 0);

            vertex.emplace_back(radius * cos(currentlon - lon) * sin(currentlat), radius * sin(currentlon - lon), radius * cos(currentlon - lon) * cos(currentlat));
            normals.emplace_back(cos(currentlon - lon) * sin(currentlat), sin(currentlon - lon), cos(currentlon - lon) * cos(currentlat));
            texture.emplace_back((float)j / slices, 1.0f - (float)(i + 1) / stacks, 0);

            vertex.emplace_back(radius * cos(currentlon) * sin(currentlat + lat), radius * sin(currentlon), radius * cos(currentlon) * cos(currentlat + lat));
            normals.emplace_back(cos(currentlon) * sin(currentlat + lat), sin(currentlon), cos(currentlon) * cos(currentlat + lat));
            texture.emplace_back((float)(j + 1) / slices, 1.0f - (float)i / stacks, 0);

            vertex.emplace_back(radius * cos(currentlon - lon) * sin(currentlat), radius * sin(currentlon - lon), radius * cos(currentlon - lon) * cos(currentlat));
            normals.emplace_back(cos(currentlon - lon) * sin(currentlat), sin(currentlon - lon), cos(currentlon - lon) * cos(currentlat));
            texture.emplace_back((float)j / slices, 1.0f - (float)(i + 1) / stacks, 0);

            vertex.emplace_back(radius * cos(currentlon - lon) * sin(currentlat + lat), radius * sin(currentlon - lon), radius * cos(currentlon - lon) * cos(currentlat + lat));
            normals.emplace_back(cos(currentlon - lon) * sin(currentlat + lat), sin(currentlon - lon), cos(currentlon - lon) * cos(currentlat + lat));
            texture.emplace_back((float)(j + 1) / slices, 1.0f - (float)(i + 1) / stacks, 0);

            vertex.emplace_back(radius * cos(currentlon) * sin(currentlat + lat), radius * sin(currentlon), radius * cos(currentlon) * cos(currentlat + lat));
            normals.emplace_back(cos(currentlon) * sin(currentlat + lat), sin(currentlon), cos(currentlon) * cos(currentlat + lat));
            texture.emplace_back((float)(j + 1) / slices, 1.0f - (float)i / stacks, 0);
        }
    }

    for (auto& v : vertex) 
		fprintf(fd, "%f %f %f\n", v.getX(), v.getY(), v.getZ());

    for (auto& n : normals) 
		fprintf(fd, "%f %f %f\n", n.getX(), n.getY(), n.getZ());

    for (auto& t : texture) 
		fprintf(fd, "%f %f\n", t.getX(), t.getY());

    fclose(fd);
}



// plane() e sphere() já atualizadas

void box(float dimension, int divisions, char* file){
	FILE *fd = fopen(file, "w+");
	int vertices = divisions * divisions * 2 * 3 * 6;
	fprintf(fd,"%d\n",vertices);
	float espacamento = dimension/((float)divisions);
	float ponto[3];
	std::vector<Ponto> vertex;
    std::vector<Ponto> normals;
    std::vector<Ponto> texture;

	// planos no xy
	for(int i=0; i<divisions;i++){
		for(int j=0;j<divisions;j++){
			ponto[0] = -0.5 * dimension + j * espacamento;
			ponto[1] = 0.5 * dimension - i * espacamento;
			ponto[2] = -(dimension/2);

            vertex.emplace_back(ponto[0]+espacamento, ponto[1]-espacamento, ponto[2]);
            vertex.emplace_back(ponto[0], ponto[1], ponto[2]);
            vertex.emplace_back(ponto[0]+espacamento, ponto[1], ponto[2]);

            texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)(i + 1) / divisions, 0);
            texture.emplace_back((float)j / divisions, 1.0f - (float)i / divisions, 0);
            texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)i / divisions, 0);

            vertex.emplace_back(ponto[0]+espacamento, ponto[1]-espacamento, ponto[2]);
            vertex.emplace_back(ponto[0], ponto[1]-espacamento, ponto[2]);
            vertex.emplace_back(ponto[0], ponto[1], ponto[2]);

            texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)(i + 1) / divisions, 0);
            texture.emplace_back((float)j / divisions, 1.0f - (float)(i + 1) / divisions, 0);
            texture.emplace_back((float)j / divisions, 1.0f - (float)i / divisions, 0);

            for (int k = 0; k < 6; k++)
                normals.emplace_back(0.0f, 0.0f, -1.0f);
		}
	}
	for(int i=0; i<divisions;i++){
		for(int j=0;j<divisions;j++){
			ponto[0] = -0.5 * dimension + j * espacamento;
			ponto[1] = 0.5 * dimension - i * espacamento;
			ponto[2] = (dimension/2);

            vertex.emplace_back(ponto[0]+espacamento, ponto[1], ponto[2]);
            vertex.emplace_back(ponto[0], ponto[1], ponto[2]);
            vertex.emplace_back(ponto[0]+espacamento, ponto[1]-espacamento, ponto[2]);

            texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)i / divisions, 0);
            texture.emplace_back((float)j / divisions, 1.0f - (float)i / divisions, 0);
            texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)(i + 1) / divisions, 0);

            vertex.emplace_back(ponto[0], ponto[1], ponto[2]);
            vertex.emplace_back(ponto[0], ponto[1]-espacamento, ponto[2]);
            vertex.emplace_back(ponto[0]+espacamento, ponto[1]-espacamento, ponto[2]);

            texture.emplace_back((float)j / divisions, 1.0f - (float)i / divisions, 0);
            texture.emplace_back((float)j / divisions, 1.0f - (float)(i + 1) / divisions, 0);
            texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)(i + 1) / divisions, 0);

            for (int k = 0; k < 6; k++)
                normals.emplace_back(0.0f, 0.0f, 1.0f);
		}
	}
	// planos no xz inferior
	for(int i=0; i<divisions;i++){
		for(int j=0;j<divisions;j++){
			ponto[0] = -0.5 * dimension + j * espacamento;
			ponto[1] = -(dimension/2);
			ponto[2] = 0.5 * dimension - i * espacamento;

            vertex.emplace_back(ponto[0]+espacamento, ponto[1], ponto[2]);
            vertex.emplace_back(ponto[0], ponto[1], ponto[2]);
            vertex.emplace_back(ponto[0]+espacamento, ponto[1], ponto[2]-espacamento);

            texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)i / divisions, 0);
            texture.emplace_back((float)j / divisions, 1.0f - (float)i / divisions, 0);
            texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)(i + 1) / divisions, 0);

            vertex.emplace_back(ponto[0], ponto[1], ponto[2]);
            vertex.emplace_back(ponto[0], ponto[1], ponto[2]-espacamento);
            vertex.emplace_back(ponto[0]+espacamento, ponto[1], ponto[2]-espacamento);

            texture.emplace_back((float)j / divisions, 1.0f - (float)i / divisions, 0);
            texture.emplace_back((float)j / divisions, 1.0f - (float)(i + 1) / divisions, 0);
            texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)(i + 1) / divisions, 0);

            for (int k = 0; k < 6; k++)
                normals.emplace_back(0.0f, -1.0f, 0.0f);
		}
	}
	// planos no xz superior
	for(int i=0; i<divisions;i++){
		for(int j=0;j<divisions;j++){
			ponto[0] = -0.5 * dimension + j * espacamento;
			ponto[1] = dimension/2;
			ponto[2] = 0.5 * dimension - i * espacamento;

            vertex.emplace_back(ponto[0]+espacamento, ponto[1], ponto[2]-espacamento);
            vertex.emplace_back(ponto[0], ponto[1], ponto[2]);
            vertex.emplace_back(ponto[0]+espacamento, ponto[1], ponto[2]);

            texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)(i + 1) / divisions, 0);
            texture.emplace_back((float)j / divisions, 1.0f - (float)i / divisions, 0);
            texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)i / divisions, 0);

            vertex.emplace_back(ponto[0]+espacamento, ponto[1], ponto[2]-espacamento);
            vertex.emplace_back(ponto[0], ponto[1], ponto[2]-espacamento);
            vertex.emplace_back(ponto[0], ponto[1], ponto[2]);

            texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)(i + 1) / divisions, 0);
            texture.emplace_back((float)j / divisions, 1.0f - (float)(i + 1) / divisions, 0);
            texture.emplace_back((float)j / divisions, 1.0f - (float)i / divisions, 0);

            for (int k = 0; k < 6; k++)
                normals.emplace_back(0.0f, 1.0f, 0.0f);
		}
	}
	// planos no yz esquerdo
	for(int i=0; i<divisions;i++){
		for(int j=0;j<divisions;j++){
			ponto[0] = -(dimension/2);
			ponto[1] = -0.5 * dimension + j * espacamento;
			ponto[2] = 0.5 * dimension - i * espacamento;

            vertex.emplace_back(ponto[0], ponto[1]+espacamento, ponto[2]-espacamento);
            vertex.emplace_back(ponto[0], ponto[1], ponto[2]);
            vertex.emplace_back(ponto[0], ponto[1]+espacamento, ponto[2]);

            texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)(i + 1) / divisions, 0);
            texture.emplace_back((float)j / divisions, 1.0f - (float)i / divisions, 0);
            texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)i / divisions, 0);

            vertex.emplace_back(ponto[0], ponto[1]+espacamento, ponto[2]-espacamento);
            vertex.emplace_back(ponto[0], ponto[1], ponto[2]-espacamento);
            vertex.emplace_back(ponto[0], ponto[1], ponto[2]);

            texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)(i + 1) / divisions, 0);
            texture.emplace_back((float)j / divisions, 1.0f - (float)(i + 1) / divisions, 0);
            texture.emplace_back((float)j / divisions, 1.0f - (float)i / divisions, 0);

            for (int k = 0; k < 6; k++)
                normals.emplace_back(-1.0f, 0.0f, 0.0f);
		}
	}
	// planos no yz direito
	for(int i=0; i<divisions;i++){
		for(int j=0;j<divisions;j++){
			ponto[0] = (dimension/2);
			ponto[1] = -0.5 * dimension + j * espacamento;
			ponto[2] = 0.5 * dimension - i * espacamento;

            vertex.emplace_back(ponto[0], ponto[1]+espacamento, ponto[2]);
            vertex.emplace_back(ponto[0], ponto[1], ponto[2]);
            vertex.emplace_back(ponto[0], ponto[1]+espacamento, ponto[2]-espacamento);

            texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)i / divisions, 0);
            texture.emplace_back((float)j / divisions, 1.0f - (float)i / divisions, 0);
            texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)(i + 1) / divisions, 0);

            vertex.emplace_back(ponto[0], ponto[1], ponto[2]);
            vertex.emplace_back(ponto[0], ponto[1], ponto[2]-espacamento);
            vertex.emplace_back(ponto[0], ponto[1]+espacamento, ponto[2]-espacamento);

            texture.emplace_back((float)j / divisions, 1.0f - (float)i / divisions, 0);
            texture.emplace_back((float)j / divisions, 1.0f - (float)(i + 1) / divisions, 0);
            texture.emplace_back((float)(j + 1) / divisions, 1.0f - (float)(i + 1) / divisions, 0);

            for (int k = 0; k < 6; k++)
                normals.emplace_back(1.0f, 0.0f, 0.0f);
		}
	}

    for (auto& v : vertex)
        fprintf(fd, "%f %f %f\n", v.getX(), v.getY(), v.getZ());

    for (auto& n : normals)
        fprintf(fd, "%f %f %f\n", n.getX(), n.getY(), n.getZ());

    for (auto& t : texture)
        fprintf(fd, "%f %f\n", t.getX(), t.getY());

	fclose(fd);
}


void cone(float radius, float height, int slices, int stacks, char *file){
	int vertices = 6 * stacks * slices + 3 * slices;
	FILE *fd = fopen(file, "w+");
	fprintf(fd,"%d\n", vertices);
	float angle = 2 * M_PI / slices;
	float height_change = height / stacks;
    float radius_change = radius / stacks;
	std::vector<Ponto> vertex;
	std::vector<Ponto> normals;
	std::vector<Ponto> texture;
	
	for (int i = 0; i < stacks; i++) {
        float current_height = height - height_change * i;
        float current_radius = i * radius_change;
        for (int j = 0; j < slices; j++) {
			vertex.emplace_back(current_radius * sin(angle * j), current_height, current_radius * cos(angle * j));
			vertex.emplace_back((current_radius+radius_change) * sin(angle * j), current_height - height_change, (current_radius+radius_change) * cos(angle * j));
			vertex.emplace_back((current_radius+radius_change) * sin(angle * (j+1)), current_height - height_change, (current_radius+radius_change) * cos(angle * (j+1)));
			
			texture.emplace_back((float)j / slices, 1.0f - (float)i / stacks, 0);
			texture.emplace_back((float)j / slices, 1.0f - (float)(i + 1) / stacks, 0);
			texture.emplace_back((float)(j + 1) / slices, 1.0f - (float)(i + 1) / stacks, 0);

			vertex.emplace_back(current_radius * sin(angle * j), current_height, current_radius * cos(angle * j));
			vertex.emplace_back((current_radius+radius_change) * sin(angle * (j+1)), current_height - height_change, (current_radius+radius_change) * cos(angle * (j+1)));
			vertex.emplace_back(current_radius * sin(angle * (j+1)), current_height, current_radius * cos(angle * (j+1)));

			texture.emplace_back((float)j / slices, 1.0f - (float)i / stacks, 0);
			texture.emplace_back((float)(j + 1) / slices, 1.0f - (float)(i + 1) / stacks, 0);
			texture.emplace_back((float)(j + 1) / slices, 1.0f - (float)i / stacks, 0);

			for (int k = 6; k > 0; k--) {
				Ponto& v = vertex[vertex.size() - k];
				float x = v.getX();
				float y = radius / height;
				float z = v.getZ();
				float len = sqrt(x * x + y * y + z * z);
				normals.emplace_back(x / len, y / len, z / len);
			}
        }
    }

	// base
	for(int i = 0;i < slices; i++){
		vertex.emplace_back(radius * sin(angle * i), 0.0f, radius * cos(angle * i));
		vertex.emplace_back(0.0f, 0.0f, 0.0f);
		vertex.emplace_back(radius * sin(angle * (i+1)), 0.0f, radius * cos(angle * (i+1)));

		texture.emplace_back(0.5f + 0.5f * sin(angle * i), 1.0f - (0.5f + 0.5f * cos(angle * i)), 0);
		texture.emplace_back(0.5f, 1.0f - 0.5f, 0);
		texture.emplace_back(0.5f + 0.5f * sin(angle * (i+1)), 1.0f - (0.5f + 0.5f * cos(angle * (i+1))), 0);

		for (int j = 0; j < 3; j++)
			normals.emplace_back(0.0f, -1.0f, 0.0f);
	}

	for (auto& v : vertex)
		fprintf(fd, "%f %f %f\n", v.getX(), v.getY(), v.getZ());

	for (auto& n : normals)
		fprintf(fd, "%f %f %f\n", n.getX(), n.getY(), n.getZ());

	for (auto& t : texture)
		fprintf(fd, "%f %f\n", t.getX(), t.getY());

	fclose(fd);
}

// plane(), sphere(), box() e cone() já atualizadas

void torus(float innerRadius, float outerRadius, int slices, int stacks, char* file) {
	FILE* fd = fopen(file, "w+");
	int vertices = slices * stacks * 6;
	fprintf(fd, "%d\n", vertices);

	std::vector<Ponto> vertex;
	std::vector<Ponto> normals;
	std::vector<Ponto> texture;

	float alpha = 2 * M_PI / slices;
	float beta = 2 * M_PI / stacks;
	float r = (outerRadius - innerRadius) / 2.0f;
	float c = innerRadius + r;

	for (int i = 0; i < slices; i++) {
		float currentAlpha = i * alpha;
		float nextAlpha = (i + 1) * alpha;

		for (int j = 0; j < stacks; j++) {
			float currentBeta = j * beta;
			float nextBeta = (j + 1) * beta;

			Ponto p1((c + r * cos(currentBeta)) * cos(currentAlpha), r * sin(currentBeta), (c + r * cos(currentBeta)) * sin(currentAlpha));
			Ponto p2((c + r * cos(currentBeta)) * cos(nextAlpha), r * sin(currentBeta), (c + r * cos(currentBeta)) * sin(nextAlpha));
			Ponto p3((c + r * cos(nextBeta)) * cos(nextAlpha), r * sin(nextBeta), (c + r * cos(nextBeta)) * sin(nextAlpha));
			Ponto p4((c + r * cos(nextBeta)) * cos(currentAlpha), r * sin(nextBeta), (c + r * cos(nextBeta)) * sin(currentAlpha));

			vertex.push_back(p1);
			vertex.push_back(p2);
			vertex.push_back(p3);

			vertex.push_back(p1);
			vertex.push_back(p3);
			vertex.push_back(p4);

			normals.push_back(Ponto(cos(currentBeta) * cos(currentAlpha), sin(currentBeta), cos(currentBeta) * sin(currentAlpha)).normalize());
			normals.push_back(Ponto(cos(currentBeta) * cos(nextAlpha), sin(currentBeta), cos(currentBeta) * sin(nextAlpha)).normalize());
			normals.push_back(Ponto(cos(nextBeta) * cos(nextAlpha), sin(nextBeta), cos(nextBeta) * sin(nextAlpha)).normalize());

			normals.push_back(Ponto(cos(currentBeta) * cos(currentAlpha), sin(currentBeta), cos(currentBeta) * sin(currentAlpha)).normalize());
			normals.push_back(Ponto(cos(nextBeta) * cos(nextAlpha), sin(nextBeta), cos(nextBeta) * sin(nextAlpha)).normalize());
			normals.push_back(Ponto(cos(nextBeta) * cos(currentAlpha), sin(nextBeta), cos(nextBeta) * sin(currentAlpha)).normalize());

			texture.emplace_back((float)i / slices, 1.0f - (float)j / stacks, 0);
			texture.emplace_back((float)(i + 1) / slices, 1.0f - (float)j / stacks, 0);
			texture.emplace_back((float)(i + 1) / slices, 1.0f - (float)(j + 1) / stacks, 0);

			texture.emplace_back((float)i / slices, 1.0f - (float)j / stacks, 0);
			texture.emplace_back((float)(i + 1) / slices, 1.0f - (float)(j + 1) / stacks, 0);
			texture.emplace_back((float)i / slices, 1.0f - (float)(j + 1) / stacks, 0);
		}
	}

	for (auto& v : vertex)
		fprintf(fd, "%f %f %f\n", v.getX(), v.getY(), v.getZ());
	for (auto& n : normals)
		fprintf(fd, "%f %f %f\n", n.getX(), n.getY(), n.getZ());
	for (auto& t : texture)
		fprintf(fd, "%f %f\n", t.getX(), t.getY());

	fclose(fd);
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

// plane(), sphere(), box(), cone() e torus() já atualizadas

void bezier(char* file_in, int tessellation, char* file_out) {
    std::ifstream infile(file_in);
    FILE* outfile = fopen(file_out, "w+");

    std::vector<std::vector<int>> patches;
    std::vector<Ponto> controlPoints;

    int patchCount;
    infile >> patchCount;

    for (int i = 0; i < patchCount; ++i) {
        std::string line;
        std::getline(infile >> std::ws, line);
        for (char& c : line) if (c == ',') c = ' ';
        std::stringstream ss(line);
        std::vector<int> patch(16);
        for (int j = 0; j < 16; ++j) ss >> patch[j];
        patches.push_back(patch);
    }

    std::string line;
    while (std::getline(infile, line)) {
        for (char& c : line) if (c == ',') c = ' ';
        std::stringstream ss(line);
        float x, y, z;
        if (ss >> x >> y >> z) controlPoints.emplace_back(x, y, z);
    }

    std::vector<Ponto> vertices;
    std::vector<Ponto> normals;
    std::vector<Ponto> textures;

    for (const auto& patch : patches) {
        std::vector<Ponto> patchPoints;
        for (int idx : patch) patchPoints.push_back(controlPoints[idx]);

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

                Ponto t1(u, 1.0f - v, 0), t2(u_next, 1.0f - v, 0), t3(u, 1.0f - v_next, 0), t4(u_next, 1.0f - v_next, 0);

                // Triangle 1
                Ponto u1 = p4 - p1;
                Ponto v1 = p2 - p1;
                Ponto n1 = u1.cross(v1).normalize();

                vertices.push_back(p1); normals.push_back(n1); textures.push_back(t1);
                vertices.push_back(p4); normals.push_back(n1); textures.push_back(t4);
                vertices.push_back(p2); normals.push_back(n1); textures.push_back(t2);

                // Triangle 2
                Ponto u2 = p3 - p1;
                Ponto v2 = p4 - p1;
                Ponto n2 = u2.cross(v2).normalize();

                vertices.push_back(p1); normals.push_back(n2); textures.push_back(t1);
                vertices.push_back(p3); normals.push_back(n2); textures.push_back(t3);
                vertices.push_back(p4); normals.push_back(n2); textures.push_back(t4);
            }
        }
    }

    fprintf(outfile, "%lu\n", vertices.size());
    for (const auto& v : vertices) 
		fprintf(outfile, "%f %f %f\n", v.getX(), v.getY(), v.getZ());

    for (const auto& n : normals) 
		fprintf(outfile, "%f %f %f\n", n.getX(), n.getY(), n.getZ());
    for (const auto& t : textures) 
		fprintf(outfile, "%f %f\n", t.getX(), t.getY());

    infile.close();
    fclose(outfile);
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