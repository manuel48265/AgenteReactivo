#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include<queue>
#include<math.h>
#include<set>
#include<tgmath.h>
#include<list>
#include<stack>
#include <utility>
#include <chrono>
#include <thread>

using namespace std;


struct myComp {
    constexpr bool operator()(
        pair<point , int> const& a,
        pair<point , int> const& b)
        const noexcept
    {
        return a.second < b.second;
    }
};

void ComportamientoJugador::Actualizar(int &x,int &y, const Orientacion &brujula){

	switch (brujula)
		{
		case norte:
			x--;
			break;
		case noreste:
			x--;
			y++;
			break;	
		case este:
			y++;
			break;
		case sureste:
			x++;
			y++;
			break;
		case sur:
			x++;
			break;
		case suroeste:
			x++;
			y--;
			break;
		case oeste:
			y--;
			break;
		case noroeste:
			x--;
			y--;
			break;
		default:
			break;
		}

		x = (x + 3*tam)%tam;
		y = (y + 3*tam)%tam; 

}

Action ComportamientoJugador::CambiaDir(int init, int there){
	if(init == there){
		return actIDLE;
	}
	else if((there-init+8)%8 <= 3){
		return actTURN_SR;
	}
	else{
		return actTURN_L;
	}
}


void ComportamientoJugador::Asignar(vector<vector<unsigned char>> &matriz, const point &p, unsigned char c){
	matriz.at((p.fil + 3*tam)%tam).at((p.col+ 3*tam)%tam) = c;
}

bool ComportamientoJugador::Igual(const vector<vector<unsigned char>> &matriz, const point &p, unsigned char c){
	return (matriz.at((p.fil + 3*tam)%tam).at((p.col+ 3*tam)%tam) == c);
}

unsigned char ComportamientoJugador::Dato(const vector<vector<unsigned char>> &matriz, const point &p){
	return (matriz.at((p.fil+ 3*tam)%tam).at((p.col+ 3*tam)%tam));
}

point ComportamientoJugador::PuntoOrientado(const state &st,Orientacion brujula){
	point p = st.p_virtual;
	Actualizar(p.fil,p.col,brujula);
	return (p);
}

point ComportamientoJugador::BuscaPunto(const state &st){
	int random = rand()%7;
	random++;
	point p = st.p_virtual;
	for(int i = 0 ; i < 4; i++){
		Actualizar(p.fil,p.col,static_cast<Orientacion>((st.brujula_virtual + random)%8));
	}

	return(p);
}

point ComportamientoJugador::Seguir(const state &st){
	point p = st.p_virtual;
	for(int i = 0 ; i < 4; i++){
		Actualizar(p.fil,p.col,st.brujula_virtual);
	}

	return(p);
}



/*
casilla& casilla::operator= (const casilla& otro){
	if(this != &otro){
		this->p = otro.p;
		this->valoracion = otro.valoracion;
		this->pasos = otro.pasos;
		this->valor = otro.valor;
	}
	return(*this);
}
*/

