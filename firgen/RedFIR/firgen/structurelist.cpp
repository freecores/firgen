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
                          structurelist.cpp  -  description
                             -------------------
    begin                : Fri Oct 26 2001
    copyright            : (C) 2001 by otto stephan
                            enhanced by Holm Fruehauf 11.Nov
    email                : ottosn
 ***************************************************************************/



#include "structurelist.h"


double abs(double d)
{
  if (d<0) return -d;
  else return d;
};
structureList::structureList(){

  newInfo();


  maxLine = 0;
	filterLine = new map< unsigned int, moduleComponent>*[5];
	for ( int l=0; l<5; l++)
		filterLine[l] = new map< unsigned int, moduleComponent>;
	cost = UINT_MAX;
	// initialisierungswert
	currentLine = -1;
	
	// needed for coef
	MyCoefs.coefCnt = 0;
	MyCoefs.coefDouble = 0;
	MyCoefs.coefRaw = 0;
	MyCoefs.coefSgn = 0;
	MyCoefs.coef =0;
	MyCoefs.coefShift =0;
	MyCoefs.i_coef_width=8;
	MyCoefs.f_scaling=1.0;
	MyCoefs.f_deviation=0.0;
	MyCoefs.f_quant=0.0;
	
	lineCnt = 2;
	}



int structureList::newInfo(){
  MyInfo.z_userID = new char [1];
  MyInfo.z_username = new char [1];
  MyInfo.z_project  = new char [1];
  MyInfo.z_version  = new char [1];
  MyInfo.z_revision = new char [1];
	MyInfo.z_description=  new char [1];
	MyInfo.z_company = new char [1];
	MyInfo.z_date = new char [1];
	
	MyInfo.i_indexHDL=0;
	MyInfo.i_indexHDL=0;
	MyInfo.i_in_width=0;
	MyInfo.i_out_width=0;
	MyInfo.i_coef_no=0;
	MyInfo.i_mode=0;
	MyInfo.f_effort=1;
	MyInfo.i_logging=0;
	MyInfo.i_generations=1;

  return 1;
}
structureList::~structureList(){
//	clear();
  delInfo();
}

int structureList::delInfo(){
	delete MyInfo.z_userID;
	delete MyInfo.z_username;
	delete MyInfo.z_project;
	delete MyInfo.z_version;
	delete MyInfo.z_revision;	
	delete MyInfo.z_description;
	delete MyInfo.z_company;
	delete MyInfo.z_date;
	
		return 1;
	
	};

void structureList::setModule(  unsigned int line, unsigned int node, moduleComponent *mC){
	// resize filterLine
	if ( (int) line > maxLine) {
		maxLine = (int)line;
	}
	// insert moduleComponent
	(*filterLine[line])[node].alpha = mC->alpha;
	(*filterLine[line])[node].beta = mC->beta;
	(*filterLine[line])[node].gamma = mC->gamma;
	(*filterLine[line])[node].klasse = mC->klasse;
}

int structureList::getModule( filterModul *fm) {
	// end has reached
	if ( currentLine > maxLine) return 0;
	// initialisieren
	if ( currentLine == -1) {
		currentLine = 0;
		getIter = filterLine[currentLine]->begin();
	}
	while( currentLine != -1) {
		if ( getIter == filterLine[currentLine]->end()) {
			currentLine++;
			if ( currentLine > maxLine) return 0;
			getIter = filterLine[currentLine]->begin();
			continue;
		}
 		fm->line = currentLine;
		fm->node = getIter->first;
		fm->mC->alpha = getIter->second.alpha;
		fm->mC->beta = getIter->second.beta;
		fm->mC->gamma = getIter->second.gamma;
		fm->mC->klasse = getIter->second.klasse;
		
		getIter++;
		return 1;
	}
	return 1;
}


void structureList::clear() {

	for ( int l=(int)maxLine; l>=0; l--) {
		delete filterLine[l];
	}
	for ( int l=0; l<5; l++)
		filterLine[l] = new map< unsigned int, moduleComponent>;
	maxLine = 0;
	cost = UINT_MAX;
	currentLine = -1;
}

bool structureList::isValid() {
	for ( int l=maxLine; l>=0; l--) {
		
	}
	return true;
}

