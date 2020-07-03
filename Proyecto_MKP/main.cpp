#include "Header.h"
#include "instancia.h"
#include "heuristico.h"
#include "solucion.h"

int main(int argc, char const *argv[])
{
	srand(time(NULL));
	
	instancia *MKP = new instancia(argv[1]);
	heuristico *H = new heuristico(MKP);
	//H->imprimirPoblacion();
	//solucion *sol = H->constructivo();

	//delete sol;
	delete H;
	delete MKP;

	return 0;
}
