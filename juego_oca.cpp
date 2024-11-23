#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

using namespace std;
const int MAXJUG = 50;
const int FILA = 7;
const int COL = 9;
const int MAX_EVENTOS = 8;  // Incluimos 7 eventos y la Oca
const int VACIO = 0;

typedef char tcad[12];
typedef struct tjugador {
	tcad apodo;
	tcad nombre;
	tcad apellido;
	int puntaje;
	int partidasJugadas;
	int partidasGanadas;
};
typedef tjugador tjugadores[MAXJUG];

typedef struct tcasilla {
	int evento;
	tcad nombreEvento;
};

typedef tcasilla ttablero[FILA][COL];

typedef struct ttitular {
	tcad nombre;
	int posicionActual = 0;
	int puntaje = 0;
	int turnosPerdidos = 0;
};
typedef ttitular ttitulares[MAXJUG];

// Declaración de funciones
void menuPrincipal();
void gestionarJugadores(tjugadores jugadores, int &cantidad, int &jugadoresRegistrados);
void registrarJugadoresAleatorios(tjugadores jugadores, int &cantidad, int cantidadCrear, int &jugadoresRegistrados);
void insercion(tjugadores jugadores, int& cantidad);
void generarApodo(char nombre[], char apellido[], char apodo[]);
void agregarJugador(tjugadores jugadores, tjugador jugador, int &cant);
void listarJugadores(tjugadores jugadores, int cantidad, int index = 0);
int buscarJugadorPorApodo(tjugadores jugadores, int cantidad, tcad apodo);
int buscarTitularPorApodo(ttitulares jugadores, int cantidad, tcad apodo);
void consultarJugador(tjugadores jugadores, int cantidad);
void modificarJugador(tjugadores jugadores, int cantidad);
void eliminarJugador(tjugadores jugadores, int &cantidad);
void mostrarJugador(const tjugador &jugador);
void generarDatosAleatorios(tjugador &jugador);

void listarApodos(tjugadores jugadores, int cantidad);

void crearTablero(ttablero tablero);
void mostrarTablero(ttablero tablero);

int tirarDados();    // Simula el lanzamiento de dos dados
void manejarEvento(ttitulares jugadoresTitulares, int i, ttablero tablero, int numJugadores);
const char* obtenerEvento(int casilla, ttablero tablero);   // Determina el evento asociado a una casilla específica
void Jugar(int numJugadores, tjugadores jugadores, ttitulares jugadoresTitulares, ttablero tablero); // Controla la lógica principal del juego
void reiniciarJugadoresTitulares(ttitulares jugadoresTitulares, int numJugadores);

void shell(tjugadores jugadores, int ocupado);
void mostrarRanking(tjugadores jugadores, int numJugadores);

