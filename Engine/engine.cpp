#include "engine.hpp"
#include <map>
list<Group*> mainGrupos;
vector<Light> lightsVector;
map<string, GLuint> texturesMap;
int nrgroupcounter = 0;
float camX, camY, camZ;  //posicao x,y,z da camara
int alpha = 0, beta1 = 0, r = 20;      // angulos e raio da camara
int xInicial, yInicial, modoRato = 0;   //posicoes anteriores da camara e modo da mesma
double lookX;
double lookY;
double lookZ;
double upX;
double upY;
double upZ;
int fov;
int near;
double far;
int typeDraw = GL_FILL;

int timebase = 0;
float frame = 0;
float fps;

//VBO stuff
//GLuint vertices, verticeCount;
//GLuint indices;
//unsigned int indexCount;

void readFile(string caminho3d, Models& model) {
	string linha;
	ifstream file(caminho3d);
	if (file.is_open()) {
		getline(file, linha);                   
		int nLinhas = atoi(linha.c_str());
		//list<Ponto> pontos;
		vector<float> vertexData, normalData, textureData;

		//VERTEX
		for (int i = 1; i <= nLinhas; i++) {
			getline(file, linha);
			stringstream ss(linha);		
			vector<string> result{
				istream_iterator<string>(ss), {}
			};
			
			vertexData.push_back(stof(result[0]));
			vertexData.push_back(stof(result[1]));
			vertexData.push_back(stof(result[2]));
		}
		
		//NORMALS
		for (int i = 1; i <= nLinhas; i++) {
			getline(file, linha);
			stringstream ss(linha);		
			vector<string> result{
				istream_iterator<string>(ss), {}
			};
			
			normalData.push_back(stof(result[0]));
			normalData.push_back(stof(result[1]));
			normalData.push_back(stof(result[2]));
		}

		//TEXTURE
		for (int i = 1; i <= nLinhas; i++) {
			getline(file, linha);
			stringstream ss(linha);		
			vector<string> result{
				istream_iterator<string>(ss), {}
			};
			
			textureData.push_back(stof(result[0]));
			textureData.push_back(stof(result[1]));
		}

		//group->setPontos(pontos);
		GLuint vboVertex;
		glGenBuffers(1, &vboVertex);
		glBindBuffer(GL_ARRAY_BUFFER, vboVertex);
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

		GLuint vboNormals;
		glGenBuffers(1, &vboNormals);
		glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
		glBufferData(GL_ARRAY_BUFFER, normalData.size() * sizeof(float), normalData.data(), GL_STATIC_DRAW);

		GLuint vboTexture;
		glGenBuffers(1, &vboTexture);
		glBindBuffer(GL_ARRAY_BUFFER, vboTexture);
		glBufferData(GL_ARRAY_BUFFER, textureData.size() * sizeof(float), textureData.data(), GL_STATIC_DRAW);

		model.setVertices(vboVertex);
		model.setNormals(vboNormals);
		model.setTexCoords(vboTexture);
		model.setVertexCount(nLinhas);
	}
	else { cout << "Erro ao ler o ficheiro .3d" << endl;}
}

