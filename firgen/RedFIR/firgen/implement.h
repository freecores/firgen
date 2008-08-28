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
                          implement.h  -  description
                             -------------------
    begin                : Mon Oct 15 2001
    copyright            : (C) 2001 by otto stephan
    email                : ottosn
 ***************************************************************************/


// implement.h: interface for the implement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMPLEMENT_H__BBD7EB51_C845_11D3_B30E_004033A018DE__INCLUDED_)
#define AFX_IMPLEMENT_H__BBD7EB51_C845_11D3_B30E_004033A018DE__INCLUDED_

#include <stdlib.h>
#include "bitnumber.h"
class implement  
{
private:

/// new synthesis matrix

	uint ** synth_matrix;
	int *weight_vector;
	char ** z_module_names;
	char ** z_ressource_names;
	
	uint i_No_of_Ressources;
	uint i_No_of_Modules;
///loadable module structure information from library
	uint no_of_module;
	
/// loadable architecture information
	uint i_width;

	char * z_specification;
	char * z_vendor;
	char * z_partno;
	


	uint price_register(uint iclass, uint ialpha, uint ibeta, uint igamma);
	uint price_long_lines(uint iclass, uint ialpha, uint ibeta, uint igamma);
	uint price_internal_lines(uint iclass,uint ialpha,uint ibeta, uint igamma);
	uint price_logic(uint iclass,uint ialpha, uint ibeta, uint igamma);
	uint calc_module(uint iclass, uint ialpha, uint ibeta, uint igamma);
	
  uint calc_Value_of_Ressource(uint cnt_Res,uint iclass, uint ialpha, uint ibeta, uint igamma);
 public:

	implement();
	virtual ~implement();
 	uint set_bit_width(uint iwidth);
	uint get_price_module( uint iclass, uint ialpha, uint ibeta, uint igamma);
	

  /**  */
  uint create_synth_table (int iNo_of_Modules, int iNo_of_Ressources);
  /**  */
  uint set_Synth_Matrix_Element (uint iModule_Number, uint iRessource, uint iCost);
  /**  */
  uint set_target_ressource (uint i_Ressource, int i_weight);
  /** adding module to list
 */
  int add_module(char * z_modulename);;
  /** adding resourcname to list */
  int add_ressource(char * z_ressourcename,int resource_weigth);
  /** set specification string for target technology */
  int set_specification(char * z_spec);
  /** returns specification string; */
  char * get_vendor();
  /** set specification string for target technology */
  int set_vendor(char * z_ven);
  /** returns specification string; */
  char * get_partno();
  /** set specification string for target technology */
  int set_partno(char * z_part);
  /** returns specification string; */
  char * get_specification();

  /** gets module name from number */
  char * get_module(int i);
		

};

#endif // !defined(AFX_IMPLEMENT_H__BBD7EB51_C845_11D3_B30E_004033A018DE__INCLUDED_)
