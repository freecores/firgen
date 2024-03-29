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
                          fir2hdl.cpp  -  description
                             -------------------
    begin                : Mon Nov 12 2001
    copyright            : (C) 2001 by Holm Fruehauf
    email                : fhf@iis.fhg.de
 ***************************************************************************/


#include "fir2hdl.h"
#include <stdio.h>
#include <string.h>
#include "bitnumber.h"

fir2hdl::fir2hdl()
{
  z_sourcecode=NULL;
  z_path=new char[255];
  z_path[0]='\0';
  i_success=0;
};
fir2hdl::fir2hdl(structureList * sl, implement * im,char *z_SrcFile, char * z_LogFile){
  MyStructure=sl;
  MyTarget=im;
  set_comment("#");
  set_extantion("HDL");
}
fir2hdl::~fir2hdl(){

}

int fir2hdl::save(char * z_SrcFile)
{
  int i_success=0;
  char * z_filename;
  z_filename=new char[255];
  FILE * ptr_MyHDL;


  sprintf(z_filename,"%s.%s",z_SrcFile,z_extantion);

  if (z_sourcecode!=NULL)
  {
    ptr_MyHDL=NULL;
    ptr_MyHDL=fopen(z_filename,"w");

    if (ptr_MyHDL!=NULL)
      {
        fprintf(ptr_MyHDL,z_sourcecode);
        fclose(ptr_MyHDL);
        i_success=1;
      };
  };
  delete z_filename;
  return i_success;
};

/** adding line of comment chars */
int fir2hdl::comment_line(char * z, int i_charperline){
  int i_success=0;

  int cnt=(int) (i_charperline/strlen(z_comment));

  while (cnt>0)
  {
    cnt--;
    strcat(z,z_comment);
    i_success=1;
  }
  strcat(z,"\n");
  return i_success;
}
/** adding headder to vhdl source */
int fir2hdl::include_headder(){
  int i_success=0;

  char * z_headder;
  z_headder =new char[500*100]; // maximum 500 lines a 100 char
  z_headder[0]='\0';

  char * z = new char [1023];
  comment_line(z_headder,80);

  strcat(z_headder,z_comment);
  strcat(z_headder," This sourcecode was automaticaly generated by wizard redFIR(TM)\n");

  sprintf(z," optmizer version : %s\n",VERSION);
  strcat(z_headder,z_comment);strcat(z_headder,z);

  comment_line(z_headder,80);

  sprintf(z," redFIR constraints: \n");
  strcat(z_headder,z_comment);strcat(z_headder,z);

  sprintf(z,"\tdate : %s\n",MyStructure->get_date());
  strcat(z_headder,z_comment);  strcat(z_headder,z);

  sprintf(z,"\tusername %s ,\tuserID  : %s\n",MyStructure->get_userid(),MyStructure->get_username());
  strcat(z_headder,z_comment);strcat(z_headder,z);

  sprintf(z,"\tproject : %s\n",MyStructure->get_project());
  strcat(z_headder,z_comment);strcat(z_headder,z);


  sprintf(z,"\tversion : %s,\trevision: %s\n",MyStructure->get_version(),MyStructure->get_revision());
  strcat(z_headder,z_comment);strcat(z_headder,z);


  sprintf(z,"\trevision: %s\n",MyStructure->get_revision());
  strcat(z_headder,z_comment);strcat(z_headder,z);

  comment_line(z_headder,80);


  comment_line(z_headder,80);
  strcat(z_headder,z_comment);strcat(z_headder," disclaimer:\n");
  strcat(z_headder,z_comment);strcat(z_headder,"\tredFir in a registered trademark of \n");
  strcat(z_headder,z_comment);strcat(z_headder,"\tFraunhofer Institute for Integrated Circuits\n");
  strcat(z_headder,z_comment);strcat(z_headder,"\tAm Wolfsmantel 13 / 91058 Erlangen / Germany\n");
  strcat(z_headder,z_comment);strcat(z_headder," http://www.iis.fraunhofer.de \t redfir@iis.fhg.de\n");



  strcat(z_headder,z_comment);strcat(z_headder," look an http://www.redfir.de for further information and updates \n");

  comment_line(z_headder,80);



  i_success=add2src(z_headder);

  delete z;
  delete z_headder;
  return i_success;


}/**  */
int fir2hdl::set_comment(const char * z){
  z_comment = new char [strlen(z)];
  sprintf(z_comment,z);
  return strlen(z);

}
int fir2hdl::set_extantion(const char * z){
  z_extantion = new char [strlen(z)];
  sprintf(z_extantion,z);
  return strlen(z);
}


/** adds character to source code */
int fir2hdl::add2src(char * z){

  int i_success=0;

  if (z_sourcecode!=NULL)
  {
    int length=strlen(z)+strlen(z_sourcecode);

    char * z_tmp =new char[length+100];
    z_tmp[0]='\0';

    sprintf(z_tmp,"%s",z_sourcecode);
    strcat(z_tmp,z);


    delete z_sourcecode;

    z_sourcecode=z_tmp;

    i_success=1;

    z_sourcecode[length+1]='\0';
  } else{
    z_sourcecode=new char[strlen(z)+100];
    sprintf(z_sourcecode,"%s",z);
    i_success=1;
  };

  z[0]='\0';


  return i_success;

}
