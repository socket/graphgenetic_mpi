
#undef SEEK_SET
#undef SEEK_END
#undef SEEK_CUR
#include <mpi.h>

#include <iostream>
#include "GGUtil.h"
#include "GGIndividual.h"
#include "GGPopulation.h"

int main (int argc, char **argv) {
	int taskid = -1;
	int numtasks = -1;
	
	if ( MPI_Init(&argc, &argv) != MPI_SUCCESS ) {
		cout << "MPI init failed";
	}
	else {
		MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
		MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	}
	
	srand(time(NULL));
	
	FILE *f = fopen("../../graph.txt", "r");
	if (!f) {
		f = fopen("graph.txt", "r");
	}
	GG_loadMatrixFile(f, GGIndividual::adj_matrix, GGIndividual::adj_matrix_size, GGIndividual::adj_matrix_size);
	
	fclose(f);

	cout << "Parallel graph partitioning genetic algorithm demonstration\n";
	cout << "Streltsov A.A.\n\n";
	
	GGPopulation g = GG_evolve(500, 100, GGIndividual::adj_matrix_size, 60, 30);
	g.best_ind().print();
	
	cout << "\n";
	
	MPI_Finalize();
	
	return 0;
}
