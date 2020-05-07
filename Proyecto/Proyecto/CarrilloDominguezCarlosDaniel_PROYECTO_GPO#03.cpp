

#include "texture.h"
#include "figuras.h"
#include "Camera.h"

#include "cmodel/CModel.h"

#if (_MSC_VER == 1900)
#   pragma comment( lib, "legacy_stdio_definitions.lib" )
#endif

//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////
static GLuint ciudad_display_list;	//Display List for the Monito


//******************************************************
//NEW// Keyframes
//Variables de dibujo y manipulacion
float posX =0, posY = 0, posZ =-3.5, giroBancoIzq = 0, giroBancoCen = 0, giroBancoDer = 0 ;
//float giroBanco = 0;

#define MAX_FRAMES 20
int i_max_steps = 90;
int i_curr_steps = 0;

typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	
	float giroBancoCen;
	float giroBancoCenInc;
	float giroBancoIzq;
	float giroBancoIzqInc;
	float giroBancoDer;
	float giroBancoDerInc;
	
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex=0;			//introducir datos
bool play=false;
int playIndex=0;
bool play2 = false;


//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

int w = 500, h = 500;
int frame=0,time,timebase=0;
char s[30];

CCamera objCamera;	//Create objet Camera

GLfloat g_lookupdown = 0.0f;    // Look Position In The Z-Axis (NEW) 

int font=(int)GLUT_BITMAP_HELVETICA_18;


//GLfloat Diffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };				// Diffuse Light Values
GLfloat Diffuse[]= { 0.5f, 0.5f, 0.5f, 1.0f };				// Diffuse Light Values
GLfloat Specular[] = { 1.0, 1.0, 1.0, 1.0 };				// Specular Light Values
GLfloat Position[]= { 0.0f, 7.0f, -5.0f, 0.0f };			// Light Position
GLfloat Position2[]= { 0.0f, 0.0f, -5.0f, 1.0f };			// Light Position

GLfloat m_dif1[] = { 0.0f, 0.2f, 1.0f, 1.0f };				// Diffuse Light Values
GLfloat m_spec1[] = { 0.0, 0.0, 0.0, 1.0 };				// Specular Light Values
GLfloat m_amb1[] = { 0.0, 0.0, 0.0, 1.0 };				// Ambiental Light Values
GLfloat m_s1[] = {18};

GLfloat m_dif2[] = { 0.8f, 0.2f, 0.0f, 1.0f };				// Diffuse Light Values
GLfloat m_spec2[] = { 0.0, 0.0, 0.0, 1.0 };				// Specular Light Values
GLfloat m_amb2[] = { 0.0, 0.0, 0.0, 1.0 };				// Ambiental Light Values
GLfloat m_s2[] = {22};


CTexture text1; //Madera
CTexture text2; //SillasEXT
CTexture text3;	//Alberca
CTexture text4;	//Piso
CTexture text5;	//barandal
CTexture text6;	//Vidrio
CTexture textSM; //Marco de la silla exteriores
CTexture textAlberca;

//************TEXTURAS CIELO******************
//***Texturas para el cielo***
CTexture textSkyboxUnica;
CTexture textSkyboxFrontal;
CTexture textSkyboxAtras;
CTexture textSkyboxIzquierda;
CTexture textSkyboxDerecha;
CTexture textSkyboxArriba;
CTexture textSkyboxAbajo;

//***Texturas casa fachada***
CTexture textpared;
CTexture textMarco;
CTexture textPisoCasa;
CTexture textGlassWasser;
CTexture textPuerta;
CTexture textventana;
CTexture textMesa;
CTexture textPata;
CTexture textWall;
CTexture textSilla;
CTexture textTecho;
CTexture textMarble;
CTexture tapete;
CTexture textMesaT;
CTexture textTele;
CTexture textTele2;
CTexture textCuadro1;
CTexture textburo;
CTexture textsillon;
CTexture textsillon2;
CTexture textcojin;
CTexture textsillon3;
CTexture textbanco;
CTexture textpelota;
CTexture texthumo;
CFiguras techoCasa;
CFiguras pisoCasa;
CFiguras cubo;
CFiguras sky;
CFiguras figSkybox;


//END NEW//////////////////////////////////////////

CFiguras fig1;
CFiguras fig2;
CFiguras fig3;
CFiguras fig4;	//Cilindro mesa
CFiguras fig5;	//Casa01
CFiguras fig6;
CFiguras fig7;	//Para crear Monito
CFiguras figMarco;
CFiguras figtapete;
CFiguras figtele;
CFiguras figtele2;
CFiguras figburo;
CFiguras figcuadro;
CFiguras figsillon;
CFiguras figBanco;
CFiguras figpelota;
CFiguras fighumo;

//Modelos 3D
CModel amaca;
CModel florero;
CModel palmera;

//Animacion
float movPX = 0.0;
float movPY = 0.0;
float movPZ = 0.0;
float rotP = 0.0;
bool g_fanimacion = false;
bool g_avanza = false;

void saveFrame ( void )
{
	
	printf("frameindex %d\n",FrameIndex);			

	KeyFrame[FrameIndex].posX=posX;
	KeyFrame[FrameIndex].posY=posY;
	KeyFrame[FrameIndex].posZ=posZ;

	
	KeyFrame[FrameIndex].giroBancoIzq = giroBancoIzq;
	KeyFrame[FrameIndex].giroBancoCen = giroBancoCen;
	KeyFrame[FrameIndex].giroBancoDer = giroBancoDer;
			
	FrameIndex++;
}

void resetElements( void )
{
	posX=KeyFrame[0].posX;
	posY=KeyFrame[0].posY;
	posZ=KeyFrame[0].posZ;

	giroBancoIzq = KeyFrame[0].giroBancoIzq;
	giroBancoCen = KeyFrame[0].giroBancoCen;
	giroBancoDer = KeyFrame[0].giroBancoDer;

}
void interpolation ( void )
{
	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;	
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;	
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;	

	KeyFrame[playIndex].giroBancoCenInc = (KeyFrame[playIndex + 1].giroBancoCen - KeyFrame[playIndex].giroBancoCen) / i_max_steps;
	KeyFrame[playIndex].giroBancoIzqInc = (KeyFrame[playIndex + 1].giroBancoIzq - KeyFrame[playIndex].giroBancoIzq) / i_max_steps;
	KeyFrame[playIndex].giroBancoDerInc = (KeyFrame[playIndex + 1].giroBancoDer - KeyFrame[playIndex].giroBancoDer) / i_max_steps;

}

void humo()
{	
	glTranslatef(300.0, 30,15.0);
	glPushMatrix();
		glDisable(GL_LIGHTING);
		glEnable(GL_ALPHA_TEST);
		
		glAlphaFunc(GL_GREATER, 0.1);
		
		glBindTexture(GL_TEXTURE_2D, texthumo.GLindex);
		glBegin(GL_QUADS); //plano
		glColor3f(1.0, 1.0, 1.0);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0, 20.0, 0.0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0, 20.0, 0.0);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glRotatef(45, 0, 1, 0);
		glBegin(GL_QUADS); //plano
		glColor3f(1.0, 1.0, 1.0);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0, 20.0, 0.0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0, 20.0, 0.0);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glRotatef(-45, 0, 1, 0);
		glBegin(GL_QUADS); //plano
		glColor3f(1.0, 1.0, 1.0);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0, 20.0, 0.0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0, 20.0, 0.0);
		glEnd();
		glPopMatrix();

		glPushMatrix();


		glPopMatrix();


		glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glBegin(GL_QUADS); //plano
		glColor3f(1.0, 1.0, 1.0);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0, 20.0, 0.0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0, 20.0, 0.0);
		glEnd();
		glDisable(GL_ALPHA_TEST);
		//glDisable(GL_BLEND);        // Turn Blending Off
		//glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On
		glEnable(GL_LIGHTING);
	glPopMatrix();

	
	return;
}


void ciudad ()
{

		glPushMatrix(); //Camino1
			glTranslatef(23.5,0.0,-10.5);
			glScalef(40,0.1,59.5);
			glDisable(GL_LIGHTING);
			fig3.prisma2(text4.GLindex, text4.GLindex);
			glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix(); //Camino2
			glTranslatef(27.0,0.0,22.25);
			glScalef(33,0.1,7.5);
			glDisable(GL_LIGHTING);
			fig3.prisma2(text4.GLindex, text4.GLindex);
			glEnable(GL_LIGHTING);
		glPopMatrix();

		
		
		

}


