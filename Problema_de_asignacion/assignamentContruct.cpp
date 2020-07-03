#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <list>

using namespace std;

struct task
{
	int id;
	int cost;
};

struct machine
{
	int id;
	std::list<task> tasks;
};

struct assignment
{
	int mcn;
	int tsk;
	int cost;
};

list<machine> readData(string fileName);
list<assignment> constructor(list<machine> machines);
void saveSolution(list<assignment> solution, string fileName, float tiempo);

int machineMaxCost(list<machine> machines);
int taskMinCost(std::list<machine>::iterator it);
int costTotal(std::list<assignment> solution);

void impresion(list<machine> machines);

int main(int argc, char const *argv[])
{

	float tiempo = clock();
	list<machine> machines = readData(argv[1]);
	list<assignment> solution = constructor(machines);
	saveSolution(solution, argv[1], tiempo);

	return 0;

}

list<machine> readData(string fileName) {
	ifstream leer(fileName.c_str());
	int size;
	list<machine> machines;

	cout << "Leyendo instancia: ";

	leer >> size;

	for (int i = 0; i < size; i++)
	{
		machine maq;
		maq.id = i + 1;

		for (int j = 0; j < size; j++)
		{
			task tar;
			tar.id = j + 1;
			leer >> tar.cost;

			maq.tasks.push_back(tar);
		}

		machines.push_back(maq);
	}

	cout << "Listo!!" << endl;

	return machines;

}

void impresion(list<machine> machines) {
	for (std::list<machine>::iterator i = machines.begin(); i != machines.end(); ++i)
	{
		cout << "Maquina: " << i->id << endl;
		for (std::list<task>::iterator j = i->tasks.begin(); j != i->tasks.end(); ++j)
		{
			//cout << "[" << j->id << ", " << j->cost << "] ";
		}

		//cout << endl;
	}
}

list<assignment> constructor(list<machine> machines) {
	list<assignment> solution;

	cout << "Creando solucion: ";

	while (!machines.empty()) {
		assignment sol;

		int maquina = machineMaxCost(machines);
		std::list<machine>::iterator it = machines.begin();
		std::advance(it, maquina);

		int tarea = taskMinCost(it);
		std::list<task>::iterator jt = it->tasks.begin();
		std::advance(jt, tarea);

		sol.mcn = it->id;
		sol.tsk = jt->id;
		sol.cost = jt->cost;

		solution.push_back(sol);

		machines.erase(it);
		for (std::list<machine>::iterator i = machines.begin(); i != machines.end(); ++i)
		{
			std::list<task>::iterator k = i->tasks.begin();
			std::advance(k, tarea);
			i->tasks.erase(k);
		}
	}

	cout << "Creada!!" << endl;

	return solution;
}

int machineMaxCost(list<machine> machines) {
	int max = machines.front().tasks.front().cost;
	int num = 0, it = 0;

	for (std::list<machine>::iterator i = machines.begin(); i != machines.end(); ++i)
	{
		for (std::list<task>::iterator j = i->tasks.begin(); j != i->tasks.end(); ++j)
		{
			if (max < j->cost) {
				max = j->cost;
				num = it;
			}
		}

		it++;
	}

	return num;
}

int taskMinCost(std::list<machine>::iterator it) {
	int min = it->tasks.front().cost;
	int tarea = 0, iter = 0;

	for (std::list<task>::iterator i = it->tasks.begin(); i != it->tasks.end(); ++i)
	{
		if (min > i->cost)
		{
			min = i->cost;
			tarea = iter;
		}

		iter++;
	}

	return tarea;
}

void saveSolution(list<assignment> solution, string fileName, float tiempo) {
	string salida;
	salida = "sol_" + fileName;
	ofstream file(salida.c_str());
	float t = (clock() - tiempo) / CLOCKS_PER_SEC;

	cout << "Creando archivo: ";

	file << (int)costTotal(solution) << " " << t << endl;

	for (std::list<assignment>::iterator i = solution.begin(); i != solution.end(); ++i)
	{
		file << i->mcn << " " << i->tsk << " " << i->cost << endl;
	}

	file.close();

	cout << "Creado!!" << endl;
}

int costTotal(std::list<assignment> solution) {
	int cost = 0;

	for (std::list<assignment>::iterator i = solution.begin(); i != solution.end(); ++i)
	{
		cost = cost + i->cost;
	}

	return cost;
}