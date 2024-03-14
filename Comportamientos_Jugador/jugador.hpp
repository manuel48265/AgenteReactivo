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

  point(int i, int j, int size){
    int tam = size;
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

struct necesidad{
  bool bien_ubicado;
  bool zapatillas; 
  bool bikini; 
  bool bateria;
  bool atrapado_muros;
  bool mucho_precipicio;

  necesidad(){
    bien_ubicado = false;
    zapatillas = false; 
    bikini = false; 
    bateria = true;
    atrapado_muros = false;
    mucho_precipicio = false;
  }

  necesidad(const necesidad &n){
    if( this != &n){
      bien_ubicado = n.bien_ubicado ;
      zapatillas = n.zapatillas; 
      bikini = n.bikini; 
      bateria = n.bateria;
      atrapado_muros = n.atrapado_muros;
      mucho_precipicio = n.mucho_precipicio;
    }
    
  }

};

struct state{
    point p_real;
    Orientacion brujula_real;
    point p_virtual;
    Orientacion brujula_virtual;
    point target;
    necesidad condiciones; 

};



/*
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
  
  

};

*/




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
      current_state.condiciones = necesidad();

      last_action = actIDLE;
      RellenarBorde(mapaResultado);

      for(int i = 0; i < mapaResultado.size(); i++){
        vector<unsigned char> v; 
        for (int j = 0; j < mapaResultado.size(); j++){
          unsigned char c = '?'; 
          v.push_back(c);
        }
        mapa_aux.push_back(v);
      }

      last_action = actIDLE;
      tam = mapaResultado.size();
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    int CBateria(unsigned char v, bool zapatillas, bool bikini, bool correr, bool girar);
    int ValCasilla(unsigned char v, const state &st);
    Orientacion CalOrientacion(point a, point b);
    void PonerTerrenoEnMatriz(const vector<unsigned char> & terreno, state &st, vector<vector<unsigned char>> &matriz);
    void ActulizarMapa(vector<vector<unsigned char>> &mapa_virtual, vector<vector<unsigned char>> &mapa_real, const state &st);
    pair<point,int> CalculaValoracion (const state &st,const vector<vector<unsigned char>> &matriz, const point &p);
    void Actualizar(int &x,int &y, const Orientacion &brujula);
    Action CambiaDir(int init, int there);
    void Asignar(vector<vector<unsigned char>> &matriz, const point &p, unsigned char c);
    bool Igual(const vector<vector<unsigned char>> &matriz, const point &p, unsigned char c);
    unsigned char Dato(const vector<vector<unsigned char>> &matriz, const point &p);
    point PuntoOrientado(const state &st,Orientacion brujula);
    point BuscaPunto(const state &st);
    point Seguir(const state &st);

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
      vector<vector<unsigned char>> mapa_aux; 
      int tam;

      
};
#endif
