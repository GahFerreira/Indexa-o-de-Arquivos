CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic-errors -std=c++17

all: main

main: main.o
	$(CXX) $(CXXFLAGS) -o main.exe main.o

main.o: manipulador.o tituloNetflix.o geradorArquivos.o gerenciadorRegistros.o estatistica.o
	$(CXX) $(CXXFLAGS) main.cpp

manipulador.o:
	$(CXX) $(CXXFLAGS) Manipulador.cpp Manipulador.h

tituloNetflix.o:
	$(CXX) $(CXXFLAGS) TituloNetflix.cpp TituloNetflix.h

geradorArquivos.o:
	$(CXX) $(CXXFLAGS) GeradorArquivos.cpp GeradorArquivos.h

gerenciadorRegistros.o:
	$(CXX) $(CXXFLAGS) GerenciadorRegistros.cpp GerenciadorRegistros.h

estatistica.o:
	$(CXX) $(CXXFLAGS) Estatistica.cpp Estatistica.h