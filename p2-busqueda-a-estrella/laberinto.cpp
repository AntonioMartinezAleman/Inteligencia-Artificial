#include "laberinto.h"

laberinto::laberinto(std::ifstream& file) {
  file >> nFilas;
  file >> nCols;
  mapa.resize(nFilas, std::vector<char>(nCols));
  for (int i = 0; i < nFilas; i++) {
    for (int j = 0; j < nCols; j++) {
      file >> mapa[i][j];
      if (mapa[i][j] == '3') {
        inicio = {i,j};
        mapa[i][j] = 'S';
      } else if (mapa[i][j] == '4') {
        fin = {i,j};
        mapa[i][j] = 'E';
      } 
    }
  }
}

void laberinto::print() const {
  for (int i = 0; i < nFilas; i++) {
    for (int j = 0; j < nCols; j++) {
      std::cout << mapa[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

bool laberinto::esLibre(int f, int c) const {
  if (f < 0 || f >= nFilas || c < 0 || c >= nCols) {
    return false;
  }
  return mapa[f][c] != '1';
}

int laberinto::heuristica(int f, int c) const {
  return (std::abs(f - fin.first) + std::abs(c - fin.second)) * 3;
}

void laberinto::marcarCamino(int f, int c) {
  if (mapa[f][c] == '0' )
    mapa[f][c] = '*';
}

std::vector<laberinto::Vecino> laberinto::vecinos(int f, int c) const {

  std::vector<Vecino> result;

  static const int dr[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
  static const int dc[8] = { 0, 0,-1, 1, -1,  1,-1, 1};

  for (int k = 0; k < 8; ++k) {
    int nf = f + dr[k];
    int nc = c + dc[k];
    int coste = (k < 4 ? COSTE_ORTOGONAL : COSTE_DIAGONAL);

    if (inBounds(nf, nc) && esLibre(nf, nc)) {
      result.push_back({nf, nc, coste});
    }
  }

  return result;
}

void laberinto::actualizar_mapa(double pin, double pout) {
  static std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<double> dist(0.0, 1.0);

  for (int i = 0; i < nFilas; ++i) {
    for (int j = 0; j < nCols; ++j) {
      char &cell = mapa[i][j];
      if (cell == 'S' || cell == 'E' || cell == '*') continue;
      double r = dist(rng);
      if (cell == '0') {
        if (r < pin) cell = '1';
      } else if (cell == '1') {
        if (r < pout) cell = '0';
      }
    }
  }
}

int laberinto::contar_obstaculos() const {
  int contador = 0;
  for (int i = 0; i < nFilas; ++i) {
    for (int j = 0; j < nCols; ++j) {
      if (mapa[i][j] == '1')
        contador++;
    }
  }
  return contador;
}

void laberinto::forzar_tope_obstaculos(double ratio_max) {
  int total = nFilas * nCols;
  int max_obs = static_cast<int>(std::floor(ratio_max * total));
  int actuales = contar_obstaculos();
  if (actuales <= max_obs) return;

  std::vector<std::pair<int,int>> obstaculos;
  obstaculos.reserve(actuales);
  for (int i = 0; i < nFilas; ++i) {
    for (int j = 0; j < nCols; ++j) {
      if (mapa[i][j] == '1')
        obstaculos.push_back({i, j});
    }
  }

  static std::mt19937 rng(std::random_device{}());
  std::shuffle(obstaculos.begin(), obstaculos.end(), rng);

  int quitar = actuales - max_obs;
  for (int k = 0; k < quitar && k < static_cast<int>(obstaculos.size()); ++k) {
    int i = obstaculos[k].first;
    int j = obstaculos[k].second;
    mapa[i][j] = '0';
  }
}

void laberinto::mover_agente_a(int f, int c) {
  if (!inBounds(f, c)) return;
  inicio = {f, c};
}

void laberinto::cambiar_S(int f, int c) {
  if (!inBounds(f, c)) return;
  if (mapa[inicio.first][inicio.second] == 'S')
    mapa[inicio.first][inicio.second] = '0';
  inicio = {f, c};
  if (mapa[f][c] != 'E')
    mapa[f][c] = 'S';
}

void laberinto::cambiar_E(int f, int c) {
  if (!inBounds(f, c)) return;
  if (mapa[fin.first][fin.second] == 'E')
    mapa[fin.first][fin.second] = '0';
  fin = {f, c};
  if (mapa[f][c] != 'S')
    mapa[f][c] = 'E';
}