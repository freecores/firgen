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
                          structure2vhdl87.cpp  -  description
                             -------------------
    begin                : Fri Nov 9 2001
    copyright            : (C) 2001 by Holm Fruehauf
    email                : fhf@iis.fhg.de
 ***************************************************************************/


#include "structure2vhdl87.h"
#include "structurelist.h"
#include "implement.h"
#include <string.h>
#include <stdio.h>
#include "bitnumber.h"
#include <math.h>

structure2vhdl87::structure2vhdl87(structureList * sl,implement *im, char * z_SrcFile, char * z_LogFile){

  MyStructure=sl;
  MyTarget=im;

  set_comment("--");
  set_extantion("vhd");

  FILE * log=fopen(z_LogFile,"a");
  if (log)
  {
    fprintf(log,"\n... VHDL wizard: starting VHDL87 now\n");

    fprintf(log,"... VHDL wizard: including headder\n");
    i_success=include_headder();

    fprintf(log,"... VHDL wizard: including entity\n");
    if (i_success) i_success=include_entity();

    fprintf(log,"... VHDL wizard: including  architecture headder\n");
    if (i_success) i_success=include_arch_head();


    fprintf(log,"... VHDL wizard: including architecture body\n");
    if (i_success) i_success=include_arch_body();

    fprintf(log,"... VHDL wizard: saving source code\n");
    if (i_success) i_success=save(z_SrcFile);

    if (i_success) fprintf(log,"... VHDL wizard: succesfully done: \"%s.vhd\" size: %d\n\n",z_SrcFile,strlen(z_sourcecode));
      else fprintf(log,"... VHDL wizard: error occured! no valid file generated\n\n\n");

    fflush(log);
    fclose(log);
  };

}
structure2vhdl87::~structure2vhdl87(){

}