void structureList::show(int mode /*=4*/) {
	map< unsigned int, moduleComponent>::iterator iter;
	cout << "--" << endl;
	for ( int l=maxLine; l>=0; l--) {
		printf("line %d: \n", l + 1);
		iter = filterLine[l]->begin();
		while( iter != filterLine[l]->end()) {
			cout << iter->first;
			if ( mode & 8)	cout << "[" << iter->second.alpha << " " << iter->second.beta << "] ";
			if ( mode & 4) {
				switch ( iter->second.klasse) {
					case 1: cout << "=[R] "; break;
					case 2: cout << "=" << iter->second.alpha <<"*(1+2^"<<iter->second.gamma<<") "; break;
					case 3: cout <<"="<<iter->second.alpha<<"*(2^"<<iter->second.gamma<<"-1) "; break;
					case 4: cout <<"="<<iter->second.alpha<<"+"<<iter->second.beta<<"*2^"<<iter->second.gamma<<" "; break;
					case 5: cout <<"="<<iter->second.alpha<<"*2^"<<iter->second.gamma<<"-"<<iter->second.beta<<" "; break;
					case 6: cout <<"="<<iter->second.alpha<<"-"<<iter->second.beta<<"*2^"<<iter->second.gamma<<" "; break;
				}
			}
			if ( mode & 1) {
				cout <<" ";
			}
			iter++;
		}
		cout << endl;
	}
}

/**  */
int structureList::set_userid(const char * z_userid){
  int i_success=0;
  delete [] MyInfo.z_userID;
  if (strlen(z_userid)>0){
    MyInfo.z_userID=new char [strlen(z_userid)+1];
    strcpy(MyInfo.z_userID,z_userid);
    i_success=1;
  };
  return i_success;
}

int structureList::set_username(const char * z_username){
 int i_success=0;
  delete [] MyInfo.z_username;
  if (strlen(z_username)>0){
    MyInfo.z_username=new char [strlen(z_username)+1];
    strcpy(MyInfo.z_username,z_username);
    i_success=1;
  };
  return i_success;
}
int structureList::set_project(const char * z_project){
 int i_success=0;
  delete [] MyInfo.z_project;
  if (strlen(z_project)>0){
    MyInfo.z_project=new char [strlen(z_project)+1];
    strcpy(MyInfo.z_project,z_project);
    i_success=1;
  };
  return i_success;
}

int structureList::set_version(const char * z_version){
 int i_success=0;
  delete [] MyInfo.z_version;
  if (strlen(z_version)>0){
    MyInfo.z_version=new char [strlen(z_version)+1];
    strcpy(MyInfo.z_version,z_version);
    i_success=1;
  };
  return i_success;
}
int structureList::set_revision(const char * z_revision){
 int i_success=0;
  delete [] MyInfo.z_revision;
  if (strlen(z_revision)>0){
    MyInfo.z_revision=new char [strlen(z_revision)+1];
    strcpy(MyInfo.z_revision,z_revision);
    i_success=1;
  };
  return i_success;
}
int structureList::set_description(char * z_description){
  int i_success=1;
  delete [] MyInfo.z_description;
  if (z_description)
    {
      MyInfo.z_description=new char [50];
      sprintf(MyInfo.z_description,"no description aviable");
    }

  else{

    MyInfo.z_description=new char [strlen(z_description)+1];
    strcpy(MyInfo.z_description,z_description);
  };
  return i_success;
}
int structureList::set_company(const char * z_company){
 int i_success=0;
  delete [] MyInfo.z_company;
  if (strlen(z_company)>0){
    MyInfo.z_company=new char [strlen(z_company)+1];
    strcpy(MyInfo.z_company,z_company);
    i_success=1;
  };
  return i_success;
}
int structureList::set_date(const char * z_date){
 int i_success=0;
  delete [] MyInfo.z_date;
  if (strlen(z_date)>0){
    MyInfo.z_date=new char [strlen(z_date)+1];
    strcpy(MyInfo.z_date,z_date);
    i_success=1;
  };
  return i_success;
}
int structureList::set_language(const char * z_language){
 int i_success=0;
 MyInfo.i_indexHDL=0;
 if (0==strncasecmp(z_language,"VHDL93",6))
    {
      MyInfo.i_indexHDL=1;
      i_success=1;
    };
 if (0==strncasecmp(z_language,"VHDL87",6))
    {
      MyInfo.i_indexHDL=2;
      i_success=1;
    };
  if (0==strncasecmp(z_language,"MATLAB",6))
    {
      MyInfo.i_indexHDL=3;
      i_success=1;
    };
 if (MyInfo.i_indexHDL==0)  MyInfo.i_indexHDL=1;




  return i_success;
}



