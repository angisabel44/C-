#include "heuristico.h"

bool comparacionSoluciones(solucion *a, solucion *b)
{
	return a->getBeneficio() > b->getBeneficio();
}

bool comparacionSolucionesUnos(solucion *a, solucion *b)
{
	return a->getCantidadUnos() > b->getCantidadUnos();
}

heuristico::heuristico(instancia *mkp)
{
	MKP = mkp;
}

solucion *heuristico::constructivo()
{
	float tiempo = clock();
	std::vector<int> orden;
	
	int *_sol = new int [MKP->getObjetos()];
	int *_cap = new int [MKP->getMochilas()];

	for (int i = 0; i < MKP->getObjetos(); ++i) 
		_sol[i] = 0;
	for (int i = 0; i < MKP->getMochilas(); ++i)
		_cap[i] = 0;
	for (int i = 0; i < MKP->getObjetos(); ++i)
		orden.push_back(i);

	random_shuffle(orden.begin(), orden.end());

	for (int i = 0; i < MKP->getObjetos(); ++i)
	{
		if (cabeObjeto(orden[i], _cap))
		{
			_sol[orden[i]] = 1;

			for (int j = 0; j < MKP->getMochilas(); ++j)
				_cap[j] += MKP->getPeso(j, orden[i]);
		}
	}

	solucion *sol = new solucion(_sol, MKP);
	delete [] _cap;

	//Guardamos la solucion en un archivo
	/*
	tiempo = (clock() - tiempo)/CLOCKS_PER_SEC;
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
			archivo << carpeta << " ";
		}
	}
	nombre = nombre.substr(0, nombre.find(".dat"));

	archivo << nombre << " " << MKP->getValorOptimo() << " " << sol->getBeneficio() << " " << std::fixed << tiempo << " ";
	archivo.close();
	*/

	return sol;
}

bool heuristico::cabeObjeto(int i, int *cap)
{
	for (int j = 0; j < MKP->getMochilas(); ++j)
	{
		if ((cap[j] + MKP->getPeso(j, i)) > MKP->getCapacidad(j))
		{
			return false;
		}
	} return true;
}

/*
void heuristico::imprimirPoblacion()
{
	for (list<solucion*>::iterator it = poblacion.begin(); it != poblacion.end(); ++it)
	{
		solucion *sol = *it;
		sol->imprimirSolucion();
	}
}
*/

