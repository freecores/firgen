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
                          fir2hdl.h  -  description
                             -------------------
    begin                : Mon Nov 12 2001
    copyright            : (C) 2001 by Holm Fruehauf
    email                : fhf@iis.fhg.de
 ***************************************************************************/


#ifndef FIR2HDL_H
#define FIR2HDL_H

#include "structurelist.h"
#include "implement.h"
#include "bitnumber.h"

/**base class for compiling structure into source code

  *@author Holm Fruehauf
  */
  #define VERSION "2.030908ALPHA_FHF"



class fir2hdl {

protected:
  int save(char *z_SrcFile);
  int comment_line(char * z,int i_charperline); /** adding line of comment chars */
  int include_headder(); /** adding headder to vhdl source */

  /**  */
  int set_comment(const char * z);
  int set_extantion(const char * z);


  char * z_path;
  char * z_comment;
  char * z_extantion;
  structureList * MyStructure;
  implement * MyTarget;
  int add2src(char * z);


//  int show();
//  int bill();

 int i_success;
 char * z_sourcecode;

public:
  fir2hdl();
	fir2hdl(structureList * sl, implement * im, char *z_SrcFile, char *z_LogFile);
	virtual ~fir2hdl();
   int get_success(){return i_success;};

private: // Private methods
  /** adds character to source code */



};

#endif
