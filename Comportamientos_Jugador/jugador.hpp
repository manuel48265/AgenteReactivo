#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H
#include "comportamientos/comportamiento.hpp"
using namespace std;

struct state{
    int fil; 
    int col;
    Orientacion brujula;
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
      rotar = true;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);

  private:
      state current_state;
      Action last_action;
      bool bien_situado;
      bool rotar;
      vector<casilla> casillas_importantes; 
};
#endif
