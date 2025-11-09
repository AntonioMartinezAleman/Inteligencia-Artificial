#include "grafo.h"

grafo::grafo() : numero_nodos_(0) {}

grafo::grafo(std::istream& in) {
  in >> numero_nodos_;
  w_.assign(numero_nodos_ + 1, std::vector<double>(numero_nodos_ + 1, -1.0));
  for (int i = 1; i <= numero_nodos_; ++i) {
    for (int j = 1; j <= numero_nodos_; ++j) {
      double v;
      in >> v;
      w_[i][j] = v;
    }
  }
}

int grafo::nodos() const {
  return numero_nodos_;
}

double grafo::coste(int u, int v) const {
  if (u < 1 || v < 1 || u > numero_nodos_ || v > numero_nodos_) return -1.0;
  return w_[u][v];
}

void grafo::printIteracion(int iter, std::ostream& fout, const std::vector<int>& generados, const std::vector<int>& inspeccionados) const {
  fout << "Iteracion " << iter << '\n';
  fout << "Generados: ";
  for (size_t i = 0; i < generados.size(); ++i) {
    if (i) fout << ' ';
    fout << generados[i];
  }
  fout << '\n';
  fout << "Inspeccionados: ";
  for (size_t i = 0; i < inspeccionados.size(); ++i) {
    if (i) fout << ' ';
    fout << inspeccionados[i];
  }
  fout << '\n';
  fout << "-------------------------------\n";
}

void grafo::Amplitud(int ini, int fin, std::ostream& fout) const {
  if (ini == fin) {
    printIteracion(1, fout, std::vector<int>{ini}, {});
    fout << "Camino: " << ini << '\n';
    fout << "-------------------------------\n";
    fout << std::fixed << std::setprecision(2) << "Costo: 0.00\n";
    fout << "-------------------------------\n";
    return;
  }
  std::vector<int> padre(numero_nodos_ + 1, -1);
  std::vector<bool> vis(numero_nodos_ + 1, false);
  std::queue<int> q;
  std::vector<int> generados;
  std::vector<int> inspeccionados;
  q.push(ini);
  vis[ini] = true;
  int iter = 1;
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    inspeccionados.push_back(u);
    if (u == fin) {
      printIteracion(iter++, fout, generados, inspeccionados);
      std::vector<int> camino;
      for (int v = u; v != -1; v = padre[v]) camino.push_back(v);
      std::reverse(camino.begin(), camino.end());
      fout << "Camino: ";
      for (size_t i = 0; i < camino.size(); ++i) {
        if (i) fout << " - ";
        fout << camino[i];
      }
      fout << '\n';
      fout << "-------------------------------\n";
      fout << std::fixed << std::setprecision(2) << "Costo: 0.00\n";
      fout << "-------------------------------\n";
      return;
    }
    for (int v = 1; v <= numero_nodos_; ++v) {
      double w = coste(u, v);
      if (w >= 0 && !vis[v]) {
        vis[v] = true;
        padre[v] = u;
        q.push(v);
        generados.push_back(v);
      }
    }
    printIteracion(iter++, fout, generados, inspeccionados);
  }
  fout << "Camino: -\n";
  fout << "-------------------------------\n";
  fout << std::fixed << std::setprecision(2) << "Costo: 0.00\n";
  fout << "-------------------------------\n";
}

