#include <GL/glut.h>
#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


GLuint idArvore;


GLuint carregarTextura(const std::string& caminhoArquivo) {
    int largura, altura, canais;
    
    // Força 4 canais (RGBA) para garantir a transparência do PNG
    unsigned char* dados = stbi_load(caminhoArquivo.c_str(), &largura, &altura, &canais, 4);
    
    GLuint idTextura = 0;

    if (dados) {
        glGenTextures(1, &idTextura);
        glBindTexture(GL_TEXTURE_2D, idTextura);

        // Filtros ideais para Pixel Art (Maltém os pixels nítidos)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       
        // Envia os pixels para a GPU
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, largura, altura, 0, GL_RGBA, GL_UNSIGNED_BYTE, dados);
        
        stbi_image_free(dados); // Libera a memória RAM
        std::cout << "Sucesso ao carregar: " << caminhoArquivo << std::endl;
    } else {
        std::cout << "Erro ao carregar a textura: " << caminhoArquivo << std::endl;
    }

    return idTextura;
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
        // Inferior Esquerdo
        glTexCoord2f(0.0f, 1.0f); glVertex3f(posX - meioX, posY - meioY, posZ);
        // Inferior Direito
        glTexCoord2f(1.0f, 1.0f); glVertex3f(posX + meioX, posY - meioY, posZ);
        // Superior Direito
        glTexCoord2f(1.0f, 0.0f); glVertex3f(posX + meioX, posY + meioY, posZ);
        // Superior Esquerdo
        glTexCoord2f(0.0f, 0.0f); glVertex3f(posX - meioX, posY + meioY, posZ);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}


void desenharCena() {
    // Limpa a tela e o buffer de profundidade
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 8.0, 4.0,      
              0.0, 0.0, 0.0,      
              0.0, 1.0, 0.0);
   // glTranslatef(0.0f, 0.0f, -5.0f);


    // ---------------- CHÃO ----------------
    glBegin(GL_QUADS);
        glColor3f(0.3f, 0.3f, 0.3f);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-6.0f, -0.6f, -5.0f);
        glVertex3f(-6.0f, -0.6f,  5.0f);
        glVertex3f( 6.0f, -0.6f,  5.0f);
        glVertex3f( 6.0f, -0.6f, -5.0f);
    glEnd();
    
    // ---------------- ÁRVORE ----------------
    // Parâmetros: (ID da textura, PosX, PosY, PosZ, Largura, Altura)
    // Ajustei a altura para a base da árvore encostar perfeitamente no chão (-1.2f)
    desenharSprite(idArvore, 0.0f, 0.0f, 0.0f, 2.0f, 2.4f);

    glutSwapBuffers();
}

void inicializar() {
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f); // Fundo escuro (azul da noite)
    glEnable(GL_DEPTH_TEST);
    
    // Inicializa a textura usando nossa nova função genérica
    idArvore = carregarTextura("arvore.png");
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
    glutCreateWindow("Arvore OpenGL - Reutilizavel");

    inicializar();

    glutDisplayFunc(desenharCena);
    glutReshapeFunc(redimensionar);

    glutMainLoop();
    return 0;
}