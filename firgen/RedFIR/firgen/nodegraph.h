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
                          nodegraph.h  -  description
                             -------------------
    begin                : Tue Oct 2 2001
    copyright            : (C) 2001 by otto stephan
    email                : ottosn
 ***************************************************************************/



#ifndef NODEGRAPH_H
#define NODEGRAPH_H


/**
  *@author otto stephan
  */

#include "exception.h"
#include "map.h"
#include "implement.h"
#include "graphtool.h"



class nodeGraph {
public: 
	nodeGraph();
	~nodeGraph();
	moduleComponent *getModul( unsigned int line, unsigned int value);
	int getConstrain( unsigned int value);
	int setTargetFunction( implement *Target);
	unsigned int getCost( moduleComponent *mC);
private:
	implement *target;
	char LifeChar;
};

#endif
