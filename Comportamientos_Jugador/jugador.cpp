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
#include<iomanip>

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

Importantes& Importantes::operator=(const Importantes &rhs ){
	if(this != &rhs){
		this->bateria = rhs.bateria;
		this->dis_origen = rhs.dis_origen;
		this->hijos = rhs.hijos;
		this->huida = rhs.huida;
	}

	return *this;
}



void ComportamientoJugador::Actualizar(int &x,int &y, const Orientacion &brujula, int k){

	switch (brujula)
		{
		case norte:
			x-=k;
			break;
		case noreste:
			x-=k;
			y+=k;
			break;	
		case este:
			y+=k;
			break;
		case sureste:
			x+=k;
			y+=k;
			break;
		case sur:
			x+=k;
			break;
		case suroeste:
			x+=k;
			y-=k;
			break;
		case oeste:
			y-=k;
			break;
		case noroeste:
			x-=k;
			y-=k;
			break;
		default:
			break;
		}

		x = (x + 3*tam)%tam;
		y = (y + 3*tam)%tam; 

}

void ComportamientoJugador::Actualizar(point &p, const Orientacion &brujula, int k){
	Actualizar(p.fil,p.col,brujula, k); 
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
	matriz.at((p.fil + 4*tam)%tam).at((p.col+ 4*tam)%tam) = c;
}

bool ComportamientoJugador::Igual(const vector<vector<unsigned char>> &matriz, const point &p, unsigned char c){
	return (matriz.at((p.fil + 4*tam)%tam).at((p.col+ 4*tam)%tam) == c);
}

unsigned char ComportamientoJugador::Dato(const vector<vector<unsigned char>> &matriz, const point &p){
	return (matriz.at((p.fil+ 4*tam)%tam).at((p.col+ 4*tam)%tam));
}

void ComportamientoJugador::AsignarRecorridos(vector<vector<Importantes>> &matriz, const point &p, int cual, int valor){

	switch (cual)
	{
	case 0:
		matriz.at((p.fil + 4*tam)%tam).at((p.col+ 4*tam)%tam).bateria = valor;
		break;
	case 1:
		matriz.at((p.fil + 4*tam)%tam).at((p.col+ 4*tam)%tam).dis_origen = valor;
		break;
	case 2:
		matriz.at((p.fil + 4*tam)%tam).at((p.col+ 4*tam)%tam).hijos += valor;
		break;
	default:
		break;
	}
	
}

point ComportamientoJugador::PuntoOrientado(const state &st,Orientacion brujula){
	point p = st.p_virtual;
	Actualizar(p.fil,p.col,brujula,1);
	return (p);
}

bool ComportamientoJugador::CasillaPosible(int x, int y, const vector<vector<unsigned char>> &mapa){
	unsigned char c;
	int i = -1;
	int j = -1;
	bool posible = false;

	while( i <= 1 and !posible){
		while ( j <= 1 and !posible){
			c = Dato(mapa,point(x+i,y+j));

			if((c != 'M' and c != 'P') and (i != 0 and j != 0)){
				posible = true; 
			}
			j++;
		}
		j = -1;
		i++;
	}

	return posible;
}



point ComportamientoJugador::BuscaPunto(const state &st){
	int random = rand()%7;
	random++;
	point p = st.p_virtual;
	for(int i = 0 ; i < 4; i++){
		Actualizar(p.fil,p.col,static_cast<Orientacion>((st.brujula_virtual + random)%8),1);
	}

	return(p);
}

