#include "laberinto.h"
#include "astar.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sstream>

static void imprimir_tabla_astar(std::ostream& out,
                                 const std::string& instancia,
                                 const laberinto& lab,
                                 const AStarResult& R) {
  auto S = lab.getInicio();
  auto E = lab.getFin();

  out << std::left
      << std::setw(12) << "Instancia"
      << std::setw(5)  << "n"
      << std::setw(5)  << "m"
      << std::setw(8)  << "S"
      << std::setw(8)  << "E"
      << std::setw(10) << "Camino"
      << std::setw(8)  << "Coste"
      << std::setw(24) << "Nodos generados"
      << std::setw(26) << "Nodos inspeccionados"
      << "\n";

  out << std::string(12+5+5+8+8+10+8+24+26, '-') << "\n";

  std::ostringstream sstr, estr;
  sstr << "(" << S.first << "," << S.second << ")";
  estr << "(" << E.first << "," << E.second << ")";

  out << std::left
      << std::setw(12) << instancia
      << std::setw(5)  << lab.filas()
      << std::setw(5)  << lab.cols()
      << std::setw(8)  << sstr.str()
      << std::setw(8)  << estr.str()
      << std::setw(10) << (R.ok ? "Si" : "No")
      << std::setw(8)  << (R.ok ? std::to_string(R.cost) : "-")
      << std::setw(24) << R.generados
      << std::setw(26) << R.inspeccionados
      << "\n";
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Uso: " << argv[0] << " <fichero_laberinto> <fichero_salida>\n";
    return 1;
  }

  std::ifstream entrada(argv[1]);
  if (!entrada.is_open()) {
    std::cerr << "Error: no se pudo abrir el fichero de entrada: " << argv[1] << "\n";
    return 1;
  }

  std::ofstream salida(argv[2]);
  if (!salida.is_open()) {
    std::cerr << "Error: no se pudo abrir el fichero de salida: " << argv[2] << "\n";
    return 1;
  }

  const std::string instancia = argv[1];
  laberinto lab(entrada);

  // Helper: volcar el laberinto a un ostream (sin usar print()).
  auto volcar_laberinto = [](std::ostream& out, const laberinto& L) {
    for (int i = 0; i < L.filas(); ++i) {
      for (int j = 0; j < L.cols(); ++j) {
        out << L.celda(i, j);
        if (j + 1 < L.cols()) out << ' ';
      }
      out << '\n';
    }
  };

  // --- ÚNICA interacción por terminal: permitir cambiar S/E ---
  char resp;
  std::cout << "¿Quieres cambiar la entrada S? (s/n): ";
  std::cin >> resp;
  if (resp == 's' || resp == 'S') {
    int fs, cs;
    std::cout << "Introduce fila y columna para S (0-index): ";
    std::cin >> fs >> cs;
    if (!lab.inBounds(fs, cs)) {
      salida << "Aviso: S fuera de rango (" << fs << "," << cs << "). Se mantiene S original.\n";
    } else {
      lab.cambiar_S(fs, cs);
      salida << "S actualizada a (" << fs << "," << cs << ").\n";
    }
  }

  std::cout << "¿Quieres cambiar la salida E? (s/n): ";
  std::cin >> resp;
  if (resp == 's' || resp == 'S') {
    int fe, ce;
    std::cout << "Introduce fila y columna para E (0-index): ";
    std::cin >> fe >> ce;
    if (!lab.inBounds(fe, ce)) {
      salida << "Aviso: E fuera de rango (" << fe << "," << ce << "). Se mantiene E original.\n";
    } else {
      lab.cambiar_E(fe, ce);
      salida << "E actualizada a (" << fe << "," << ce << ").\n";
    }
  }
  salida << "\n";

  // ---- Estado inicial al fichero ----
  salida << "Laberinto inicial (tras posibles cambios de S/E):\n";
  volcar_laberinto(salida, lab);
  salida << "\n";

  // --------- MODO DINÁMICO (toda la salida al fichero) ---------
  salida << "Inicio de la simulacion (dinamica):\n";
  volcar_laberinto(salida, lab);
  salida << "\n";

  auto objetivo = lab.getFin();
  auto pos = lab.getInicio();

  int pasos_totales = 0;
  long long generados_tot = 0;
  long long inspeccionados_tot = 0;
  long long coste_total = 0;

  while (pos != objetivo) {
    // Plan A* sobre el estado actual
    AStarResult R = run_a_star(lab);

    // Reintentos si no hay plan
    if (!R.ok) {
      bool encontrado = false;
      for (int t = 0; t < 5; ++t) {
        lab.actualizar_mapa(0.5, 0.5);
        lab.forzar_tope_obstaculos(0.25);
        R = run_a_star(lab);
        if (R.ok) { encontrado = true; break; }
      }
      if (!encontrado) {
        salida << "Objetivo inalcanzable tras reintentos.\n\n";

        AStarResult R_final;
        R_final.ok = false;
        R_final.cost = static_cast<int>(coste_total);
        R_final.generados = static_cast<int>(generados_tot);
        R_final.inspeccionados = static_cast<int>(inspeccionados_tot);

        imprimir_tabla_astar(salida, instancia, lab, R_final);
        salida << "\nNO\n";
        volcar_laberinto(salida, lab);
        return 0;
      }
    }

    if (R.path.size() < 2) break; // nada que mover

    // ----- ANTES DE MOVER: imprimir la SOLUCIÓN A* ESTÁTICA -----
    {
      laberinto lab_vis = lab;
      for (size_t i = 1; i + 1 < R.path.size(); ++i)
        lab_vis.marcarCamino(R.path[i].first, R.path[i].second);

      int obs = lab.contar_obstaculos();
      int total_celdas = lab.filas() * lab.cols();
      double ratio = static_cast<double>(obs) / static_cast<double>(total_celdas);

      salida << "--- A* resuelto previo al paso " << (pasos_totales + 1) << " ---\n";
      volcar_laberinto(salida, lab_vis);
      salida << "(CostePlan=" << R.cost
             << ", Generados=" << R.generados
             << ", Inspeccionados=" << R.inspeccionados
             << ", LongitudPath=" << R.path.size()
             << ", Ratio_obstaculos=" << std::fixed << std::setprecision(3) << ratio
             << ")\n\n";
    }

    // Siguiente paso del plan
    auto siguiente = R.path[1];
    int df = std::abs(siguiente.first - pos.first);
    int dc = std::abs(siguiente.second - pos.second);
    int coste_paso = (df == 1 && dc == 1) ? 7 : 5;

    // Aplicar movimiento
    coste_total += coste_paso;
    lab.marcarCamino(siguiente.first, siguiente.second);
    lab.mover_agente_a(siguiente.first, siguiente.second);
    pos = siguiente;

    pasos_totales++;
    generados_tot += R.generados;
    inspeccionados_tot += R.inspeccionados;

    // Estadísticas tras mover
    int obs2 = lab.contar_obstaculos();
    int total2 = lab.filas() * lab.cols();
    double ratio2 = static_cast<double>(obs2) / static_cast<double>(total2);

    salida << "--- Paso " << pasos_totales
           << " (+" << coste_paso
           << ", total_coste=" << coste_total
           << ", generados_acum=" << generados_tot
           << ", inspeccionados_acum=" << inspeccionados_tot
           << ", ratio_obstaculos=" << std::fixed << std::setprecision(3) << ratio2
           << ") ---\n";
    volcar_laberinto(salida, lab);
    salida << "\n";

    // Dinámica del entorno
    lab.actualizar_mapa(0.5, 0.5);
    lab.forzar_tope_obstaculos(0.25);
  }

  // --------- FIN SIMULACIÓN ---------
  salida << "Fin de la simulacion.\n";
  salida << "Pasos " << pasos_totales << "\n";
  salida << "Generados " << generados_tot << "\n";
  salida << "Inspeccionados " << inspeccionados_tot << "\n";
  salida << "Coste_total " << coste_total << "\n\n";

  AStarResult R_final;
  R_final.ok = (pos == objetivo);
  R_final.cost = static_cast<int>(coste_total);
  R_final.generados = static_cast<int>(generados_tot);
  R_final.inspeccionados = static_cast<int>(inspeccionados_tot);

  imprimir_tabla_astar(salida, instancia, lab, R_final);
  salida << "\n" << (pos == objetivo ? "OK\n" : "NO\n");

  // Volcado final del laberinto
  volcar_laberinto(salida, lab);

  return 0;
}
