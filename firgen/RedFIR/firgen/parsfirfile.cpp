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
                          parsfirfile.cpp  -  description
                             -------------------
    begin                : Sun Oct 21 2001
    copyright            : (C) 2001 by otto stephan
    email                : ottosn
 ***************************************************************************/


#include "parsfirfile.h"
#include "nodegraph.h"
#include "implement.h"
#include "structurelist.h"
#include <sys/stat.h>			/* declare the 'stat' structure	*/
#include <stdio.h>		
#include <string.h>
#include <stdlib.h>
#include <bitnumber.h>
#include <math.h>


parsFirFile::parsFirFile(){

   z_rawFIRfile= new char[1];
   z_errorlog= new char[1];
   i_errorlogsize=01;

   MyCalc.debug=0;
   MyCalc.out_width=16;

   z_LogFile = NULL;

}

parsFirFile::~parsFirFile(){
      delete z_rawFIRfile;
}


/** loading FIR file from disk */
uint parsFirFile::load_file(const char * zfilename){

  struct stat stat_p;

  uint success=0;
  uint i_filesize=0;
  uint cnt=0;
  FILE *ptr = NULL;	
  char c;
  FILE *log = NULL;
  if (z_LogFile) log=fopen(z_LogFile,"a");
  if ( -1 ==  stat (zfilename, &stat_p))
    {
     if (log) fprintf(log,"ERROR occoured attempting to stat %s\n", zfilename);
    } else {
      i_filesize=stat_p.st_size;
      delete z_rawFIRfile;
      z_rawFIRfile= new char[i_filesize];
		  ptr = fopen(zfilename,"r");
		  while (((c = fgetc(ptr)) != EOF) && (cnt<i_filesize))
      {
        z_rawFIRfile[cnt]=c;
        cnt++;
      }
    success=cnt;
    fclose(ptr);			/* Close the file.			*/
    if (log) fprintf(log,"\n\n... parser info: \"FIR file \"%s\" successfully loaded\"\n",zfilename);
  };
  if (log) fclose(log);
  return success;
}
/**  */
uint parsFirFile::preprocess(uint i_ASCIIsize){
  uint cnt=0;
  uint success=0;

  if (i_ASCIIsize>0)
  {
    for (cnt=0;cnt<i_ASCIIsize;cnt++)
    {
      switch (z_rawFIRfile[cnt])
      {
        case 13:
          z_rawFIRfile[cnt]=' ';
          break;
        case '\'':
          z_rawFIRfile[cnt]='"';
          break;
         case '\t':
          z_rawFIRfile[cnt]=' ';
          break;
        case 'ä': z_rawFIRfile[cnt]='a';
          break;
        case 'Ä': z_rawFIRfile[cnt]='A';
          break;
        case 'ö': z_rawFIRfile[cnt]='o';
          break;
        case 'Ö': z_rawFIRfile[cnt]='O';
          break;
        case 'ü': z_rawFIRfile[cnt]='u';
          break;
        case 'Ü': z_rawFIRfile[cnt]='U';
          break;
        case ':': z_rawFIRfile[cnt]=';';
          break;
        case ',': z_rawFIRfile[cnt]='.';
          break;
        case '*': // C block comment
          if (cnt>0)
          {
            if (z_rawFIRfile[cnt-1]=='/') {
              z_rawFIRfile[cnt-1]=' ';
              z_rawFIRfile[cnt]='{';}
           }
          break;

        case '/': // C comment
          if (cnt>0)
          {
            if (z_rawFIRfile[cnt-1]=='/') {
              z_rawFIRfile[cnt-1]='#';
              z_rawFIRfile[cnt]=' ';}
            if (z_rawFIRfile[cnt-1]=='*') {
              z_rawFIRfile[cnt-1]='}'; //block comment
              z_rawFIRfile[cnt]=' ';}
          }

          break;
        case '-': // vhdl comment
          if (cnt>0)
            if (z_rawFIRfile[cnt-1]=='-') {
              z_rawFIRfile[cnt-1]='#';
              z_rawFIRfile[cnt]=' ';}
          break;
        case '%': // matlab comment
             z_rawFIRfile[cnt]='#';
             break;
        default:
          break;
      }

    }
    success=cnt;
  };

  return success;
}
/** check synthax of firfile and extracts values
 */
