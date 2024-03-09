#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include<queue>
#include<math.h>
#include<tgmath.h>

using namespace std;



struct point{
	int fil;
	int col;
	point(int i,int j)
	{
		fil = i;
		col = j;
	}
	bool operator==(point &p){
		return (fil == p.fil) and (col == p.col);
	}
	bool operator!=(point &p){
		return !(*this==p);
	}
};

int Ajuste(int i){
	cout << "aqui7" << endl;
	if(i >= 0 and i <= 99){
		return i;
	}else if(i > 99){
		return 99;
	}else {
		return 0;
	}
	
}

void CalculoCasilla(int i,const state &st, const vector<vector<unsigned char>> &matriz, bool orden, point &p, bool &no_vacio){
	cout << "aqui6" << endl;
	int j = -4;
	while (no_vacio and orden){
		while(no_vacio and j <= 4){
			if(matriz.at(Ajuste(st.fil+i)).at(Ajuste(st.col + j)) == '?'){
				no_vacio = false; 
				p.fil = Ajuste(st.fil+i);
				p.col = Ajuste(st.col + j);

			}
			j++;
		}
	}
	while (no_vacio and !orden){
		while(no_vacio and j <= 4){
			if(matriz.at(Ajuste(st.fil+j)).at(Ajuste(st.col + i)) == '?'){
				no_vacio = false; 
				p.fil = Ajuste(st.fil + j);
				p.col = Ajuste(st.col + i);

			}
			j++;
		}
	}

}

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

void CambiaDir(int init, priority_queue<rutina> &pq, int there){
	cout << "aqui4" << endl;

	if(init == there){
	}
	else if((there-init+8)%8 <= 3){
		pq.push(rutina(actTURN_SR,30 - pq.size()));
		CambiaDir((init+1)%7,pq,there);
	}
	else{
		pq.push(rutina(actTURN_L,30 - pq.size()));
		CambiaDir((init+6)%8,pq,there);
	}
}

point ExploracionLocal(const state &st, const vector<vector<unsigned char>> &matriz){
	cout << "aqui3" << endl;
	int i = -4;

	bool no_vacio = true;

	point p = point(st.fil, st.col);

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

	return p;
}




