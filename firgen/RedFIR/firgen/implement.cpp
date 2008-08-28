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

// implement.cpp: implementation of the implement class.
//
//////////////////////////////////////////////////////////////////////

#include "implement.h"
#include <stdlib.h>
#include "math.h"
#include <stdio.h>
#include <string.h>
#include "bitnumber.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/// list of modules
#define ERROR  0
#define DELAY  1
#define P_MULT 2
#define S_MULT 3
#define ADDER  4
#define A_SUB  5
#define B_SUB  6

implement::implement()
{
	i_No_of_Modules=7;
	i_No_of_Ressources=4;

	//synth_matrix= new uint[i_No_of_Ressources][i_No_of_Modules];
	typedef uint* puint;
	typedef char* pchar;
	synth_matrix = new puint [i_No_of_Modules];
	z_module_names = new pchar[i_No_of_Modules];
	for (uint i=0; i<i_No_of_Modules; i++) {
	  z_module_names[i]= new char[255];
		synth_matrix[i] = new uint[i_No_of_Ressources];
		sprintf(z_module_names[i],"module_%d",i);
		for ( uint j=0; j<i_No_of_Ressources;j++)
			synth_matrix[i][j] = 1;
	}
	weight_vector = new int[i_No_of_Ressources];
	z_ressource_names= new pchar[i_No_of_Ressources];
	for ( uint j=0; j<i_No_of_Ressources;j++)
	{
	  z_ressource_names[j]=new char [255];
	  sprintf( z_ressource_names[j],"ressource_%d",j);
		weight_vector[j] = 1;
	}
	i_width=16;
	////////////////////
	// target information
	z_specification=new char[255];
	sprintf(z_specification,"redFIR default target");
	z_vendor=new char[255];
	sprintf(z_vendor,"redCHIP");
	z_partno=new char[255];
	sprintf(z_partno,"redLINE");
	
	
}

implement::~implement()
{
  delete [] z_specification;

	/*for ( uint i=0; i<i_No_of_Modules; i++)
	{
		 delete synth_matrix[i];
		 delete z_module_names[i];
	};*/
	delete [] synth_matrix;
	delete [] z_module_names;
	delete [] z_ressource_names;
	delete weight_vector;
}




uint implement::set_bit_width(uint iwidth)
{

	if (iwidth>4)
	  i_width=iwidth;
	else
	  i_width=4;
	return 1;
}
uint implement::calc_module(uint iclass, uint ialpha, uint ibeta, uint igamma)
{
	uint result=0;
	switch (iclass)
	{
		case DELAY:
			result=(uint)ialpha;
			break;
		case P_MULT:
			result=(uint)(ialpha*(pow(2,igamma)+1));
			break;
		case S_MULT:
			result=(uint)(ialpha*(pow(2,igamma)-1));
			break;
		case ADDER:
			result=(uint)(ialpha+ibeta*pow(2,igamma));
			break;
		case A_SUB:
			result=(uint)(ialpha*pow(2,igamma)-ibeta);
			break;
		case B_SUB:
			result=(uint)(ialpha-ibeta*pow(2,igamma));
			break;
		default:
			result=0;
			break;
	}
	return result;
};

uint implement::get_price_module(uint iclass, uint ialpha, uint ibeta, uint igamma)
{
  uint price_of_module=0;


  uint cnt_Res=0;

  for (cnt_Res=1;cnt_Res<i_No_of_Ressources;cnt_Res++)
    if (weight_vector[cnt_Res]!=0)
      price_of_module += weight_vector[cnt_Res] * calc_Value_of_Ressource(cnt_Res,iclass,ialpha,ibeta,igamma);
 	
 	return price_of_module;
}


/**  */
uint implement::create_synth_table (int iNo_of_Modules, int iNo_of_Ressources){
	typedef uint* puint;
	uint success =0;

	for (uint i=0; i<i_No_of_Modules; i++)
	    delete synth_matrix[i];
	
	delete synth_matrix;
	delete weight_vector;
	
	i_No_of_Modules=iNo_of_Modules;
	i_No_of_Ressources=iNo_of_Ressources;
	
	synth_matrix = new puint [i_No_of_Modules];
	for ( uint i=0; i<i_No_of_Modules; i++)
	  {
  		synth_matrix[i] = new uint[i_No_of_Ressources];
  		for ( uint j=0; j<i_No_of_Ressources;j++)
  			synth_matrix[i][j] = 0;
	  };
	
	weight_vector = new int[i_No_of_Ressources];
	for ( uint j=0; j<i_No_of_Ressources;j++)
		weight_vector[j] = 0;

	return success;

}

