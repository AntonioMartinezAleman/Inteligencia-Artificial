# Compilador y flags
CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Wpedantic -MMD -MP

# Binario final
TARGET := busqueda

# Fuentes y objetos
SRC := main.cpp laberinto.cpp astar.cpp
OBJ := $(SRC:.cpp=.o)
DEP := $(OBJ:.o=.d)

# Regla por defecto
all: $(TARGET)

# Enlace
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@

# Compilación (con dependencias .d)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ejecutar con ejemplo de archivos (ajusta a los tuyos)
run: $(TARGET)
	./$(TARGET) input.txt salida.txt

# Limpiar
clean:
	rm -f $(OBJ) $(DEP) $(TARGET)

-include $(DEP)

.PHONY: all run clean