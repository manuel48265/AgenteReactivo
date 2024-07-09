#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include<queue>
#include<math.h>
#include<set>
#include<tgmath.h>
#include <utility>
#include<iomanip>

using namespace std;



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

Action ComportamientoJugador::Movimiento(const vector<unsigned char> &v, const state &st, unsigned char c){
	Action action; 

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

	return tmp;




}



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
						Por_actualizar.at(3).push(q);
					}
				}
			}

		}

		if(mapa_recorrido.at(p.fil).at(p.col).huida == 0){
			for(int i = 0; i < 8; i++){
				q = p; 
				Actualizar(q.fil,q.col,static_cast<Orientacion>(i),1);
				if(mapa_aux.at(q.fil).at(q.col) != '?' and mapa_aux.at(q.fil).at(q.col) != 'M' and mapa_aux.at(q.fil).at(q.col) != 'P'){
					if(mapa_recorrido.at(q.fil).at(q.col).huida > min + 1 or mapa_recorrido.at(q.fil).at(q.col).huida < 0){
						Por_actualizar.at(3).push(q);
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

		if(mapa_recorrido.at(p.fil).at(p.col).bateria == 0){
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

			if(mapa_real.at(pos_real.fil).at(pos_real.col) == 'X'){
				aux.at(pos_real.fil).at(pos_real.col).bateria = 0; 
			}

			


		}

	}

	mapa_recorrido = aux;

}

int ComportamientoJugador::CosteManiobra(int i){
	int coste = CBateria(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col),0,1);
	const int CONST_GIRO = 10;
	int agregado = 0; 
	switch(i){
		case 0:
			coste = 0; 
			break;
		case 6:
			if(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col) == 'B' and !current_state.condiciones.zapatillas){
				agregado = agregado + 10;
			}
		case 1:
			coste = coste + CONST_GIRO;
			
			break;
		case 4:
			if(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col) == 'B' and !current_state.condiciones.zapatillas){
				agregado = agregado + 10; 
			}
		case 7: 
			if(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col) == 'B' and !current_state.condiciones.zapatillas){
				agregado = agregado + 10;
			}
		case 2:
			coste = 2*coste + 2*CONST_GIRO;
			break;
		case 5:
			if(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col) == 'B' and !current_state.condiciones.zapatillas){
				agregado = agregado + 20;
			}
		case 3:
			coste = 3*coste + 3*CONST_GIRO;
			break;
		default:
			coste = 0;
			break; 
	}

	coste = coste + agregado; 

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

point ComportamientoJugador::casillaHuida(){
	
	point p = point(-1,-1);
	point pivote = point(0,0);
	double suma = 0; 
	double max = 0;
	bool entrar = false; 
	double multiplicador = 0; 
	const int CONST_MULT = 60;

	for(int i = 0; i < mapa_aux.size(); i++){
		for(int j = 0; j < mapa_aux.size(); j++){
			if(mapa_recorrido.at(i).at(j).dis_origen != -1){
				suma = 0;
				/*
				suma = mapa_recorrido.at(i).at(j).hijos; 
				for(int k  = 0; k < 8; k++){
					pivote = point(i,j);
					Actualizar(pivote, static_cast<Orientacion>(i), 1);
					suma += mapa_recorrido.at(pivote.fil).at(pivote.col).hijos; 
				}
				*/

				entrar = false; 

				for(int k = -1; k < 2 and !entrar; k++){
					for(int l = -1; l < 2 and !entrar; l++){
						if(mapa_aux.at((i+k + 4*tam)%tam).at((j + l + 4*tam)%tam) == '?'){
							entrar = true;
						}
					}
				}

				if(entrar){
					for(int k = -3; k < 4; k++){
						for(int l = -3; l < 4; l++){
							suma += mapa_recorrido.at((i+k + 4*tam)%tam).at((j + l + 4*tam)%tam).hijos;
						}
					}
					multiplicador = current_state.p_virtual.dis(point(i,j),tam); 

					if(multiplicador > CONST_MULT){
						multiplicador = 1; 
					}else{
						multiplicador = 2 - multiplicador/CONST_MULT;
					}

					suma *= multiplicador; 
					
				}


				if( suma > max ){
					p = point(i,j);
					max = suma;
				}
			}
		}
	}

	if(p == point(-1,-1)){
		cout << " ⛏💎⛏💎⛏💎⛏💎⛏💎 He acabado ⛏💎⛏💎⛏💎⛏💎⛏💎 " << endl; 
		mapa_100 = true;
		p = point(3,3);
	}

	return p;
}

