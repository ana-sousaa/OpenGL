#include <GL/glut.h>
#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int larguraJanela = 800;
int alturaJanela = 600;

int telaAtual = 0;

float posX = 0.0;

GLuint idArvore1, idArvore2, idArvore3; 
GLuint idBotao, idMoeda, idQuiz, idCercaFrontal, idCercaLateral2, idCercaLateral1;
GLuint idLapide, idLapide2, idLapide3, idLapide4, idLapide5, idLapide6;
GLuint idVela, idGato, idFlor, idCorvo, idCoracao, idCaveira, idCaixao1, idCaixao2, idArbusto; 
GLuint zumbiDireita[2];
GLuint zumbiEsquerda[2];
GLuint zumbiFrente[3];
GLuint zumbiCostas[2];

float camX, camY;

int frameAtual = 0;

GLuint spriteAtual;

float zumbiX = 0.0;
float zumbiY = 4.0;

float velocidadeZumbi = 0.03;

int estadoZumbi = 0;

bool movendoEmX = true;

bool zumbiComendo = false;

bool zumbiVoltando = false;



struct Posicao {
    float x;
    float y;
    float z;
};

Posicao arvores[50];
Posicao carca[50];

struct Pergunta {
    std::string pergunta;
    std::string A;
    std::string B;
    std::string C;
    std::string D;
    char correta;
};

Pergunta quiz[] = {
    {"Para movimentar um objeto ou a camera pelo plano cartesiano. Qual funcao do OpenGL ele deve usar?", "glLoadIdentity()", "glScaled()", "glTranslatef()", "glClearColor()", 'C'},

    {"Para limpar a memoria de transformacoes anteriores e resetar a camera de volta para a posicao inicial (0,0), qual funcao e usada?", "glutSwapBuffers()", "glLoadIdentity()", "glBegin(GL_QUADS)", "glutTimerFunc()", 'B'},

    {"Qual funcao e utilizada para iniciar o desenho de um quadrado ou outro poligono no OpenGL?", "glBegin(GL_QUADS)", "glVertex3f()", "glEnd()", "glColor3f()", 'A'},

    {"Qual funcao define a cor utilizada para desenhar os proximos objetos?", "glClear()", "glColor3f()", "glTranslatef()", "glFlush()", 'B'},

    {"Qual funcao encerra a definicao de um objeto iniciado com glBegin()?", "glEnd()", "glFinish()", "glFlush()", "glClose()", 'A'},

    {"Qual funcao troca o buffer de desenho quando se utiliza double buffering?", "glutSwapBuffers()", "glFlush()", "glClear()", "glLoadIdentity()", 'A'},

    {"Qual funcao limpa o buffer de cores da janela?", "glColor3f()", "glClear(GL_COLOR_BUFFER_BIT)", "glFlush()", "glTranslatef()", 'B'},

    {"Qual funcao define a cor de fundo da janela?", "glClearColor()", "glColor3f()", "glLoadIdentity()", "glVertex3f()", 'A'},

    {"Qual funcao e utilizada para aumentar ou diminuir o tamanho de um objeto?", "glTranslatef()", "glRotatef()", "glScaled()", "glVertex3f()", 'C'},

    {"Qual funcao e utilizada para rotacionar um objeto?", "glRotatef()", "glTranslatef()", "glScaled()", "glLoadIdentity()", 'A'},

    {"Qual funcao agenda a execucao de uma funcao apos um determinado tempo em milissegundos?", "glutDisplayFunc()", "glutTimerFunc()", "glutKeyboardFunc()", "glutIdleFunc()", 'B'},

    {"Qual funcao registra a funcao responsavel por desenhar a cena?", "glutDisplayFunc()", "glutTimerFunc()", "glutReshapeFunc()", "glutMouseFunc()", 'A'},

    {"Qual funcao registra o tratamento de eventos do teclado?", "glutKeyboardFunc()", "glutMouseFunc()", "glutDisplayFunc()", "glutInit()", 'A'},

    {"Qual funcao registra o tratamento de cliques do mouse?", "glutMouseFunc()", "glutKeyboardFunc()", "glutDisplayFunc()", "glutMotionFunc()", 'A'},

    {"Qual funcao envia os comandos de desenho para a placa de video?", "glFlush()", "glBegin()", "glClear()", "glTranslatef()", 'A'},
};

int perguntaAtual = 0;
int pontuacao = 0;
int moedas;

std::string mensagemQuiz = "";
bool mostrandoResultado = false;

char alternativaSelecionada = ' ';

float mouseX = 0.0;
float mouseY = 0.0;

float moverMouseX = 0.0;
float moverMouseY = 0.0;

bool segurandoObjeto = false;
int objetoSelecionado = 0;
std::string mensagemLoja = "";
int tempoMensagemLoja = 0;



struct ObjetoCenario {
    int tipo; 
    float x;
    float y;
};

#include <vector>
std::vector<ObjetoCenario> objetosColocados;


void desenharTexto(std::string texto, float x, float y) {
    glRasterPos2f(x, y);

    for(char c : texto) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
}

void movimentoMouse(int x, int y)
{
    moverMouseX = (x / (float)larguraJanela) * 10.0f - 5.0f;
    moverMouseY = 4.0f - (y / (float)alturaJanela) * 8.0f;


    glutPostRedisplay();
}

void desenharSprite(GLuint idTextura, float posX, float posY, float posZ, float largura, float altura) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Ativa a transparência do PNG
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTextura);
    
    glColor3f(1.0f, 1.0f, 1.0f);  
    glNormal3f(0.0f, 0.0f, 1.0f); 
   
    float meioX = largura / 2.0f;
    float meioY = altura / 2.0f;

    glBegin(GL_QUADS);
        
        glTexCoord2f(0.0f, 1.0f); glVertex3f(posX - meioX, posY - meioY, posZ);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(posX + meioX, posY - meioY, posZ);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(posX + meioX, posY + meioY, posZ);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(posX - meioX, posY + meioY, posZ);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

GLuint carregarTextura(const std::string& caminhoArquivo) {
    int largura, altura, canais;
    
    
    unsigned char* dados = stbi_load(caminhoArquivo.c_str(), &largura, &altura, &canais, 4);
    
    GLuint idTextura = 0;

    if (dados) {
        glGenTextures(1, &idTextura);
        glBindTexture(GL_TEXTURE_2D, idTextura);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       
       
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, largura, altura, 0, GL_RGBA, GL_UNSIGNED_BYTE, dados);
        
        stbi_image_free(dados); // Libera a memória RAM
        std::cout << "Sucesso ao carregar: " << caminhoArquivo << std::endl;
    } else {
        std::cout << "Erro ao carregar a textura: " << caminhoArquivo << std::endl;
    }

    return idTextura;
}

