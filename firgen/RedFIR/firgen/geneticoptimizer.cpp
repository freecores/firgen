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
                          geneticoptimizer.cpp  -  description
                             -------------------
    begin                : Mon Oct 15 2001
    copyright            : (C) 2001 by otto stephan
    email                : ottosn
 ***************************************************************************/

#include "geneticoptimizer.h"
#include "exception.h"
#include <limits.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include "benchmark.h"


geneticOptimizer::geneticOptimizer( nodeGraph *n, structureList *sl){

  ng = n;
	bestDNA = sl;
	genLines = bestDNA->getLineCnt();
	// if only one line exist, no optimizing process is needed
	
	lastBest = 0;
	// set default values:
	idleGenerations = 100;
	runTime = 600.00;        // seconds
	mutationRate = -0.02;
	populationSize = 50;
	parentPerIndividuum = 2;
	plotFile = 0;
	srand( (unsigned)time( NULL));
	setEffort(sl->get_effort(),sl->get_generations());
	
	
}
geneticOptimizer::~geneticOptimizer(){
	delete cost;
	delete ranking;
	delete parentDNA;
	for ( int p=0; p<populationSize; p++)
  	deleteDNA( populationDNA[p]);
  delete populationDNA;
	for ( int p=0; p<parentPerIndividuum; p++)
  delete parentIter[p];
  delete parentIter;
}
// set time to optimize in minutes
void geneticOptimizer::setEffort( double min, int iGenerations /*= 100*/) {
	runTime = min;
	idleGenerations = iGenerations;
}

int geneticOptimizer::optimize( char *benchmarkFile) {

  int i_success=0;
  benchmark b;
	plotFile = fopen( benchmarkFile, "a");
 	b.now();
  long startTime = b.get_process_time();
	long lastTime=1;

	
	if (plotFile) {
	  fprintf( plotFile, "\n\n... genOpt info: genetic optimizer started\n");
		fprintf( plotFile, "... genOpt value: population size   : %d individuum per generation\n", populationSize);
		fprintf( plotFile, "... genOpt value: mutation rate     : %2.4f\n", mutationRate*100);
		fprintf( plotFile, "... genOpt value: count of parents  : %d per individuum\n", parentPerIndividuum);
		fprintf( plotFile, "... genOpt value: foot count        : %d nodes\n", bestDNA->getCoefCnt());
		fprintf( plotFile, "... genOpt value: coefficient width : %d bit\n", bestDNA->getCoefWidth());
		fprintf( plotFile, "... genOpt value: maximal runtime   : %f min\n", runTime);
		fprintf( plotFile, "... genOpt value: idle break        : %d generations\n", idleGenerations);
    fflush(plotFile);
		
		b.now();

    fprintf(plotFile,"\n\n... genOpt progress: initialize population (+%ld.%02ld.%03ld min)\n",(b.get_process_time()-startTime) /60000,
  			(b.get_process_time()-startTime) /1000, (b.get_process_time()-startTime)% 1000);
    fflush(plotFile);
    initializePopulation();


  	g = 0;
	  b.now();
 		fprintf(plotFile,"\n... genOpt progress: search best DNA (+%ld.%02ld.%03ld min)",(b.get_process_time()-startTime) /60000,
  			(b.get_process_time()-startTime) /1000, (b.get_process_time()-startTime)% 1000);
    fflush(plotFile);
  	do{
  	  if ((g%50)==0)
   		{
   		  fprintf(plotFile,"\n... genOpt progress: ");
   		  fflush(plotFile);
   		};
  	  b.now();
  	  g++;
  		fprintf(plotFile,".");
  		analyzeCost();
  		if (saveBestDNA()) {
  		  fprintf(plotFile,"\n... genOpt progress: new best DNA found! cost score: %d ", bestDNA->cost);
  			fprintf(plotFile,"\n... genOpt progress: generation: %d time: +%ld.%02ld.%03ld min\n",g, (b.get_process_time()-startTime) /60000,
  			(b.get_process_time()-startTime) /1000, (b.get_process_time()-startTime)% 1000);
  			lastBest = g;
  			lastTime=(b.get_process_time()-startTime) /60000;
  	    fprintf(plotFile,"... genOpt progress: ");
   		  fflush(plotFile);
  		}
  		mutate();
  		combine();		
   		
  	} while (( ((b.get_process_time()-startTime)/60000) < runTime)
  	      &&( ( g-lastBest)<idleGenerations));
  	
  	fprintf(plotFile,"\n... genOpt progress: last generation: %d time: +%ld.%02ld.%03ld min\n",g, (b.get_process_time()-startTime) /60000,
  			(b.get_process_time()-startTime) /1000, (b.get_process_time()-startTime)% 1000);

  	i_actual_best_cost=bestDNA->cost;
    fprintf( plotFile, "... genOpt progress: best cost: %d ,",i_actual_best_cost );
  	if ( ((b.get_process_time()- startTime)/ 60000) >= runTime)
  		fprintf(plotFile,"time limit reached \n");
  	
  	if ( ( g - lastBest) >= idleGenerations)
  	  fprintf( plotFile, "idle count reached\n");
     	
   fprintf( plotFile, "... genOpt info: genetic optimizer done\n");
   fclose( plotFile);
 		i_success=1;

	};

	
	return i_success;
}

