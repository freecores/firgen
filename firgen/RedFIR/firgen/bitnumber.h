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
                          bitnumber.h  -  description
                             -------------------
    begin                : Wed Oct 10 2001
    copyright            : (C) 2001 by otto stephan
    email                : ottosn
 ***************************************************************************/

#ifndef BITNUMBER_H
#define BITNUMBER_H


/**
  *@author otto stephan
  */

class bitNumber{
public: 
	bitNumber( unsigned int changes, unsigned int width);
	~bitNumber();
	unsigned int next();
	unsigned int getNumber() {return number;}
	void print() {print( number);}
	static void print( unsigned int value);
	int btc() {	return btc( number);}
	static int btc( unsigned int value);
	static int cs( unsigned int value);
	static int ccs( unsigned int value);
	static unsigned int reduceToOneLow( unsigned int value);
	static int countZeroShift(unsigned int value);
	static int msb( unsigned int value);
	static int constrain( unsigned int value);
	static int getLowZero( unsigned int value);
	static unsigned int width_of(unsigned int iValue);
private:
	int bitChanges;
	int bitWidth;
	int shift;
	unsigned int number;
	unsigned int maxNumber;
};



#endif
