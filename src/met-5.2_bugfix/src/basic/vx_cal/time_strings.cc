// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*


////////////////////////////////////////////////////////////////////////

using namespace std;

#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>

//#include <regex.h>
void my_log(const char *frmt, const char *s = NULL);

#include "vx_cal.h"
#include "vx_log.h"


////////////////////////////////////////////////////////////////////////


void sec_to_hhmmss(int in_sec, char *str)

{

int hour, minute, second;

if ( in_sec == bad_data_int )  {
   strcpy(str, na_str);
}
else  {

   sec_to_hms(in_sec, hour, minute, second);

   if(in_sec < 0) {
      sprintf(str, "-%.2i%.2i%.2i", abs(hour), abs(minute), abs(second));
   }
   else {
      sprintf(str, "%.2i%.2i%.2i", hour, minute, second);
   }
}

return;

}


////////////////////////////////////////////////////////////////////////


ConcatString sec_to_hhmmss(int in_sec)

{

char junk[1024];

sec_to_hhmmss(in_sec, junk);

ConcatString str = junk;

return ( str );

}


////////////////////////////////////////////////////////////////////////


ConcatString sec_to_hhmmss_colon(int in_sec)

{

ConcatString s;
int hour, minute, second;

if ( in_sec == bad_data_int )  {
   s = na_str;
}
else  {

   sec_to_hms(in_sec, hour, minute, second);

   if(in_sec < 0) {
      s.format("-%.2i:%.2i:%.2i", abs(hour), abs(minute), abs(second));
   }
   else {
      s.format("%.2i:%.2i:%.2i", hour, minute, second);
   }
}

return(s);

}


////////////////////////////////////////////////////////////////////////


int hhmmss_to_sec(const char * text)

{

int i, hour, minute, second;

i = atoi(text);

hour   = i/10000;

minute = (i%10000)/100;

second = i%100;

i = hms_to_sec(hour, minute, second);

return ( i );

}


////////////////////////////////////////////////////////////////////////


void unix_to_yyyymmdd_hhmmss(unixtime u, char *str)

{

int year, month, day, hour, minute, second;

unix_to_mdyhms(u, month, day, year, hour, minute, second);

sprintf(str, "%.4i%.2i%.2i_%.2i%.2i%.2i",
        year, month, day, hour, minute, second);

return;

}


////////////////////////////////////////////////////////////////////////


ConcatString unix_to_yyyymmdd_hhmmss(unixtime u)

{

char junk[1024];

unix_to_yyyymmdd_hhmmss(u, junk);

ConcatString str = junk;

return ( str );

}


////////////////////////////////////////////////////////////////////////


unixtime yyyymmdd_hhmmss_to_unix(const char * text)

{

int month, day, year, hour, minute, second;
unixtime t;
char junk[32];


substring_vx_cal(text, junk, 0, 3);

year = atoi(junk);

substring_vx_cal(text, junk, 4, 5);

month = atoi(junk);

substring_vx_cal(text, junk, 6, 7);

day = atoi(junk);

substring_vx_cal(text, junk, 9, 10);

hour = atoi(junk);

substring_vx_cal(text, junk, 11, 12);

minute = atoi(junk);

substring_vx_cal(text, junk, 13, 14);

second = atoi(junk);


t = mdyhms_to_unix(month, day, year, hour, minute, second);

return ( t );

}


////////////////////////////////////////////////////////////////////////


unixtime yyyymmdd_hh_to_unix(const char * text)

{

int month, day, year, hour;
unixtime t;
char junk[32];


substring_vx_cal(text, junk, 0, 3);

year  = atoi(junk);

substring_vx_cal(text, junk, 4, 5);

month = atoi(junk);

substring_vx_cal(text, junk, 6, 7);

day   = atoi(junk);

   //
   //  skip the "_"
   //

substring_vx_cal(text, junk, 9, 10);

hour   = atoi(junk);



t = mdyhms_to_unix(month, day, year, hour, 0, 0);

return ( t );

}


////////////////////////////////////////////////////////////////////////


unixtime yyyymmdd_to_unix(const char * text)

