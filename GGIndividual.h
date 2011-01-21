/*
 *  GGIndividual.h
 *  graphgenetic
 *
 *  Created by Alexey Streltsow on 1/17/11.
 *  Copyright 2011 Zila Networks LLC. All rights reserved.
 *
 */

#ifndef _GG_Individual_h
#define _GG_Individual_h

#include "GGUtil.h"

class GGIndividual {
	int				 _numvert;
	double		 _fitness;
	

	vertex_set _picklist;
	
	void evalFitness();
	
public:
	vertex_set _left;
	vertex_set _right;
	
	GGIndividual(int numvert);
	GGIndividual(const vertex_set& l, const vertex_set& r);
	
	GGIndividual mutate() const;
	static GGIndividual crossoverLP(const GGIndividual& f, const GGIndividual& m);
	
	inline int vertices() const { 
		return _numvert;
	}
	
	inline double fitness() const {
		return _fitness;
	}
	
	static double* adj_matrix;
	static int	adj_matrix_size;
	
	void print() const;
	
	void serialize(char* &buff, int &size) const;
	static GGIndividual deserialize(char *buff, int size);
	
};

#endif //_GG_Individual_h

