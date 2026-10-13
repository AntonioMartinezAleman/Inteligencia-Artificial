#include "astar.h"

AStarResult run_a_star(const laberinto& lab) {
  AStarResult R;
  const int INF = std::numeric_limits<int>::max() / 4;
  int N = lab.filas(), M = lab.cols();
  std::priority_queue<Nodo, std::vector<Nodo>, CmpNodo> open;
  std::vector<std::vector<int>> best_g(N, std::vector<int>(M, INF));
  std::vector<std::vector<std::pair<int,int>>> parent(N, std::vector<std::pair<int,int>>(M, {-1,-1}));
  auto [si, sj] = lab.getInicio();
  auto [fi, fj] = lab.getFin();
  int h0 = lab.heuristica(si, sj);
  best_g[si][sj] = 0;
  open.push(Nodo{si, sj, 0, h0, -1, -1});
  R.generados = 1;

  while (!open.empty()) {
    Nodo u = open.top();
    open.pop();
    R.inspeccionados++;
    if (u.fila == fi && u.col == fj) {
      std::vector<std::pair<int,int>> camino;
      int cf = fi, cc = fj;
      while (cf != -1 && cc != -1) {
        camino.push_back({cf, cc});
        auto p = parent[cf][cc];
        cf = p.first;
        cc = p.second;
      }
      std::reverse(camino.begin(), camino.end());
      R.path = camino;
      R.cost = best_g[fi][fj];
      R.ok = true;
      return R;
    }
    for (auto v : lab.vecinos(u.fila, u.col)) {
      int g2 = u.g + v.coste;
      if (g2 < best_g[v.f][v.c]) {
        best_g[v.f][v.c] = g2;
        parent[v.f][v.c] = {u.fila, u.col};
        int h2 = lab.heuristica(v.f, v.c);
        open.push(Nodo{v.f, v.c, g2, h2, u.fila, u.col});
        R.generados++;
      }
    }
  }

  R.ok = false;
  return R;
}