#ifndef ASTAR_H_
#define ASTAR_H_

#include "laberinto.h"
#include "nodo.h"

struct AStarResult {
  bool ok{false};
  int cost{0};
  int generados{0};
  int inspeccionados{0};
  std::vector<std::pair<int,int>> path;
};

AStarResult run_a_star(const laberinto& lab);
  
#endif