solucion *heuristico::busquedaLocal(solucion *_sol, float alpha, int iteraciones)
{
	float tiempo = clock();
	int objetos = MKP->getObjetos();
	int dimensiones = MKP->getMochilas();
	int contador = 0;
	int numDeIteraciones = 0;
	
	solucion *mejorEnLaBusquedaLocal = generarCopia(_sol);
	solucion *solucionParaMoverme = generarCopia(_sol);

	list<objt> listaObjts = MKP->getListaObjts();

	do
	{
		int *solucionOrig = solucionParaMoverme->getSolucion();

		//Copia de la solucion y su capacidad.
		int *copiaSolucion;
		int *cap;

		copiaSolucion = new int [objetos];
		cap = new int [dimensiones];

		for (int i = 0; i < objetos; ++i)
			copiaSolucion[i] = solucionOrig[i];

		for (int i = 0; i < dimensiones; ++i)
		{
			cap[i] = 0;

			for (int j = 0; j < objetos; ++j)
			{
				int peso = MKP->getPeso(i, j);
				cap[i] += copiaSolucion[j] * peso;
			}
		}
		

		//Sacamos el objeto y actualizamos capacidad
		list<objt>::iterator it = listaObjts.end();

	    for (std::list<objt>::iterator i = listaObjts.begin(); i != listaObjts.end(); ++i)
	    {
	    	--it;
	    	int indice = it->id;

	    	if (copiaSolucion[indice] == 1)
	    	{
	    		copiaSolucion[indice] = 0;

	    		for (int j = 0; j < dimensiones; ++j)
	    		{
	    			int peso = MKP->getPeso(j, indice);
	    			cap[j] = cap[j] - peso;
	    		}
	    		break;
	    	}
	    }

	    //Creamos vecinos
	    list<solucion*> vecinos;

	    for (int i = 0; i < objetos; ++i)
	    {
	    	if (copiaSolucion[i] == 0)
	    	{
	    		if (cabeObjeto(i, cap))
	    		{
	    			int *nuevoVecino = generarVecino(copiaSolucion, i);
	    			solucion *sol = new solucion(nuevoVecino, MKP);

	    			vecinos.push_back(sol);
	    		}
	    	}
	    }

	    vecinos.sort(comparacionSoluciones);
	    
	    //Liberamos memoria, ya no los ocupo
	    delete [] copiaSolucion;
	    delete [] cap;

	    //Cantidad entre los que escogere para moverme
	    int cuantos = 0;
	    float tolerancia = (1 - alpha) * mejorEnLaBusquedaLocal->getBeneficio();

	    for (std::list<solucion*>::iterator i = vecinos.begin(); i != vecinos.end(); ++i)
	    {
	    	solucion *auxSol = *i;
	    	
	    	if (auxSol->getBeneficio() < tolerancia)
	    		break;
	    		
	    	cuantos++;
	    }

		//Lo escojo y guardo su direcccion de memoria
		int elElegido;
	    if (cuantos < 1)	elElegido = 0;
	    else				elElegido = rand() % cuantos;

	    list<solucion*>::iterator jt = vecinos.begin();
	    advance(jt, elElegido);

	    solucion *meMuevo = *jt;
	    vecinos.erase(jt); //Lo borro como iterador

		//Borro la lista ya no la ocupo.
	    while(!vecinos.empty()) {
	        solucion *s = vecinos.front();
	        vecinos.pop_front();
	        delete s;
	    }
	    
	    //Me muevo
	    delete solucionParaMoverme; //Borro memoria alojada
	    solucionParaMoverme = meMuevo; //Asigno nueva direccion de memoria
	    
	    contador++; numDeIteraciones++;
	    
	    if (mejorEnLaBusquedaLocal->getBeneficio() < solucionParaMoverme->getBeneficio())
	    {
	    	delete mejorEnLaBusquedaLocal;
	    	mejorEnLaBusquedaLocal = generarCopia(solucionParaMoverme);
	    	
	    	contador = 0;
		}
	    
	} while(contador < iteraciones);

	//Guardamos la solucion en un archivo
	/*
	string salida = "resultadosMKP";
	ofstream archivo(salida.c_str(), ios::app);

	tiempo = (clock() - tiempo)/CLOCKS_PER_SEC;

	archivo << iteraciones << " " << numDeIteraciones - iteraciones << " " << alpha << " " << mejorEnLaBusquedaLocal->getBeneficio() << " " << std::fixed << tiempo << endl;
	archivo.close();
	*/
	
	delete solucionParaMoverme;
	return mejorEnLaBusquedaLocal;
}

int *heuristico::generarVecino(int *_sol, int indice)
{
	int objetos = MKP->getObjetos();

	int *nuevaSolucion = new int [objetos];

	for (int i = 0; i < indice; ++i)
		nuevaSolucion[i] = _sol[i];

	nuevaSolucion[indice] = 1;

	for (int i = indice + 1; i < objetos; ++i)
		nuevaSolucion[i] = _sol[i];
	
	return nuevaSolucion;
}

solucion *heuristico::generarCopia(solucion *_sol)
{
	int objetos = MKP->getObjetos();
	int *sol = _sol->getSolucion();
	
	int *copiaSol;
	copiaSol = new int [objetos];
	
	for (int i = 0; i < objetos; ++i)
		copiaSol[i] = sol[i];
		
	solucion *S = new solucion(copiaSol, MKP);
		
	return S;
}

