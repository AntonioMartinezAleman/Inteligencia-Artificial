#ifndef NODO_H_
#define NODO_H_

#include <utility>
#include <cstddef>

class Nodo {
public:
  int fila{0}, col{0};
  int g{0};
  int h{0};
  int pf{-1}, pc{-1};

  Nodo() = default;
  Nodo(int f, int c, int g_, int h_, int padre_f = -1, int padre_c = -1)
    : fila(f), col(c), g(g_), h(h_), pf(padre_f), pc(padre_c) {}

  inline int f() const { return g + h; }

  bool operator==(const Nodo& other) const {
    return fila == other.fila && col == other.col;
  }
};

struct CmpNodo {
  bool operator()(const Nodo& a, const Nodo& b) const {
    if (a.f() != b.f()) return a.f() > b.f();
    return a.h > b.h;
  }
};


#endif