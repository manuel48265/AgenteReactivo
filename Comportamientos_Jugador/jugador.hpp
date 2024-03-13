#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H
#include "comportamientos/comportamiento.hpp"
#include "queue"
#include "cmath"
#include "iostream"
#include <chrono>
#include <thread>


using namespace std;

int CBateria(unsigned char v, bool zapatillas, bool bikini, bool correr, bool girar);
int ValCasilla(unsigned char v, const vector<bool> &condiciones);

struct point{
	int fil;
	int col;

	point(int i,int j){
		this->fil = i;
		this->col = j;
	}

  point(){
    point(0,0);
  }


  point(const point &p){
    if(this != &p){
      this->fil = p.fil;
      this->col = p.col;
    }
  }

  int dis(const point &a, int tam)const {
    int x;
    int y;
    if(abs(this->fil - a.fil) < tam/2){
      x = abs(this->fil - a.fil);
    }else{
      x = abs(this->fil - a.fil) - tam;
    }

    if(abs(this->col - a.col) < tam/2){
      y = this->col - a.col;
    }else{
      y = abs(this->col - a.col) - tam;
    }
    return max(abs(x),abs(y));
  }

  string to_s(){
    return (to_string(fil) + "," + to_string(col));
  }

  bool operator ==(const point &p) const ;
	bool operator !=(const point &p) const ;
  point& operator =(const point &p);
  bool operator<(const point &p)const ;

};

struct state{
    point p_real;
    Orientacion brujula_real;
    point p_virtual;
    Orientacion brujula_virtual;
    point target;
    vector<bool> condiciones;
    int tam; 
    int suma;

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



struct casilla
{
  point p;
  unsigned char valor;
  int valoracion;
  int pasos;

  casilla(){
    p = point(0,0);
    valor = '?';
    valoracion = -1;
    pasos = 0;
  }

  casilla(int x, int y){
    p = point(x,y);
    valor = '?';
    valoracion = -1;
    pasos = 0;
    
  }
  casilla(const casilla &q){

    if(this != &q){
      this->p = q.p;
      this->valoracion = q.valoracion;
      this->pasos = q.pasos;
      this->valor = q.valor;
    }

  }

  void init(int x, int y){
    p = point(x,y);
    valor = '?';
    valoracion = -1;
    pasos = 0;
    
  }

  void mValor(unsigned char valor){
    this->valor = valor;
  }
  bool operator< (const casilla& otro)const;
  bool comparaPunteros (const casilla*& uno, const casilla*& otro);
  casilla& operator= (const casilla& otro);
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
  int dis(casilla a,int tam){
    return p.dis(a.p,tam);
  }
  /*
  Condiciones:
    0. Nos indica si el usuario está ubicado
      Da prioridad a buscar una casilla tipo 'G'
    1. Nos indica si tenemos o no las zapatillas
      Da prioridad a la búsqueda de una casilla 'D'
      Modifica la toma de decisiones, pues correr puede gastar menos batería.
    2. Nos indica si tenemos o no el Bikini 
      Da prioridad a la búsqueda de una casilla 'K'
      Modifica la toma de decisiones, pues correr puede gastar menos batería.
    3.Batería un bool que nos indica si queda batería (susceptible de ser cambiado)
      Perder por culpa de la batería puede ser un gran problema.
      Tomará acciones más conservadoras para evitar gastar batería.
  */
  

};




class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      point p = point(0,0);
      current_state.p_virtual = p;
      current_state.brujula_virtual = norte;
      current_state.p_real = p;
      current_state.brujula_real = norte;
      current_state.target = point(5,5);
      for(int i = 0; i < 4; i++){
        current_state.condiciones.push_back(false); 
      }

      last_action = actIDLE;
      RellenarBorde(mapaResultado);

      for(int i = 0; i < mapaResultado.size(); i++){
        vector<casilla> v; 
        for (int j = 0; j < mapaResultado.size(); j++){
          casilla c = casilla(i,j) ; 
          v.push_back(c);
        }
        mapa_aux.push_back(v);
      }

      last_action = actIDLE;
      current_state.tam = mapaResultado.size();
      current_state.suma = 0;
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
      vector<vector<casilla>> mapa_aux; 
      priority_queue<rutina> prioridad;
      
};
#endif
