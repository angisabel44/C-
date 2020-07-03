#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <math.h>
#include <ctime>

using namespace std;

struct coordenada
{
	int id;
	float x;
	float y;
	int beneficio;
};

bool comparacion(coordenada a, coordenada b);

class orienteering
{
public:
	orienteering(string file_name);
	~orienteering() {};

	void funDisInicial();
	void constructivo();
	void busquedaLocal();

	void agregarElemento(std::list<coordenada>::iterator it, std::vector<coordenada>::iterator jt);
	void borrarElemento(std::vector<coordenada>::iterator it);
	float calcularDistancia(std::list<coordenada>::iterator it, std::vector<coordenada>::iterator jt);

	std::vector<coordenada>::iterator mejorPosicion(list<coordenada>::iterator it);
	std::vector<coordenada>::iterator quienQuito();

	int cuantosQuito();
	int cuantosPongo();

	void guardarSolucion();
	void impresion();
	
private:
	std::list<coordenada> nodos, mejoresNodos;

	std::vector<coordenada> solucion, mejorSolucion;
	int funcionObjetivo, mejorObjetivo;
	float distancia, mejorDistancia;

	string instancia;
	int distanciaLimite;
	float inicio;
};

int main(int argc, char const *argv[])
{	

	orienteering problema(argv[1]);
	problema.constructivo();
	problema.impresion();
	problema.busquedaLocal();
	problema.impresion();

	return 0;
}

bool comparacion(coordenada a, coordenada b)
{
	return a.beneficio > b.beneficio;
}

orienteering::orienteering(string file_name)
{
	srand(time(NULL));
	inicio = clock();
	ifstream lectura(file_name.c_str());
	string basura;
	int i = 1;

	instancia = file_name;

	if (!lectura.is_open())
	{
		cout << "Archivo no encontrado!!" << endl;
		exit(0);
	}

	lectura >> distanciaLimite >> basura;
	//cout << distanciaLimite << endl;

	for (int j = 0; j < 2; j++)
	{
		coordenada nodoAux;
		float x, y;
		int beneficio;

		lectura >> x >> y >> beneficio;
		//cout << x << " " << y << " " << beneficio << endl;

		nodoAux.x = x;
		nodoAux.y = y;
		nodoAux.beneficio = beneficio;
		nodoAux.id = 0;

		solucion.push_back(nodoAux);
	}

	while(!lectura.eof())
	{
		coordenada nodoAux;
		float x, y;
		int beneficio;

		lectura >> x >> y >> beneficio;
		//cout << x << " " << y << " " << beneficio << endl;

		nodoAux.x = x;
		nodoAux.y = y;
		nodoAux.beneficio = beneficio;
		nodoAux.id = i;

		nodos.push_back(nodoAux);

		i++;
	}

	lectura.close();

	funDisInicial();
	nodos.sort(comparacion);
}

void orienteering::funDisInicial()
{
	std::vector<coordenada>::iterator it = solucion.begin();
	std::vector<coordenada>::iterator jt = it + 1;

	float deltax = it->x - jt->x;
	//cout << deltax << endl;
	float deltay = it->y - jt->y;
	//cout << deltay << endl;

	funcionObjetivo = it->beneficio + jt->beneficio;
	distancia = sqrt(pow(deltax, 2) + pow(deltay, 2));
}

void orienteering::impresion()
{
	cout << "Distancia limite: " << distanciaLimite << endl;
	cout << endl;

	for (std::list<coordenada>::iterator i = nodos.begin(); i != nodos.end(); ++i)
	{
		cout << "Nodo: " << i->id << " (" << i->x << ", " << i->y << ")\t Beneficio: " << i->beneficio << endl; 
	} cout << endl;

	cout << "Solucion: ";

	for (std::vector<coordenada>::iterator i = solucion.begin(); i != solucion.end(); ++i)
	{
		cout << i->id << " -> ";
	}
	cout << endl;
	cout << "Funcion Objetivo: " << funcionObjetivo << "\tDistancia: " << distancia << endl;
	cout << endl;
}

