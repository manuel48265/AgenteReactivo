#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H
#include "comportamientos/comportamiento.hpp"
#include "queue"
#include "cmath"


using namespace std;

int CBateria(unsigned char v, bool zapatillas, bool bikini, bool correr, bool girar);
int ValCasilla(unsigned char v, bool zapatillas, bool bikini);

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

  int dis(point &a, int tam){
    return(max(abs((this->fil - a.fil + tam)%tam),abs((this->col - a.col + tam)%tam));
  }

  bool operator ==(point &p);
	bool operator !=(point &p);
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
  int generado;
  casilla* padre;

  casilla(int x, int y){
    p = point(x,y);
    valor = '?';
    valoracion = -1;
    padre = nullptr;
    
  }
  casilla(const point &q){
    p = q;
    valor = '?';
    valoracion = -1;
    padre = nullptr;
    
  }

  void init(int x, int y){
    p = point(x,y);
    valor = '?';
    valoracion = -1;
    padre = nullptr;
    
  }

  void mValor(unsigned char valor){
    this->valor = valor;
  }
  bool operator< (const casilla& otro)const;
  bool comparaPunteros (const casilla*& uno, const casilla*& otro);
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
  void setPadre(casilla a){
    padre = &a;
  }
  int dis(casilla a){
    return p.dis(a.p);
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
  
  
  void CalculaValoracion (const state &st,const vector<vector<casillas>> &matriz){
    int intervalo = -4;
    int sum = 0;
    int tam = matriz.size();

    if(matriz.at(st.p_virtual).at(st.p_virtual) == 'M' or matriz.at(st.p_virtual).at(st.p_virtual) == 'P'){
      sum = -5000;
    }else{
      for(int i = -intervalo; i <= intervalo; i++){
        for (int j = -intervalo; j <= intervalo; j++){
          if(matriz.at(st.p_virtual +i).at(st.p_virtual+j) == '?'){
            
            double val = ValCasilla(matriz.at((st.p_virtual.fil + i + tam)%tam).at((st.p_virtual.col + j + tam)%tam),st.condiciones);

            if (val > 0){
              sum += val*pow((point((st.p_virtual.fil + i + tam)%tam,(st.p_virtual.col + j + tam)%tam).dis(st.target)),2);
            }else{
              sum += val/pow((point((st.p_virtual.fil + i + tam)%tam,(st.p_virtual.col + j + tam)%tam).dis(st.target)),2);
            }
          }
        }
      }
    }
    valoracion = sum;
  }

};




class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado

      current_state.p_virtual = point(0,0);
      current_state.brujula_virtual = norte;
      current_state.p_real = point(0,0);
      current_state.brujula_real = norte;
      current_state.target = point(5,5);
      for(int i = 0; i <= 4; i++){
        current_state.condiciones.at(i) = false; 
      }

      last_action = actIDLE;
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
      vector<vector<casilla>> mapa_aux; 
      priority_queue<rutina> prioridad;
      
};
#endif