uint parsFirFile::synthax_check(uint i_ASCIIsize){

  uint success=0;
  uint expect=1;
  uint cnt_command=0;
  uint cnt=0;
  uint cnt_line=0;
  uint flag_block_comment=0;
  uint flag_line_comment=0;
  uint flag_EOL=0;
  uint flag_block_descriptor=0;
  uint flag_string=0;
  uint cnt_MyString=0;

  char z_MyString[1024];
  char c;

  while ((i_ASCIIsize>cnt) && (expect!=0))
  {
      c=z_rawFIRfile[cnt];
      if ((c==' ') && (flag_string==1)) c='_';
      cnt++;
      switch (c)
      {
        case '"':
          c=' ';
          if (flag_string==0) flag_string=1;
          else flag_string=0;
          break;
        case '\n':
          c=' ';
          flag_line_comment=0;
          cnt_line++;
          break;
        case ';':
          c=' ';
          if ((flag_line_comment==0) && (flag_block_comment==0)) flag_EOL=1;
          break;
        case '#':
          c=' ';
          flag_line_comment=1;
          break;
        case '{':
           c=' ';
           flag_block_comment=1;
        break;
        case '}':
          c=' ';
           if (flag_block_comment==1){
              flag_block_comment=0;
          }else{
              add_error(cnt_line,"Can't assign block comment to Block (may be missing)",z_MyString);
          };
          break;
        case '[':
          c=' ';
          flag_block_descriptor=1;
          break;
        case ']':
          c=' ';
          if (flag_block_descriptor==1){
             // block descriptor will be ignored (at the moment)
              cnt_MyString=0;
              flag_block_descriptor=0;
             }else{
              add_error(cnt_line,"Can't assign ']' to Block (may be missing '[')",z_MyString);
              cnt_MyString=0;
             };
             break;
        default:
          if (cnt_MyString<1023){
            if ((flag_block_comment==0) && (flag_line_comment==0)){
              if (flag_EOL==1)   // there is something to interprete
              {
                cnt_command++;
                flag_EOL=0;
                cnt_MyString=0;
                expect=interprete_command(z_MyString,expect);
                if (expect==0) add_error(cnt_line,"Can't resolve command ",z_MyString);
                if (c!=' '){
                  z_MyString[0]=c;
                  cnt_MyString=1;
                };
                z_MyString[cnt_MyString]='\0';
              }else{

                z_MyString[cnt_MyString]=c;
                z_MyString[cnt_MyString+1]='\0';
                if ((cnt_MyString>0) || (c!=' '))  cnt_MyString++;
              };
            };
          }else{
              add_error(cnt_line,"Command line to long",z_MyString);
              cnt_MyString=0;
              z_MyString[0]='\0';
              expect=0;
          };

        break;
      }; //switch c

  }; //while cnt
   if (expect==0) success=0;
   else success=1;


  sprintf(z_MyString,"interpreted %d commands and options in %d lines",cnt_command,cnt_line);
  add_log(z_MyString);

  return success;
}
/** adds an error to errortext */
uint parsFirFile::add_error(uint iLine,const char * z_errortext,const char * z_errorcause)
{

  int i_success=0;
  char * z_errorline= new char[strlen(z_errortext)+strlen(z_errorcause)+64];

  sprintf(z_errorline,"ERROR: \"%s\"in Line %d : \"%s\" \n",z_errortext, iLine,z_errorcause);

  //printf(z_errorline);
  if (z_LogFile)
  {
      FILE * log=fopen(z_LogFile,"a");
      if (log) fprintf(log,z_errorline);
      if (log) fclose(log);
      i_success=1;
  };

  delete z_errorline;

  return i_success;
}
uint parsFirFile::add_log(const char * z_errorcause)
{

  int i_success=0;
  char * z_errorline= new char[strlen(z_errorcause)+64];
  sprintf(z_errorline,"... parser info: \"%s\" \n", z_errorcause);


  //printf(z_errorline);
  if (z_LogFile)
  {
      FILE * log=fopen(z_LogFile,"a");
      if (log) fprintf(log,z_errorline);
      if (log) fclose(log);
      i_success=1;
  };


  delete z_errorline;

  return i_success;
}

