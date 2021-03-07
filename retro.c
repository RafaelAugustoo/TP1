#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SOIL/SOIL.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#define TRUE 1
#define FALSE 0
#define QT_INIMIGOS 35
#define ALT_NAVE 80
#define LARG_NAVE 80
#define LARG_INIMIGOS 50
#define ALT_INIMIGOS 50
#define ALT_TIRO 40
#define LARG_TIRO 20
#define VIDASTOTAIS 3
//=====================================Ids das texturas==================================
GLuint idTexturaNave;
GLuint idTexturaTiro;
GLuint idTexturaTiroInimigo;
GLuint idTexturaInimigos;
GLuint idTexturaVidas;
GLuint idTexturaFundo;
GLuint idTexturaVidasPerdidas;
GLuint idTexturaGameOver;
GLuint idTexturaGameOverNome;
GLuint idTexturaPlayerWin;
GLuint idTexturaMenu;
GLuint idTexturaPause;
GLuint idTexturaNome;
GLuint idTexturaStarTrek;
GLuint idTexturaAmong;
GLuint idTexturaPauseNome;
GLuint idTexturaPause;
GLuint idTexturaCredito;
GLuint idTexturaInstrucoes;



//=========================Ids das musicas=================================================

Mix_Chunk *MusicaEscolha;
Mix_Chunk *MusicaTiroPlayer;
Mix_Chunk *MusicaTiroInimigo;
Mix_Chunk *MusicaPerdeVida;

Mix_Music *MusicaAbertura;
Mix_Music *MusicaFase;
Mix_Music *MusicaGameOver;
Mix_Music *MusicaPlayerWin;

//===================================================Carregar texturas=======================
GLuint carregaTextura(const char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(
                           arquivo,
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );

    if (idTextura == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }
    return idTextura;
}

//=====================================STRUCTS==================================
typedef struct {
    float tiro, inimigos, tiroInimigo;
}dados_velocidade;

typedef struct {
    float x, y;
    int alt, larg;
}dados_nave;

typedef struct {
    float x, y;
    int alt, larg;
}dados_inimigos;

typedef struct {
    float x,y;
    int alt,larg;
}dados_tiros;

typedef struct {
    float x, y;
}posicao_mouse;

//////////// Faz parte do deslocamento vertical dos fundos
static GLfloat velocidadeT = -0.010f;
static GLfloat deslocamentoT = 0;

static GLfloat deslocamentoM1 = 0;
static GLfloat velocidadeM1 = 0.3;

static GLfloat velocidadeM = -0.010f;
static GLfloat deslocamentoM = 0;

////////////// Dados e variaveis
posicao_mouse mouse;
dados_velocidade velocidades;
dados_nave nave;
dados_inimigos inimigos[QT_INIMIGOS];
dados_tiros tiros[1000];
dados_tiros tirosInimigos[3];
int i, j, k, l,aux,aux_tiros, kill;
int vidas;
int jogoPausado = 1, jogoComMouse = 0;

