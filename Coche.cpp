#define PROYECTO "ISG::P9:: Juego Coche velocidad: 0 m/s"
#include <iostream> // Biblioteca de entrada salida
#include <freeglut.h> // Biblioteca grafica
#include <sstream>
#include <cmath>
#include <time.h>
#include <Utilidades.h>
#include <irrKlang.h>
#include <conio.h>
#include <../freeimage/FreeImage.h>

#define PI 3.141569656546546816851

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
static GLuint ejese; // Identificador de los ejes
using namespace irrklang;
using namespace std;


static const int tasaFPS = 120;
float velocidad = 0;
float v_d = 0; //vector director de la velocidad
float pos[3] = { 0,0,0 }; //posicion de la camara y del vehiculo
float amplitud = 3;
float periodo = 50;
float u = 0;
float aux = 0;
static GLuint rb[2];
static GLuint del[3];
static GLuint ter[3];
static GLuint gro[3];
static GLuint sing[4];
bool made1 = false, made2 = false, made3 = false, made4 = false;
static enum ModoDibujo { ALAMBRICO, SOLIDO };
ModoDibujo modo = SOLIDO;
bool noche = false;
bool niebla = false;
bool delante = false;
bool race = false;
bool terror = false;
bool scream = false;
bool mute = false;
GLfloat c0[3] = { pos[0] + cos(v_d) + 1.3, -0.5, pos[2] + sin(v_d) - 1.7 };
GLfloat c1[3] = { pos[0] + cos(v_d) + 1.3, -0.5, pos[2] + sin(v_d) + 2.3 };
GLfloat c2[3] = { pos[0] + cos(v_d) + 1.3, 2.1, c1[2] };
GLfloat c3[3] = { pos[0] + cos(v_d) + 1.3, 2.1, c0[2] };
float tex = 0.0;
int points = 0;
int saltados = 0;
ISoundEngine* engine;
int w = 0;
GLfloat posi[3] = { pos[0]+11, -2.5, 2.5 };

void muestraFPS()
// Calcula la frecuencia y la muestra en el título de la ventana
// cada segundo
{
	int ahora, tiempo_transcurrido;
	static int antes = 0;
	static int FPS = 0;
	stringstream titulo;
	//Cuenta de llamadas a esta función en el último segundo
	FPS++;
	ahora = glutGet(GLUT_ELAPSED_TIME); //Tiempo transcurrido desde el inicio
	tiempo_transcurrido = ahora - antes; //Tiempo transcurrido desde antes
	if (tiempo_transcurrido > 1000) { //Acaba de rebasar el segundo
		titulo <<"FPS: " << FPS << "ISG::P9::  Juego Coche velocidad: " << velocidad << " m/s";
		glutSetWindowTitle(titulo.str().c_str());
		antes = ahora; //Ahora ya es antes
		FPS = 0; //Reset del conteo
	}
}
/*void saveScreenshot(char* nombre, int ancho, int alto)
// Utiliza FreeImage para grabar un png
// nombre: Nombre del fichero con extensión p.e. salida.png
// ancho: Ancho del viewport en pixeles
// alto: Alto del viewport en pixeles
{
	int pix = ancho * alto;
	BYTE* pixels = new BYTE[3 * pix];
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, ancho, alto, GL_BGR, GL_UNSIGNED_BYTE, pixels);
	FIBITMAP* img = FreeImage_ConvertFromRawBits(pixels, ancho, alto,
		ancho * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	FreeImage_Save(FIF_PNG, img, nombre, 0);
	delete pixels;
}*/
void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
	static const GLfloat ANCHO(2.0), ALTO(2.0);
	// Usamos toda el area de dibujo
	glViewport(0, 0, w, h);
	// Definimos la camara (matriz de proyeccion)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float razon = (float)w / h;
	gluPerspective(45, razon, 1, 110);

}

