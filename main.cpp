
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
	int msize;
	MPI_Status status;
	
	if ( taskid == 0 ) {
		cout << "Parallel graph partitioning genetic algorithm demonstration\n";
		cout << "Streltsov A.A.\n\n";

		FILE *f = fopen("../../graph.txt", "r");
		if (!f) {
			f = fopen("graph.txt", "r");
		}
		GG_loadMatrixFile(f, GGIndividual::adj_matrix, GGIndividual::adj_matrix_size, GGIndividual::adj_matrix_size);

		fclose(f);

		if (FILE* ff = fopen("random", "r") ) {
			char buff[128];
			fgets(buff, 128, ff);
			int size = atoi(buff);
			fclose(ff);
			if ( size<5) size = 10;
			
			GGIndividual::adj_matrix_size = size;
			GG_genMatrix(GGIndividual::adj_matrix, GGIndividual::adj_matrix_size);
		}
		msize = GGIndividual::adj_matrix_size;
		for (int i=1; i<numtasks; ++i) {
			MPI_Send(&msize, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(GGIndividual::adj_matrix, msize*msize, MPI_DOUBLE, i, 2, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Recv(&msize, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		
		GGIndividual::adj_matrix = (double*)malloc(sizeof(double)*msize*msize);
		GGIndividual::adj_matrix_size = msize;
		
		MPI_Recv(GGIndividual::adj_matrix, msize*msize, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &status);
		
	  cout << "#" << taskid << " received problem matrix of size=" << msize << "\n";
	}
		
	GGPopulation g = GG_evolve(100, 50, GGIndividual::adj_matrix_size, 60, 30);
	
	if ( taskid == 0 ) {
		g.best_ind().print();
	}
	
	cout << "\n";
	
	MPI_Finalize();
	
	return 0;
}

