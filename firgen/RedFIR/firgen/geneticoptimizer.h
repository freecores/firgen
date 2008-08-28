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
                          geneticoptimizer.h  -  description
                             -------------------
    begin                : Mon Oct 15 2001
    copyright            : (C) 2001 by otto stephan
    email                : ottosn
 ***************************************************************************/

#ifndef GENETICOPTIMIZER_H
#define GENETICOPTIMIZER_H


/**
  *@author otto stephan
  */
#include "nodegraph.h"
#include "structurelist.h"
#include <stdio.h>


class geneticOptimizer {
public: 
	geneticOptimizer( nodeGraph *n, structureList *sl);
	~geneticOptimizer();
	int optimize( char *benchmarkFile = "");
	void setEffort( double min, int iGenerations = 100);
	void setMutationRate( float mRate) { mutationRate = mRate;}
	void setPopulationSize( int popSize) { populationSize = popSize;}
	void setParentPerIndividuum( int parents) { parentPerIndividuum = parents;}
	int get_best_cost(){return i_actual_best_cost;};

private:
	struct gen{
		moduleComponent *mC;
		bool active;
	};
	struct dna {
		map< unsigned int, gen> **genLine;
		float mutationRate;
	};
	void initializePopulation();
	void analyzeCost();
	bool saveBestDNA();
	void combine();
	void mutate();
	void insertGen( unsigned int line, unsigned int node, dna *DNA);
	
	geneticOptimizer::dna *mergeDNA();
	geneticOptimizer::dna *createDNA();
	void deleteDNA( dna *DNA);
	void printDNA( dna *d, int mode = 0);
	
	nodeGraph *ng;
	dna **populationDNA;
	dna **parentDNA;
	structureList *bestDNA;
	
	//unsigned int bestCost;
	unsigned int *cost;
	unsigned int *ranking;
	int lastBest;
	// parameter
	float mutationRate;
	int idleGenerations;
	int populationSize;
	int parentPerIndividuum;
	double runTime;
	
	int genLines;
	map< unsigned int, gen>::iterator **parentIter;
	FILE *plotFile;
	int g;
	
	// included by fhf
	int i_actual_best_cost;
};

#endif
