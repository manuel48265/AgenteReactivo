#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include<queue>
#include<math.h>
#include<set>
#include<tgmath.h>
#include<list>
#include<stack>


using namespace std;

int CBateria(unsigned char v, bool zapatillas, bool bikini, bool correr, bool girar){
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

int ValCasilla(unsigned char v, const vector<bool> &condiciones){
	int coste;
  switch (v)
  {
  	case 'A':
		if(condiciones.at(2)){
			coste = 460;
		}else{
			coste = 25;
		}
    break;
  
  	case 'B':
    
		if(condiciones.at(3)){
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
	coste = 800;

	case 'X': 
	if(condiciones.at(3)){
		coste = 600;
	}else{
		coste = 50000;
	}

	case 'D':
	if(condiciones.at(1)){
		coste = 600;
	}else{
		coste = 50000;
	}
	case 'K':
	if(condiciones.at(2)){
		coste = 600;
	}else{
		coste = 50000;
	}
	case 'G':
	if(condiciones.at(0)){
		coste = 600;
	}else{
		coste = 50000;
	}
	case 'S':
	coste = 600;
	case 'a':
	coste = -200;
	case 'l':
	coste = -3000;


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

bool rutina::operator<(const rutina  &otro) const {
  return prioridad < otro.prioridad;
}


bool casilla::operator< (const casilla& otro)const{
  return (this->p < otro.p);
}

bool casilla::comparaPunteros (const casilla*& uno, const casilla*& otro){
    return ((*uno) < (*otro));
}

int Ajuste(int i, int adjust){
	cout << "aqui7" << endl;
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

void Actualizar(int &x,int &y, const Orientacion &brujula){
	cout << "aqui5" << endl;
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

}

Action CambiaDir(int init, int there){
	cout << "aqui4" << endl;

	cout << init << endl;
	cout << there << endl;

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




Orientacion CalOrientacion(point a, point b, int tam){
	cout << "aqui2" << endl;
	int x = (b.col - a.col + tam/2)%tam;
	int y = (b.fil - a.fil + tam/2)%tam;
	Orientacion tmp;

	cout << "valorx" << x << endl;
	cout << "valory" << y << endl;


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

	return tmp;




}

point MasVacio(const vector<vector<unsigned char>> &matriz){
	cout << "aqui" << endl;
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
	for(int i = 3; i< matriz.size()-3; i++){
		for(int j=3; j<matriz.size()-3; j++){
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

void RutinaBasica(const vector<unsigned char> & terreno, Action &accion, unsigned int &algoritmo){
	cout << "aqui11" << endl;
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

void PonerTerrenoEnMatriz(const vector<unsigned char> & terreno, const state &st, vector<vector<casilla>> &matriz){

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
	int tam = matriz.size();

    if(st.brujula_virtual%2 == 0){
        for (int j = 0; j <= 3; j++){
            for(int i = -j; i <= j; i++){
                matriz.at((st.p_virtual.fil + tam +i*y +j*x)%tam).at((st.p_virtual.col +tam + -i*x +j*y)%tam).valor = terreno.at(j*(j+1)+i);
            }
        }
    }
    if(st.brujula_virtual%2 == 1){
        for (int j = 0; j <= 3; j++){

            for(int i = -j; i < 0; i++){
                matriz.at((st.p_virtual.fil + tam +j*x)%tam).at((st.p_virtual.col + tam + i*y +j*y)%tam).valor = terreno.at(j*(j+1)+i*(-1)*x*y);
            }

            for(int i = 0; i <= j; i++){
                matriz.at((st.p_virtual.fil + tam -i*x +j*x)%tam).at((st.p_virtual.col + tam+j*y)%tam).valor = terreno.at(j*(j+1)+i*(-1)*x*y);
            }
        }
    }


}

void ActulizarMapa(vector<vector<casilla>> &mapa_virtual, vector<vector<unsigned char>> &mapa_real, const state &st){
	
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
			if((mapa_real.at((st.p_real.fil+i)%tam).at((st.p_real.col+j)%tam) == '?') and (mapa_virtual.at((st.p_virtual.fil+tam+i*x+j*y)%tam).at((st.p_virtual.col+tam+i*w+j*z)%tam).valor != '?') ){
				mapa_real.at((st.p_real.fil+i)%tam).at((st.p_real.col+j)%tam) = mapa_virtual.at((st.p_virtual.fil+tam+i*x+j*y)%tam).at((st.p_virtual.col+tam+i*w+j*z)%tam).valor;
			}else if((mapa_virtual.at((st.p_virtual.fil+tam+i*x+j*y)%tam).at((st.p_virtual.col+tam+i*w+j*z)%tam).valor == '?') and (mapa_real.at((st.p_real.fil+i)%tam).at((st.p_real.col+j)%tam) != '?')){
				mapa_virtual.at((st.p_virtual.fil+tam+i*x+j*y)%tam).at((st.p_virtual.col+tam+i*w+j*z)%tam).valor = mapa_real.at((st.p_real.fil+i)%tam).at((st.p_real.col+j)%tam);
			}
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

	//Modificar variables
	switch (last_action)
	{
	case actWALK:
		Actualizar(current_state.p_virtual.fil,current_state.p_virtual.col,current_state.brujula_virtual);
			break;
	case actRUN:

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
		current_state.condiciones.at(0) = true;
	}else if (sensores.terreno.at(0) == 'D'){
		current_state.condiciones.at(1) = true;
	}else if(sensores.terreno.at(0) == 'K'){
		current_state.condiciones.at(2) = true;
	}
	if (sensores.bateria < 1000){
		current_state.condiciones.at(3) = true;
	}

	if(current_state.p_virtual == current_state.target){
		current_state.target = point(rand()%100,rand()%100);
	}
	
	if (current_state.condiciones.at(0)){
		current_state.p_real.fil = sensores.posF;
		current_state.p_real.col = sensores.posC;
		current_state.brujula_real = sensores.sentido;
		ActulizarMapa(mapa_aux,mapaResultado,current_state);
	}

	//Toma de decisiones
	point max_run,max_walk,max_idle;
	int val_run, val_walk,val_idle;
	int tam = mapaResultado.size();

	cout << tam << endl;

	max_run = max_walk = max_idle  = current_state.p_real;
	cout << "hola1" << endl; 
	for(int i = -2; i <= 2 ;i++){
		for(int j = -2; j<= 2; i++){
			cout << (current_state.p_virtual.fil+i + tam)%tam << endl;
			cout << mapa_aux.at((current_state.p_virtual.fil+i + tam)%tam).at((current_state.p_virtual.col+j + tam)%tam).valor << endl;
			mapa_aux.at((current_state.p_virtual.fil+i + tam)%tam).at((current_state.p_virtual.col+j + tam)%tam).CalculaValoracion(current_state,mapa_aux);
		}

	}
	cout << "hola2" << endl;
	casilla c;
	for(int i = -2; i <= 2 ;i++){
		for(int j = -2; j<= 2; i++){
			switch (current_state.p_virtual.dis(point((current_state.p_virtual.fil+i + tam)%tam,(current_state.p_virtual.col+j + tam)%tam),tam))
			{
			case 0:
				c = mapa_aux.at((current_state.p_virtual.fil+i + tam)%tam).at((current_state.p_virtual.col+j + tam)%tam);
				val_idle = c.valoracion - 200;
				max_idle = c.p ;
			break;
			case 1:
				c = mapa_aux.at((current_state.p_virtual.fil+i + tam)%tam).at((current_state.p_virtual.col+j + tam)%tam);
				c.valoracion -= 2*CBateria(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col).valor,current_state.condiciones.at(2),current_state.condiciones.at(3),false, false);
				if(val_walk < c.valoracion ){
					val_walk = c.valoracion;
					max_walk = c.p ;
				}
			case 2: 
				c = mapa_aux.at((current_state.p_virtual.fil+i + tam)%tam).at((current_state.p_virtual.col+j + tam)%tam);
				c.valoracion -= 2*CBateria(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col).valor,current_state.condiciones.at(2),current_state.condiciones.at(3),true, false);
				if(val_run < c.valoracion){
					val_run = c.valoracion;
					max_run = c.p ;
				}

			break;
			default:
			break;
			}
			
		}
	}
	cout << "hola3" << endl;

	if(val_idle > val_walk){
		if(val_idle > val_run){
			accion = actIDLE;
		}else{
			Action act = CambiaDir(current_state.brujula_virtual, CalOrientacion(current_state.p_virtual,max_run,mapa_aux.size()));
			if( act == actIDLE) {
				accion = actRUN;
			}
		}
	}else{
		if(val_walk > val_run){
			Action act = CambiaDir(current_state.brujula_virtual, CalOrientacion(current_state.p_virtual,max_walk,mapa_aux.size()));
			if( act == actIDLE) {
				accion = actWALK;
			}
		}else{
			Action act = CambiaDir(current_state.brujula_virtual, CalOrientacion(current_state.p_virtual,max_run,mapa_aux.size()));
			if( act == actIDLE) {
				accion = actRUN;
			}
		}
	}
	cout << "hola4" << endl;
	


	return accion;
};

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}