int inimigoAleatorioVivo(){
    srand(time(0));
    do{
    int numero = rand() % QT_INIMIGOS;

    if(inimigos[numero].y>0)
        return numero;
    }while(1);
}
//=========================================Funcções de Desenho=============================================
void desenhaFundo(){

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glTranslatef(0, deslocamentoT, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1, 1, 1);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaFundo);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //faz ir de cima pra baixo
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPushMatrix();
        glBegin(GL_POLYGON);
            glTexCoord2f(0, 0);
            glVertex3f(0, 0, 0);
            glTexCoord2f(1, 0);
            glVertex3f(1000, 0, 0);
            glTexCoord2f(1, 1);
            glVertex3f(1000, 1000, 0);
            glTexCoord2f(0, 1);
            glVertex3f(0, 1000, 0);
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void desenhaNave(){

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glTranslatef(0, 0, 0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaNave);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(nave.x, nave.y, 0);

        glTexCoord2f(1, 0);
        glVertex3f(nave.x + LARG_NAVE, nave.y, 0);

        glTexCoord2f(1, 1);
        glVertex3f(nave.x + LARG_NAVE, nave.y + ALT_NAVE, 0);

        glTexCoord2f(0, 1);
        glVertex3f(nave.x, nave.y + ALT_NAVE, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    }

void desenhaInimigos(){
        for(i=0;i<QT_INIMIGOS;i++){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, idTexturaInimigos);
        glBegin(GL_POLYGON);
          glTexCoord2f(0, 0);
          glVertex3f(inimigos[i].x , inimigos[i].y, 0);
          glTexCoord2f(1, 0);
          glVertex3f(inimigos[i].x + LARG_INIMIGOS , inimigos[i].y, 0);
          glTexCoord2f(1, 1);
          glVertex3f(inimigos[i].x + LARG_INIMIGOS , inimigos[i].y + ALT_INIMIGOS, 0);
          glTexCoord2f(0, 1);
          glVertex3f(inimigos[i].x , inimigos[i].y + ALT_INIMIGOS, 0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        }
}
void desenhaTirosInimigos(){
    for(i=0;i<3;i++){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaTiroInimigo);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(tirosInimigos[i].x, tirosInimigos[i].y, 0);
        glTexCoord2f(1, 0);
        glVertex3f(tirosInimigos[i].x + LARG_TIRO, tirosInimigos[i].y, 0);
        glTexCoord2f(1, 1);
        glVertex3f(tirosInimigos[i].x + LARG_TIRO, tirosInimigos[i].y + ALT_TIRO, 0);
        glTexCoord2f(0, 1);
        glVertex3f(tirosInimigos[i].x, tirosInimigos[i].y + ALT_TIRO, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    }
}
void desenhaTiros(){
    for(i=0;i<aux_tiros;i++){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaTiro);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(tiros[i].x, tiros[i].y, 0);
        glTexCoord2f(1, 0);
        glVertex3f(tiros[i].x + LARG_TIRO, tiros[i].y, 0);
        glTexCoord2f(1, 1);
        glVertex3f(tiros[i].x + LARG_TIRO, tiros[i].y + ALT_TIRO, 0);
        glTexCoord2f(0, 1);
        glVertex3f(tiros[i].x, tiros[i].y + ALT_TIRO, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
  }
}

void desenhaVidas(){
    for(i=0;i<vidas;i++){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaVidas);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(940 - i*60, 30, 0);
        glTexCoord2f(1, 0);
        glVertex3f(970 - i*60, 30, 0);
        glTexCoord2f(1, 1);
        glVertex3f(970 - i*60, 60, 0);
        glTexCoord2f(0, 1);
        glVertex3f(940 - i*60, 60, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    }
    for(i=vidas;i<VIDASTOTAIS;i++){
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, idTexturaVidasPerdidas);
      glBegin(GL_POLYGON);
          glTexCoord2f(0, 0);
          glVertex3f(940 - i*60, 30, 0);
          glTexCoord2f(1, 0);
          glVertex3f(970 - i*60, 30, 0);
          glTexCoord2f(1, 1);
          glVertex3f(970 - i*60, 60, 0);
          glTexCoord2f(0, 1);
          glVertex3f(940 - i*60, 60, 0);
      glEnd();
      glDisable(GL_TEXTURE_2D);
    }
}


void desenhaFundoMenu(){



  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1, 1, 1);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, idTexturaMenu);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //faz ir de cima pra baixo
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glPushMatrix();
      glBegin(GL_POLYGON);
          glTexCoord2f(0, 0);
          glVertex3f(0, 0, 0);
          glTexCoord2f(1, 0);
          glVertex3f(1000, 0, 0);
          glTexCoord2f(1, 1);
          glVertex3f(1000, 1000, 0);
          glTexCoord2f(0, 1);
          glVertex3f(0, 1000, 0);
      glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

void desenhaNomeJogo(){

  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glTranslatef(0, 0, 0);


  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, idTexturaNome);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(250, 700, 0);
       glTexCoord2f(1, 0);
        glVertex3f(750, 700, 0);
       glTexCoord2f(1, 1);
        glVertex3f(750, 1000, 0);
       glTexCoord2f(0, 1);
        glVertex3f(250, 1000, 0);
    glEnd();
   glDisable(GL_TEXTURE_2D);
}

void desenhaFundoPause(){


  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glRotatef(deslocamentoM1, 500, 0, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1, 1, 1);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, idTexturaPause);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //faz ir de cima pra baixo
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glPushMatrix();
      glBegin(GL_POLYGON);
          glTexCoord2f(0, 0);
          glVertex3f(0, 0, 0);
          glTexCoord2f(1, 0);
          glVertex3f(1000, 0, 0);
          glTexCoord2f(1, 1);
          glVertex3f(1000, 1000, 0);
          glTexCoord2f(0, 1);
          glVertex3f(0, 1000, 0);
      glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}