// Función principal
int main() {
	srand(time(NULL));
	menuPrincipal();
	
	
	return 0;
}
void menuPrincipal() {
	int opcion;
	tjugadores jugadores;
	int cantidadJugadores = -1;
	int jugadoresRegistrados = 0;
	
	ttitulares jugadoresTitulares;
	//		
	ttablero tablero;
	bool menuJugador = false;
	bool menuTablero = false;
	
	do {
		cout << "=== MENU PRINCIPAL ===" << endl;
		cout << "1- Gestionar jugadores" << endl;
		cout << "2- Crear tablero" << endl;
		cout << "3- Jugar" << endl;
		cout << "4- Ranking de jugadores" << endl;
		cout << "0- Salir" << endl;
		cout << "Seleccione una opcion: ";
		
		cin >> opcion;
		cout <<endl;
		
		tcad apodo; // Variable para el apodo del jugador
		int indiceJugadorEncontrado;   // Bandera para verificar si el jugador está registrado
		int indiceJugadorTitular;
		switch(opcion) {
		case 1:
			// Gestión de jugadores (registro, modificación, eliminación)
			gestionarJugadores(jugadores, cantidadJugadores, jugadoresRegistrados);
			menuJugador = true;
			break;
		case 2:
			// Crear el tablero y mostrarlo
			crearTablero(tablero);
			mostrarTablero(tablero);
			menuTablero = true;
			break;
		case 3:
			
			// Si no se ha creado el tablero o no se han registrado jugadores, no se puede jugar
			if(!menuJugador || !menuTablero){
				cout <<"Elegir las 2 primeras opciones, antes de jugar" <<endl;
				break;
			}
			if(cantidadJugadores + 1 < 2){
				cout <<"Registra al menos 2 jugadores" <<endl;
				break;
			}
			// Permitir al usuario seleccionar el número de jugadores que participarán en el juego
			int numJugadores;
			cout << "Ingrese el número de jugadores (min: 2, max: Jugadores registrados " << cantidadJugadores + 1 << "): ";
			cin >> numJugadores;
			
			if (numJugadores < 2 || numJugadores > cantidadJugadores + 1) {
				cout << "Número de jugadores no válido.\n";
				break;
				
				
			}
			// Mostrar los jugadores registrados y permitir elegir a los jugadores
			for(int j=0; j <= cantidadJugadores; j++){
				cout << (j+1) <<") "<< jugadores[j].apodo << endl;
			}
			
			for (int i = 0; i < numJugadores; i++) {
				
				cout << "Ingrese el apodo del jugador " << i + 1 << ": ";
				fflush(stdin);
				gets(apodo);
				indiceJugadorEncontrado = buscarJugadorPorApodo(jugadores, cantidadJugadores, apodo);
				indiceJugadorTitular = buscarTitularPorApodo(jugadoresTitulares, i, apodo);
				
				
				if(indiceJugadorEncontrado != -1)
					if(indiceJugadorTitular != -1){
						cout<<"No puedes jugar contra ti mismo, elige otro jugador"<<endl;
						i--;
					}else{
						strcpy(jugadoresTitulares[i].nombre, apodo);
					}
					
						
				else{
						cout << "jugador no registrado" << endl;
						i--;
					}
			}
			
			// Iniciar la partida
			Jugar(numJugadores, jugadores, jugadoresTitulares, tablero);
			
			break;
		case 4:
			mostrarRanking(jugadores, cantidadJugadores);
			break;
		case 0:
			cout << "Saliendo del programa..." << endl;
			break;
		default:
			cout << "Opcion invalida, intente nuevamente." << endl;
		}
	} while(opcion != 0);
}
	


void gestionarJugadores(tjugadores jugadores, int &cantidad, int &jugadoresRegistrados) {
	int opcion;
	do {
		cout<<endl;
		cout << "=== GESTIONAR JUGADORES ===" << endl;
		cout << "1- Registrar jugadores aleatorios" << endl;
		cout << "2- Listar jugadores" << endl;
		cout << "3- Consultar jugador" << endl;
		cout << "4- Modificar jugador" << endl;
		cout << "5- Eliminar jugador" << endl;
		cout << "0- Volver al menu principal" << endl;
		cout << "Seleccione una opcion: ";
		cin >> opcion;
		
		switch(opcion) {
		case 1:
			
			if(cantidad == MAXJUG -1){
				cout<<"Ya existen 50 jugadores registrados"<<endl;
				return;
			}
			// Registrar jugadores aleatorios
			int cantidadCrear;
			cout << "¿Cuántos jugadores desea crear (máximo 50)? ";
			cin >> cantidadCrear;
			if (cantidadCrear <= 0 || cantidadCrear > 50) {
				cout << "Cantidad no válida. Debe ser entre 1 y 50." << endl;
			} else {
				int aux = cantidad + cantidadCrear;
				
				if(aux >= MAXJUG){
					cout<<"solo puede agregar: "<<aux - (MAXJUG-2)<<endl;
					
					return;
				}
				
				registrarJugadoresAleatorios(jugadores, cantidad, cantidadCrear, jugadoresRegistrados);
//				jugadoresregistrados = cantidadCrear;
			}
			break;
		case 2:
			if(cantidad < 0){
				cout<<"No hay datos"<<endl;
				return;
			}
			listarJugadores(jugadores, cantidad, 0);
			break;
		case 3:
			
			consultarJugador(jugadores, cantidad);
			break;
		case 4:
			modificarJugador(jugadores, cantidad);
			break;
		case 5:
			eliminarJugador(jugadores, cantidad);
			break;
		case 0:
			cout << "Volviendo al menu principal..." << endl;
			break;
		default:
			cout << "Opcion invalida, intente nuevamente." << endl;
		}
	} while(opcion != 0);
}