void orienteering::constructivo()
{
	for (std::list<coordenada>::iterator i = nodos.begin(); i != nodos.end(); ++i)
	{
		if (calcularDistancia(i, solucion.end() - 1) < distanciaLimite)
		{
			agregarElemento(i, solucion.end() - 1);
			i = nodos.begin();
		}
	}

	mejoresNodos = nodos;
	mejorSolucion = solucion;
	mejorDistancia = distancia;
	mejorObjetivo = funcionObjetivo;
}

void orienteering::agregarElemento(std::list<coordenada>::iterator it, std::vector<coordenada>::iterator lt)
{
	coordenada nodoAux;

	nodoAux.x = it->x;
	nodoAux.y = it->y;
	nodoAux.id = it->id;
	nodoAux.beneficio = it->beneficio;

	lt = solucion.insert(lt, nodoAux);

	std::vector<coordenada>::iterator jt, kt;
	//cout << lt->id << endl;
	jt = lt - 1;
	kt = lt + 1;

	float a = sqrt(pow(jt->x - lt->x, 2) + pow(jt->y - lt->y, 2));
	float b = sqrt(pow(kt->x - lt->x, 2) + pow(kt->y - lt->y, 2));
	float c = sqrt(pow(jt->x - kt->x, 2) + pow(jt->y - kt->y, 2));

	//cout << a << " " << b << " " << c << " " << distancia << endl;
	funcionObjetivo = funcionObjetivo + lt->beneficio;
	distancia = distancia + a + b - c;
	nodos.erase(it);
}

void orienteering::borrarElemento(std::vector<coordenada>::iterator it)
{
	std::vector<coordenada>::iterator jt, kt;
	//cout << it->id << endl;
	jt = it - 1;
	kt = it + 1;

	float a = sqrt(pow(jt->x - it->x, 2) + pow(jt->y - it->y, 2));
	float b = sqrt(pow(kt->x - it->x, 2) + pow(kt->y - it->y, 2));
	float c = sqrt(pow(jt->x - kt->x, 2) + pow(jt->y - kt->y, 2));

	//cout << a << " " << b << " " << c << " " << distancia << endl;
	funcionObjetivo = funcionObjetivo - it->beneficio;
	distancia = distancia - a - b + c;

	coordenada nodoAux;

	nodoAux.x = it->x;
	nodoAux.y = it->y;
	nodoAux.id = it->id;
	nodoAux.beneficio = it->beneficio;

	nodos.push_back(nodoAux);
	solucion.erase(it);
}

void orienteering::guardarSolucion()
{
	string salida = "resultados";
	ofstream archivo(salida.c_str(), ios::app);

	float ejecucion = (clock() - inicio)/CLOCKS_PER_SEC;

	archivo << instancia << " " << funcionObjetivo << " " << ejecucion << endl;

	archivo.close();
}

float orienteering::calcularDistancia(std::list<coordenada>::iterator it, std::vector<coordenada>::iterator jt)
{
	std::vector<coordenada>::iterator i = jt - 1;

	float a = sqrt(pow(i->x - it->x, 2) + pow(i->y - it->y, 2));
	float b = sqrt(pow(jt->x - it->x, 2) + pow(jt->y - it->y, 2));
	float c = sqrt(pow(i->x - jt->x, 2) + pow(i->y - jt->y, 2));

	return (distancia + a + b - c);
}

