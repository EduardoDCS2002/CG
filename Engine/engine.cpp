#include "engine.hpp"

list<Group> mainGrupos;
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

void readFile(string caminho3d, Group* group) {
	string linha;
	vector<string> coordenadas;
	cout<< caminho3d<<endl;
	ifstream file(caminho3d);
	if (file.is_open()) {
		cout << "potato" << endl;
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
	cout<<"chegou no readXMLgroups galera"<<endl;
	for(XMLElement* childgroup = group;childgroup != nullptr; childgroup = childgroup->NextSiblingElement("group")){
		if(childgroup){
			Group grupo = Group(nrgroupcounter);
			nrgroupcounter++;
			
			//MODELS
			XMLElement* models = childgroup->FirstChildElement("models");
			if(models){
				cout<<"chegou no segundo if do readxmlgroups galera"<<endl;
				XMLElement* model = models->FirstChildElement("model");

				while (model != nullptr) {
					cout<<"chegou no while galera"<<endl;
					if (strcmp(model->Attribute("file"), "sphere.3d") == 0) {
						readFile("sphere.3d", &grupo);
					}
					if (strcmp(model->Attribute("file"), "cone.3d") == 0) {
						readFile("cone.3d", &grupo);
					}
					if (strcmp(model->Attribute("file"), "plane.3d") == 0) {
						readFile("plane.3d", &grupo);
					}
					if (strcmp(model->Attribute("file"), "box.3d") == 0) {
						readFile("box.3d", &grupo);
					}
					cout<<"chega aqui e ao pai"<<endl;
					model = model->NextSiblingElement();
				}

			}
			//TRANSFORM
			
			cout<<"antes do if transform"<<endl;
			XMLElement* transform = childgroup->FirstChildElement("transform");
			if(transform){
				cout<<"pode ser"<<endl;
				XMLElement* rotate = transform->FirstChildElement("rotate");
				if(rotate){
					float r[4] ={
						rotate->FloatAttribute("angle"),
						rotate->FloatAttribute("x"),
						rotate->FloatAttribute("y"),
						rotate->FloatAttribute("z")
					};
					grupo.setRotation(r);
				}

				XMLElement* translate = transform->FirstChildElement("translate");
				if(translate){
					float t[3] ={
						translate->FloatAttribute("x"),
						translate->FloatAttribute("y"),
						translate->FloatAttribute("z")
					};
					grupo.setTranslation(t);
				}
				
				XMLElement* scale = transform->FirstChildElement("scale");
				if(scale){
					float s[3] ={
						scale->FloatAttribute("x"),
						scale->FloatAttribute("y"),
						scale->FloatAttribute("z")
					};
					grupo.setScale(s);
				}
			}
			if(pai != nullptr){
				cout<<"banana"<<endl;
				pai->addSubgroup(grupo);
				cout<< pai->getSubgroups().size() <<endl;
			}
			else{
				mainGrupos.emplace_back(grupo);
			}
			cout<<"chega aqui e anda mi amore"<<endl;
			if(childgroup->FirstChildElement("group")){
				readXMLgroups(childgroup->FirstChildElement("group"), &grupo);
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


void draw(list<Group> mainGrupos){
	
	for(auto grupo : mainGrupos){
		//cout<<"olha o draw "<< grupo.getnr() <<endl;
		
		glPushMatrix();
		list<Ponto> pontosatual = grupo.getPontos();
		/*if(pontosatual.size()>0){
			cout << "No :C" << endl;
		}
		for(auto ponto : pontosatual ){
			cout<<"x: " << ponto.getX() << " y: " <<ponto.getY()<<" z: "<<ponto.getZ()<<endl;
		}*/
		float t[3], r[4], s[3];
		grupo.getTranslation(t);
		grupo.getRotation(r);
		grupo.getScale(s);

		glScalef(s[0],s[1],s[2]);
		glRotatef(r[0],r[1],r[2],r[3]);
		glTranslatef(t[0],t[1],t[2]);

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
		draw(grupo.getSubgroups());
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