{

int month, day, year;
unixtime t;
char junk[32];


substring_vx_cal(text, junk, 0, 3);

year  = atoi(junk);

substring_vx_cal(text, junk, 4, 5);

month = atoi(junk);

substring_vx_cal(text, junk, 6, 7);

day   = atoi(junk);


t = mdyhms_to_unix(month, day, year, 0, 0, 0);

return ( t );

}


////////////////////////////////////////////////////////////////////////


void unix_to_yyyymmddhh(unixtime u, char *str)

{

int year, month, day, hour, minute, second;

unix_to_mdyhms(u, month, day, year, hour, minute, second);

sprintf(str, "%.4i%.2i%.2i%.2i", year, month, day, hour);

return;

}


////////////////////////////////////////////////////////////////////////


ConcatString unix_to_yyyymmddhh(unixtime u)

{

char junk[1024];

unix_to_yyyymmddhh(u, junk);

ConcatString str = junk;

return ( str );

}


////////////////////////////////////////////////////////////////////////


unixtime yyyymmddhh_to_unix(const char * text)

{

ConcatString str;

str << text << "0000";

return ( yyyymmddhhmmss_to_unix(str) );

}

////////////////////////////////////////////////////////////////////////


unixtime yyyymmddhhmm_to_unix(const char * text)

{

ConcatString str;

str << text << "00";

return ( yyyymmddhhmmss_to_unix(str) );

}

////////////////////////////////////////////////////////////////////////


unixtime yyyymmddhhmmss_to_unix(const char * text)

{

int month, day, year, hour, minute, second;
unixtime t;
char junk[32];


substring_vx_cal(text, junk, 0, 3);

year = atoi(junk);

substring_vx_cal(text, junk, 4, 5);

month = atoi(junk);

substring_vx_cal(text, junk, 6, 7);

day = atoi(junk);

substring_vx_cal(text, junk, 8, 9);

hour = atoi(junk);

substring_vx_cal(text, junk, 10, 11);

minute = atoi(junk);

substring_vx_cal(text, junk, 12, 13);

second = atoi(junk);

t = mdyhms_to_unix(month, day, year, hour, minute, second);

return ( t );

}


////////////////////////////////////////////////////////////////////////


void substring_vx_cal(const char * text, char * out, int first, int last)

{

if ( first > last )  {
  my_log("err #%s\n", "0x6aceede1");

  return;
}

int j, n;

n = last - first + 1;

for (j=0; j<n; ++j)  {

   out[j] = text[first + j];

}

out[n] = (char) 0;

return;

}


////////////////////////////////////////////////////////////////////////


void make_timestring(unixtime t, char * junk)

{

int month, day, year, hour, minute, second;


unix_to_mdyhms(t, month, day, year, hour, minute, second);

sprintf(junk, "%s %2d, %d  %02d:%02d:%02d",
        short_month_name[month], day, year,
        hour, minute, second);

return;

}


////////////////////////////////////////////////////////////////////////


ConcatString make_timestring(unixtime u)

{

char junk[1024];

make_timestring(u, junk);

ConcatString str = junk;

return ( str );

}


////////////////////////////////////////////////////////////////////////


unixtime timestring_to_unix(const char * text)

{

unixtime t;

t = (unixtime) 0;

     if ( !text                     )  t = (unixtime) 0;
else if ( strlen(text) == 0         )  t = (unixtime) 0;
else if ( is_yyyymmdd_hhmmss (text) )  t = yyyymmdd_hhmmss_to_unix (text);
else if ( is_yyyymmdd_hh     (text) )  t = yyyymmdd_hh_to_unix     (text);
else if ( is_yyyymmddhhmmss  (text) )  t = yyyymmddhhmmss_to_unix  (text);
else if ( is_yyyymmddhhmm    (text) )  t = yyyymmddhhmm_to_unix    (text);
else if ( is_yyyymmddhh      (text) )  t = yyyymmddhh_to_unix      (text);
else if ( is_yyyymmdd        (text) )  t = yyyymmdd_to_unix        (text);
else {
  my_log("err #%s\n", "0x64bd5ee0");

  return NAN;
}


return ( t );

}


////////////////////////////////////////////////////////////////////////


bool is_yyyymmdd(const char * text)

{
  my_log("FATAL ERROR!\n");
  
  
  exit(-1);
///return ( check_reg_exp("^[0-9]\\{8\\}$", text) );

  return true;

}