/**  */
uint implement::set_target_ressource(uint i_Ressource, int i_weight){

  if (i_Ressource<i_No_of_Ressources)
  {
    weight_vector[i_Ressource]=i_weight;
    return i_weight;
  }else
    return 0;
}

/**  */
uint implement::set_Synth_Matrix_Element (uint iModule_Pos, uint iRessource_Pos, uint iCost){

  if (( iModule_Pos<i_No_of_Modules) && (iRessource_Pos<i_No_of_Ressources))
  {
    synth_matrix[iModule_Pos][iRessource_Pos]=iCost;
    return iCost;
  } else return 0;
}


uint implement::calc_Value_of_Ressource(uint i_Res,uint iclass, uint ialpha, uint ibeta, uint igamma)
{
  uint result=0;
  switch (i_Res)
  {
    case 0: // logic element
      result= price_logic(iclass, ialpha, ibeta, igamma);
      break;
    case 1: // register / memory elements
      result= price_register(iclass, ialpha, ibeta, igamma);
      break;
    case 2: // internal or short lines
      result= price_internal_lines(iclass, ialpha, ibeta, igamma);
      break;
    case 3:// long lines
      result= price_long_lines(iclass, ialpha, ibeta, igamma);
      break;
    default:
      result =0;
  }
  return result;
}


uint implement::price_logic(uint iclass, uint ialpha, uint ibeta, uint igamma)
{
	// this functon approximates the price of logic of a module
	// the calculation are based on simle one cycle behavioral mvhdl modell
	// the matrix value can be taken as Size of used gates in the model
	// or power per frequency or number of gate equivalents or number of loocup table or maximum frequency
 	uint value_of_logic=0;
	uint number_of_logic_elements=0;
	switch (iclass)
	{
		case DELAY:
			number_of_logic_elements=(1+i_width+bitNumber::width_of(ialpha));
			break;
		case P_MULT:
			number_of_logic_elements=(1+i_width+bitNumber::width_of(calc_module(iclass,ialpha,ibeta,igamma))-igamma);;
			break;
		case S_MULT:
			number_of_logic_elements=(1+i_width+bitNumber::width_of(calc_module(iclass,ialpha,ibeta,igamma))-igamma);
			break;
		case ADDER:
			number_of_logic_elements=(1+i_width+bitNumber::width_of(calc_module(iclass,ialpha,ibeta,igamma))-igamma);
			break;
		case A_SUB:
			number_of_logic_elements=(1+i_width+bitNumber::width_of(ialpha));
			break;
		case B_SUB:
			number_of_logic_elements=(1+i_width+bitNumber::width_of(ialpha));
			break;
		default:
			number_of_logic_elements=0;
			break;
	}
	value_of_logic=(number_of_logic_elements*synth_matrix[iclass][0]); // multiplication with synthesis value
	return value_of_logic;

}

uint implement::price_internal_lines(uint iclass, uint ialpha, uint ibeta, uint igamma)
{
	// aproximation for internal lines
	// internal lines proprtional to width/2 of module_result
	uint count_of_lines=0;
	uint result=0;
	switch (iclass)
	{
		case DELAY:
			result=(1+i_width+bitNumber::width_of(ialpha));
			break;
		case P_MULT:
			result=(1+i_width+bitNumber::width_of(calc_module(iclass,ialpha,ibeta,igamma))-igamma);
			break;
		case S_MULT:
			result=(1+i_width+bitNumber::width_of(calc_module(iclass,ialpha,ibeta,igamma))-igamma);
			break;
		case ADDER:
			result=(1+i_width+bitNumber::width_of(calc_module(iclass,ialpha,ibeta,igamma))-igamma);
			break;
		case A_SUB:
			result=(1+i_width+bitNumber::width_of(ialpha));
			break;
		case B_SUB:
			result=(1+i_width+bitNumber::width_of(ialpha));
			break;
		default:
			result=0;
			break;
	}
	count_of_lines=(result*synth_matrix[iclass][2]/2);
	return count_of_lines;


}