void loadGround() {
	glGenTextures(3, gro);
	glBindTexture(GL_TEXTURE_2D, gro[0]);
	//1c. Cargar la imagen que servira de textura
	loadImageFile((char*)"text-gras.jpg");
	//1d. Habilitar las texturas
	glEnable(GL_TEXTURE_2D);
	//2a. Seleccionar el objeto textura
	glBindTexture(GL_TEXTURE_2D, gro[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//2c. Definir la forma de combinar
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, gro[1]);
	//1c. Cargar la imagen que servira de textura
	loadImageFile((char*)"leaves.jpg");
	//1d. Habilitar las texturas
	glEnable(GL_TEXTURE_2D);
	//2a. Seleccionar el objeto textura
	glBindTexture(GL_TEXTURE_2D, gro[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//2c. Definir la forma de combinar
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, gro[2]);
	//1c. Cargar la imagen que servira de textura
	loadImageFile((char*)"snow.jpg");
	//1d. Habilitar las texturas
	glEnable(GL_TEXTURE_2D);
	//2a. Seleccionar el objeto textura
	glBindTexture(GL_TEXTURE_2D, gro[2]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//2c. Definir la forma de combinar
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
void loadDel() {
	glGenTextures(3, del);
	glBindTexture(GL_TEXTURE_2D, del[0]);
	//1c. Cargar la imagen que servira de textura
	loadImageFile((char*)"Parabrisas.png");
	//1d. Habilitar las texturas
	glEnable(GL_TEXTURE_2D);
	//2a. Seleccionar el objeto textura
	glBindTexture(GL_TEXTURE_2D, del[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//2c. Definir la forma de combinar
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBindTexture(GL_TEXTURE_2D, del[2]);
	//1c. Cargar la imagen que servira de textura
	loadImageFile((char*)"race.jpg");
	//1d. Habilitar las texturas
	glEnable(GL_TEXTURE_2D);
	//2a. Seleccionar el objeto textura
	glBindTexture(GL_TEXTURE_2D, del[2]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//2c. Definir la forma de combinar
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
void loadRoadB() {
	glGenTextures(2, rb);
	glBindTexture(GL_TEXTURE_2D, rb[0]);
	//1c. Cargar la imagen que servira de textura
	loadImageFile((char*)"carretera.jpg");
	//1d. Habilitar las texturas
	glEnable(GL_TEXTURE_2D);
	//2a. Seleccionar el objeto textura
	glBindTexture(GL_TEXTURE_2D, rb[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//2c. Definir la forma de combinar
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBindTexture(GL_TEXTURE_2D, rb[1]);
	//1c. Cargar la imagen que servira de textura
	loadImageFile((char*)"texture_back.jpg");
	//1d. Habilitar las texturas
	glEnable(GL_TEXTURE_2D);
	//2a. Seleccionar el objeto textura
	glBindTexture(GL_TEXTURE_2D, rb[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//2c. Definir la forma de combinar
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
void randomS() {
	int r = (rand() % 4) + 1;
	switch (r)
	{
	case 1:
		loadImageFile((char*)"texture_ad_1.png");
		break;
	case 2:
		loadImageFile((char*)"texture_ad_2.jpg");
		break;
	case 3:
		loadImageFile((char*)"texture_ad_3.jpg");
		break;
	default:
		loadImageFile((char*)"texture_ad_4.jpg");
		break;
	}
}
void loadT() {
	glGenTextures(3, ter);
	glBindTexture(GL_TEXTURE_2D, ter[0]);
	//1c. Cargar la imagen que servira de textura
	loadImageFile((char*)"payaso.jpg");
	//1d. Habilitar las texturas
	glEnable(GL_TEXTURE_2D);
	//2a. Seleccionar el objeto textura
	glBindTexture(GL_TEXTURE_2D, ter[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//2c. Definir la forma de combinar
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, ter[1]);
	//1c. Cargar la imagen que servira de textura
	loadImageFile((char*)"grito.jpg");
	//1d. Habilitar las texturas
	glEnable(GL_TEXTURE_2D);
	//2a. Seleccionar el objeto textura
	glBindTexture(GL_TEXTURE_2D, ter[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//2c. Definir la forma de combinar
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, ter[2]);
	//1c. Cargar la imagen que servira de textura
	loadImageFile((char*)"exorcista.jpg");
	//1d. Habilitar las texturas
	glEnable(GL_TEXTURE_2D);
	//2a. Seleccionar el objeto textura
	glBindTexture(GL_TEXTURE_2D, ter[2]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//2c. Definir la forma de combinar
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}
void loadSing() {
	glGenTextures(4, sing);
	glBindTexture(GL_TEXTURE_2D, sing[0]);
	//1c. Cargar la imagen que servira de textura
	randomS();
	//1d. Habilitar las texturas
	glEnable(GL_TEXTURE_2D);
	//2a. Seleccionar el objeto textura
	glBindTexture(GL_TEXTURE_2D, sing[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//2c. Definir la forma de combinar
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBindTexture(GL_TEXTURE_2D, sing[1]);
	//1c. Cargar la imagen que servira de textura
	randomS();
	//1d. Habilitar las texturas
	glEnable(GL_TEXTURE_2D);
	//2a. Seleccionar el objeto textura
	glBindTexture(GL_TEXTURE_2D, sing[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//2c. Definir la forma de combinar
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, sing[2]);
	//1c. Cargar la imagen que servira de textura
	randomS();
	//1d. Habilitar las texturas
	glEnable(GL_TEXTURE_2D);
	//2a. Seleccionar el objeto textura
	glBindTexture(GL_TEXTURE_2D, sing[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//2c. Definir la forma de combinar
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, sing[3]);
	//1c. Cargar la imagen que servira de textura
	randomS();
	//1d. Habilitar las texturas
	glEnable(GL_TEXTURE_2D);
	//2a. Seleccionar el objeto textura
	glBindTexture(GL_TEXTURE_2D, sing[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//2c. Definir la forma de combinar
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
void loadSing1(int r) {
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, sing[r]);
	randomS();
	glEnable(GL_TEXTURE_2D);
	//2a. Seleccionar el objeto textura
	glBindTexture(GL_TEXTURE_2D, sing[r]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//2c. Definir la forma de combinar
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void init()
// Funcion propia de inicializacion
{
	cout << "Version: OpenGL " << glGetString(GL_VERSION) << endl;
	engine = createIrrKlangDevice();
	glClearColor(1.0, 1.0, 1.0, 1.0);
	loadDel();
	loadSing();
	loadT();
	loadGround();
	loadRoadB();
	glShadeModel(GL_SMOOTH);
	GLfloat Al0[] = { 0.05, 0.05, 0.05, 1.0 }; // Color ambiental de la luz
	GLfloat Dl0[] = { 0.05, 0.05, 0.05, 1.0 }; // Color difuso de la luz
	GLfloat Sl0[] = { 0.0, 0.0, 0.0, 1.0 }; // Color especular de la luz
	glLightfv(GL_LIGHT0, GL_AMBIENT, Al0); // Caracteristicas de LIGHT0
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Dl0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Sl0);
	glEnable(GL_LIGHT0);
	GLfloat Al1[] = { 1, 1, 0.0, 1.0 }; // Color ambiental de la luz
	GLfloat Dl1[] = { 1.0, 1.0, 1.0, 1.0 }; // Color difuso de la luz
	GLfloat Sl1[] = { 1, 1, 0.0, 1.0 }; // Color especular de la luz
	glLightfv(GL_LIGHT1, GL_AMBIENT, Al1); // Caracteristicas de LIGHT1
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Dl1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, Sl1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 5.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 5.0);
	glEnable(GL_LIGHT1);
	GLfloat Al2[] = { 1, 1.0, 0.0, 1.0 }; // Color ambiental de la luz
	GLfloat Dl2[] = { 1.0, 1.0, 0.0, 1.0 }; // Color difuso de la luz
	GLfloat Sl2[] = { 1.0, 1.0, 0.0, 1.0 }; // Color especular de la luz
	glLightfv(GL_LIGHT2, GL_AMBIENT, Al2); // Caracteristicas de LIGHT2
	glLightfv(GL_LIGHT2, GL_DIFFUSE, Dl2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, Sl2);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 40.0);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10.0);
	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT3, GL_AMBIENT, Al2); // Caracteristicas de LIGHT2
	glLightfv(GL_LIGHT3, GL_DIFFUSE, Dl2);
	glLightfv(GL_LIGHT3, GL_SPECULAR, Sl2);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 40.0);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 10.0);
	glEnable(GL_LIGHT3);
	glLightfv(GL_LIGHT4, GL_AMBIENT, Al2); // Caracteristicas de LIGHT2
	glLightfv(GL_LIGHT4, GL_DIFFUSE, Dl2);
	glLightfv(GL_LIGHT4, GL_SPECULAR, Sl2);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 40.0);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 10.0);
	glEnable(GL_LIGHT4);
	glLightfv(GL_LIGHT5, GL_AMBIENT, Al2); // Caracteristicas de LIGHT2
	glLightfv(GL_LIGHT5, GL_DIFFUSE, Dl2);
	glLightfv(GL_LIGHT5, GL_SPECULAR, Sl2);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 40.0);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 10.0);
	glEnable(GL_LIGHT5);

	glEnable(GL_DEPTH_TEST);
	ejese = glGenLists(1);
	glNewList(1, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);
	glLineWidth(4.0);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0); // Eje X en rojo
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glColor3f(0, 1, 0); // Eje Y en verde
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glColor3f(0, 0, 1); // Eje Z en azul
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	glEnd();
	glColor3f(0.5, 0.5, 0.5); // Origen en gris
	glutWireCube(0.1);
	glPopAttrib();
	glEndList();
	//glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
	printf("Pulse las flechas para desplazarse: adelante acelerar, atras frenar, a los lados girar\n");
	printf("Pulse s/S para cambiar entre solido y alambrico\n");
	printf("Pulse l/L para cambiar entre dia y noche\n");
	printf("Pulse n/N para cambiar entre despejado y niebla\n");
	printf("Pulse c/C para mostrar y ocultar el salpicadero\n");
	printf("Pulse r/R para activar/descativar el modo carrera (parte superior de la ventana), desactivarlo resetea los puntos (Activar el modo race desactiva el modo terror)\n");
	printf("Pulse w/W para cambiar de estacion\n");
	printf("Pulse t/T para cambiar a modo Terror (se recomienda modo solido, noche y niebla activada, activar el modo terror desactiva el modo race)\n");
	printf("Pulse m/M para eliminar el sonido\n");
}

float fu(float u) { //funcion m.a.s. con sin
	return amplitud * sin(u * 2 * PI / periodo);
}

float fud(float u) { //derivada funcion m.a.s. con sin
	return 2*PI*(amplitud/periodo) * cos(u * 2 * PI / periodo);
}

int calculaCartel(int s) { //Calcula que cartel corresponde a cada quad
	return (s + ((int(aux) / 11) % 4)) % 4;
}

void Scream() {
	float aumento = 0.7;
	float radio = 3.1;
	c0[0] = pos[0] + cos(v_d - aumento) * radio;
	c0[2] = pos[2] + sin(v_d - aumento) * radio;
	c1[0] = pos[0] + cos(v_d + aumento) * radio;
	c1[2] = pos[2] + sin(v_d + aumento) * radio;

	c2[0] = c1[0];
	c2[2] = c1[2];
	c3[0] = c0[0];
	c3[2] = c0[2];
	int terr = rand() % 3;
	switch(terr) {
		case 0:
			engine->play2D("./grito1.mp3");
			break;
		case 1:
			engine->play2D("./risa.mp3");
			break;
		case 2:
			engine->play2D("./eco.mp3");
			break;
	}
	
	glPushMatrix();
	glColor3f(1,1,1);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, ter[(rand() % 3)]);
	quadtex(c0, c1, c2, c3, 0, 1, 0, 1, 1, 10);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPopMatrix();
}
void Seta(float x, float z) {
	GLfloat scol[3];
	switch (w)
	{
	case 0:
		//Estacion: Otono
		scol[0] = 0.07;
		scol[1] = 0.38;
		scol[2] = 0.03;
		break;
	case 1:
		//Estacion: Primavera
		scol[0] = 0.81;
		scol[1] = 0.2;
		scol[2] = 0.18;
		break;
	case 2:
		//Estacion: Invierno
		scol[0] = 0;
		scol[1] = 0.38;
		scol[2] = 1;
		break;
	}
	glColor3f(0.75, 0.75, 0.75);
	glTranslatef(x, 0, z);
	glRotatef(-90, 1, 0, 0);
	if (modo == SOLIDO) {
		glutSolidCylinder(0.1, 2.5, 5, 5);
		glColor3f(scol[0], scol[1], scol[2]);
		glTranslatef(0, 0, 2.5);
		glutSolidCylinder(2.5, 0.1, 20, 20);
		glColor3f(scol[0], scol[1]+0.12, scol[2]);
		glTranslatef(0, 0, -0.75);
		glutSolidCylinder(1.5, 0.1, 20, 20);
		glColor3f(scol[0], scol[1] + 0.24, scol[2]);
		glTranslatef(0, 0, -0.75);
		glutSolidCylinder(1, 0.1, 20, 20);
	}
	else { 
		glColor3f(1, 1, 1);
		glutWireCylinder(0.1, 2.5, 5, 5);
		glTranslatef(0, 0, 2.5);
		glutWireCylinder(2.5, 0.1, 20, 20);
		glTranslatef(0, 0, -0.75);
		glutWireCylinder(1.5, 0.1, 20, 20);
		glTranslatef(0, 0, -0.75);
		glutWireCylinder(1, 0.1, 20, 20);
	}
}
void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Borra la pantalla
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (noche) { glEnable(GL_LIGHTING); }
	else { glDisable(GL_LIGHTING); }
	if (modo == SOLIDO) {
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
	}
	else { 
		glPolygonMode(GL_FRONT, GL_LINE); 
		glPolygonMode(GL_BACK, GL_LINE);
	}
	if (niebla) {
		glEnable(GL_FOG);
		static GLfloat cniebla[] = { 0.8,0.8,0.8,1.0 };
		if (noche) {
			cniebla[0] = 0.2; // Color de la niebla radioactiva
			cniebla[1] = 0.2;
			cniebla[2] = 0.2;
		}
		else {
			cniebla[0] = 0.8; // Color de la niebla radioactiva
			cniebla[1] = 0.8;
			cniebla[2] = 0.8;
		}
		glFogfv(GL_FOG_COLOR, cniebla);
		glFogf(GL_FOG_DENSITY, 0.05);
	}
	else { glDisable(GL_FOG); }

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	if (modo == SOLIDO) { glEnable(GL_COLOR_MATERIAL); }

	GLfloat v0[3] = { 0,0,0 }, v1[3] = { 0,0,0 }, v2[3] = { 0,0,0 }, v3[3] = { 0,0,0 }, n[2] = { 0,0 };
	glClearColor(0.0, 0.0, 0.0, 1);
	//glClearColor(1, 1, 1, 1);
	GLfloat posicion[] = { 0,0,0,0 };
	glBindTexture(GL_TEXTURE_2D, 0);
	posicion[1] = 0.7;
	posicion[0] = 0;
	posicion[2] = 0;
	posicion[3] = 1;
	GLfloat dir_central[] = { 0, -0.15, -1 }; //Faros
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir_central);
	glLightfv(GL_LIGHT1, GL_POSITION, posicion);
	
	
	//gluLookAt(pos[0], 1, pos[2], pos[0] + cos(PI/2-v_d), 1, pos[2] + sin(PI/2-v_d), 0, 1, 0); 
	//con v_d inicial=PI/2 y derecha-= e izuierda+=
	gluLookAt(pos[0], 1, pos[2], pos[0] + cos(v_d), 1, pos[2] + sin(v_d), 0, 1, 0);
	//gluLookAt(pos[0], 99, pos[2], pos[0] + cos(v_d), 1, pos[2] + sin(v_d), 0, 1, 0);
	//con v_d inicial=0 y derecha+= e izuierda-=
	//glCallList(ejese);
	if (!mute && terror && modo == SOLIDO) {
		if (scream) {
			Scream();
			scream = false;
		}
		//Sleep((rand() % 7) + 1000);
	}

	posicion[3] = 0;
	posicion[2] = 0;
	posicion[1] = 0;
	posicion[0] = 0;
	glLightfv(GL_LIGHT0, GL_POSITION, posicion); //Luz ambiente
	
	posicion[0] = 1;
	posicion[1] = 4;
	posicion[2] = 0;
	posicion[3] = 1;
	dir_central[0] = 0.1; //Vectores de farolas
	dir_central[1] = -1.0;
	dir_central[2] = 0.0;

	//glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir_central); //Aparece farola 1
	//glLightfv(GL_LIGHT2, GL_POSITION, posicion);
	
	GLfloat Dm[] = { 0.8,0.8,0.8,1.0 }; //Textura carretera
	GLfloat Sm[] = { 0.3,0.3,0.3,1.0 };
	GLfloat s = 3.0;
	glBindTexture(GL_TEXTURE_2D, 0);
	if (modo == SOLIDO) {
		glBindTexture(GL_TEXTURE_2D, rb[1]);
		v0[1] = -45, v1[1] = -45, v2[1] = 50, v3[1] = 50;
		int i = 0;
		while (i < 10) {
			float a = i * 2 * PI / 10;
			float b = (i + 1) * 2 * PI / 10;
			v0[0] = pos[0] + 100 * cos(a);
			v1[0] = pos[0] + 100 * cos(b);
			v0[2] = pos[2] + 100 * sin(a);
			v1[2] = pos[2] + 100 * sin(b);

			v2[0] = pos[0] + 100 * cos(b);
			v3[0] = pos[0] + 100 * cos(a);
			v2[2] = pos[2] + 100 * sin(b);
			v3[2] = pos[2] + 100 * sin(a);
			a = (i + 5) * 2 * PI / 10;
			b = (i + 6) * 2 * PI / 10;
			quadtex(v0, v1, v2, v3, a / (2 * PI), b / (2 * PI), 0, 1, 100, 100);
			i++;
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	//Vuelta a la normalidad
	v0[0] = 0, v1[0] = 0, v2[0] = 0, v3[0] = 0;
	v0[1] = 0, v1[1] = 0, v2[1] = 0, v3[1] = 0;
	v0[2] = 0, v1[2] = 0, v2[2] = 0, v3[2] = 0;

	int u = int(pos[0]) - 2;
	tex = 0;
	float inc = 0.1;
	
	while (u < int(pos[0]) + 48) {
		n[1] = (1 / sqrt(1 + fud(u) * fud(u))); //Calcula nuevos puntos del quad
		n[0] = n[1] * -fud(u);
		v0[0] = u + n[0] * 5 / 2;
		v0[2] = fu(u) + n[1] * 5 / 2;
		v3[0] = u - n[0] * 5 / 2;
		v3[2] = fu(u) - n[1] * 5 / 2;
		if (u > 11 + aux && !made1) {
			posicion[0] = aux + 11;
			posicion[2] = fu(posicion[0]);
			glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir_central); //Aparece farola 1
			glLightfv(GL_LIGHT2, GL_POSITION, posicion);
			float w0[3] = { posicion[0], 1.75, fu(posicion[0])+2.75 };
			float w1[3] = { posicion[0], 1.75, fu(posicion[0])-2.75 };
			float w2[3] = { posicion[0], 4, w1[2] };
			float w3[3] = { posicion[0], 4, w0[2] };
			if (modo == SOLIDO) {
				glBindTexture(GL_TEXTURE_2D, sing[calculaCartel(0)]); //Pintar cartel cercano
				quadtex(w0, w1, w2, w3, 0, 1, 0, 1, 50, 50);
			}
			else { quad(w0, w1, w2, w3, 1, 10); }
			glBindTexture(GL_TEXTURE_2D, 0);
			glPushMatrix();
			glColor3f(0.4, 0.4, 0.4);
			glTranslatef(posicion[0], 0, w1[2]);		//Palos que sujetan el cartel
			glRotatef(-90, 1, 0, 0);
			if (modo == SOLIDO) {
				glutSolidCylinder(0.1, 4, 5, 5);
			}
			else { glColor3f(1, 1, 1);  glutWireCylinder(0.1, 4, 5, 5); }
			glPopMatrix();
			glPushMatrix();
			Seta(posicion[0] - 3, w1[2] - 6);
			glPopMatrix();
			glPushMatrix();
			Seta(posicion[0] - 3, w1[2] + 12);
			glPopMatrix();
			glPushMatrix();
			glColor3f(0.4, 0.4, 0.4);
			glTranslatef(posicion[0], 0, w0[2]);
			glRotatef(-90, 1, 0, 0);
			if (modo == SOLIDO) {
				glutSolidCylinder(0.1, 4, 5, 5);
			}
			else { glColor3f(1, 1, 1); glutWireCylinder(0.1, 4, 5, 5); }
			glColor3f(1, 1, 1);
			glPopMatrix();
			if (race && modo == SOLIDO) {
				glBindTexture(GL_TEXTURE_2D, del[2]);
				w0[1] = 0.75;
				w1[1] = w0[1];
				w2[1] = 1;
				w3[1] = w2[1];
				quadtex(w0, w1, w2, w3, 0, 13, 0, 1, 10, 10);
			}
			if (race && posi[0] < pos[0] && pos[2] > posi[1] && pos[2] < posi[2]) {
				points++;
				posi[0] = aux + 22;
				posi[1] = fu(posi[0]) - 2.75;
				posi[2] = fu(posi[0]) + 2.75;
			}
			if (race && posi[0] < pos[0] && !(pos[2] > posi[1] && pos[2] < posi[2])) {
				saltados++;
				posi[0] = aux + 22;
				posi[1] = fu(posi[0]) - 2.75;
				posi[2] = fu(posi[0]) + 2.75;
			}
			int i = 0;
			float f0[3] = { posicion[0] - 14, -0.01, posicion[2] - 50 };
			float f1[3] = { posicion[0] - 14, -0.01, posicion[2] + 50 };
			float f2[3] = { posicion[0] + 50, -0.01, posicion[2] + 50 };
			float f3[3] = { posicion[0] + 50, -0.01, posicion[2] - 50 };
			if (modo == SOLIDO) {
				glPushMatrix();
				glBindTexture(GL_TEXTURE_2D, gro[w]);
				quadtex(f0, f1, f2, f3, 0, 50, 0, 50, 750, 750);
				glPopMatrix();
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else {
				glPushMatrix();
				glColor3f(0.2,0.2,0.2);
				quad(f0, f1, f2, f3, 750, 750);
				glPopMatrix();
				glColor3f(1, 1, 1);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			made1 = true;
		}
		if (u > 22 + aux && !made2) {
			posicion[0] = aux + 22;
			posicion[2] = fu(posicion[0]);
			glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, dir_central); //Aparece farola 2
			glLightfv(GL_LIGHT3, GL_POSITION, posicion);
			float w0[3] = { posicion[0], 1.75, fu(posicion[0]) + 2.75 };
			float w1[3] = { posicion[0], 1.75, fu(posicion[0]) - 2.75 };
			float w2[3] = { posicion[0], 4, w1[2] };
			float w3[3] = { posicion[0], 4, w0[2] };
			if (modo == SOLIDO) {
				glBindTexture(GL_TEXTURE_2D, sing[calculaCartel(1)]); //Pintar cartel semi cercano
				quadtex(w0, w1, w2, w3, 0, 1, 0, 1, 50, 50);
			}
			else { quad(w0, w1, w2, w3, 1, 10); }
			glBindTexture(GL_TEXTURE_2D, 0);
			glPushMatrix();
			glColor3f(0.4, 0.4, 0.4);
			glTranslatef(posicion[0], 0, w1[2]);		//Palos que sujetan el cartel
			glRotatef(-90, 1, 0, 0);
			if (modo == SOLIDO) {
				glutSolidCylinder(0.1, 4, 5, 5);
			}
			else { glColor3f(1, 1, 1);  glutWireCylinder(0.1, 4, 5, 5); }
			glPopMatrix();
			glPushMatrix();
			Seta(posicion[0] - 3, w1[2] - 6);
			glPopMatrix();
			glPushMatrix();
			Seta(posicion[0] - 3, w1[2] + 12);
			glPopMatrix();
			glPushMatrix();
			glColor3f(0.4, 0.4, 0.4);
			glTranslatef(posicion[0], 0, w0[2]);
			glRotatef(-90, 1, 0, 0);
			if (modo == SOLIDO) {
				glutSolidCylinder(0.1, 4, 5, 5);
			}
			else { glColor3f(1, 1, 1);  glutWireCylinder(0.1, 4, 5, 5); }
			glColor3f(1, 1, 1);
			glPopMatrix();
			made2 = true;
		}
		if (modo == SOLIDO) {
			glPushMatrix();
			/*glMaterialfv(GL_FRONT, GL_DIFFUSE, Dm);
			glMaterialfv(GL_FRONT, GL_SPECULAR, Sm); //Aplicar Material al quad
			glMaterialf(GL_FRONT, GL_SHININESS, s);*/
			glBindTexture(GL_TEXTURE_2D, rb[0]);
			quadtex(v0, v1, v2, v3, tex, inc+tex, 0, 1,100,100); //Dibuja el quad
			glPopMatrix();
		}
		else { quad(v1, v2, v3, v0, 1, 10); }

		n[1] = (1 / sqrt(1 + fud(u) * fud(u))); //Calcula nuevos puntos del quad
		n[0] = n[1] * -fud(u);
		v1[0] = u + n[0] * 5 / 2;
		v1[2] = fu(u) + n[1] * 5 / 2;
		v2[0] = u - n[0] * 5 / 2;
		v2[2] = fu(u) - n[1] * 5 / 2;

		if (u > 33 + aux && !made3) {
			posicion[0] = aux + 33;
			posicion[2] = fu(posicion[0]);
			glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, dir_central); //Aparece farola 3
			glLightfv(GL_LIGHT4, GL_POSITION, posicion);
			float w0[3] = { posicion[0], 1.75, fu(posicion[0]) + 2.75 };
			float w1[3] = { posicion[0], 1.75, fu(posicion[0]) - 2.75 };
			float w2[3] = { posicion[0], 4, w1[2] };
			float w3[3] = { posicion[0], 4, w0[2] };
			if (modo == SOLIDO) {
				glBindTexture(GL_TEXTURE_2D, sing[calculaCartel(2)]); //Pintar cartel semi lejano
				quadtex(w0, w1, w2, w3, 0, 1, 0, 1, 50, 50);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else { quad(w0, w1, w2, w3, 1, 10); }
			made3 = true;
			glPushMatrix();
			glColor3f(0.4, 0.4, 0.4);
			glTranslatef(posicion[0], 0, w1[2]);		//Palos que sujetan el cartel
			glRotatef(-90, 1, 0, 0);
			if (modo == SOLIDO) {
				glutSolidCylinder(0.1, 4, 5, 5);
			}
			else { glColor3f(1, 1, 1);  glutWireCylinder(0.1, 4, 5, 5); }
			glPopMatrix();
			glPushMatrix();
			Seta(posicion[0] - 3, w1[2] - 6);
			glPopMatrix();
			glPushMatrix();
			Seta(posicion[0] - 3, w1[2] + 12);
			glPopMatrix();
			glPushMatrix();
			glColor3f(0.4, 0.4, 0.4);
			glTranslatef(posicion[0], 0, w0[2]);
			glRotatef(-90, 1, 0, 0);
			if (modo == SOLIDO) {
				glutSolidCylinder(0.1, 4, 5, 5);
			}
			else { glColor3f(1, 1, 1); glutWireCylinder(0.1, 4, 5, 5); }
			glColor3f(1, 1, 1);
			glPopMatrix();
		}
		if (u > 44 + aux && !made4) {
			posicion[0] = aux + 44;
			posicion[2] = fu(posicion[0]);
			glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, dir_central); //Aparece farola 4
			glLightfv(GL_LIGHT5, GL_POSITION, posicion);
			float w0[3] = { posicion[0], 1.75, fu(posicion[0]) + 2.75 };
			float w1[3] = { posicion[0], 1.75, fu(posicion[0]) - 2.75 };
			float w2[3] = { posicion[0], 4, w1[2] };
			float w3[3] = { posicion[0], 4, w0[2] };
			if (modo == SOLIDO) {
				glBindTexture(GL_TEXTURE_2D, sing[calculaCartel(3)]); //Pintar cartel lejano
				quadtex(w0, w1, w2, w3, 0, 1, 0, 1, 50, 50);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else { quad(w0, w1, w2, w3, 1, 10); }
			glPushMatrix();
			glColor3f(0.4, 0.4, 0.4);
			glTranslatef(posicion[0], 0, w1[2]);		//Palos que sujetan el cartel
			glRotatef(-90, 1, 0, 0);
			if (modo == SOLIDO) {
				glutSolidCylinder(0.1, 4, 5, 5);
			}
			else { glColor3f(1, 1, 1); glutWireCylinder(0.1, 4, 5, 5); }
			glPopMatrix();
			glPushMatrix();
			Seta(posicion[0] - 3, w1[2] - 6);
			glPopMatrix();
			glPushMatrix();
			Seta(posicion[0] - 3, w1[2] + 12);
			glPopMatrix();
			glPushMatrix();
			glColor3f(0.4, 0.4, 0.4);
			glTranslatef(posicion[0], 0, w0[2]);
			glRotatef(-90, 1, 0, 0);
			if (modo == SOLIDO) {
				glutSolidCylinder(0.1, 4, 5, 5);
			}
			else { glColor3f(1, 1, 1); glutWireCylinder(0.1, 4, 5, 5); }
			glColor3f(1, 1, 1);
			glPopMatrix();
			made4 = true;
		}
		if (modo == SOLIDO) {
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, Dm);
			glMaterialfv(GL_FRONT, GL_SPECULAR, Sm); //Aplicar Material al quad
			glMaterialf(GL_FRONT, GL_SHININESS, s);
			glBindTexture(GL_TEXTURE_2D, rb[0]);
			quadtex(v3, v0, v1, v2, tex+inc, tex+inc*2, 0, 1, 100, 100); //Dibuja el quad
			glPopMatrix();
		}
		else { quad(v0, v1, v2, v3, 1, 10); }
		u++;
		if (u>= int(pos[0]) + 48) { made1 = false, made2 = false, made3 = false, made4 = false; }
		
	}
	if (int(pos[0])>11+aux) { //Pasada 1 farola genera la siguiente
		aux += 11;
		loadSing1(((int(aux) / 11) % 4) - 1); //Comentar para eliminar generación aleatoria de carteles
	}

	if (delante && modo == SOLIDO) { //si se cambia la razon se ven los limites del quad
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		float aumento = 0.7;
		c0[0] = pos[0] + cos(v_d - aumento) * 3;
		c0[2] = pos[2] + sin(v_d - aumento) * 3;
		c1[0] = pos[0] + cos(v_d + aumento) * 3;
		c1[2] = pos[2] + sin(v_d + aumento) * 3;

		c2[0] = c1[0];
		c2[2] = c1[2];
		c3[0] = c0[0];
		c3[2] = c0[2];
		glPushMatrix();
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D, del[0]);
		quadtex(c0, c1, c2, c3, 0, 1, 0, 1, 1, 10);
		glPopMatrix();
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
	glutSwapBuffers();
}

float puntos(int buenos, int malos) {
	return buenos * 0.1 - malos * 0.3;
}

void onSpecialKey(int specialKey, int x, int y) {
	switch (specialKey) {
	case GLUT_KEY_LEFT:
		v_d -= 0.25 * PI / 180;
		break;
	case GLUT_KEY_RIGHT:
		v_d += 0.25 * PI / 180;
		break;
	case GLUT_KEY_UP:
		velocidad += 0.1;
		break;
	case GLUT_KEY_DOWN:
		if (velocidad>=0.1) {
			velocidad -= 0.1;
		}
		else { velocidad = 0; }
		break;
	}
	stringstream titulo;
	titulo << "ISG::P9:: Juego Coche velocidad: " << velocidad << " m/s";
	if (race) {
		titulo << ", " << "CP(dentro): " << points << " CP(fuera): " << saltados << " Puntos: " << puntos(points,saltados);
	}
	glutSetWindowTitle(titulo.str().c_str());
	glutPostRedisplay();
}

void onKey(unsigned char tecla, int x, int y) {
	switch (tecla) {
	case 's':
		if (modo == ALAMBRICO) {
			modo = SOLIDO;
			printf("Modo SOLIDO activado\n");
		}
		else { modo = ALAMBRICO; printf("Modo ALAMBRICO activado\n"); }
		break;
	case 'S':
		if (modo == ALAMBRICO) {
			modo = SOLIDO;
			printf("Modo SOLIDO activado\n");
		}
		else { modo = ALAMBRICO; printf("Modo ALAMBRICO activado\n"); }
		break;
	case 'l':
		noche = !noche;
		if (noche) { printf("Modo NOCHE activado\n"); }
		else { printf("Modo NOCHE desactivado\n"); }
		break;
	case 'L':
		noche = !noche;
		if (noche) { printf("Modo NOCHE activado\n"); }
		else { printf("Modo NOCHE desactivado\n"); }
		break;
	case 'n':
		niebla = !niebla;
		if (niebla) { printf("NIEBLA activada\n"); }
		else { printf("NIEBLA desactivada\n"); }
		break;
	case 'N':
		niebla = !niebla;
		if (niebla) { printf("NIEBLA activada\n"); }
		else { printf("NIEBLA desactivada\n"); }
		break;
	case 'c':
		delante = !delante;
		if (delante) { printf("Salpicadero visible\n"); }
		else { printf("Salpicadero invisible\n"); }
		break;
	case 'C':
		delante = !delante;
		if (delante) { printf("Salpicadero visible\n"); }
		else { printf("Salpicadero invisible\n"); }
		break;
	case 'r':
		race = !race;
		if (race) {
			printf("Modo CARRERA activado\n");
			terror = false;
			points = 0;
			saltados = 0;
			if (!mute) {
				engine->stopAllSounds();
				engine->play2D("music.mp3", true);
			}
		}
		else { printf("Modo CARRERA desactivado\n"); engine->stopAllSounds(); }
		break;
	case 'R':
		race = !race;
		if (race) {
			printf("Modo CARRERA activado\n");
			terror = false;
			points = 0;
			saltados = 0;
			if (!mute) {
				engine->stopAllSounds();
				engine->play2D("music.mp3", true);
			}
		}
		else { printf("Modo CARRERA desactivado\n"); engine->stopAllSounds(); }
		break;
	case 'w':
		w = (w + 1) % 3;
		switch (w)
		{
		case 0:
			printf("Estacion: Primavera\n");
			break;
		case 1:
			printf("Estacion: Otono\n");
			break;
		case 2:
			printf("Estacion: Invierno\n");
			break;
		}
		break;
	case 'W':
		w = (w + 1) % 3;
		switch (w)
		{
		case 0:
			printf("Estacion: Primavera\n");
			break;
		case 1:
			printf("Estacion: Otono\n");
			break;
		case 2:
			printf("Estacion: Invierno\n");
			break;
		}
		break;
	case 't':
		terror = !terror;
		if (terror) {
			printf("Modo TERROR activado\n");
			race = false;
			if (!mute) {
				engine->stopAllSounds();
				engine->play2D("cementerio.mp3", true);
				engine->play2D("ambiente.mp3", true);
			}
		}
		else { printf("Modo TERROR desactivado\n"); engine->stopAllSounds(); }
		break;
	case 'T':
		terror = !terror;
		if (terror) {
			printf("Modo TERROR activado\n");
			race = false;
			if (!mute) {
				engine->stopAllSounds();
				engine->play2D("cementerio.mp3", true);
				engine->play2D("ambiente.mp3", true);
			}
		}
		else { printf("Modo TERROR desactivado\n"); engine->stopAllSounds(); }
		break;
	case 'm':
		mute = !mute;
		if (mute) { 
			engine->stopAllSounds(); 
			printf("Sonidos eliminados\n");
		}
		else {
			printf("Sonidos activados\n");
			if (terror) {
				engine->stopAllSounds();
				engine->play2D("cementerio.mp3", true);
				engine->play2D("ambiente.mp3", true);
			}
			if (race) {
				engine->stopAllSounds();
				engine->play2D("music.mp3", true);
			}
		}
		break;
	case 'M':
		mute = !mute;
		if (mute) { engine->stopAllSounds(); }
		else {
			if (terror) {
				engine->stopAllSounds();
				engine->play2D("cementerio.mp3", true);
				engine->play2D("ambiente.mp3", true);
			}
			if (race) {
				engine->stopAllSounds();
				engine->play2D("music.mp3", true);
			}
		}
		break;
	case 27: // Pulso escape
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void onTimer(int valor)
// Funcion de atencion al evento idle
{
	static int antes = 0;
	int ahora, tiempo_transcurrido;
	ahora = glutGet(GLUT_ELAPSED_TIME); //Tiempo transcurrido desde el inicio
	tiempo_transcurrido = ahora - antes;
	pos[0] += velocidad * cos(v_d) * (tiempo_transcurrido / 1000.0);
	pos[2] += velocidad * sin(v_d) * (tiempo_transcurrido / 1000.0);
	antes = ahora;
	glutTimerFunc(1000 / tasaFPS, onTimer, tasaFPS); // Se encola un nuevo timer
	glutPostRedisplay();
}
void onScream(int valor) {
	scream = true;
	glutTimerFunc(valor, onScream, valor);
}
void main(int argc, char** argv)
// Programa principal
{
	glutInit(&argc, argv); // Inicializacion de GLUT
	glutInitWindowSize(600, 400); // Tamanyo inicial de la ventana
	glutInitWindowPosition(50, 200);
	glutCreateWindow(PROYECTO); // Creacion de la ventana con su titulo
	std::cout << PROYECTO << " running" << std::endl; // Mensaje por consola
	glutDisplayFunc(display); // Alta de la funcion de atencion a display
	glutReshapeFunc(reshape); // Alta de la funcion de atencion a reshape
	glutSpecialFunc(onSpecialKey);
	glutKeyboardFunc(onKey);
	glutTimerFunc(1000 / tasaFPS, onTimer, tasaFPS);
	int mils = 10000 * ((rand() % 10)+1);
	glutTimerFunc(mils, onScream, mils);
	//int mil = (rand() % 9) * 100;
	//glutTimerFunc(mil, onScream, mil);
	init();
	glutMainLoop(); // Puesta en marcha del programa
	FreeImage_DeInitialise();
}