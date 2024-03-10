#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H
#include "comportamientos/comportamiento.hpp"
#include "queue"
#include "cmath"


using namespace std;

struct point{
	int fil;
	int col;

  

	point(int i,int j){
		fil = i;
		col = j;
	}
  point(){
    point(0,0);
  }
  void copy(const point &p){
    if(this != &p){
      point(p.fil,p.col);
    }
    
  }
  point(const point &p){
    copy(p);
  }
  int dis(point &a){
    return(max(abs(this->fil - a.fil),abs(this->col - a.col)));
  }
  bool operator ==(point &p);
	bool operator !=(point &p);
  point& operator =(const point &p);
  bool operator<(point &p);

};

bool point::operator<(point &p) const {
  if(this->fil < p.fil){
    return true;
  }else if (this->fil ==  p.fil){
    return this->col < p.col;
  }else{
    return false;
  }
}
bool point::operator ==(point &p){
  return (fil == p.fil) and (col == p.col);
}
bool point::operator !=(point &p){
  return !(*this==p);
}
point& point::operator =(const point &p){
  copy(p);
  return *this;
}


struct state{
    point p;
    Orientacion brujula;
};

struct rutina{
  Action accion;
  int prioridad;

  rutina(Action a, int pri){
    accion = a;
    prioridad = pri; 
  }
  bool operator<(const rutina  &otro) const;
};

bool rutina::operator<(const rutina  &otro) const {
  return prioridad < otro.prioridad;
}

struct casilla
{
  point p;
  unsigned char valor;
  int camino; 
  int valoracion;
  casilla* next;

  casilla(int x, int y){
    p = point(x,y);
    valor = '?';
    camino = -1;
    valoracion = -1;
    next = nullptr;
    
  }
  casilla(const point &q){
    p = q;
    valor = '?';
    camino = -1;
    valoracion = -1;
    next = nullptr;
    
  }

  void init(int x, int y){
    p = point(x,y);
    valor = '?';
    camino = 1000000;
    valoracion = -1;
    next = nullptr;
    
  }

  void mValor(unsigned char valor){
    this->valor = valor;
  }
  void mCamino(int i){
    camino = i;
  }
  bool operator< (const casilla& otro)const;
  /*
  bool operator< (const casilla& otro) const{
    if (desarrollar == true){
      if (desarrollar == otro.desarrollar){
          return (favorable>otro.favorable and otro.favorable != 200);
      }else{
        return false;
      }
    }else{
      if (desarrollar == otro.desarrollar){
        return (favorable>otro.favorable and otro.favorable != 200);
      }else{
        return true;
      }
    }
    
  } 
  */
  void setNext(casilla a){
    next = &a;
  }
  int dis(casilla a){
    return p.dis(a.p);
  }
  void CalculaValoracion (casilla a){
    valoracion = dis(a);
  }

};

bool casilla::operator< (const casilla& otro)const{
  return (this->p < otro.p);
}




class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado

      current_state.p = point(0,0);
      current_state.brujula = norte;
      last_action = actIDLE;
      bien_situado = true;
      contador = 0;
      algoritmo = 0;
      RellenarBorde(mapaResultado);
      for(int i = 0; i < mapaResultado.size(); i++){
        vector<casilla> v; 
        for (int j = 0; j < mapaResultado.size(); j++){
          v.push_back(casilla(i,j));
        }
        mapa_aux.push_back(v);
      }
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
      vector<vector<casilla>> mapa_aux; 
      unsigned int contador;
      priority_queue<rutina> prioridad;
      
};
#endif