int ComportamientoJugador::CBateria(unsigned char v, bool zapatillas, bool bikini, bool correr, bool girar){
  int coste;
  switch (v)
  {
  case 'A':
    if(correr){
		if(bikini){
			coste = 10;
		}else{
			coste = 500;
		}
	}else if(girar) {
		if(bikini){
			coste = 5;
		}else{
			coste = 50;
		}
	}else{
		if(bikini){
			coste = 5;
		}else{
			coste = 50;
		}
	}
    break;
  
  case 'B':
    if(correr){
		if(zapatillas){
			coste = 15;
		}else{
			coste = 250;
		}
	}else if(girar) {
		if(zapatillas){
			coste = 1;
		}else{
			coste = 10;
		}
	}else{
		if(zapatillas){
			coste = 5;
		}else{
			coste = 25;
		}
	}
    break;

  case 'T':
    if(correr){
		coste = 3;
	}else if(girar) {
		coste = 2;
	}else{
		coste = 2;
	}
    break;

  default:
  coste = 1;
    break;
  }

  return coste;

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

int ComportamientoJugador::ValCasilla(unsigned char v, const state &st){
	int coste;
  switch (v)
  {
  	case 'A':
		if(st.condiciones.bikini){
			coste = 460;
		}else{
			coste = 25;
		}
    break;
  
  	case 'B':
    
		if(st.condiciones.zapatillas){
			coste = 460;
		}else{
			coste = 50;
		}
	
    break;

  	case 'T':
		coste = 500;
    break;

	case 'M':
	case 'P':
		coste = -500;
    break;

	case '?':
	coste = 1500;

	case 'X': 
	if(st.condiciones.bateria){
		coste = 200;
	}else{
		coste = 10000;
	}

	case 'D':
	if(st.condiciones.zapatillas){
		coste = 600;
	}else{
		coste = 10000;
	}
	case 'K':
	if(st.condiciones.bikini){
		coste = 600;
	}else{
		coste = 10000;
	}
	case 'G':
	if(st.condiciones.bien_ubicado){
		coste = 600;
	}else{
		coste = 10000;
	}
	case 'S':
	coste = 600;
	case 'a':
	coste = -200;
	case 'l':
	coste = -300000;
	default:
	coste = 3;
	break;


  }

  return coste;

}

bool point::operator<(const point &p) const {
  if(this->fil < p.fil){
    return true;
  }else if (this->fil ==  p.fil){
    return this->col < p.col;
  }else{
    return false;
  }
}
bool point::operator ==(const point &p)const{
  return ((fil == p.fil) and (col == p.col));
}
bool point::operator !=(const point &p)const{
  return !(*this==p);
}
point& point::operator =(const point &p){
  if(this!=&p){ 
        this->fil = p.fil;
		this->col = p.col;
    }
  return *this;
}

/*

bool casilla::operator< (const casilla& otro)const{
  return (this->p < otro.p);
}

bool casilla::comparaPunteros (const casilla*& uno, const casilla*& otro){
    return ((*uno) < (*otro));
}

int Ajuste(int i, int adjust){
	if(i >= 0 and i <= adjust){
		return i;
	}else if(i > adjust){
		return adjust;
	}else {
		return 0;
	}
}

/*

void CalculoCasilla(int i,const state &st, const vector<vector<unsigned char>> &matriz, bool orden, point &p, bool &no_vacio){
	cout << "aqui6" << endl;
	int j = i;
	if (orden){
		while(no_vacio and j <= -i){
			if(matriz.at(Ajuste(st.p_virtual.fil+i,matriz.size()-1)).at(Ajuste(st.p_virtual.col + j,matriz.size()-1)) == '?'){
				no_vacio = false; 
				p.fil = Ajuste(st.p_virtual.fil+i,matriz.size()-1);
				p.col = Ajuste(st.p_virtual.col + j,matriz.size()-1);

			}
			j++;
			cout << j << endl;
		}
	}else{
		
		while(no_vacio and j <= -i){
			if(matriz.at(Ajuste(st.p_virtual.fil + j,matriz.size()-1)).at(Ajuste(st.p_virtual.col + i,matriz.size()-1)) == '?'){
				no_vacio = false; 
				p.fil = Ajuste(st.p_virtual.fil + j,matriz.size()-1);
				p.col = Ajuste(st.p_virtual.col + i,matriz.size()-1);

			}
			j++;
			cout << j << endl;
		}
	}
	
}

*/



/*
point ExploracionLocal(const state &st, const vector<vector<unsigned char>> &matriz){
	cout << "aqui3" << endl;
	int i = -4;

	bool no_vacio = true;

	point p = point(st.p_virtual.fil, st.p_virtual.col);
	while(no_vacio and i > -8){
		CalculoCasilla(-i,st,matriz, 0, p, no_vacio);
		if(no_vacio){
			CalculoCasilla(-i,st,matriz, 1, p, no_vacio);
			if(no_vacio){
				CalculoCasilla(i,st,matriz, 0, p, no_vacio);
				if(no_vacio){
					CalculoCasilla(i,st,matriz, 1, p, no_vacio);
				}
			}
		}
		i--;
	}

	return p;
}
*/




Orientacion ComportamientoJugador::CalOrientacion(point a, point b){

	int x = (b.col + (- a.col + 3*tam))%tam;
	int y = (b.fil + (- a.fil + 3*tam))%tam;

	Orientacion tmp; 

	
	if(y <=2 and y > 0){
		if (x == 0){
			tmp = sur;
		}else if(x <= 2){
			tmp = sureste;
		}else{
			tmp = suroeste;
		}
	}else if(y == 0){
		if (x == 0){
			tmp = este;
		}else if(x <= 2){
			tmp = este;
		}else{
			tmp = oeste;
		}
	}else{
		if (x == 0){
			tmp = norte;
		}else if(x <= 2){
			tmp = noreste;
		}else{
			tmp = noroeste;
		}
	}
	



	/*

	if(y = 0){
		if (x > 0){
			tmp = Orientacion::este;
		}else{
			tmp = Orientacion::oeste;
		}
	}
	else if(x = 0){
		if (y > 0){
			tmp = Orientacion::sur;
		}else{
			tmp = Orientacion::norte;
		}
	}else{
		double z = 1.0*y/x;
		if (z > 0){
			if(y > 0){
				if(z > sqrt(3)){
					tmp = Orientacion::sur;

				} else if(z < 1/sqrt(3)){
					tmp = Orientacion::este;
				}else{
					tmp = Orientacion::sureste; 
				}

			}else {
				if(z > sqrt(3)){
					tmp = Orientacion::norte;

				} else if(z < 1/sqrt(3)){
					tmp = Orientacion::este;

				}else{
					tmp = Orientacion::noreste;
				}
				
				
			}
		}else{
			if(y > 0){
				if(-z > sqrt(3)){
					tmp = Orientacion::sur;

				} else if(-z < 1/sqrt(3)){
					tmp = Orientacion::oeste;
				}else{
					tmp = Orientacion::suroeste; 
				}

			}else{
				if(-z > sqrt(3)){
					tmp = Orientacion::norte;

				} else if(-z < 1/sqrt(3)){
					tmp = Orientacion::este;
				}else{
					tmp = Orientacion::noreste; 
				}
				
				
			}
		}
	}
	*/

	return tmp;




}
/*

point MasVacio(const vector<vector<casilla>> &matriz){
	int i_max=0;
	int j_max=0;
	int max_j_counter = 0;
	int max_i_counter = 0;
	vector<int> j_vector;
	vector<int> i_vector;
	for(int i = 0; i< matriz.size(); i++){
		j_vector.push_back(0);
		i_vector.push_back(0);
	}
	for(int i = 0; i< matriz.size(); i++){
		for(int j=0; j<matriz.size(); j++){
			if (matriz.at(i).at(j).valor == '?'){
				j_vector.at(j)++;
				i_vector.at(i)++;
			}
		}
	}
	for(int i = 3; i< matriz.size() -3; i++){
		if(j_vector.at(i) > max_j_counter){
			max_j_counter = j_vector.at(i);
			j_max = i;
		}
		if(i_vector.at(i) > max_i_counter){
			max_i_counter = i_vector.at(i);
			i_max = i;
		}
	}
	point p(i_max,j_max);
	p.fil = i_max;
	p.col = j_max;

	return p;
	
}
*/

/*

void NuevaRuta(const state &st, const vector<vector<unsigned char>> &matriz){
	cout << "aqui9" << endl;
	point p(st.p_virtual.fil,st.p_virtual.col);
		
	point q = MasVacio(matriz);
	

	cout << p.fil << "," << p.col << endl;
	cout << q.fil << "," << q.col << endl;

	CambiaDir(st.brujula_virtual,pq,CalOrientacion(p,q,Ma));

}
*/

/*

void RutinaExploracion(const vector<unsigned char> & terreno, Action &accion, unsigned int &algoritmo, const state &st, const vector<vector<unsigned char>> &matriz, priority_queue<rutina> &pq){
	cout << "aqui10" << endl;
	point p(st.p_virtual);
	for(int i = 0; i <4; i++){
		Actualizar(p_virtual.fil,p_virtual.col,st.brujula_virtual);
	}
	p_virtual.fil = Ajuste(p_virtual.fil,matriz.size()-1);
	p_virtual.col = Ajuste(p_virtual.col,matriz.size()-1);
	
	if((terreno[2] == 'T' or terreno[2] =='S' or terreno[2] == 'G' or terreno[2] == 'B' )
		and (matriz.at(p_virtual.fil).at(p_virtual.col) == '?')){
			accion = actWALK;
		}
		else if((matriz.at(p_virtual.fil).at(p_virtual.col) == '?')) {
			accion =actTURN_L;
			algoritmo = 0;
		}else{
			NuevaRuta(st,matriz,pq);
		}
}

*/

void RutinaBasica(const vector<unsigned char> & terreno, Action &accion, unsigned int &algoritmo){
	if((terreno[2] == 'T' or terreno[2] =='S' or terreno[2] == 'G' or terreno[2] == 'B' )){
			accion = actWALK;
			algoritmo = 0;
	}
	else {
		accion =actTURN_L;
		algoritmo = 0;
	}

}

/*

void Explorar(point inicio,const vector<vector<casilla>> &matriz,point target,list<casilla*> &lis){
	set<casilla> visitados; 
	priority_queue<casilla*,vector<casilla*>,casilla::operator()> desarrollo; 

	casilla* top = matriz.at(inicio.fil).at(inicio.col);
	casilla* back = matriz.at(target.fil).at(target.col);

	top->setPadre(nullptr);

	int dist =(*top).dis(*target);
	int iteraciones = 0;

	visitados.insert(matriz.at((*top).p_virtual.fil).at((*top).p_virtual.col));


	while((*top->dis(*target) != 0) and (iteraciones < 150)){
		for(int i = -1; i <= 1; i++){
			for(int j = -1; j<=1; j++ ){
				if(visitados.insert(matriz.at(((*top).p_virtual.fil+i)%matriz.size()).at(((*top).p_virtual.col+j)%matriz.size())).second == true ){
					if(matriz.at(((*top).p_virtual.fil+i)%matriz.size()).at(((*top).p_virtual.col+j)%matriz.size()).valor != 'M' and matriz.at(((*top).p_virtual.fil+i)%matriz.size()).at(((*top).p_virtual.col+j)%matriz.size()).valor != 'P' ){
						matriz.at(((*top).p_virtual.fil+i)%matriz.size()).at(((*top).p_virtual.col+j)%matriz.size()).CalculaValoracion(*target);
						matriz.at(((*top).p_virtual.fil+i)%matriz.size()).at(((*top).p_virtual.col+j)%matriz.size()).setPadre(top);
						desarrollo.push(&matriz.at(((*top).p_virtual.fil+i)%matriz.size()).at(((*top).p_virtual.col+j)%matriz.size()));
					}
				}
			}
		}

		top = desarrollo.top();
		desarrollo.pop();
	}

	while(top->padre != nullptr){
		lis.emplace_front(top);
		top = top->padre;
	}

}

*/

void ComportamientoJugador::PonerTerrenoEnMatriz(const vector<unsigned char> & terreno, state &st, vector<vector<unsigned char>> &matriz){

    int x;
    int y;
    switch (st.brujula_virtual)
    {
        case norte:
            x = -1;
            y = 0;
            break;
        case sur:
            x = 1;
            y = 0;
            break;
        case este:
            x = 0;
            y = 1;
            break;
        case oeste:
            x = 0;
            y = -1;
            break;
        case noreste:
            x = -1;
            y = 1;
            break;
        case noroeste:
            x = -1;
            y = -1;
            break;
        case sureste:
            x = 1;
            y = 1;
            break;
        case suroeste:
            x = 1;
            y = -1;
            break;
        default:
            break;
    }
    if(st.brujula_virtual%2 == 0){
        for (int j = 0; j <= 3; j++){
            for(int i = -j; i <= j; i++){
                Asignar(matriz, point((st.p_virtual.fil + +i*y +j*x ), (st.p_virtual.col -i*x +j*y)), terreno.at(j*(j+1)+i)); 
			}
            
        }
    }

    if(st.brujula_virtual%2 == 1){
		 
        for (int j = 0; j <= 3; j++){

            for(int i = -j; i < 0; i++){
				Asignar(matriz, point((st.p_virtual.fil + +j*x ), (st.p_virtual.col + i*y +j*y)), terreno.at(j*(j+1)+i*(-1)*x*y));
            }

            for(int i = 0; i <= j; i++){
                Asignar(matriz, point((st.p_virtual.fil -i*x +j*x), (st.p_virtual.col +j*y)), terreno.at(j*(j+1)+i*(-1)*x*y));
			}
        }
    }

}

void ComportamientoJugador::ActulizarMapa(vector<vector<unsigned char>> &mapa_virtual, vector<vector<unsigned char>> &mapa_real, const state &st){
	
	int x,y,z,w;
	unsigned int val = (8+st.brujula_real-st.brujula_virtual)%8;
	switch (val)
	{
	case 0:
		x = 1;
		y = 0;
		w = 0;
		z = 1;
		break;
	case 2:
		x = 0;
		y = 1;
		w = -1;
		z = 0;
		break;
	case 4:
		x = -1;
		y = 0;
		w = 0;
		z = 1;
		break;
	case 6:
		x = 0;
		y = -1;
		w = 1;
		z = 0;
		break;
	default:
		break;
	}
	int tam = mapa_virtual.size();
	for(int i = 0; i < tam ; i++){
		for(int j = 0; j < tam; j++){
			if (Igual(mapa_real, point((st.p_real.fil + i),(st.p_real.col +j)), '?') and !Igual(mapa_virtual, point((st.p_virtual.fil +i*x+j*y),(st.p_virtual.col+i*w+j*z)), '?')){
				Asignar(mapa_real, point((st.p_real.fil + i),(st.p_real.col +j)), Dato(mapa_virtual,point((st.p_virtual.fil +i*x+j*y),(st.p_virtual.col+i*w+j*z))));
			}
		}
	}

}

pair<point,int>  ComportamientoJugador::CalculaValoracion (const state &st,const vector<vector<unsigned char>> &matriz, const point &p){
    

	int sum = 0;
    if(Igual(matriz, p,'M') or Igual(matriz, p,'P')){
      sum = -50000;
    }else{
			double val = ValCasilla(Dato(matriz, p),st);
			sum += val; 

			if(st.brujula_virtual == CalOrientacion(st.p_virtual,p)){
				sum += 100;
			}

			if( st.target != point(-1,-1) ){
				sum += 5000/pow(st.target.dis(p,tam) + 1,2);
			}
    }

	pair<point,int> par = pair<point,int>(p,sum); 

    return par;

    
  }

Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;

	// Mostrar el valor de los sensores
	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC;
	switch (sensores.sentido)
	{
		case norte:	  cout << " Norte\n";	break;
		case noreste: cout << " Noreste\n";	break;
		case este:    cout << " Este\n";	break;
		case sureste: cout << " Sureste\n";	break;
		case sur:     cout << " Sur\n";	break;
		case suroeste:cout << " Suroeste\n";	break;
		case oeste:   cout << " Oeste\n";	break;
		case noroeste:cout << " Noroeste\n";	break;
	}
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];

	cout << "  Agentes: ";
	for (int i=0; i<sensores.agentes.size(); i++)
		cout << sensores.agentes[i];

	cout << "\nColision: " << sensores.colision;
	cout << "  Reset: " << sensores.reset;
	cout << "  Vida: " << sensores.vida << endl<< endl;

	//std::this_thread::sleep_for(std::chrono::seconds(3));

	//Modificar variables
	switch (last_action)
	{
	case actWALK:
		Actualizar(current_state.p_virtual.fil,current_state.p_virtual.col,current_state.brujula_virtual);
			break;
	case actRUN:
		Actualizar(current_state.p_virtual.fil,current_state.p_virtual.col,current_state.brujula_virtual);
		Actualizar(current_state.p_virtual.fil,current_state.p_virtual.col,current_state.brujula_virtual);

		break;

	case actTURN_L:
		current_state.brujula_virtual = static_cast<Orientacion>((current_state.brujula_virtual+6)%8);

		break;

	case actTURN_SR:
		current_state.brujula_virtual = static_cast<Orientacion>((current_state.brujula_virtual+1)%8);
		break;
	
	default:
		break;
	}

	//Actualizamos el punto

	if(sensores.terreno.at(0) == 'G'){
		current_state.condiciones.bien_ubicado = true;

	}else if (sensores.terreno.at(0) == 'D'){
		current_state.condiciones.zapatillas = true;

	}else if(sensores.terreno.at(0) == 'K'){
		current_state.condiciones.bikini = true;

	}
	if (sensores.bateria < 1000){
		current_state.condiciones.bateria = true;

	}
	if (sensores.bateria > 3000){
		current_state.condiciones.bateria = false;

	}

	PonerTerrenoEnMatriz(sensores.terreno,current_state,mapa_aux);
	
	if (current_state.condiciones.bien_ubicado){
		current_state.p_real.fil = sensores.posF;
		current_state.p_real.col = sensores.posC;
		current_state.brujula_real = sensores.sentido;
		ActulizarMapa(mapa_aux,mapaResultado,current_state);
	}

	vector<vector<unsigned char>>* mapa;
	if(current_state.condiciones.bien_ubicado){
		mapa = &mapaResultado;
	}else{
		mapa = &mapa_aux;
	}

	bool continuar = true;
	int i,j; 
	i = 2;
	j = 4;
	while( i < 16 and continuar){
		if(sensores.terreno.at(i) == 'M' or sensores.terreno.at(i) == 'P'){
			continuar = false;
		}
		i += j;
		j+=2;
	}

	if(!continuar){
		current_state.target = BuscaPunto(current_state);
	}

	if(current_state.p_virtual == current_state.target){
		current_state.target = Seguir(current_state);
	}

	
	for(int i = 0; i< mapa_aux.size(); i++){
		for(int j = 0; j < mapa_aux.size(); j++){
			cout << (*mapa).at(i).at(j).valor << " ";
		}
		cout << endl;
	}
	

	priority_queue<pair<point,int>,vector<pair<point,int>>,myComp> movimiento;
	for(int i = -1; i <= 1 ;i++){
		for(int j = -1; j<= 1; j++){
			pair<point,int> pa = CalculaValoracion(current_state,*mapa,point((current_state.p_virtual.fil +i),(current_state.p_virtual.col + j)));
			movimiento.push(pa);
			cout << pa.second << " ";
		}
		cout << endl;

	}
	Action act; 

	switch (current_state.p_virtual.dis(movimiento.top().first,tam))
	{
	case 0:
		accion = actIDLE;
		break;
	case 1:
		act = CambiaDir(current_state.brujula_virtual, CalOrientacion(current_state.p_virtual,movimiento.top().first));
		if( act == actIDLE) {
			accion = actWALK;
		}else{
			accion = act;
		}

		break;
	default:
		break;

	}
	





		
	/*
	int i = 9;
	bool no_seguir = true; 
	while( i < 16 and no_seguir ){
		if(sensores.terreno.at(i) != 'M' or sensores.terreno.at(i) != 'P'){
			no_seguir = false;
		}
	}

	if(!seguir){
		current_state.condiciones.atrapado_muros = true;
	}
	if()
		
	}
	
	if(current_state.condiciones.mucho_precipicio){

	}

	*/



	
	

	

	/*
	for(int i = 0; i< mapa_aux.size(); i++){
		for(int j = 0; j < mapa_aux.size(); j++){
			cout << mapa_aux.at(i).at(j).valor << " ";
		}
		cout << endl;
	}
	*/


	//std::this_thread::sleep_for(std::chrono::seconds(3));
