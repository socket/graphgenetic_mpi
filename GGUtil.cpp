/*
 *  GGUtil.cpp
 *  graphgenetic
 *
 *  Created by Alexey Streltsow on 1/17/11.
 *  Copyright 2011 Zila Networks LLC. All rights reserved.
 *
 */
const int LINE_BUFFER_SIZE = 0xFF;

#define AI(arr, size, i, j) arr[i*(size) + j]

#include "GGUtil.h"

vertex_set GG_picklist2perm(vertex_set& pl) {
	vertex_set perm;
	vertex_set tmp;
	
	for(int i=0; i<pl.size(); i++) {
		tmp.push_back(i);		
	}
	
	for(int i=0; i<pl.size(); i++) {
		int pli = pl[i];
		if ( pl[i] >= tmp.size() ) {  /* <<----| */
			pli = tmp.size() - 1;
			assert(pli >= 0);
		}
		
		perm.push_back(tmp[pli]);
		tmp.erase( tmp.begin() + pli );
	}
	
	return perm;
}

vertex_set GG_perm2picklist(vertex_set& pr) {
	vertex_set pl;
	vertex_set tmp;
	
	pl.resize(pr.size());
	
	for(int i=0; i<pr.size(); ++i) {
		tmp.push_back(i);
	}
	
	for(int i=0; i<pr.size(); ++i) {
		int index;
		for(int k=0; k<tmp.size(); ++k) {
			if ( tmp[k] == pr[i] ) {
				index = k;
				tmp.erase(tmp.begin() + index);
				break;
			}
		}
		pl[i] = index;
	}
	
	return pl;
}

vertex_set GG_randperm(int n) {
	vertex_set perm;
	perm.resize(n);
	for(int i=0; i<n; i++) {
		perm[i] = i;
	}
	for(int i=0; i<n-1; i++ ) {
		int j = rand()%(n-i) + i;
		int temp = perm[i];
		perm[i] = perm[j];
		perm[j] = temp;
	}
	return perm;
}

void GG_printset(const vertex_set& s) {
	cout << '[';
	for (int i=0; i<s.size(); ++i) {
		cout << s[i] << ' ';
	}
	cout << ']';
}

bool GG_loadMatrixFile(FILE* f, double* &adj, int &n, int &m) {
	if ( !f ) {
		cerr << "load matrix file failed";
		return false;
	}
	
	char buff[LINE_BUFFER_SIZE+1];
	buff[LINE_BUFFER_SIZE] = 0xFF; // guard
	memset(buff, LINE_BUFFER_SIZE, 0x00);
	
	vector<vertex_set> lines;
	
	while(char *pbl = fgets(buff, LINE_BUFFER_SIZE, f)) {
		if ( *pbl == '/' && *(1+pbl) == '/' ) {
			continue; // skip comments
		}

		while( *pbl == ' ' ) pbl++;
		if ( *pbl == '\n' )
			continue;
		
		vertex_set newline;
		pbl--;
		
		do {
			int value = atoi(++pbl);
			newline.push_back( value );
		}
		while(pbl = strchr(pbl, ' '));
		
		lines.push_back(newline);
	}
	
	vector<vertex_set> :: iterator line_it = lines.begin();
	vector<vertex_set> :: iterator line_it_end = lines.end();
	
	int max_index = 0;
	// determine max index
	for (int i=0; line_it != line_it_end; ++line_it, ++i ) {
		vertex_set& tmp = *line_it;
		if ( tmp[0] > max_index ) {
			max_index = (int) tmp[0];
		}
		if ( tmp[1] > max_index ) {
			max_index = (int) tmp[1];
		}
	}
	
	adj = (double*)malloc( (max_index+1) * (max_index+1) * sizeof( double ) );
	memset(adj, 0, (max_index+1)*(max_index+1)*sizeof(double));
	
	line_it = lines.begin();
	for (int i=0; line_it != line_it_end; ++line_it, ++i ) {
		vertex_set& tmp = *line_it;
		int i = (int)tmp[0];
		int j = (int)tmp[1];
		assert(tmp[2]);
		
		AI(adj, max_index+1, i, j) = tmp[2];
		AI(adj, max_index+1, j, i) = tmp[2];
		cout << i << "," << j << " " << tmp[2] << "\n";
	}
	
	cout << "\n";
	for (int i=0; i<=max_index; i++) {
		for (int j=0; j<=max_index; j++) {
			cout << AI(adj, max_index+1, i, j) << " ";
		}
		cout << "\n";
	}
	
	n = m = max_index+1;
	
	return true;
}

bool GG_randboolean() {
	return (rand()%1) == 1;
}

int	GG_randint(int max) {
	return rand()%max;
}