void registrarJugadoresAleatorios(tjugadores jugadores, int &cantidad, int cantidadCrear, int &jugadoresRegistrados) {
	
	
	
	for (int i = 0; i < cantidadCrear; i++) {
	
		tjugador nuevoJugador;
		generarDatosAleatorios(nuevoJugador);
		nuevoJugador.puntaje = 0;
		nuevoJugador.partidasJugadas = 0;
		nuevoJugador.partidasGanadas = 0;
		agregarJugador(jugadores, nuevoJugador, cantidad);
		insercion(jugadores, cantidad);
		
		//		for (int j=0; j < cantidad; j++) {
		//			if (strcmp(jugadores[j].apodo, nuevoJugador.apodo) == 0) {
		//				//cout << "Error: Apodo repetido, generando otro apodo." << endl;
		//				i--;  // Reducir el contador para volver a intentar el registro
		//				cantidad--;
		//				break;
		//			}
		//		}

	}
	listarApodos(jugadores, cantidad);
}
	void generarDatosAleatorios(tjugador &nuevoJugador){
		char nombres[10][12] = {"dibu", "huevo", "cuti", "rogelio", "agustin", "enzo", "lionel", "colorado", "julian", "angel"};
		char apellidos[10][12] = {"martinez", "acuña", "romero", "otamendi", "montiel", "fernandez", "messi", "macalister", "dimaria", "alvarez"};
		// Generar nombre y apellido aleatorio
		strcpy(nuevoJugador.nombre, nombres[rand() % 10]);
		strcpy(nuevoJugador.apellido, apellidos[rand() % 10]);
		
		// Generar apodo único
		generarApodo(nuevoJugador.nombre, nuevoJugador.apellido, nuevoJugador.apodo);
		
				
	}
void agregarJugador(tjugadores jugadores, tjugador jugador, int &cant){
	if(cant >= MAXJUG -1){
		cout << "No se puede agregar mas jugadores" << endl;
	}
	else{
		cant++;
		jugadores[cant] = jugador;
	}
}
void insercion(tjugadores jugadores, int& cantidad) {
	int i, j;
	tjugador aux;
	
	// Usamos un ciclo for para iterar sobre los jugadores registrados.
	for (i = 1;  i < cantidad; i++) {
		aux = jugadores[i];
		j = i-1;
		// Usamos el ciclo while para verificar la condición y mover los jugadores hacia la derecha.
		while (j >= 0 && strcmp(aux.apodo, jugadores[j].apodo) < 0) {
			jugadores[j + 1] = jugadores[j]; // Desplazamos el jugador
			j--; // Decrementamos i
		}
		
		// Insertamos el nuevo jugador en la posición correcta
		jugadores[j + 1] = aux;
	}
	
}

void generarApodo(char nombre[], char apellido[], char apodo[]) {
	// Obtener los primeros 3 caracteres del nombre y copiarlos al inicio del apodo
	strncpy(apodo, nombre, 3);
	
	// Obtener los primeros 3 caracteres del apellido y copiarlos a continuación en el apodo
	strncpy(apodo + 3, apellido, 3);
	
	// Generar un número aleatorio de 2 dígitos entre 10 y 99
	int numeroAleatorio = rand() % 90 + 10;
	
	// Convertir el número aleatorio a cadena y concatenarlo en la posición 6 de apodo
	sprintf(apodo + 6, "%d", numeroAleatorio);
}
void listarApodos(tjugadores jugadores, int cantidad){
	
	for(int i = 0; i <= cantidad; i++){
		cout << (i+1) <<") "<< jugadores[i].apodo << endl;
	}
	
}

