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
                          nodegraph.cpp  -  description
                             -------------------
    begin                : Tue Oct 2 2001
    copyright            : (C) 2001 by otto stephan
    email                : ottosn
 ***************************************************************************/
//#define LIFE 1
#include "nodegraph.h"
#include "bitnumber.h"
#include "exception.h"
#include "implement.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


nodeGraph::nodeGraph(){

	target = 0;
	srand( (unsigned)time( NULL));
	LifeChar='C';
}
nodeGraph::~nodeGraph(){
}


int nodeGraph::getConstrain( unsigned int value) {
	unsigned int b = bitNumber::btc( value);
	unsigned int c = bitNumber::cs( value);
	unsigned int cc= bitNumber::ccs( value);
	if (  c < b) b = c;
	if ( cc < b) return cc;
	return b;
}



unsigned int nodeGraph::getCost( moduleComponent *mC) {
	if (target) return target->get_price_module( mC->klasse, mC->alpha, mC->beta, mC->gamma);
	//get_price_module(uint iclass, uint ialpha, uint ibeta, uint igamma)
	return 0;
}

int nodeGraph::setTargetFunction( implement *Target) {
  int	i_success=0;
	target = Target;
	if (Target!=NULL) i_success=1;
	return i_success;
	
}

moduleComponent *nodeGraph::getModul( unsigned int line, unsigned int value) {
	// check, if value and line valid
	if ( line < 1) {
		printf("invalid line\n");
		return 0;
	}
	// create modul
	moduleComponent *mC = new moduleComponent();
	mC->alpha = 0;
	mC->beta = 0;
	mC->gamma = 0;
	mC->klasse = 0;
	
	int bitChanges = (1 << (line - 1)); //change by fhf
	try {
		bitNumber *bn = new bitNumber( bitChanges, bitNumber::msb( value));
		do {
			//fprintf(stdout,".");fflush(stdout);
			mC->alpha = bn->next();
			if ( mC->alpha > value) {
				mC->beta = mC->alpha - value;
				// check of Delay use cause by input BETA equal to Output Value
				if ( bitNumber::reduceToOneLow( mC->beta) == value) {
					//printf("found beta as value!!\n");
					mC->klasse = DELAY; // register
					mC->alpha = value;
					mC->beta = value;
					return mC;
				}
			}
			else {
				if ( mC->alpha == value) {
					mC->klasse = DELAY; // register
					mC->beta = value;
					return mC;
				}
				mC->beta = value - mC->alpha;
#ifdef LIFE				
				if ( mC->alpha == 1){
				  printf("u\b");
				  fflush(stdout);
				};
				LifeChar=LifeChar^32;
				printf("%c\b",LifeChar);
				fflush(stdout);
#endif
			}
		} while( bitNumber::constrain( mC->beta) > bitChanges);
		// evaluate result:
		// alpha > value: S_MULT, A_SUB, B_SUB
		if ( mC->alpha > value) {
			if ( mC->alpha & 1) { // B_SUB
				mC->gamma = bitNumber::getLowZero( mC->beta);
				mC->beta = bitNumber::reduceToOneLow( mC->beta);
				mC->klasse = B_SUB;
			}
			else {                 // S_MULT or A_SUB
				mC->gamma = bitNumber::getLowZero( mC->alpha);
				mC->alpha = bitNumber::reduceToOneLow( mC->alpha);
				mC->klasse = A_SUB;
			}
			if ( mC->alpha == mC->beta) mC->klasse = S_MULT;
		}
		// alpha < value: P_MULT or ADD
		else {
			if ( mC->beta & 1) {
				unsigned int dummy = mC->beta;
				mC->beta = mC->alpha;
				mC->alpha = dummy;
			}
			mC->gamma = bitNumber::getLowZero( mC->beta);
			mC->beta = bitNumber::reduceToOneLow( mC->beta);
			if ( mC->alpha == mC->beta) mC->klasse = P_MULT;
			else mC->klasse = ADDER;
		}
		delete bn;
	}
	catch ( exc bne){
		printf("\n!!exception\n");
		return 0;
	}
	return mC;
}