void desenhaPauseNome(){


  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glTranslatef(0, 0, 0);


  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, idTexturaPauseNome);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(200, 500, 0);
       glTexCoord2f(1, 0);
        glVertex3f(800, 500, 0);
       glTexCoord2f(1, 1);
        glVertex3f(800, 900, 0);
       glTexCoord2f(0, 1);
        glVertex3f(200, 900, 0);
    glEnd();
   glDisable(GL_TEXTURE_2D);
}

void desenhaReferenciaMenu(){

  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glTranslatef(0, 0, 0);


  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, idTexturaStarTrek);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(250, 200, 0);
        glTexCoord2f(1, 0);
        glVertex3f(300, 200, 0);
        glTexCoord2f(1, 1);
        glVertex3f(300, 250, 0);
        glTexCoord2f(0, 1);
        glVertex3f(250, 250, 0);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaStarTrek);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(250, 150, 0);
        glTexCoord2f(1, 0);
        glVertex3f(300, 150, 0);
        glTexCoord2f(1, 1);
        glVertex3f(300, 200, 0);
        glTexCoord2f(0, 1);
        glVertex3f(250, 200, 0);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaStarTrek);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(250, 100, 0);
        glTexCoord2f(1, 0);
        glVertex3f(300, 100, 0);
        glTexCoord2f(1, 1);
        glVertex3f(300, 150, 0);
        glTexCoord2f(0, 1);
        glVertex3f(250, 150, 0);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaStarTrek);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(250, 50, 0);
        glTexCoord2f(1, 0);
        glVertex3f(300, 50, 0);
        glTexCoord2f(1, 1);
        glVertex3f(300, 100, 0);
        glTexCoord2f(0, 1);
        glVertex3f(250, 100, 0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}


void desenhaJogo() {

    Mix_Pause(3);

    glEnable(GL_BLEND);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,1,1);
    glEnable(GL_BLEND);
    desenhaFundo();
    desenhaNave();
    desenhaInimigos();
    desenhaTiros();
    desenhaTirosInimigos();
    desenhaVidas();
    glutSwapBuffers();
}


void desenhaGameOver(){

  jogoPausado==1;

  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glTranslatef(deslocamentoM, 0, 0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1, 1, 1);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, idTexturaGameOver);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//vai pro lado
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //faz ir de cima pra baixo
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glPushMatrix();
      glBegin(GL_POLYGON);
          glTexCoord2f(0, 0);
          glVertex3f(0, 0, 0);
          glTexCoord2f(1, 0);
          glVertex3f(1000, 0, 0);
          glTexCoord2f(1, 1);
          glVertex3f(1000, 1000, 0);
          glTexCoord2f(0, 1);
          glVertex3f(0, 1000, 0);
      glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}


void desenhaPlayerWin(){
  jogoPausado==1;
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glTranslatef(0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1, 1, 1);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, idTexturaPlayerWin);

  glPushMatrix();
      glBegin(GL_POLYGON);
          glTexCoord2f(0, 0);
          glVertex3f(0, 0, 0);
          glTexCoord2f(1, 0);
          glVertex3f(1000, 0, 0);
          glTexCoord2f(1, 1);
          glVertex3f(1000, 1000, 0);
          glTexCoord2f(0, 1);
          glVertex3f(0, 1000, 0);
      glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

void desenhaInstrucoes(){

  glEnable(GL_BLEND);
  glClear(GL_COLOR_BUFFER_BIT);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, idTexturaInstrucoes);

  glPushMatrix();
      glBegin(GL_POLYGON);
          glTexCoord2f(0, 0);
          glVertex3f(0, 0, 0);
          glTexCoord2f(1, 0);
          glVertex3f(1000, 0, 0);
          glTexCoord2f(1, 1);
          glVertex3f(1000, 1000, 0);
          glTexCoord2f(0, 1);
          glVertex3f(0, 1000, 0);
      glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
  glutSwapBuffers();
}

void desenhaReferenciaCredito(){

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glTranslatef(0, 0, 0);


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaAmong);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      glBegin(GL_POLYGON);
          glTexCoord2f(0, 0);
          glVertex3f(300, 100, 0);
          glTexCoord2f(1, 0);
          glVertex3f(800, 100, 0);
          glTexCoord2f(1, 1);
          glVertex3f(800, 600, 0);
          glTexCoord2f(0, 1);
          glVertex3f(300, 600, 0);
      glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