void listarJugadores(tjugadores jugadores, int cantidad, int index) {
	if (index > cantidad) return; // Base case: no hay más jugadores
	mostrarJugador(jugadores[index]);
	listarJugadores(jugadores, cantidad, index + 1);  // Llamada recursiva
	
}



void consultarJugador(tjugadores jugadores, int cantidad) {
	
	if(cantidad < 0){
		cout<<"No hay datos"<<endl;
		return;
	}
	tcad apodo;
	cout << "Ingrese el apodo del jugador a consultar: ";
	cin >> apodo;
	
	int indice = buscarJugadorPorApodo(jugadores, cantidad, apodo);
	if (indice != -1) {
		cout << "Jugador encontrado:" << endl;
		mostrarJugador(jugadores[indice]);
	} else {
		cout << "Jugador no encontrado." << endl;
	}
}

int buscarJugadorPorApodo(tjugadores jugadores, int cantidad, tcad apodo) {
	for (int i = 0; i <= cantidad; i++) {
		if (strcmp(jugadores[i].apodo, apodo) == 0) {
			return i;  // Retorna el índice del jugador encontrado
		}
	}
	return -1;  // Si no se encuentra, retorna -1
}
int buscarTitularPorApodo(ttitulares jugadores, int cantidad, tcad apodo) {
	for (int i = 0; i <= cantidad; i++) {
		if (strcmp(jugadores[i].nombre
				   , apodo) == 0) {
			return i;  // Retorna el índice del jugador encontrado
		}
	}
	return -1;  // Si no se encuentra, retorna -1
}

void modificarJugador(tjugadores jugadores, int cantidad) {
	
	if(cantidad < 0){
		cout<<"No hay datos"<<endl;
		return;
	}
	tcad apodo;
	cout << "Ingrese el apodo del jugador a modificar: ";
	cin >> apodo;
	
	int indice = buscarJugadorPorApodo(jugadores, cantidad, apodo);
	if (indice != -1) {
		cout << "Jugador encontrado. Ingrese los nuevos datos:" << endl;
		
		cout << "Nuevo nombre (sin cambiar el apodo): ";
		cin >> jugadores[indice].nombre;
		
		cout << "Nuevo apellido (sin cambiar el apodo): ";
		cin >> jugadores[indice].apellido;
		
		cout << "Jugador modificado exitosamente." << endl;
	} else {
		cout << "Jugador no encontrado." << endl;
	}
}

void eliminarJugador(tjugadores jugadores, int &cantidad) {
	
	if(cantidad < 0){
		cout<<"Aun no se han registrados jugadores"<<endl;
		return;
	}
	tcad apodo;
	cout << "Ingrese el apodo del jugador a eliminar: ";
	cin >> apodo;
	
	int indice = buscarJugadorPorApodo(jugadores, cantidad, apodo);
	if (indice != -1) {
		for (int i = indice; i < cantidad - 1; i++) {
			jugadores[i] = jugadores[i + 1];  // Desplazar los jugadores hacia la izquierda
		}
		cantidad--;
		cout << "Jugador eliminado exitosamente." << endl;
	} else {
		cout << "Jugador no encontrado." << endl;
	}
}

void mostrarJugador(const tjugador &jugador) {
	cout << "Información del Jugador:" << endl;
	cout << "Nombre: " << jugador.nombre << endl;
	cout << "Apellido: " << jugador.apellido << endl;
	cout << "Apodo: " << jugador.apodo << endl;
	cout << "Puntaje: " << jugador.puntaje << endl;
	cout << "Partidas Jugadas: " << jugador.partidasJugadas << endl;
	cout << "Partidas Ganadas: " << jugador.partidasGanadas << endl;
	cout << "-----------------------------" << endl;
}