void readXMLgroups(XMLElement* group, Group* pai) {
    for (XMLElement* childgroup = group; childgroup != nullptr; childgroup = childgroup->NextSiblingElement("group")) {
        if (childgroup) {
            Group* grupo = new Group(nrgroupcounter++);

            //MODELS
            XMLElement* models = childgroup->FirstChildElement("models");
            if (models) {

				//MODEL
                for (XMLElement* modelElement = models->FirstChildElement("model"); modelElement != nullptr; modelElement = modelElement->NextSiblingElement("model")) {
                    const char* fileAttr = modelElement->Attribute("file");
                    if (fileAttr) {
                        Models model;
                        readFile(fileAttr, model);

                        //TEXTURE
                        XMLElement* texture = modelElement->FirstChildElement("texture");
                        if (texture && texture->Attribute("file")) {
                            string path = "test_files/test_files_phase_4/";
                            path += texture->Attribute("file");
                            model.setTexture(path);
                        }

                        //COLOR
                        XMLElement* color = modelElement->FirstChildElement("color");
                        if (color) {
                            Color c;
							
							//DIFFUSE
                            XMLElement* diffuse = color->FirstChildElement("diffuse");
                            if (diffuse) {
                                float temp[3] = {
                                    diffuse->FloatAttribute("R") / 255.0f,
                                    diffuse->FloatAttribute("G") / 255.0f,
                                    diffuse->FloatAttribute("B") / 255.0f
                                };
                                c.setDiffuse(temp);
                            }
							
							//AMBIENT
                            XMLElement* ambient = color->FirstChildElement("ambient");
                            if (ambient) {
                                float temp[3] = {
                                    ambient->FloatAttribute("R") / 255.0f,
                                    ambient->FloatAttribute("G") / 255.0f,
                                    ambient->FloatAttribute("B") / 255.0f
                                };
                                c.setAmbient(temp);
                            }
							
							//SPECULAR
                            XMLElement* specular = color->FirstChildElement("specular");
                            if (specular) {
                                float temp[3] = {
                                    specular->FloatAttribute("R") / 255.0f,
                                    specular->FloatAttribute("G") / 255.0f,
                                    specular->FloatAttribute("B") / 255.0f
                                };
                                c.setSpecular(temp);
                            }
							
							//EMISSIVE
                            XMLElement* emissive = color->FirstChildElement("emissive");
                            if (emissive) {
                                float temp[3] = {
                                    emissive->FloatAttribute("R") / 255.0f,
                                    emissive->FloatAttribute("G") / 255.0f,
                                    emissive->FloatAttribute("B") / 255.0f
                                };
                                c.setEmissive(temp);
                            }

							//SHININESS
                            XMLElement* shininess = color->FirstChildElement("shininess");
                            if (shininess) {
                                c.setShininess(shininess->FloatAttribute("value"));
                            }

                            model.setColor(c);
                        }

                        grupo->modelos.push_back(model);  // adiciona ao grupo
                    }
                }
            }

            //TRANSFORM
            XMLElement* transform = childgroup->FirstChildElement("transform");
            if (transform) {
                XMLElement* child = transform->FirstChildElement();
                int order[3] = { -1, -1, -1 };
                int i = 0;

                while (child) {
                    string name = child->Name();

					//ROTATE
                    if (name == "rotate") {
                        order[i] = ROTATE;
                        if (child->Attribute("time")) {
                            float rt[4] = {
                                child->FloatAttribute("time"),
                                child->FloatAttribute("x"),
                                child->FloatAttribute("y"),
                                child->FloatAttribute("z")
                            };
                            grupo->setRotationTime(rt);
                        }
                        else if (child->Attribute("angle")) {
                            float r[4] = {
                                child->FloatAttribute("angle"),
                                child->FloatAttribute("x"),
                                child->FloatAttribute("y"),
                                child->FloatAttribute("z")
                            };
                            grupo->setRotation(r);
                        }
                    }

					//TRANSLATE
                    else if (name == "translate") {
                        order[i] = TRANSLATE;
                        float time = child->FloatAttribute("time");
                        if (time) {
                            grupo->setTranslationTime(time);
                            const char* alignAttr = child->Attribute("align");
                            grupo->setAlign(alignAttr && strcmp(alignAttr, "true") == 0);

                            for (XMLElement* point = child->FirstChildElement("point"); point != nullptr; point = point->NextSiblingElement("point")) {
                                float x = point->FloatAttribute("x");
                                float y = point->FloatAttribute("y");
                                float z = point->FloatAttribute("z");
                                grupo->addPontoTranslacao(Ponto(x, y, z));
                            }
                        }
                        else {
                            float t[3] = {
                                child->FloatAttribute("x"),
                                child->FloatAttribute("y"),
                                child->FloatAttribute("z")
                            };
                            grupo->setTranslation(t);
                        }
                    }

					//SCALE
                    else if (name == "scale") {
                        order[i] = SCALE;
                        float s[3] = {
                            child->FloatAttribute("x"),
                            child->FloatAttribute("y"),
                            child->FloatAttribute("z")
                        };
                        grupo->setScale(s);
                    }
                    i++;
                    child = child->NextSiblingElement();
                }
                grupo->setOrder(order);
            }

            if (pai != nullptr) {
                pai->addSubgroup(grupo);
            }
            else {
                mainGrupos.emplace_back(grupo);
            }

            if (childgroup->FirstChildElement("group")) {
                readXMLgroups(childgroup->FirstChildElement("group"), grupo);
            }
        }
    }
}