void geneticOptimizer::combine() {
	typedef dna* pdna;
	// step 1:
	// get maximal cost
  uint maxCost = 0;                // needed for creating rate array

  for ( int p=0; p<populationSize; p++)
  	if ( cost[p] > maxCost)
  		maxCost = cost[p];
  maxCost++;												// avoid ranking error !!
	// step 2:
  // create ranking array to select good parents with an higher probability
  int sumRate = 0;					// needed for random parent-choose
  int diff = 0;
  for ( int p=0; p<populationSize; p++) {
  	diff = maxCost - cost[p];
  	ranking[p] =  diff + sumRate;
  	sumRate += diff;
  }
  // step 3:
  // generate new population by
  // choose parents per random
  dna **newGenerationDNA = new pdna [populationSize];
  unsigned int choice = 0;
	for ( int i=0; i<populationSize; i++) {
		// select parents per ranking
		for ( int parent=0; parent<parentPerIndividuum; parent++) {
      choice = rand() % sumRate;
      for ( int dummy=0; dummy < populationSize; dummy++) {
        if ( ranking[dummy] > choice) {
          parentDNA[parent] = populationDNA[dummy];
          break;
        }
      }
  	}
  	// create new dna by choosen parents
  	newGenerationDNA[i] = mergeDNA();
  }
  // step 4:
  // delete old dna
  for ( int p=0; p<populationSize; p++)
  	deleteDNA( populationDNA[p]);
  delete populationDNA;
	populationDNA = newGenerationDNA;
}

// merge all parents to new dna
// merge with delete
// version 2.01
geneticOptimizer::dna *geneticOptimizer::mergeDNA() {
	dna *d = createDNA();
	// step 1:
	// insert all coeffs from parent to child
	unsigned int dummy;
	map< unsigned int, gen>::iterator iter;
	iter = parentDNA[0]->genLine[ genLines-1]->begin();
	while( iter != parentDNA[0]->genLine[ genLines-1]->end()) {
		dummy = rand() % parentPerIndividuum;
		(*d->genLine[ genLines-1])[iter->first].mC = new moduleComponent;
		(*d->genLine[ genLines-1])[iter->first].mC->alpha = (*parentDNA[dummy]->genLine[ genLines-1])[iter->first].mC->alpha;
		(*d->genLine[ genLines-1])[iter->first].mC->beta = (*parentDNA[dummy]->genLine[ genLines-1])[iter->first].mC->beta;
		(*d->genLine[ genLines-1])[iter->first].mC->gamma = (*parentDNA[dummy]->genLine[ genLines-1])[iter->first].mC->gamma;
		(*d->genLine[ genLines-1])[iter->first].mC->klasse = (*parentDNA[dummy]->genLine[ genLines-1])[iter->first].mC->klasse;
		(*d->genLine[ genLines-1])[iter->first].active = true;
		if ( genLines > 1) {
			(*d->genLine[ genLines-2])[ (*parentDNA[dummy]->genLine[ genLines-1])[iter->first].mC->alpha].active = true;
			(*d->genLine[ genLines-2])[ (*parentDNA[dummy]->genLine[ genLines-1])[iter->first].mC->beta].active = true;
		}
		iter++;
	}
	if( genLines > 1) {
		unsigned int parentCnt;
		unsigned int choosenParent;
		for ( int l=(genLines-2); l>=0; l--) {
			iter = d->genLine[l]->begin();
			while( iter != d->genLine[l]->end()) {
				parentCnt = 0;
				for ( int p=0; p<parentPerIndividuum; p++) {
					*parentIter[p] = (*parentDNA[p]->genLine[l]).find( iter->first);
					if ( *parentIter[p] != parentDNA[p]->genLine[l]->end()) parentCnt++;
				}
				dummy = rand() % parentCnt;
				choosenParent = 0;
				// get parent
				for ( int p=0; p<parentPerIndividuum; p++) {
					if ( *parentIter[p] != parentDNA[p]->genLine[l]->end()) {
						if ( choosenParent == dummy) {choosenParent = p;break;}
						choosenParent++;
					}
				}
				(*d->genLine[ l])[iter->first].mC = new moduleComponent;
				(*d->genLine[ l])[iter->first].mC->alpha = (*parentDNA[choosenParent]->genLine[ l])[iter->first].mC->alpha;
				(*d->genLine[ l])[iter->first].mC->beta = (*parentDNA[choosenParent]->genLine[ l])[iter->first].mC->beta;
				(*d->genLine[ l])[iter->first].mC->gamma = (*parentDNA[choosenParent]->genLine[ l])[iter->first].mC->gamma;
				(*d->genLine[ l])[iter->first].mC->klasse = (*parentDNA[choosenParent]->genLine[ l])[iter->first].mC->klasse;
				if ( l > 0) {
					(*d->genLine[ l-1])[ (*parentDNA[choosenParent]->genLine[ l])[iter->first].mC->alpha].active = true;
					(*d->genLine[ l-1])[ (*parentDNA[choosenParent]->genLine[ l])[iter->first].mC->beta].active = true;
				}
				iter++;
			}
		}
	}
	// merge mutationRate
	float sumMutationRate = 0;
	for ( int p=0; p<parentPerIndividuum; p++) {
		sumMutationRate += parentDNA[p]->mutationRate;
	}
	d->mutationRate = sumMutationRate / parentPerIndividuum;
	return d;
}


