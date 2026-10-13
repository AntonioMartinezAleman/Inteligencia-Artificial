# 🤖 Inteligencia Artificial

![C++](https://img.shields.io/badge/C++17-00599C?logo=cplusplus&logoColor=white)
![Prolog](https://img.shields.io/badge/Prolog-E61B23)
![Jupyter](https://img.shields.io/badge/Jupyter-F37626?logo=jupyter&logoColor=white)
![scikit-learn](https://img.shields.io/badge/scikit--learn-F7931E?logo=scikitlearn&logoColor=white)

Prácticas de la asignatura de Inteligencia Artificial: algoritmos de búsqueda en C++, programación lógica en Prolog y aprendizaje no supervisado en Python.

## Prácticas

| # | Tema | Descripción |
|---|------|-------------|
| [P1](p1-busqueda-no-informada) | Búsqueda no informada | Búsqueda en **amplitud (BFS)** y **profundidad (DFS)** sobre grafos ponderados que se leen de fichero. Muestra en cada iteración los nodos generados y los inspeccionados, el camino y su coste. |
| [P2](p2-busqueda-a-estrella) | Búsqueda heurística | Algoritmo **A\*** para encontrar la salida de un laberinto, con heurística de distancia Manhattan. |
| [P3](p3-prolog) | Programación lógica | Ejercicios de **Prolog**: listas, recursión, corte, árboles genealógicos y representación del conocimiento. |
| [P4](p4-clustering-kmeans) | Aprendizaje no supervisado | Notebook de **clustering con k-means** sobre el dataset Iris: PCA, métricas de evaluación y elección de *k* por el método del codo y el coeficiente de silueta. |

## Compilación y ejecución

```bash
# P1: búsqueda en grafos
cd p1-busqueda-no-informada && make
./busqueda grafos/Grafo1.txt salida.txt

# P2: A* en laberinto
cd p2-busqueda-a-estrella && make

# P3: Prolog (SWI-Prolog)
swipl p3-prolog/ejemplos-prolog/familia.prolog

# P4: notebook
jupyter notebook p4-clustering-kmeans/clustering_iris.ipynb
```

---

Grado en Ingeniería Informática · Universidad de La Laguna · Antonio Martínez Alemán