int structureList::getLineCnt() {return lineCnt;}

int structureList::getCoefCnt() {return MyCoefs.coefCnt;}

// returns the coef on pos pos	
int structureList::getCoef( int pos) {
	if ( (pos < 0) || (pos >= MyCoefs.coefCnt)) return 0;
	return MyCoefs.coef[pos];
}
int structureList::getCoefRaw( int pos) {
	if ( (pos < 0) || (pos >= MyCoefs.coefCnt)) return 0;
	return MyCoefs.coefRaw[pos];
}
int structureList::getCoefSgn( int pos) {
	if ( (pos < 0) || (pos >= MyCoefs.coefCnt)) return 0;
	return MyCoefs.coefSgn[pos];
}
int structureList::getCoefShift( int pos) {
	if ( (pos < 0) || (pos >= MyCoefs.coefCnt)) return 0;
	return MyCoefs.coefShift[pos];
}
double structureList::getCoefDouble( int pos) {
	if ( (pos < 0) || (pos >= MyCoefs.coefCnt)) return 0;
	return MyCoefs.coefDouble[pos];
}

// add a coef to coefDouble
void structureList::addCoef( double coef) {
	
	
	MyCoefs.coefCnt++;
	MyCoefs.coefDouble = (double*)realloc( MyCoefs.coefDouble, sizeof(double) * MyCoefs.coefCnt);
	MyCoefs.coefDouble[MyCoefs.coefCnt - 1] = coef;
	
	
	
}

// set width of coefficients

// ***** minimum width is 4! *****
// ***** maximum width is 31! *****
int structureList::setCoefWidth(int width) {
  int i_success=0;
	if ((width > 3) && (width < 32))
	{
	  MyCoefs.i_coef_width = width;
	  i_success=1;
	};
	return i_success;
}
int structureList::setChainWidth(int width) {
  int i_success=0;
	if ((width > 3) && (width < 64))
	{
	  MyCoefs.i_chain_width = width;
	  i_success=1;
	};
	return i_success;
}
int structureList::setMaxCoefWidth(int width) {
  int i_success=0;
	if ((width > 3) && (width < 32))
	{
	  MyCoefs.i_max_coef_width = width;
	  i_success=1;
	};
	return i_success;
}



// invoke calcCoef after all coeffs added
int structureList::calcCoef() {
  int i_success=0;
  clear();

	if ( MyCoefs.coef)      delete  (MyCoefs.coef);
	if ( MyCoefs.coefRaw)   delete  (MyCoefs.coefRaw);
	if ( MyCoefs.coefSgn)   delete  (MyCoefs.coefSgn);
	if ( MyCoefs.coefShift) delete (MyCoefs.coefShift);
	
	MyCoefs.coefRaw=new int[MyCoefs.coefCnt];
	MyCoefs.coefSgn=new int[MyCoefs.coefCnt];
	MyCoefs.coef=new uint[MyCoefs.coefCnt];
	MyCoefs.coefShift=new int[MyCoefs.coefCnt];

	double f_temp=0.0;
	double f_scaling=MyCoefs.f_scaling+MyCoefs.f_deviation;
	int coef_abs=0;
	int i=0;
	int max=0;
	double f_max_temp=0.0;
	
	
	
	for ( i=0; i<MyCoefs.coefCnt;i++)
	{
		f_temp =f_scaling*MyCoefs.coefDouble[MyCoefs.coefCnt-1-i]; //scaling with width
	
		// rounding ; set signum
		if (f_temp<0){
			MyCoefs.coefRaw[i]=(int)(f_temp-0.5);
			MyCoefs.coefSgn[i]=1;
		}	else {
			MyCoefs.coefRaw[i]=(int)(f_temp+0.5);	
			MyCoefs.coefSgn[i]=0;
		};
	
	  // calc abs
    coef_abs=(int)abs(MyCoefs.coefRaw[i]);

    MyCoefs.coefShift[i]=bitNumber::countZeroShift(coef_abs);
    MyCoefs.coef[i]=bitNumber::reduceToOneLow(coef_abs);
    if (f_temp>f_max_temp) f_max_temp=f_temp;
    cout << "["<<i<<":"<<  MyCoefs.coef[i] << "]"<<endl;
   //bitNumber::print(MyCoefs.coef[i]);
	};
	
	//fix coef width to new neccesary width (past scaling)
	setMaxCoefWidth(bitNumber::width_of( (int)f_max_temp+1));
		
	// change in_width
		
  int i_max_temp=(int)f_max_temp;
  f_temp=(1.0/(float)i_max_temp);
  MyCoefs.f_quant=(1.0/(float)i_max_temp);

	////calculate neccesary number of lines
  lineCnt=0;
	
	for (i=0;i<MyCoefs.coefCnt;i++)
	  if (max< bitNumber::constrain (MyCoefs.coef[i]))
				max = bitNumber::constrain(MyCoefs.coef[i]);

	lineCnt=(int)(log(max)/log(2)+1);
	
	if (lineCnt<6) i_success=1;
	  else i_success=0;
	

	return i_success;
}

