#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;


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
                matriz.at(st.fil +j*x).at(st.col + i*y +j*y) = terreno.at(j*(j+1)+i);
            }

            for(int i = 0; i <= j; i++){
                matriz.at(st.fil -i*x +j*x).at(st.col +j*y) = terreno.at(j*(j+1)+i);
            }
        }
    }


}

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
		switch (current_state.brujula)
		{
		case norte:
			current_state.fil--;
			break;
		case noreste:
			current_state.fil--;
			current_state.col++;
			break;	
		case este:
			current_state.col++;
			break;
		case sureste:
			current_state.fil++;
			current_state.col++;
			break;
		case sur:
			current_state.fil++;
			break;
		case suroeste:
			current_state.fil++;
			current_state.col--;
			break;
		case oeste:
			current_state.col--;
			break;
		case noroeste:
			current_state.fil--;
			current_state.col--;
			break;
		default:
			break;
		}
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
	}

	//Toma de decisiones
	
	if(((sensores.terreno[2] == 'T' or sensores.terreno[2] =='S' or sensores.terreno[2] == 'G') and sensores.agentes[2] =='_') and !rotar){
		accion =actWALK;
		rotar = true;
	}
	if (rotar){
		accion =actTURN_L;
		rotar = (rand()%4 == 0);
	}

	

	
	if(bien_situado){
		PonerTerrenoEnMatriz(sensores.terreno,current_state,mapaResultado);
	}


	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}