void orienteering::busquedaLocal() 
{
	int noMejoras = 0;
	int iteraciones = 0;

	while (noMejoras < 10)
	{
		if (solucion.size() < 4)
		{
			for (std::list<coordenada>::iterator i = nodos.begin(); i != nodos.end(); ++i)
			{
				std::vector<coordenada>::iterator jt = mejorPosicion(i);
				if (calcularDistancia(i, jt) < distanciaLimite)
				{
					//cout << "Se agrego el nodo: " << i->id << endl;
					agregarElemento(i, jt);
					i = nodos.begin();
				}
			} //cout << endl;
			cout << "Funcion Objetivo: " << funcionObjetivo << "\tDistancia: " << distancia << endl;
			//cout << endl;
		} else
		{
			int num = cuantosQuito();
			//cout << "Se van a quitar " << num << " nodos de la solucion." << endl;
			for (int i = 0; i < num; ++i)
			{
				std::vector<coordenada>::iterator it = quienQuito();
				//cout << "Se va a quitar el nodo: " << it->id << endl;
				borrarElemento(it);
			} //cout << endl;

			for (std::list<coordenada>::iterator i = nodos.begin(); i != nodos.end(); ++i)
			{
				std::vector<coordenada>::iterator jt = mejorPosicion(i);
				if (calcularDistancia(i, jt) < distanciaLimite)
				{
					//cout << "Se agrego el nodo: " << i->id << endl;
					agregarElemento(i, jt);
					i = nodos.begin();
				}
			} //cout << endl;
			cout << "Funcion Objetivo: " << funcionObjetivo << "\tDistancia: " << distancia << endl;
			//cout << endl;
		}

		if (funcionObjetivo > mejorObjetivo)
		{
			nodos.sort(comparacion);
			mejoresNodos = nodos;
			mejorSolucion = solucion;
			mejorDistancia = distancia;
			mejorObjetivo = funcionObjetivo;
			noMejoras = 0;
		} else
		{
			nodos = mejoresNodos;
			solucion = mejorSolucion;
			distancia = mejorDistancia;
			funcionObjetivo = mejorObjetivo;
			noMejoras++;
		}
		iteraciones++;
	} cout << endl;
	cout << "Se hicieron " << iteraciones << " iteraciones.\n" << endl;
}

std::vector<coordenada>::iterator orienteering::mejorPosicion(list<coordenada>::iterator it)
{
	vector<coordenada>::iterator jt = solucion.begin() + 1;
	float dist = calcularDistancia(it, jt);
	for (std::vector<coordenada>::iterator i = solucion.begin() + 1; i != solucion.end(); ++i)
	{
		if (dist > calcularDistancia(it, i))
		{
			jt = i;
		}
	}
	return jt;
}

std::vector<coordenada>::iterator orienteering::quienQuito()
{
	std::vector<coordenada>::iterator it = solucion.begin() + 1;
	std::vector<coordenada>::iterator jt, lt;
	jt = it - 1;
	lt = it + 1;

	float a = sqrt(pow(jt->x - it->x, 2) + pow(jt->y - it->y, 2));
	float b = sqrt(pow(lt->x - it->x, 2) + pow(lt->y - it->y, 2));
	float mayordistancia = a + b;

	for (std::vector<coordenada>::iterator i = solucion.begin() + 1; i != solucion.end() - 1; ++i)
	{
		jt = i - 1;
		lt = i + 1;
	
		a = sqrt(pow(jt->x - i->x, 2) + pow(jt->y - i->y, 2));
		b = sqrt(pow(lt->x - i->x, 2) + pow(lt->y - i->y, 2));
		
		if (mayordistancia < (a + b))
		{
			mayordistancia = a + b;
			it = i;
		}
	}

	return it;
}

int orienteering::cuantosQuito()
{
	float numero = rand() % 1000;
	//cout << "El numero es: " << numero << endl;
	numero = numero / 1000;
	//cout << "El numero es: " << numero << endl;
	if (numero < 0.1)
	{
		return 0;
	} else
	{
		if (numero < 0.85)
		{
			return 1;
		} else
		{
			return 2;
		}
	}
}

int orienteering::cuantosPongo()
{
	srand(time(NULL));
	int random = rand() % 1000;
	random = random / 1000;
	if (random < 0.6)
	{
		return 1;
	} else
	{
		if (random < 0.9)
		{
			return 2;
		} else
		{
			return 3;
		}
	}
}