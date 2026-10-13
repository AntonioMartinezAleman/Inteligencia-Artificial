#ifndef LABERINTO_H_
#define LABERINTO_H_

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <queue>
#include <limits>
#include <algorithm>
#include <random>


class laberinto {
 public:
  laberinto(std::ifstream& file);

  int filas() const { return nFilas; }
  int cols() const { return nCols; }
  std::pair<int,int> getInicio() const { return inicio; }
  std::pair<int,int> getFin() const { return fin; }

  char celda(int f,int c) const { return mapa[f][c]; }
  void print() const;
  bool esLibre(int f, int c) const;
  int heuristica(int f, int c) const;
  struct Vecino { 
    int f, c, coste; 
  };

  std::vector<Vecino> vecinos(int f, int c) const;
  void marcarCamino(int f, int c);
  inline bool inBounds(int f,int c) const { return 0<=f && f<nFilas && 0<=c && c<nCols; }
  
  void actualizar_mapa(double pin, double pout);
  int contar_obstaculos() const;
  void forzar_tope_obstaculos(double ratio_max = 0.25);
  void mover_agente_a(int f, int c);
  

  void cambiar_S(int f, int c);
  void cambiar_E(int f, int c);

 private:
  std::vector<std::vector<char>> mapa;
  int nFilas, nCols;
  std::pair<int,int> inicio, fin;
  const int COSTE_ORTOGONAL = 5;
  const int COSTE_DIAGONAL = 7;
};

#endif