void desenhaCreditos(){

  jogoPausado==1;
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glTranslatef(deslocamentoM, 0, 0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1, 1, 1);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, idTexturaCredito);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //faz ir de cima pra baixo
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glPushMatrix();
      glBegin(GL_POLYGON);
          glTexCoord2f(0, 0);
          glVertex3f(0, 0, 0);
          glTexCoord2f(1, 0);
          glVertex3f(1000, 0, 0);
          glTexCoord2f(1, 1);
          glVertex3f(1000, 1000, 0);
          glTexCoord2f(0, 1);
          glVertex3f(0, 1000, 0);
      glEnd();

  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

//========================= MENUS E CRÉDITOS =========================================================================
///Menu

void Menu(){

    velocidadeM = 0.001f;

    jogoPausado==1;

    glEnable(GL_BLEND);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,1,1);
    glEnable(GL_BLEND);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    desenhaFundoMenu();

    desenhaNomeJogo();

    desenhaReferenciaMenu();

    glColor3f(1,1,1);
    glRasterPos3f(300,215,0);
    char msgm[]="Pressione X para INICIAR o Jogo";
    for(int i=0; i<strlen(msgm); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msgm[i]);
    }

    glColor3f(1,1,1);
    glRasterPos3f(300,165,0);
    char msgm2[]="Pressione C para ver os Creditos";
    for(int i=0; i<strlen(msgm2); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msgm2[i]);
    }

    glColor3f(1,1,1);
    glRasterPos3f(300,115,0);
    char msgm3[]="Pressione I para ver as Instrucoes";
    for(int i=0; i<strlen(msgm3); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msgm3[i]);
    }

    glColor3f(1,1,1);
    glRasterPos3f(300,65,0);
    char msgm4[]="Pressione ESC para SAIR";
    for(int i=0; i<strlen(msgm4); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msgm4[i]);
    }

    glutSwapBuffers();
}

//Menu de Pausa
void MenuPausa(){

    jogoPausado==1;

    glEnable(GL_BLEND);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,1,1);
    glEnable(GL_BLEND);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    desenhaFundoPause();
    desenhaPauseNome();

    glColor3f(1,1,1);
    glRasterPos3f(300,650,0);
    char msgm[]="Pressione P para VOLTAR ao Jogo";
    for(int i=0; i<strlen(msgm); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msgm[i]);
    }

    glColor3f(1,1,1);
    glRasterPos3f(300,600,0);
    char msgm1[]="Pressione R para Resetar";
    for(int i=0; i<strlen(msgm1); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msgm1[i]);
    }

    glColor3f(1,1,1);
    glRasterPos3f(300,550,0);
    char msgm2[]="Pressione I para ver as Instrucoes";
    for(int i=0; i<strlen(msgm2); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msgm2[i]);
    }

    glColor3f(1,1,1);
    glRasterPos3f(300,500,0);
    char msgm3[]="Pressione M para ir ao Menu";
    for(int i=0; i<strlen(msgm3); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msgm3[i]);
    }

    glColor3f(1,1,1);
    glRasterPos3f(300,450,0);
    char msgm4[]="Pressione ESC para FECHAR do Jogo";
    for(int i=0; i<strlen(msgm4); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msgm4[i]);
    }


    glutSwapBuffers();
}

void GameOver(){

    jogoPausado==1;

    glEnable(GL_BLEND);
    glColor3f(1,1,1);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    desenhaGameOver();

    glColor3f(1,1,1);
    glRasterPos3f(330,150,0);
    char msgm[]="Pressione M para ir ao Menu";
    for(int i=0; i<strlen(msgm); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msgm[i]);
    }

    glColor3f(1,1,1);
    glRasterPos3f(330,100,0);
    char msgm2[]="Pressione C para ver os Creditos";
    for(int i=0; i<strlen(msgm2); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msgm2[i]);
    }

    glColor3f(1,1,1);
    glRasterPos3f(330,050,0);
    char msgm1[]="Pressione ESC para SAIR";
    for(int i=0; i<strlen(msgm1); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msgm1[i]);
    }


  glutSwapBuffers();
}

void PlayerWin(){

  jogoPausado==1;

  Mix_Pause(-1);
  glEnable(GL_BLEND);
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1,1,1);
  glEnable(GL_BLEND);

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


  desenhaPlayerWin();



  glColor3f(1,1,1);
  glRasterPos3f(300,150,0);
  char msgm[]="Pressione M para ir ao Menu";
  for(int i=0; i<strlen(msgm); i++){
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msgm[i]);
  }

  glColor3f(1,1,1);
  glRasterPos3f(250,100,0);
  char msgm1[]="QUE A FORCA ESTEJA COM VOCE";
  for(int i=0; i<strlen(msgm1); i++){
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msgm1[i]);
  }

  glutSwapBuffers();

}