void readXML(string file) {
	XMLDocument xml;
	
	string path = file;
	XMLError result = xml.LoadFile(path.c_str());
	if (result != XML_SUCCESS) {
		cerr << "Erro ao abrir o ficheiro XML: " << path << endl;
		exit(1);
	} else {
		cout << "Ficheiro lido com sucesso" << endl;
	}

	//WORLD
	XMLElement* world = xml.FirstChildElement("world");
	if (world == nullptr) {
		cout << "No Root Found\n" << endl;
	}

	//CAMARA
	XMLElement* camera = world->FirstChildElement("camera");

	if(camera){
		XMLElement* position = camera->FirstChildElement("position");
		XMLElement* lookAt = camera->FirstChildElement("lookAt");
		XMLElement* up = camera->FirstChildElement("up");
		XMLElement* projection = camera->FirstChildElement("projection");
		
		//POSITION
		if(position){
			camX = atof(position->Attribute("x"));
			camY = atof(position->Attribute("y"));
			camZ = atof(position->Attribute("z"));
			xInicial = camX;
        	yInicial = camY;
		}
		//LOOKAT
		if(lookAt){
			lookX = atof(lookAt->Attribute("x"));
			lookY = atof(lookAt->Attribute("y"));
			lookZ = atof(lookAt->Attribute("z"));
		}
		//UP
		if(up){
			upX = atof(up->Attribute("x"));
			upY = atof(up->Attribute("y"));
			upZ = atof(up->Attribute("z"));
		}
		//PROJECTION
		if(projection){
			fov = atof(projection->Attribute("fov"));
			near = atof(projection->Attribute("near"));
			far = atof(projection->Attribute("far"));
		}
	}

	//LIGHTS
	XMLElement* lights = world->FirstChildElement("lights");
	if(lights){
		XMLElement* light = lights->FirstChildElement("light");

		//TYPE
		while(light != nullptr){
			Light lightTemp;
			string type = light->Attribute("type");
			lightTemp.setType(type);
			//POINT
			if(type == "point"){
				float pos[3] = {
					light->FloatAttribute("posx"),
					light->FloatAttribute("posy"),
					light->FloatAttribute("posz")
				};
				lightTemp.setPos(pos);
			}
			//DIRECTIONAL
			else if(type == "directional"){
				float dir[3] = {
					light->FloatAttribute("dirx"),
					light->FloatAttribute("diry"),
					light->FloatAttribute("dirz")
				};
				lightTemp.setDir(dir);
			}
			//SPOTLIGHT
			else if(type == "spot"){
				//pos
				float pos[3] = {
					light->FloatAttribute("posx"),
					light->FloatAttribute("posy"),
					light->FloatAttribute("posz")
				};
				lightTemp.setPos(pos);
				
				//dir
				float dir[3] = {
					light->FloatAttribute("dirx"),
					light->FloatAttribute("diry"),
					light->FloatAttribute("dirz")
				};
				lightTemp.setDir(dir);

				//cutoff
				float cutoff = light->FloatAttribute("cutoff");
				lightTemp.setCutoff(cutoff);
			}
			lightsVector.push_back(lightTemp);
			light = light->NextSiblingElement();
		}
	}
	
	//GROUP
	for(XMLElement* group = world->FirstChildElement("group");group != nullptr; group = group->NextSiblingElement("group")){
		if(group){
			readXMLgroups(group, nullptr);
		}
	}
}

//
void cross(float* a, float* b, float* res) {

    res[0] = a[1] * b[2] - a[2] * b[1];
    res[1] = a[2] * b[0] - a[0] * b[2];
    res[2] = a[0] * b[1] - a[1] * b[0];
}

//
void normalize(float* a) {

    float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0] / l;
    a[1] = a[1] / l;
    a[2] = a[2] / l;
}