Orientacion CalOrientacion(point a, point b){
	cout << "aqui2" << endl;
	int x = b.fil - a.fil;
	int y = b.col - a.col;
	Orientacion tmp;

	if(x = 0){
		if (y > 0){
			tmp = Orientacion::norte;
		}else{
			tmp = Orientacion::sur;
		}
	}
	else if(y = 0){
		if (x > 0){
			tmp = Orientacion::este;
		}else{
			tmp = Orientacion::oeste;
		}
	}else{
		double z = 1.0*y/x;
		if (z > 0){
			if(x > 0){
				if(z > sqrt(3)){
					tmp = Orientacion::norte;

				} else if(z < 1/sqrt(3)){
					tmp = Orientacion::este;
				}else{
					tmp = Orientacion::noreste; 
				}

			}else {
				if(z > sqrt(3)){
					tmp = Orientacion::sur;

				} else if(z < 1/sqrt(3)){
					tmp = Orientacion::oeste;

				}else{
					tmp = Orientacion::suroeste;
				}
			}
		}else{
			if(x > 0){
				if(z > sqrt(3)){
					tmp = Orientacion::sur;

				} else if(z < 1/sqrt(3)){
					tmp = Orientacion::este;
				}else{
					tmp = Orientacion::sureste; 
				}

			}else{
				if(z > sqrt(3)){
					tmp = Orientacion::norte;

				} else if(z < 1/sqrt(3)){
					tmp = Orientacion::oeste;
				}else{
					tmp = Orientacion::noroeste; 
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
	for(int i = 3; i< matriz.size()-3; i++){
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

void NuevaRuta(const state &st, const vector<vector<unsigned char>> &matriz, priority_queue<rutina> &pq){
	cout << "aqui9" << endl;
	point p(st.fil,st.col);
	point q = ExploracionLocal(st,matriz);

	if(p == q){
		q = MasVacio(matriz);
	}

	CambiaDir(st.brujula,pq,CalOrientacion(p,q));

}

void RutinaExploracion(const vector<unsigned char> & terreno, Action &accion, unsigned int &algoritmo, const state &st, const vector<vector<unsigned char>> &matriz, priority_queue<rutina> &pq){
	point p(st.fil,st.col);
	for(int i = 0; i <4; i++){
		Actualizar(p.fil,p.col,st.brujula);
	}
	p.fil = Ajuste(p.fil);
	p.col = Ajuste(p.col);
	
	if((terreno[2] == 'T' or terreno[2] =='S' or terreno[2] == 'G' or terreno[2] == 'B' )
		and (matriz.at(p.fil).at(p.col) == '?')){
			accion = actWALK;
		}
		else if((matriz.at(p.fil).at(p.col) == '?')) {
			accion =actTURN_L;
			algoritmo = 1;
		}else{
			NuevaRuta(st,matriz,pq);
		}
}

void RutinaBasica(const vector<unsigned char> & terreno, Action &accion, unsigned int &algoritmo){
	if((terreno[2] == 'T' or terreno[2] =='S' or terreno[2] == 'G' or terreno[2] == 'B' )){
			accion = actWALK;
			algoritmo = 0;
	}
	else {
		accion =actTURN_L;
		algoritmo = 1;
	}

}

void PonerTerrenoEnMatriz(const vector<unsigned char> & terreno, const state &st, vector<vector<unsigned char>> &matriz){

    int x;
    int y;
    switch (st.brujula)
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

    if(st.brujula%2 == 0){
        for (int j = 0; j <= 3; j++){
            for(int i = -j; i <= j; i++){
                matriz.at(st.fil +i*y +j*x).at(st.col -i*x +j*y) = terreno.at(j*(j+1)+i);
            }
        }
    }
    if(st.brujula%2 == 1){
        for (int j = 0; j <= 3; j++){

            for(int i = -j; i < 0; i++){
                matriz.at(st.fil +j*x).at(st.col + i*y +j*y) = terreno.at(j*(j+1)+i*(-1)*x*y);
            }

            for(int i = 0; i <= j; i++){
                matriz.at(st.fil -i*x +j*x).at(st.col +j*y) = terreno.at(j*(j+1)+i*(-1)*x*y);
            }
        }
    }


};

Action ComportamientoJugador::think(Sensores sensores)
{

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
		Actualizar(current_state.fil,current_state.col,current_state.brujula);
			break;
	case actRUN:

		break;

	case actTURN_L:
		current_state.brujula = static_cast<Orientacion>((current_state.brujula+6)%8);

		break;

	case actTURN_SR:
		current_state.brujula = static_cast<Orientacion>((current_state.brujula+1)%8);
		break;
	
	default:
		break;
	}

	//Pintamos mapa
	
	if (bien_situado){
		current_state.fil = sensores.posF;
		current_state.col = sensores.posC;
		current_state.brujula = sensores.sentido;
		PonerTerrenoEnMatriz(sensores.terreno,current_state,mapaResultado);
	}

	//Toma de decisiones
	if (!prioridad.empty())
	{
		accion = prioridad.top().accion;
		prioridad.pop();
		algoritmo = 50;
		return accion;
	}

	switch (algoritmo)
	{
	case 0:
		RutinaBasica(sensores.terreno,accion,algoritmo);
		if (rand()%8 == 0){
			RutinaExploracion(sensores.terreno,accion,algoritmo,current_state,mapaResultado,prioridad);
		}
		break;
	case 1:
		switch (contador)
		{
		case 0:
			if((sensores.terreno[3] == 'T' or sensores.terreno[3] =='S' or sensores.terreno[3] == 'G' or sensores.terreno[3] == 'B' ) and
			(sensores.terreno[2] == 'T' or sensores.terreno[2] =='S' or sensores.terreno[2] == 'G' or sensores.terreno[2] == 'B' )){
				contador = 1;
				accion = actWALK;
			}else {
				RutinaBasica(sensores.terreno,accion,algoritmo);
				if (rand()%8 == 0){
					
				}
			}

			break;
		case 1:
			accion = actTURN_SR;
			contador = 2;
			break;
		case 2: 
			accion = actTURN_SR;
			contador = 0;
			algoritmo = 0;
			break;
		default:
			break;
		}
		break;
	case 3:
		switch (contador)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			contador++;
			accion = actTURN_L;
			break;
		case 4:
			contador=0;
			algoritmo = 0;
			accion = actTURN_L;
			break;
		default:
			break;
		}
	default:
		break;
	}

	if (!prioridad.empty())
	{
		accion = prioridad.top().accion;
		prioridad.pop();
		algoritmo = 50;
		return accion;
	}
	


	return accion;
};

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}