void grafo::Profundidad(int ini, int fin, std::ostream& fout) const {
  std::vector<int> padre(numero_nodos_ + 1, -1);
  std::vector<bool> vis(numero_nodos_ + 1, false);
  std::vector<int> generados;
  std::vector<int> inspeccionados;
  int iter = 1;
  std::function<bool(int)> dfs = [&](int u) {
    inspeccionados.push_back(u);
    if (u == fin) {
      printIteracion(iter++, fout, generados, inspeccionados);
      std::vector<int> camino;
      for (int v = u; v != -1; v = padre[v]) camino.push_back(v);
      std::reverse(camino.begin(), camino.end());
      fout << "Camino: ";
      for (size_t i = 0; i < camino.size(); ++i) {
        if (i) fout << " - ";
        fout << camino[i];
      }
      fout << '\n';
      fout << "-------------------------------\n";
      fout << std::fixed << std::setprecision(2) << "Costo: 0.00\n";
      fout << "-------------------------------\n";
      return true;
    }
    vis[u] = true;
    for (int v = 1; v <= numero_nodos_; ++v) {
      if (!vis[v] && coste(u, v) >= 0) {
        padre[v] = u;
        generados.push_back(v);
        printIteracion(iter++, fout, generados, inspeccionados);
        if (dfs(v)) return true;
      }
    }
    return false;
  };
  if (!dfs(ini)) {
    fout << "Camino: -\n";
    fout << "-------------------------------\n";
    fout << std::fixed << std::setprecision(2) << "Costo: 0.00\n";
    fout << "-------------------------------\n";
  }
}

void grafo::AmplitudAleatoria(int ini, int fin, std::ostream& fout) const {
  if (ini == fin) {
    printIteracion(1, fout, std::vector<int>{ini}, {});
    fout << "Camino: " << ini << '\n';
    fout << "-------------------------------\n";
    fout << std::fixed << std::setprecision(2) << "Costo: 0.00\n";
    fout << "-------------------------------\n";
    return;
  }
  using PQ = std::pair<double,int>;
  std::set<PQ> abiertos;
  std::vector<double> g(numero_nodos_ + 1, std::numeric_limits<double>::infinity());
  std::vector<int> padre(numero_nodos_ + 1, -1);
  std::vector<bool> cerrado(numero_nodos_ + 1, false);
  std::vector<int> generados;
  std::vector<int> inspeccionados;
  g[ini] = 0.0;
  abiertos.insert({0.0, ini});
  int iter = 1;
  std::mt19937 rng(std::random_device{}());
  while (!abiertos.empty()) {
    std::vector<PQ> candidatos;
    while (!abiertos.empty() && candidatos.size() < 2) {
      auto it = abiertos.begin();
      PQ p = *it;
      abiertos.erase(it);
      int n = p.second;
      double gc = p.first;
      if (cerrado[n]) continue;
      if (gc != g[n]) continue;
      candidatos.push_back(p);
    }
    if (candidatos.empty()) break;
    int elegir = 0;
    if (candidatos.size() == 2) {
      std::uniform_int_distribution<int> dist(0,1);
      elegir = dist(rng);
    }
    for (size_t i = 0; i < candidatos.size(); ++i) {
      if ((int)i != elegir) abiertos.insert(candidatos[i]);
    }
    int u = candidatos[elegir].second;
    if (u == fin) {
      inspeccionados.push_back(u);
      printIteracion(iter++, fout, generados, inspeccionados);
      std::vector<int> camino;
      for (int v = u; v != -1; v = padre[v]) camino.push_back(v);
      std::reverse(camino.begin(), camino.end());
      fout << "Camino: ";
      for (size_t i = 0; i < camino.size(); ++i) {
        if (i) fout << " - ";
        fout << camino[i];
      }
      fout << '\n';
      fout << "-------------------------------\n";
      fout << std::fixed << std::setprecision(2) << "Costo: " << g[u] << '\n';
      fout << "-------------------------------\n";
      return;
    }
    cerrado[u] = true;
    inspeccionados.push_back(u);
    for (int v = 1; v <= numero_nodos_; ++v) {
      double w = coste(u, v);
      if (w >= 0 && !cerrado[v]) {
        double ng = g[u] + w;
        if (ng < g[v]) {
          if (g[v] < std::numeric_limits<double>::infinity()) {
            auto it_old = abiertos.find({g[v], v});
            if (it_old != abiertos.end()) abiertos.erase(it_old);
          }
          g[v] = ng;
          padre[v] = u;
          abiertos.insert({g[v], v});
          generados.push_back(v);
        }
      }
    }
    printIteracion(iter++, fout, generados, inspeccionados);
  }
  fout << "Camino: -\n";
  fout << "-------------------------------\n";
  fout << std::fixed << std::setprecision(2) << "Costo: 0.00\n";
  fout << "-------------------------------\n";
}