void Creditos(){

  jogoPausado==1;

  glEnable(GL_BLEND);
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1,1,1);
  glEnable(GL_BLEND);

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  desenhaCreditos();

  desenhaReferenciaCredito();

  glColor3f(1,1,1);
  glRasterPos3f(400,800,0);
  glTranslatef(deslocamentoM, 0, 0);
  glLoadIdentity();
  char msgm[]="Desenvolvido Por:";
  for(int i=0; i<strlen(msgm); i++){
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msgm[i]);
  }
  glColor3f(1,1,1);
  glRasterPos3f(350,650,0);
  glTranslatef(deslocamentoM, 0, 0);
  glLoadIdentity();
  char msgm2[]="Fernando Lorenzeto Rodrigues";
  for(int i=0; i<strlen(msgm2); i++){
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msgm2[i]);
  }
  glColor3f(1,1,1);
  glRasterPos3f(350,600,0);
  glTranslatef(deslocamentoM, 0, 0);
  glLoadIdentity();
  char msgm3[]="Rafael Assuncao de Ladeira Torga";
  for(int i=0; i<strlen(msgm3); i++){
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msgm3[i]);
  }
  glColor3f(1,1,1);
  glRasterPos3f(350,550,0);
  glTranslatef(deslocamentoM, 0, 0);
  glLoadIdentity();
  char msgm4[]="Rafael Augusto de Souza";
  for(int i=0; i<strlen(msgm4); i++){
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msgm4[i]);
  }

  glColor3f(1,1,1);
  glRasterPos3f(340,100,0);
  char msgm5[]="Pressione M para ir ao Menu";
  for(int i=0; i<strlen(msgm5); i++){
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msgm5[i]);
  }


    glutSwapBuffers();
}

//================================================Inicializações====================================================
void inicializaTiros(){
    for(i=0;i<1000;i++){
        tiros[i].x = 0;
        tiros[i].y = 0;
        velocidades.tiro = 10;
    }
}


void inicializaInimigos(){
    aux=0;
    velocidades.inimigos = 8;
    for(i=0;i<7;i++){
        for(j=0;j<5;j++){
        inimigos[aux].x = 81.25 + i*131.25;
        inimigos[aux].y = 930 - j*60;
        aux++;
        }
    }
}

void inicializaTirosInimigos(){
    for(i=0;i<3;i++){
        tirosInimigos[i].x = -100;
        tirosInimigos[i].y = 600 - i*100;
        velocidades.tiroInimigo = 8;
    }
}

void naveAtira(){
    tiros[aux_tiros].x = nave.x+(LARG_NAVE/2);
    tiros[aux_tiros].y = nave.y+(ALT_NAVE);
    aux_tiros++;
}

void abaixarTodosInimigos(){
for(i=0;i<QT_INIMIGOS;i++)
    inimigos[i].y -= 8;
}
void inimigosAtiram(){
    Mix_PlayChannel(-1,MusicaTiroInimigo,0);
    int inim = inimigoAleatorioVivo();
    for(i=0;i<3;i++){
        tirosInimigos[i].x = inimigos[inim].x + (LARG_INIMIGOS/2);
        tirosInimigos[i].y = inimigos[inim].y - i*100;
    }
}


