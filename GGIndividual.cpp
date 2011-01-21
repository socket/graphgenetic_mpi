/*
 *  GGIndividual.cpp
 *  graphgenetic
 *
 *  Created by Alexey Streltsow on 1/17/11.
 *  Copyright 2011 Zila Networks LLC. All rights reserved.
 *
 */


#include "GGIndividual.h"

double* GGIndividual::adj_matrix = NULL;
int			GGIndividual::adj_matrix_size = 0;

GGIndividual::GGIndividual(int numvert) {
	_numvert = numvert;
	int leftsize = rand()%(numvert-1) + 1;
	_left.resize( leftsize );
	_right.resize( numvert - leftsize );
	
	vertex_set tmp = GG_randperm(numvert);
	for (int i=0; i<_left.size(); i++) {
		_left[i] = tmp[i];
	}
	for (int i=_left.size(); i<_numvert; i++ ) {
		_right[i-_left.size()] = tmp[i];
	}
	
	_picklist = GG_perm2picklist(tmp);
	
	evalFitness();
}

GGIndividual::GGIndividual(const vertex_set& l, const vertex_set& r) {
	_numvert = l.size() + r.size();
	vertex_set tmp;
	tmp.resize(_numvert);
	
	_left.resize(l.size());
	for(int i=0; i<l.size(); i++) {
		_left[i] = l[i];
		tmp[i] = l[i];
	}
	
	_right.resize(r.size());
	for(int i=0; i<r.size(); i++) {
		_right[i] = r[i];
		tmp[i] = r[i];
	}
	
	_picklist = GG_perm2picklist(tmp);
	evalFitness();
}

void GGIndividual::serialize(char* &buff, int &size) const {
	int l, r, ps;
	l = _left.size();
	r = _right.size();
	ps = _picklist.size();
	
	size = sizeof(int) * _picklist.size() + 2 ;
	
	buff = (char*)malloc( size );
	memcpy(buff, &l, sizeof(int));
	memcpy(buff+4, &r, sizeof(int));
	memcpy(buff+8, &ps, sizeof(int));
	memcpy(buff+12, &_picklist[0], sizeof(int)*ps);
	
}

GGIndividual GGIndividual::deserialize(char *buff, int size) {
	int l, r, ps;
	memcpy(&l, buff, 4);
	memcpy(&r, buff+4, 4);
	memcpy(&ps, buff+8, 4);
	
	vertex_set _left;
	vertex_set _right;
	vertex_set _picklist;
	
	_picklist.reserve(ps);
	_left.reserve(l);
	_right.reserve(r);
	
	memcpy(&_picklist[0], buff+12, ps*4);
	
	vertex_set tmp = GG_picklist2perm(_picklist);
	
	for (int i=0; i<_left.size(); i++) {
		_left[i] = tmp[i];
	}
	for (int i=0; i<_right.size(); i++) {
		_right[i] = tmp[_right.size()+i];
	}
	
	return GGIndividual(_left, _right);
}


void GGIndividual::print() const {
	cout << "{ ";
	GG_printset(_left);
	GG_printset(_right);
	cout << " " << _fitness << "(" << ((1.0f / _fitness)-1) << ")" << " }";
}

/* private */
void GGIndividual::evalFitness() {
	int sum = 0;
	for(int i=0; i<_left.size(); i++) {
		for(int j=0; j<_right.size(); j++) {
			if ( adj_matrix[_left[i]*adj_matrix_size + _right[j]] == 1.0f ) {
				sum++;
			}
		}
	}
	_fitness = 1.0f / (1.0f + sum);
}

GGIndividual GGIndividual::mutate() const {
	vertex_set child_picklist;
	child_picklist.resize(_numvert);
	for (int i=0; i<_numvert; i++) {
		child_picklist[i] = _picklist[i];
	}
	
	// mutate one point
	int m_point = GG_randint(_numvert-1);
	child_picklist[m_point] = GG_randint(_numvert - m_point);
	
	vertex_set tmp = GG_picklist2perm(child_picklist);
	vertex_set l, r;
	
	if ( GG_randboolean() ) {
		// keep the structure
		if ( GG_randboolean() ) {
			l.resize( _left.size() );
			r.resize( _right.size() );
		}
		else {
			r.resize( _left.size() );
			l.resize( _right.size() );
		}
		
	}
	else {
		// mutate structure
		int dpoint = 1 + GG_randint(_numvert-1);
		l.resize( dpoint );
		r.resize( _numvert - dpoint );
	}
	
	for (int i=0; i<l.size(); i++) {
		l[i] = tmp[i];
	}
	for (int i=0; i<r.size(); i++) {
		r[i] = tmp[l.size()+i];
	}
	
	return GGIndividual(l, r);
}

GGIndividual GGIndividual::crossoverLP(const GGIndividual& f, const GGIndividual& m) {
	int xpoint = 1 + GG_randint(f._numvert - 1);
	vertex_set child_picklist;
	child_picklist.resize((f._numvert));
	
	// X-over chromosomes
	for (int i=0; i<xpoint; i++) {
		child_picklist[i] = f._picklist[i];
	}
	for (int i=xpoint; i<f._numvert; i++) {
		child_picklist[i] = m._picklist[i];
	}
	
//	GG_printset(child_picklist);

//	cout << flush;
	
	vertex_set tmp = GG_picklist2perm(child_picklist);
	vertex_set l, r;
	
	// assumptions mostly (non ideal?)
	if ( GG_randboolean() ) {
		l.resize( f._left.size() );
		r.resize( tmp.size() - f._left.size());
	}
	else {
		l.resize( m._left.size() );
		r.resize( tmp.size() - m._left.size());
	}

	for (int i=0; i<l.size(); i++) {
		l[i] = tmp[i];
	}
	for (int i=0; i<r.size(); i++) {
		r[i] = tmp[l.size()+i];
	}
	
	return GGIndividual(l, r);
}