/** interprete a character line in search of a command */
uint parsFirFile::interprete_command(char * z_cline, uint i_expect){
  float f_value=0.0;
  int   i_value=0;
  char * z_value;
  z_value= new char[4096];
  z_value[0]='\0';
  char * z_seperator;
  z_seperator= new char[16];
  z_seperator[0]='\0';
  int i_sx=0;
  char *z_token;
  z_token=new char[4096];
  z_token[0]='\0';
  int cnt_modules=0;
  char * p_z=NULL;
  char * p_zc=NULL;



  int i_act_ressource=0;

switch (i_expect)
 {

//////////post- commands ////////////////////////////////////////////////

 case 2: // coefficients
      if (sscanf(z_cline,"%f",&f_value)==1)
      {
        if ((f_value<(-1)) || (f_value>1)){
          i_expect=0;
          add_error(0,"Wrong coefficient range (-1=<coef=<1)","");
        }else  MyFIR->addCoef((double)f_value);
      }else i_expect=1;
      break;

  case 3: // ressource table
      if ((sscanf(z_cline,"%s %s %d",z_value,z_seperator,&i_value)!=0) && (strncmp("=>",z_seperator,2)==0))
         i_expect=MyTarget->add_ressource(z_value,i_value);
      else i_expect=1;
      break;
    case 4: // synthesis table
      if ((sscanf(z_cline,"%s %s %d",z_value,z_seperator,&i_value)!=0)&&
          (strncmp("=>",z_seperator,2)==0)) //valid synth table;
        {
          i_act_ressource=MyTarget->add_module(z_value);// locking for next
          z_token=strtok(z_cline," \t,|"); // remove module_name´
          z_token=strtok(NULL," \t,|"); // remove seperator
          while ((z_token=strtok(NULL," \t,|"))!=NULL)
          {
            sscanf(z_token,"%d",&i_value);
            MyTarget->set_Synth_Matrix_Element(i_act_ressource,cnt_modules,i_value);
            cnt_modules++;
          };
        }else i_expect=1;
      break;
  };


  if (i_expect==1)
  {

//////////////pree-instrunctions ////////////////////////////////
    i_expect=0;

    p_zc=strtok(z_cline,"=");
    p_z=strtok(NULL," ");
//////////////real commands ///////////////////////////////////////
    if (p_z!=NULL)
    {
        // scanning:
        sscanf(p_z,"%d",&i_value);
        sscanf(p_z,"%f",&f_value);

        if  (0==strncasecmp("userid",p_zc,6)) i_expect=MyFIR->set_userid(p_z);
        else if (0==strncasecmp("username",p_zc,8))  i_expect=MyFIR->set_username(p_z);
        else if (0==strncasecmp("project",p_zc,7))  i_expect=MyFIR->set_project(p_z);
        else if (0==strncasecmp("version",p_zc,7))  i_expect=MyFIR->set_version(p_z);
        else if (0==strncasecmp("revision",p_zc,8)) i_expect=MyFIR->set_revision(p_z);
        else if (0==strncasecmp("company",p_zc,7))  i_expect=MyFIR->set_company(p_z);
        else if (0==strncasecmp("date",p_zc,4))     i_expect=MyFIR->set_date(p_z);
        else if (0==strncasecmp("HDLang",p_zc,6))   i_expect=MyFIR->set_language(p_z);
        else if (0==strncasecmp("vendor",p_zc,6))   i_expect=1;
        else if (0==strncasecmp("partno",p_zc,6))
          {
             char * z_vendor=strtok(p_z,",.- ");
             char * z_partno=strtok(NULL,",.- ");
             i_expect=MyTarget->set_vendor(z_vendor);
             if (z_partno!=NULL) i_expect=MyTarget->set_partno(z_partno);
             else i_expect=MyTarget->set_partno("redLINE");
          }
        else if (0==strncasecmp("specification",z_cline,13))i_expect=MyTarget->set_specification(p_z);
        else if (0==strncasecmp("description",z_cline,11)) i_expect=MyFIR->set_description(p_z);

        else if ((0==strncasecmp("debug",p_zc,5)) && (i_value>=0) && (i_value<5))
          { i_expect=1; MyCalc.debug=i_value;}
        else if (0==strncasecmp("mode",p_zc,4)) i_expect=MyFIR->set_mode(i_value);
        else if (0==strncasecmp("effort",p_zc,6))
          if  (NULL!=(p_z=strtok(NULL," ,/")))
              {
                sscanf(p_z,"%d",&i_sx);
                i_expect=MyFIR->set_effort(i_value,i_sx);
              } else i_expect=MyFIR->set_effort((double)i_value);

        else if (0==strncasecmp("logging",p_zc,7)) i_expect=MyFIR->set_logging(i_value);
        else if ((0==strncasecmp("in_width",p_zc,8)) && ((i_value == 0) || (i_value > 3)))
          { i_expect=1; MyCalc.in_width=i_value;}
        else if ((0==strncasecmp("out_width",p_zc,9))&& ((i_value == 0) || (i_value > 3)) && (i_value<64))
          { i_expect=1; MyCalc.out_width=i_value;}
        else if ((0==strncasecmp("scaling_limit",p_zc,13)) && (f_value<=100.0) &&  (f_value>=-100.0))
          { i_expect=1;  MyCalc.f_scale=(double)f_value;}
        else if ((0==strncasecmp("deviation",z_cline,9))&& (f_value<=10.0) &&  (f_value>=-10.0))
            { i_expect=1;  MyCalc.f_deviation=(double)f_value;};
    }else {
      if (0==strncasecmp("coefficients",z_cline,12)) i_expect=2;
      else if (0==strncasecmp("resources",z_cline,9)) i_expect=3;
      else if (0==strncasecmp("synthesis",z_cline,9))  i_expect=4;

      else if (0==strncasecmp("specification",z_cline,13))
        i_expect=MyTarget->set_specification("no specification aviable");
      else if (0==strncasecmp("description",z_cline,11))
        i_expect=MyFIR->set_description("no desciption aviable");
      else if (0==strncasecmp("username",p_zc,8))  i_expect=MyFIR->set_username("redUser");
      else if (0==strncasecmp("project",p_zc,7))  i_expect=MyFIR->set_project("redFIR");
      else if (0==strncasecmp("version",p_zc,7))  i_expect=MyFIR->set_version("no version");
      else if (0==strncasecmp("revision",p_zc,8)) i_expect=MyFIR->set_revision("no revision");
      else if (0==strncasecmp("company",p_zc,7))  i_expect=MyFIR->set_company("FhG IIS-A");
      else if (0==strncasecmp("date",p_zc,4))     i_expect=MyFIR->set_date("any monday");
      else if (0==strncasecmp("HDLang",p_zc,6))   i_expect=MyFIR->set_language("VHDL93");
    };

  };
  delete [] z_value;
  delete [] z_token;
  delete [] z_seperator;
  return i_expect;
}

