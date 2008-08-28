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
                          structure2matlab.cpp  -  description
                             %%%%%%%%%-
    begin                : Fri Nov 9 2001
    copyright            : (C) 2001 by Holm Fruehauf
    email                : fhf@iis.fhg.de
 ***************************************************************************/


#include "structure2matlab.h"
#include "structurelist.h"
#include "implement.h"
#include <string.h>
#include <stdio.h>
#include "bitnumber.h"
#include <math.h>

structure2matlab::structure2matlab(structureList * sl,implement *im, char * z_SrcFile, char * z_LogFile){

  MyStructure=sl;
  MyTarget=im;

  set_comment("##");
  z_comment[0]='\%';
  z_comment[1]='\%';


  set_extantion("m");

  FILE * log=fopen(z_LogFile,"a");
  if (log)
  {
    fprintf(log,"\n... MATLAB wizard: starting MATLAB now");
    fprintf(log,"\n... version 0.213m\n");
    fflush(log);

    fprintf(log,"... MATLAB wizard: including headder informations\n");
    i_success=include_headder();
    fflush(log);

    fprintf(log,"... MATLAB wizard: including entity of function\n");
    if (i_success) i_success=include_entity();
    fflush(log);


    fprintf(log,"... MATLAB wizard: including coefficient declaration\n");
    if (i_success) i_success=include_coefficients();
    fflush(log);

    fprintf(log,"... MATLAB wizard: including  floating point model\n");
    if (i_success) i_success=include_float_model();
    fflush(log);


    fprintf(log,"... MATLAB wizard: including  real filter model\n");
    if (i_success) i_success=include_fir_model();
    fflush(log);



    fprintf(log,"... MATLAB wizard: saving source code\n");
    if (i_success) i_success=save(z_SrcFile);
 fflush(log);



    if (i_success) fprintf(log,"... MATLAB wizard: succesfully done: \"%s.m\" size: %d\n\n",z_SrcFile,strlen(z_sourcecode));
      else fprintf(log,"... MATLAB wizard: error occured! no valid file generated\n\n\n");

    fflush(log);
    fclose(log);
  };

}
structure2matlab::~structure2matlab(){

}


int structure2matlab::include_entity(){
  int i_success=0;

  char * z_entity=new char[500*100]; // maximum 500 lines a 100 char
  z_entity[0]='\0';
  char *z = new char [255];

// insert first word of project name
  sprintf(z_entity,"function [fir_out,float_out,deviation] = %s(in_data);\n\n\n",MyStructure->get_projectname());

  ;
  sprintf(z," in_width=%d;\n",MyStructure->get_inwidth()); strcat(z_entity,z);
  sprintf(z," chain_cut= %d;\n",bitNumber::width_of(MyStructure->getCoefCnt())); strcat(z_entity,z);
  strcat(z_entity," l=length(in_data);\n");
  strcat(z_entity," in_data=in_data/max(abs(in_data));\n");
  sprintf(z," if (l<%d)\n \terror ('error ... not enought input values in vector in_data');\n end;\n ",MyStructure->getCoefCnt()+10);
  strcat(z_entity,z);
  strcat(z_entity," rnd_in_data=round(in_data*2^(in_width-1));\n\n");

  i_success=add2src(z_entity);
  delete z_entity;
  delete z;
  return i_success;

}
/**  */
int structure2matlab::include_coefficients(){
  int i_success=0;
  int i_estimate_lines=500+3*(MyStructure->getCoefCnt()); //+MyStructure->get_ModuleCnt();

  char * z_coeff_set= new char [i_estimate_lines*100];
  char * z =new char[255];
  z[0]='\0';
  int cnt=0;
  double f=0.0;

  strcat(z_coeff_set,z_comment);strcat(z_coeff_set," Signal declaration for coefficient structure \n");

  sprintf(z," structure will contain %d lines and %d coefficients\n\n",MyStructure->getLineCnt(),MyStructure->getCoefCnt());
  strcat(z_coeff_set,z_comment);strcat(z_coeff_set,z);



  sprintf(z,"\ncoef_float=zeros(%d,1);\n\n",MyStructure->getCoefCnt());strcat(z_coeff_set,z);
  for (cnt=0;cnt<(MyStructure->getCoefCnt());cnt++)
  {
      f=MyStructure->getCoefDouble(cnt)/pow(2.0,MyStructure->getCoefWidth());
      sprintf(z,"\t coef_float(%d)=%1.64f;\n",cnt+1,f);strcat(z_coeff_set,z);
  };



  i_success=add2src(z_coeff_set);

  delete z;
  delete z_coeff_set;

  return i_success;
}
/**  */

/**  */
int structure2matlab::include_float_model(){
  int i_success=0;
  char * z;
  z=new char[255];
  z[0]='\0';

  char * z_float_model=new char [500+3*MyStructure->getCoefCnt()];
  z_float_model[0]='\0';

  strcat(z_float_model,z_comment);strcat(z_float_model," maximum precision floating point filter estimation\n\n");

  strcat(z_float_model," float_out=conv(in_data,coef_float);\n");
  sprintf(z," float_out=float_out(1:l)*2^%d;\n",MyStructure->get_outwidth()-1);
  strcat(z_float_model,z);

  i_success=add2src(z_float_model);

  delete z;
  delete z_float_model;

  return i_success;
}


