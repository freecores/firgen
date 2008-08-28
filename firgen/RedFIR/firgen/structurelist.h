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
                          structurelist.h  -  description
                             -------------------
    begin                : Fri Oct 26 2001
    copyright            : (C) 2001 by otto stephan
    email                : ottosn
 ***************************************************************************/

#ifndef STRUCTURELIST_H
#define STRUCTURELIST_H


/**
  *@author otto stephan
  enhanced by fhf
  */

#include "graphtool.h"
#include <map.h>
#include <limits.h>
#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include "bitnumber.h"
#include <math.h>		

struct filterModul {
		unsigned int line;
		unsigned int node;
		moduleComponent *mC;
};




class structureList {
public: 
	structureList();
	void setModule( unsigned int line, unsigned int node, moduleComponent *mC);
	int getModule( filterModul *fm);
	void clear();
	bool isValid();
	void show( int mode=4);
	virtual ~structureList();
	unsigned int cost;
	unsigned int getNodeCnt();
	
  /**  */

  int set_userid(const char * z_userid);
  char * get_userid(){return MyInfo.z_userID;};
  int set_username(const char * z_username);
  char * get_username(){return MyInfo.z_username;};
  int set_project(const char * z_project);
  char * get_project(){return MyInfo.z_project;};
  int set_version(const char * z_version);
  char * get_version(){return MyInfo.z_version;};
  int set_revision(const char * z_revision);
  char * get_revision(){return MyInfo.z_revision;};

  int set_description(char * z_description);
  char * get_description(){return MyInfo.z_description;};

  int set_company(const char * z_company);
  char * get_company(){return MyInfo.z_company;};

  int set_date(const char *z_date);
  char * get_date(){return MyInfo.z_date;};
	
	int set_language(const char *z_language);
  uint get_indexHDL(){return MyInfo.i_indexHDL;};
	
	
	// needed for coefs
	void addCoef( double coef);
	int setCoefWidth(int width);
	int getCoefWidth() { return MyCoefs.i_coef_width;}
	int setChainWidth(int width);
	int getChainWidth() { return MyCoefs.i_chain_width;}
	int setMaxCoefWidth(int width);
	int getMaxCoefWidth() { return MyCoefs.i_max_coef_width;}

	
	int calcCoef();
	int getLineCnt();
	int getCoefCnt();
	int getCoef(int pos);
	int getCoefRaw(int pos);
	int getCoefSgn(int pos);
	int getCoefShift(int pos);
  double getCoefDouble( int pos);

  /**  */
  int set_mode(uint mode);
  int get_mode(){return MyInfo.i_mode;};
  /**  */
  int set_inwidth(uint in_width);
  int get_inwidth(){return MyInfo.i_in_width;};

  int set_outwidth(uint out_width);
  int get_outwidth(){return MyInfo.i_out_width;};
  /**  */
  int set_effort(double effort,int gen=1000);
  double get_effort(){return MyInfo.f_effort;};
  int get_generations(){return MyInfo.i_generations;};
  /**  */
  int set_logging(int logging);
  int get_logging(){return MyInfo.i_logging;};
  /**  */
  int set_scaling(double f_scal);
  int set_deviation(double f_devi);


  char *get_projectname(){return MyInfo.z_project;};
  /** No descriptions */
  int firstModule();
  /**  */
  uint scale_coefs();

  double get_deviation(){ return MyCoefs.f_deviation;};
  double get_scaling(){ return MyCoefs.f_scaling;};
  double get_quant(){ return MyCoefs.f_quant;};
  /**  */
  uint getChainWidthAt(uint iPos);

private:
	map< unsigned int, moduleComponent> **filterLine;
	map< unsigned int, moduleComponent>::iterator getIter;
	int maxLine;
	
	

	int currentLine;

  /**  */
  int newInfo();
  int delInfo();

  struct sct_Info{
		char * z_userID;
	  char * z_username;
	  char * z_project;
	  char * z_version;
	  char * z_revision;	
	  char * z_description;		
	  char * z_company;
	  char * z_date;
	  uint i_indexHDL;
	  uint i_in_width;
	  uint i_out_width;
	  uint i_coef_no;
	  uint i_mode;
	  double f_effort;
	  uint i_logging;
	  uint i_generations;
	} MyInfo;
	
	// needed for coefs
	struct sct_Coef{
	  int coefCnt;
	  double *coefDouble;
	  int *coefRaw;
	  int *coefSgn;
	  unsigned int *coef;
	  int *coefShift;
	  uint i_coef_width;
	  uint i_chain_width;
	  uint i_max_coef_width;
	  double f_scaling;
	  double f_deviation;
	  double f_quant;
  } MyCoefs;

   int lineCnt;	
};

#endif