void desenhaLoja() {
    glTranslatef(0.0, camY, 0.0);
    glBegin(GL_QUADS);
        glColor3f(0.15, 0.15, 0.15);
        glVertex3f(-10.0, -6.0, 0.0);
        glVertex3f(-10.0, 6.0,  0.0);
        glVertex3f( 10.0, 6.0,  0.0);
        glVertex3f( 10.0, -6.0, 0.0);
    glEnd();
     
  
    desenharSprite(idLapide,  -3.6,  1.9, 0.0, 1.5, 1.5);
    desenharSprite(idLapide2, -1.8,  1.9, 0.0, 1.5, 1.5);
    desenharSprite(idLapide3,  0.0,  1.9, 0.0, 1.5, 1.5);
    desenharSprite(idLapide4,  1.8,  1.9, 0.0, 1.5, 1.5);
    desenharSprite(idLapide5,  3.6,  1.9, 0.0, 1.5, 1.5);
    desenharSprite(idLapide6, -3.6, -0.5, 0.0, 1.5, 1.5);
    desenharSprite(idArbusto,    -1.8, -0.5, 0.0, 1.5, 1.5);
    desenharSprite(idCaixao1,  0.0, -0.5, 0.0, 1.5, 1.5);
    desenharSprite(idCaixao2,  1.8, -0.5, 0.0, 1.5, 1.5);
    desenharSprite(idCaveira,  3.6, -0.5, 0.0, 1.5, 1.5);
    desenharSprite(idCorvo, -3.6, -2.8, 0.0, 1.5, 1.5);
    desenharSprite(idFlor, -1.8, -2.8, 0.0, 1.5, 1.5);
    desenharSprite(idGato,     0.0, -2.8, 0.0, 1.5, 1.5);
    desenharSprite(idVela,    1.8, -2.8, 0.0, 1.5, 1.5);
    desenharSprite(idCoracao, 3.6, -3.1, 0.0, 1.5, 1.5);
    
    glColor3f(0.0, 0.0, 0.0);
    desenharTexto(mensagemLoja, -0.7, 0.0);

        glBegin(GL_QUADS);
            glColor3f(0.0, 0.0, 0.0); 
            glVertex3f(-4.1, 0.8, 0.0);
            glVertex3f(-4.1, 1.2, 0.0);
            glVertex3f(-3.1, 1.2, 0.0); 
            glVertex3f(-3.1, 0.8, 0.0);
        glEnd(); 
        glColor3f(1.0, 1.0, 1.0); 
        desenharTexto("15$", -3.7, 0.9);
        
        glBegin(GL_QUADS);
            glColor3f(0.0, 0.0, 0.0);
            glVertex3f(-2.3, 0.8, 0.0); 
            glVertex3f(-2.3, 1.2, 0.0);
            glVertex3f(-1.3, 1.2, 0.0);
            glVertex3f(-1.3, 0.8, 0.0);
        glEnd();
        glColor3f(1.0, 1.0, 1.0); 
        desenharTexto("15$", -1.9, 0.9);

        glBegin(GL_QUADS);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(-0.5f, 0.8f, 0.0f); 
            glVertex3f(-0.5f, 1.2f, 0.0f);
            glVertex3f( 0.5f, 1.2f, 0.0f);
            glVertex3f( 0.5f, 0.8f, 0.0f);
        glEnd();
        glColor3f(1.0, 1.0, 1.0); 
        desenharTexto("15$", -0.1, 0.9);

        glBegin(GL_QUADS);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f( 1.3f, 0.8f, 0.0f);
            glVertex3f( 1.3f, 1.2f, 0.0f);
            glVertex3f( 2.3f, 1.2f, 0.0f);
            glVertex3f( 2.3f, 0.8f, 0.0f);
        glEnd();
        glColor3f(1.0, 1.0, 1.0); 
        desenharTexto("15$", 1.7, 0.9);

        glBegin(GL_QUADS);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f( 3.1f, 0.8f, 0.0f);
            glVertex3f( 3.1f, 1.2f, 0.0f);
            glVertex3f( 4.1f, 1.2f, 0.0f);
            glVertex3f( 4.1f, 0.8f, 0.0f);
        glEnd();
        glColor3f(1.0, 1.0, 1.0); 
        desenharTexto("20$", 3.5, 0.9);

        glBegin(GL_QUADS);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(-4.1f, -1.5f, 0.0f); 
            glVertex3f(-4.1f, -1.1f, 0.0f); 
            glVertex3f(-3.1f, -1.1f, 0.0f);
            glVertex3f(-3.1f, -1.5f, 0.0f);
        glEnd();
        glColor3f(1.0, 1.0, 1.0); 
        desenharTexto("20$", -3.7, -1.4);

        glBegin(GL_QUADS);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(-2.3f, -1.5f, 0.0f);
            glVertex3f(-2.3f, -1.1f, 0.0f);
            glVertex3f(-1.3f, -1.1f, 0.0f);
            glVertex3f(-1.3f, -1.5f, 0.0f);
        glEnd();
        glColor3f(1.0, 1.0, 1.0); 
        desenharTexto("30$", -1.9, -1.4);

        glBegin(GL_QUADS);
           glColor3f(0.0f, 0.0f, 0.0f);
           glVertex3f(-0.5f, -1.5f, 0.0f);
           glVertex3f(-0.5f, -1.1f, 0.0f);
           glVertex3f( 0.5f, -1.1f, 0.0f);
           glVertex3f( 0.5f, -1.5f, 0.0f);
       glEnd();
       glColor3f(1.0, 1.0, 1.0); 
       desenharTexto("35$", -0.1, -1.4);

       glBegin(GL_QUADS);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f( 1.3f, -1.5f, 0.0f);
            glVertex3f( 1.3f, -1.1f, 0.0f);
            glVertex3f( 2.3f, -1.1f, 0.0f);
            glVertex3f( 2.3f, -1.5f, 0.0f);
       glEnd();
       glColor3f(1.0, 1.0, 1.0); 
       desenharTexto("35$", 1.7, -1.4);

       glBegin(GL_QUADS);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f( 3.1f, -1.5f, 0.0f);
            glVertex3f( 3.1f, -1.1f, 0.0f);
            glVertex3f( 4.1f, -1.1f, 0.0f);
            glVertex3f( 4.1f, -1.5f, 0.0f);
       glEnd();
       glColor3f(1.0, 1.0, 1.0); 
       desenharTexto("50$", 3.5, -1.4);

       glBegin(GL_QUADS);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(-4.1f, -3.8f, 0.0f);
            glVertex3f(-4.1f, -3.4f, 0.0f);
            glVertex3f(-3.1f, -3.4f, 0.0f);
            glVertex3f(-3.1f, -3.8f, 0.0f);
       glEnd();
       glColor3f(1.0, 1.0, 1.0); 
       desenharTexto("60$", -3.7, -3.7);

       glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(-2.3f, -3.8f, 0.0f);
            glVertex3f(-2.3f, -3.4f, 0.0f);
            glVertex3f(-1.3f, -3.4f, 0.0f);
            glVertex3f(-1.3f, -3.8f, 0.0f);
      glEnd();
      glColor3f(1.0, 1.0, 1.0); 
      desenharTexto("40$", -1.9, -3.7);

      glBegin(GL_QUADS);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(-0.5f, -3.8f, 0.0f);
            glVertex3f(-0.5f, -3.4f, 0.0f);
            glVertex3f( 0.5f, -3.4f, 0.0f);
            glVertex3f( 0.5f, -3.8f, 0.0f);
      glEnd();
      glColor3f(1.0, 1.0, 1.0); 
      desenharTexto("70$", -0.1, -3.7);

      glBegin(GL_QUADS);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f( 1.3f, -3.8f, 0.0f);
            glVertex3f( 1.3f, -3.4f, 0.0f);
            glVertex3f( 2.3f, -3.4f, 0.0f);
            glVertex3f( 2.3f, -3.8f, 0.0f);
     glEnd();
     glColor3f(1.0, 1.0, 1.0); 
     desenharTexto("40$", 1.7, -3.7);

     glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(3.1f, -3.8f, 0.0f);
    glVertex3f(3.1f, -3.4f, 0.0f);
    glVertex3f(4.1f, -3.4f, 0.0f);
    glVertex3f(4.1f, -3.8f, 0.0f);
glEnd();

glColor3f(1.0f, 1.0f, 1.0f);
desenharTexto("100$", 3.5f, -3.7f);

}