int structure2matlab::include_fir_model(){
  int i_success=0;
  char * z;
  z=new char[255];
  z[0]='\0';
  int l=0;
  int g=0;
  int a=0;
  int b=0;
  int cnt=0;
  ;
  char * z_fir_model=new char [500+1000*MyStructure->getCoefCnt()];
  z_fir_model[0]='\0';

  int CoefCnt=MyStructure->getCoefCnt();

  filterModul *fm = new filterModul;
  fm->mC = new moduleComponent;
  char * z_module;

  strcat(z_fir_model,"\n\n");
  strcat(z_fir_model,z_comment);strcat(z_fir_model," real filter estimation\n\n");

  strcat(z_fir_model," \n\n");

  // Knoten durchfahren und berechnen
  MyStructure->firstModule();
  while (0!=(MyStructure->getModule(fm)))
  {
    z_module=MyTarget->get_module(fm->mC->klasse);

    sprintf(z,"\tsg_%d_%d_module = ",fm->line,fm->node);strcat(z_fir_model,z);

    if (fm->line==0)
    {
      switch (fm->mC->klasse) {
        case 1:
            sprintf(z,"rnd_in_data;");
          break;
        case 2:
            sprintf(z,"rnd_in_data * (2^%d+1);",fm->mC->gamma);
          break;
        case 3:
            sprintf(z,"rnd_in_data * (2^%d-1);",fm->mC->gamma);
        break;
      };
 		}else{
 		    l=fm->line-1;
 		    g =fm->mC->gamma;
 		    a =fm->mC->alpha;
 		    b =fm->mC->beta;
		
	      switch (fm->mC->klasse) {
        case 1:
            sprintf(z,"sg_%d_%d_module;",l,fm->node);
          break;
        case 2:
            sprintf(z,"sg_%d_%d_module* (2^%d+1);",l,a,g);
          break;
        case 3:
            sprintf(z,"sg_%d_%d_module* (2^%d-1);",l,a,g);
        break;
        case 4:
            sprintf(z,"sg_%d_%d_module + sg_%d_%d_module* 2^%d ;",l,a,l,b,g);
        break;
        case 5:
            sprintf(z,"sg_%d_%d_module* 2^%d - sg_%d_%d_module;",l,a,g,l,b);
        break;
        case 6:
            sprintf(z,"sg_%d_%d_module - sg_%d_%d_module* 2^%d ;",l,a,l,b,g);
        break;
        };


		}
    strcat(z_fir_model,z);
    sprintf(z,"\t%s i_%d_node: entity work.%s level: %d result: %d \n ",z_comment,cnt,z_module,fm->line,fm->node);
    strcat(z_fir_model,z);

  	cnt++;
  };
  strcat(z_fir_model,"\n");
  strcat(z_fir_model,z_comment);
  strcat(z_fir_model," remapping to coefficient values\n");

  int shift=0;
  int node=0;
  int line=0;


  for (cnt=0;cnt<CoefCnt;cnt++)
  {
    shift=MyStructure->getCoefShift(cnt);
    node =MyStructure->getCoef(cnt);
    line=MyStructure->getLineCnt()-1;
    if (node==0)
      sprintf(z,"\tsg_%d_coef = zeros(l,1);\n",cnt);
    else
      sprintf(z,"\tsg_%d_coef = sg_%d_%d_module*2^%d;\n",cnt,line,node,shift);

    strcat(z_fir_model,z);
  }
  strcat(z_fir_model,"\n\n");
  strcat(z_fir_model,z_comment);
  strcat(z_fir_model," adderchain instantiation\n");


  strcat(z_fir_model,"\tfir_out = zeros(l,1);\n");
  for (cnt=0;cnt<CoefCnt;cnt++){
    sprintf(z,"\tsg_%d_chain = 0;\n",cnt);
    strcat(z_fir_model,z);
  }

  strcat(z_fir_model,"\n");

  sprintf(z," for k=1:l\n");strcat(z_fir_model,z);

  strcat(z_fir_model,"\tif (mod(k,1000)==0) \n\t\tfprintf('.');\n\tend;\n");
  for (cnt=CoefCnt-1;cnt>0;cnt--){
      if (MyStructure->getCoefSgn(cnt)==0)
        sprintf(z,"\tsg_%d_chain = sg_%d_chain+sg_%d_coef(k);\n",cnt,cnt-1,cnt);
      else
        sprintf(z,"\tsg_%d_chain = sg_%d_chain-sg_%d_coef(k);\n",cnt,cnt-1,cnt);

      strcat(z_fir_model,z);
  };
  if (MyStructure->getCoefSgn(0)==0)
     strcat(z_fir_model,"\tsg_0_chain = +sg_0_coef(k);\n");
  else
     strcat(z_fir_model,"\tsg_0_chain = -sg_0_coef(k);\n");


  sprintf(z," fir_out(k)=floor(sg_%d_chain/(2^chain_cut));\n",CoefCnt-1);strcat(z_fir_model,z);
  strcat(z_fir_model,"end;\n");

  strcat(z_fir_model," deviation=fir_out./float_out;\n");

  i_success=add2src(z_fir_model);

  delete z;
  delete z_fir_model;
  delete fm->mC;
  delete fm;
  return i_success;
}
