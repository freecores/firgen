/*
 * firgen is the name of the Programm which is optimized for creating FIR filter with less resources
 * copyright (C) 2007 
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU 
 * General Public License as published by the Free Software Foundation; either version 3 of the 
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without 
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with this program; if not, 
 * /see <http://www.gnu.org/licenses/>.
*/

/***************************************************************************
                          dna.cpp  -  description
                             -------------------
    begin                : Mon Oct 15 2001
    copyright            : (C) 2001 by otto stephan
    email                : ottosn
 ***************************************************************************/

#include "dna.h"
#include <stdio.h>
#include <limits.h>
#include <iostream.h>

dna::dna( unsigned int lines){
	lineCnt = lines;
	nodeLine = new map< unsigned int, dnaNode>*[lineCnt];
	for ( unsigned int i=0;i<lineCnt;i++) {
		nodeLine[i] = new map< unsigned int, dnaNode>;
	}
	activeGenIterLine = -1;
}
//copy constructor
dna::dna( dna &DNA) {
	lineCnt = DNA.lineCnt;
	nodeLine = new map< unsigned int, dnaNode>*[lineCnt];
	for ( unsigned int i=0;i<lineCnt;i++) {
		nodeLine[i] = new map< unsigned int, dnaNode>;
		// now copy all the nodes
		DNA.iter = DNA.nodeLine[i]->begin();
		while( DNA.iter != DNA.nodeLine[i]->end()) {
			(*nodeLine[i])[DNA.iter->first].mC = new moduleComponent;
			(*nodeLine[i])[DNA.iter->first].mC->alpha = DNA.iter->second.mC->alpha;
			(*nodeLine[i])[DNA.iter->first].mC->beta = DNA.iter->second.mC->beta;
			(*nodeLine[i])[DNA.iter->first].mC->gamma = DNA.iter->second.mC->gamma;
			(*nodeLine[i])[DNA.iter->first].mC->klasse = DNA.iter->second.mC->klasse;
			DNA.iter++;
		}
	}
	activeGenIterLine = -1;
}
// merge all dna's to new one
// all dna's must have the same number of lines
// otherwise the application will killed
dna::dna( dna **DNA, unsigned int cnt) {

	// variables needed for merging:
	unsigned int min = UINT_MAX;
	unsigned int minCnt = 0;
	unsigned int choosenDNA = 0;
	unsigned int choosenDNAVirtual = 0;
	unsigned int lastValid = cnt;
	unsigned int valid;
	
	lineCnt = DNA[0]->lineCnt;
	nodeLine = new map< unsigned int, dnaNode>*[lineCnt];
	for ( unsigned int l=0;l<lineCnt;l++) {
		nodeLine[l] = new map< unsigned int, dnaNode>;
		// initialize all iterators from current line in alien dna's
		for ( unsigned int it=0; it<cnt; it++)
			DNA[it]->iter = DNA[it]->nodeLine[l]->begin();
		while( lastValid > 0) {
			min = UINT_MAX;
			minCnt = 0;
			choosenDNAVirtual = 0;
			// check if an iterator the end has reached
			valid = 0;
			while ( valid != lastValid) {
				if ( DNA[valid]->iter == DNA[valid]->nodeLine[l]->end()) {
					lastValid--;
					dna *dnaDummy = DNA[lastValid];
					DNA[lastValid] = DNA[valid];
					DNA[valid] = dnaDummy;
				}
				else valid++;
			}
			if ( !lastValid) { break;}
			// search min
			for ( unsigned int n=0; n<lastValid; n++)
				if ( DNA[n]->iter->first < min) min = DNA[n]->iter->first;
			// count number of min's
			for ( unsigned int n=0; n<lastValid; n++)
				if ( DNA[n]->iter->first == min) minCnt++;
			// take from random choosen dna a gen
			choosenDNA = rand() % minCnt;
			for ( unsigned int n=0; n<lastValid; n++) {
				if ( DNA[n]->iter->first == min) {
					if ( choosenDNAVirtual == choosenDNA) {
						choosenDNA = n;
						//printf("take gen from dna %d\n", n);
						break;
					}
					choosenDNAVirtual++;
				}
			}
			(*nodeLine[l])[min].mC = new moduleComponent;
			(*nodeLine[l])[min].mC->alpha = DNA[choosenDNA]->iter->second.mC->alpha;
			(*nodeLine[l])[min].mC->beta = DNA[choosenDNA]->iter->second.mC->beta;
			(*nodeLine[l])[min].mC->gamma = DNA[choosenDNA]->iter->second.mC->gamma;
			(*nodeLine[l])[min].mC->klasse = DNA[choosenDNA]->iter->second.mC->klasse;
			//(*nodeLine[l])[min] = DNA[choosenDNA]->
			// go to next value
			for ( unsigned int n=0; n<lastValid; n++) {
				if (DNA[n]->iter->first == min) DNA[n]->iter++;
			}
			// reset a few values
		}
		lastValid = cnt;
	}
}