void crearTablero(ttablero tablero) {
	// Nombres de los eventos, el índice 0 se mantiene como VACIO
	tcad eventosNombres[MAX_EVENTOS] = {
		"_____", "PERRO", "ARCOIRIS", "POSADA", "ZORRO", "LEON", "DUENDE", "OCA"
	};
	
	// Inicializamos el tablero en vacío
	for (int i = 0; i < FILA; i++) {
		for (int j = 0; j < COL; j++) {
			tablero[i][j].evento = VACIO;
			strcpy(tablero[i][j].nombreEvento, eventosNombres[VACIO]);
		}
	}
	
	// Asignamos las 4 ocas en el tablero con al menos 7 casillas de separación
	int posOcas[4];
	for (int i = 0; i < 4; i++) {
		int posFila, posCol;
		bool posicionValida;
		do {
			posicionValida = true;
			posFila = rand() % FILA;
			posCol = rand() % COL;
			
			int posCasilla = posFila * COL + posCol;  // Convertimos la posición 2D a 1D
			
			// Verificamos que la posición de la oca no sea la casilla 42 o 63 y que no esté ocupada
			if (posCasilla == 41 || posCasilla == 62 || tablero[posFila][posCol].evento != VACIO) {
				posicionValida = false;
			}
			
			// Verificamos que esté a al menos 7 casillas de otras ocas
			for (int j = 0; j < i && posicionValida; j++) {
				int posCasillaAnterior = posOcas[j];
				if (abs(posCasilla - posCasillaAnterior) < 7) {
					posicionValida = false;
				}
			}
		} while (!posicionValida);
		
		// Asignamos la posición de la oca y guardamos la posición
		tablero[posFila][posCol].evento = 7;  // Código para Oca en nuestro arreglo
		strcpy(tablero[posFila][posCol].nombreEvento, eventosNombres[7]);
		posOcas[i] = posFila * COL + posCol;  // Guardamos la posición como índice lineal
	}
	
	// Asignamos los otros eventos aleatoriamente, evitando la casilla 42 y 63
	for (int i = 1; i < MAX_EVENTOS - 1; i++) {
		int posFila, posCol;
		do {
			posFila = rand() % FILA;
			posCol = rand() % COL;
		} while ((posFila * COL + posCol == 41 || posFila * COL + posCol == 62) || tablero[posFila][posCol].evento != VACIO);
		
		// Asignamos el evento y el nombre del evento directamente desde el arreglo
		tablero[posFila][posCol].evento = i;
		strcpy(tablero[posFila][posCol].nombreEvento, eventosNombres[i]);
	}
}

void mostrarTablero(ttablero tablero) {
	for (int i = 0; i < FILA; i++) {
		for (int j = 0; j < COL; j++) {
			int casillaNum = i * COL + j;
			cout << "Casilla " << casillaNum << ": ";
			cout << tablero[i][j].nombreEvento << endl;
		}
	}
}

// Función principal de juego
void Jugar(int numJugadores, tjugadores jugadores, ttitulares jugadoresTitulares, ttablero tablero) {
	
	srand(time(NULL)); // Inicializar la semilla para los números aleatorios
	
	const int meta = 63;
	bool juegoEnCurso = true;
	
	while (juegoEnCurso) {
		
		
		for (int i = 0; i < numJugadores; i++) {
			
			cout << "\nTurno de " << jugadoresTitulares[i].nombre << ". Escriba 'D o d' y presione Enter para lanzar los dados: ";
			
			// Espera a que el jugador escriba "D o d"
			char comando;
			cin >> comando;
			if (comando != 'D' && comando != 'd') {
				cout << "Comando incorrecto. Intente de nuevo.\n";
				i--; // Repetir turno si el comando no es 'D' o 'd'
				continue;
			}
			
			manejarEvento(jugadoresTitulares, i, tablero, numJugadores);
			
			if (jugadoresTitulares[i].posicionActual == meta) {
				cout << jugadores[i].nombre << "\n¡¡¡¡¡¡¡¡¡ GANASTE !!!!!!!\n";
				jugadores[i].puntaje = jugadoresTitulares[i].puntaje;
				jugadores[i].partidasJugadas ++;
				jugadores[i].partidasGanadas ++;
				reiniciarJugadoresTitulares(jugadoresTitulares, numJugadores);
				juegoEnCurso = false;
				break;
			}
		}
	}
	
}

