#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <set>
#include <queue>
#include <limits>
#include <random>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <functional>


class grafo {
 public:
  grafo();
  explicit grafo(std::istream& in);
  int nodos() const;
  double coste(int u, int v) const;
  void Amplitud(int ini, int fin, std::ostream& fout) const;
  void Profundidad(int ini, int fin, std::ostream& fout) const;
  void AmplitudAleatoria(int ini, int fin, std::ostream& fout) const;
 private:
  int numero_nodos_;
  std::vector<std::vector<double>> w_;
  void printIteracion(int iter, std::ostream& fout, const std::vector<int>& generados, const std::vector<int>& inspeccionados) const;
};

#endif