/*
	//Toma de decisiones
	point max_run,max_walk,max_idle;
	int val_run, val_walk,val_idle;
	int tam = mapaResultado.size();

	val_idle = val_walk = val_run = -400000;

	max_run = max_walk = max_idle  = current_state.p_virtual;
	for(int i = -2; i <= 2 ;i++){
		for(int j = -2; j<= 2; j++){
			casilla c;
			c = mapa_aux.at((current_state.p_virtual.fil + (i + 3*tam))%tam).at((current_state.p_virtual.col+ (j + 3*tam))%tam);
			c.valoracion = CalculaValoracion(current_state,mapa_aux,(current_state.p_virtual.fil + (i + 3*tam))%tam,(current_state.p_virtual.col+ (j + 3*tam))%tam);
			mapa_aux.at((current_state.p_virtual.fil + (i + 3*tam))%tam).at((current_state.p_virtual.col+ (j + 3*tam))%tam)	= c;

		}

	}

	for(int i = -2; i <= 2 ;i++){
		for(int j = -2; j<= 2; j++){
			cout << mapa_aux.at((current_state.p_virtual.fil + (i + 3*tam))%tam).at((current_state.p_virtual.col+ (j + 3*tam))%tam).valoracion << "  ";
		}
		cout << endl;

	}

	//std::this_thread::sleep_for(std::chrono::seconds(2)); 
	casilla c;
	for(int i = -2; i <= 2 ;i++){
		for(int j = -2; j<= 2; j++){

			if (i == 0 and j == 0){
				c = mapa_aux.at((current_state.p_virtual.fil+ (i + 3*tam))%tam).at((current_state.p_virtual.col+ (j + 3*tam))%tam);
				val_idle = c.valoracion  - 400;
				max_idle = c.p ;
			}else if ((abs(i) == 1) and (abs(j) == 1)){
				c = mapa_aux.at((current_state.p_virtual.fil+ (i + 3*tam))%tam).at((current_state.p_virtual.col+ (j + 3*tam))%tam);
				c.valoracion -= 2*CBateria(mapa_aux.at((current_state.p_virtual.fil+ 3*tam )%tam).at((current_state.p_virtual.col + 3*tam)%tam).valor,current_state.condiciones.at(2),current_state.condiciones.at(3),false, false);
				if(val_walk < c.valoracion ){
					val_walk = c.valoracion;
					max_walk = c.p ;
				}
			}else if((abs(j) != 1) and(abs(i) != 1)){
				c = mapa_aux.at((current_state.p_virtual.fil + (i + tam))%tam).at((current_state.p_virtual.col + (j + tam))%tam);
				c.valoracion -= 2*CBateria(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col).valor,current_state.condiciones.at(2),current_state.condiciones.at(3),true, false);
				
				cout << c.p.to_s() << endl; 
				if(val_run < c.valoracion){
					val_run = c.valoracion;
					max_run = c.p ;
				}
			}

			
			/*
			case 2: 
				c = mapa_aux.at((current_state.p_virtual.fil + (i + tam))%tam).at((current_state.p_virtual.col + (j + tam))%tam);
				c.valoracion -= 2*CBateria(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col).valor,current_state.condiciones.at(2),current_state.condiciones.at(3),true, false);
				
				if(val_run < c.valoracion){
					val_run = c.valoracion;
					max_run = c.p ;
				}

			break;
			
		}
			
	}
	

	cout << "aqui" << val_idle << endl; 
	cout << "caminar" << val_walk << endl;
	cout << "correr" << val_run << endl;

	

	//std::this_thread::sleep_for(std::chrono::seconds(1)); 
	

	
	if(val_idle > val_walk){
		if(val_idle > val_run){
			accion = actIDLE;
		}else{
			Action act = CambiaDir(current_state.brujula_virtual, CalOrientacion(current_state.p_virtual,max_run,mapa_aux.size()));
			if( act == actIDLE) {
				accion = actRUN;
			}else{
				accion = act;
			}
		}
	}else{
		if(val_walk > val_run){
			Action act = CambiaDir(current_state.brujula_virtual, CalOrientacion(current_state.p_virtual,max_walk,mapa_aux.size()));

			cout << current_state.p_virtual.to_s() << endl; 
			cout << max_walk.to_s() << endl; 
			//std::this_thread::sleep_for(std::chrono::seconds(1));

			if( act == actIDLE) {
				
				accion = actWALK;
			}else{
				accion = act;
			}
			
		}else{
			cout << current_state.p_virtual.to_s() << endl; 
			cout << max_run.to_s() << endl; 
			//std::this_thread::sleep_for(std::chrono::seconds(1));
			Action act = CambiaDir(current_state.brujula_virtual, CalOrientacion(current_state.p_virtual,max_run,mapa_aux.size()));
			cout << mapa_aux.at(max_run.fil).at(max_run.col).valoracion << endl;
			cout << CalOrientacion(current_state.p_virtual,max_run,mapa_aux.size()) << endl;
			cout << current_state.brujula_virtual << endl;
			
			if( act == actIDLE) {
				accion = actRUN;
			}else{
				accion = act;
			}
		}
	}

	*/

	cout << current_state.target.to_s() << endl; 

	last_action = accion;
	
	return accion;
};

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}