void reiniciarJugadoresTitulares(ttitulares jugadoresTitulares, int numJugadores) {
	for (int i = 0; i < numJugadores; i++) {
		jugadoresTitulares[i].posicionActual = 0;  // Reiniciar posición a la primera casilla
		jugadoresTitulares[i].turnosPerdidos = 0;  // Reiniciar turnos perdidos
		jugadoresTitulares[i].puntaje = 0;  // Reiniciar puntaje
	}
	cout << "Valores de los jugadores titulares han sido reiniciados." << endl;
}
// Función para simular la tirada de dos dados
int tirarDados() {
	return (rand() % 6 + 1) + (rand() % 6 + 1);
}

// Función de obtener evento con el índice lineal
const char* obtenerEvento(int casilla, ttablero tablero) {
	// Verificar que el número de casilla esté dentro del rango permitido (0 a 62)
	if (casilla < 0 || casilla >= FILA * COL) {
		return "Casilla no válida";  // Devuelve un mensaje de error si la casilla es inválida
	}
	
	// Convertir el índice lineal a índices bidimensionales
	int fila = casilla / COL;
	int columna = casilla % COL;
	
	// Retornar el nombre del evento en la casilla especificada
	return tablero[fila][columna].nombreEvento;
}



void manejarEvento(ttitulares jugadoresTitulares, int i, ttablero tablero, int numJugadores) {
	bool habilitado = (jugadoresTitulares[i].turnosPerdidos == 0);
	
	if (!habilitado) {
		printf("%s debes %d turnos \n", jugadoresTitulares[i].nombre, jugadoresTitulares[i].turnosPerdidos);
		jugadoresTitulares[i].turnosPerdidos -= 1;
	} else {
		int valorDados = tirarDados();
		int nuevaPosicion = jugadoresTitulares[i].posicionActual + valorDados;
		
		// Ajustar si la nueva posición supera la meta
		if (nuevaPosicion > 63) {
			nuevaPosicion = 63;  // Limitar la posición al último casillero
		}
		
		// Convertir nueva posición en coordenadas de fila y columna
		int fila = nuevaPosicion / COL;
		int columna = nuevaPosicion % COL;
		
		// Obtener el evento en la casilla actual
		const char* evento = obtenerEvento(nuevaPosicion, tablero);
		int primeraPosicion = jugadoresTitulares[i].posicionActual;
		int posicionSinEvento;
		
		// Actualizar la posición si no se ha encontrado un evento
		if (primeraPosicion + valorDados < 64) {
			posicionSinEvento = jugadoresTitulares[i].posicionActual += valorDados;
		}
		
		// Verificar si otro jugador ocupa la misma casilla
		for (int k = 0; k < numJugadores; k++) {
			// Asegúrate de no comparar el jugador consigo mismo
			if (nuevaPosicion == jugadoresTitulares[k].posicionActual && k != i) {
				printf("%s desplazaste a tu compañero %s a la primera posición porque llegaste a su posición.\n", 
					   jugadoresTitulares[i].nombre, jugadoresTitulares[k].nombre);
				jugadoresTitulares[k].posicionActual = 1;  // El compañero vuelve a la primera posición
				break;
			}
		}
		
		// Verificar el evento de la casilla y aplicar sus efectos
		if (strcmp(evento, "PERRO") == 0) {
			printf("El jugador %s se encontraba en la casilla %d, ha encontrado un PERRO en la posición %d y avanza %d casillas más (valor de los dados), llegando a la posición %d.\n", 
				   jugadoresTitulares[i].nombre, primeraPosicion, nuevaPosicion, valorDados, jugadoresTitulares[i].posicionActual);
		} else if (strcmp(evento, "ARCOIRIS") == 0) {
			jugadoresTitulares[i].posicionActual = 42;
			printf("El jugador %s ha encontrado un ARCOIRIS y avanza a la casilla 42.\n", jugadoresTitulares[i].nombre);
		} else if (strcmp(evento, "POSADA") == 0) {
			jugadoresTitulares[i].turnosPerdidos += 2;
			printf("El jugador %s ha encontrado una POSADA y pierde 2 turnos.\n", jugadoresTitulares[i].nombre);
		} else if (strcmp(evento, "ZORRO") == 0) {
			jugadoresTitulares[i].posicionActual = 1;
			jugadoresTitulares[i].puntaje = 0;
			printf("El jugador %s ha encontrado un ZORRO y vuelve a la casilla 1 perdiendo su puntaje.\n", jugadoresTitulares[i].nombre);
		} else if (strcmp(evento, "LEON") == 0) {
			jugadoresTitulares[i].turnosPerdidos += 1;
			printf("El jugador %s ha encontrado un LEON y pierde 1 turno.\n", jugadoresTitulares[i].nombre);
		} else if (strcmp(evento, "DUENDE") == 0) {
			jugadoresTitulares[i].puntaje -= valorDados;
			if (jugadoresTitulares[i].puntaje < 0) jugadoresTitulares[i].puntaje = 0;
			printf("El jugador %s ha encontrado un DUENDE y pierde %d puntos.\n", jugadoresTitulares[i].nombre, valorDados);
		} else if (strcmp(evento, "OCA") == 0) {
			// Avanzar hasta encontrar la próxima OCA
			do {
				nuevaPosicion++;
				fila = nuevaPosicion / COL;
				columna = nuevaPosicion % COL;
			} while (nuevaPosicion < 63 && strcmp(tablero[fila][columna].nombreEvento, "OCA") != 0);
			
			jugadoresTitulares[i].posicionActual = nuevaPosicion;
			printf("El jugador %s ha encontrado una OCA y avanza a la próxima OCA en la casilla %d.\n", jugadoresTitulares[i].nombre, nuevaPosicion);
		} else {
			printf("%s estaba en la casilla %d, lanzó los dados y obtuvo %d, avanzó a la casilla %d sin eventos.\n",
				   jugadoresTitulares[i].nombre, primeraPosicion, valorDados, jugadoresTitulares[i].posicionActual);
		}
		
		// Aumentar el puntaje por el valor de los dados
		jugadoresTitulares[i].puntaje += valorDados;  
	}
}


