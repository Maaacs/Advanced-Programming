#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <GL/glut.h>

typedef struct {
  double x, y;
} Ponto;

typedef struct {
  Ponto p, q;
} Segmento;


int largura, altura, quantidade; 
Segmento *segmentos; // Vetor 


double rand01() {
  return (double)rand() / RAND_MAX;
}

// Gera dentro da janela
Ponto ponto_aleatorio() {
  Ponto p;
  p.x = rand01() * largura;
  p.y = rand01() * altura;
  return p;
}


double produto_vetorial(Ponto a, Ponto b) {
  return a.x * b.y - a.y * b.x;
}


int pontos_iguais(Ponto a, Ponto b) {
  return a.x == b.x && a.y == b.y;
}

// Ponto entre dois outros pontos na reta
int ponto_entre(Ponto p, Ponto a, Ponto b) {
  return produto_vetorial(a, b) == 0 && 
         (a.x - p.x) * (b.x - p.x) <= 0 && 
         (a.y - p.y) * (b.y - p.y) <= 0; 
}


int segmentos_intersectam(Segmento s1, Segmento s2) {
  if (pontos_iguais(s1.p, s2.p) || pontos_iguais(s1.p, s2.q) ||
      pontos_iguais(s1.q, s2.p) || pontos_iguais(s1.q, s2.q)) {
    return 1;
  }
  if (ponto_entre(s1.p, s2.p, s2.q) || ponto_entre(s1.q, s2.p, s2.q) ||
      ponto_entre(s2.p, s1.p, s1.q) || ponto_entre(s2.q, s1.p, s1.q)) {
    return 1;
  }
  
  int ccw1 = produto_vetorial(s2.q, s1.p) > produto_vetorial(s2.q, s2.p);
  int ccw2 = produto_vetorial(s2.q, s1.q) > produto_vetorial(s2.q, s2.p);
  int ccw3 = produto_vetorial(s1.q, s2.p) > produto_vetorial(s1.q, s1.p);
  int ccw4 = produto_vetorial(s1.q, s2.q) > produto_vetorial(s1.q, s1.p);
  
  return ccw1 != ccw2 && ccw3 != ccw4;
}


Segmento segmento_aleatorio() {
  Segmento s;
  
  while (1) {
    do {
      s.p = ponto_aleatorio();
      s.q = ponto_aleatorio();
    } while (pontos_iguais(s.p, s.q));
    
    // Verifica se o segmento intersecta outro
    int valido = 1;
    for (int i = 0; i < quantidade; i++) {
      if (segmentos_intersectam(s, segmentos[i])) {
        valido = 0;
        break;
      }
    }
    if (valido) {
      return s;
    }
  }
}

void inicializar_segmentos() {
  segmentos = malloc(quantidade * sizeof(Segmento));
  for (int i = 0; i < quantidade; i++) {
    segmentos[i] = segmento_aleatorio();
  }
}


void desenhar_segmentos() {
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.0, 0.0, 0.0);

  for (int i = 0; i < quantidade; i++) {
    glBegin(GL_LINES);
    glVertex2d(segmentos[i].p.x, segmentos[i].p.y);
    glVertex2d(segmentos[i].q.x, segmentos[i].q.y);
    glEnd();
  }
  
  glFlush();
}


void configurar_janela(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(largura, altura);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Geometria Computacional / OpenGL");
  glutDisplayFunc(desenhar_segmentos);
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, largura, 0.0, altura);
}


int main(int argc, char **argv) {

  if (argc != 4) {
    printf("Digite: %s <largura> <altura> <quantidade>\n", argv[0]);
    return -1;
  }
  
  largura = atoi(argv[1]);
  altura = atoi(argv[2]);
  quantidade = atoi(argv[3]);
  
  if (largura <= 0 || altura <= 0 || quantidade <= 0) {
    printf("Os valores devem ser positivos\n");
    return -1;
  }

  srand(time(NULL));

  inicializar_segmentos();
  
  configurar_janela(argc, argv);
  
  glutMainLoop();
  
  free(segmentos);
  
  return 0;
}