void bara()
{
	glPushMatrix(); //tubo1
		glTranslatef(0.0, 1.0, 0.0);
		glDisable(GL_LIGHTING);
		fig4.cilindro(0.1,1.5,20.0,text5.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix(); //tubo2
		glTranslatef(0.0, 1.0, 2.5);
		glDisable(GL_LIGHTING);
		fig4.cilindro(0.1, 1.5, 20.0, text5.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix(); //vidrio
		glTranslatef(0.0, 2.0, 1.2);
		glRotatef(90, 0.0, 1.0, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		glScalef(2.4, 0.1, 0.75);
		glDisable(GL_LIGHTING);
		fig4.prisma2(text6.GLindex, text6.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix(); //vidrio 2
		glTranslatef(0.0, 2.0, -1.2);
		glRotatef(90, 0.0, 1.0, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		glScalef(2.4, 0.1, 0.75);
		glDisable(GL_LIGHTING);
		fig4.prisma2(text6.GLindex, text6.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //tubo3
		glTranslatef(0.0, 1.0, -2.5);
		glDisable(GL_LIGHTING);
		fig4.cilindro(0.1, 1.5, 20.0, text5.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //vidrio 3
		glTranslatef(0.0, 2.0, -3.5);
		glRotatef(90, 0.0, 1.0, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		glScalef(2.4, 0.1, 0.75);
		glDisable(GL_LIGHTING);
		fig4.prisma2(text6.GLindex, text6.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //tubo4
		glTranslatef(0.0, 1.0, -4.75);
		glDisable(GL_LIGHTING);
		fig4.cilindro(0.1, 1.5, 20.0, text5.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();


	//glEndList();
}


void amacaa()
{
	glPushMatrix(); 
		glDisable(GL_LIGHTING);
		glTranslatef(0.0, 0.0, 0.0);
		glScalef(.01, .01, .01);
		amaca.GLrender(NULL, _SHADED, 1);
		glEnable(GL_LIGHTING);
	glPopMatrix();
}


void salaExt ()
{
	glPushMatrix(); //Mesa 1
		glTranslatef(0.0, 1.0, 0.0);
		glDisable(GL_LIGHTING);
		fig4.cilindro(2.0, 0.01,180, text1.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //Mesa 2
		glTranslatef(0.0, 0.5, 0.0);
		glDisable(GL_LIGHTING);
		fig4.cilindro(1, 0.5, 40, text1.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //Silla 1 izq
		glTranslatef(0.0, 1.0, -4.0);
		glDisable(GL_LIGHTING);
		fig2.prisma(1.0,1.0,1.0, text2.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //Silla 2 izq
		glTranslatef(0.0, 1.3, -4.61);
		glDisable(GL_LIGHTING);
		fig2.prisma(1.5, 1.0, 0.3, textSM.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //Silla 3 izq
		glTranslatef(0.65, 1.3, -4.25);
		glRotatef(90, 0.0, 1.0, 0.0);
		glDisable(GL_LIGHTING);
		fig2.prisma(1.5, 1.0, 0.3, textSM.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //Silla 4 izq
		glTranslatef(-0.65, 1.3, -4.25);
		glRotatef(90, 0.0, 1.0, 0.0);
		glDisable(GL_LIGHTING);
		fig2.prisma(1.5, 1.0, 0.3, textSM.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //Silla 1 der
		glTranslatef(0.0, 1.0, 4.0);
		glDisable(GL_LIGHTING);
		fig2.prisma(1.0, 1.0, 1.0, text2.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //Silla 2 der
		glTranslatef(0.0, 1.3, 4.61);
		glDisable(GL_LIGHTING);
		fig2.prisma(1.5, 1.0, 0.3, textSM.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //Silla 3 der
		glTranslatef(0.65, 1.3, 4.25);
		glRotatef(90, 0.0, 1.0, 0.0);
		glDisable(GL_LIGHTING);
		fig2.prisma(1.5, 1.0, 0.3, textSM.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //Silla 4 izq
		glTranslatef(-0.65, 1.3, 4.25);
		glRotatef(90, 0.0, 1.0, 0.0);
		glDisable(GL_LIGHTING);
		fig2.prisma(1.5, 1.0, 0.3, textSM.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //Silla 1 atr
		glTranslatef(4.0, 1.0, 0.0);
		glDisable(GL_LIGHTING);
		fig2.prisma(1.0, 1.0, 1.0, text2.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //Silla 2 atr
		glTranslatef(4.61, 1.3, 0.0 );
		glRotatef(90, 0.0, 1.0, 0.0);
		glDisable(GL_LIGHTING);
		fig2.prisma(1.5, 1.0, 0.3, textSM.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //Silla 3 atr
		glTranslatef(4.25, 1.3, 0.65 );
		glDisable(GL_LIGHTING);
		fig2.prisma(1.5, 1.0, 0.3, textSM.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //Silla 4 atr
		glTranslatef(4.25 , 1.3, -0.65);
		glDisable(GL_LIGHTING);
		fig2.prisma(1.5, 1.0, 0.3, textSM.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //Silla 1 adl
		glTranslatef(-4.0, 1.0, 0.0);
		glDisable(GL_LIGHTING);
		fig2.prisma(1.0, 1.0, 1.0, text2.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //Silla 2 adl
		glTranslatef(-4.61, 1.3, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		glDisable(GL_LIGHTING);
		fig2.prisma(1.5, 1.0, 0.3, textSM.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //Silla 3 adl
		glTranslatef(-4.25, 1.3, 0.65);
		glDisable(GL_LIGHTING);
		fig2.prisma(1.5, 1.0, 0.3, textSM.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix(); //Silla 4 adl
		glTranslatef(-4.25, 1.3, -0.65);
		glDisable(GL_LIGHTING);
		fig2.prisma(1.5, 1.0, 0.3, textSM.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();

}


void alberca ()
{
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix(); //Alberca
		glTranslatef(0.0, 1.05, 0.0);
		glScalef(8.5, 0.01, 15);
		glDisable(GL_LIGHTING);
		fig3.prisma_anun(text3.GLindex, text3.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix(); //Alberca2
		glTranslatef(0.0, 1.0, 0.0);
		glRotatef(90,0.0,0.0,1.0);
		glDisable(GL_LIGHTING);
		fig3.prisma(9.5, 0.1,16.,text5.GLindex);
		glEnable(GL_LIGHTING);
	glPopMatrix();

}



//NEW CASA//////////////////////////////////////

void bancooIzq()
{
	glPushMatrix();
	glRotatef(giroBancoIzq, 0.0, 1.0, 0.0);
		glPushMatrix();// base 1	
			figBanco.cono(2, 1.2, 70.0, text5.GLindex);
		glPopMatrix();
		glPushMatrix();//Tubo
			glTranslatef(0.0,1.0,0.0);
			figBanco.cilindro(0.5,10.0,40,text5.GLindex);
		glPopMatrix();
		glPushMatrix();//cojin 1
			glTranslatef(0.0, 11.0, 0.0);
			figBanco.prisma(0.5, 3.0, 3, textbanco.GLindex);
		glPopMatrix();
		glPushMatrix();//cojin 2
			glTranslatef(1.40, 14.0, 0.0);
			glRotatef(90,0.0,0.0,1.0);
			figBanco.prisma(0.25, 6.5, 3, textbanco.GLindex);
		glPopMatrix();
	glPopMatrix();
}

void bancooCen()
{
	glPushMatrix();
	glRotatef(giroBancoCen, 0.0, 1.0, 0.0);
		glPushMatrix();// base 1	
		figBanco.cono(2, 1.2, 70.0, text5.GLindex);
		glPopMatrix();
		glPushMatrix();//Tubo
		glTranslatef(0.0, 1.0, 0.0);
		figBanco.cilindro(0.5, 10.0, 40, text5.GLindex);
		glPopMatrix();
		glPushMatrix();//cojin 1
		glTranslatef(0.0, 11.0, 0.0);
		figBanco.prisma(0.5, 3.0, 3, textbanco.GLindex);
		glPopMatrix();
		glPushMatrix();//cojin 2
		glTranslatef(1.40, 14.0, 0.0);
		glRotatef(90, 0.0, 0.0, 1.0);
		figBanco.prisma(0.25, 6.5, 3, textbanco.GLindex);
		glPopMatrix();
	glPopMatrix();
}

void bancooDer()
{
	glPushMatrix();
	glRotatef(giroBancoDer, 0.0, 1.0, 0.0);
		glPushMatrix();// base 1	
		figBanco.cono(2, 1.2, 70.0, text5.GLindex);
		glPopMatrix();
		glPushMatrix();//Tubo
		glTranslatef(0.0, 1.0, 0.0);
		figBanco.cilindro(0.5, 10.0, 40, text5.GLindex);
		glPopMatrix();
		glPushMatrix();//cojin 1
		glTranslatef(0.0, 11.0, 0.0);
		figBanco.prisma(0.5, 3.0, 3, textbanco.GLindex);
		glPopMatrix();
		glPushMatrix();//cojin 2
		glTranslatef(1.40, 14.0, 0.0);
		glRotatef(90, 0.0, 0.0, 1.0);
		figBanco.prisma(0.25, 6.5, 3, textbanco.GLindex);
		glPopMatrix();
	glPopMatrix();
}

void sillon3()
{
	glPushMatrix();// base 1
		glTranslatef(16.0, -5.20, 5.0);
		figsillon.cilindro(2.0,2.5, 70.0, textsillon2.GLindex);
	glPopMatrix();
	glPushMatrix();//Pata sillon  1
		glTranslatef(16.0, -5.5, 4.25);
		glRotatef(180, 0.0, 0.0, 1.0);
		figburo.cono(4.75, 0.5, 30, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();//Pata sillon  2	
		glTranslatef(14.75, -5.5, 5.);
		glRotatef(180, 0.0, 0.0, 1.0);
		figburo.cono(4.75, 0.5, 30, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();//Pata sillon  3	
		glTranslatef(17., -5.5, 5.5);
		glRotatef(180, 0.0, 0.0, 1.0);
		figburo.cono(4.75, 0.5, 30, textburo.GLindex);
	glPopMatrix();
}

void sillon2()
{

	glPushMatrix();//Pata sillon  1
		glTranslatef(8.5, -4.65, -14.25);
		glRotatef(180, 0.0, 0.0, 1.0);
		figburo.cono(4.75, 0.5, 30, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();//Pata sillon  2
		glTranslatef(8.5, -4.65, -17.75);
		glRotatef(180, 0.0, 0.0, 1.0);
		figburo.cono(4.75, 0.5, 30, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();//Pata sillon  3
		glTranslatef(11.25, -4.65, -17.75);
		glRotatef(180, 0.0, 0.0, 1.0);
		figburo.cono(4.75, 0.5, 30, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();//Pata sillon  4
		glTranslatef(11.25, -4.65, -14.25);
		glRotatef(180, 0.0, 0.0, 1.0);
		figburo.cono(4.75, 0.5, 30, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();// base 1
		glTranslatef(10.0, -3.5, -16.0);
		figsillon.prisma(2.0, 4.0, 5.0, textsillon2.GLindex);
	glPopMatrix();
	glPushMatrix();// base 2
		glTranslatef(10.0, 1.0, -18.6);
		figsillon.prisma(10.0, 4.0, 0.5, textsillon2.GLindex);
	glPopMatrix();
	glPushMatrix();// base 3
		glTranslatef(12.1, 1.0, -16.5);
		glRotatef(90,0.0,1.0,0.0 );
		figsillon.prisma(8.0, 4.0, 0.5, textsillon2.GLindex);
	glPopMatrix();
	glPushMatrix();// base 4
		glTranslatef(7.9, 1.0, -16.5);
		glRotatef(90, 0.0, 1.0, 0.0);
		figsillon.prisma(8.0, 4.0, 0.5, textsillon2.GLindex);
	glPopMatrix();
	glPushMatrix();// base 5
		glTranslatef(10.0, 1.1, -16.6);
		glRotatef(-45,1.0,0.0,0.0);
		figsillon.prisma(5.0, 2.0, 0.5, textcojin.GLindex);
	glPopMatrix();

	glPushMatrix();// Cojin 1
		glColor3f(1, 1, 1);
		glTranslatef(10.0, -1.75, -16.0);
		figsillon.prisma(1.5, 4.0, 5.0, textsillon2.GLindex);
	glPopMatrix();
}

void sillon()
{
	glPushMatrix();// Cojin 1
		glTranslatef(0.5,-1.3,13.0);
		figsillon.prisma(1.5, 5.0, 4.0, textsillon.GLindex);
	glPopMatrix();
	glPushMatrix();// Cojin 2
		glTranslatef(-7.0, -1.3, 12.75);
		figsillon.prisma(1.5, 10.0, 3.25, textsillon.GLindex);
	glPopMatrix();
	glPushMatrix();// Cojin 3
		glTranslatef(-10.75, -1.3, 3.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		figsillon.prisma(1.5, 20.0, 2.6, textsillon.GLindex);
	glPopMatrix();
	glPushMatrix();// Cojin 4
		glTranslatef(-11.8, 2.2, 3.5);
		figsillon.prisma(5.83, 0.5, 21.0, textsillon.GLindex);
	glPopMatrix();
	glPushMatrix();// Cojin 5
		glTranslatef(-7.0, 2.2, 14.25);
		glRotatef(90,0.0,1.0,0.0);
		figsillon.prisma(5.83, 0.5, 10.0, textsillon.GLindex);
	glPopMatrix();
	glPushMatrix();//Pata sillon  1
		glTranslatef(-9.0, -4.75,14.5);
		glRotatef(180, 0.0, 0.0, 1.0);
		figburo.cono(4.75, 0.5, 30, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();//Pata sillon 2
		glTranslatef(-9.0, -4.75, 11.5);
		glRotatef(180, 0.0, 0.0, 1.0);
		figburo.cono(4.75, 0.5, 30, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();//Pata sillon  3
		glTranslatef(1.5, -4.75, 14.5);
		glRotatef(180, 0.0, 0.0, 1.0);
		figburo.cono(4.75, 0.5, 30, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();//Pata sillon  4
		glTranslatef(1.5, -4.75, 11.5);
		glRotatef(180, 0.0, 0.0, 1.0);
		figburo.cono(4.75, 0.5, 30, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();//Pata sillon  1.a
		glTranslatef(-12.0, -4.75, 14.5);
		glRotatef(180, 0.0, 0.0, 1.0);
		figburo.cono(4.75, 0.5, 30, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();//Pata sillon  2.a
		glTranslatef(-12.0, -4.75, -5.5);
		glRotatef(180, 0.0, 0.0, 1.0);
		figburo.cono(4.75, 0.5, 30, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();//Pata sillon  3.a
		glTranslatef(-10.0, -4.75, -5.5);
		glRotatef(180, 0.0, 0.0, 1.0);
		figburo.cono(4.75, 0.5, 30, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();// base 3
		glColor3f(0.709, 0.709, 0.709);
		glTranslatef(-12.25,1.25,4.0);
		figsillon.prisma(8.0, 0.5, 22.0, textsillon.GLindex);
	glPopMatrix();
	glPushMatrix();// base 4
		glColor3f(0.709, 0.709, 0.709);
		glTranslatef(-7.0, 1.25, 14.75);
		glRotatef(90,0.0,1.0,0.0);
		figsillon.prisma(8.0, 0.5, 10.0, textsillon.GLindex);
	glPopMatrix();
	glPushMatrix();// base 1
		glColor3f(0.709, 0.709, 0.709);
		glTranslatef(-4.0, -3.5, 13.0);
		figsillon.prisma(3.0, 14.0, 4.0, textsillon.GLindex);
	glPopMatrix();
	glPushMatrix();// base 2
		glColor3f(0.709, 0.709, 0.709);
		glTranslatef(-11.0, -3.5, 4.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		figsillon.prisma(3.0, 22.0, 3.0, textsillon.GLindex);
	glPopMatrix();

}

void mesaCentro()
{
	glPushMatrix();// Tabla 
		glTranslatef(1.0, -1.0, -4.0);
		figburo.prisma(0.5,5.75, 11.5, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();// pata 1
		glTranslatef(-1.0, -4.5, -9.5);
		figburo.prisma(7.4, 0.5, 0.5, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();// pata 2
		glTranslatef(3.0, -4.5, -9.5);
		figburo.prisma(7.4, 0.5, 0.5, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();// pata 3
		glTranslatef(3.0, -4.5, 1.0);
		figburo.prisma(7.4, 0.5, 0.5, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();// pata 4
		glTranslatef(-1.0, -4.5, 1.0);
		figburo.prisma(7.4, 0.5, 0.5, textburo.GLindex);
	glPopMatrix();

	return;
}

void buro()
{
	glPushMatrix();//Buro base
		figburo.prisma(9.0, 15.0, 3.0, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();//Pata buro izq adl
		glTranslatef(6.0, -4.5, -1.0);
		glRotatef(180,0.0,0.0,1.0);
		figburo.cono(4.75,0.5,30, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();//Pata buro der adl
		glTranslatef(-6.0, -4.5, -1.0);
		glRotatef(180, 0.0, 0.0, 1.0);
		figburo.cono(4.75, 0.5, 30, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();//Pata buro der atr
		glTranslatef(-6.0, -4.5, 1.0);
		glRotatef(180, 0.0, 0.0, 1.0);
		figburo.cono(4.75, 0.5, 30, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();//Pata buro izq atr
		glTranslatef(6.0, -4.5, 1.0);
		glRotatef(180, 0.0, 0.0, 1.0);
		figburo.cono(4.75, 0.5, 30, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();//Buro cuadrito 1
		glColor3f(1,1,1);
		glTranslatef(5.5,0.0,-1.6);
		figburo.prisma(7.0, 2.5, 0.1, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();//Buro cuadrito 2
		glColor3f(1, 1, 1);
		glTranslatef(1.75, 0.0, -1.6);
		figburo.prisma(7.0, 2.5, 0.1, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();//Buro cuadrito 3
		glColor3f(1, 1, 1);
		glTranslatef(-1.75, 0.0, -1.6);
		figburo.prisma(7.0, 2.5, 0.1, textburo.GLindex);
	glPopMatrix();
	glPushMatrix();//Buro cuadrito 4
		glColor3f(1, 1, 1);
		glTranslatef(-5.5, 0.0, -1.6);
		figburo.prisma(7.0, 2.5, 0.1, textburo.GLindex);
	glPopMatrix();

	return;
}



void pelota()
{
	glTranslatef(8.5,0.5,-14.6);
	figpelota.esfera(0.4,20,20,textpelota.GLindex);
}


void mesaTele(void)
{
	glPushMatrix();//Base Mesa Tele
		glColor3f(0.901, 0.901, 0.898);
		figMarco.prisma(8.0,15.0,2.0, textMesaT.GLindex);
	glPopMatrix();

	glPushMatrix();//Base Mesa Tele 2
		glColor3f(0.901, 0.901, 0.898);
		glTranslatef(0.0,5.31,0.0);
		figMarco.prisma(3.0, 8.0, 2.0, textMesaT.GLindex);
	glPopMatrix();
	glPushMatrix();//Pm tele frn arriba
		glColor3f(0.768, 0.768, 0.768);
		glTranslatef(0.0,3.9, -1.1);
		figMarco.prisma(0.3, 15.0, 0.2, textMesaT.GLindex);
	glPopMatrix();
	glPushMatrix();//Pm tele frn abajo
		glColor3f(0.768, 0.768, 0.768);
		glTranslatef(0.0, -4.1, -1.1);
		figMarco.prisma(0.3, 15.0, 0.2, textMesaT.GLindex);
	glPopMatrix();
	glPushMatrix();//Pm tele atr arriba
		glColor3f(0.768, 0.768, 0.768);
		glTranslatef(0.0, 3.9, 1.1);
		figMarco.prisma(0.3, 15.0, 0.2, textMesaT.GLindex);
	glPopMatrix();
	glPushMatrix();//Pm tele atr abajo
		glColor3f(0.768, 0.768, 0.768);
		glTranslatef(0.0, -4.1, 1.1);
		figMarco.prisma(0.3, 15.0, 0.2, textMesaT.GLindex);
	glPopMatrix();
	glPushMatrix();//Pm tele der abajo
		glColor3f(0.768, 0.768, 0.768);
		glTranslatef(-7.5, -4.0, 0.0);
		glRotatef(90,0.0,1.0,0.0);
		figMarco.prisma(0.3, 2.25, 0.2, textMesaT.GLindex);
	glPopMatrix();
	glPushMatrix();//Pm tele der arriba
		glColor3f(0.768, 0.768, 0.768);
		glTranslatef(-7.5, 4.0, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		figMarco.prisma(0.3, 2.25, 0.2, textMesaT.GLindex);
	glPopMatrix();
	glPushMatrix();//Pm tele izq arriba
		glColor3f(0.768, 0.768, 0.768);
		glTranslatef(7.5, 4.0, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		figMarco.prisma(0.3, 2.25, 0.2, textMesaT.GLindex);
	glPopMatrix();
	glPushMatrix();//Pm tele izq abajo
		glColor3f(0.768, 0.768, 0.768);
		glTranslatef(7.5, -4.0, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		figMarco.prisma(0.3, 2.25, 0.2, textMesaT.GLindex);
	glPopMatrix();
	glPushMatrix();//Pm tele frn arriba  2
		glColor3f(0.768, 0.768, 0.768);
		glTranslatef(0.0, 6.6, -1.1);
		figMarco.prisma(0.3, 8.0, 0.2, textMesaT.GLindex);
	glPopMatrix();
	glPushMatrix();//Pm tele atr arriba  2
		glColor3f(0.768, 0.768, 0.768);
		glTranslatef(0.0, 6.6, 1.1);
		figMarco.prisma(0.3, 8.0, 0.2, textMesaT.GLindex);
	glPopMatrix();
	glPushMatrix();//Pm tele der arriba 2
		glColor3f(0.768, 0.768, 0.768);
		glTranslatef(-4.0, 6.6, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		figMarco.prisma(0.3, 2.25, 0.2, textMesaT.GLindex);
	glPopMatrix();
	glPushMatrix();//Pm tele izq arriba 2
		glColor3f(0.768, 0.768, 0.768);
		glTranslatef(4.0, 6.6, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		figMarco.prisma(0.3, 2.25, 0.2, textMesaT.GLindex);
	glPopMatrix();
	glPushMatrix();//Pm tele enfrente der
		glColor3f(0.768, 0.768, 0.768);
		glTranslatef(-4.0, 1.25, -1.1);
		glRotatef(90, 0.0, 0.0, 1.0);
		figMarco.prisma(0.3, 11.0, 0.2, textMesaT.GLindex);
	glPopMatrix();
	glPushMatrix();//Pm tele enfrente izq
		glColor3f(0.768, 0.768, 0.768);
		glTranslatef(4.0, 1.25, -1.1);
		glRotatef(90, 0.0, 0.0, 1.0);
		figMarco.prisma(0.3, 11.0, 0.2, textMesaT.GLindex);
	glPopMatrix();
	glPushMatrix();//Pm tele enfrente der
		glColor3f(0.768, 0.768, 0.768);
		glTranslatef(-7.5, 0.0, -1.1);
		glRotatef(90, 0.0, 0.0, 1.0);
		figMarco.prisma(0.3, 8.0, 0.2, textMesaT.GLindex);
	glPopMatrix();
	glPushMatrix();//Pm tele atras der
		glColor3f(0.768, 0.768, 0.768);
		glTranslatef(-7.5, 0.0, 1.1);
		glRotatef(90, 0.0, 0.0, 1.0);
		figMarco.prisma(0.3, 8.0, 0.2, textMesaT.GLindex);
	glPopMatrix();
	glPushMatrix();//Pm tele atras izq
		glColor3f(0.768, 0.768, 0.768);
		glTranslatef(7.5, 0.0, 1.1);
		glRotatef(90, 0.0, 0.0, 1.0);
		figMarco.prisma(0.3, 8.0, 0.2, textMesaT.GLindex);
	glPopMatrix();
	glPushMatrix();//Pm tele adelante izq
		glColor3f(0.768, 0.768, 0.768);
		glTranslatef(7.5, 0.0, -1.1);
		glRotatef(90, 0.0, 0.0, 1.0);
		figMarco.prisma(0.3, 8.0, 0.2, textMesaT.GLindex);
	glPopMatrix();
	glPushMatrix();//Pm tele manija
		glColor3f(0.768, 0.768, 0.768);
		glTranslatef(0.0, 5.0, -1.1);
		figMarco.prisma(0.3, 1.0, 0.2, textMesaT.GLindex);
	glPopMatrix();

}

void casa3()
{
	glPushMatrix(); //Creamos paredes
		/*glColor3f(1, 1, 1);*/
		glTranslatef(0, 30, 0);
		fig1.skybox2(7.0, 17.0, 17.0, textPata.GLindex);
	glPopMatrix();
	glPushMatrix();//Techo
		glTranslatef(0, 33.5, 0);
		techoCasa.prisma(1, 17, 17, textPata.GLindex);
	glPopMatrix();
	glPushMatrix();//Pared
		glTranslatef(0, 30, -8.5);
		techoCasa.prisma(7, 17, 0.1, textPata.GLindex);
	glPopMatrix();
	glPushMatrix();//Ventana 1
		glTranslatef(-4.5, 30, -8.5);
		techoCasa.prisma(3, 5, 0.2, textventana.GLindex);
	glPopMatrix();
	glPushMatrix();//Ventana 2
		glTranslatef(4.5, 30, -8.5);
		techoCasa.prisma(3, 5, 0.2, textventana.GLindex);
	glPopMatrix();



}

void casa2() 

{
	glPushMatrix();//Chimenea	
		glColor3f(0.843, 0.843, 0.839);
		glTranslatef(-8.6, 31, 0.0);
		techoCasa.prisma(7, 2,3, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //Creamos paredes
		glColor3f(1,1,1);
		glTranslatef(0, 30, 0);
		fig1.skybox2(5.0,17.0,17.0, textPata.GLindex);
	glPopMatrix();
	glPushMatrix();//Techo
		glTranslatef(0, 32.5, 0);
		techoCasa.prisma(1,17,17, textPata.GLindex);
	glPopMatrix();
	glPushMatrix();//Pared
		glTranslatef(0, 30,-8.5);
		techoCasa.prisma(5, 17, 0.1, textPata.GLindex);
	glPopMatrix();	
	glPushMatrix();//Ventana 1
		glTranslatef(-4.5, 30, -8.5);
		techoCasa.prisma(3,5, 0.2, textventana.GLindex);
	glPopMatrix();
	glPushMatrix();//Ventana 2
		glTranslatef(4.5, 30, -8.5);
		techoCasa.prisma(3, 5, 0.2, textventana.GLindex);
	glPopMatrix();


}

void casa( void ){
//*****casa*****
	glPushMatrix(); //Creamos paredes
		glTranslatef(0,20,0);
		fig1.skybox2(60.0, 85.0, 85.0, textPata.GLindex);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0,50,0);
		techoCasa.prisma(1,90,90, textPata.GLindex);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.0,-10,0.0);
		pisoCasa.prisma(1,85,85,textPisoCasa.GLindex);
		glTranslatef(0,-3,0);		
	glPopMatrix();
	//*****************************
	glPushMatrix(); //Pared 1
		glTranslatef(28.5, 42.0, -42.5);
		glRotatef(90, 0.0, 0.0, 1.0);
		figMarco.prisma(27.0, 13.25, 0.1, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //Pared 2
		glTranslatef(28.5, -2.2, -42.5);
		glRotatef(90, 0.0, 0.0, 1.0);
		figMarco.prisma(27.0, 14.5, 0.1, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //Pared 3
		glTranslatef(17.5, 20.0, -42.5);
		figMarco.prisma(31.0, 5, 0.1, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //Pared 4
		glTranslatef(41.5, 20.0, -42.5);
		figMarco.prisma(60.0, 2.0, 0.1, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //Pared 1 d
		glTranslatef(-36.25, 42.0, -42.5);
		glRotatef(90, 0.0, 0.0, 1.0);
		figMarco.prisma(12.5, 13.25, 0.1, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //Pared 2 d
		glTranslatef(-36.25, -2.2, -42.5);
		glRotatef(90, 0.0, 0.0, 1.0);
		figMarco.prisma(12.5, 14.5, 0.1, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //Ventana 1
		glTranslatef(35.25, 20.0, -42.5);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.1);
		figMarco.prisma(30.0, 10.0, 0.1, textGlassWasser.GLindex);
		glDisable(GL_ALPHA_TEST);
	glPopMatrix();
	glPushMatrix(); //Ventana 1 d
		glTranslatef(-36.25, 20.0, -42.5);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.1);
		figMarco.prisma(30.0, 12.5, 0.1, textGlassWasser.GLindex);
		glDisable(GL_ALPHA_TEST);
	glPopMatrix();
	glPushMatrix(); //Ventana 2
		glTranslatef(25.25, 20.0, -42.5);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.1);
		figMarco.prisma(30.0, 10.0, 0.1, textGlassWasser.GLindex);
		glDisable(GL_ALPHA_TEST);
	glPopMatrix();
	glPushMatrix(); //Puerta 1
		glTranslatef(7.8, 20.0, -42.5);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.1);
		figMarco.prisma(60.0, 15.0, 0.1, textGlassWasser.GLindex);
		glDisable(GL_ALPHA_TEST);
	glPopMatrix();
	glPushMatrix(); //Puerta larga
		glTranslatef(-7.5, 20.0, -42.5);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.1);
		figMarco.prisma(60.0, 15.0, 0.1, textGlassWasser.GLindex);
		glDisable(GL_ALPHA_TEST);
	glPopMatrix();
	glPushMatrix(); //Puerta 2
		glTranslatef(-22.5, 20.0, -42.5);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.1);
		figMarco.prisma(60.0, 15.0, 0.1, textGlassWasser.GLindex);
		glDisable(GL_ALPHA_TEST);
	glPopMatrix();
	glPushMatrix(); //TAPETE
		glTranslatef(0.0,-9.4,0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		figtapete.prisma(35.0,29.0, 0.1, tapete.GLindex);
	glPopMatrix();

	//*****
	glPushMatrix(); //PALO GUIA
		glColor3f(0.682, 0.678, 0.678);
		glTranslatef(15.0, 20.0, -42.5);
		figMarco.prisma(60.0, 0.5, 0.5, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //PALO GUIA 2
		glColor3f(0.682, 0.678, 0.678);
		glTranslatef(0.0, 20.0, -42.5);
		figMarco.prisma(60.0, 0.5, 0.5, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //PALO GUIA 3
		glColor3f(0.682, 0.678, 0.678);
		glTranslatef(-15.0, 20.0, -42.5);
		figMarco.prisma(60.0, 0.5, 0.5, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //PALO GUIA 4
		glColor3f(0.682, 0.678, 0.678);
		glTranslatef(-30.0, 20.0, -42.5);
		figMarco.prisma(60.0, 0.5, 0.5, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //PALO GUIA  PUERTA 1
		glColor3f(0.682, 0.678, 0.678);
		glTranslatef(7.5, 10.0, -42.5);
		figMarco.prisma(40.0, 0.5, 0.5, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //PALO GUIA  PUERTA 2
		glColor3f(0.682, 0.678, 0.678);
		glRotatef(90,0.0,0.0,1.0);
		glTranslatef(30, -7.5, -42.5);
		figMarco.prisma(15.0, 0.5, 0.5, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //PALO GUIA  PUERTA 1 D
		glColor3f(0.682, 0.678, 0.678);
		glTranslatef(-22.5, 10.0, -42.5);
		figMarco.prisma(40.0, 0.5, 0.5, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //PALO GUIA  PUERTA 2 D
		glColor3f(0.682, 0.678, 0.678);
		glRotatef(90, 0.0, 0.0, 1.0);
		glTranslatef(30, 22.5, -42.5);
		figMarco.prisma(15.0, 0.5, 0.5, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //Marco ventana 1
		glColor3f(0.682, 0.678, 0.678);
		glTranslatef(20.0, 20.0, -42.5);
		figMarco.prisma(30.0, 0.5, 0.5, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //Marco vnetana 2
		glColor3f(0.682, 0.678, 0.678);
		glTranslatef(30.0, 20.0, -42.5);
		figMarco.prisma(30.0, 0.5, 0.5, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //Marco ventana 3
		glColor3f(0.682, 0.678, 0.678);
		glTranslatef(40.0, 20.0, -42.5);
		figMarco.prisma(30.0, 0.5, 0.5, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //Marco ventana 4
		glColor3f(0.682, 0.678, 0.678);
		glTranslatef(30.0, 35.0, -42.5);
		glRotatef(90, 0.0, 0.0, 1.0);
		figMarco.prisma(20.0, 0.5, 0.5, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //Marco ventana 5
		glColor3f(0.682, 0.678, 0.678);
		glTranslatef(30.0, 5.0, -42.5);
		glRotatef(90, 0.0, 0.0, 1.0);
		figMarco.prisma(20.0, 0.5, 0.5, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //Marco ventana 1 d
		glColor3f(0.682, 0.678, 0.678);
		glTranslatef(-36.25, 35.0, -42.5);
		glRotatef(90,0.0,0.0,1.0);
		figMarco.prisma(12.0, 0.5, 0.5, textPata.GLindex);
	glPopMatrix();
	glPushMatrix(); //Marco ventana 2 d
		glColor3f(0.682, 0.678, 0.678);
		glTranslatef(-36.25, 5.0, -42.5);
		glRotatef(90, 0.0, 0.0, 1.0);
		figMarco.prisma(12.0, 0.5, 0.5, textPata.GLindex);
	glPopMatrix();

	
	return;
}

void cuadro(void){
	glPushMatrix();
		/*glScalef(2,2,-1);*/
		glTranslatef(-19,30,-42);
		figcuadro.prisma(16,15,.2, textCuadro1.GLindex);
		//cubo.prisma(17,16,.05, textMarco.GLindex);
	glPopMatrix();
	return;
}


void tele (void)
{
	//glPushMatrix(); //Tele
	//	glTranslatef(0.5, 8.0, -0.21);
	//	glScalef(11.0, 8.5, 0.01);
	//	glRotatef(180,0.0,0.0,1.0);
	//	glDisable(GL_LIGHTING);
	//	figtele2.prisma_anun(0, textTele2.GLindex);
	//	glEnable(GL_LIGHTING);
	//glPopMatrix();
	glPushMatrix(); //Tele
		glTranslatef(0.5, 8.0, -0.20);
		glScalef(1.0, 7.0, 1.0);
		figtele.prisma(1.25, 10.5, 0.01, textTele2.GLindex);
	glPopMatrix();
	glPushMatrix(); //Base 
		glColor3f(0.431, 0.431, 0.431);
		glTranslatef(0.5,1.0,-0.0);
		glScalef(1.0,2.0,1.0);
		figtele.cono(0.5,1.0,30.0, textTele.GLindex);
	glPopMatrix();
	
	glPushMatrix(); //Tele
		glColor3f(0.431, 0.431, 0.431);
		glTranslatef(0.5, 8.0, 0.0);
		glScalef(1.0, 7.0, 1.0);
		figtele.prisma(1.5, 12.0, 0.25, textTele.GLindex);
	glPopMatrix();
	glPushMatrix(); //Base 2
		glColor3f(0.431, 0.431, 0.431);
		glTranslatef(0.5, 1.0, -0.0);
		glScalef(1.0,3.0, 1.0);
		figtele.cilindro(0.25, 0.7, 30.0, textTele.GLindex);
		glPopMatrix();
		
	return ;
}


GLuint createDL() 
{
	GLuint ciudadDL;
	//GLuint cieloDL;

	// Create the id for the list
	ciudadDL = glGenLists(1);
	// start list
	glNewList(ciudadDL,GL_COMPILE);
	// call the function that contains 
	// the rendering commands
		ciudad();
		//monito();
	// endList
	glEndList();

	return(ciudadDL);
}
			
void InitGL ( GLvoid )     // Inicializamos parametros
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Negro de fondo	

	glEnable(GL_TEXTURE_2D);

	glShadeModel (GL_SMOOTH);

	glLightfv(GL_LIGHT1, GL_POSITION, Position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Diffuse);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glClearDepth(1.0f);									// Configuramos Depth Buffer
	glEnable(GL_DEPTH_TEST);							// Habilitamos Depth Testing
	glDepthFunc(GL_LEQUAL);								// Tipo de Depth Testing a realizar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

    //********CIELO****************
	//con imágenes hechas para skybox para cada lado y arriba

	textSkyboxFrontal.LoadTGA("skybox/ely_hills/hills_ft.tga");
	textSkyboxFrontal.BuildGLTexture();
	textSkyboxFrontal.ReleaseImage();

	textSkyboxAtras.LoadTGA("skybox/ely_hills/hills_bk.tga");
	textSkyboxAtras.BuildGLTexture();
	textSkyboxAtras.ReleaseImage();

	textSkyboxIzquierda.LoadTGA("skybox/ely_hills/hills_lf.tga");
	textSkyboxIzquierda.BuildGLTexture();
	textSkyboxIzquierda.ReleaseImage();

	textSkyboxDerecha.LoadTGA("skybox/ely_hills/hills_rt.tga");
	textSkyboxDerecha.BuildGLTexture();
	textSkyboxDerecha.ReleaseImage();

	textSkyboxArriba.LoadTGA("skybox/ely_hills/hills_up.tga");
	textSkyboxArriba.BuildGLTexture();
	textSkyboxArriba.ReleaseImage();

	//Textura Casa
    text1.LoadTGA("casatexturas/mesa1.tga");
	text1.BuildGLTexture();
	text1.ReleaseImage();

	text2.LoadTGA("casatexturas/silla1.tga");
	text2.BuildGLTexture();
	text2.ReleaseImage();

	textSM.LoadTGA("casatexturas/silla2.tga");
	textSM.BuildGLTexture();
	textSM.ReleaseImage();

	text3.LoadTGA("casatexturas/alberca.tga");
	text3.BuildGLTexture();
	text3.ReleaseImage();

	text4.LoadTGA("casatexturas/piso.tga");
	text4.BuildGLTexture();
	text4.ReleaseImage();

	text5.LoadTGA("casatexturas/barandal.tga");
	text5.BuildGLTexture();
	text5.ReleaseImage();

	text6.LoadTGA("casatexturas/vidrio.tga");
	text6.BuildGLTexture();
	text6.ReleaseImage();

	textPata.LoadTGA("casatexturas/pared.tga");
	textPata.BuildGLTexture();
	textPata.ReleaseImage();

	textPisoCasa.LoadTGA("casatexturas/pisoCasa.tga");
	textPisoCasa.BuildGLTexture();
	textPisoCasa.ReleaseImage();

	textGlassWasser.LoadTGA("casatexturas/ventana2.tga");
	textGlassWasser.BuildGLTexture();
	textGlassWasser.ReleaseImage();

	textPuerta.LoadTGA("casatexturas/puerta.tga");
	textPuerta.BuildGLTexture();
	textPuerta.ReleaseImage();

	tapete.LoadTGA("casatexturas/tapete.tga");
	tapete.BuildGLTexture();
	tapete.ReleaseImage();

	textMesaT.LoadTGA("casatexturas/mesaTele.tga");
	textMesaT.BuildGLTexture();
	textMesaT.ReleaseImage();

	textTele.LoadTGA("casatexturas/tele.tga");
	textTele.BuildGLTexture();
	textTele.ReleaseImage();

	textTele2.LoadTGA("casatexturas/glich.tga");
	textTele2.BuildGLTexture();
	textTele2.ReleaseImage();

	textCuadro1.LoadTGA("casatexturas/cuadro.tga");
	textCuadro1.BuildGLTexture();
	textCuadro1.ReleaseImage();

	textburo.LoadTGA("casatexturas/buro.tga");
	textburo.BuildGLTexture();
	textburo.ReleaseImage();

	textsillon.LoadTGA("casatexturas/sillon.tga");
	textsillon.BuildGLTexture();
	textsillon.ReleaseImage();

	textsillon2.LoadTGA("casatexturas/sillon2.tga");
	textsillon2.BuildGLTexture();
	textsillon2.ReleaseImage();

	textcojin.LoadTGA("casatexturas/cojin.tga");
	textcojin.BuildGLTexture();
	textcojin.ReleaseImage();

	textPuerta.LoadTGA("casatexturas/Puerta.tga");
	textPuerta.BuildGLTexture();
	textPuerta.ReleaseImage();

	textsillon3.LoadBMP("casatexturas/sillon3.bmp");
	textsillon3.BuildGLTexture();
	textsillon3.ReleaseImage();

	textbanco.LoadTGA("casatexturas/banco.tga");
	textbanco.BuildGLTexture();
	textbanco.ReleaseImage();

	textventana.LoadTGA("casatexturas/ventana.tga");
	textventana.BuildGLTexture();
	textventana.ReleaseImage();

	textpelota.LoadTGA("casatexturas/pelota.tga");
	textpelota.BuildGLTexture();
	textpelota.ReleaseImage();

	texthumo.LoadTGA("casatexturas/humo.tga");
	texthumo.BuildGLTexture();
	texthumo.ReleaseImage();

	//***********MODELOS 3D***********
	
	amaca._3dsLoad("amaca/0008.3ds");
	amaca.LoadTextureImages();
	amaca.GLIniTextures();
	amaca.ReleaseTextureImages();

	florero._3dsLoad("flor/flowerpot004.3ds");
	florero.LoadTextureImages();
	florero.GLIniTextures();
	florero.ReleaseTextureImages();

	palmera._3dsLoad("Palm_Tree.3ds");
		
	
	//END NEW//////////////////////////////

	objCamera.Position_Camera(0,2.5f,3, 0,2.5f,0, 0, 1, 0);

	//NEW Crear una lista de dibujo
	ciudad_display_list = createDL();

	//NEW Iniciar variables de KeyFrames
	for(int i=0; i<MAX_FRAMES; i++)
	{
		KeyFrame[i].posX =0;
		KeyFrame[i].posY =0;
		KeyFrame[i].posZ =0;
		KeyFrame[i].incX =0;
		KeyFrame[i].incY =0;
		KeyFrame[i].incZ =0;
		
		KeyFrame[i].giroBancoCen =0;
		KeyFrame[i].giroBancoCenInc =0;
		KeyFrame[i].giroBancoIzq = 0;
		KeyFrame[i].giroBancoIzqInc = 0;
		KeyFrame[i].giroBancoDer = 0;
		KeyFrame[i].giroBancoDerInc = 0;
	}


	//NEW//////////////////NEW//////////////////NEW///////////
	KeyFrame[0].posX = 0;
	KeyFrame[0].posY = 0;
	KeyFrame[0].posZ = 0;
	FrameIndex++;

	KeyFrame[1].posX = 0;
	KeyFrame[1].posY = 3.5;
	KeyFrame[1].posZ = 0;
	FrameIndex++;

	KeyFrame[2].posX =0;
	KeyFrame[2].posY = 4.5;
	KeyFrame[2].posZ = 0;
	FrameIndex++;

	KeyFrame[3].posX = 0;
	KeyFrame[3].posY = 5.5;
	KeyFrame[3].posZ = 0;
	FrameIndex++;

	KeyFrame[4].posX = 0;
	KeyFrame[4].posY = 6.5;
	KeyFrame[4].posZ = 0;
	FrameIndex++;

	KeyFrame[5].posX = 0;
	KeyFrame[5].posY = 7.5;
	KeyFrame[5].posZ = 0;
	FrameIndex++;

	KeyFrame[6].posX = 0;
	KeyFrame[6].posY = 21.5;
	KeyFrame[6].posZ = 0;
	FrameIndex++;


}

void pintaTexto(float x, float y, float z, void *font,char *string)
{
  
  char *c;     //Almacena los caracteres a escribir
  glRasterPos3f(x, y, z);	//Posicion apartir del centro de la ventana
  //glWindowPos2i(150,100);
  for (c=string; *c != '\0'; c++) //Condicion de fin de cadena
  {
    glutBitmapCharacter(font, *c); //imprime
  }
}

void display ( void )   // Creamos la funcion donde se dibuja
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	glPushMatrix();
	

	glRotatef(g_lookupdown,1.0f,0,0);

		gluLookAt(	objCamera.mPos.x,  objCamera.mPos.y,  objCamera.mPos.z,	
					objCamera.mView.x, objCamera.mView.y, objCamera.mView.z,	
					objCamera.mUp.x,   objCamera.mUp.y,   objCamera.mUp.z);


		glPushMatrix();		
		    //****CIELO****
			glPushMatrix();
				glDisable(GL_LIGHTING);
				figSkybox.skyboxH(140.0, 140.0, 140.0, textSkyboxFrontal.GLindex, textSkyboxAtras.GLindex, textSkyboxIzquierda.GLindex, textSkyboxDerecha.GLindex, textSkyboxArriba.GLindex);
				glEnable(GL_LIGHTING);
			glPopMatrix();

			//***HUMO***
			glPushMatrix();
			if (posY > 21.5f)
			{
				posY = 0;
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
			}

				glTranslatef(0,posY,0);
				glScalef(.1,.3,.1);
				humo();
			glPopMatrix();

			//PELOTA
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				
				if (movPX > 8.2f)
					{
						g_avanza = false;
						
					}
				else if (movPX <=5.0f)
				{
					g_avanza = true;
				}

				glTranslatef(movPX, 0, 0);
				pelota();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();

			

			
		//***************************************BARRAS**********************************
			glPushMatrix();
				glEnable ( GL_COLOR_MATERIAL );
					glCallList(ciudad_display_list);
					glTranslatef(40, -1.0,-39.5);
					glRotatef(90,0.0,1.0,0.0);
					bara();
				glDisable ( GL_COLOR_MATERIAL );
			glPopMatrix();	
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(32.7, -1.0, -39.5);
				glRotatef(90, 0.0, 1.0, 0.0);
				bara();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(25.4, -1.0, -39.5);
				glRotatef(90, 0.0, 1.0, 0.0);
				bara();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(18.1, -1.0, -39.5);
				glRotatef(90, 0.0, 1.0, 0.0);
				bara();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(10.8, -1.0, -39.5);
				glRotatef(90, 0.0, 1.0, 0.0);
				bara();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(8.4, -1.0, -39.5);
				glRotatef(90, 0.0, 1.0, 0.0);
				bara();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(3.65, -1.0, -34.75);
				/*glRotatef(90, 0.0, 1.0, 0.0);*/
				bara();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(3.65, -1.0, -27.5);
				/*glRotatef(90, 0.0, 1.0, 0.0);*/
				bara();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(3.65, -1.0, -20.25);
				/*glRotatef(90, 0.0, 1.0, 0.0);*/
				bara();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(3.65, -1.0, -13);
				/*glRotatef(90, 0.0, 1.0, 0.0);*/
				bara();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(3.65, -1.0, -5.75);
				/*glRotatef(90, 0.0, 1.0, 0.0);*/
				bara();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(3.65, -1.0, 1.5);
				/*glRotatef(90, 0.0, 1.0, 0.0);*/
				bara();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(3.65, -1.0, 8.75);
				/*glRotatef(90, 0.0, 1.0, 0.0);*/
				bara();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(3.65, -1.0, 16);
				/*glRotatef(90, 0.0, 1.0, 0.0);*/
				bara();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(8.35, -1.0, 18.5);
				glRotatef(90, 0.0, 1.0, 0.0);
				bara();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(10.9, -1.0, 23.25);
				/*glRotatef(90, 0.0, 1.0, 0.0);*/
				bara();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(15.6, -1.0, 25.75);
				glRotatef(90, 0.0, 1.0, 0.0);
				bara();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(22.85, -1.0, 25.75);
				glRotatef(90, 0.0, 1.0, 0.0);
				bara();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(30.1, -1.0, 25.75);
				glRotatef(90, 0.0, 1.0, 0.0);
				bara();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(37.35, -1.0, 25.75);
				glRotatef(90, 0.0, 1.0, 0.0);
				bara();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();

			

			//*************************SALA EXTERIOR********************
			glPushMatrix(); //1
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(9.0, -0.5, -31.0);
				salaExt();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glPushMatrix();//2
				glEnable(GL_COLOR_MATERIAL);
				glCallList(ciudad_display_list);
				glTranslatef(14.0, -0.5, 15.0);
				salaExt();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			//*******Alberca********
			glPushMatrix();
			glEnable(GL_COLOR_MATERIAL);
			glTranslatef(10.0,-0.90,-10.0);
				alberca();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			//***PALMERAS***
			glPushMatrix();//Palmera 1
				glTranslatef(14.0, 0, -20.0);
				glScalef(3, 3, 3);
				palmera.GLrender(NULL, _SHADED, 1.0);
			glPopMatrix();
			glPushMatrix();//Palmera 2
				glTranslatef(20.0, 0, -33.0);
				glScalef(3, 3, 3);
				palmera.GLrender(NULL, _SHADED, 1.0);
			glPopMatrix();
			glPushMatrix();//Palmera 3
				glTranslatef(12.5, 0, 5.0);
				glScalef(2,2,2);
				palmera.GLrender(NULL, _SHADED, 1.0);
			glPopMatrix();
			glPushMatrix();//Palmera 4
				glTranslatef(25, 9.0, 5.0);
				glScalef(2, 2, 2);
				palmera.GLrender(NULL, _SHADED, 1.0);
			glPopMatrix();
			//PARTE CONECCION CASA
			glPushMatrix();
				glDisable(GL_LIGHTING);
				glTranslatef(30.0,-26.31, 15.3);
				/*glTranslatef(30.0, -18.5, -7.0);*/
				glRotatef(90, 0.0, 1.0, 0.0);
				casa3();
				glEnable(GL_LIGHTING);
			glPopMatrix();
			//PARTE De ARRIBA DE LA CASA
			glPushMatrix();
			glDisable(GL_LIGHTING);
				glTranslatef(30.0,-18.5,-7.0);
				glRotatef(90,0.0,1.0,0.0);
				casa2();
			glEnable(GL_LIGHTING);
			glPopMatrix();
			//CASA///////////////////
			//¨****casa*****

			glPushMatrix();
				glScalef(1.0, 0.5,1.5);
				glTranslatef(30, 3, -8.2);
				glRotatef(90, 0, 1, 0);
				glDisable(GL_LIGHTING);
				glScalef(0.3, 0.3, 0.3);
					casa();

					//Banco 1
					glPushMatrix();
						
						glTranslatef(-35.0,-8.0,0.0);
						bancooCen();
					glPopMatrix();
					//Banco 2
					glPushMatrix();
						glTranslatef(-35.0, -8.0,7.0);
						bancooIzq();
					glPopMatrix();
					//Banco 3
					glPushMatrix();
						glTranslatef(-35.0, -8.0, -7.0);
						bancooDer();
					glPopMatrix();

					

					//PUERTA EXT
					glPushMatrix();
						glTranslatef(20.0,8.5,42.5);
						figMarco.prisma(38.0,9.0,0.1, textPuerta.GLindex);
					glPopMatrix();
					
					//***BURO***
					glPushMatrix();
						glTranslatef(-9.0, -0.5, 40.0);
						buro();
					glPopMatrix();
					//***SILLON
					glPushMatrix();
						sillon();
					glPopMatrix();
					//***SILLON REDONDO***
					glPushMatrix();
						glTranslatef(-7.0, 0.0, -10.0);
						sillon3();
					glPopMatrix();
					//***SILLON INDIVIDUAL***
					glPushMatrix();					
						glTranslatef(-7.0,0.0,-10.0);
						glRotatef(-45,0.0,1.0,0.0);
						sillon2();
					glPopMatrix();
					
					//***MESA CENTRO***
					glPushMatrix();
						mesaCentro();
					glPopMatrix();
					//***MesaTele***
					glPushMatrix();
						glTranslatef(30.0,-5.0,-25.0);
						glRotatef(110,0.0,1.0,0.0);
						mesaTele();
					glPopMatrix();
					//***TELE***
					glPushMatrix();
						glTranslatef(30.0, 1.0, -25.0);
						glRotatef(110, 0.0, 1.0, 0.0);
						tele();
					glPopMatrix();
					//***CUADRO***
					glPushMatrix();		
						glTranslatef(10.0,0.0,84.3);
						cuadro();		
					glPopMatrix();
					//Florero
					glPushMatrix();
						glColor3f(1,1,1);
						glTranslatef(-4.0,4.0, 40);
						glRotatef(90,0.0,1.0,0.0);
						glScalef(0.1, 0.21, 0.1);
						florero.GLrender(NULL, _SHADED, 1);
					glPopMatrix();	

			glEnable(GL_LIGHTING);
			glPopMatrix();
			//***AMACAS***
			glPushMatrix();
				glDisable(GL_LIGHTING);
				glColor3f(0.933, 0.486, 0.654);
				glTranslatef(7.0,1.25,0.0);
				glRotatef(200, 0.0, 1.0, 0.0);
				amacaa();
				glEnable(GL_LIGHTING);
			glPopMatrix();
			glPushMatrix();
				glDisable(GL_LIGHTING);
				glColor3f(0.933, 0.486, 0.654);
				glTranslatef(12.0, 1.25, 0.0);
				glRotatef(200, 0.0, 1.0, 0.0);
				amacaa();
				glEnable(GL_LIGHTING);
			glPopMatrix();

			
			glColor3f(1.0,1.0,1.0);

		glPopMatrix();
	glPopMatrix();
	




		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
			glColor3f(1.0,0.0,0.0);
			pintaTexto(-11,12.0,-14.0,(void *)font,"Proyecto");
			pintaTexto(-11,8.5,-14,(void *)font,s);
			glColor3f(1.0,1.0,1.0);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);

	glutSwapBuffers ( );

}

void animacion()
{
	
	fig3.text_izq-= 0.001;
	fig3.text_der-= 0.001;
	if(fig3.text_izq<-1)
		fig3.text_izq=0;
	if(fig3.text_der<0)
		fig3.text_der=1;
	
	if (g_fanimacion)
	{
		if (g_avanza)
		{
			movPX += 0.1;
			/*rotP += 1;*/
		}
		else if (!g_avanza)
		{
			movPX -= 0.1;
			/*rotP -= 1;*/
		}

	}

	
	if (play2)
	{
		if (i_curr_steps >= i_max_steps)
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)
			{
				printf("termina anim\n");
				playIndex = 0;
				play2 = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();

			}
		}
		else
		{
			//Draw animation
			/*posX += KeyFrame[playIndex].incX;
			posY += KeyFrame[playIndex].incY;
			posZ += KeyFrame[playIndex].incZ;*/

			giroBancoIzq += KeyFrame[playIndex].giroBancoIzqInc;
			giroBancoCen += KeyFrame[playIndex].giroBancoCenInc;
			giroBancoDer += KeyFrame[playIndex].giroBancoDerInc;

			i_curr_steps++;
		}

	}


	//Movimiento del monito
	if(play)
	{		
		
		if(	i_curr_steps >= i_max_steps) //end of animation between frames?
		{			
			playIndex++;		
			if(playIndex>FrameIndex-2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex=0;
				play=false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();

			}
		}
		else
		{
			//Draw animation
			posX+=KeyFrame[playIndex].incX;
			posY+=KeyFrame[playIndex].incY;
			posZ+=KeyFrame[playIndex].incZ;

			/*giroBancoIzq +=KeyFrame[playIndex].giroBancoIzqInc;
			giroBancoCen += KeyFrame[playIndex].giroBancoCenInc;
			giroBancoDer += KeyFrame[playIndex].giroBancoDerInc;*/

			i_curr_steps++;
		}
		
	}


	glutPostRedisplay();
}


void reshape ( int width , int height )   // Creamos funcion Reshape
{
  if (height==0)										// Prevenir division entre cero
	{
		height=1;
	}

	glViewport(0,0,width,height);	

	glMatrixMode(GL_PROJECTION);						// Seleccionamos Projection Matrix
	glLoadIdentity();

	// Tipo de Vista
	
	glFrustum (-0.1, 0.1,-0.1, 0.1, 0.1, 170.0);

	glMatrixMode(GL_MODELVIEW);							// Seleccionamos Modelview Matrix
	glLoadIdentity();
}

void keyboard ( unsigned char key, int x, int y )  // Create Keyboard Function
{	//Movimientos de camara
	switch ( key ) {
		
		case 'w':   //Se mueve hacia adelante
		case 'W':
			objCamera.Move_Camera( CAMERASPEED+0.1 );
			break;

		case 's'://Se mueve hacia atras
		case 'S':
			objCamera.Move_Camera(-(CAMERASPEED+0.1));
			break;

		case 'a'://Se mueve hacia la izquierda
		case 'A':
			objCamera.Strafe_Camera(-(CAMERASPEED+0.2));
			break;

		case 'd'://Se mueve hacia la derecha
		case 'D':
			objCamera.Strafe_Camera( CAMERASPEED+0.2 );
			break;

		case 'k'://Para guardar los Frame de los bancos o del humo
		case 'K':
			if(FrameIndex<MAX_FRAMES)
			{
				saveFrame();
			}

			break;

		case 'o'://Para iniciar la animacion de los bancos con los frame que guarde el usuario
		case 'O':
			if (play2 == false && (FrameIndex > 1))
			{

				resetElements();
				/*First Interpolation ();*/
				interpolation();

				play2 = true;
				playIndex = 5;
				i_curr_steps = 0;
			}
			else
			{
				play2 = false;
			}
			break;
			

		case 'i'://Inicia y detiene la animacion de la pelota
		case 'I':
			g_fanimacion ^= true;
			

		case 'l'://Inicia y detiene  la aniamcion del humo 	
		case 'L':
			if(play==false && (FrameIndex>1))
			{

				resetElements();
				/*First Interpolation ();*/
				interpolation();

				play=true;
				playIndex=0;
				i_curr_steps = 0;
			}
			else
			{
				play=false;
			}
			break;

		case 'y'://Mueve el humo hacia arriba e imprime sus coordenadas en Y en la terminal
			posY++;
			printf("%f \n", posY);
			break;
		case 'Y'://Mueve el humo hacia abajo e imprime sus coordenadas en Y en la terminal
			posY--;
			printf("%f \n", posY);
			break;
		
		case 'n'://Gira el banco del centro sobre su mismo eje hacia la izquierda
			giroBancoCen++;
			break;

		case 'N'://Gira el banco del centro sobre su mismo eje hacia la derecha		
			giroBancoCen--;
			break;

		case 'b'://Gira el banco de la izquierda sobre su mismo eje hacia la derecha
			giroBancoIzq++;
			break;

		case 'B'://Gira el banco de la izquierda sobre su mismo eje hacia la izquierda
			giroBancoIzq--;
			break;

		case 'm'://Gira el banco de la derecha sobre su mismo eje hacia la derecha
			giroBancoDer++;
			break;

		case 'M'://Gira el banco de la derecha sobre su mismo eje hacia la izquierda
			giroBancoDer--;
			break;

		case 27:        // Cuando Esc es presionado...
			exit ( 0 );   // Salimos del programa
			break;        
		default:        // Cualquier otra
			break;
  }

  glutPostRedisplay();
}

void arrow_keys ( int a_keys, int x, int y )  // Funcion para manejo de teclas especiales (arrow keys)
{
  switch ( a_keys ) {
	case GLUT_KEY_PAGE_UP:
		objCamera.UpDown_Camera(CAMERASPEED);
		break;

	case GLUT_KEY_PAGE_DOWN:
		objCamera.UpDown_Camera(-CAMERASPEED);
		break;

    case GLUT_KEY_UP:     // Presionamos tecla ARRIBA...
		g_lookupdown -= 1.0f;
		break;

    case GLUT_KEY_DOWN:               // Presionamos tecla ABAJO...
		g_lookupdown += 1.0f;
		break;

	case GLUT_KEY_LEFT:
		objCamera.Rotate_View(-CAMERASPEED);
		break;

	case GLUT_KEY_RIGHT:
		objCamera.Rotate_View( CAMERASPEED);
		break;

    default:
		break;
  }
  glutPostRedisplay();
}

void menuKeyFrame( int id)
{
	switch (id)
	{
		case 0:	//Save KeyFrame
			if(FrameIndex<MAX_FRAMES)
			{
				saveFrame();
			}
			break;

		case 1:	//Play animation
			if(play==false && FrameIndex >1)
			{

				resetElements();
				//First Interpolation
				interpolation();

				play=true;
				playIndex=0;
				i_curr_steps = 0;
			}
			else
			{
				play=false;
			}
			break;


	}
}


void menu( int id)
{
	
}



int main ( int argc, char** argv )   // Main Function
{
  int submenu;
  glutInit            (&argc, argv); // Inicializamos OpenGL
  glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // Display Mode (Clores RGB y alpha | Buffer Doble )
  glutInitWindowSize  (500, 500);	// Tamaño de la Ventana
  glutInitWindowPosition (0, 0);	//Posicion de la Ventana
  glutCreateWindow    ("Carrillo Dominguez Carlos Daniel"); // Nombre de la Ventana
  //glutFullScreen     ( );         // Full Screen
  InitGL ();						// Parametros iniciales de la aplicacion
  glutDisplayFunc     ( display );  //Indicamos a Glut función de dibujo
  glutReshapeFunc     ( reshape );	//Indicamos a Glut función en caso de cambio de tamano
  glutKeyboardFunc    ( keyboard );	//Indicamos a Glut función de manejo de teclado
  glutSpecialFunc     ( arrow_keys );	//Otras
  glutIdleFunc		  ( animacion );
  submenu = glutCreateMenu	  ( menuKeyFrame );
  glutAddMenuEntry	  ("Guardar KeyFrame", 0);
  glutAddMenuEntry	  ("Reproducir Animacion", 1);
  glutCreateMenu	  ( menu );
  glutAddSubMenu	  ("Animacion Monito", submenu);
  glutAttachMenu	  (GLUT_RIGHT_BUTTON);
  glutMainLoop        ( );          // 
  return 0;
}