void shell(tjugadores jugadores, int ocupado, bool creciente) {
	int i, j, salto;
	tjugador aux;
	
	salto = ocupado / 2;  // Inicializamos el salto como la mitad del tamaño del array
	while (salto > 0) {
		for (i = salto; i < ocupado; i++) {
			aux = jugadores[i];
			j = i - salto;
			
			// Desplazamos los elementos en función del orden elegido
			while (j >= 0 && ((creciente && jugadores[j].puntaje > aux.puntaje) || 
				   (!creciente && jugadores[j].puntaje < aux.puntaje))) {
				jugadores[j + salto] = jugadores[j];
				j -= salto;
			}
			
			jugadores[j + salto] = aux;  // Insertamos el elemento en la posición correcta
		}
		salto /= 2;  // Reducimos el salto
	}
}

void mostrarRanking(tjugadores jugadores, int cantidad) {
	int count = 0;
	for (int i = 0; i <= cantidad; i++) {
		if (jugadores[i].puntaje > 0) {
			count++;
		}
	}
	
	if (count == 0) {
		cout << "No hay ganadores" << endl;
		return;
	}
	
	// Pedir al usuario el orden
	char orden;
	cout << "Seleccione el orden para mostrar el ranking (C - Creciente, D - Decreciente): ";
	cin >> orden;
	
	bool creciente = (orden == 'C' || orden == 'c'); // Si es 'C' o 'c', orden creciente
	
	// Ordenar jugadores por puntaje en el orden seleccionado
	shell(jugadores, cantidad, creciente);
	
	// Mostrar el ranking
	cout << "Ranking de jugadores en orden " << (creciente ? "creciente: " : "decreciente: ") << endl;
	for (int i = 0; i < cantidad; i++) {
		if (jugadores[i].puntaje > 0) {
			printf("%s - Puntaje: %d\n", jugadores[i].nombre, jugadores[i].puntaje);
		}
	}
}