void coracaoZumbi() {
    zumbiComendo = false;    
    zumbiVoltando = true;    
    estadoZumbi = 2;   
    
    segurandoObjeto = false;
    objetoSelecionado = 0;
}

void atualizar(int valor) {

   if (zumbiComendo)
{
    for (size_t i = 0; i < objetosColocados.size(); i++)
    {
        if (objetosColocados[i].tipo == 15) 
        {
            if (abs(objetosColocados[i].x - zumbiX) < 0.8f &&
                abs(objetosColocados[i].y - zumbiY) < 0.8f)
            {
                coracaoZumbi();

                objetosColocados.erase(objetosColocados.begin() + i);

                break;
            }
        }
    }
}
 if(estadoZumbi == 2)
{
    if (!zumbiComendo) 
    {
        if (zumbiVoltando) {
            zumbiY += velocidadeZumbi; 
            
            if(zumbiY >= 4.0f) {       
                estadoZumbi = 1;      
            }
        } else {
            zumbiY -= velocidadeZumbi; 
            if(zumbiY <= 0.0f) {
                zumbiComendo = true;   
            }
        }
    }
}
else if(estadoZumbi == 1)
{
    if (zumbiVoltando) {
        zumbiX += velocidadeZumbi;    
        if(zumbiX >= 4.0f) {
            estadoZumbi = 0;          
        }
    } else {
        zumbiX -= velocidadeZumbi;   
        if(zumbiX <= -4.0f) {
            estadoZumbi = 2;
        }
    }
}
else if(estadoZumbi == 0)
{
    if (zumbiVoltando) {
        zumbiX -= velocidadeZumbi;    
        if(zumbiX <= 0.0f) {          
            zumbiVoltando = false; 
            zumbiX = 0.0f; zumbiY = 4.0f; 
            estadoZumbi = 0;
        }
    } else {
        zumbiX += velocidadeZumbi;    
        if(zumbiX >= 4.0f) {
            estadoZumbi = 1;
        }
    }
}

if (!zumbiComendo) {
    frameAtual++; 
}

    if (tempoMensagemLoja > 0) {
        tempoMensagemLoja--;
        if (tempoMensagemLoja == 0) {
            mensagemLoja = "";
        }
    }

    glutPostRedisplay();
    glutTimerFunc(100, atualizar, 0);
}

