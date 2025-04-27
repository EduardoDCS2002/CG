#include "engine.hpp"

list<Group*> mainGrupos;
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
int typeDraw = GL_LINE;

int timebase = 0;
float frame = 0;
float fps;

//VBO stuff
GLuint vertices, verticeCount;
GLuint indices;
unsigned int indexCount;

void readFile(string caminho3d, Group* group) {
	string linha;
	vector<string> coordenadas;
	cout<< caminho3d<<endl;
	ifstream file(caminho3d);
	if (file.is_open()) {
		getline(file, linha);                   
		int nLinhas = atoi(linha.c_str());
		list<Ponto> pontos;
		for (int i = 1; i <= nLinhas; i++) {
			getline(file, linha);
			stringstream ss(linha);		
			vector<string> result{
				istream_iterator<string>(ss), {}
			};

			pontos.push_back(Ponto(stof(result[0]), stof(result[1]), stof(result[2])));
			
			
			
		}
		/*for(auto ponto : pontos ){
			cout<<"x: " << ponto.getX() << " y: " <<ponto.getY()<<" z: "<<ponto.getZ()<<endl;
		}*/
		group->setPontos(pontos);
		/*for(auto ponto : group->getPontos()){
			cout<<"x: " << ponto.getX() << " y: " <<ponto.getY()<<" z: "<<ponto.getZ()<<endl;
		}*/
	}
	else { cout << "Erro ao ler o ficheiro .3d" << endl;}
}

void readXMLgroups(XMLElement* group, Group* pai){
	for(XMLElement* childgroup = group;childgroup != nullptr; childgroup = childgroup->NextSiblingElement("group")){
		if(childgroup){
			Group* grupo = new Group(nrgroupcounter++);
			//MODELS
			XMLElement* models = childgroup->FirstChildElement("models");
			if(models){
				cout<<"Entrou no models"<<endl;
				XMLElement* model = models->FirstChildElement("model");

				while (model != nullptr) {
					cout<<"Entrou no model"<<endl;
					if (strcmp(model->Attribute("file"), "sphere.3d") == 0) {
						readFile("sphere.3d", grupo);
					}
					if (strcmp(model->Attribute("file"), "cone.3d") == 0) {
						readFile("cone.3d", grupo);
					}
					if (strcmp(model->Attribute("file"), "plane.3d") == 0) {
						readFile("plane.3d", grupo);
					}
					if (strcmp(model->Attribute("file"), "box.3d") == 0) {
						readFile("box.3d", grupo);
					}
					if (strcmp(model->Attribute("file"), "torus.3d") == 0) {
						readFile("torus.3d", grupo);
					}
					if (strcmp(model->Attribute("file"), "bezier.3d") == 0) {
						readFile("bezier.3d", grupo);
					}
					model = model->NextSiblingElement();
				}

			}
			//TRANSFORM
			
			
			XMLElement* transform = childgroup->FirstChildElement("transform");
			if(transform){
				cout<<"Entrou no transform"<<endl;
				XMLElement* rotate = transform->FirstChildElement("rotate");
				if (rotate) {
					if (rotate->Attribute("time")) {
						float rt[4] = {
							rotate->FloatAttribute("time"),
							rotate->FloatAttribute("x"),
							rotate->FloatAttribute("y"),
							rotate->FloatAttribute("z")
						};
						grupo->setRotationTime(rt);
					}
					else if (rotate->Attribute("angle")) {
						float r[4] = {
							rotate->FloatAttribute("angle"),
							rotate->FloatAttribute("x"),
							rotate->FloatAttribute("y"),
							rotate->FloatAttribute("z")
						};
						grupo->setRotation(r);
					}
				}


				XMLElement* translate = transform->FirstChildElement("translate");
				if(translate){
					float time = translate->FloatAttribute("time");
					if(time){
						grupo->setTranslationTime(time);
						const char* alignAttr = translate->Attribute("align");
						if (alignAttr && strcmp(alignAttr, "true") == 0) {
							grupo->setAlign(true);
						} else {
							grupo->setAlign(false);
						}
						
						XMLElement* point = translate->FirstChildElement("point");
						if(point){
							float x = point->FloatAttribute("x");
							float y = point->FloatAttribute("y");
							float z = point->FloatAttribute("z");
							
							Ponto ponto = *new Ponto(x,y,z);
							grupo->addPontoTranslacao(ponto);

							while(point->NextSiblingElement() != nullptr){
								point = point->NextSiblingElement();

								float x = point->FloatAttribute("x");
								float y = point->FloatAttribute("y");
								float z = point->FloatAttribute("z");
								
								Ponto ponto = *new Ponto(x,y,z);
								grupo->addPontoTranslacao(ponto);
							}
						}
					}
					else{
						float t[3] ={
							translate->FloatAttribute("x"),
							translate->FloatAttribute("y"),
							translate->FloatAttribute("z")
						};
						grupo->setTranslation(t);
				}
				
				XMLElement* scale = transform->FirstChildElement("scale");
				if(scale){
					float s[3] ={
						scale->FloatAttribute("x"),
						scale->FloatAttribute("y"),
						scale->FloatAttribute("z")
					};
					grupo->setScale(s);
				}
			}
			if(pai != nullptr){
				cout<<"Tem pai"<<endl;
				pai->addSubgroup(grupo);
				cout << "Subgroups size: " << pai->getSubgroups().size() <<endl;
			}
			else{
				mainGrupos.emplace_back(grupo);
			}
			if(childgroup->FirstChildElement("group")){
				cout<<"Avança para o proximo group"<<endl;
				readXMLgroups(childgroup->FirstChildElement("group"), grupo);
				cout << "Primeira vez:" << &grupo << endl;
			}
		}
	}
}
}

