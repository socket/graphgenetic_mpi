/*
 *  GGCommon.h
 *  graphgenetic
 *
 *  Created by Alexey Streltsow on 1/17/11.
 *  Copyright 2011 Zila Networks LLC. All rights reserved.
 *
 */

#ifndef _GG_Util_h
#define _GG_Util_h

#include <set>
#include <list>
#include <vector>
#include <iostream>
#include <assert.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

using namespace std;

typedef std::vector<int> vertex_set;
typedef std::vector<int>::iterator vertex_it;

vertex_set GG_picklist2perm(vertex_set& pl);
vertex_set GG_perm2picklist(vertex_set& pr);
vertex_set GG_randperm(int n);

bool GG_loadMatrixFile(FILE* f, double* &adj, int &n, int &m);

void GG_printset(const vertex_set& s) ;

bool GG_randboolean();
int	 GG_randint(int max);
void GG_genMatrix(double* &adj, int size );

#endif //_GG_Util_h