solucion *heuristico::busquedaDispersa(int num, int iteraciones)
{
	//cout << "Busqueda" << endl;
	int contador = 0;
	list<solucion*> poblacion;

	int numMejores = round(num * 0.1);
	int numDiversas = round(num * 0.1);

	//cout << "Creando poblacion:";
	for (int i = 0; i < num; ++i)
	{
		solucion *s = constructivo(); 
		if (i == 1) {s->imprimirSolucion();}
		
		solucion *S = busquedaLocal(s, 0.15, 16);
		if (i == 1) {S->imprimirSolucion2();}

		delete s;
		poblacion.push_back(S);
		//cout << ".";
	}
	poblacion.sort(comparacionSoluciones);
	//cout << "TERMINADO." << endl;
	//cout << "Poblacion de: " << poblacion.size() << endl;

	solucion *mejorEnBusquedaDispersa = generarCopia(poblacion.front());
	//mejorEnBusquedaDispersa->imprimirSolucion();

	do
	{
		//Creamos conjunto referencia
		//cout << "Creando conjunto referencia:";
		list<solucion*> conjuntoReferencia;

		list<solucion*>::iterator mejores = poblacion.begin();
		advance(mejores, numMejores + 1);

		for (list<solucion*>::iterator i = poblacion.begin(); i != mejores; ++i)
		{
			solucion *s = *i;
			conjuntoReferencia.push_back(s);
			//cout << ".";
		}

		poblacion.sort(comparacionSolucionesUnos);

		list<solucion*>::iterator diversas = poblacion.begin();
		advance(diversas, numDiversas + 1);

		for (list<solucion*>::iterator i = poblacion.begin(); i != diversas; ++i)
		{
			solucion *s = *i;
			conjuntoReferencia.push_back(s);
			//cout << ".";
		} //cout << "TERMIANDO" << endl;
		//cout << "Conjunto de referencia:" << conjuntoReferencia.size() << endl;

		//Combinamos soluciones
		//cout << "Combinado:";
		list<solucion*> solucionesCombinadas;
		for (std::list<solucion*>::iterator i = conjuntoReferencia.begin(); i != conjuntoReferencia.end(); ++i)
		{
			list<solucion*>::iterator x, y;
			x = conjuntoReferencia.begin();
			y = conjuntoReferencia.begin();

			int a, b; do
			{
				int tamano = conjuntoReferencia.size();
				a = rand() % tamano;
				b = rand() % tamano;
			} while (a == b);

			advance(x, a); advance(y, b);
			
			solucion *s1 = *x; //s1->imprimirSolucion();
			solucion *s2 = *y; //s2->imprimirSolucion();

			//cout << "Se esta trabando: ";
			solucion *s = combinar(s1, s2);
			//cout << "Salio" << endl;
			solucionesCombinadas.push_back(s);
			//cout << ".";
		} //cout << "TERMIANDO" << endl;
		//cout << "Cantidad de combinaciones: " << solucionesCombinadas.size() << endl;

		solucionesCombinadas.sort(comparacionSoluciones);
		poblacion.sort(comparacionSoluciones);

		poblacion.merge(solucionesCombinadas);

		//cout << "Poblacion de: " << poblacion.size() << endl;
		//cout << "Reajustando poblacion:";
		while(num < poblacion.size()) {
		    solucion *s = poblacion.back();
		    poblacion.pop_back();
		    delete s;
		    //cout << ".";
		} //cout << "TERMIANDO" << endl;
		//cout << "Poblacion reajustada: " << poblacion.size() << endl;

		solucion *encontrada = generarCopia(poblacion.front());
		if (mejorEnBusquedaDispersa->getBeneficio() < encontrada->getBeneficio())
		{
			delete mejorEnBusquedaDispersa;
			mejorEnBusquedaDispersa = encontrada;
			//mejorEnBusquedaDispersa->imprimirSolucion();
		} else {
			delete encontrada;
		}

		conjuntoReferencia.clear();
		contador++;

	} while (contador < iteraciones);

	return mejorEnBusquedaDispersa;
}

solucion *heuristico::combinar(solucion *sol_1, solucion *sol_2)
{
	int objetos = MKP->getObjetos();

	int *s_1 = sol_1->getSolucion();
	int *s_2 = sol_2->getSolucion();
	int *sol = new int [objetos];

	int k = rand() % objetos;

	for (int i = 0; i < k; ++i)
		sol[i] = s_1[i];
	for (int i = k; i < objetos; ++i)
		sol[i] = s_2[i];


	solucion *S = new solucion(sol, MKP);
	//S->imprimirSolucion();
	//system("pause");

	if(S->factible()) {
		meterObjetos(S);
	}
	else {
		quitarObjetos(S);
	}

	solucion *Sol = busquedaLocal(S, 0.15, 16);

	delete S;
	return Sol;
}

void heuristico::meterObjetos(solucion *sol)
{
	//cout << "Meter" << endl;
	list<objt> listaObjts = MKP->getListaObjts();
	list<objt>::iterator it = listaObjts.begin();
	list<objt>::iterator jt = listaObjts.end();

	while(sol->factible()) {
	    int i = it->id;
	    sol->agregarObjeto(i);
	    ++it;
	}

	while(!sol->factible()) {
		--jt;
	    int i = jt->id;
	    sol->removerObjeto(i);
	}
}

void heuristico::quitarObjetos(solucion *sol)
{
	//cout << "Quitar" << endl;
	//sol->imprimirSolucion();
	list<objt> listaObjts = MKP->getListaObjts();
	list<objt>::iterator jt = listaObjts.end();

	while(!sol->factible()) {
		//cout << "Factible: " << sol->factible() << endl;
		--jt;
	    int i = jt->id;
	    //cout << i << endl;
	    sol->removerObjeto(i);
	    //sol->imprimirSolucion();
	}
}