void inicializa() {

    glClearColor(0, 0, 0, 0);      //Fundo Preto

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    aux=0;
    aux_tiros=0;
    kill=0;
    nave.x = 460;
    nave.y = 20;

      //CARREGAR TEXTURAS DO JOGO

    idTexturaNave = carregaTextura("Recursos/Naves/nave.png");  //carrega a nave
    idTexturaInimigos = carregaTextura("Recursos/Naves/inimigo1.png"); //carrega inimigo
    idTexturaTiro = carregaTextura("Recursos/Tiros/TiroNave.png"); //tiro nave
    idTexturaTiroInimigo = carregaTextura ("Recursos/Tiros/TiroInimigo.png"); //tiro inimigo
    idTexturaVidas = carregaTextura ("Recursos/Vidas/Vida.png"); //carrega vidas cheias
    idTexturaVidasPerdidas = carregaTextura ("Recursos/Vidas/perdeu.png"); //carrega vidas perdidas
    idTexturaFundo = carregaTextura ("Recursos/Fundos/FundoJogo.jpg"); //carrega fundo do jogo
    idTexturaGameOver = carregaTextura ("Recursos/Fundos/GameOver.png"); //carrega fundo do game over
    idTexturaPlayerWin = carregaTextura ("Recursos/Fundos/YouWin.jpeg"); //carrega fundo player win
    idTexturaMenu = carregaTextura ("Recursos/Fundos/FundoMenu.jpg"); //carrega fundo do menu
    idTexturaNome = carregaTextura("Recursos/Fundos/NomeJogo.png"); //carrega nome do jogo no menu
    idTexturaStarTrek = carregaTextura("Recursos/Fundos/StarTrek.png"); //easter egg
    idTexturaPause = carregaTextura("Recursos/Fundos/PauseMenu.jpg"); //carrega fundo pause
    idTexturaPauseNome = carregaTextura("Recursos/Fundos/PAUSE.png"); //carrega nome pause
    idTexturaCredito = carregaTextura("Recursos/Fundos/FundoCreditos.jpg"); //carrega fundo creditos
    idTexturaInstrucoes = carregaTextura("Recursos/Fundos/Instrucoes.png"); //carrega tela intrucoes
    idTexturaAmong = carregaTextura("Recursos/Fundos/Among.png"); //carrega tela intrucoes

    Mix_AllocateChannels (10);

    MusicaAbertura = Mix_LoadMUS("Recursos/Sons/Inicio.mp3");
    MusicaGameOver = Mix_LoadMUS("Recursos/Sons/Inicio.mp3");
    MusicaPlayerWin = Mix_LoadMUS("Recursos/Sons/Inicio.mp3");
    MusicaFase = Mix_LoadMUS("Recursos/Sons/GamePlay.mp3");

    MusicaEscolha = Mix_LoadWAV("Recursos/Sons/Selecao.wav"); //Mix_PlayChannel(-1,MusicaEscolha,1);
    MusicaTiroPlayer = Mix_LoadWAV("Recursos/Sons/TiroNave.wav"); //Mix_PlayChannel(-1,MusicaTiroPlayer,1);
    MusicaTiroInimigo = Mix_LoadWAV("Recursos/Sons/InimigosTiro.wav");//Mix_PlayChannel(-1,MusicaTiroInimigo,1);
    MusicaPerdeVida = Mix_LoadWAV("Recursos/Sons/PerdaVida.wav");//Mix_PlayChannel(-1,MusicaTiroInimigo,1);

    Mix_PlayMusic(MusicaAbertura, -1);
    inicializaInimigos();
    inicializaTiros();
    inicializaTirosInimigos();
    vidas = VIDASTOTAIS;
}