void geneticOptimizer::mutate() {
	map< unsigned int, gen>::iterator iter;
	float mRate = mutationRate;
	for ( int p=0; p<populationSize; p++) {
		// if variable mutation
		if ( mutationRate < 0 || mutationRate > 1)
			mRate = populationDNA[p]->mutationRate;
			
		for ( int l=genLines-1; l>=0; l--) {
			iter = populationDNA[p]->genLine[l]->begin();
			while ( iter != populationDNA[p]->genLine[l]->end()) {
				if ( ((float)rand() / (float)RAND_MAX) < mRate )	{
					delete iter->second.mC;
					iter->second.mC = ng->getModul( l+1, iter->first);
					if ( !iter->first || !iter->second.mC->alpha || !iter->second.mC->beta) {
						printf("aerger bei p: %d\n", p);
					}
					if (l) {
						insertGen( l-1, iter->second.mC->alpha, populationDNA[p]);
						insertGen( l-1, iter->second.mC->beta , populationDNA[p]);
					}
				}
				iter++;
			}
		}
		// mutate mutationrate
		if ( mutationRate < 0 || mutationRate > 1) {
			if ( ((float)rand() / (float)RAND_MAX) < mRate) {
				populationDNA[p]->mutationRate += (float)(rand() - RAND_MAX/2) / (float)(RAND_MAX) / 10;
				if ( populationDNA[p]->mutationRate < 0)
					populationDNA[p]->mutationRate = -populationDNA[p]->mutationRate;
			}
		}
	}
}

void geneticOptimizer::initializePopulation() {
	typedef dna* pdna;
  int cnt=0;
	cost = new unsigned int[populationSize];
	ranking = new unsigned int[populationSize];
	parentDNA = new pdna [parentPerIndividuum];
	parentIter = new map< unsigned int, gen>::iterator*;
	for ( int p=0; p<parentPerIndividuum; p++)
		parentIter[p] = new map< unsigned int, gen>::iterator;
	

	populationDNA = new pdna [populationSize];
	for ( int i=0; i<populationSize; i++) {
		// create individuum with genLines
		populationDNA[i] = createDNA();
		// fill nodeLines
		// step 1: insert coeffs
		for ( int c=0; c<bestDNA->getCoefCnt(); c++) {
		// live out
			if ( bestDNA->getCoef( c) == 0) continue;
		  if ( populationDNA[i]->genLine[ genLines-1]->find( bestDNA->getCoef( c)) ==
  			populationDNA[i]->genLine[genLines-1]->end())
  				insertGen( genLines-1, bestDNA->getCoef(c), populationDNA[i]);	
		}
		if (plotFile)
		{
		
		  if ((cnt%50)==0) fprintf(plotFile,"\n... genOpt progress: ");

		  fprintf(plotFile,".");
		  fflush(plotFile);
		  cnt++;
		};
		
	}
	bestDNA->cost = UINT_MAX;

};

void geneticOptimizer::analyzeCost() {
	map< unsigned int, gen>::iterator iter;
	for ( int p=0; p<populationSize; p++) {
		cost[p] = 0;
		for ( int l=genLines-1; l>=0; l--) {
			iter = populationDNA[p]->genLine[l]->begin();
			while ( iter != populationDNA[p]->genLine[l]->end()) {
				cost[p] += ng->getCost( iter->second.mC);
				iter++;
			}
		}
	}
}


