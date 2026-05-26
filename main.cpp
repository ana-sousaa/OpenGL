#include <GL/glut.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


float camX = 0.0;
bool direita = false;
bool esquerda = false;

GLuint idArvore;
.gitignore        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0
    );

    glTranslated(camX, 0.0, 0.0);

    // ---------------- CHÃO ----------------
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
       glColor3f(0.5f, 0.5f, 0.5f);

    // um eixo tem que ser 0.0
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-10.0f, -1.2f, -5.0f);  // ajusta para 8.0
        glVertex3f(-10.0f, -1.2f,  5.0f);
        glVertex3f( 10.0f, -1.2f,  5.0f);
        glVertex3f( 10.0f, -1.2f, -5.0f);
    glEnd();
    
   // desenhaArvore(-5.0f, 0.0f, 1.0f);
    desenhaArvore( 0.0, 4.0, 0.5);    // o terceiro parametro e o tamanho da arvore
    //desenhaArvore( 5.0f, 0.0f, 0.8f);

    glutSwapBuffers();
}

void inicializar() {
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    configurarLuz();
    inicializarTexturas();
    
    
}

void redimensionar(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Arvore OpenGL");

    inicializar();

    glutDisplayFunc(desenharCena);
    glutReshapeFunc(redimensionar);
    glutSpecialFunc(pressionarTeclasEspeciais);
    glutSpecialUpFunc(soltarTeclasEspeciais);
    glutIdleFunc(atualizar);
    glutTimerFunc(16, atualizarJogo, 0);

    glutMainLoop();
    return 0;
}