// Rotation matrix
void buildRotMatrix(float* x, float* y, float* z, float* m) {

    m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
    m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
    m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

//
void getCatmullRomPoint(float t, Ponto p0, Ponto p1, Ponto p2, Ponto p3, float* pos, float* deriv) {

    float M[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
                    { 1.0f, -2.5f,  2.0f, -0.5f},
                    {-0.5f,  0.0f,  0.5f,  0.0f},
                    { 0.0f,  1.0f,  0.0f,  0.0f} };

    float P[4][3] = { {p0.getX(),p0.getY(), p0.getZ()},
                    {p1.getX(), p1.getY(), p1.getZ()},
                    {p2.getX(), p2.getY(), p2.getZ()},
                    {p3.getX(), p3.getY(), p3.getZ()} };

    // Compute A = M * P
    float A[4][3] = { 0 };

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 4; k++) A[i][j] += M[i][k] * P[k][j];

    float T[4] = { t * t * t, t * t, t, 1 };

    // pos = T * A

    pos[0] = 0;
    pos[1] = 0;
    pos[2] = 0;

    for (int j = 0; j < 3; j++)
        for (int k = 0; k < 4; k++) pos[j] += T[k] * A[k][j];

    deriv[0] = 0;
    deriv[1] = 0;
    deriv[2] = 0;

    float Tderiv[4] = { 3 * t * t, 2 * t, 1, 0 };

    for (int j = 0; j < 3; j++)
        for (int k = 0; k < 4; k++) deriv[j] += Tderiv[k] * A[k][j];

}

//
void getGlobalCatmullRomPoint(float gt, float* pos, float* deriv, vector<Ponto> pontos) {

    int tamLoop = pontos.size(); // Points that make up the loop for catmull-rom interpolation
    float t = gt * tamLoop; // this is the real global t
    int index = floor(t);  // which segment
    t = t - index; // where within  the segment

    // indices store the points
    int indices[4];
    indices[0] = (index + tamLoop - 1) % tamLoop;
    indices[1] = (indices[0] + 1) % tamLoop;
    indices[2] = (indices[1] + 1) % tamLoop;
    indices[3] = (indices[2] + 1) % tamLoop;

    getCatmullRomPoint(t, pontos[indices[0]], pontos[indices[1]], pontos[indices[2]], pontos[indices[3]], pos, deriv);
}

void alinhamentoCurva(float* deriv) {

    float Z[3];
    float Y[3] = { 0,1,0 };
    float X[3] = { deriv[0],deriv[1],deriv[2] };
    float m[16];

    cross(X, Y, Z);
    cross(Z, X, Y);

    normalize(X);
    normalize(Y);
    normalize(Z);

    buildRotMatrix(X, Y, Z, m);

    glMultMatrixf((float*)m);
}

int loadTexture(std::string s) {

    ILuint t;
    ilGenImages(1, &t);
    ilBindImage(t);
	
	ilEnable(IL_ORIGIN_SET);
    ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_LOWER_LEFT);

    if (!ilLoadImage((ILstring)s.c_str())) {
        std::string alt = "../" + s;
        if (!ilLoadImage((ILstring)alt.c_str())) {
            printf("Erro ao procurar o ficheiro com a encontrada.\n", s.c_str());
            ilDeleteImages(1, &t);
            return 0;
        }
    }

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    unsigned char* texData = ilGetData();
    int tw = ilGetInteger(IL_IMAGE_WIDTH);
    int th = ilGetInteger(IL_IMAGE_HEIGHT);

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    ilDeleteImages(1, &t);

    return texID;
}