// insert or replace moduleComponent
void dna::setModul( unsigned int line, unsigned int node, moduleComponent *mC) {
	if ( line < 1 || line > (unsigned int)lineCnt) return;
	iter = nodeLine[ line - 1]->find( node);
	// if exist, delete before insert
	if ( iter != nodeLine[line - 1]->end()) delete iter->second.mC;
	(*nodeLine[ line - 1])[node].mC = mC;
}

dna::~dna(){
	for ( unsigned int i=0; i<lineCnt; i++) {
		iter = nodeLine[i]->begin();
		while( iter != nodeLine[i]->end()) {
			delete iter->second.mC;
			iter++;
		}
		delete nodeLine[i];
	}
	delete nodeLine;
}
moduleComponent *dna::getModul( unsigned int line, unsigned int node) {
	if ( line < 1 || line > (unsigned int)lineCnt) return 0;
	iter = nodeLine[ line - 1]->find( node);
	if ( iter != nodeLine[line - 1]->end())
		return iter->second.mC;
	return 0;
}
// return true, if modul already exist
bool dna::existModul( unsigned int line, unsigned int node) {
	if ( line < 1 || line > (unsigned int)lineCnt) return false;
	if ( nodeLine[line - 1]->end() != nodeLine[line - 1]->find( node))
		return true;
	return false;
}

// mark all used genes
void dna::markActiveGenes() {
	// mark all coeffs
	iter = nodeLine[ lineCnt - 1]->begin();
	while ( iter != nodeLine[ lineCnt - 1]->end()) {
		iter->second.active = true;
		iter++;
	}
	if (lineCnt < 2) return;
	// walk trough from Topline - 1 to line 0 (in graph line 1)
	// and clear all
	for ( int l=lineCnt-2; l>=0; l--) {
		iter = nodeLine[l]->begin();
		while( iter != nodeLine[l]->end()) {
			iter->second.active = false;
			iter++;
		}
	}
	// now walk trough from Topline to the last but one (in graph line 2)
	// ... and mark all needed as active in the underlying line
	for ( int l=lineCnt-1; l>0; l--) {
		iter = nodeLine[l]->begin();
		while( iter != nodeLine[l]->end()) {
			if ( iter->second.active) {
				(*nodeLine[l-1])[iter->second.mC->alpha].active = true;
				(*nodeLine[l-1])[iter->second.mC->beta].active = true;
			}
			iter++;
		}
	}
	activeGenIterLine = lineCnt - 1;
	activeGenIter = nodeLine[activeGenIterLine]->begin();
}

moduleComponent *dna::getActiveGen() {
/*
	// end of graph has reached
	if ( activeGenIterLine < 0) return 0;
	// end of line has reached
	if ( activeGenIter == nodeLine[activeGenIterLine]->end()) {
		// decrement line
		activeGenIterLine--;
		if ( activeGenIterLine < 0) return 0;
		activeGenIter = nodeLine[activeGenIterLine]->begin();
	}
	
	moduleComponent *mC = activeGenIter->second.mC;
	//printf("activeGen: %d ", activeGenIter->first);
	activeGenIter++;
	return mC;*/
	
	
	// if not end of graph
	while ( activeGenIterLine > 0) {
		// current line end reached
		if ( activeGenIter == nodeLine[activeGenIterLine]->end()) {
			activeGenIterLine--;
			if ( activeGenIterLine < 0) return 0;
			activeGenIter = nodeLine[activeGenIterLine]->begin();
			continue;
		}
		if ( activeGenIter->second.active) {
			moduleComponent *mC = activeGenIter->second.mC;
			activeGenIter++;
			return mC;
		}
		activeGenIter++;	
	}
	// end of graph has reached
	return 0;
}

// bit 1 - print active gene
// bit 2 - print operation
// bit 3 - print alpha und beta
// bit 4 - print gamma
void dna::print( int mode = 0) {
	cout << "--" << endl;
	for ( int l=lineCnt-1; l>=0; l--) {
		printf("line %d: ", l + 1);
		iter = nodeLine[l]->begin();
		while( iter != nodeLine[l]->end()) {
			cout << iter->first;
			if ( mode & 1 && iter->second.active) cout << "! ";
			if ( mode & 8)	cout << "[" << iter->second.mC->alpha;
			if ( mode & 4) {
				switch ( iter->second.mC->klasse) {
					case 1: cout << " R "; break;
					case 2: cout << " + "; break;
					case 3: cout << " - "; break;
					case 4: cout << " + "; break;
					case 5: cout << " - "; break;
					case 6: cout << " - "; break;
				}
			}
			if ( mode & 16) cout <<	iter->second.mC->gamma << " ";
			if ( mode & 8) cout <<	iter->second.mC->beta << "]";
			cout << " ";
			iter++;
		}
		cout << endl;
	}
}