void readXML(string file) {
	XMLDocument xml;
	XMLDocument xmltv;
	string s;

	
	string path = file;
	if (!xml.LoadFile(path.c_str())) {
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


void draw(list<Group*> mainGrupos){
	
	for(auto grupo : mainGrupos){
		//cout<<"olha o draw "<< grupo.getnr() <<endl;
		
		glPushMatrix();
		list<Ponto> pontosatual = grupo->getPontos();
		
		float t[3], r[4], s[3], rt[4];

		//Translation
		float translationTime = grupo->getTranslationTime();

		if (translationTime != -1) {
			list<Ponto> translationPoints = grupo->getPontosTranslacao();
			vector<Ponto> pontosVec(translationPoints.begin(), translationPoints.end());

			float te = glutGet(GLUT_ELAPSED_TIME) % (int)(translationTime * 1000);
			float gt = te / (translationTime * 1000);

			float pos[3], deriv[3];

			glPushMatrix();
			glBegin(GL_LINE_LOOP);
			for (float t = 0; t < 1; t += 0.01) {
				float posAux[3], derivAux[3];
				getGlobalCatmullRomPoint(t, posAux, derivAux, pontosVec);
				glVertex3f(posAux[0], posAux[1], posAux[2]);
			}
			glEnd();
			glPopMatrix();
			
			getGlobalCatmullRomPoint(gt, pos, deriv, pontosVec);	//posiciona a teapot ao longo da curva

			glTranslatef(pos[0], pos[1], pos[2]);
			if (grupo->getAlign()) {
				alinhamentoCurva(deriv);
			}

		} else {
			grupo->getTranslation(t);
			glTranslatef(t[0], t[1], t[2]);
		}

		//Rotation
		grupo->getRotationTime(rt);
		grupo->getRotation(r);

		if (rt[0] != 0) {
			float elapsed = glutGet(GLUT_ELAPSED_TIME) % (int)(rt[0] * 1000);
			float angle = (elapsed * 360.0f) / (rt[0] * 1000);  // Ângulo proporcional ao tempo
			glRotatef(angle, rt[1], rt[2], rt[3]);
		}
		else {
			glRotatef(r[0], r[1], r[2], r[3]);
		}	
		
		//Scale
		grupo->getScale(s);


		glScalef(s[0],s[1],s[2]);

		for(auto it = pontosatual.begin(); it != pontosatual.end();){
			glBegin(GL_TRIANGLES);
			glVertex3f(it->getX(), it->getY(), it->getZ());
			++it;
			glVertex3f(it->getX(), it->getY(), it->getZ());
			++it;
			glVertex3f(it->getX(), it->getY(), it->getZ());
			++it;
			glEnd();
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

	glColor3f(1.0f, 1.0f, 1.0f);
		
	draw(mainGrupos);
	
	eixos();
	

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
				if (r < 3) r = 3.0;
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



int main(int argc, char** argv) {
    if (argc == 2) {
		readXML(argv[1]);
	}
	else {
		readXML("test_1_5.xml");
	}

	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1200, 800);
	glutCreateWindow("CG@DI-UM");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	

	// Callback registration for keyboard processing
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutKeyboardFunc(processKeys);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);		//see through if disable
	
	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}