void draw(list<Group*> mainGrupos){
    for(auto grupo : mainGrupos){
        glPushMatrix();

        float t[3], r[4], s[3], rt[4];
        int order[3];
        grupo->getOrder(order);

        for(int i = 0; i < 3; i++) {

			//ROTATE
            if(order[i] == ROTATE) {
                grupo->getRotationTime(rt);
                grupo->getRotation(r);

                if (rt[0] != 0) {
                    float elapsed = glutGet(GLUT_ELAPSED_TIME) % (int)(rt[0] * 1000);
                    float angle = (elapsed * 360.0f) / (rt[0] * 1000);
                    glRotatef(angle, rt[1], rt[2], rt[3]);
                } else {
                    glRotatef(r[0], r[1], r[2], r[3]);
                }
            }

			//ROTATE
            if(order[i] == TRANSLATE) {
                float translationTime = grupo->getTranslationTime();
                if (translationTime != -1) {
                    list<Ponto> translationPoints = grupo->getPontosTranslacao();
                    vector<Ponto> pontosVec(translationPoints.begin(), translationPoints.end());

                    float te = glutGet(GLUT_ELAPSED_TIME) % (int)(translationTime * 1000);
                    float gt = te / (translationTime * 1000);

                    float pos[3], deriv[3];

                    glBegin(GL_LINE_LOOP);
                    for (float t = 0; t < 1; t += 0.01) {
                        float posAux[3], derivAux[3];
                        getGlobalCatmullRomPoint(t, posAux, derivAux, pontosVec);
                        glVertex3f(posAux[0], posAux[1], posAux[2]);
                    }
                    glEnd();

                    getGlobalCatmullRomPoint(gt, pos, deriv, pontosVec);
                    glTranslatef(pos[0], pos[1], pos[2]);

                    if (grupo->getAlign()) {
                        alinhamentoCurva(deriv);
                    }

                } else {
                    grupo->getTranslation(t);
                    glTranslatef(t[0], t[1], t[2]);
                }
            }

			//SCALE
            if(order[i] == SCALE) {
                grupo->getScale(s);
                glScalef(s[0], s[1], s[2]);
            }
        }

        for (Models& model : grupo->modelos) {
            Color c = model.getColor();
            float amb[3], diff[3], spec[3], emiss[3];
            c.getAmbient(amb);
            c.getDiffuse(diff);
            c.getSpecular(spec);
            c.getEmissive(emiss);
            float shininess = c.shininess;

            glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
            glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
            glMaterialfv(GL_FRONT, GL_EMISSION, emiss);
            glMaterialf(GL_FRONT, GL_SHININESS, shininess);

            string tex = model.getTexture();
            if (!tex.empty()) {
                GLuint texID;
                if (texturesMap.find(tex) != texturesMap.end()) {
                    texID = texturesMap[tex];
                } else {
                    texID = loadTexture(tex);
                    texturesMap[tex] = texID;
                }

                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texID);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            } else {
                glDisable(GL_TEXTURE_2D);
            }

            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);

            glBindBuffer(GL_ARRAY_BUFFER, model.getVertices());
            glVertexPointer(3, GL_FLOAT, 0, 0);

            glBindBuffer(GL_ARRAY_BUFFER, model.getNormals());
            glNormalPointer(GL_FLOAT, 0, 0);

            glBindBuffer(GL_ARRAY_BUFFER, model.getTexCoords());
            glTexCoordPointer(2, GL_FLOAT, 0, 0);

            glDrawArrays(GL_TRIANGLES, 0, model.getVertexCount());

            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }

        draw(grupo->getSubgroups());

        glPopMatrix();
    }
}


void eixos() {
	glBegin(GL_LINES);
	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(fov, ratio, near, far);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void createLights() {
	if (lightsVector.empty()) {
        glDisable(GL_LIGHTING);  
        return;
    }

	glEnable(GL_LIGHTING);
    float globalAmbient[4] = { 1.0f, 1.0f, 1.0f, 1.0f };  // sem luz ambiente global
    float lightAmbient[4]  = { 0.2f, 0.2f, 0.2f, 1.0f };  // ambiente base
    float lightDiffuse[4]  = { 1.0f, 1.0f, 1.0f, 1.0f };  // luz branca
    float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };  // brilho branco

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    for (int i = 0; i < lightsVector.size(); i++) {
        GLenum lightID = GL_LIGHT0 + i;
        Light& l = lightsVector[i];

        glEnable(lightID);

        glLightfv(lightID, GL_AMBIENT,  lightAmbient);
        glLightfv(lightID, GL_DIFFUSE,  lightDiffuse);
        glLightfv(lightID, GL_SPECULAR, lightSpecular);

        float position[4];
		float posDir[4];
        float direction[3];
		
		//DIRECTIONAL
        if (l.getType() == "directional") {
            l.getDir(direction);
            posDir[0] = direction[0];
            posDir[1] = direction[1];
            posDir[2] = direction[2];
            posDir[3] = 0.0f;
            glLightfv(lightID, GL_POSITION, posDir);
        }

		//POINT
        else if (l.getType() == "point") {
            l.getPos(position);
            position[3] = 1.0f;
            glLightfv(lightID, GL_POSITION, position);
        }

		//SPOTLIGHT
        else if (l.getType() == "spot") {
            l.getPos(position);
            l.getDir(direction);
            float cutoff = l.getCutoff();

            position[3] = 1.0f;
            glLightfv(lightID, GL_POSITION, position);
            glLightfv(lightID, GL_SPOT_DIRECTION, direction);
            glLightf(lightID, GL_SPOT_CUTOFF, cutoff); // ângulo de abertura do cone
        }
    }
}


