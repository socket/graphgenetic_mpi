/*
 *  Population.cpp
 *  graphgenetic
 *
 *  Created by Alexey Streltsow on 1/17/11.
 *  Copyright 2011 Zila Networks LLC. All rights reserved.
 *
 */

#include "GGPopulation.h"

GGPopulation::GGPopulation(int size, int nvert) {
	_popsize = size;
	for (int i=0; i<size; i++ ) {
		_ind.push_back( GGIndividual(nvert) );
	}
	evaluate();
}

GGPopulation::GGPopulation(const individual_list& individuals) {
	_popsize = individuals.size();
	for (int i=0; i<individuals.size(); ++i) {
		_ind.push_back( GGIndividual( individuals[i] ) );
	}
	evaluate();
}

GGPopulation GGPopulation::generate(const GGPopulation& p, int xrate, int mrate) {
	individual_list newg;
	
	// number of offsprings to be produced by crossover and mutations
	int xn = xrate * p._popsize/100;
	int mn = mrate * p._popsize/100;
	
	for (int i=0; i<xn; i++) {
		// find 2 parents for X-over
		int p1, p2;
		do {
			p1 = p.select();
			p2 = p.select();
		} while (p1 == p2);
		
		newg.push_back( GGIndividual::crossoverLP(p._ind[p1], p._ind[p2]) );
	}
	
	// mutation
	for(int i=0; i<mn; i++) {
		newg.push_back( p._ind[p.select()].mutate() );
	}
	
	// rest are reproduction
	for (int i=newg.size(); i<p._popsize; i++) {
		newg.push_back( p._ind[p.select()] );
	}
	
	return GGPopulation( newg );
}

int	GGPopulation::select() const{
	// tournament selection of size pop_size/10
	int s_index = GG_randint(_popsize);
	float s_fitness = _ind[s_index].fitness();
	
	int tr_size = MIN(10, _popsize);
	for (int i=1; i < tr_size; i++) {
		int tmp = GG_randint(_popsize);
		if ( _ind[tmp].fitness() > s_fitness ) {
			s_index = tmp;
			s_fitness = _ind[tmp].fitness();
		}
	}
	return s_index;
}

void GGPopulation::evaluate() {
	int best = 0;			// index of best individual
	double best_fitness = _ind[0].fitness();
	int worst = 0;
	double worst_fitness = _ind[0].fitness();
	double sum = _ind[0].fitness(); // sum of the fitness of individuals of this population

	for (int i=1; i<_popsize; i++) {
		sum += _ind[i].fitness();
		if ( _ind[i].fitness() > best_fitness ) {
			best = i;
			best_fitness = _ind[i].fitness();
		}
		else if (_ind[i].fitness() < worst_fitness ) {
			worst = i;
			worst_fitness = _ind[i].fitness();
		}
	}
	_best_index = best;
	_worst_index = worst;
	_avr_fitness = sum / (double)_popsize;
}

void GGPopulation::print() const {
	cout << "< best:";
	_ind[_best_index].print();
	cout << " worst:";
	_ind[_worst_index].print();
	cout << "  " << _avr_fitness << ">\n";
}