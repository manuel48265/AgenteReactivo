#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H
#include "comportamientos/comportamiento.hpp"
#include "queue"


using namespace std;

struct state{
    int fil; 
    int col;
    Orientacion brujula;
};

struct rutina{
  Action accion;
  int prioridad;

  rutina(Action a, int pri){
    accion = a;
    prioridad = pri; 
  }

  bool operator<(const rutina  &otro) const {
      return prioridad < otro.prioridad;
  }

};

struct casilla
{
  int fil;
  int col; 
  unsigned char valor;

  bool operator<(casilla otro){
    if(this->valor <= otro.valor){
      return true;
    }
    else{
      return false;
    }
  }
};


class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado

      current_state.fil = current_state.col = 97;
      current_state.brujula = norte;
      last_action = actIDLE;
      bien_situado = true;
      contador = 0;
      algoritmo = 0;
      RellenarBorde(mapaResultado);
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);

    void RellenarBorde(vector<vector<unsigned char>> &matriz){
    for(int i = 0; i < matriz.size(); i++){
      for (int j = 0; j<3; j++){
        matriz.at(i).at(j) = matriz.at(j).at(i) = matriz.at(matriz.size() -1 - j).at(i) = matriz.at(i).at(matriz.size() -1 - j) ='P';
      }
    }
}

  private:
      state current_state;
      Action last_action;
      bool bien_situado;
      bool derecha;
      unsigned int algoritmo;
      vector<casilla> casillas_importantes; 
      unsigned int contador;
      priority_queue<rutina> prioridad;
};
#endif
