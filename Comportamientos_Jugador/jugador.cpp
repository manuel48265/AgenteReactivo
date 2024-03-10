#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include<queue>
#include<math.h>
#include<set>
#include<tgmath.h>

using namespace std;





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

void CalculoCasilla(int i,const state &st, const vector<vector<unsigned char>> &matriz, bool orden, point &p, bool &no_vacio){
	cout << "aqui6" << endl;
	int j = i;
	if (orden){
		while(no_vacio and j <= -i){
			if(matriz.at(Ajuste(st.p.fil+i,matriz.size()-1)).at(Ajuste(st.p.col + j,matriz.size()-1)) == '?'){
				no_vacio = false; 
				p.fil = Ajuste(st.p.fil+i,matriz.size()-1);
				p.col = Ajuste(st.p.col + j,matriz.size()-1);

			}
			j++;
			cout << j << endl;
		}
	}else{
		
		while(no_vacio and j <= -i){
			if(matriz.at(Ajuste(st.p.fil + j,matriz.size()-1)).at(Ajuste(st.p.col + i,matriz.size()-1)) == '?'){
				no_vacio = false; 
				p.fil = Ajuste(st.p.fil + j,matriz.size()-1);
				p.col = Ajuste(st.p.col + i,matriz.size()-1);

			}
			j++;
			cout << j << endl;
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

	cout << init << endl;
	cout << there << endl;

	if(init == there){
	}
	else if((there-init+8)%8 <= 3){
		pq.push(rutina(actTURN_SR,30 - pq.size()));
		CambiaDir((init+1)%8,pq,there);
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

	point p = point(st.p.fil, st.p.col);
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




Orientacion CalOrientacion(point a, point b){
	cout << "aqui2" << endl;
	int x = b.col - a.col;
	int y = b.fil - a.fil;
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

void NuevaRuta(const state &st, const vector<vector<unsigned char>> &matriz, priority_queue<rutina> &pq){
	cout << "aqui9" << endl;
	point p(st.p.fil,st.p.col);
	point q = ExploracionLocal(st,matriz);

	if(p == q){
		q = MasVacio(matriz);
	}

	cout << p.fil << "," << p.col << endl;
	cout << q.fil << "," << q.col << endl;

	CambiaDir(st.brujula,pq,CalOrientacion(p,q));

}

void RutinaExploracion(const vector<unsigned char> & terreno, Action &accion, unsigned int &algoritmo, const state &st, const vector<vector<unsigned char>> &matriz, priority_queue<rutina> &pq){
	cout << "aqui10" << endl;
	point p(st.p);
	for(int i = 0; i <4; i++){
		Actualizar(p.fil,p.col,st.brujula);
	}
	p.fil = Ajuste(p.fil,matriz.size()-1);
	p.col = Ajuste(p.col,matriz.size()-1);
	
	if((terreno[2] == 'T' or terreno[2] =='S' or terreno[2] == 'G' or terreno[2] == 'B' )
		and (matriz.at(p.fil).at(p.col) == '?')){
			accion = actWALK;
		}
		else if((matriz.at(p.fil).at(p.col) == '?')) {
			accion =actTURN_L;
			algoritmo = 0;
		}else{
			NuevaRuta(st,matriz,pq);
		}
}

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

void Explorar(priority_queue<rutina> &pq,const vector<vector<casilla>> &matriz,casilla target){
	set<casilla> visitados; 
	priority_queue<casilla> target; 

	


}

void Actualizar(){

}

void Colocar(){
	
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
                matriz.at(st.p.fil +i*y +j*x).at(st.p.col -i*x +j*y) = terreno.at(j*(j+1)+i);
            }
        }
    }
    if(st.brujula%2 == 1){
        for (int j = 0; j <= 3; j++){

            for(int i = -j; i < 0; i++){
                matriz.at(st.p.fil +j*x).at(st.p.col + i*y +j*y) = terreno.at(j*(j+1)+i*(-1)*x*y);
            }

            for(int i = 0; i <= j; i++){
                matriz.at(st.p.fil -i*x +j*x).at(st.p.col +j*y) = terreno.at(j*(j+1)+i*(-1)*x*y);
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
		Actualizar(current_state.p.fil,current_state.p.col,current_state.brujula);
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
		current_state.p.fil = sensores.posF;
		current_state.p.col = sensores.posC;
		current_state.brujula = sensores.sentido;
		PonerTerrenoEnMatriz(sensores.terreno,current_state,mapaResultado);
	}

	//Toma de decisiones
	if (!prioridad.empty())
	{
		accion = prioridad.top().accion;
		prioridad.pop();
		algoritmo = 0;
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
					RutinaExploracion(sensores.terreno,accion,algoritmo,current_state,mapaResultado,prioridad);
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
		RutinaBasica(sensores.terreno,accion,algoritmo);
		if (rand()%8 == 0){
			RutinaExploracion(sensores.terreno,accion,algoritmo,current_state,mapaResultado,prioridad);
		}
		break;
	}

	if (!prioridad.empty())
	{
		accion = prioridad.top().accion;
		prioridad.pop();
		algoritmo = 0;
		return accion;
	}
	


	return accion;
};

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}