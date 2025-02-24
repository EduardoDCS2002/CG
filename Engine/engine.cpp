#include "engine.hpp"

float camX, camY, camZ;  //posicao x,y,z da camara
int alpha = 0, beta = 0, r = 20;      // angulos e raio da camara
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

list<Ponto> pontosLista;

void readFile(string caminho3d) {
	string linha;
	vector<string> coordenadas;

	ifstream file(caminho3d);
	if (file.is_open()) {
		
		getline(file, linha);                   
		int nLinhas = atoi(linha.c_str());     
		for (int i = 1; i <= nLinhas; i++) {
			getline(file, linha);
			stringstream ss(linha);		
			vector<string> result{
				istream_iterator<string>(ss), {}
			};
			pontosLista.push_back(Ponto(stof(result[0]), stof(result[1]), stof(result[2])));
		}
	}
	else { cout << "Erro ao ler o ficheiro .3d" << endl;}
}

void readXML(string file) {
	XMLDocument xml;
	XMLDocument xmltv;
	string s;

	
	string path = "/mnt/c/Users/diogo/Desktop/Trabalho_CG/CG/test_files/test_files_phase_1/" + file;
	if (!(xml.LoadFile((path).c_str())) && 
	!(xmltv.LoadFile((path).c_str()))) {
		
		cout << "Ficheiro lido com sucesso" << endl;
		//Para o .3d
		XMLElement* elemento = xml.FirstChildElement("world")->FirstChildElement("group");
		if (elemento == nullptr) {
			cout << "No Root Found\n" << endl;
		}
		XMLElement* elemento2 = elemento->FirstChildElement("models");
		if (elemento2 == nullptr) {
			cout << "No Root Found\n" << endl;
		}
		XMLElement* elemento3 = elemento2->FirstChildElement("model");
		if (elemento3 == nullptr) {
			cout << "No Root Found\n" << endl;
		}
		
        while (elemento3 != nullptr) {
			
            if (strcmp(elemento3->Attribute("file"), "sphere.3d") == 0) {
                readFile("/mnt/c/Users/diogo/Desktop/Trabalho_CG/CG/sphere.3d");
            }
            if (strcmp(elemento3->Attribute("file"), "cone.3d") == 0) {
                readFile("/mnt/c/Users/diogo/Desktop/Trabalho_CG/CG/cone.3d");
            }
            if (strcmp(elemento3->Attribute("file"), "plane.3d") == 0) {
                readFile("/mnt/c/Users/diogo/Desktop/Trabalho_CG/CG/plane.3d");
            }
            if (strcmp(elemento3->Attribute("file"), "box.3d") == 0) {
                readFile("/mnt/c/Users/diogo/Desktop/Trabalho_CG/CG/box.3d");
            }
            elemento3 = elemento3->NextSiblingElement();
        }
		

		//Camara
		XMLElement* tv = xmltv.FirstChildElement("world")->FirstChildElement("camera");
		XMLElement* tv2 = tv->FirstChildElement("position");
        XMLElement* tv3 = tv->FirstChildElement("lookAt");
        XMLElement* tv4 = tv->FirstChildElement("up");
        XMLElement* tv5 = tv->FirstChildElement("projection");

		camX = atof(tv2->Attribute("x"));
		camY = atof(tv2->Attribute("y"));
		camZ = atof(tv2->Attribute("z"));

        xInicial = camX;
        yInicial = camY;

        lookX = atof(tv3->Attribute("x"));
        lookY = atof(tv3->Attribute("y"));
        lookZ = atof(tv3->Attribute("z"));

        upX = atof(tv4->Attribute("x"));
        upY = atof(tv4->Attribute("y"));
        upZ = atof(tv4->Attribute("z"));

        fov = atof(tv5->Attribute("fov"));
        near = atof(tv5->Attribute("near"));
        far = atof(tv5->Attribute("far"));


	}
	else {
		cout << "Erro ao ler o xml" << endl;
	}
	return;
}

float cameraY = 5.0f;

void draw() {
	for (auto it = pontosLista.begin(); it != pontosLista.end(); ) {
		glBegin(GL_TRIANGLES);
		glVertex3f(it->getX(), it->getY(), it->getZ());
		++it;
		glVertex3f(it->getX(), it->getY(), it->getZ());
		++it;
		glVertex3f(it->getX(), it->getY(), it->getZ());
		++it;
		glEnd();
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

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX,camY,camZ,
		lookX,lookY,lookZ,
		upX, upY, upZ);

	glColor3f(1.0f, 1.0f, 1.0f);

	draw();
	
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
	}
}


void processSpecialKeys(int key, int xx, int yy) {

	// put code to process special keys in here

}


int main(int argc, char** argv) {

    if (argc == 2) {
		readXML(argv[1]);
	}
	else {
		readXML("test_1_5.xml");
	}

	// init GLUT and the window
	/*
	cout << "Printing all coordinates from pontosLista:" << endl;
    for (Ponto ponto : pontosLista) {
        cout << "Vertex: (" << ponto.getX() 
             << ", " << ponto.getY() 
             << ", " << ponto.getZ() << ")" << endl;
    }
	*/

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
	glutKeyboardFunc(processKeys);
	//glutSpecialFunc(processSpecialKeys);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);				//see through if disable

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}
