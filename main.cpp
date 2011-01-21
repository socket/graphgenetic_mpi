
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
	
	int numGens = 100;
	int genSize = 50;
	int mrate = 30;
	int xrate = 60;
	int criteria = 0;
	
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
			int size;
			fscanf(ff, "%d,%d,%d,%d,%d,%d", &size, &criteria, &numGens, &genSize, &mrate, &xrate);
			fclose(ff);
			
			GGIndividual::adj_matrix_size = size;
			GG_genMatrix(GGIndividual::adj_matrix, GGIndividual::adj_matrix_size);
			
			cout << "RNDMATRIX rank=" << size << "\n";
			cout << "criteria=" << criteria << " gens=" << numGens << " size=" << genSize << "\n";
			cout << "mrate=" << mrate << " xrate=" << xrate;
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
		
//	GGPopulation g = GG_evolve(numGens, genSize, GGIndividual::adj_matrix_size, mrate, xrate);
	GGPopulation gap(genSize, GGIndividual::adj_matrix_size);
	
	bool found = false;
	
	char mpi_finish = 1;
	for (int i=1; i<numGens; i++) {
		gap = GGPopulation::generate(gap, xrate, mrate);
		
		int flag;
		MPI_Iprobe(MPI_ANY_SOURCE, 1111, MPI_COMM_WORLD, &flag, &status);
		if ( flag ) {
			found = true;
			//cout << "#" << taskid << ":: terminating further computations\n";
			break;
		}
		
		if (gap.best_ind().fitness() >= (1.0f/(1.0f+criteria) - 0.00001) ) {
			cout << "#" << taskid << ":: Found criteria match on iteration=" << i << "/" << numGens << "\n";
			found = true;
			for (int t=0; t<numtasks; t++) {
				MPI_Request req;
				if ( t != taskid ) {
					MPI_Isend(&mpi_finish, 1, MPI_CHAR, t, 1111, MPI_COMM_WORLD, &req);
				}
			}
			gap.best_ind().print();
			break;
		}
	}
	
	if (!found) {
		cout << "criteria match failed" << "\n";
		gap.best_ind().print();		
	}
	
	cout << "\n";
	
	MPI_Finalize();
	
	return 0;
}