int structure2vhdl87::include_entity(){
  int i_success=0;

  char * z_entity=new char[500*100]; // maximum 500 lines a 100 char
  z_entity[0]='\0';
  char * z = new char [255];



  strcat(z_entity,"\n\n\n");
  strcat(z_entity,"library ieee;\n");
  strcat(z_entity,"use ieee.std_logic_1164.all;\n");
  strcat(z_entity,"use ieee.std_logic_arith.all;\n\n");

  strcat(z_entity,"library work;\n");
  strcat(z_entity,"use work.components_redfir.all; \t-- modify packagename for other technologies\n");

  strcat(z_entity,"\n\n\n");
  comment_line(z_entity,80);
;
// insert first word of project name
  sprintf(z,"entity %s is \n",MyStructure->get_projectname());strcat(z_entity,z);
  strcat(z_entity,"generic (\n");
  strcat(z_entity,"\tflag_clk : integer := 1; \t-- this flag is to enable the clock for the coefficient structure\n");
  strcat(z_entity,"\tflag_rst : integer := 1; \t-- this flag is to enable async reset in modules\n");
  strcat(z_entity,"\tflag_en  : integer := 1); \t-- this flag is to enable the 'en' signale for data enable\n");

  strcat(z_entity,"port (\n");
  strcat(z_entity,"\trst      : in std_logic;  \t-- asynchronous reset signal\n");
  strcat(z_entity,"\ten       : in std_logic;  \t-- data enable signal\n");
  strcat(z_entity,"\tclk      : in std_logic;  \t-- clock signal\n");

  sprintf(z,"\tin_data  : in std_logic_vector  (%d downto 0);   \t-- incomming data\n",MyStructure->get_inwidth()-1);
  strcat(z_entity,z);
  sprintf(z,"\tout_data : out std_logic_vector (%d downto 0) ); \t-- outgoing data\n",MyStructure->get_outwidth()-1);
  strcat(z_entity,z);
  sprintf(z,"end  %s;\n",MyStructure->get_projectname());strcat(z_entity,z);
  comment_line(z_entity,80);

  i_success=add2src(z_entity);
  delete z;

 delete z_entity;

  return i_success;

}
/**  */
int structure2vhdl87::include_arch_head(){
  int i_success=0;
  int i_estimate_lines=500+5*(MyStructure->getCoefCnt()); //+MyStructure->get_ModuleCnt();

  char * z_arch_head= new char [i_estimate_lines*100];
  char * z =new char[255];
  int cnt=0;
  filterModul *fm = new filterModul;
  fm->mC = new moduleComponent;
  int i_tmp_wdt=0;
   int i=0;
 double f=0.0;
 int n=0;

  sprintf(z_arch_head,"\n\n\narchitecture %s_arch of %s is \n\n",MyStructure->get_projectname(),MyStructure->get_projectname());
 //insert coefficient width


  strcat(z_arch_head,"-- Signal declaration for coefficient structure --\n");

  strcat(z_arch_head,"-- structure will contain ");
  sprintf(z,"%d lines and  %d coefficients\n\n",MyStructure->getLineCnt(),MyStructure->getCoefCnt());
  strcat(z_arch_head,z);
  strcat(z_arch_head,"\n\n\n -- signal identifications are\n\n");
  for (cnt=0;cnt<(MyStructure->getCoefCnt());cnt++)
  {
      i=MyStructure->getCoefRaw(cnt);
      f=MyStructure->getCoefDouble(cnt)/pow(2.0,MyStructure->getCoefWidth());
      n=MyStructure->getCoef(cnt);
      /* Auskommentiert um die Koeffizienten griffiger zu haben (die nächste )
      sprintf(z,"-- coefficient %d: is float %1.20f  decimal: %d  signal value id: %d  ;\n",cnt,f,i,n);
      */
      sprintf(z,"-- %1.20f \n",f);
      strcat(z_arch_head,z);
  };

  strcat(z_arch_head,"\n\n-- mapped coefficients \n\n");
  sprintf(z,"  signal sg_in : signed (%d downto 0);\n",MyStructure->get_inwidth()-1);
  strcat(z_arch_head,z);
 strcat(z_arch_head,"\n\n");
 comment_line(z_arch_head,80);

  MyStructure->firstModule();
  while (0!=(MyStructure->getModule( fm)))
  {
    i_tmp_wdt=MyStructure->get_inwidth()+bitNumber::width_of(fm->node)-1;
    strcat(z_arch_head,"  signal ");
    sprintf(z,"sg_%d_%d_module \t:\tsigned(%d downto 0);\n",fm->line,fm->node,i_tmp_wdt);
    strcat(z_arch_head,z);
  };
  MyStructure->firstModule();

 strcat(z_arch_head,"\n\n");


 for (cnt=0;cnt<(MyStructure->getCoefCnt());cnt++)
  {
    i=(MyStructure->getCoefRaw(cnt));
    if (i!=0)
    {
      i_tmp_wdt=MyStructure->get_inwidth()+MyStructure->getChainWidthAt(cnt)-1;
      sprintf(z,"  signal sg_%d_coef\t:\tsigned (%d downto 0);\n",cnt,i_tmp_wdt);
      strcat(z_arch_head,z);
    };
  };

 strcat(z_arch_head,"\n\n-- adder chain signals \n\n");
 sprintf(z,"  signal sg_zero\t\t:\tsigned (%d downto 0);\n",MyStructure->getChainWidth()-1);
 strcat(z_arch_head,z);

 strcat(z_arch_head,"\n\n");
 comment_line(z_arch_head,80);


 for (cnt=0;cnt<(MyStructure->getCoefCnt());cnt++)
  {

    i_tmp_wdt=MyStructure->get_inwidth()+MyStructure->getChainWidthAt(cnt+1)-1;
    sprintf(z,"  signal sg_%d_chain\t:\tsigned (%d downto 0);\n",cnt,i_tmp_wdt);
    strcat(z_arch_head,z);
  };


  comment_line(z_arch_head,80);
  i_success=add2src(z_arch_head);
  delete fm->mC;
 	delete fm;
  delete z;
  delete z_arch_head;

  return i_success;
}
/**  */
int structure2vhdl87::include_arch_body(){


  int i_success=0;
  int i_estimate_lines=500+8*(MyStructure->getCoefCnt())+20*MyStructure->getNodeCnt();
  int i=0;
  char * z_arch_body= new char [i_estimate_lines*100];
  z_arch_body[0]='\0';
  char * z =new char[255];
  int cnt=0;
  filterModul *fm = new filterModul;
  fm->mC = new moduleComponent;
  char * z_module;

  int i_tmp_wdt=0;

  strcat(z_arch_body,"begin\n\n");

  strcat(z_arch_body," -- b_InMapping:   mapping of input signal from std_logic to signed\n");
  comment_line(z_arch_body,80);
  sprintf(z,"    sg_in(%d downto 0)<=signed(in_data(%d downto 0));\n",MyStructure->get_inwidth()-1,MyStructure->get_inwidth()-1);
  strcat(z_arch_body,z);
  strcat(z_arch_body,"--  end b_InMapping; \n\n");


  strcat(z_arch_body,"  --b_CofficientStructure: coefficient structure graph\n");
  comment_line(z_arch_body,80);
  while (0!=(MyStructure->getModule( fm)))
  {
    z_module=MyTarget->get_module(fm->mC->klasse);
    sprintf(z,"   i_%d_node: %s --line: %d result: %d \n ",cnt,z_module,fm->line,fm->node);strcat(z_arch_body,z);
    strcat(z_arch_body,"    generic map (\n");
    strcat(z_arch_body,"      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,\n");

    if (fm->line==0)
    {
      sprintf(z,"      a_width=>%d, b_width=>%d,",MyStructure->get_inwidth(),MyStructure->get_inwidth());
      strcat(z_arch_body,z);
      i_tmp_wdt=MyStructure->get_inwidth()+bitNumber::width_of(fm->node);
      sprintf(z," o_width=>%d,",i_tmp_wdt);strcat(z_arch_body,z);
      sprintf(z," gamma=>%d)\n",fm->mC->gamma);strcat(z_arch_body,z);

      strcat(z_arch_body,"     port map (\n");
      strcat(z_arch_body,"       clk=>clk, rst=>rst, en=>en,\n");
      strcat(z_arch_body,"       IN_A=>sg_in, IN_B=>sg_in,\n");
  	  	
	}else{
	   i_tmp_wdt=MyStructure->get_inwidth();
      sprintf(z,"       a_width=>%d,",i_tmp_wdt+bitNumber::width_of(fm->mC->alpha));   strcat(z_arch_body,z);
      sprintf(z," b_width=>%d,\n",i_tmp_wdt+bitNumber::width_of(fm->mC->beta));   strcat(z_arch_body,z);
      sprintf(z,"       o_width=>%d,",i_tmp_wdt+bitNumber::width_of(fm->node)); strcat(z_arch_body,z);
      sprintf(z," gamma=>%d)\n",fm->mC->gamma); strcat(z_arch_body,z);
      strcat(z_arch_body,"     port map (\n");
      strcat(z_arch_body,"       clk=>clk, rst=>rst, en=>en,\n");
  	   sprintf(z,"       IN_A=>sg_%d_%d_module,",fm->line-1,fm->mC->alpha);  strcat(z_arch_body,z);
  	   sprintf(z," IN_B=>sg_%d_%d_module,\n",fm->line-1,fm->mC->beta);  strcat(z_arch_body,z);
   }
		
	sprintf(z,"       OUT_DATA=>sg_%d_%d_module);\n\n",fm->line,fm->node);strcat(z_arch_body,z);
	cnt++;
		
  };
  strcat(z_arch_body,"-- end b_CoefficientStructure \n\n\n");

//mapping

  strcat(z_arch_body,"--  b_MappingUnit: -- mapping coefficents to nedded width\n");
  comment_line(z_arch_body,80);
  for (cnt=0;cnt<MyStructure->getCoefCnt();cnt++)
  {
    i=(MyStructure->getCoefRaw(cnt));
    if (i!=0){
      i_tmp_wdt=MyStructure->get_inwidth()+bitNumber::width_of(MyStructure->getCoef(cnt));

      sprintf(z,"    i_%d_map: REMAP\n",cnt); strcat(z_arch_body,z);
      strcat(z_arch_body,"    generic map (\n");
      sprintf(z,"      in_width=>%d,",i_tmp_wdt);
      strcat(z_arch_body,z);
      sprintf(z," gamma=>%d,",MyStructure->getCoefShift(cnt)); strcat(z_arch_body,z);
      i_tmp_wdt=MyStructure->get_inwidth()+MyStructure->getChainWidthAt(cnt);

      sprintf(z," out_width=>%d)\n",i_tmp_wdt);strcat(z_arch_body,z);
      strcat(z_arch_body,"    port map (\n");
      sprintf(z,"      IN_MAP=>sg_%d_%d_module,",MyStructure->getLineCnt()-1,MyStructure->getCoef(cnt));
      strcat(z_arch_body,z);
      sprintf(z," OUT_MAP=>sg_%d_coef);\n\n\n",cnt);strcat(z_arch_body,z);
    };
  };

  strcat(z_arch_body,"--  end b_MappingUnit;\n\n");


//adderchain
  strcat(z_arch_body,"--  b_AdderChain:chain to add taps\n");
  comment_line(z_arch_body,80);

  strcat(z_arch_body,"    sg_zero<=(others=>'0');\n\n");

  for (cnt=0;cnt<MyStructure->getCoefCnt();cnt++)
  {

    sprintf(z,"    i_%d_chain: CHAIN\n",cnt); strcat(z_arch_body,z);
    strcat(z_arch_body,"    generic map (\n");
    sprintf(z,"      flag_sgnum=>%d,\n",MyStructure->getCoefSgn(cnt));strcat(z_arch_body,z);
    strcat(z_arch_body,"      flag_rst=>flag_rst, flag_en=>flag_en,\n");

    i_tmp_wdt=MyStructure->get_inwidth()+MyStructure->getChainWidthAt(cnt);
    sprintf(z,"      in_width=>%d,",i_tmp_wdt);strcat(z_arch_body,z);

    i_tmp_wdt=MyStructure->get_inwidth()+MyStructure->getChainWidthAt(cnt+1);
    sprintf(z," out_width=>%d)\n",i_tmp_wdt);strcat(z_arch_body,z);

    strcat(z_arch_body,"    port map (\n");
    strcat(z_arch_body,"      clk=>clk, rst=>rst, en=>en,\n");
    if (cnt>0)
      sprintf(z,"      IN_CHAIN=>sg_%d_chain,",cnt-1);
    else
    {
      i_tmp_wdt=MyStructure->get_inwidth()+MyStructure->getChainWidthAt(cnt)-1;
      sprintf(z,"      IN_CHAIN=>sg_zero(%d downto 0),",i_tmp_wdt);
    };
    strcat(z_arch_body,z);
    if (0!=(MyStructure->getCoefRaw(cnt)))
      sprintf(z," IN_COEF=>sg_%d_COEF,\n",cnt);
    else
    {
      i_tmp_wdt=MyStructure->get_inwidth()+MyStructure->getChainWidthAt(cnt)-1;
      sprintf(z," IN_COEF=>sg_zero(%d downto 0),\n",i_tmp_wdt);
    };
    strcat(z_arch_body,z);
    sprintf(z,"      OUT_CHAIN=>sg_%d_chain);\n\n\n",cnt);strcat(z_arch_body,z);
  }


  strcat(z_arch_body,"--  end b_AdderChain;\n\n");
//outmapping
  strcat(z_arch_body," -- b_Out_Mapping: setting out data stream\n");
  comment_line(z_arch_body,80);

  sprintf(z,"    out_data(%d downto 0)<=",MyStructure->get_outwidth()-1);
  strcat(z_arch_body,z);
  i_tmp_wdt=MyStructure->get_inwidth()+MyStructure->getChainWidthAt(MyStructure->getCoefCnt())-1;
  sprintf(z,"std_logic_vector(sg_%d_chain(%d downto %d));\n",MyStructure->getCoefCnt()-1,i_tmp_wdt,i_tmp_wdt-MyStructure->get_outwidth()+1);
  strcat(z_arch_body,z);

  strcat(z_arch_body,"--   end b_Out_Mapping; \n\n");
  comment_line(z_arch_body,80);
  sprintf(z," end %s_arch; \n \n ",  MyStructure->get_projectname());
  strcat(z_arch_body,z);



  comment_line(z_arch_body,80);

  i_success=add2src(z_arch_body);




  delete z;
  delete z_arch_body;
  delete fm->mC;
  delete fm;
  return i_success;
}