/**  */
int structureList::set_mode(uint mode){
  int i_success=0;
  if ((mode>=0) && (mode<5))
  {
    MyInfo.i_mode=mode;
    i_success=1;
  };
  return i_success;
}
/**  */
int structureList::set_inwidth(uint in_width){

  int i_success=0;
  if ((in_width>3) && (in_width<32))
  {
    MyInfo.i_in_width=in_width;
    i_success=1;
  } else MyInfo.i_in_width=16;

  ;
  return i_success;

}
int structureList::set_outwidth(uint out_width){

  int i_success=0;
  if ((out_width>3) && (out_width<64))
  {
    MyInfo.i_out_width=out_width;
    i_success=1;
  }else MyInfo.i_out_width=32;
  return i_success;
}
/**  */
int structureList::set_effort(double effort,int gen){
  int i_success=0;
  if ( ( (effort>0.0) && (gen>0) ) && ( (effort<86400.0) && (gen<(effort*10000) ) ) )
  {
    i_success=1;
    MyInfo.f_effort=effort*10;
    MyInfo.i_generations=gen*10;
    };
  return i_success;
}
/**  */
int structureList::set_logging(int logging){
  int i_success=0;
  if ( (logging >=0) && (logging <10) )
  {
    i_success=1;
    MyInfo.i_logging=logging;
  };
  return i_success;

}
/**  */
int structureList::set_scaling(double f_scal){
  int i_success=1;
  MyCoefs.f_scaling=f_scal;
  return i_success;
}
int structureList::set_deviation(double f_devi){
  int i_success=1;
  MyCoefs.f_deviation=f_devi;
  return i_success;
}
// returns the count of all nodes in all lines
unsigned int structureList::getNodeCnt() {
	unsigned int cnt = 0;
	for ( int i=0; i<=maxLine; i++)
		cnt += filterLine[i]->size();
	return cnt;
}

/** No descriptions */
int structureList::firstModule(){

currentLine = -1;
  return 1;
}
/**  */
uint structureList::scale_coefs(){


  uint i_success=0;
  double f_scale=1.0;
  double f_sum=0.0;
  int cnt=0;
  double f_max=0.0;


  // berechnen breite der kette:
  for (cnt=0;cnt<getCoefCnt();cnt++)
  {
      f_sum+=abs(getCoefDouble(cnt));
  }

  f_scale=(1.0)/f_sum; // primäre scalierung

  // anpassen der Koeffizienten
  f_max=0;
  for (cnt=0;cnt<getCoefCnt();cnt++){
    MyCoefs.coefDouble[cnt]=MyCoefs.coefDouble[cnt]*f_scale*(pow(2,getCoefWidth())-1);
    if (abs(getCoefDouble(cnt))>f_max) f_max=abs(getCoefDouble(cnt)+0.5);
  };

  int max_coef_width=bitNumber::width_of((int)(f_max));

  i_success=setMaxCoefWidth(max_coef_width+1);

  return i_success;

}
/**  */
uint structureList::getChainWidthAt(uint iPos){

  uint cnt=0;
  int i_sum=0;
  int i_wdt=0;

  if (iPos>=(int)getCoefCnt()) iPos=(int)getCoefCnt()-1;


  for (cnt=0;cnt<=iPos;cnt++)
  {
    i_sum+=getCoef(cnt)*(int)pow(2.0,getCoefShift(cnt));
  };
  i_wdt=bitNumber::width_of(i_sum);

  //if (i_wdt>getChainWidth())  i_wdt=getChainWidth();

  return i_wdt;

}
