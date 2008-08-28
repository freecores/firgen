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
                          parsfirfile.h  -  description
                             -------------------
    begin                : Sun Oct 21 2001
    copyright            : (C) 2001 by otto stephan
    email                : ottosn
 ***************************************************************************/


#ifndef PARSFIRFILE_H
#define PARSFIRFILE_H


/**Klass to parse FIR file for setting values to optimizer
  *@author Holm Fruehauf */

#include "geneticoptimizer.h"
#include "implement.h"
#include "structurelist.h"
#include <stdlib.h>

class parsFirFile {

public: 
	parsFirFile();
	~parsFirFile();
  /**  */

  /** starts parser return errorcode; */

  uint run_parser(implement * setMyTarget,structureList * setMyStructure,char *zfilename, char *zlogfile);
  /** precalculation of several value */

private: // Private attributes
  /** raw FIR file for processing*/
  char* z_rawFIRfile;
  char* z_errorlog;
  int i_errorlogsize;
  char * z_LogFile;

  uint precalc(uint i_ASCIIsize);

	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              private: // Private methods
  /** loading FIR file from disk */
  uint load_file(const char * zfilename);
  /**  */
  uint preprocess(uint i_ASCIIsize);
  /** adds an error to errortext */
  uint add_error(uint iLine,const char * z_errortext, const char * z_errorcause);
  uint add_log(const char * z_errorcause);/** check synthax of firfile and extracts values*/
  uint synthax_check(uint i_ASCIIsize);
  /** interprete a character line in search of a command */
  uint interprete_command(char * z_cline,uint i_expect);
  /**  */


  implement * MyTarget;
  structureList *MyFIR;


  struct strCalc
    {
      uint in_width;
      uint out_width;
      uint debug;
      double f_scale;
      double f_deviation;
    } MyCalc;



};

#endif
