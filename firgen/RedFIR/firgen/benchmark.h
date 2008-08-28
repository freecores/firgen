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

#ifndef BENCHMARK_H
#define BENCHMARK_H


/**
  *@author Holm Fruehauf
  */
#include <asm/param.h>  /* HZ */
#include <asm/page.h>   /* PAGE_SIZE */

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/dir.h>

//#include <stl.h>

class benchmark {
public: 
	benchmark();
	~benchmark();
	void now();

	
  long get_process_time(){return (P_utime+P_stime)*10;};
  long get_pvsize(){return P_vsize;};
  int get_pid(){return P_pid;};


private:
	int stat2proc(int pid);


  int P_euid;
  int P_pid;
  char P_cmd[16];
  char P_state;
  int P_ppid, P_pgrp, P_session, P_tty, P_tpgid;
  unsigned long P_flags, P_min_flt, P_cmin_flt, P_maj_flt, P_cmaj_flt, P_utime, P_stime;
  long P_cutime, P_cstime, P_priority, P_nice, P_timeout, P_it_real_value;
  unsigned long P_start_time, P_vsize;
  long P_rss;
  unsigned long P_rss_rlim, P_start_code, P_end_code, P_start_stack, P_kstk_esp, P_kstk_eip;
  unsigned P_signal, P_blocked, P_sigignore, P_sigcatch;
  unsigned long P_wchan, P_nswap, P_cnswap;




};

#endif
