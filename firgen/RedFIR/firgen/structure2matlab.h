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
                          structure2matlab.h  -  description
                             -------------------
    begin                : Fri Nov 9 2001
    copyright            : (C) 2001 by Holm Fruehauf
    email                : fhf@iis.fhg.de
 ***************************************************************************/

#ifndef STRUCTURE2matlab_H
#define STRUCTURE2matlab_H

#include "fir2hdl.h"
#include "structurelist.h"
#include "implement.h"


/**
  *@author Holm Fruehauf
  */
// redefine


class structure2matlab:protected fir2hdl{
public:

  structure2matlab(structureList * sl, implement *im,char * z_SrcFile, char * z_LogFile);
  ~structure2matlab();
  /**  */


private:

	
private: // Private methods

public: // Public methods
  /**  */
private: // Private methods
  /** No descriptions */
  int include_entity();
  /**  */
  int include_coefficients();
  /**  */
  int include_float_model();
  int include_fir_model();
};

#endif
