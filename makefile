CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -c

all: main.o Estatistica.o GeradorArquivos.o GerenciadorRegistros.o Manipulador.o TituloNetflix.o
	g++ main.o Estatistica.o GeradorArquivos.o GerenciadorRegistros.o Manipulador.o TituloNetflix.o -o ProjetoPratico.exe

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp

Estatistica.o: Estatistica.cpp Estatistica.h
	$(CXX) $(CXXFLAGS) Estatistica.cpp

GeradorArquivos.o: GeradorArquivos.cpp GeradorArquivos.h
	$(CXX) $(CXXFLAGS) GeradorArquivos.cpp

GerenciadorRegistros.o: GerenciadorRegistros.cpp GerenciadorRegistros.h
	$(CXX) $(CXXFLAGS) GerenciadorRegistros.cpp

Manipulador.o: Manipulador.cpp Manipulador.h
	$(CXX) $(CXXFLAGS) Manipulador.cpp

TituloNetflix.o: TituloNetflix.cpp TituloNetflix.h
	$(CXX) $(CXXFLAGS) TituloNetflix.cpp