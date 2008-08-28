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
                          bitnumber.cpp  -  description
                             -------------------
    begin                : Wed Oct 10 2001
    copyright            : (C) 2001 by otto stephan
    email                : ottosn
 ***************************************************************************/

#include "bitnumber.h"
#include "exception.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

bitNumber::bitNumber( unsigned int changes, unsigned int width){
	// no valid number of bitchanges
	if ( width >= 1) bitChanges = changes;
	// width is to expensive
	if ( sizeof( unsigned int)*8 > width) {
   	bitWidth = width;
   	maxNumber = (1 << width);
   	shift = 0;
   	// start with a random number, except 0
   	// if start from 0 the time will be dramatically shrink!!
   	//number = 0;
   	number = rand() % maxNumber;
   	next();
  };
}

bitNumber::~bitNumber(){
}

unsigned int bitNumber::next() {
	if ( ++number > maxNumber) {
		number = 1;
		//printf("überlauf\n");
	}
	while( bitNumber::constrain( number) > bitChanges) {
		number++;
		if ( number > maxNumber) number = 1;
	}
	return number;
}

void bitNumber::print( unsigned int value) {
	printf(" %10d: ", value);
	for ( int i=31; i>=0; i--)
		if ( value & (1 << i)) printf("1");
		else printf("0");
	printf("\n");
}
// count the bitchanges
int bitNumber::btc( unsigned int value) {
	int lastBit = 0;
	int btw = 0;
	//value = reduceToOneLow( value);
	while ( value) {
		if ( (value & 1) ^ lastBit) btw++;
		lastBit = (value & 1);
		value = value >> 1;
	}
	return ++btw;
}
// calculate the complement crosssum
int bitNumber::ccs( unsigned int value) {
	int crosssum = 0;
	value = bitNumber::reduceToOneLow( value);
	int msb = bitNumber::msb( value);
	value = ~value++;
	for ( int i=0; i<msb; i++) {
		if ( (value & 1) == 1) crosssum++;
		value = (value >> 1);
	}
	return ++crosssum;
}

int bitNumber::cs( unsigned int value) {
	int crosssum = 0;
	while  ( 0 != value) {
		if ( (value & 1) == 1) crosssum++;
		value = (value >> 1);
	}
	return crosssum;
}
//function to reduce the LSB of an unsigned integer if they are zero
unsigned int bitNumber::reduceToOneLow(unsigned int value) {
	if ( !value) return value;
  while ( !(value & 1))
    value=(value>>1);
  return value;
}
//calculates how many lsb are zero
int bitNumber::countZeroShift(unsigned int value) {
  int counter=0;
	while (((value&1)==0) && value>0)
	{
		value=value>>1;
		counter++;
	};
	return counter;
}

int bitNumber::msb( unsigned int value) {
	if ( !value) return 0;
	for ( int i=31; i>=0; i--)
		if ( value & (1 << i)) return i+1;
	return 0;
}

int bitNumber::constrain( unsigned int value) {
	unsigned int b = bitNumber::btc( value);
	unsigned int c = bitNumber::cs( value);
	unsigned int cc= bitNumber::ccs( value);
	if (  c < b) b = c;
	if ( cc < b) return cc;
	return b;
	//return bitNumber::cs(value);
}

int bitNumber::getLowZero( unsigned int value) {
	for ( int i=0;i<32; i++) {
		if ( value & 1) return i;
		value = value >> 1;
	}
	return 0;
}
unsigned int bitNumber::width_of(unsigned int iValue)
{
	//returns bitwidht of value)
	if (iValue==0) return 0;
	else
	{ unsigned int cnt_bit=0;
	  unsigned int itemp=iValue;
	  while (itemp!=0)
	  {
		  itemp=(itemp>>1);
		  cnt_bit++;
	  }
	  return (cnt_bit);

	}
};