point ComportamientoJugador::Seguir(const state &st){
	point p = st.p_virtual;
	for(int i = 0 ; i < 4; i++){
		Actualizar(p.fil,p.col,st.brujula_virtual,1);
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

int ComportamientoJugador::CBateria(unsigned char v, bool correr,bool girar){
  int coste;
  
  

  switch (v)
  {
  case 'A':
    if(correr){
		if(current_state.condiciones.bikini){
			coste = 10;
		}else{
			coste = 500;
		}
	}else if(girar) {
		if(current_state.condiciones.bikini){
			coste = 5;
		}else{
			coste = 50;
		}
	}else{
		if(current_state.condiciones.bikini){
			coste = 5;
		}else{
			coste = 50;
		}
	}
    break;
  
  case 'B':
    if(correr){
		if(current_state.condiciones.zapatillas){
			coste = 15;
		}else{
			coste = 250;
		}
	}else if(girar) {
		if(current_state.condiciones.zapatillas){
			coste = 1;
		}else{
			coste = 10;
		}
	}else{
		if(current_state.condiciones.zapatillas){
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

Action ComportamientoJugador::Andar_Correr(const vector<unsigned char> &agentes){
	Action devolver; 
	point there = current_state.p_virtual;
	point next_1 = current_state.p_virtual;
	point next_2 = current_state.p_virtual;

	Actualizar(next_1,current_state.brujula_virtual,1);
	Actualizar(next_2,current_state.brujula_virtual,2);

	if((mapa_aux.at(next_2.fil).at(next_2.col) != 'M' and mapa_aux.at(next_2.fil).at(next_2.col) != 'P' and mapa_aux.at(next_2.fil).at(next_2.col) != '?') and agentes.at(6) == '_'){
		int suma1 = CBateria(mapa_aux.at(there.fil).at(there.col),0,0) + CBateria(mapa_aux.at(next_1.fil).at(next_1.col),0,0);
		int suma2 = CBateria(mapa_aux.at(there.fil).at(there.col),1,0);

		if(suma1 >= suma2){
			devolver = actRUN;
		}else{
			devolver = actWALK;
		}

	}else{
		devolver = actWALK;

	}

	return devolver; 


}	

point ComportamientoJugador::Mas_Cercana(){
	point devolver = point(-1,-1); 
	const int VALOR = 1; 
	int i,j,incremento_i,incremento_j;



	switch(rand()%4){
		default:
		case 0:
			i = -VALOR;
			j = -VALOR;
			incremento_i = 1;
			incremento_j = 1;
			break;
		case 1:
			i = VALOR;
			j = -VALOR;
			incremento_i = -1;
			incremento_j = 1;
		case 2:
			i = VALOR ;
			j = VALOR ;
			incremento_i = -1;
			incremento_j = -1;
		case 3: 
			i = VALOR;
			j = -VALOR;
			incremento_i = -1;
			incremento_j = 1;

			break; 
	}
	bool encontrado = false;
	int x_virtual;
	int y_virtual;
	while(abs(i) != VALOR +1 and !encontrado){
		while(abs(j) != VALOR + 1 and !encontrado ){
			x_virtual = (current_state.p_virtual.fil + i + 4*tam)%tam;
			y_virtual = (current_state.p_virtual.col + j + 4*tam)%tam; 

			if(mapa_recorrido.at(x_virtual).at(y_virtual).hijos > 0 and mapa_recorrido.at(x_virtual).at(y_virtual).dis_origen != -1){
				encontrado = true;
				devolver = point(x_virtual,y_virtual);
			}
			i+=incremento_i;
			j+=incremento_j;
		}

	}

	return devolver; 

}

Action ComportamientoJugador::Movimiento(const vector<unsigned char> &v, const state &st, unsigned char c){
	Action action; 

	/*

	if(v.at(2) == c or v.at(6) == c or v.at(12) == c){
		action = actWALK;
	}else if(v.at(3) == c or v.at(8) == c or v.at(15) == c ){
		action = actTURN_SR;
	}else if(v.at(1) == c or v.at(4) == c or v.at(9) == c){
		action = actTURN_L;
	}else {
		action = actWALK;
	} 
	*/

	if(v.at(2) == c){
		action = actWALK;
	}else if(v.at(6) == c) {
		if((v.at(2) == 'M' or v.at(2) == 'P')){
			action = actIDLE;
		}else{
			action = actWALK;
		}
		
	}else if(v.at(12) == c){
		
		if(((v.at(2) == 'M' or v.at(2) == 'P') or (v.at(6) == 'M' or v.at(6) == 'P'))){
			action = actIDLE;
		}else{
			action = actWALK;
		}
	}else if(v.at(3) == c){
		action = actTURN_SR;
	}else if(v.at(8) == c) {
		if((v.at(3) == 'M' or v.at(3) == 'P')){
			action = actIDLE;
		}else{
			action = actTURN_SR;
		}
		
	}else if(v.at(15) == c){
		if((v.at(3) == 'M' or v.at(3) == 'P') or (v.at(8) == 'M' or v.at(8) == 'P')){
			action = actIDLE;
		}else{
			action = actTURN_SR;
		}
	}else if(v.at(1) == c){
		action = actTURN_L;
	}else if(v.at(4) == c ) {
		if((v.at(1) == 'M' or v.at(1) == 'P')){
			action = actIDLE;
		}else{
			action = actTURN_L;
		}
		
	}else if(v.at(9) == c){
		if(((v.at(1) == 'M' or v.at(1) == 'P') or (v.at(4) == 'M' or v.at(4) == 'P'))){
			action = actIDLE;
		}else{
			action = actTURN_L;
		}
	}else{
		if(v.at(2) == 'M' or v.at(2) == 'P'){
			action = actIDLE;
		}else{
			action = actWALK;
		}
	}

	return action;

	

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


point MasVacio(const vector<vector<unsigned char>> &matriz){
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
			if (matriz.at(i).at(j) == '?'){
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

void ComportamientoJugador::CambiarHijos(const point &p){

	point q;

	for(int i = -intervalo; i <= intervalo; i++ ){
		for(int j = -intervalo; j <= intervalo; j++){
			q = point(p.fil + i, p.col +j);
			AsignarRecorridos(mapa_recorrido,q,2,-1);			
			
		}
	}

}

void ComportamientoJugador::CambiarDisOrigen(const point &p){
	point q;
	int min = tam*tam;


	if(mapa_aux.at(p.fil).at(p.col) == '?' or mapa_aux.at(p.fil).at(p.col) == 'M' or mapa_aux.at(p.fil).at(p.col) == 'P'){
		mapa_recorrido.at(p.fil).at(p.col).dis_origen = -1;
	}else{
		for(int i = 0; i < 8; i++){
			q = p; 
			Actualizar(q.fil,q.col,static_cast<Orientacion>(i),1);
			if(mapa_aux.at(q.fil).at(q.col) != '?' and mapa_aux.at(q.fil).at(q.col) != 'M' and mapa_aux.at(q.fil).at(q.col) != 'P'){
				if(mapa_recorrido.at(q.fil).at(q.col).dis_origen >= 0 and mapa_recorrido.at(q.fil).at(q.col).dis_origen < min){
					min = mapa_recorrido.at(q.fil).at(q.col).dis_origen;
				}
			}
		}

		if(min != tam*tam and (mapa_recorrido.at(p.fil).at(p.col).dis_origen < 0 or  mapa_recorrido.at(p.fil).at(p.col).dis_origen > min +1 )){

			mapa_recorrido.at(p.fil).at(p.col).dis_origen = min + 1;

			for(int i = 0; i < 8; i++){
				q = p; 
				Actualizar(q.fil,q.col,static_cast<Orientacion>(i),1);
				if(mapa_aux.at(q.fil).at(q.col) != '?' and mapa_aux.at(q.fil).at(q.col) != 'M' and mapa_aux.at(q.fil).at(q.col) != 'P'){
					if(mapa_recorrido.at(q.fil).at(q.col).dis_origen > min + 1 or mapa_recorrido.at(q.fil).at(q.col).dis_origen < 0){
						Por_actualizar.at(1).push(q);
					}
				}
			}

		}

	}

}

void ComportamientoJugador::CambiarHuida (const point &p){
	point q;
	int min = tam*tam;


	if(mapa_aux.at(p.fil).at(p.col) == '?' or mapa_aux.at(p.fil).at(p.col) == 'M' or mapa_aux.at(p.fil).at(p.col) == 'P'){
		mapa_recorrido.at(p.fil).at(p.col).huida= -1;
	}else{
		for(int i = 0; i < 8; i++){
			q = p; 
			Actualizar(q.fil,q.col,static_cast<Orientacion>(i),1);
			if(mapa_aux.at(q.fil).at(q.col) != '?' and mapa_aux.at(q.fil).at(q.col) != 'M' and mapa_aux.at(q.fil).at(q.col) != 'P'){
				if(mapa_recorrido.at(q.fil).at(q.col).huida >= 0 and mapa_recorrido.at(q.fil).at(q.col).huida < min){
					min = mapa_recorrido.at(q.fil).at(q.col).huida;
				}
			}
		}

		if(min != tam*tam and (mapa_recorrido.at(p.fil).at(p.col).huida < 0 or  mapa_recorrido.at(p.fil).at(p.col).huida > min +1 )){

			mapa_recorrido.at(p.fil).at(p.col).huida = min + 1;

			for(int i = 0; i < 8; i++){
				q = p; 
				Actualizar(q.fil,q.col,static_cast<Orientacion>(i),1);
				if(mapa_aux.at(q.fil).at(q.col) != '?' and mapa_aux.at(q.fil).at(q.col) != 'M' and mapa_aux.at(q.fil).at(q.col) != 'P'){
					if(mapa_recorrido.at(q.fil).at(q.col).huida > min + 1 or mapa_recorrido.at(q.fil).at(q.col).huida < 0){
						Por_actualizar.at(1).push(q);
					}
				}
			}

		}

	}

}


void ComportamientoJugador::CambiarBateria(const point &p){
	point q;
	int min = tam*tam;

	if(mapa_aux.at(p.fil).at(p.col) == '?' or mapa_aux.at(p.fil).at(p.col) == 'M' or mapa_aux.at(p.fil).at(p.col) == 'P'){
		mapa_recorrido.at(p.fil).at(p.col).bateria = -1;
	}else{
		for(int i = 0; i < 8; i++){
			q = p; 
			Actualizar(q.fil,q.col,static_cast<Orientacion>(i),1);
			if(mapa_aux.at(q.fil).at(q.col) != '?' and mapa_aux.at(q.fil).at(q.col) != 'M' and mapa_aux.at(q.fil).at(q.col) != 'P'){
				if(mapa_recorrido.at(q.fil).at(q.col).bateria >= 0  and mapa_recorrido.at(q.fil).at(q.col).bateria < min){
					min = mapa_recorrido.at(q.fil).at(q.col).bateria;
				}
			}
		}
		if(min != tam*tam and (mapa_recorrido.at(p.fil).at(p.col).bateria < 0 or  mapa_recorrido.at(p.fil).at(p.col).bateria > min +1 )){
			mapa_recorrido.at(p.fil).at(p.col).bateria = min + 1;

			for(int i = 0; i < 8; i++){
				q = p; 
				Actualizar(q.fil,q.col,static_cast<Orientacion>(i),1);
				if(mapa_aux.at(q.fil).at(q.col) != '?' and mapa_aux.at(q.fil).at(q.col) != 'M' and mapa_aux.at(q.fil).at(q.col) != 'P'){
					if(mapa_recorrido.at(q.fil).at(q.col).bateria > min + 1 or mapa_recorrido.at(q.fil).at(q.col).bateria < 0){
						Por_actualizar.at(0).push(q);
					}
				}
			}
		}

	}

	
	

	
}

void ComportamientoJugador::ActualizacionGlobal(){
	point p;
	while(!Por_actualizar.at(0).empty()){
		p = Por_actualizar.at(0).front();
		Por_actualizar.at(0).pop();

		CambiarBateria(p);
	}
	while(!Por_actualizar.at(1).empty()){
		p = Por_actualizar.at(1).front();
		Por_actualizar.at(1).pop(); 

		CambiarDisOrigen(p);
	}
	while(!Por_actualizar.at(2).empty()){
		p = Por_actualizar.at(2).front();
		Por_actualizar.at(2).pop();

		CambiarHijos(p);
	}
	while(!Por_actualizar.at(3).empty()){
		p = Por_actualizar.at(3).front();
		Por_actualizar.at(3).pop();

		CambiarHuida(p);
	}

}

void ComportamientoJugador::PonerTerrenoEnMatriz(const vector<unsigned char> & terreno, state &st, vector<vector<unsigned char>> &matriz, bool act_cola){
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

	int x_virtual;
	int y_virtual; 

    if(st.brujula_virtual%2 == 0){
        for (int j = 0; j <= 3; j++){
            for(int i = -j; i <= j; i++){
				x_virtual = (st.p_virtual.fil +i*y +j*x + 4*tam)%tam;
				y_virtual = (st.p_virtual.col -i*x +j*y + 4*tam)%tam;
				if(terreno.at(j*(j+1)+i) != '?' and matriz.at(x_virtual).at(y_virtual) == '?'){
					
					Asignar(matriz, point(x_virtual,y_virtual), terreno.at(j*(j+1)+i)); 
					if(act_cola){
						for(int k = 0; k < 3; k++){
							Por_actualizar.at(k).push(point(x_virtual,y_virtual));
						}
					}
				}
                
			}
            
        }
    }

    if(st.brujula_virtual%2 == 1){
		 
        for (int j = 0; j <= 3; j++){
            for(int i = -j; i < 0; i++){
				x_virtual = (st.p_virtual.fil +j*x + 4*tam)%tam;
				y_virtual = (st.p_virtual.col + i*y +j*y + 4*tam)%tam;

				if(terreno.at(j*(j+1)+i*(-1)*x*y) != '?' and matriz.at(x_virtual).at(y_virtual) == '?'){
					Asignar(matriz, point(x_virtual,y_virtual ), terreno.at(j*(j+1)+i*(-1)*x*y));
					if(act_cola){
						for(int k = 0; k < 3; k++){
							Por_actualizar.at(k).push(point(x_virtual,y_virtual));
						}
					}
				}
            }

            for(int i = 0; i <= j; i++){
				x_virtual = (st.p_virtual.fil -i*x +j*x + 4*tam)%tam;
				y_virtual = (st.p_virtual.col +j*y + 4*tam)%tam;

				if(terreno.at(j*(j+1)+i*(-1)*x*y) != '?' and matriz.at(x_virtual).at(y_virtual) == '?'){
                	Asignar(matriz, point(x_virtual,y_virtual), terreno.at(j*(j+1)+i*(-1)*x*y));
					if(act_cola){
						for(int k = 0; k < 3; k++){
							Por_actualizar.at(k).push(point(x_virtual,y_virtual));
						}
					}
				}
			}
        }
    }

}

point ComportamientoJugador::CalculaPunto(const point &p, Orientacion org, int pos){
	point devolver = p; 
	int j = 1;
	int k = 1;

	if(pos != 0){
		while(!((j*(j+1)-k <= pos) and (pos <= j*(j+1)+k))){
				j++; 
				k++; 
		}
		
		int z = pos - j*(j+1);
		cout << "mira: "<< j << "," << z << endl; 
		if(org%2 == 0){
			Actualizar(devolver.fil,devolver.col,org,j);
			if(z > 0){
				Actualizar(devolver.fil,devolver.col, static_cast<Orientacion>((org +2)%8),abs(z));
			}else{
				Actualizar(devolver.fil,devolver.col, static_cast<Orientacion>((org +6)%8),abs(z));
			}
			

		}else{
			Actualizar(devolver.fil,devolver.col,org,j);
			if(z > 0){
				Actualizar(devolver.fil,devolver.col, static_cast<Orientacion>((org +3)%8),abs(z));
			}else{
				Actualizar(devolver.fil,devolver.col, static_cast<Orientacion>((org +5)%8),abs(z));
			}

		}
		
	}

	return devolver;
}


Orientacion ComportamientoJugador::Investiga(const vector<vector<unsigned char>> &mapa, const point &casilla, unsigned char c){

	Orientacion way; 
	bool acaba = false; 
	bool incrementa = true;


	int x,y,z,w;
	x = z = casilla.fil;
	y = w = casilla.col;

	while (!acaba and incrementa){
		
		incrementa = false; 

		if(x != 0){
			x--;
			incrementa = true; 
		}
		if( y != 0){
			y--;
			incrementa = true; 
		}
		if( z != mapa.size() -1){
			z++;
			incrementa = true; 
		}
		if( w!= mapa.size() -1){
			w++;
			incrementa = true; 
		}

		if(mapa.at(x).at(casilla.col) == c ){
			way = norte;
			acaba = true;

		}else if(mapa.at(z).at(casilla.col) == c ){
			way = sur; 
			acaba = true;

		}else if(mapa.at(casilla.fil).at(y) == c){
			way = oeste; 
			acaba = true;

		}else if(mapa.at(casilla.fil).at(w) == c){
			way = este;
			acaba = true;

		}

	}

	if(!incrementa){
		way = noreste;
	}

	return way;
	
}



Orientacion ComportamientoJugador::PorExplorar(const vector<vector<unsigned char>> &mapa, unsigned char c){
	vector<point> vec;
	vector<int> vi;
	bool actualiza = true; 
	for(int i = 0; i < 8; i++){
		vec.push_back(current_state.p_virtual);
		vi.push_back(0);
	}

	if(!current_state.condiciones.bien_ubicado){
		for(int i = 0; i < mapa.size()/5; i++){
			for(int j = 0; j < 8; j++){
				Actualizar(vec.at(j).fil,vec.at(j).col, static_cast<Orientacion>(j),1); 
				if(Dato(mapa,vec.at(j)) == c){
					vi.at(j)++;
				}
			}
		}
	}else{
		int z = 0; 
		while(actualiza){
			actualiza = false; 
			for(int j = 0; j < 8; j++){
				if((vec.at(j).fil != 0 )and  (vec.at(j).fil != mapa.size() -1 )and  (vec.at(j).col != 0 )and(vec.at(j).col != mapa.size() -1) ){
					Actualizar(vec.at(j).fil,vec.at(j).col, static_cast<Orientacion>(j),1); 
					if(Dato(mapa,vec.at(j)) == c){
						vi.at(j)++;
					}
					actualiza = true;
				}

			}
			
			z++;
		}	
	}

	int max = -1;
	int index = 0; 
	for(int i = 0; i < 8; i++){
		if(vi.at(i) > max){
			max = vi.at(i);
			index = i;
		}
	}

	return(static_cast<Orientacion>(index));


	

	

}	

void ComportamientoJugador::ActulizarMapa(vector<vector<unsigned char>> &mapa_virtual, vector<vector<unsigned char>> &mapa_real, const state &st){ 
	int x,y,z,w;
	vector<vector<Importantes>> aux; 
	vector<vector<int>> aux_i;
	for(int i = 0; i < tam; i++){
		vector<Importantes> v; 
		vector<int> vi;
		for(int j = 0; j< tam; j++){
			Importantes c =Importantes();
			c.hijos = pow((2*intervalo+1),2);
			v.push_back(c);
			vi.push_back(0);
		}
		aux.push_back(v);
		aux_i.push_back(vi);
	}
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
		z = -1;
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
			point pos_real = point((st.p_real.fil +i*x+j*y + 4*tam)%tam,(st.p_real.col+i*w+j*z+ 4*tam)%tam);
			point pos_virtual = point((st.p_virtual.fil + i + 4*tam)%tam, (st.p_virtual.col +j + 4*tam)%tam);
			
			if (mapa_real.at(pos_real.fil).at(pos_real.col) == '?' and mapa_virtual.at(pos_virtual.fil).at(pos_virtual.col) != '?'){
				 //Igual(mapa_real, point((st.p_real.fil +i*x+j*y),(st.p_real.col+i*w+j*z)), '?') and !Igual(mapa_virtual, point((st.p_virtual.fil + i),(st.p_virtual.col +j)), '?')

				mapa_real.at(pos_real.fil).at(pos_real.col) = mapa_virtual.at(pos_virtual.fil).at(pos_virtual.col);
				//Asignar(mapa_real, point((st.p_real.fil +i*x+j*y),(st.p_real.col+i*w+j*z)), Dato(mapa_virtual,point((st.p_virtual.fil + i),(st.p_virtual.col +j))));
				

			}else if(mapa_real.at(pos_real.fil).at(pos_real.col) != '?' and mapa_virtual.at(pos_virtual.fil).at(pos_virtual.col) == '?'){
				for(int i = 0; i < 3; i++){
					Por_actualizar.at(i).push(pos_real);
				}
			}

			aux.at(pos_real.fil).at(pos_real.col) = mapa_recorrido.at(pos_virtual.fil).at(pos_virtual.col);
			aux_i.at(pos_real.fil).at(pos_real.col) = frecuencias.at(pos_virtual.fil).at(pos_virtual.col);

			


		}

	}

	mapa_recorrido = aux;

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

int ComportamientoJugador::CosteManiobra(int i){
	int coste = CBateria(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col),0,1);

	switch(i){
		case 0:
			coste = 0; 
			break;
		case 1:
		case 6:
			coste = coste + 10;
			break;
		case 2:
		case 4: 
		case 7:
			coste = 2*coste + 20;
			break;
		case 3:
		case 5:
			coste = 3*coste + 30;
			break;
		default:
			break; 
	}

	return coste;

}

int ComportamientoJugador::CosteMedioCasilla( unsigned char c){
	int coste = CBateria(c,1,0) + CBateria(c,0,0) + 2*CBateria(c,0,1);
	return (coste/4);
}

int ComportamientoJugador::Explorara(const point &p, Orientacion ori, const Sensores &sen ){
	point pivote = p;
	point pivote_der,pivote_izd;
	int suma = 0;
	int limit = 4;

	if(sen.nivel == 3){
		limit = 2; 
	}
	
	for(int i = 0; i < limit; i++){
		pivote = p;
		Actualizar(pivote,ori,i);
		if(mapa_aux.at(pivote.fil).at(pivote.col) == '?'){
			suma++;
		}
		
		pivote_der = pivote_izd = pivote; 
		for(int j = 0; j < 3-i; j++){
			Actualizar(pivote_der,static_cast<Orientacion>((ori+1)%8),1);
			Actualizar(pivote_izd,static_cast<Orientacion>((ori+7)%8),1);
			if(mapa_aux.at(pivote_der.fil).at(pivote_der.col) == '?'){
				suma++;
			}
			if(mapa_aux.at(pivote_izd.fil).at(pivote_izd.col) == '?'){
				suma++;
			}
		}
	}

	return suma; 
}

void ComportamientoJugador::ExtraeTerreno(const point &p, Orientacion ori, const Sensores &sen, vector<unsigned char> &v){
	point pivote = p;
	point pivote_der,pivote_izd;
	v = sen.terreno; 
	

	if(sen.nivel == 3){

		int x = 5; 
		int y = 5; 

		for(int i = 2; i < 4; i++){
		pivote = p;
		Actualizar(pivote,ori,i);
		v.at(i*(i+1)) = mapa_aux.at(pivote.fil).at(pivote.col);
		pivote_der = pivote_izd = pivote; 
		x = y;
		for(int j = 0; j < 3-i; j++){
			Actualizar(pivote_der,static_cast<Orientacion>((ori+1)%8),1);
			Actualizar(pivote_izd,static_cast<Orientacion>((ori+7)%8),1);
			v.at(i*(i+1) + x) = mapa_aux.at(pivote_izd.fil).at(pivote_izd.col);
			v.at(i*(i+1) + x + 2 ) = mapa_aux.at(pivote_der.fil).at(pivote_der.col);
			x+=2;
		}
		y+=2;
		}
	}
	


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
	if(sensores.colision == false){
		switch (last_action)
		{
		case actWALK:
			Actualizar(current_state.p_virtual.fil,current_state.p_virtual.col,current_state.brujula_virtual,1);
			break;
		case actRUN:
			Actualizar(current_state.p_virtual.fil,current_state.p_virtual.col,current_state.brujula_virtual,2);

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
	}
	

	//Actualizamos el punto

	if(sensores.reset == true){
		init();
	}
	if(sensores.nivel == 0 and !current_state.condiciones.bien_ubicado){
		current_state.condiciones.bien_ubicado = true;
		current_state.p_real.fil = sensores.posF;
		current_state.p_real.col = sensores.posC;
		current_state.brujula_real = sensores.sentido;
		current_state.p_virtual = current_state.p_real;
		current_state.brujula_virtual = current_state.brujula_real;
		mapa_aux = vector<vector<unsigned char>>(mapaResultado);
		current_state.target = point(-1,-1);

	}


	ActualizacionGlobal();



	frecuencias.at(current_state.p_virtual.fil).at(current_state.p_virtual.col)++;

	if(sensores.terreno.at(0) == 'G' and !current_state.condiciones.bien_ubicado){
		current_state.condiciones.bien_ubicado = true;
		current_state.p_real.fil = sensores.posF;
		current_state.p_real.col = sensores.posC;
		current_state.brujula_real = sensores.sentido;
		ActulizarMapa(mapa_aux,mapaResultado,current_state);
		current_state.p_virtual = current_state.p_real;
		current_state.brujula_virtual = current_state.brujula_real;
		mapa_aux = vector<vector<unsigned char>>(mapaResultado);
		current_state.target = point(-1,-1);

	}else if (sensores.terreno.at(0) == 'D'){
		current_state.condiciones.zapatillas = true;
		current_state.target = point(-1,-1);

	}else if(sensores.terreno.at(0) == 'K'){
		current_state.condiciones.bikini = true;
		current_state.target = point(-1,-1);

	}

	if (sensores.bateria < 1000 and sensores.vida > 300){
		current_state.condiciones.bateria = true;
		current_state.condiciones.investiga = false; 

	}
	if (sensores.bateria > 3500 or sensores.vida < 300){
		current_state.condiciones.bateria = false;

	}

	if(sensores.terreno.at(0) == 'X' and current_state.condiciones.bateria){
		last_action = actIDLE;
		return actIDLE;
	}

	PonerTerrenoEnMatriz(sensores.terreno,current_state,mapa_aux,true);
	
	if (current_state.condiciones.bien_ubicado){
		PonerTerrenoEnMatriz(sensores.terreno,current_state,mapaResultado,false);
	}

	ActualizacionGlobal();


	/*
	
	for(int i = 0; i < mapa_aux.size(); i++){
		cout << i << endl;
		for(int j = 0; j < mapa_aux.size(); j++){
			cout << j << endl; 
			cout << mapa_aux.at(i).at(j)<< " ";
		}
		cout << endl;
	}

	*/
	
	
	/*
	for(int i = 0; i< mapa_aux.size(); i++){
		for(int j = 0; j < mapa_aux.size(); j++){
			cout << setw(3) << mapa_recorrido.at(i).at(j).bateria<< " ";
		}
		cout << endl;
	}
	*/

	bool entra = false; 
	bool no_mover = false;

	if((sensores.terreno.at(2) != 'M' and sensores.terreno.at(2) != 'P') and sensores.agentes.at(2) == '_' ){
		 
		if(current_state.target.fil != -1){
			vector<unsigned char> v; 
			
			ExtraeTerreno(current_state.p_virtual, current_state.brujula_virtual, sensores, v);
			
			
			accion = Movimiento(v,current_state,mapa_aux.at(current_state.target.fil).at(current_state.target.col)); 
			entra = true; 
			if(accion == actIDLE){
				no_mover = true;
			}else{
				no_mover = false;
			}

			
		}
		
	}

	int i = 0;
	while(i <16 and !entra and !no_mover){
		unsigned char c = sensores.terreno.at(i);
		if(c == 'G' and !current_state.condiciones.bien_ubicado){
			accion = Movimiento(sensores.terreno,current_state,'G');
			entra = true; 
			current_state.condiciones.investiga = false; 
			current_state.target = CalculaPunto(current_state.p_virtual,current_state.brujula_virtual,i);

		}else if (c == 'D' and !current_state.condiciones.zapatillas){
			accion = Movimiento(sensores.terreno,current_state,'D');
			entra = true; 
			current_state.condiciones.investiga = false; 
			current_state.target = CalculaPunto(current_state.p_virtual,current_state.brujula_virtual,i);

		}else if(c == 'K' and !current_state.condiciones.bikini) {
			accion = Movimiento(sensores.terreno,current_state,'K');
			entra = true; 
			current_state.condiciones.investiga = false;
			current_state.target = CalculaPunto(current_state.p_virtual,current_state.brujula_virtual,i);

		}else if(c == 'X'){
			point p = CalculaPunto(current_state.p_virtual,current_state.brujula_virtual,i);
			mapa_recorrido.at(p.fil).at(p.col).bateria = 0; 
			Por_actualizar.at(0).push(CalculaPunto(current_state.p_virtual,current_state.brujula_virtual,i));
			cont_bateria++;
		}
		i++;
	}

	if(entra){
		cout << "entra:true" << endl; 
	}else{
		cout << "entra:false" << endl; 
	}

	if(no_mover){
		cout << "no_mover:true" << endl; 
	}else{
		cout << "no_mover:false" << endl; 
	}

	

	if(accion == actIDLE){
		entra = false;
		current_state.target = point(-1,-1); 
	}

	if(!entra){
		vector<point> v; 
		point p;
		vector<bool> disponible;
		for(int i = 0; i < 16; i++){
			disponible.push_back(true);
		}
		unsigned char c; 
		
		for(int i = 0; i < 8; i++){
			p = current_state.p_virtual;
			Actualizar(p,static_cast<Orientacion>((current_state.brujula_virtual + i)%8), 1);
			v.push_back(p);
			c = mapa_aux.at(v.at(i).fil).at(v.at(i).col);
			if( c == 'M' or c == 'P'){
				cout << "no valido: " << i << "," << i+8 << endl;
				disponible.at(i) = disponible.at(i + 8) = false;

			}else{
				if(i == 0){
					if(sensores.agentes.at(2) != '_'){
						disponible.at(i) = disponible.at(i + 8) = false; 
					}
				}else if( i == 7){
					if(sensores.agentes.at(1) != '_'){
						disponible.at(i) = disponible.at(i + 8) = false;
					}

				}else if( i == 1){
					if(sensores.agentes.at(3) != '_'){
						disponible.at(i) = disponible.at(i + 8) = false;
					}
				}
			}
		}
		
		for(int i = 8; i < 16; i++){
			p = current_state.p_virtual;
			Actualizar(p,static_cast<Orientacion>((current_state.brujula_virtual + i)%8), 2);
			v.push_back(p);
			c = mapa_aux.at(v.at(i).fil).at(v.at(i).col);
			if( c == 'M' or c == 'P'){
				disponible.at(i) = false;
			}else{

				if(i == 8){
					if(sensores.agentes.at(6) != '_'){
						disponible.at(i) = false; 
					}
				}else if( i == 15){
					if(sensores.agentes.at(4) != '_'){
						disponible.at(i) =  false;
					}

				}else if( i == 9){
					if(sensores.agentes.at(8) != '_'){
						disponible.at(i) = false;
					}
				}
			}

			if(sensores.nivel == 3 and c == '?'){
				disponible.at(i) = false;
			}
		}

		double max = 0;
		int index = -1;
		double valor = 0;
		double bateria = 0;
		double explora = 0;
		
		

		for(int i = 0; i < 16; i++){
			
			if(disponible.at(i)){

				bateria = CosteManiobra(i%8);
				bateria += CosteMedioCasilla(mapa_aux.at(v.at(i).fil).at(v.at(i).col));

				if(i >= 8){
					bateria += CBateria(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col),1,0);
				

				}else{
					bateria += CBateria(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col),0,0);
					bateria += CBateria(mapa_aux.at(v.at(i).fil).at(v.at(i).col),0,0);

				}

				explora = Explorara(v.at(i), static_cast<Orientacion>((current_state.brujula_virtual + i)%8), sensores);

				if(bateria > 400){
					valor = 0;
				}else{
					bateria = (1 - bateria/400);
					explora = (explora/19);
					valor = pow(bateria,2) + pow(explora,2);
				}

				if(frecuencias.at(v.at(i).fil).at(v.at(i).col) > 50 ){
					valor = 0;
				}else{
					double disminuir = 1 - (1.0*frecuencias.at(v.at(i).fil).at(v.at(i).col))/50;
					valor = valor*(disminuir);
				}

				
				
				if(valor > max ){
					max = valor;
					index = i; 
				}

				cout << index << endl;

			}
		}

		if(cont_bateria > 0 and current_state.condiciones.bateria){
			for(int i = 0; i < 16; i++){
			
				if(disponible.at(i)){

					bateria = CosteManiobra(i%8);
					bateria += CosteMedioCasilla(mapa_aux.at(v.at(i).fil).at(v.at(i).col));

					if(i >= 8){
						bateria += CBateria(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col),1,0);
					

					}else{
						bateria += CBateria(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col),0,0);
						bateria += CBateria(mapa_aux.at(v.at(i).fil).at(v.at(i).col),0,0);

					}
					if(mapa_recorrido.at(v.at(i).fil).at(v.at(i).col).bateria != -1){
						explora = mapa_recorrido.at(current_state.p_virtual.fil).at(current_state.p_virtual.col).bateria + 2; 
						explora = explora - mapa_recorrido.at(v.at(i).fil).at(v.at(i).col).bateria; 
						
					}else{
						explora = 0;
					}

					

					 

					if(bateria > 400){
						valor = 0;
					}else{
						bateria = (1 - bateria/400);
						explora = (explora/5); 
						valor = pow(bateria,2) + pow(explora,2);
					}

					cout << "bateria: " << valor << " indice: " << index ; 
					
					
					if(valor > max ){
						max = valor;
						index = i; 
					}

					cout << index << endl;

				}
			}
		}
	


		if(index == 0){
			accion = actWALK;
		}else if (index == 8){
			accion = actRUN;
		}else{
			accion = CambiaDir(current_state.brujula_virtual, (current_state.brujula_virtual + index)%8 );
		}
	}

	


	
	
	

	/*
	if(current_state.brujula_virtual != static_cast<Orientacion>(current_state.way)){
		current_state.way = -1;
	}
	

	if(current_state.way != -1 and current_state.brujula_virtual != static_cast<Orientacion>(current_state.way) ){
		accion = CambiaDir(current_state.brujula_virtual, static_cast<Orientacion>(current_state.way));

	}else if(current_state.condiciones.atrapado_muros){

		if(current_state.condiciones.salida_muros == true){
			current_state.condiciones.salida_muros = false; 
			current_state.condiciones.atrapado_muros = false;
		}
		

		if(false){

			Orientacion way = PorExplorar(mapa_aux, '?');
			current_state.condiciones.investiga = true; 
			current_state.condiciones.salida_muros = false; 
			current_state.condiciones.atrapado_muros = false;
			current_state.way = way;
			accion = CambiaDir(current_state.brujula_virtual,way);


		}else if(sensores.terreno.at(2) == 'M' or sensores.terreno.at(2) == 'P' or sensores.agentes.at(2) != '_'){
			accion = actTURN_L;
		}else if(sensores.terreno.at(3) != 'M' and sensores.terreno.at(3) != 'P') {
			accion = actTURN_SR;
			current_state.condiciones.salida_muros = true;
		}else{
			accion = actWALK;
		}



		
	}else if(sensores.terreno.at(2) == 'M' or sensores.terreno.at(2) == 'P' or sensores.agentes.at(2) != '_'){
		
		if(current_state.target.fil != -1){
			int i = 1; 
			while(i < 4){
				if(sensores.terreno.at(i) == 'M' or sensores.terreno.at(i) == 'P' or sensores.agentes.at(i) != '_')
				i++;
			}
			if( i != 4){
				
			}
			
		}
		orientacion_deseada = current_state.brujula_virtual; 
		
		if(current_state.brujula_virtual%2 == 0){
			accion = actTURN_L;
		}else{
			accion = actTURN_SR;
		}
		if(current_state.condiciones.investiga){
			current_state.condiciones.atrapado_muros = true; 
			current_state.condiciones.investiga = false;
		}

		current_state.target = point(-1,-1);
		
		

	}else if((sensores.terreno.at(2) != 'M' and sensores.terreno.at(2) != 'P') and sensores.agentes.at(2) == '_' ){
		 
		if(current_state.target.fil != -1){
			cout << "entra arriba" << endl; 
			accion = Movimiento(sensores.terreno,current_state,mapa_aux.at(current_state.target.fil).at(current_state.target.col)); 
			
		}else if(current_state.condiciones.bateria and cont_bateria > 0){

			point p = current_state.p_virtual;
			cout << "entra" << endl; 
			if(mapa_recorrido.at(p.fil).at(p.col).bateria == 0){
				accion = actIDLE;
			}else{

				Actualizar(p.fil,p.col,current_state.brujula_virtual,1);
				if(mapa_recorrido.at(current_state.p_virtual.fil).at(current_state.p_virtual.col).bateria > mapa_recorrido.at(p.fil).at(p.col).bateria){
					accion = actWALK;
				}else{
					p = current_state.p_virtual;
					int i = 0; 
					int dis = mapa_recorrido.at(p.fil).at(p.col).bateria;
					int min = dis; 
					while (dis <= min and i < 8 ){
						p = current_state.p_virtual;

						Actualizar(p.fil,p.col,static_cast<Orientacion>(i),1);

						if(mapa_recorrido.at(p.fil).at(p.col).bateria >= 0 ){
							min = mapa_recorrido.at(p.fil).at(p.col).bateria;
						}
						
						i++;
					}

					i--;
					 
					accion = CambiaDir(current_state.brujula_virtual,i);
				}
				
			}

		}else{
			bool entra = false; 
			int i = 0;
			while(i <16 and !entra){
				unsigned char c = sensores.terreno.at(i);
				if(c == 'G' and !current_state.condiciones.bien_ubicado){
					accion = Movimiento(sensores.terreno,current_state,'G');
					entra = true; 
					current_state.condiciones.investiga = false; 
					current_state.target = CalculaPunto(current_state.p_virtual,current_state.brujula_virtual,i);

				}else if (c == 'D' and !current_state.condiciones.zapatillas){
					accion = Movimiento(sensores.terreno,current_state,'D');
					entra = true; 
					current_state.condiciones.investiga = false; 
					current_state.target = CalculaPunto(current_state.p_virtual,current_state.brujula_virtual,i);

				}else if(c == 'K' and !current_state.condiciones.bikini) {
					accion = Movimiento(sensores.terreno,current_state,'K');
					entra = true; 
					current_state.condiciones.investiga = false;
					current_state.target = CalculaPunto(current_state.p_virtual,current_state.brujula_virtual,i);

				}else if(c == 'X'){
					point p = CalculaPunto(current_state.p_virtual,current_state.brujula_virtual,i);
					mapa_recorrido.at(p.fil).at(p.col).bateria = 0; 
					Por_actualizar.at(0).push(CalculaPunto(current_state.p_virtual,current_state.brujula_virtual,i));
					cont_bateria++;
				}
				i++;
			}
			if(!entra){
				/*
				if(!current_state.condiciones.investiga and rand()%7 == 0){
					
					Orientacion way = PorExplorar(mapa_aux, '?');

					if(way != current_state.brujula_virtual){
						accion = actWALK;
					}

					current_state.condiciones.investiga = true; 
					current_state.way = way;
					accion = CambiaDir(current_state.brujula_virtual,way);
					

				}else 
				
				if(current_state.condiciones.investiga) {
					point frente = current_state.p_virtual;

					if(sensores.nivel == 3 ){
						Actualizar(frente, current_state.brujula_virtual, 2);
					}else{
						Actualizar(frente, current_state.brujula_virtual,4);
					}

					if(mapa_aux.at(frente.fil).at(frente.col) == '?'){
						current_state.condiciones.investiga = false; 
					}

					accion = Andar_Correr(sensores.agentes);



				}else {
					point frente = current_state.p_virtual;

					if(sensores.nivel == 3 ){
						Actualizar(frente, current_state.brujula_virtual, 2);
					}else{
						Actualizar(frente, current_state.brujula_virtual,4);
					}

					
					if(mapa_aux.at(frente.fil).at(frente.col) == '?' ){
						accion = Andar_Correr(sensores.agentes);
					}else{
						point p = Mas_Cercana(); 
						if(p != point(-1,-1)){

							Action act= CambiaDir(current_state.brujula_virtual,CalOrientacion(current_state.p_virtual, p));  
							if(act == actIDLE){
								accion = actWALK;
							}else{
								accion = act;
							} 

						}else{
							
							Orientacion way = PorExplorar(mapa_aux, '?');

							if(way != current_state.brujula_virtual){
								accion = actWALK;
							}

							current_state.condiciones.investiga = true; 
							current_state.way = way;
							accion = CambiaDir(current_state.brujula_virtual,way);
									
						}
					}
				}
				
			}
		}
		
	}
	
	/*
	point frente = current_state.p_virtual;

	if(sensores.nivel != 3 ){
		Actualizar(frente, current_state.brujula_virtual, 2);
	}else{
		Actualizar(frente, current_state.brujula_virtual,4);
	}

	if(mapa_aux.at(frente.fil).at(frente.col) == '?'){
		accion = Andar_Correr(sensores.agentes);
	}else{
		point p = Mas_Cercana(); 
		if(p == point(-1,-1)){
			current_state.target = p;
		}else{
			
			Orientacion way = PorExplorar(mapa_aux, '?');

			if(way != current_state.brujula_virtual){
				accion = actWALK;
			}

			current_state.condiciones.investiga = true; 
			current_state.way = way;
			accion = CambiaDir(current_state.brujula_virtual,way);
					
		}
	}
	*/



	

	/*



	if(current_state.brujula_virtual != static_cast<Orientacion>(current_state.way)){
		current_state.way = -1;
	}
	

	if(current_state.way != -1 and current_state.brujula_virtual != static_cast<Orientacion>(current_state.way) ){
		accion = CambiaDir(current_state.brujula_virtual, static_cast<Orientacion>(current_state.way));
	}else if(sensores.terreno.at(2) == 'M' or sensores.terreno.at(2) == 'P' or sensores.agentes.at(2) != '_'){
		
		if(he_acabado){
			point p = Hay_cerca();
			if( p == point(-1,-1)){
				orientacion_deseada = current_state.brujula_virtual;

				origen_huida = BuscarZonaIdonea();

				Por_actualizar.at(3).push(origen_huida);
				ActualizacionGlobal();
			}else{
				//Buscar puntos no explorados cercanos.
			}
			
		}else{
			//Buscar lo el punto mas cercano sin peligro, evitar lobos, sortear obstaculos, etc..

		}

	}else{
		if(current_state.condiciones.investiga) {

			if(he_acabado){

				point frente = current_state.p_virtual;

				if(sensores.nivel == 3 ){
					Actualizar(frente, current_state.brujula_virtual, 2);
				}else{
					Actualizar(frente, current_state.brujula_virtual,4);
				}

				if(mapa_aux.at(frente.fil).at(frente.col) == '?'){
					current_state.condiciones.investiga = false; 
				}

				accion = Andar_Correr(sensores.agentes);

			}else{
				vector<point> v; 
				point p;
				vector<bool> disponible;
				for(int i = 0; i < 16; i++){
					disponible.push_back(true);
				}
				unsigned char c; 
				for(int i = 0; i < 8; i++){
					p = current_state.p_virtual;
					Actualizar(p,(current_state.brujula_virtual + i)%8, 1);
					v.push_back(p);
					c = mapa_aux.at(v.at(i).fil).at(v.at(i).col);
					if( c == 'M' or c == 'P'){
						disponible.at(i) = disponible.at(i + 8) = false;

					}else{
						if(i == 0){
							if(sensores.agentes.at(2) != '_'){
								disponible.at(i) = false; 
							}
						}else if( i == 7){
							if(sensores.agentes.at(1) == '_'){
								disponible.at(i) = false;
							}

						}else if( i == 1){
							if(sensores.agentes.at(3) == '_'){
								disponible.at(i) = false;
							}
						}
					}
				}
				for(int i = 8; i < 16; i++){
					p = current_state.p_virtual;
					Actualizar(p,(current_state.brujula_virtual + i)%8, 2);
					v.push_back(p);
					c = mapa_aux.at(v.at(i).fil).at(v.at(i).col);
					if( c == 'M' or c == 'P'){
						disponible.at(i) = false;

					}
				}

				int max = 0;
				int index = -1;
				double valor = 0;
				double bateria = 0;
				double explora = 0;
				
				

				for(int i = 0; i < 16; i++){
					
					if(disponible.at(i)){

						bateria = CosteManiobra(i%8);
						bateria += CosteMedioCasilla(mapa_aux.at(v.at(i).fil).at(v.at(i).col));

						if(i >= 8){
							bateria += CBateria(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col),1,0);
						

						}else{
							bateria += CBateria(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col),0,0);
							bateria += CBateria(mapa_aux.at(v.at(i).fil).at(v.at(i).col),0,0);

						}

						explora = Explorara(v.at(i), static_cast<Orientacion>((current_state.brujula_virtual + i)%8), sensores);

						bateria = (1 - bateria/322);
						explora = (explora/16);

						valor = pow(bateria,2) + pow(explora,2);

						if(valor > max){
							max = valor;
							index = i; 
						}

					}
				}


				if(index == 0){
					accion = actWALK;
				}else if (index == 8){
					accion = actRUN;
				}else{
					accion = CambiaDir(current_state.brujula_virtual, (current_state.brujula_virtual + index)%8 );
				}




				

				
				

				
			}

			



		}else {
			point frente = current_state.p_virtual;

			if(sensores.nivel == 3 ){
				Actualizar(frente, current_state.brujula_virtual, 2);
			}else{
				Actualizar(frente, current_state.brujula_virtual,4);
			}

			
			if(mapa_aux.at(frente.fil).at(frente.col) == '?' ){
				accion = Andar_Correr(sensores.agentes);
			}else{
				point p = Mas_Cercana(); 
				if(p != point(-1,-1)){

					Action act= CambiaDir(current_state.brujula_virtual,CalOrientacion(current_state.p_virtual, p));  
					if(act == actIDLE){
						accion = actWALK;
					}else{
						accion = act;
					} 

				}else{
					
					Orientacion way = PorExplorar(mapa_aux, '?');

					if(way != current_state.brujula_virtual){
						accion = actWALK;
					}

					current_state.condiciones.investiga = true; 
					current_state.way = way;
					accion = CambiaDir(current_state.brujula_virtual,way);
							
				}
			}
		}
	
	}


	



	
		
	*/
	/*
	//Gestion de muros:
	
	if((sensores.terreno.at(2) == 'M' or sensores.terreno.at(2) == 'P') and sensores.agentes.at(2) != '_' ){
		
	}
	*/

		last_action = accion;
	
		return accion;
	
		
		
} 
	
	


int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}