void desenharQuiz()
{
    if(mostrandoResultado)
    {
        desenharTexto(mensagemQuiz, -0.8, 0);
        desenharTexto("Clique para continuar", -0.8, -1);
        return;
    }

    desenharTexto(quiz[perguntaAtual].pergunta, -3.5, 2.6);

    // ---------- BOTÃO A ----------
    if(moverMouseX >= -3.5 && moverMouseX <= 3.5 &&
       moverMouseY >= 1.2 && moverMouseY <= 2.0)
    {
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    else
    {
        glColor3f(0.3f, 0.3f, 0.3f);
    }

    glBegin(GL_QUADS);
        glVertex2f(-3.5f, 2.0f);
        glVertex2f( 3.5f, 2.0f);
        glVertex2f( 3.5f, 1.2f);
        glVertex2f(-3.5f, 1.2f);
    glEnd();

    glColor3f(1,1,1);
    desenharTexto("A) " + quiz[perguntaAtual].A, -3.0f, 1.6f);

    // ---------- BOTÃO B ----------
    if(moverMouseX >= -3.5 && moverMouseX <= 3.5 &&
       moverMouseY >= 0.2 && moverMouseY <= 1.0)
    {
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    else
    {
        glColor3f(0.3f, 0.3f, 0.3f);
    }

    glBegin(GL_QUADS);
        glVertex2f(-3.5f, 1.0f);
        glVertex2f( 3.5f, 1.0f);
        glVertex2f( 3.5f, 0.2f);
        glVertex2f(-3.5f, 0.2f);
    glEnd();

    glColor3f(1,1,1);
    desenharTexto("B) " + quiz[perguntaAtual].B, -3.0f, 0.6f);

    // ---------- BOTÃO C ----------
    if(moverMouseX >= -3.5 && moverMouseX <= 3.5 &&
       moverMouseY >= -0.8 && moverMouseY <= 0.0)
    {
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    else
    {
        glColor3f(0.3f, 0.3f, 0.3f);
    }

    glBegin(GL_QUADS);
        glVertex2f(-3.5f, 0.0f);
        glVertex2f( 3.5f, 0.0f);
        glVertex2f( 3.5f,-0.8f);
        glVertex2f(-3.5f,-0.8f);
    glEnd();

    glColor3f(1,1,1);
    desenharTexto("C) " + quiz[perguntaAtual].C, -3.0f, -0.4f);

    // ---------- BOTÃO D ----------
    if(moverMouseX >= -3.5 && moverMouseX <= 3.5 &&
       moverMouseY >= -1.8 && moverMouseY <= -1.0)
    {
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    else
    {
        glColor3f(0.3f, 0.3f, 0.3f);
    }

    glBegin(GL_QUADS);
        glVertex2f(-3.5f,-1.0f);
        glVertex2f( 3.5f,-1.0f);
        glVertex2f( 3.5f,-1.8f);
        glVertex2f(-3.5f,-1.8f);
    glEnd();

    glColor3f(1,1,1);
    desenharTexto("D) " + quiz[perguntaAtual].D, -3.0f, -1.4f);
}

void responder(char resposta) {
    mostrandoResultado = true;

    if(resposta == quiz[perguntaAtual].correta) {
        mensagemQuiz = "ACERTOU! +10 moedas";
        moedas += 10;
        pontuacao++;
    } else {
        mensagemQuiz = "ERROU!";
    }
}

void desenharJogo() {

    glTranslatef(camX, camY, 0.0);

    // ---------------- CHÃO ----------------
    glBegin(GL_QUADS);
        glColor3f(0.65f, 0.50f, 0.39f);
        glVertex3f(-10.0, -6.0, 0.0);
        glVertex3f(-10.0, 6.0,  0.0);
        glVertex3f( 10.0, 6.0,  0.0);
        glVertex3f( 10.0, -6.0, 0.0);
    glEnd();


  if (zumbiComendo) 
{
    spriteAtual = zumbiFrente[2]; // Parado comendo
}
else if (estadoZumbi == 2)
{
    // No estado 2, ele só se move no eixo Y
    if (zumbiVoltando) {
        spriteAtual = zumbiCostas[(frameAtual / 2) % 2]; // Subindo
    } else {
        spriteAtual = zumbiFrente[(frameAtual / 2) % 2];  // Descendo
    }
}
else if (estadoZumbi == 1)
{
    // No estado 1, a direção muda dependendo de estar indo ou voltando
    if (zumbiVoltando) {
        spriteAtual = zumbiDireita[(frameAtual / 2) % 2]; // Voltando -> Vai para a Direita
    } else {
        spriteAtual = zumbiEsquerda[(frameAtual / 2) % 2]; // Indo -> Vai para a Esquerda
    }
}
else if (estadoZumbi == 0)
{
    // No estado 0, também muda de acordo com o retorno
    if (zumbiVoltando) {
        spriteAtual = zumbiEsquerda[(frameAtual / 2) % 2]; // Voltando -> Vai para a Esquerda
    } else {
        spriteAtual = zumbiDireita[(frameAtual / 2) % 2];  // Indo -> Vai para a Direita
    }
}


    desenharSprite(spriteAtual, zumbiX, zumbiY, 0.0, 0.8, 0.8);

    //--------- arvores ----------

    for(int x = 0; x < 7; x++) {
        for(int y = 0; y < 7; y++) {
            float posArvoreX = -9.3 + (x * 2.0);
            float posArvoreY =  6.0;

            desenharSprite(idArvore3, posArvoreX, posArvoreY, 0.0, 2.5, 2.5);
        }
    }

    for(int x = 0; x < 3; x++) {
        for(int y = 0; y < 6; y++) {
            float posArvoreX = -9.3 + (x * 1.65);
            float posArvoreY =  5.0 - (y * 1.4);

            desenharSprite(idArvore2, posArvoreX, posArvoreY, 0.0, 2.5, 2.5);
        }
    }
   
    for(int x = 0; x < 3; x++) {
        for(int y = 0; y < 6; y++) {
            float posArvoreX = -10 + (x * 1.5);
            float posArvoreY =  6.0 - (y * 2.0);

            desenharSprite(idArvore3, posArvoreX, posArvoreY, 0.0, 2.5, 2.5);
        }
    }
    
    for(int x = 0; x < 2; x++) {
        for(int y = 0; y < 6; y++) {
            float posArvoreX = 8.0 + (x * 1.65);
            float posArvoreY =  5.0 - (y * 2.0);

            desenharSprite(idArvore2, posArvoreX, posArvoreY, 0.0, 2.5, 2.5);
        }
    }
   
    for(int x = 0; x < 3; x++) {
        for(int y = 0; y < 4; y++) {
            float posArvoreX = 7.0 + (x * 1.5);
            float posArvoreY =  6.0 - (y * 2.8);

            desenharSprite(idArvore3, posArvoreX, posArvoreY, 0.0, 2.5, 2.5);
        }
    }

    for(int x = 0; x < 10; x++) {
        for(int y = 0; y < 2; y++) {
            float posArvoreX = -9.3 + (x * 1.65);
            float posArvoreY =  -3.0 - (y * 1.4);

            desenharSprite(idArvore2, posArvoreX, posArvoreY, 0.0, 2.5, 2.5);
        }
    }

    for(int x = 0; x < 10; x++) {
        for(int y = 0; y < 2; y++) {
            float posArvoreX = -9.2 + (x * 1.5);
            float posArvoreY =  -2.6 - (y * 1.4);

            desenharSprite(idArvore3, posArvoreX, posArvoreY, 0.0, 2.5, 2.5);
        }
    }

    // ----- cerca ------
    for(int x = 0; x < 12; x++) {
        for(int y = 0; y < 12; y++) {
            float posCercaFrontalX = -5.0 + (x * 1.0);
            float posCercaFrontaly =  5.0;
           
            desenharSprite(idCercaFrontal, posCercaFrontalX, posCercaFrontaly, 0.0, 1.0, 1.0);
        }
    }

    for(int x = 0; x < 5; x++) {
        for(int y = 0; y < 5; y++) {
            float posCercaFrontalX = -5.0 + (x * 1.0);
            float posCercaFrontaly =  -2.3;
           
            desenharSprite(idCercaFrontal, posCercaFrontalX, posCercaFrontaly, 0.0, 1.0, 1.0);
        }
    }

    for(int x = 0; x < 5; x++) {
        for(int y = 0; y < 5; y++) {
            float posCercaFrontalX = 2.0 + (x * 1.0);
            float posCercaFrontaly =  -2.3;
           
            desenharSprite(idCercaFrontal, posCercaFrontalX, posCercaFrontaly, 0.0, 1.0, 1.0);
        }
    }

    desenharSprite(idArvore2, 1.6, -2.0, 0.0, 2.5, 2.5);

    for(int x = 0; x < 8; x++) {
        for(int y = 0; y < 8; y++) {
            float posCercaLateral1X = -5.4;
            float posCercaLateral1y =  4.74 - (y * 1.0);
           
            desenharSprite(idCercaLateral1, posCercaLateral1X, posCercaLateral1y, 0.0, 1.0, 1.0);
        }
    }

    for(int x = 0; x < 8; x++) {
        for(int y = 0; y < 8; y++) {
            float posCercaLateral2X = 6.5;
            float posCercaLateral2y =  4.74 - (y * 1.0);
           
            desenharSprite(idCercaLateral2, posCercaLateral2X, posCercaLateral2y, 0.0, 1.0, 1.0);
        }
    }


    //---------- objetos----------------
    for (size_t i = 0; i < objetosColocados.size(); i++) {
        if (objetosColocados[i].tipo == 1) { 
            desenharSprite(idLapide, objetosColocados[i].x, objetosColocados[i].y, 0.0, 0.7, 0.6);
        }
    }
    if (segurandoObjeto && objetoSelecionado == 1) {
       
        float lapideMouseX = moverMouseX - camX;
        float lapideMouseY = moverMouseY - camY;
        
        desenharSprite(idLapide, lapideMouseX, lapideMouseY, 0.0, 0.7, 0.6);
    }


    for (size_t i = 0; i < objetosColocados.size(); i++) {
        if (objetosColocados[i].tipo == 2) { 
            desenharSprite(idLapide2, objetosColocados[i].x, objetosColocados[i].y, 0.0, 0.7, 0.6);
        }
    }
    if (segurandoObjeto && objetoSelecionado == 2) {
       
        float lapide2MouseX = moverMouseX - camX;
        float lapide2MouseY = moverMouseY - camY;
        
        desenharSprite(idLapide2, lapide2MouseX, lapide2MouseY, 0.0, 0.7, 0.6);
    }


     for (size_t i = 0; i < objetosColocados.size(); i++) {
        if (objetosColocados[i].tipo == 3) { 
            desenharSprite(idLapide3, objetosColocados[i].x, objetosColocados[i].y, 0.0, 0.7, 0.6);
        }
    }
    if (segurandoObjeto && objetoSelecionado == 3) {
       
        float lapide3MouseX = moverMouseX - camX;
        float lapide3MouseY = moverMouseY - camY;
        
        desenharSprite(idLapide3, lapide3MouseX, lapide3MouseY, 0.0, 0.7, 0.6);
    }


    for (size_t i = 0; i < objetosColocados.size(); i++) {
        if (objetosColocados[i].tipo == 4) { 
            desenharSprite(idLapide4, objetosColocados[i].x, objetosColocados[i].y, 0.0, 0.7, 0.6);
        }
    }
    if (segurandoObjeto && objetoSelecionado == 4) {
       
        float lapide4MouseX = moverMouseX - camX;
        float lapide4MouseY = moverMouseY - camY;
        
        desenharSprite(idLapide4, lapide4MouseX, lapide4MouseY, 0.0, 0.7, 0.6);
    }


     for (size_t i = 0; i < objetosColocados.size(); i++) {
        if (objetosColocados[i].tipo == 5) { 
            desenharSprite(idLapide5, objetosColocados[i].x, objetosColocados[i].y, 0.0, 0.7, 0.6);
        }
    }
    if (segurandoObjeto && objetoSelecionado == 5) {
       
        float lapide5MouseX = moverMouseX - camX;
        float lapide5MouseY = moverMouseY - camY;
        
        desenharSprite(idLapide5, lapide5MouseX, lapide5MouseY, 0.0, 0.7, 0.6);
    }


     for (size_t i = 0; i < objetosColocados.size(); i++) {
        if (objetosColocados[i].tipo == 6) { 
            desenharSprite(idLapide6, objetosColocados[i].x, objetosColocados[i].y, 0.0, 0.7, 0.6);
        }
    }
    if (segurandoObjeto && objetoSelecionado == 6) {
       
        float lapide6MouseX = moverMouseX - camX;
        float lapide6MouseY = moverMouseY - camY;
        
        desenharSprite(idLapide6, lapide6MouseX, lapide6MouseY, 0.0, 0.7, 0.6);
    }

     for (size_t i = 0; i < objetosColocados.size(); i++) {
        if (objetosColocados[i].tipo == 7) { 
            desenharSprite(idArbusto, objetosColocados[i].x, objetosColocados[i].y, 0.0, 0.7, 0.6);
        }
    }

    if (segurandoObjeto && objetoSelecionado == 7) {
       
        float arbustoMouseX = moverMouseX - camX;
        float arbustoMouseY = moverMouseY - camY;
        
        desenharSprite(idArbusto, arbustoMouseX, arbustoMouseY, 0.0, 0.7, 0.6);
    }


     for (size_t i = 0; i < objetosColocados.size(); i++) {
        if (objetosColocados[i].tipo == 8) { 
            desenharSprite(idCaixao1, objetosColocados[i].x, objetosColocados[i].y, 0.0, 0.7, 0.6);
        }
    }

    if (segurandoObjeto && objetoSelecionado == 8) {
       
        float caixao1MouseX = moverMouseX - camX;
        float caixao1MouseY = moverMouseY - camY;
        
        desenharSprite(idCaixao1, caixao1MouseX, caixao1MouseY, 0.0, 0.7, 0.6);
    }


     for (size_t i = 0; i < objetosColocados.size(); i++) {
        if (objetosColocados[i].tipo == 9) { 
            desenharSprite(idCaixao2, objetosColocados[i].x, objetosColocados[i].y, 0.0, 0.7, 0.6);
        }
    }

    if (segurandoObjeto && objetoSelecionado == 9) {
       
        float caixao2MouseX = moverMouseX - camX;
        float caixao2MouseY = moverMouseY - camY;
        
        desenharSprite(idCaixao2, caixao2MouseX, caixao2MouseY, 0.0, 0.7, 0.6);
    }

    for (size_t i = 0; i < objetosColocados.size(); i++) {
        if (objetosColocados[i].tipo == 10) { 
            desenharSprite(idCaveira, objetosColocados[i].x, objetosColocados[i].y, 0.0, 0.7, 0.6);
        }
    }

    if (segurandoObjeto && objetoSelecionado == 10) {
       
        float caveiraMouseX = moverMouseX - camX;
        float caveiraMouseY = moverMouseY - camY;
        
        desenharSprite(idCaveira, caveiraMouseX, caveiraMouseY, 0.0, 0.7, 0.6);
    }


    for (size_t i = 0; i < objetosColocados.size(); i++) {
        if (objetosColocados[i].tipo == 11) { 
            desenharSprite(idCorvo, objetosColocados[i].x, objetosColocados[i].y, 0.0, 0.7, 0.6);
        }
    }

    if (segurandoObjeto && objetoSelecionado == 11) {
       
        float corvoMouseX = moverMouseX - camX;
        float corvoMouseY = moverMouseY - camY;
        
        desenharSprite(idCorvo, corvoMouseX, corvoMouseY, 0.0, 0.7, 0.6);
    }


    for (size_t i = 0; i < objetosColocados.size(); i++) {
        if (objetosColocados[i].tipo == 12) { 
            desenharSprite(idFlor, objetosColocados[i].x, objetosColocados[i].y, 0.0, 0.7, 0.6);
        }
    }

    if (segurandoObjeto && objetoSelecionado == 12) {
       
        float florMouseX = moverMouseX - camX;
        float florMouseY = moverMouseY - camY;
        
        desenharSprite(idFlor, florMouseX, florMouseY, 0.0, 0.7, 0.6);
    }


    for (size_t i = 0; i < objetosColocados.size(); i++) {
        if (objetosColocados[i].tipo == 13) { 
            desenharSprite(idGato, objetosColocados[i].x, objetosColocados[i].y, 0.0, 0.7, 0.6);
        }
    }

    if (segurandoObjeto && objetoSelecionado == 13) {
       
        float gatoMouseX = moverMouseX - camX;
        float gatoMouseY = moverMouseY - camY;
        
        desenharSprite(idGato, gatoMouseX, gatoMouseY, 0.0, 0.7, 0.6);
    }


    for (size_t i = 0; i < objetosColocados.size(); i++) {
        if (objetosColocados[i].tipo == 14) { 
            desenharSprite(idVela, objetosColocados[i].x, objetosColocados[i].y, 0.0, 0.7, 0.6);
        }
    }

    if (segurandoObjeto && objetoSelecionado == 14) {
       
        float velaMouseX = moverMouseX - camX;
        float velaMouseY = moverMouseY - camY;
        
        desenharSprite(idVela, velaMouseX, velaMouseY, 0.0, 0.7, 0.6);
    }


     for (size_t i = 0; i < objetosColocados.size(); i++) {
        if (objetosColocados[i].tipo == 15) { 
            desenharSprite(idCoracao, objetosColocados[i].x, objetosColocados[i].y, 0.0, 0.7, 0.6);
        }
    }

    if (segurandoObjeto && objetoSelecionado == 15) {
       
        float coracaoMouseX = moverMouseX - camX;
        float coracaoMouseY = moverMouseY - camY;
        
        desenharSprite(idCoracao, coracaoMouseX, coracaoMouseY, 0.0, 0.7, 0.6);
    }

}

void desenharCena() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); 

    if(telaAtual == 0) {  
     //------fixa elementos no todo ------
        desenharJogo();
        glPushMatrix();
            glLoadIdentity(); 
            desenharSprite(idBotao, 4.0, 3.6, 0.0, 1.0, 1.0); 
            desenharSprite(idMoeda, 3.0, 3.62, 0.0, 0.8, 0.8);
            glColor3f(0.290196f, 0.200000f, 0.098039f);
            desenharTexto( std::to_string(moedas), 2.6, 3.52);
            desenharSprite(idQuiz, 1.7, 3.6, 0.0, 0.8, 0.3);
        glPopMatrix();  

    } else if(telaAtual == 1) {

    desenhaLoja();
    glPushMatrix();
        glLoadIdentity(); 
        desenharSprite(idBotao, 4.0, 3.6, 0.0, 1.0, 1.0);
        desenharSprite(idMoeda, 3.0, 3.62, 0.0, 0.8, 0.8);
        glColor3f(0.290196f, 0.200000f, 0.098039f);
        desenharTexto(  std::to_string(moedas), 2.6, 3.52);
        desenharSprite(idQuiz, 1.7, 3.6, 0.0, 0.8, 0.3);
    glPopMatrix();

    } else if(telaAtual == 2) {
        desenharQuiz();
        
        desenharSprite(idBotao, 4.0, 3.6, 0.0, 1.0, 1.0);
        desenharSprite(idMoeda, 3.0, 3.62, 0.0, 0.8, 0.8);
        glColor3f(0.290196f, 0.200000f, 0.098039f);
        desenharTexto( std::to_string(moedas), 2.6, 3.52);
        desenharSprite(idQuiz, 1.7, 3.6, 0.0, 0.8, 0.3);
    }

    glutSwapBuffers();
}

void mouse(int botao, int estado, int x, int y) {
    if(botao == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {

        float mouseX = (x / (float) larguraJanela) * 10.0f - 5.0f;
        float mouseY = 4.0f - (y / (float) alturaJanela) * 8.0f;

        float mouseYReal = mouseY - camY;

        if(telaAtual == 2 && mostrandoResultado) {
            mostrandoResultado = false;
            perguntaAtual++;

            alternativaSelecionada = ' ';

            int totalPerguntas = sizeof(quiz) / sizeof(quiz[0]);

            if(perguntaAtual >= totalPerguntas) {
                telaAtual = 0;
                perguntaAtual = 0;
            }
                return;
        }

        if(mouseX >= 3.8 && mouseX <= 4.2 && mouseY >= 3.5 && mouseY <= 3.9) {
            if(telaAtual == 0) {
                telaAtual = 1;
            } else if(telaAtual == 1) {
                telaAtual = 0; 
            } else if(telaAtual == 2) {
                telaAtual = 1;
            }
        }

        if(mouseX >= 1.6 && mouseX <= 2.4 && mouseY >= 3.45 && mouseY <= 3.75) {
            if(telaAtual == 0) {
                telaAtual = 2;
            } else if(telaAtual == 2) {
                telaAtual = 0;
            } else if(telaAtual == 1) {
                telaAtual = 2;
            }
        }

       if(telaAtual == 2){
        // Alternativa A
        if(mouseX >= -3.5 && mouseX <= 3.5 &&
        mouseY >= 1.2 && mouseY <= 2.0)
        {
            alternativaSelecionada = 'A';
            responder('A');
        }

        // Alternativa B
        if(mouseX >= -3.5 && mouseX <= 3.5 &&
        mouseY >= 0.2 && mouseY <= 1.0)
        {
            alternativaSelecionada = 'B';
            responder('B');
        }

        // Alternativa C
        if(mouseX >= -3.5 && mouseX <= 3.5 &&
        mouseY >= -0.8 && mouseY <= 0.0)
        {
            alternativaSelecionada = 'C';
            responder('C');
        }

        // Alternativa D
        if(mouseX >= -3.5 && mouseX <= 3.5 &&
        mouseY >= -1.8 && mouseY <= -1.0)
        {
            alternativaSelecionada = 'D';
            responder('D');
        }
}

        if( telaAtual == 1){
            if (mouseX >= -4.1 && mouseX <= -3.1 && mouseYReal >= 0.8 && mouseYReal <= 1.2){
                if(moedas >= 15){
                    moedas -= 15;

                    segurandoObjeto = true;
                    objetoSelecionado = 1; 

                    telaAtual = 0; 
                    return;
                }
                else
                {
                    mensagemLoja = "Moedas insuficientes!";
                    tempoMensagemLoja = 10;
                }
                
            }

        }


         if( telaAtual == 1){
            if (mouseX >= -2.3 && mouseX <= -1.3 && mouseYReal >= 0.8 && mouseYReal <= 1.2) {
                if(moedas >= 15){
                    moedas -= 15;

                    segurandoObjeto = true;
                    objetoSelecionado = 2; 

                    telaAtual = 0; 
                    return;
                }
                else
                {
                    mensagemLoja = "Moedas insuficientes!";
                    tempoMensagemLoja = 10;
                }
                
            }

        }

         if( telaAtual == 1){
            if (mouseX >= -0.5 && mouseX <= 0.5 && mouseYReal >= 0.8 && mouseYReal <= 1.2) {
                if(moedas >= 15){
                    moedas -= 15;

                    segurandoObjeto = true;
                    objetoSelecionado = 3; 

                    telaAtual = 0; 
                    return;
                }
                else
                {
                    mensagemLoja = "Moedas insuficientes!";
                    tempoMensagemLoja = 10;
                }
                
            }

        }
//-----------
        if( telaAtual == 1){
           if (mouseX >= 1.3 && mouseX <= 2.3 && mouseYReal >= 0.8 && mouseYReal <= 1.2) {
                if(moedas >= 15){
                    moedas -= 15;

                    segurandoObjeto = true;
                    objetoSelecionado = 4; 

                    telaAtual = 0; 
                    return;
                }
                else
                {
                    mensagemLoja = "Moedas insuficientes!";
                    tempoMensagemLoja = 10;
                }
                
            }

        }

        if( telaAtual == 1){
            if (mouseX >= 3.1 && mouseX <= 4.1 && mouseYReal >= 0.8 && mouseYReal <= 1.2) {
                if(moedas >= 20){
                    moedas -= 20;

                    segurandoObjeto = true;
                    objetoSelecionado = 5; 

                    telaAtual = 0; 
                    return;
                }
                else
                {
                    mensagemLoja = "Moedas insuficientes!";
                    tempoMensagemLoja = 10;
                }
                
            }

        }

        if( telaAtual == 1){
            if (mouseX >= -4.1 && mouseX <= -3.1 && mouseYReal >= -1.5 && mouseYReal <= -1.1) {
                if(moedas >= 20){
                    moedas -= 20;

                    segurandoObjeto = true;
                    objetoSelecionado = 6; 

                    telaAtual = 0; 
                    return;
                }
                else
                {
                    mensagemLoja = "Moedas insuficientes!";
                    tempoMensagemLoja = 10;
                }
                
            }

        }

        if( telaAtual == 1){
           if (mouseX >= -2.3 && mouseX <= -1.3 && mouseYReal >= -1.5 && mouseYReal <= -1.1) {
                if(moedas >= 30){
                    moedas -= 30;

                    segurandoObjeto = true;
                    objetoSelecionado = 7; 

                    telaAtual = 0; 
                    return;
                }
                else
                {
                    mensagemLoja = "Moedas insuficientes!";
                    tempoMensagemLoja = 10;
                }
                
            }

        }

        if( telaAtual == 1){
            if (mouseX >= -0.5 && mouseX <= 0.5 && mouseYReal >= -1.5 && mouseYReal <= -1.1) {
                if(moedas >= 35){
                    moedas -= 35;

                    segurandoObjeto = true;
                    objetoSelecionado = 8; 

                    telaAtual = 0; 
                    return;
                }
                else
                {
                    mensagemLoja = "Moedas insuficientes!";
                    tempoMensagemLoja = 10;
                }
                
            }

        }

        if( telaAtual == 1){
            if (mouseX >= 1.3 && mouseX <= 2.3 && mouseYReal >= -1.5 && mouseYReal <= -1.1) {
                if(moedas >= 35){
                    moedas -= 35;

                    segurandoObjeto = true;
                    objetoSelecionado = 9; 

                    telaAtual = 0; 
                    return;
                }
                else
                {
                    mensagemLoja = "Moedas insuficientes!";
                    tempoMensagemLoja = 10;
                }
                
            }

        }


        if( telaAtual == 1){
            if (mouseX >= 3.1 && mouseX <= 4.1 && mouseYReal >= -1.5 && mouseYReal <= -1.1) {
                if(moedas >= 50){
                    moedas -= 50;

                    segurandoObjeto = true;
                    objetoSelecionado = 10; 

                    telaAtual = 0; 
                    return;
                }
                else
                {
                    mensagemLoja = "Moedas insuficientes!";
                    tempoMensagemLoja = 10;
                }
                
            }

        }

        if( telaAtual == 1){
            if (mouseX >= -4.1 && mouseX <= -3.1 && mouseYReal >= -3.8 && mouseYReal <= -3.4) {
                if(moedas >= 60){
                    moedas -= 60;

                    segurandoObjeto = true;
                    objetoSelecionado = 11; 

                    telaAtual = 0; 
                    return;
                }
                else
                {
                    mensagemLoja = "Moedas insuficientes!";
                    tempoMensagemLoja = 10;
                }
                
            }

        }

        if( telaAtual == 1){
            if (mouseX >= -2.3 && mouseX <= -1.3 && mouseYReal >= -3.8 && mouseYReal <= -3.4) {
                if(moedas >= 40){
                    moedas -= 40;

                    segurandoObjeto = true;
                    objetoSelecionado = 12; 

                    telaAtual = 0; 
                    return;
                }
                else
                {
                    mensagemLoja = "Moedas insuficientes!";
                    tempoMensagemLoja = 10;
                }
                
            }

        }

        if( telaAtual == 1){
            if (mouseX >= -0.5 && mouseX <= 0.5 && mouseYReal >= -3.8 && mouseYReal <= -3.4) {
                if(moedas >= 70){
                    moedas -= 70;

                    segurandoObjeto = true;
                    objetoSelecionado = 13; 

                    telaAtual = 0; 
                    return;
                }
                else
                {
                    mensagemLoja = "Moedas insuficientes!";
                    tempoMensagemLoja = 10;
                }
                
            }

        }

        if( telaAtual == 1){
            if (mouseX >= 1.3 && mouseX <= 2.3 && mouseYReal >= -3.8 && mouseYReal <= -3.4) {
                if(moedas >= 40){
                    moedas -= 40;

                    segurandoObjeto = true;
                    objetoSelecionado = 14; 

                    telaAtual = 0; 
                    return;
                }
                else
                {
                    mensagemLoja = "Moedas insuficientes!";
                    tempoMensagemLoja = 10;
                }
                
            }

        }

        if( telaAtual == 1){
            if (mouseX >= 3.1 && mouseX <= 4.1 && mouseYReal >= -3.8 && mouseYReal <= -3.4) {
                if(moedas >= 100){
                    moedas -= 100;

                    segurandoObjeto = true;
                    objetoSelecionado = 15; 

                    telaAtual = 0; 
                    return;
                }
                else
                {
                    mensagemLoja = "Moedas insuficientes!";
                    tempoMensagemLoja = 10;
                }
                
            }

        }

        else if(telaAtual == 0 && segurandoObjeto == true) {
    
            ObjetoCenario novoObjeto;
            novoObjeto.tipo = objetoSelecionado;
    
   
            novoObjeto.x = mouseX - camX; 
            novoObjeto.y = mouseY - camY;

    
            objetosColocados.push_back(novoObjeto);

   
            segurandoObjeto = false; 
            objetoSelecionado = 0;   
    
        }        
            return; 
        }
    }

void inicializar() {
    glClearColor(0.05, 0.05, 0.1, 1.0); 
    
    // Inicializa a textura 
    idBotao = carregarTextura("botao_cerebro.png");
    idMoeda = carregarTextura("moeda.png");
    idLapide = carregarTextura("lapide_vazia.png");
    idArvore1 = carregarTextura("arvore1.png");
    idArvore2 = carregarTextura("arvore2.png");
    idArvore3 = carregarTextura("arvore3.png");
    idCercaFrontal = carregarTextura("cerca_frontal.png");
    idCercaLateral1 = carregarTextura("cerca_lateral01.png");
    idCercaLateral2 = carregarTextura("cerca_lateral02.png");
    idQuiz = carregarTextura("quiz.png");
    idLapide2 = carregarTextura("Lapide2.png");
    idLapide3 = carregarTextura("Lapide3.png");
    idLapide4 = carregarTextura("Lapide4.png");
    idLapide5 = carregarTextura("Lapide5.png");
    idLapide6 = carregarTextura("Lapide6.png");
    idArbusto = carregarTextura("arbusto.png");
    idCaixao1 = carregarTextura("caixao1.png");
    idCaixao2 = carregarTextura("caixao2.png");
    idCaveira = carregarTextura("caveira.png");
    idCoracao = carregarTextura("coracao.png");
    idCorvo = carregarTextura("corvo.png");
    idVela = carregarTextura("vela.png");
    idFlor = carregarTextura("flor_murcha.png");
    idGato = carregarTextura("gatinho.png");





   zumbiDireita[0] = carregarTextura("zumbi_dir01.png");
   zumbiDireita[1] = carregarTextura("zumbi_dir02.png");

   zumbiEsquerda[0] = carregarTextura("zumbi_esq01.png");
   zumbiEsquerda[1] = carregarTextura("zumbi_esq02.png");

  
   zumbiCostas[0] = carregarTextura("zumbi_costas02.png");
   zumbiCostas[1] = carregarTextura("zumbi_costas03.png");

   zumbiFrente[0] = carregarTextura("zumbi_frente01.png");
   zumbiFrente[1] = carregarTextura("zumbi_frente02.png");
   zumbiFrente[2] = carregarTextura("zumbi_frente03.png");

   
    
}

void pressionarTeclasEspeciais(int tecla, int x, int y) {
    float velocidade = 0.2; 

    if (telaAtual == 1) {
        switch (tecla) {
            case GLUT_KEY_UP:
                camY -= velocidade; 
                break;
            case GLUT_KEY_DOWN:
                camY += velocidade; 
                break;
        }

       
        if (camY < 0.0f) camY = 0.0f;
        if (camY > 2.0f) camY = 2.0f;
    } 
   
    else {
        switch (tecla) {
            case GLUT_KEY_RIGHT:
                camX -= velocidade;
                break;
            case GLUT_KEY_LEFT:
                camX += velocidade; 
                break;
            case GLUT_KEY_DOWN:
                camY += velocidade; 
                break;
            case GLUT_KEY_UP:
                camY -= velocidade;
                break;
        }

        
        if (camX < -5.0f) camX = -5.0f;
        if (camX >  5.0f) camX =  5.0f;
        if (camY < -2.0f) camY = -2.0f;
        if (camY >  2.0f) camY =  2.0f;
    }

    glutPostRedisplay();
}

void redimensionar(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    larguraJanela = w;
    alturaJanela = h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-5, 5, -4, 4, -5, 5);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow(" jogo");

    inicializar();

    glutDisplayFunc(desenharCena);
    glutReshapeFunc(redimensionar);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(movimentoMouse);
    glutTimerFunc(100, atualizar, 0);

    glutSpecialFunc(pressionarTeclasEspeciais);

    glutMainLoop();
    return 0;
}