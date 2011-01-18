#include <iostream>
#include "GGUtil.h"
#include "GGIndividual.h"
#include "GGPopulation.h"

int main (int argc, char * const argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
	
	srand(time(NULL));
	
	/*vertex_set perm = GG_randperm(10);
	vertex_set ppl = GG_perm2picklist(perm);
	vertex_set perm2 = GG_picklist2perm(ppl);
	
	GG_printset(perm);
	GG_printset(ppl);
	GG_printset(perm2);
	*/
	
	FILE *f = fopen("../../graph.txt", "r");
	if (!f) {
		f = fopen("graph.txt", "r");
	}
	GG_loadMatrixFile(f, GGIndividual::adj_matrix, GGIndividual::adj_matrix_size, GGIndividual::adj_matrix_size);
	
	fclose(f);
	/*
	double maxfit = 0;
	
	for (int i=0; i<100; i++) {
		GGIndividual *ind = new GGIndividual(GGIndividual::adj_matrix_size);
		printf("\n\n");
		GGIndividual::crossoverLP(*ind, ind->mutate()).print();
		printf("\n");
		ind->print();
		maxfit = MAX(ind->fitness(), maxfit);
	}
	*/
	
	/*GGPopulation gap(11, GGIndividual::adj_matrix_size);
	
	for (int i=0; i<200; i++) {
		gap.print();
		gap = GGPopulation::generate(gap, 0, 10);
	}*/
	
	cout << "Parallel graph partitioning genetic algorithm demonstration\n";
	cout << "Streltsov A.A.\n\n";
	
	GGPopulation g = GG_evolve(500, 100, GGIndividual::adj_matrix_size, 60, 30);
	g.best_ind().print();
	
	cout << "\n";
	
	return 0;
}
