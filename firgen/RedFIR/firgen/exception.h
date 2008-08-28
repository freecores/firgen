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
                          exception.h  -  description
                             -------------------
    begin                : Mon Oct 15 2001
    copyright            : (C) 2001 by otto stephan
    email                : ottosn
 ***************************************************************************/


#ifndef EXCEPTION_H
#define EXCEPTION_H


/**
  *@author otto stephan
  */

class exc {
public: 
	exc(char *msg = "  ");
	~exc();
};

#endif