/** starts parser return errorcode; */
uint parsFirFile::run_parser(implement * setMyTarget, structureList * setMyStructure,char *zfilename, char *zlogfile=NULL){



  MyTarget=setMyTarget;
  MyFIR=setMyStructure;
  z_LogFile=zlogfile;

  uint success=0;
  uint isize=0;


  if (0==strlen(zfilename))
    strcpy(zfilename,"example.fir");


  isize=load_file(zfilename);

  if (isize>0) add_log("starting redFIR parser now");

  if (isize>0)
  {
    isize=preprocess(isize);
    if (isize>0) add_log("preprocessing fir-file successfull done");
    else add_error(0,"ERROR:","preprocessing failed");
  };
  if (isize>0)
  {
    isize=synthax_check(isize);
    if (isize>0) add_log("syntax check of fir-file successfull done");
    else add_error(0,"ERROR:","synthax check failed");
  };
  if (isize>0)
  {
    isize=precalc(isize);
    if (isize>0) add_log("precalculation of start values successfull done");
    else add_error(0,"ERROR:","precalculation failed");
  };
  success=isize;

  delete zfilename;

return success;

}
/** precalculation of several value */
uint parsFirFile::precalc(uint i_ASCIIsize){
  int i_success=0;
  int i=0;
  double f_coef=0.0;
  srand((unsigned int)time((time_t *)NULL));

  //debug modi
  switch (MyCalc.debug){
    case 1:
        for (i=0;i<20;i++)
        {
          f_coef=((double)rand()*2/ (double)RAND_MAX)-1;
          MyFIR->addCoef(f_coef);
        };
        break;
    case 2:
       for (i=0;i<40;i++)
        {
          f_coef=((double)rand()*2/ (double)RAND_MAX)-1;
          MyFIR->addCoef(f_coef);
         };
        break;
    case 3:
        for (i=0;i<100;i++)
        {
          f_coef=((double)rand()*2/ (double)RAND_MAX)-1;
          MyFIR->addCoef(f_coef);
         };
        break;
    case 4:
        for (i=0;i<1000;i++)
        {
          f_coef=((double)rand()*2/ (double)RAND_MAX)-1;
          MyFIR->addCoef(f_coef);
         };
        break;

    default:
        break;

  };

  // setting mode, scaling and deviation:
  switch (MyFIR->get_mode())
  {
    case 1:
      MyFIR->set_scaling(pow(10,MyCalc.f_scale/10));
      MyFIR->set_deviation(0.0);
      break;
    case 2:
      MyFIR->set_scaling(1.0);
      MyFIR->set_deviation(pow(10,MyCalc.f_deviation/10));
      break;

    case 3:
      MyFIR->set_scaling(pow(10,MyCalc.f_scale/10));
      MyFIR->set_deviation(pow(10,MyCalc.f_deviation/10));
      break;

    case 4:
    default:
      MyFIR->set_scaling(1.0);
      MyFIR->set_deviation(0.0);
      break;
  };

  int i_coef_cnt=MyFIR->getCoefCnt();
  int i_chain_overflow=0;
  int i_coef_width=0;
  int i_chain_width=0;

  if (i_coef_cnt>=4)
  {
    i_success=1;
    i_chain_overflow=(int)(bitNumber::width_of(i_coef_cnt)/2);
  }
  else add_error(0,"fatal ERROR","not enough coefficients for sensful use of redFIR-wizard");

  if (i_success &&(MyCalc.out_width==0) && (MyCalc.in_width==0))
  {
    MyCalc.in_width=16;
    MyCalc.out_width=2*MyCalc.in_width;
  }
  else if (i_success && (MyCalc.out_width==0) ) MyCalc.out_width=2*MyCalc.in_width;
  else if (i_success && (MyCalc.in_width==0) )  MyCalc.in_width=(int)(MyCalc.out_width/2+0.5);

  i_coef_width=MyCalc.out_width-MyCalc.in_width+i_chain_overflow;



  if (i_coef_width>31)
  {
    i_coef_width=31;
    MyCalc.in_width=MyCalc.out_width-i_coef_width-i_chain_overflow;

  };
  i_chain_width=MyCalc.in_width+i_coef_width+i_chain_overflow;



  //


  // calculate width of coefficients

  if (i_success) i_success=MyFIR->set_outwidth(MyCalc.out_width);
  if (0==i_success) add_error(0,"fatal ERROR","not sensfull width of outgoing stream w>4 && w<63");

  if (i_success) i_success=MyFIR->set_inwidth(MyCalc.in_width);
  if (0==i_success) add_error(0,"fatal ERROR","not sensfull width of incomming stream w>4 && w<31");

  if (i_success) i_success=MyFIR->setCoefWidth(i_coef_width);
  if (0==i_success) add_error(0,"fatal ERROR","not sensfull internal coefficient width");


  if (i_success) i_success*=MyFIR->setChainWidth(i_chain_width);
  if (0==i_success) add_error(0,"fatal ERROR","not sensfull internal precision set");


  if (i_success) i_success=MyFIR->scale_coefs();
  if (0==i_success) add_error(0,"fatal ERROR","sensfull ratio of incomming and outgoing stream-width; can't estimate proper coefficient width");



  return i_success;
}