void renderScene(void) {

	frame++;
	int time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		fps = frame*1000.0/(time-timebase);
		timebase = time;
		frame = 0;

		char title[50];
		sprintf(title, "FPS: %.2f", fps);
		glutSetWindowTitle(title);
	}

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, typeDraw);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX,camY,camZ,
		lookX,lookY,lookZ,
		upX, upY, upZ);
	
	createLights();
	
	glColor3f(1.0f, 1.0f, 1.0f);
		

	draw(mainGrupos);
	
	glDisable(GL_LIGHTING);
    eixos();
    glEnable(GL_LIGHTING);
	
	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char key, int xx, int yy) {

	// put code to process keys in here
	switch(key){
		case 'w' | 'W':
			camY += 0.1f;
			lookY += 0.1f;
			break;
		case 's' | 'S':
			camY -= 0.1f;
			lookY -= 0.1f;
			break;
		case 'a' | 'A':
			camX += 0.1f;
			lookX += 0.1f;
			break;
		case 'd' | 'D':
			camX -= 0.1f;
			lookX -= 0.1f;
			break;
		case 'q' | 'Q':
			camZ += 0.1f;
			lookZ += 0.1f;
			break;
		case 'e' | 'E':
			camZ -= 0.1f;
			lookZ -= 0.1f;
			break;
		case 'f' | 'F':
			typeDraw = GL_FILL;
			break;
		case 'p' | 'P':
			typeDraw = GL_POINT;
			break;
		case 'l' | 'L':
			typeDraw = GL_LINE;
			break;
	}
}

void processMouseButtons(int button, int state, int x, int y){
	switch (state){
		case GLUT_DOWN:
			xInicial = x;
			yInicial = y;
			if (button == GLUT_LEFT_BUTTON)		modoRato = 1; //rodar camara
			else if (button == GLUT_RIGHT_BUTTON)  modoRato = 2; //aproximar ou afastar camara
			else modoRato = 0;
			break;

		case GLUT_UP:
			if (modoRato == 1) { 
				alpha += (x - xInicial); 
				beta1 +=1;
			}
			else if (modoRato == 2) { 
				r -= y - yInicial;
				if (r < 3) r = 2.0;
			}
			break;
	}
}

void processMouseMotion(int x, int y){
	int xAux, yAux;
	int alphaAux, betaAux;
	int rAux;

	if (!modoRato) return;

	xAux = x - xInicial;
	yAux = y - yInicial;

	if (modoRato == 1) {

		alphaAux = alpha + xAux;
		betaAux = beta1 + yAux;
		if (betaAux > 90.0) betaAux = 90.0;
		else if (betaAux < -90.0) betaAux = -90.0;
		rAux = r;
	}
	else if (modoRato == 2) {

		alphaAux = alpha;
		betaAux = beta1;
		rAux = r - yAux;
		if (rAux < 3) rAux = 3;
	}
	//nova posicao x,y,z da camara
	camX = rAux * sin(-alphaAux * M_PI / 180.0) * cos(betaAux * M_PI / 180.0);
	camZ = rAux * cos(-alphaAux * M_PI / 180.0) * cos(betaAux * M_PI / 180.0);
	camY = rAux * sin(betaAux * M_PI / 180.0);
}

void init() {
    glEnable(GL_RESCALE_NORMAL);
    glShadeModel(GL_SMOOTH);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    ilInit();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("CG@DI-UM");

	glewInit();
    
	if (argc == 2) {
        readXML(argv[1]);
    }
    else {
        readXML("test_1_5.xml");
    }

    init(); 

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    glutMouseFunc(processMouseButtons);
    glutMotionFunc(processMouseMotion);
    glutKeyboardFunc(processKeys);

    glutMainLoop();

    return 1;
}