double ComportamientoJugador::ExploraHuida(point origen, point destino){

	double explora = 0;
	if(mapa_recorrido.at(destino.fil).at(destino.col).huida != -1){
		explora = mapa_recorrido.at(origen.fil).at(origen.col).huida + 2; 
		explora = explora - mapa_recorrido.at(destino.fil).at(destino.col).huida; 
		
	}else{
		explora = 0;
	}


	//normalizo el resultado;
	explora = explora/5;

	return explora;
}

void ComportamientoJugador::ReseteoHuida(){
	for(int i = 0; i < mapa_recorrido.size(); i++){
		for(int j = 0; j < mapa_recorrido.size(); j++){
			mapa_recorrido.at(i).at(j).huida = -1;
 		}
	}

	point p = casillaHuida();

	mapa_recorrido.at(p.fil).at(p.col).huida = 0;

	Por_actualizar.at(3).push(p);

	ActualizacionGlobal();

}




Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;
	
	/*
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
	*/

	

	//std::this_thread::sleep_for(std::chrono::seconds(3));

	//Modificar variables

	if(sensores.colision == false){
		vector<bool> aux_bool;
		switch (last_action)
		{
		case actWALK:
			Actualizar(current_state.p_virtual.fil,current_state.p_virtual.col,current_state.brujula_virtual,1);
			for(int i = 0; i < 16; i++){
        		no_agentes.at(i) = true;
      		}
			accion_vacia = 0;
			break;
		case actRUN:
			Actualizar(current_state.p_virtual.fil,current_state.p_virtual.col,current_state.brujula_virtual,2);
			for(int i = 0; i < 16; i++){
        		no_agentes.at(i) = true;
      		}
			accion_vacia = 0;
			break;

		case actTURN_L:
			current_state.brujula_virtual = static_cast<Orientacion>((current_state.brujula_virtual+6)%8);
			aux_bool = no_agentes;

			for(int i = 0; i<8; i++){
				no_agentes.at((i+2)%8) = aux_bool.at(i);
			}
			for(int i = 8; i < 16; i++){
				no_agentes.at(8+(i+2)%8) = aux_bool.at(i);
			}
			accion_vacia++;

			break;

		case actTURN_SR:
			current_state.brujula_virtual = static_cast<Orientacion>((current_state.brujula_virtual+1)%8);
			aux_bool = no_agentes;

			for(int i = 0; i<8; i++){
				no_agentes.at((i+7)%8) = aux_bool.at(i);
			}
			for(int i = 8; i < 16; i++){
				no_agentes.at(8+(i+7)%8) = aux_bool.at(i);
			}
			accion_vacia++;
			break;
		
		default:
			accion_vacia++;
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
		ActulizarMapa(mapa_aux,mapaResultado,current_state);
		mapa_aux = vector<vector<unsigned char>>(mapaResultado);
		current_state.target = point(-1,-1);
		mapa_recorrido.at(0).at(0).dis_origen = -1;
		mapa_recorrido.at(current_state.p_virtual.fil).at(current_state.p_virtual.col).dis_origen = 0;
		cont_bateria = 0;
		for(int i = 0; i < mapa_aux.size(); i++){
			for(int j = 0; j < mapa_aux.size(); j++){
				if(mapa_aux.at(i).at(j) == 'X'){
					cont_bateria++;
				} 
			}
		}

	}

	if(accion_vacia > 6){
		for(int i = 0; i < 16; i++){
        	no_agentes.at(i) = true;
      	}
		accion_vacia = 0; 
	}


	ActualizacionGlobal();

	if(mapa_recorrido.at(current_state.p_virtual.fil).at(current_state.p_virtual.col).huida < 2 ){
		huida = false; 
	}



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
		cont_bateria = 0;
		for(int i = 0; i < mapa_aux.size(); i++){
			for(int j = 0; j < mapa_aux.size(); j++){
				if(mapa_aux.at(i).at(j) == 'X'){
					cont_bateria++;
				} 
			}
		}

	}else if (sensores.terreno.at(0) == 'D' and !current_state.condiciones.zapatillas ){
		current_state.condiciones.zapatillas = true;
		current_state.target = point(-1,-1);

	}else if(sensores.terreno.at(0) == 'K' and !current_state.condiciones.bikini){
		current_state.condiciones.bikini = true;
		current_state.target = point(-1,-1);

	}

	if (sensores.bateria < 2000 and sensores.vida > 500){
		current_state.condiciones.bateria = true;
		current_state.condiciones.investiga = false; 

	}
	if (sensores.bateria > 4500 or sensores.vida < 500){
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

	if(mapa_100){
		cout << " ⛏💎⛏💎⛏💎⛏💎⛏💎 He acabado ⛏💎⛏💎⛏💎⛏💎⛏💎 " << endl; 
		last_action = actIDLE;
		return actIDLE;
	}

	ActualizacionGlobal();	

	bool entra = false; 
	bool primero = false;
	bool no_mover = true;

	if((sensores.terreno.at(2) != 'M' and sensores.terreno.at(2) != 'P') and sensores.agentes.at(2) == '_' ){
		 
		if(current_state.target.fil != -1){
			vector<unsigned char> v; 
			
			ExtraeTerreno(current_state.p_virtual, current_state.brujula_virtual, sensores, v);
			
			
			accion = Movimiento(v,current_state,mapa_aux.at(current_state.target.fil).at(current_state.target.col)); 
			primero = true; 
			if(accion == actIDLE){
				no_mover = true;
			}else{
				no_mover = false;
			}

			
		}
		
	}

	int i = 0;
	while(i <16){
		unsigned char c = sensores.terreno.at(i);
		if(c == 'X'){
			point p = CalculaPunto(current_state.p_virtual,current_state.brujula_virtual,i);
			mapa_recorrido.at(p.fil).at(p.col).bateria = 0; 
			Por_actualizar.at(0).push(CalculaPunto(current_state.p_virtual,current_state.brujula_virtual,i));
			cont_bateria++;
		}
		i++;
	}

	i = 0;
	while(i <16 and !entra and no_mover){
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

		}
		i++;
	}

	

	if(entra or primero){
		if(accion == actIDLE){
			entra = false;
			primero = false;
			current_state.target = point(-1,-1); 
		}
	}
	/*
	for(int i = 0; i < mapaResultado.size(); i++){
		for(int j = 0; j< mapaResultado.size(); j++){
			cout << mapa_aux.at(i).at(j);
		}
		cout << endl; 
	}
	*/

	if(!entra and !primero){
		vector<point> v; 
		point p;
		vector<bool> disponible;
		for(int i = 0; i < 16; i++){
			disponible.push_back(true);
		}
		unsigned char c;

		if(sensores.agentes.at(2) == 'a' ){
			disponible.at(0) = disponible.at(8) = false; 
			no_agentes.at(0) = no_agentes.at(8) = false;

		}
		if(sensores.agentes.at(1) == 'a' ){
			disponible.at(7) = disponible.at(15) = false; 
			no_agentes.at(7) = no_agentes.at(15) = false;
			
		}
		if(sensores.agentes.at(3) == 'a'){
			disponible.at(1) = disponible.at(9) = false; 
			no_agentes.at(1) = no_agentes.at(9) = false;
			
		}

		if(sensores.agentes.at(8) == 'a'){
			disponible.at(9) = false; 
			no_agentes.at(9) = false;
		}

		if( sensores.agentes.at(4) == 'a'){
			disponible.at(15) = false; 
			no_agentes.at(15) = false;
		}

		if(sensores.agentes.at(6) == 'a'){
			disponible.at(8) = false; 
			no_agentes.at(8) = false;
		}

		if(sensores.agentes.at(2) == 'l' or  sensores.agentes.at(6) == 'l' or sensores.agentes.at(12) == 'l'){
			disponible.at(0) = disponible.at(8) = false; 
			no_agentes.at(0) = no_agentes.at(8) = false;

		}
		if(sensores.agentes.at(1) == 'l' or  sensores.agentes.at(4) == 'l' or sensores.agentes.at(9) == 'l'){
			disponible.at(7) = disponible.at(15) = false; 
			no_agentes.at(7) = no_agentes.at(15) = false;
			
		}
		if(sensores.agentes.at(3) == 'l' or  sensores.agentes.at(8) == 'l' or sensores.agentes.at(15) == 'l'){
			disponible.at(1) = disponible.at(9) = false; 
			no_agentes.at(1) = no_agentes.at(9) = false;
			
		}
		if(sensores.agentes.at(11) == 'l' or  sensores.agentes.at(13) == 'l'){
			disponible.at(0) = disponible.at(8) = false; 
			no_agentes.at(0) = no_agentes.at(8) = false;
			
		}
		if(sensores.agentes.at(5) == 'l' or  sensores.agentes.at(7) == 'l'){
			disponible.at(0) = disponible.at(8) = false; 
			no_agentes.at(0) = no_agentes.at(8) = false;
			disponible.at(1) = disponible.at(9) = false; 
			no_agentes.at(1) = no_agentes.at(9) = false;
			disponible.at(7) = disponible.at(15) = false; 
			no_agentes.at(7) = no_agentes.at(15) = false;
		}
		if(sensores.agentes.at(10) == 'l'){
			disponible.at(15) = false; 
			no_agentes.at(15) = false;
			
		}
		if(sensores.agentes.at(14) == 'l' ){
			disponible.at(9) = false; 
			no_agentes.at(9) = false;
			
		}
		if(sensores.agentes.at(3) == 'l' ){
			disponible.at(0)  = false; 
			no_agentes.at(0) = false;
			
		}
		if(sensores.agentes.at(1) == 'l' ){
			disponible.at(0) = false; 
			no_agentes.at(0)  = false;
			
		}

		for(int i = 0; i < 8; i++){
			p = current_state.p_virtual;
			Actualizar(p,static_cast<Orientacion>((current_state.brujula_virtual + i)%8), 1);
			v.push_back(p);
			c = mapa_aux.at(v.at(i).fil).at(v.at(i).col);
			if( c == 'M' or c == 'P'){
				disponible.at(i) = disponible.at(i + 8) = false;
			}
		}

		
		
		for(int i = 8; i < 16; i++){
			p = current_state.p_virtual;
			Actualizar(p,static_cast<Orientacion>((current_state.brujula_virtual + i)%8), 2);
			v.push_back(p);
			c = mapa_aux.at(v.at(i).fil).at(v.at(i).col);
			if( c == 'M' or c == 'P'){
				disponible.at(i) = false;
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

		int entradas_bucle = 0; 
		int entradas_huida = 0;

		const int FREC_COST = 40; 
		const int EXPLORA_3 = 6;
		const int EXPLORA_N = 8; 


		if(cont_bateria > 0 and current_state.condiciones.bateria and mapa_recorrido.at(current_state.p_virtual.fil).at(current_state.p_virtual.col).bateria != -1){
			max = 0;
			for(int i = 0; i < 16; i++){
			
				if(disponible.at(i) and no_agentes.at(i)){

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
					
					
					
					if(valor > max ){
						max = valor;
						index = i; 
					}

				}
			}
		}else{

			for(int i = 0; i < 16; i++){
			
				if(disponible.at(i) and no_agentes.at(i)){

					entradas_bucle++;

					bateria = CosteManiobra(i%8);
					bateria += CosteMedioCasilla(mapa_aux.at(v.at(i).fil).at(v.at(i).col));

					if(i >= 8){
						bateria += CBateria(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col),1,0);
					

					}else{
						bateria += CBateria(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col),0,0);
						bateria += CBateria(mapa_aux.at(v.at(i).fil).at(v.at(i).col),0,0);

					}

					explora = Explorara(v.at(i), static_cast<Orientacion>((current_state.brujula_virtual + i)%8), sensores);
					point p_aux_1 = v.at(i);
					point p_aux_2 = v.at(i);
					Actualizar(p_aux_2, static_cast<Orientacion>((current_state.brujula_virtual + i)%8),2);
					Actualizar(p_aux_1, static_cast<Orientacion>((current_state.brujula_virtual + i)%8),1);

					unsigned char aux_1 = mapa_aux.at(p_aux_1.fil).at(p_aux_1.col);
					unsigned char aux_2 = mapa_aux.at(p_aux_2.fil).at(p_aux_2.col);
					if(aux_1 != 'P' and aux_1 != 'M' and aux_2 != 'P' and aux_2 != 'M'){
						explora += Explorara(p_aux_2, static_cast<Orientacion>((current_state.brujula_virtual + i)%8), sensores);
					}
					
					if(sensores.nivel == 3){
						if(explora > EXPLORA_3){
							huida = false;
						}
					}else{
						if(explora > EXPLORA_N){
							huida = false;
						}

					}
					
					

					if(bateria > 400){
						valor = 0;
					}else{
						bateria = (1 - bateria/400);
						if(sensores.nivel == 3){
							explora = (explora/27);
						}else{
							explora = (explora/36);
						}
						
						valor = pow(bateria,2) + pow(explora,2);
					}

					if(explora == 0){
						entradas_huida++;
					}

					

					double disminuir = 0;

					if(frecuencias.at(v.at(i).fil).at(v.at(i).col) > FREC_COST ){
						valor = 0;
					}else{
						disminuir = 1 - (1.0*frecuencias.at(v.at(i).fil).at(v.at(i).col))/FREC_COST;
						valor = valor*(disminuir);
					}
					
					
					if(valor > max ){
						max = valor;
						index = i; 
					}

				}
			}

			if(entradas_huida == entradas_bucle and !huida){
				huida = true;
				ReseteoHuida();
				max = 0;

			}

			if(huida){
				for(int i = 0; i < 16; i++){
				
					if(disponible.at(i) and no_agentes.at(i)){

						bateria = CosteManiobra(i%8);
						bateria += CosteMedioCasilla(mapa_aux.at(v.at(i).fil).at(v.at(i).col));

						if(i >= 8){
							bateria += CBateria(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col),1,0);
						

						}else{
							bateria += CBateria(mapa_aux.at(current_state.p_virtual.fil).at(current_state.p_virtual.col),0,0);
							bateria += CBateria(mapa_aux.at(v.at(i).fil).at(v.at(i).col),0,0);

						}

						explora = ExploraHuida(current_state.p_virtual,v.at(i));

						if(bateria > 400){
							valor = 0;
						}else{
							bateria = (1 - bateria/400);
							valor = pow(bateria,2) + pow(explora,2);
						}

						if(frecuencias.at(v.at(i).fil).at(v.at(i).col) > FREC_COST){
							valor = 0;
						}else{
							double disminuir = 1 - (1.0*frecuencias.at(v.at(i).fil).at(v.at(i).col))/FREC_COST;
							valor = valor*(disminuir);
						}
						
						
						if(valor > max ){
							max = valor;
							index = i; 
						}

					}
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

		last_action = accion;
	
		return accion;
	
		
		
} 
	
	


int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}