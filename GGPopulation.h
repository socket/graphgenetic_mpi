/*
 *  Population.h
 *  graphgenetic
 *
 *  Created by Alexey Streltsow on 1/17/11.
 *  Copyright 2011 Zila Networks LLC. All rights reserved.
 *
 */

#ifndef _GG_Population_h
#define _GG_Population_h

#include "GGIndividual.h"

typedef std::vector<GGIndividual> individual_list;

class GGPopulation {
	int							_popsize;			// population size	
	individual_list _ind;
	
	void evaluate();
	
public:
	GGPopulation(int size, int nvert);
	GGPopulation(const individual_list& individuals);
	
	static GGPopulation generate(const GGPopulation& p, int xrate, int mrate);
	
	int	select() const;
	
	inline const GGIndividual& best_ind() const {
		return _ind[_best_index];
	}
	
	int _best_index;
	int _worst_index;
	
	float _avr_fitness;
	
	void print() const;
};

GGPopulation GG_evolve(int gens, int pop_size, int nvert, int xrate, int mrate);

#endif //_GG_Population_h