////////////////////////////////////////////////////////////////////////


bool is_yyyymmddhh(const char * text)

{
  my_log("FATAL ERROR 2!\n");
  
  
  exit(-1);
///return ( check_reg_exp("^[0-9]\\{10\\}$", text) );
  return true;

}


////////////////////////////////////////////////////////////////////////


bool is_yyyymmddhhmm(const char * text)

{
  my_log("FATAL ERROR 3!\n");
  
  
  exit(-1);
///return ( check_reg_exp("^[0-9]\\{12\\}$", text) );
return true;

}


////////////////////////////////////////////////////////////////////////


bool is_yyyymmddhhmmss(const char * text)

{

  my_log("FATAL ERROR 4!\n");
  
  
  exit(-1);
  ///return ( check_reg_exp("^[0-9]\\{14\\}$", text) );
  return true;
}


////////////////////////////////////////////////////////////////////////


bool is_yyyymmdd_hh(const char * text)

{

  my_log("FATAL ERROR 5!\n");
  
  
  exit(-1);
  ///return ( check_reg_exp("^[0-9]\\{8\\}_[0-9]\\{2\\}$", text) );
  return true;

}


////////////////////////////////////////////////////////////////////////


bool is_yyyymmdd_hhmmss(const char * text)

{
  my_log("FATAL ERROR 6!\n");
  
  
  exit(-1);
///return ( check_reg_exp("^[0-9]\\{8\\}_[0-9]\\{6\\}$", text) );
  return true;

}


////////////////////////////////////////////////////////////////////////


int timestring_to_sec(const char * text)

{

int t;

t = 0;

     if ( !text             )  t = bad_data_int;
else if ( strlen(text) == 0 )  t = bad_data_int;
else if ( strcmp(text,       na_str) == 0 ||
          strcmp(text, bad_data_str) == 0 )
                               t = bad_data_int;
else if ( is_hhmmss (text)  )  t = hhmmss_to_sec(text);
else if ( is_hh     (text)  )  t = hms_to_sec(atoi(text), 0, 0);
else {
  my_log("err #%s\n", "0x50f6e84b");

  return NAN;
}


return ( t );

}


////////////////////////////////////////////////////////////////////////


ConcatString sec_to_timestring(int s)

{

ConcatString str;

if      ( s == bad_data_int) str = na_str;
else if ( s % 3600 == 0 )    str = HH ( s / 3600 );
else                         str = sec_to_hhmmss ( s );

return ( str );

}


////////////////////////////////////////////////////////////////////////


bool is_hhmmss(const char * text)

{
  my_log("FATAL ERROR 7!\n");
  
  
  exit(-1);

// Allow negative times and 2 or 3 digits for the number of hours
///return ( check_reg_exp("^-*[0-9]\\{6,7\\}$", text) );
return true;

}


////////////////////////////////////////////////////////////////////////


bool is_hh(const char * text)

{
  my_log("FATAL ERROR 8!\n");
  
  
  exit(-1);
// Allow negative times and 1, 2, or 3 digits for the number of hours
///return ( check_reg_exp("^-*[0-9]\\{1,3\\}$", text) );
return true;

}


////////////////////////////////////////////////////////////////////////


ConcatString HH(int hours)

{

char junk[1024];

sprintf(junk, "%02d", hours);

ConcatString str = junk;

return ( str );

}


////////////////////////////////////////////////////////////////////////

/*
bool check_reg_exp(const char *reg_exp_str, const char *test_str)

{
   bool valid = false;
   regex_t buffer;
   regex_t *preg = &buffer;

   if( regcomp(preg, reg_exp_str, REG_EXTENDED*REG_NOSUB) != 0 ) {
      mlog << Error << "\ncheck_reg_exp(char *, char *) -> "
           << "regcomp error for \""
           << reg_exp_str << "\" and \"" << test_str << "\"\n\n";

      exit ( 1 );
   }

   if( regexec(preg, test_str, 0, 0, 0) == 0 ) { valid = true; }

   //
   // Free allocated memory.
   //
   regfree( preg );

   return( valid );
}
*/

////////////////////////////////////////////////////////////////////////