bool geneticOptimizer::saveBestDNA() {
	int bestPos = 0;
	float averageCost = 0;
	float averageMr = 0;	
	map< unsigned int, gen>::iterator dummyIter;
  // search best dna in population
  for (int p=0;p<populationSize;p++) {
  	averageCost += cost[p];
  	averageMr += populationDNA[p]->mutationRate;
    if ( cost[p] < cost[bestPos])
      bestPos = p;
  }
  averageCost /= populationSize;
  averageMr /= populationSize;
  if ( bestDNA->cost > cost[bestPos]) {
  	bestDNA->clear();
  	bestDNA->cost = cost[bestPos];
  	map< unsigned int, gen>::iterator iter;
		for ( int l=genLines-1; l>=0; l--) {
			iter = populationDNA[bestPos]->genLine[l]->begin();
			while( iter != populationDNA[bestPos]->genLine[l]->end()) {
			  if (l) {
					dummyIter = (*populationDNA[bestPos]->genLine[l-1]).find( iter->first);
					// is coef contain in previous line
					if ( dummyIter != populationDNA[bestPos]->genLine[l-1]->end()) {
						iter->second.mC->alpha = iter->first;
						iter->second.mC->beta = iter->first;
						iter->second.mC->gamma = 0;
						iter->second.mC->klasse = DELAY;
						//iter++;
						//continue;
					}
				}
				else {
					if (iter->first == 1) {
						iter->second.mC->alpha = 1;
						iter->second.mC->beta = 1;
						iter->second.mC->gamma = 0;
						iter->second.mC->klasse = DELAY;
						//iter++;
						//continue;
					}
				}
				bestDNA->setModule( l, iter->first, iter->second.mC);
				iter++;
			}
		}
	  	return true;
  }
	return false;
}

void geneticOptimizer::insertGen( unsigned int line, unsigned int node, dna *DNA) {
	map< unsigned int, gen>::iterator dIter;
	if ( line < 0 || line >= (unsigned int)genLines) return;
	dIter = DNA->genLine[line]->find( node);
	if ( dIter != DNA->genLine[line]->end()) return;
	(*DNA->genLine[line])[node].mC = ng->getModul( line+1, node);
	(*DNA->genLine[line])[node].active = true;
	if ( line) {
		insertGen( line - 1, (*DNA->genLine[line])[node].mC->alpha, DNA);
		insertGen( line - 1, (*DNA->genLine[line])[node].mC->beta, DNA);
	}
}

geneticOptimizer::dna *geneticOptimizer::createDNA() {
	dna *rt = new dna;
	rt->genLine = new map< unsigned int, gen>*[genLines];
	for ( int l=0; l<genLines; l++)
			rt->genLine[l] = new map< unsigned int, gen>;
	if ( mutationRate < 0 || mutationRate > 1) {
		rt->mutationRate = -mutationRate;
	}
	else rt->mutationRate = mutationRate;
	return rt;
}

void geneticOptimizer::deleteDNA( dna *DNA) {
	map< unsigned int, gen>::iterator iter;
	for ( int l=genLines-1; l>=0; l--) {
			iter = DNA->genLine[l]->begin();
			while ( iter != DNA->genLine[l]->end()) {
				delete iter->second.mC;
				iter++;
			}
			delete DNA->genLine[l];
		}
	delete DNA->genLine;
	delete DNA;
}

void geneticOptimizer::printDNA( dna *DNA, int mode ) {
	map< unsigned int, gen>::iterator iter;
	 cout << "--" << endl;
	for ( int l=genLines-1; l>=0; l--) {
		printf("line %d: \n", l + 1);
		iter = DNA->genLine[l]->begin();
		while( iter != DNA->genLine[l]->end()) {
			cout << iter->first;
			//if ( mode & 1 && iter->second.active) cout << "! " <<	iter->second.mC->beta << "]";
			if ( mode & 8)	cout << "[" << iter->second.mC->alpha << " " << iter->second.mC->beta << "]";
			if ( mode & 4) {
				switch ( iter->second.mC->klasse) {
					case 1:  cout<< "=[R] "; break;
					case 2:  cout<< "=" << iter->second.mC->alpha <<"*(1+2^"<<iter->second.mC->gamma<<") "; break;
					case 3:  cout<<"="<<iter->second.mC->alpha<<"*(2^"<<iter->second.mC->gamma<<"-1) "; break;
					case 4:  cout<<"="<<iter->second.mC->alpha<<"+"<<iter->second.mC->beta<<"*2^"<<iter->second.mC->gamma<<" "; break;
					case 5:  cout<<"="<<iter->second.mC->alpha<<"*2^"<<iter->second.mC->gamma<<"-"<<iter->second.mC->beta<<" "; break;
					case 6:  cout<<"="<<iter->second.mC->alpha<<"-"<<iter->second.mC->beta<<"*2^"<<iter->second.mC->gamma<<" "; break;
				}
			}
			if ( mode & 16) cout <<	"(" << iter->second.mC->gamma << ") ";
			iter++;
		}
		cout << endl;
	}
}