//==============================================Callbacks=========================================================================
// Callback de redimensionamento
void redimensiona(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, 1000, 0, 1000, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/////////////////////////////////////Atualizações/////////////////////////////////////

//Para efeitos de movimento do Fundo
void atualiza() {
    deslocamentoT += velocidadeT;
}

void atualizaMenu() {
    deslocamentoM += velocidadeM;
    deslocamentoM1 += velocidadeM1;
}
//////////////////////

void atualizaInimigos(){
    for(i=0;i<QT_INIMIGOS;i++)
        inimigos[i].x += velocidades.inimigos;
}

void atualizaTiros(){
    for(i=0;i<aux_tiros;i++)
        tiros[i].y += velocidades.tiro;
}
void atualizaTirosInimigos(){
    for(i=0;i<3;i++){
        tirosInimigos[i].y -= velocidades.tiroInimigo;
    }
}

// Callback de evento de movimento do mouse
int colisao(float Ax, float Ay, int Alarg, int Aalt, float Bx, float By, int Blarg, int Balt){
    if(Ay + Aalt < By)
        return 0;
    if(Ay > By + Balt)
        return 0;
    if(Ax + Alarg < Bx)
        return 0;
    if(Ax > Bx + Blarg)
        return 0;

    return 1;
}
//=============================== Condição===================================
void condicoes(){
  if(nave.x>1000 - ALT_NAVE)
      nave.x = 1000 - ALT_NAVE;

  if(nave.x<0)
      nave.x = 0;

  for(i=0;i<QT_INIMIGOS;i++){
      if(inimigos[i].x + LARG_INIMIGOS>=1000 || inimigos[i].x<=0){
          velocidades.inimigos = -velocidades.inimigos;
          abaixarTodosInimigos();
          break;
      }
  }

  for(i=0;i<3;i++){
      if(tirosInimigos[i].y <= -250)
          inimigosAtiram();
  }
}

void condicoes2(){

  if(kill == QT_INIMIGOS){
      inicializaInimigos();
      jogoPausado == 1;
      glutDisplayFunc(PlayerWin);
      glutPostRedisplay();
  }

  if(vidas<=0){
      Mix_Pause(-1);
      jogoPausado == 1;
      glutDisplayFunc(GameOver);
      glutPostRedisplay();
  }

}

////////=====================================Colisões======================================

void verificaColisoes(){
    //Tiro da nave com inimigos
    for(i=0;i<aux_tiros;i++){
        for(j=0;j<QT_INIMIGOS;j++){
            if(colisao(tiros[i].x, tiros[i].y, LARG_TIRO, ALT_TIRO, inimigos[j].x, inimigos[j].y, LARG_INIMIGOS, ALT_INIMIGOS)==1){
                inimigos[j].y = -100;
                tiros[i].x = -100;
                kill++;
            }
        }
    }
    //Inimigos com a nave
    for(k=0;k<QT_INIMIGOS;k++){
            if(colisao(nave.x, nave.y, LARG_NAVE, ALT_NAVE, inimigos[k].x, inimigos[k].y, LARG_INIMIGOS, ALT_INIMIGOS)==1){
                inimigos[k].y = -100;
                vidas--;
            }
    }
    //Tiros inimigos com a nave
    for(l=2;l>=0;l--){
            if(colisao(nave.x, nave.y, LARG_NAVE, ALT_NAVE, tirosInimigos[l].x, tirosInimigos[l].y, LARG_TIRO, ALT_TIRO)==1){
                tirosInimigos[l].y = -20;
                Mix_Volume(-1, MIX_MAX_VOLUME/3);
                Mix_PlayChannel(-1,MusicaPerdeVida,0);
                vidas--;

            }
    }
    //Inimigos com o chao
    for(i=0;i<QT_INIMIGOS;i++){
        if(inimigos[i].y==0 && inimigos[i].x>0)
            vidas--;
    }
}
void movimentoViaMouse(int x, int y){
    mouse.x = x;
}
void atualizaNaveViaMouse(){
    nave.x = mouse.x - LARG_NAVE/2;

}
// Callback do evento timer
void atualizaCena(int periodo) {
    if(jogoPausado==0){
    condicoes();
    atualizaInimigos();
    atualizaTiros();
    atualizaTirosInimigos();
    atualiza();
    if(jogoComMouse)
        atualizaNaveViaMouse();
    condicoes2();
    verificaColisoes();
    }
    atualizaMenu();
    glutPostRedisplay();
    glutTimerFunc(periodo, atualizaCena, periodo);
}

//Callback de evento de tiro com o Mouse
void controleMouse(int button, int state, int x, int y)
{
    if(jogoComMouse){
    if (button == GLUT_LEFT_BUTTON)
         if (state == GLUT_DOWN) {
            Mix_Volume(-1, MIX_MAX_VOLUME/3);
            Mix_PlayChannel(-1,MusicaTiroPlayer,0);
            naveAtira();
            }
    glutPostRedisplay();
    }
}


//Callback de evento de teclado para teclas especiais
void tecladoMove(int key, int x, int y){
  if(key == GLUT_KEY_LEFT){
    if(jogoPausado==0){
        nave.x -= 15;
    }
  }

  if (key == GLUT_KEY_RIGHT){
    if(jogoPausado==0){
      nave.x += 15;
    }
  }

}

// Callback de evento de teclado
void teclado(unsigned char key, int x, int y) {
   switch(key) {

    case 27: //ESC
        Mix_PlayChannel(2,MusicaEscolha,1);
        exit(0);
        break;


    case 'c':
          Mix_PauseMusic();
          Mix_PlayChannel(2,MusicaEscolha,1);
          inicializa();
          jogoPausado = 1;
          Mix_VolumeMusic(MIX_MAX_VOLUME/2);
          Mix_PlayMusic(MusicaFase, -1);
          glutDisplayFunc(Creditos);
          glutPostRedisplay();
        break;

    case 'C':
          Mix_PauseMusic();
          Mix_PlayChannel(2,MusicaEscolha,1);
          inicializa();
          jogoPausado = 1;
          Mix_VolumeMusic(MIX_MAX_VOLUME/2);
          Mix_PlayMusic(MusicaFase, -1);
          glutDisplayFunc(Creditos);
          glutPostRedisplay();
        break;

    case 'x':
          Mix_PauseMusic();
          Mix_PlayChannel(2,MusicaEscolha,1);
          if(jogoPausado==1){
            jogoPausado = 0;
            Mix_VolumeMusic(MIX_MAX_VOLUME/2);
            Mix_PlayMusic(MusicaFase, -1);
          }
          glutDisplayFunc(desenhaJogo);
          glutPostRedisplay();
        break;
    case 'X':
          Mix_PauseMusic();
          Mix_PlayChannel(2,MusicaEscolha,1);
          if(jogoPausado==1){
            jogoPausado = 0;
            Mix_VolumeMusic(MIX_MAX_VOLUME/2);
            Mix_PlayMusic(MusicaFase, -1);
          }
          glutDisplayFunc(desenhaJogo);
          glutPostRedisplay();
        break;

    case ' ': //ESPAÇO
        if(jogoPausado==0){
          if(jogoComMouse==0){
            naveAtira();
            Mix_Volume(-1, MIX_MAX_VOLUME/3);
            Mix_PlayChannel(-1,MusicaTiroPlayer,0);
          }
        }
        break;

    case 'r':
        Mix_PlayChannel(2,MusicaEscolha,1);
        inicializa();
        Mix_PauseMusic();
        Mix_VolumeMusic(MIX_MAX_VOLUME/2);
        Mix_PlayMusic(MusicaFase, -1);
        break;
    case 'R':
        Mix_PlayChannel(2,MusicaEscolha,1);
        inicializa();
        Mix_PauseMusic();
        Mix_VolumeMusic(MIX_MAX_VOLUME/2);
        Mix_PlayMusic(MusicaFase, -1);
        break;

    case 'P':
        Mix_PlayChannel(2,MusicaEscolha,1);
        if(jogoPausado==1)
        {
            jogoPausado = 0;
            Mix_ResumeMusic();
            glutDisplayFunc(desenhaJogo);
            glutPostRedisplay();
        }
        else
        {
            jogoPausado = 1;
            Mix_PauseMusic();
            glutDisplayFunc(MenuPausa);
            glutPostRedisplay();
        }
        break;
    case 'p':
        Mix_PlayChannel(2,MusicaEscolha,1);
        if(jogoPausado==1)
        {
            jogoPausado = 0;
            Mix_ResumeMusic();
            glutDisplayFunc(desenhaJogo);
            glutPostRedisplay();
        }
        else
        {
            jogoPausado = 1;
            Mix_PauseMusic();
            glutDisplayFunc(MenuPausa);
            glutPostRedisplay();
        }
        break;

    case 'm':
        Mix_PlayChannel(2,MusicaEscolha,1);
        inicializa();
        jogoPausado =1;
        Mix_ResumeMusic();
        glutDisplayFunc(Menu);
        glutPostRedisplay();

        break;

    case 'M':

        Mix_PlayChannel(2,MusicaEscolha,1);
        inicializa();
        jogoPausado=1;
        Mix_ResumeMusic();
        glutDisplayFunc(Menu);
        glutPostRedisplay();

    case 'z':
        Mix_PauseMusic();
        Mix_Pause(-1);
        break;

    case 'Z':

        Mix_PauseMusic();
        Mix_Pause(-1);
        break;

    case 'i':
        glutPostRedisplay();
        glutDisplayFunc(desenhaInstrucoes);
        glutPostRedisplay();
        break;

    case 'I':
        glutDisplayFunc(desenhaInstrucoes);
        glutPostRedisplay();
        break;

    case '1':
        if(jogoComMouse==1)
            jogoComMouse = 0;
        else
            jogoComMouse = 1;
        break;

    default:
         break;
   }
}
//===================================================== Rotina principal=======================================
int main(int argc, char **argv) {

    SDL_Init(SDL_INIT_EVERYTHING|SDL_INIT_AUDIO); //Chamar SDL

    // Configuração inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(950, 750);
                  //freq, format, chanell, buffer)
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);

    glutCreateWindow("TP1");
    glutReshapeFunc(redimensiona);
    glutDisplayFunc(Menu);
    inicializa();
    glutSpecialFunc(tecladoMove);
    glutMouseFunc(controleMouse);
    glutKeyboardFunc(teclado);
    glutPassiveMotionFunc(movimentoViaMouse);
    glutTimerFunc(0, atualizaCena, 33);
    glutMainLoop();

    Mix_CloseAudio();
    //SDL_Quit();
    return 0;
}
