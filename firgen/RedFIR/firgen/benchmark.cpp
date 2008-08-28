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

#include "benchmark.h"


benchmark::benchmark(){
 now();
};
benchmark::~benchmark(){
}
/* return 1 if it works, or 0 for failure */
int benchmark::stat2proc(int pid) {
    char buf[800]; /* about 40 fields, 64-bit decimal is about 20 chars */
    int num;
    int fd;
    char* tmp;
    struct stat sb; /* stat() buf, to get EUID */
    sprintf(buf, "/proc/%d/stat", pid);
    if ( (fd = open(buf, O_RDONLY, 0) ) == -1 ) return 0;
    num = read(fd, buf, sizeof buf - 1);
    fstat(fd, &sb);
    P_euid = sb.st_uid;
    close(fd);
    if(num<80) return 0;
    buf[num] = '\0';
    tmp = strrchr(buf, ')');      /* split into "PID (cmd" and "<rest>" */
    *tmp = '\0';                  /* replace trailing ')' with NUL */
    /* parse these two strings separately, skipping the leading "(". */
    memset(P_cmd, 0, sizeof P_cmd);          /* clear */
    sscanf(buf, "%d (%15c", &P_pid, P_cmd);  /* comm[16] in kernel */
    num = sscanf(tmp + 2,                    /* skip space after ')' too */
       "%c "
       "%d %d %d %d %d "
       "%lu %lu %lu %lu %lu %lu %lu "
       "%ld %ld %ld %ld %ld %ld "
       "%lu %lu "
       "%ld "
       "%lu %lu %lu %lu %lu %lu "
       "%u %u %u %u " /* no use for RT signals */
       "%lu %lu %lu",
       &P_state,
       &P_ppid, &P_pgrp, &P_session, &P_tty, &P_tpgid,
       &P_flags, &P_min_flt, &P_cmin_flt, &P_maj_flt, &P_cmaj_flt, &P_utime, &P_stime,
       &P_cutime, &P_cstime, &P_priority, &P_nice, &P_timeout, &P_it_real_value,
       &P_start_time, &P_vsize,
       &P_rss,
       &P_rss_rlim, &P_start_code, &P_end_code, &P_start_stack, &P_kstk_esp, &P_kstk_eip,
       &P_signal, &P_blocked, &P_sigignore, &P_sigcatch,
       &P_wchan, &P_nswap, &P_cnswap
    );
/*    fprintf(stderr, "stat2proc converted %d fields.\n",num); */
    P_vsize /= 1024;
    P_rss *= (PAGE_SIZE/1024);
    if(num < 30) return 0;
    if(P_pid != pid) return 0;
    return 1;
}
/**  */

void benchmark::now(){
 int i_PID=getpid();
 stat2proc(i_PID);
}

