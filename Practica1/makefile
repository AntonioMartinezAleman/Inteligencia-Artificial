CXX=g++
CXXFLAGS=-std=c++17 -O2 -Wall -Wextra -Wpedantic

SRC=main.cpp grafo.cpp
OBJ=$(SRC:.cpp=.o)
BIN=practica

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(OBJ) -o $@

%.o: %.cpp grafo.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(BIN)
	printf "%s\n%s\n%s\n%s\n%s\n" "$(IN)" "$(OUT)" "$(ORIGEN)" "$(DESTINO)" "$(ALG)" | ./$(BIN)

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all run clean
