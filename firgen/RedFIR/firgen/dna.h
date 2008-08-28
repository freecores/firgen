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
                          dna.h  -  description
                             -------------------
    begin                : Mon Oct 15 2001
    copyright            : (C) 2001 by otto stephan
    email                : ottosn
 ***************************************************************************/


#ifndef DNA_H
#define DNA_H


/**
  *@author otto stephan
  */
//#include "map.h"
#include "nodegraph.h"
//#include <stl.h>

class dna {
public: 
	dna( unsigned int lines);
	dna( dna &DNA);
	dna( dna **DNA, unsigned int cnt);
	~dna();
	moduleComponent *getModul( unsigned int line, unsigned int node);
	bool existModul( unsigned int line, unsigned int node);
	void setModul( unsigned int line, unsigned int node, moduleComponent *mC);
	void markActiveGenes();
	moduleComponent *getActiveGen();

	void print( int mode);
private:
	struct dnaNode{
		moduleComponent *mC;
		bool active;
	};
	unsigned int lineCnt;
	map< unsigned int, dnaNode> **nodeLine;
	map< unsigned int, dnaNode>::iterator iter;
	map< unsigned int, dnaNode>::iterator activeGenIter;
	int activeGenIterLine;
};

#endif
