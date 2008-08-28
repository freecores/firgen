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
                          redfir.cpp  -  description
                             -------------------
    begin                : Tue Oct 2 2001
    copyright            : (C) 2001 by otto stephan
    email                : ottosn
 ***************************************************************************/

#include "nodegraph.h"
#include "geneticoptimizer.h"
#include "structurelist.h"

#include "parsfirfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "structure2vhdl93.h"
#include "structure2vhdl87.h"
#include "structure2matlab.h"

#include "benchmark.h"
#include "math.h"

int run_Optimizer(structureList *MySL, nodeGraph * MyNG, char * z_LogFile)
{
  int i_success=0;
  geneticOptimizer *go;

  double f_max_dev=MySL->get_deviation();
  double f_effort =MySL->get_effort();
  double f_quant_effort=0.1*f_effort;

  double f_calc_time=0.0;
  double f_act_dev=0.0;
  double f_quant=0.0;
  double f_best_dev=0.0;
  int i_best_cost=0;
  int i_cnt_deviation=1;

  switch (MySL->get_mode())
  {
    case 3:
    case 2:
        // then percent of time will be spend in search of best scaling
        // starten ohne deviation:
        MySL->set_effort(f_quant_effort);
        MySL->scale_coefs(); // setting gcoeffs to fullscale
        i_success=MySL->calcCoef();	
        go = new geneticOptimizer( MyNG, MySL);
        go->optimize(z_LogFile);

        i_best_cost=go->get_best_cost();
        f_best_dev=0.0;
        delete go;
        f_quant=MySL->get_quant();

      while ((f_calc_time<f_effort) && (i_cnt_deviation<(f_max_dev*2/f_quant)+1))
      {
        i_cnt_deviation++;
        f_calc_time=i_cnt_deviation*f_quant_effort;
        f_act_dev=((double)(i_cnt_deviation>>1))*f_quant;
        if ((i_cnt_deviation%2)==1)
          f_act_dev=-f_act_dev;
        if (f_act_dev!=-1.0) MySL->set_scaling(f_act_dev+1.0);
        MySL->scale_coefs();
        i_success=MySL->calcCoef();	
        MySL->set_effort(f_quant_effort);
        go = new geneticOptimizer( MyNG, MySL);
        go->optimize(z_LogFile);
        if (go->get_best_cost()<i_best_cost)
        {
          i_best_cost=go->get_best_cost();
          f_best_dev=f_act_dev;
         };
          //save best settings:

        delete go;
      };
      //setting best

      MySL->set_effort(f_effort);
      if (f_best_dev!=-1.0)  MySL->set_scaling(f_best_dev+1.0);
     cout<<    f_best_dev<<endl;
      MySL->scale_coefs();
      i_success=MySL->calcCoef();	
	  go = new geneticOptimizer( MyNG, MySL);
	  go->optimize(z_LogFile);
      break;
    case 1:

    default: // full scale optimizer
      i_success=MySL->calcCoef();	
	    go = new geneticOptimizer( MyNG, MySL);
	    go->optimize(z_LogFile);
	
     break;
  };
  delete go;
  return i_success;
};
int run_SrcWizard(structureList * sl,implement * target,char * z_SrcFile, char * z_LogFile )
{
    int i_success=1;

  if (sl->get_indexHDL()==1)
  {
    structure2vhdl93 MySourceCode(sl,target,z_SrcFile,z_LogFile);
    structure2matlab MySourceCode2(sl,target,z_SrcFile,z_LogFile);
  }
  else if (sl->get_indexHDL()==2)
  {
    structure2vhdl87 MySourceCode(sl,target,z_SrcFile,z_LogFile);
    structure2matlab MySourceCode2(sl,target,z_SrcFile,z_LogFile);
  }
  else if (sl->get_indexHDL()==3)
    structure2matlab MySourceCode(sl,target,z_SrcFile,z_LogFile);
  else
    i_success=0;

  return i_success;
};
	
	
	


int main(int argc, char *argv[])
{
  printf("redFIR(tm) engine ... version: 0.99030908beta \n");

  //variables
  int i_success=1;
	int cnt=0;
	int flag_Optimizer=1;
  int flag_SourceWiz=1;

  
	//filenames
	char * z_Path    = new char[1023];
	char * z_FirFile = new char[255];
	char * z_SrcFile = new char[255];
	char * z_LogFile = new char[255];
	char * z = new char[2048];
	
	// standard values
	strcpy(z_Path,"\0");
	strcpy(z_FirFile,"example.fir");
  strcpy(z,"example");
		
	
	

  // parsing arguments
  for (cnt=0;(cnt)<argc;cnt++)
  {
    if ( (0==strncasecmp(argv[cnt],"--help",3)) ||(0==strncasecmp(argv[cnt],"-h",2)))
      {
        cout << "synopsis: redfir filename -s -h" <<endl;
        cout << " Options: -s  --synthax    : validation of .fir synthax" <<endl;
        cout << " Options: -r  --resources  : resource estimation only " <<endl;
        cout << " Options: -h  --help       : getting this help" <<endl;
        i_success=0;

      } else
      if((0==strncasecmp(argv[cnt],"--synthax",4))||(0==strncasecmp(argv[cnt],"-s",2)))
      {
        flag_Optimizer=0;
        flag_SourceWiz=0;
        cout << "Syntax check of .fir File" << endl;
      } else
      if((0==strncasecmp(argv[cnt],"--resource",4))||(0==strncasecmp(argv[cnt],"-r",2)))
      {
        flag_Optimizer=1;
        flag_SourceWiz=0;
        cout << "Resource estimation only. No source code generation will be perfored!" << endl;
      } else
      if ((cnt+1)<argc)
      {
        strcpy(z_FirFile,argv[cnt+1]);
        strcpy(z,z_FirFile);
        if (z!=NULL)  strtok(z,".");
      };
  };     
  strcpy(z_SrcFile,z);
  strcat(z,".log");strcpy(z_LogFile,z);

  if (i_success>0)
    cout << "running Fraunhofer IIS redFIR(TM) with " << z_FirFile << endl;

  //random generation initiation
  srand((unsigned int)time((time_t *)NULL));

	//object instantiations
	nodeGraph *n = new nodeGraph();           // filter matrix
	implement *target = new implement();      // Technologie informations
	structureList *sl = new structureList();  // Information center
	parsFirFile *MyFir = new parsFirFile();   // Parser for config file
	
  // main of main
  if (i_success>0) i_success=MyFir->run_parser(target,sl,z_FirFile,z_LogFile);
  if (i_success>0) i_success=n->setTargetFunction(target);
  if ((i_success>0) && (flag_Optimizer==1)) i_success=run_Optimizer(sl,n,z_LogFile);
  if ((i_success>0) && (flag_SourceWiz==1)) i_success=run_SrcWizard(sl,target,z_SrcFile,z_LogFile);

  
  // end of main in main
  if (i_success==1)
    cout << "redFIR successful processed" << endl;

	delete n;
	delete target;
	delete MyFir;
	delete sl;
	delete z_Path;
	delete z_FirFile;
	delete z_SrcFile;
	delete z_LogFile;
	delete z;
  
  return i_success;
}






 
