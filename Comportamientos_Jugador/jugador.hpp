#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H
#include "comportamientos/comportamiento.hpp"
#include "queue"
#include "cmath"
#include "iostream"


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

struct necesidad{
  bool bien_ubicado;
  bool zapatillas; 
  bool bikini; 
  bool bateria;
  bool atrapado_muros;
  bool investiga;
  bool salida_muros;
  bool rebasar;

  necesidad(){
    bien_ubicado = false;
    zapatillas = false; 
    bikini = false; 
    bateria = true;
    atrapado_muros = false;
    investiga = false;
    salida_muros = false; 
    rebasar = false;
  }

  necesidad(const necesidad &n){
    if( this != &n){
      bien_ubicado = n.bien_ubicado ;
      zapatillas = n.zapatillas; 
      bikini = n.bikini; 
      bateria = n.bateria;
      atrapado_muros = n.atrapado_muros;
      investiga = n.investiga;
      rebasar = n.rebasar;
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
    int way;

    state(){
    p_real = point(0,0);
    brujula_real = norte;
    p_virtual = point(0,0);
    brujula_virtual = norte;
    target = point(-1,-1);
    condiciones = necesidad(); 
    way = -1; 
    }

};

struct Importantes{
  int dis_origen;
  int bateria; 
  int hijos; 
  int huida;
  

  Importantes(){
    dis_origen = -1;
    bateria = -1;
    hijos = 0;
    huida = -1;
  }

  Importantes& operator=(const Importantes &rhs );

};


class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      
      init();
    
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    int CBateria(unsigned char v, bool correr,bool girar);
    int ValCasilla(unsigned char v, const state &st);
    Orientacion CalOrientacion(point a, point b);
    void PonerTerrenoEnMatriz(const vector<unsigned char> & terreno, state &st, vector<vector<unsigned char>> &matriz, bool act_cola);
    void ActulizarMapa(vector<vector<unsigned char>> &mapa_virtual, vector<vector<unsigned char>> &mapa_real, const state &st);
    pair<point,int> CalculaValoracion (const state &st,const vector<vector<unsigned char>> &matriz, const point &p);
    void Actualizar(int &x,int &y, const Orientacion &brujula, int k);
    Action CambiaDir(int init, int there);
    void Asignar(vector<vector<unsigned char>> &matriz, const point &p, unsigned char c);
    bool Igual(const vector<vector<unsigned char>> &matriz, const point &p, unsigned char c);
    unsigned char Dato(const vector<vector<unsigned char>> &matriz, const point &p);
    point PuntoOrientado(const state &st,Orientacion brujula);
    point BuscaPunto(const state &st);
    point Seguir(const state &st);
    Action Movimiento(const vector<unsigned char> &v, const state &st, unsigned char c);
    bool CasillaPosible(int x, int y, const vector<vector<unsigned char>> &mapa);
    Orientacion Investiga(const vector<vector<unsigned char>> &mapa, const point &casilla, unsigned char c);
    Orientacion PorExplorar(const vector<vector<unsigned char>> &mapa, unsigned char c);
    void CambiarHijos(const point &p);
    void AsignarRecorridos(vector<vector<Importantes>> &matriz, const point &p, int cual, int valor);
    void CambiarDisOrigen(const point &p);
    void CambiarBateria(const point &p);
    void ActualizacionGlobal();
    point CalculaPunto(const point &p, Orientacion org, int pos);
    void Actualizar(point &p, const Orientacion &brujula, int k);
    Action Andar_Correr(const vector<unsigned char> &agentes);
    point Mas_Cercana();
    void CambiarHuida (const point &p);
    int CosteManiobra(int i);
    int CosteMedioCasilla( unsigned char c);
    int Explorara(const point &p, Orientacion ori, const Sensores &sen );
    void ExtraeTerreno(const point &p, Orientacion ori, const Sensores &sen, vector<unsigned char> &v);
    point casillaHuida();
    double ExploraHuida(point origen, point destino);
    void ReseteoHuida();

    

    void RellenarBorde(vector<vector<unsigned char>> &matriz){
      for(int i = 0; i < matriz.size(); i++){
        for (int j = 0; j<3; j++){
          matriz.at(i).at(j) = matriz.at(j).at(i) = matriz.at(matriz.size() -1 - j).at(i) = matriz.at(i).at(matriz.size() -1 - j) ='P';
        }
      }
    }

    void init(){
      current_state = state();
      last_action = actIDLE;
      tam = mapaResultado.size();

      RellenarBorde(mapaResultado);

      mapa_aux = vector<vector<unsigned char>>(); 
      frecuencias = vector<vector<int>>();


      vector<unsigned char> va; 
      vector<int> vi;

      for (int j = 0; j < mapaResultado.size(); j++){
        unsigned char c = '?'; 
        va.push_back(c);
        vi.push_back(0);
      }

      for(int i = 0; i < mapaResultado.size(); i++){
        mapa_aux.push_back(va);
        frecuencias.push_back(vi);
      }
      mapa_recorrido = vector<vector<Importantes>>();

      vector<Importantes> v; 

      for (int j = 0; j < mapaResultado.size(); j++){
          Importantes c = Importantes();
          c.hijos = pow((2*intervalo+1),2);
          v.push_back(c);
        }

      for(int i = 0; i < mapaResultado.size(); i++){
        mapa_recorrido.push_back(v);
      }

      mapa_recorrido.at(0).at(0).dis_origen = 0;
      Por_actualizar = vector<queue<point>>();
      for(int i = 0; i < 4; i++){
        queue<point>cola; 
        Por_actualizar.push_back(cola);
      }

      cont_bateria = 0;

      huida = false;
      origen_huida = point(-1,-1);

      no_agentes = vector<bool>();

      for(int i = 0; i < 16; i++){
        no_agentes.push_back(true);
      }

      accion_vacia = 0; 
      mapa_100 = false; 
      
    }





  private:
      state current_state;
      Action last_action;
      vector<vector<unsigned char>> mapa_aux; 
      int tam;
      /*
        0.Bateria
        1.dis_origen
        2.Hijos
        3.Huida
      
      */
      vector<queue<point>> Por_actualizar; 
      vector<vector<Importantes>> mapa_recorrido; 
      int cont_bateria; 
      static const int intervalo = 3; 
      bool huida ;
      point origen_huida;
      vector<vector<int>> frecuencias;
      vector<bool> no_agentes;
      int accion_vacia; 
      bool mapa_100; 



      
};
#endif
