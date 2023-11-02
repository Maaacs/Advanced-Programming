#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

typedef struct {
    Point p1, p2;
} bounding_box;

// img  - Imagem
// x, y - Posição da imagem onde a pintura irá começar
// whiteThreshold - Escala de cinza em que qualquer valor maior é considerado "branco"
// bbox - Retângulo que contém os pixels pintados
void floodFill(Mat img, int x, int y, int whiteThreshold, bounding_box *bbox) {
    // Se estiver fora dos limites da imagem, ignora
    if (x < 0 || x >= img.cols || y < 0 || y >= img.rows) return;

    // Acessa o valor do pixel
    uchar pixel = img.at<uchar>(y,x);

    // Se o pixel atual não é considerado "branco", ignora
    if (pixel < whiteThreshold) return;

    // Pinta o pixel para preto
    img.at<uchar>(y,x) = 0;

    // Atualiza o retângulo
    if (x < bbox->p1.x) bbox->p1.x = x;
    if (x > bbox->p2.x) bbox->p2.x = x;
    if (y < bbox->p1.y) bbox->p1.y = y;
    if (y > bbox->p2.y) bbox->p2.y = y;

    // Executa recursivamente para cima, baixo, esquerda e direita
    floodFill(img, x, y+1, whiteThreshold, bbox);
    floodFill(img, x, y-1, whiteThreshold, bbox);
    floodFill(img, x-1, y, whiteThreshold, bbox);
    floodFill(img, x+1, y, whiteThreshold, bbox);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "Sintaxe: " << argv[0] << " <nome-arquivo-png>" << endl;
        return 1;
    }

    // Abre a imagem em modo escala-de-cinza
    Mat img = imread(argv[1], IMREAD_GRAYSCALE);

    // Cria um vetor para salvar as estrelas encontradas
    vector<bounding_box> stars;
    
    // Define um limite para considerar um pixel como branco
    int whiteThreshold = 40;

    // Para cada pixel na imagem
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            // Se o pixel for branco
            if (img.at<uchar>(y,x) > whiteThreshold) {
                // Cria um bounding box
                bounding_box bbox = {
                    .p1 = Point(img.cols, img.rows),
                    .p2 = Point(0, 0)
                };
                // Executa o flood fill no pixel
                floodFill(img, x, y, whiteThreshold, &bbox);
                // Salva o bounding box no vetor de estrelas
                stars.push_back(bbox);
            }
        }
    }

    // Abre a imagem original em modo colorido
    Mat imgOriginal = imread(argv[1], IMREAD_COLOR);

    // Para cada estrela no vetor
    for (auto star : stars) {
        // Pinta o retângulo na imagem original
        rectangle(imgOriginal, star.p1, star.p2, Scalar(0, 255, 0), 1);
    }

    // Salva a imagem com os retângulos
    imwrite("output-stars.png", imgOriginal);

    // Imprime a quantidade de estrelas encontradas
    cout << "Encontrei " << stars.size() << " estrelas na imagem." << endl;

}
