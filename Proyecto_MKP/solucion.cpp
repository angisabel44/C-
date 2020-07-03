#include "solucion.h"

solucion::solucion(int *_sol, instancia *mkp)
{
	sol = _sol;
	MKP = mkp;
	beneficio = 0;
	cantidadUnos = 0;

	for (int i = 0; i < MKP->getObjetos(); ++i)
	{
		beneficio += sol[i] * MKP->getBeneficio(i);
		if (sol[i] == 1)
			cantidadUnos++;
	}
}

void solucion::imprimirSolucion()
{
	/*
	cout << "La solucion es:" << endl;
	for (int i = 0; i < MKP->getObjetos(); ++i)
	{
		cout << " " << sol[i];
	} cout << endl;
	

	cout << "Beneficio: " << beneficio << endl;
	cout << "Valor optimo: " << MKP->getValorOptimo() << endl;

	if (factible())
	{
		cout << "Factibilidad: TRUE" << endl;
	} else
	{
		cout << "Factibilidad: FALSE" << endl;
	}
	*/

	string salida = "resultadosMKP";
	ofstream archivo(salida.c_str(), ios::app);

	string FileName = MKP->getNombre();
	string nombre = FileName;
	for (int i = 0; i < 3; ++i)
	{
		nombre = nombre.substr(nombre.find("/") + 1);

		if (i == 0)
		{
			string carpeta = nombre.substr(0, nombre.find("/"));
			//archivo << carpeta << " ";
		}
	}
	nombre = nombre.substr(0, nombre.find(".dat"));

	archivo << nombre << " & " << MKP->getValorOptimo() << " & " << beneficio << "  &";
	archivo.close();
	
	/*
	cout << "Beneficio: " << beneficio;
	
	if (factible())	cout << "\tFactibilidad: TRUE" << endl;
	else			cout << "\tFactibilidad: FALSE" << endl;
	*/
}

void solucion::imprimirSolucion2()
{
	string salida = "resultadosMKP";
	ofstream archivo(salida.c_str(), ios::app);

	archivo << beneficio << " & ";

	archivo.close();
}

void solucion::imprimirSolucion3()
{
	string salida = "resultadosMKP";
	ofstream archivo(salida.c_str(), ios::app);

	archivo << beneficio <<endl;

	archivo.close();
}

bool solucion::factible()
{
	int *cap = new int [MKP->getMochilas()];

	for (int i = 0; i < MKP->getMochilas(); ++i)
		cap[i] = 0;

	for (int i = 0; i < MKP->getMochilas(); ++i)
	{
		for (int j = 0; j < MKP->getObjetos(); ++j)
			cap[i] += sol[j] * MKP->getPeso(i, j);

		if (cap[i] > MKP->getCapacidad(i))
		{
			delete [] cap;
			return false;
		}
	}

	delete [] cap;
	return true;
}

void solucion::removerObjeto(int i)
{
	if (sol[i] == 1)
		sol[i] = 0;
}

void solucion::agregarObjeto(int i)
{
	if (sol[i] == 0)
		sol[i] = 1;
}