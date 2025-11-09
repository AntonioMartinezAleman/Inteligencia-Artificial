#include "grafo.h"
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Uso: " << argv[0] << " <fichero_entrada> <fichero_salida>\n";
    return 1;
  }

  std::string inpath = argv[1];
  std::string outpath = argv[2];
  int opcion, origen, destino;

  std::ifstream fin(inpath);
  if (!fin) {
    std::cerr << "No se puede abrir entrada\n";
    return 1;
  }

  grafo G(fin);

  std::ofstream fout(outpath);
  if (!fout) {
    std::cerr << "No se puede abrir salida\n";
    return 1;
  }

  std::cout << "Origen: ";
  std::cin >> origen;
  std::cout << "Destino: ";
  std::cin >> destino;
  std::cout << "Escoja recorrido: Amplitud (1), Profundidad (2), Amplitud-2-mejores-aleatoria (3)\n";
  std::cin >> opcion;

  if (opcion == 1) G.Amplitud(origen, destino, fout);
  else if (opcion == 2) G.Profundidad(origen, destino, fout);
  else if (opcion == 3) G.AmplitudAleatoria(origen, destino, fout);
  else {
    std::cerr << "Opcion no valida\n";
    return 1;
  }

  return 0;
}