uint implement::price_long_lines(uint iclass, uint ialpha, uint ibeta, uint igamma)
{
	// aproxximation for external lines
	// internal lines proportional to width of module_result
	uint count_of_lines=0;
	uint result=0;
	switch (iclass)
	{
		case DELAY:
			result=(1+i_width+bitNumber::width_of(ialpha));
			break;
		case P_MULT:
		case S_MULT:
		case ADDER:
		case A_SUB:
		case B_SUB:
			result=(1+i_width+bitNumber::width_of(calc_module(iclass,ialpha,ibeta,igamma)));
			break;
		default:
			result=0;
			break;
	}
	count_of_lines=(result*synth_matrix[iclass][3]);

	return count_of_lines;

}

uint implement::price_register(uint iclass, uint ialpha, uint ibeta, uint igamma)
{
	// aproxximation for internal lines
	// internal lines proprtional to width of module_result
	uint count_of_register=0;
		uint result=0;
	switch (iclass)
	{
		case DELAY:
			result=(1+i_width+bitNumber::width_of(ialpha));
			break;
		case P_MULT:
		case S_MULT:
		case ADDER:
		case A_SUB:
		case B_SUB:
			result=(1+i_width+bitNumber::width_of(calc_module(iclass,ialpha,ibeta,igamma)));
			break;
		default:
			result=0;
			break;
	}
	count_of_register=(result*synth_matrix[iclass][1]);

	return count_of_register;

}

/** adding module to list
 */
int implement::add_module(char * z_modulename){

// suche freien Platz
uint i=1;
int i_success=0;

while (i<i_No_of_Modules){
 if ((strncmp(z_module_names[i],"module",6)==0)&& (i_success==0))
 {
   strcpy(z_module_names[i],z_modulename);
   i_success=1;
 };
  i++;
};



return i_success;

}
/** adding resourcename to list */
int implement::add_ressource(char * z_ressourcename, int resource_weigth){

  uint i=0;
  while ((strncmp(z_ressource_names[i],"ressource",9)!=0) && (i<i_No_of_Ressources)){
    /* bugfix by fhf */
    if (0==strncasecmp(z_ressourcename,"a_mult",6)) strcpy(z_ressourcename,"P_MULT");
    if (0==strncasecmp(z_ressourcename,"b_mult",6)) strcpy(z_ressourcename,"S_MULT");
    /* bugfix by fhf */
    strcpy(z_ressource_names[i],z_ressourcename);
    set_target_ressource(i,resource_weigth);
    i++;
  };
  if (i==i_No_of_Ressources) return 0; // set error
  else return 3;
}
/** gets module name from number */
char * implement::get_module(int i){

if (i<(int)i_No_of_Modules) return z_module_names[i];
else return NULL;

}
int implement::set_specification(char * z_spec){
  int i_size=strlen(z_spec);
  int i_success=0;
  if (i_size>0)
  {
    if (i_size>1023) i_size=1023;
    for (int i=1;i<i_size;i++)
    {
      if ((z_spec[i]>32) && (z_spec[i]<32))  z_specification[i]=z_spec[i];
        else z_specification[i]='_';
      i_success=1;
    };
  };
  return i_success;
}
/** returns specification string; */
char * implement::get_specification(){
  return z_specification;
}
/** set specification string for target vendor */
int implement::set_vendor(char * z_ven){
  int i_size=strlen(z_ven);
  int i_success=0;

  if (i_size>0){
      if (i_size<255) strcpy(z_vendor,z_ven);
      else strncpy(z_vendor,z_ven,254);
   i_success=1;
  };
  return i_success;
}
/** returns specification string; */
char * implement::get_vendor(){
  return z_vendor;
}
/** set specification string for target part-number */
int implement::set_partno(char * z_part){
  uint i_size=strlen(z_part);
  int i_success=0;
  if (i_size>0){
      if (i_size<255) strcpy(z_partno,z_part);
      else strncpy(z_partno,z_part,254);
      i_success=1;
  };
  return i_success;
}

char * implement::get_partno(){
  return z_partno;
}

