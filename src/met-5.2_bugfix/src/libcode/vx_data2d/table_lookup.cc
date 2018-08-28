// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*


////////////////////////////////////////////////////////////////////////


using namespace std;

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <sys/types.h>  
#include <sys/stat.h>  

#ifdef _WINDOWS
#include <float.h>
#include <math.h>
#define isnan _isnan
#define strcasecmp strcmpi
#endif
void my_log(const char *frmt, const char *s = NULL);

#include "table_lookup.h"
#include "vx_util.h"
#include "vx_math.h"
#include <cerrno>
#include <sys/stat.h>


////////////////////////////////////////////////////////////////////////


   //
   //  This needs external linkage
   //

/// _bp_ TableFlatFile GribTable (0);
TableFlatFile *GribTable;

////////////////////////////////////////////////////////////////////////


/// _bp_ static const char table_data_dir   [] = "MET_BASE/table_files";      //  relative to MET_BASE



////////////////////////////////////////////////////////////////////////


   //
   //  Code for class Grib1TableEntry
   //


////////////////////////////////////////////////////////////////////////


Grib1TableEntry::Grib1TableEntry()

{

init_from_scratch();

}


////////////////////////////////////////////////////////////////////////


Grib1TableEntry::~Grib1TableEntry()

{

clear();

}


////////////////////////////////////////////////////////////////////////


Grib1TableEntry::Grib1TableEntry(const Grib1TableEntry & e)

{

init_from_scratch();

assign(e);

}


////////////////////////////////////////////////////////////////////////


Grib1TableEntry & Grib1TableEntry::operator=(const Grib1TableEntry & e)

{

if ( this == &e )  return ( * this );

assign(e);

return ( * this );

}


////////////////////////////////////////////////////////////////////////


void Grib1TableEntry::init_from_scratch()

{

clear();

return;

}


////////////////////////////////////////////////////////////////////////


void Grib1TableEntry::clear()

{

code = table_number = center = subcenter = -1;

parm_name.clear();

full_name.clear();

units.clear();

return;

}


////////////////////////////////////////////////////////////////////////


void Grib1TableEntry::assign(const Grib1TableEntry & e)

{

clear();

code = e.code;
table_number = e.table_number;
center = e.center;
subcenter = e.subcenter;

parm_name = e.parm_name;

full_name = e.full_name;

units = e.units;

return;

}


////////////////////////////////////////////////////////////////////////

/* _bp_ 
void Grib1TableEntry::dump(ostream & out, int depth) const

{

Indent prefix(depth);

out << prefix << "Index values = ("
              << code << ", "
              << table_number << ", "
              << center << ", "
              << subcenter <<  ")\n";

out << prefix << "parm_name = " << parm_name.contents() << "\n";

out << prefix << "full_name = " << full_name.contents() << "\n";

out << prefix << "units     = " << units.contents()     << "\n";

return;

}
*/

////////////////////////////////////////////////////////////////////////


bool Grib1TableEntry::parse_line(const char * line) {

   clear();

   int j, n;
   int *i[5];
   const char i_delim[] = "{} \"";
   const char s_delim[] = "\"";
   const char *c = (const char *) 0;
   char *line2 = (char *) 0;
   char *L = (char *) 0;


   n = strlen(line);

   line2 = new char[1 + n];

   memset(line2, 0, 1 + n);

   strncpy(line2, line, n);

   L = line2;

   clear();

   i[0] = &code;
   i[1] = &table_number;
   i[2] = &center;
   i[3] = &subcenter;

   //
   //  grab the first 3 ints
   //

   for (j = 0; j < 4; ++j) {

      if(!(c = strtok(L, i_delim))) return (false);

      if(!is_number(c))             return (false);

      *(i[j]) = atoi(c);

      L = (char *) 0;

   }   //  while

   //
   //  parm_name
   //

   if(!(c = strtok(0, s_delim))) return (false);

   parm_name = c;

   if(!(c = strtok(0, s_delim))) return (false);

   //
   //  full_name
   //

   if(!(c = strtok(0, s_delim))) return (false);

   full_name = c;

   if(!(c = strtok(0, s_delim))) return (false);

   //
   //  units (may be empty)
   //

   c = strtok(0, s_delim);

   if (c)
   {
      if (*c == ' ') {
         units.clear();
      }
      else {
         units = c;
      }
   }
   else
   {
      return (false) ;

   }

      //
      //  done
      //

delete [] line2;  line2 = (char *) 0;

return ( true );

}


////////////////////////////////////////////////////////////////////////


   //
   //  Code for class Grib2TableEntry
   //


////////////////////////////////////////////////////////////////////////


Grib2TableEntry::Grib2TableEntry()

{

init_from_scratch();

}


////////////////////////////////////////////////////////////////////////


Grib2TableEntry::~Grib2TableEntry()

{

clear();

}


////////////////////////////////////////////////////////////////////////


Grib2TableEntry::Grib2TableEntry(const Grib2TableEntry & e)

{

init_from_scratch();

assign(e);

}


////////////////////////////////////////////////////////////////////////


Grib2TableEntry & Grib2TableEntry::operator=(const Grib2TableEntry & e)

{

if ( this == &e )  return ( * this );

assign(e);

return ( * this );

}


////////////////////////////////////////////////////////////////////////


void Grib2TableEntry::init_from_scratch()

{

clear();

return;

}


////////////////////////////////////////////////////////////////////////


void Grib2TableEntry::clear()

{

index_a = index_b = index_c = mtab_set = mtab_low = mtab_high = cntr = ltab = -1;

parm_name.clear();

full_name.clear();

units.clear();

return;

}


////////////////////////////////////////////////////////////////////////


void Grib2TableEntry::assign(const Grib2TableEntry & e)

{

clear();

index_a = e.index_a;
index_b = e.index_b;
index_c = e.index_c;
mtab_high = e.mtab_high;
mtab_low = e.mtab_low;
mtab_set = e.mtab_set;
cntr = e.cntr;
ltab = e.ltab;

parm_name = e.parm_name;

full_name = e.full_name;

units = e.units;

return;

}


////////////////////////////////////////////////////////////////////////

/* _bp_
void Grib2TableEntry::dump(ostream & out, int depth) const

{

Indent prefix(depth);

out << prefix << "Index values = ("
              << index_a << ", "
              << mtab_set << ", "
              << mtab_low << ", "
              << mtab_high << ", "
              << cntr << ", "
              << ltab << ", "
              << index_b << ", "
              << index_c << ")\n";

out << prefix << "parm_name = " << parm_name.contents() << "\n";

out << prefix << "full_name = " << full_name.contents() << "\n";

out << prefix << "units     = " << units.contents()     << "\n";

return;

}
*/

////////////////////////////////////////////////////////////////////////


bool Grib2TableEntry::parse_line(const char * line)

{

clear();

int j, n;
int * i[8];
ConcatString * s[8];
const char i_delim [] = "{} \"";
const char s_delim [] = "\"";
const char * c = (const char *) 0;
char * line2 = (char *) 0;
char * L = (char *) 0;

n = strlen(line);

line2 = new char [1 + n];

strncpy(line2, line, n);

L = line2;

clear();

i[0] = &index_a;
i[1] = &mtab_set;
i[2] = &mtab_low;
i[3] = &mtab_high;
i[4] = &cntr;
i[5] = &ltab;
i[6] = &index_b;
i[7] = &index_c;

s[0] = &parm_name;
s[1] = &full_name;
s[2] = &units;

   //
   //  grab the first 8 ints
   //

for (j=0; j<8; ++j)  {

   if(!(c = strtok(L, i_delim))) return (false);

   if(!is_number(c))             return (false);

   *(i[j]) = atoi(c);

   L = (char *) 0;

}   //  while

   //
   //  grab the strings
   //

for (j=0; j<3; ++j)  {

   if(!(c = strtok(L, s_delim))) return (false);
   if(!(c = strtok(L, s_delim))) return (false);

   *(s[j]) = c;

   L = (char *) 0;

}   //  while

   //
   //  done
   //

delete [] line2;  line2 = (char *) 0;

return ( true );

}


////////////////////////////////////////////////////////////////////////


   //
   //  Code for class TableFlatFile
   //


////////////////////////////////////////////////////////////////////////


TableFlatFile::TableFlatFile()

{

init_from_scratch();

}

////////////////////////////////////////////////////////////////////////
/* _bp_
TableFlatFile::TableFlatFile(int, char *table_data_dir) {

   init_from_scratch();

   ConcatString path;
   ConcatString path1;

   // _bp_ readUserGribTables("grib1");

   path1 << cs_erase << table_data_dir;

   ///path1 = replace_path(path1);
   vector<ConcatString> filtered_file_names = vector<ConcatString>();

   get_table_files(path1, "grib1", ".txt", filtered_file_names);

   //
   //  read the default grib1 table file, expanding MET_BASE
   //
   for (unsigned uint i = 0; i < filtered_file_names.size(); i++) {

      path << cs_erase << table_data_dir << '/' << filtered_file_names[i];

      ///path = replace_path(path);

      if (!read(path)) {
        my_log("err #%s\n", "0x36084bad");

        return;
      }
   }
   //
   //  read the default grib2 table file, expanding MET_BASE
   //
   path1.clear();

   // _bp_ readUserGribTables("grib2");

   path1 << cs_erase << table_data_dir;

   ///path1 = replace_path(path1);

   filtered_file_names.clear();

   get_table_files(path1, "grib2", ".txt", filtered_file_names);

   for (unsigned uint i = 0; i < filtered_file_names.size(); i++)
   {

      path << cs_erase << table_data_dir << '/' << filtered_file_names[i];

      /// path = replace_path(path);

      if (!read(path)) {
        my_log("err #%s\n", "0xd5002553");
      }

   }

   //
   //  done
   //

}
*/

////////////////////////////////////////////////////////////////////////

/* _bp_ 
void TableFlatFile::readUserGribTables(const char * table_type) {
   ConcatString path_to_user_tables;
   ConcatString path;
   char *ptr;
   vector<ConcatString> filtered_file_names_user = vector<ConcatString>();

   if((ptr = getenv("USER_GRIB_TABLES")) != NULL)
   {
      path_to_user_tables = ptr;

      get_table_files(path_to_user_tables, table_type, ".txt", filtered_file_names_user);

      for (unsigned uint i = 0; i < filtered_file_names_user.size(); i++) {

         path << cs_erase << path_to_user_tables << '/' << filtered_file_names_user[i];

         if (!read(path)) {
           my_log("err #%s\n", 46, __COUNTER__);
         }
      }
   }
}
*/

////////////////////////////////////////////////////////////////////////


TableFlatFile::~TableFlatFile()

{

clear();

}


////////////////////////////////////////////////////////////////////////


TableFlatFile::TableFlatFile(const TableFlatFile & f)

{

init_from_scratch();

assign(f);

}


////////////////////////////////////////////////////////////////////////


void TableFlatFile::init_from_scratch()

{

g1e = (Grib1TableEntry **) 0;
g2e = (Grib2TableEntry **) 0;

clear();

}


////////////////////////////////////////////////////////////////////////


void TableFlatFile::clear()

{

int j;

if ( g1e )  {

   for (j=0; j<N_grib1_elements; ++j)  {

      if ( g1e[j] )  { delete g1e[j];  g1e[j] = (Grib1TableEntry *) 0; }

   }

   delete [] g1e; g1e = (Grib1TableEntry **) 0;

}


if ( g2e )  {

   for (j=0; j<N_grib2_elements; ++j)  {

      if ( g2e[j] )  { delete g2e[j];  g2e[j] = (Grib2TableEntry *) 0; }

   }

   delete [] g2e; g2e = (Grib2TableEntry* *) 0;

}

N_grib1_elements = 0;
N_grib2_elements = 0;

N_grib1_alloc = 0;
N_grib2_alloc = 0;

return;

}

////////////////////////////////////////////////////////////////////////

/* _bp_
void TableFlatFile::dump(ostream & out, int depth) const

{

int j;
Indent prefix(depth);


out << prefix << "N_grib1_elements = " << N_grib1_elements << "\n";

for (j=0; j<N_grib1_elements; ++j)  {

   out << prefix << "Grib1 Element # " << j << " ...\n";

   g1e[j]->dump(out, depth + 1);

}

out << prefix << "N_grib2_elements = " << N_grib2_elements << "\n";

for (j=0; j<N_grib2_elements; ++j)  {

   out << prefix << "Grib2 Element # " << j << " ...\n";

   g2e[j]->dump(out, depth + 1);

}


return;

}
*/

////////////////////////////////////////////////////////////////////////


void TableFlatFile::assign(const TableFlatFile & f)

{

clear();

int j;


if ( f.N_grib1_elements != 0 )  {

   N_grib1_elements = N_grib1_alloc = f.N_grib1_elements;

   g1e = new Grib1TableEntry * [N_grib1_elements];

   for (j=0; j<N_grib1_elements; ++j)  {

      g1e[j] = new Grib1TableEntry;

      *(g1e[j]) = *(f.g1e[j]);

   }

}

if ( f.N_grib2_elements != 0 )  {

   N_grib2_elements = N_grib2_alloc = f.N_grib2_elements;

   g2e = new Grib2TableEntry * [N_grib2_elements];

   for (j=0; j<N_grib2_elements; ++j)  {

      g2e[j] = new Grib2TableEntry;

      *(g2e[j]) = *(f.g2e[j]);

   }

}

return;

}


////////////////////////////////////////////////////////////////////////


void TableFlatFile::extend_grib1(int n)

{

if ( n <= N_grib1_alloc )  return;

int j;
Grib1TableEntry ** u = (Grib1TableEntry **) 0;

u = new Grib1TableEntry * [n];

for (j=0; j<n; ++j)  u[j] = (Grib1TableEntry *) 0;

if ( N_grib1_elements > 0 )  {

   for (j=0; j<N_grib1_elements; ++j)  u[j] = g1e[j];

   delete [] g1e;  g1e = (Grib1TableEntry **) 0;

}

g1e = u;

u = (Grib1TableEntry **) 0;

   //
   //  done
   //

N_grib1_alloc = n;

return;

}


////////////////////////////////////////////////////////////////////////


void TableFlatFile::extend_grib2(int n)

{

if ( n <= N_grib2_alloc )  return;

int j;
Grib2TableEntry ** u = (Grib2TableEntry **) 0;

u = new Grib2TableEntry * [n];

for (j=0; j<n; ++j)  u[j] = (Grib2TableEntry *) 0;

if ( N_grib2_elements > 0 )  {

   for (j=0; j<N_grib2_elements; ++j)  u[j] = g2e[j];

   delete [] g2e;  g2e = (Grib2TableEntry **) 0;

}

g2e = u;

u = (Grib2TableEntry **) 0;

   //
   //  done
   //

N_grib2_alloc = n;

return;

}


////////////////////////////////////////////////////////////////////////


bool TableFlatFile::read(const char * filename)

{

// clear();

ifstream in;
ConcatString line;
int n_lines;

if ( empty(filename) )  {
  my_log("err #%s\n", "0x3fa0bf50");

  return false;
}

   //
   //  add one in case there is no trailing new line
   //

n_lines = file_linecount(filename) + 1;

in.open(filename);

if ( !in )  {
  my_log("err #%s\n", "0x1f92933e");
  
  return false;
}

   //
   //  get first line for format
   //

line.read_line(in);

line.chomp('\n');

line.ws_strip();

     if ( line == "GRIB1" )  { return ( read_grib1(in, filename, n_lines - 1) ); }
else if ( line == "GRIB2" )  { return ( read_grib2(in, filename, n_lines - 1) ); }
else {

  my_log("err #%s\n", "0xf9b7830d");
  
  return false;
}

   //
   //  done
   //

in.close();

return ( false );

}


////////////////////////////////////////////////////////////////////////


bool TableFlatFile::read_grib1(istream & in, const char * filename, const int n)

{

int j;
ConcatString line;
bool status = false;

   //
   //  make room for the new elements
   //

extend_grib1(N_grib1_elements + n);

   //
   //  read the new elements
   //

j = 0;

while ( line.read_line(in) )  {

   //
   //  skip blank lines
   //

   if ( line.empty() )  continue;

   g1e[N_grib1_elements + j] = new Grib1TableEntry;

   //
   //  add newline in case it is missing from the last line of the file
   //

   line << "\n";

   status = g1e[N_grib1_elements + j]->parse_line(line);

   if ( ! status )  {

     my_log("err #%s\n", "0xd11c37e5");
     
     return false;
   }

   //
   //  increment counter
   //

   j++;

}  //  while

   //
   //  increment the number of elements
   //

N_grib1_elements += j;

   //
   //  done
   //

return ( true );

}


////////////////////////////////////////////////////////////////////////


bool TableFlatFile::read_grib2(istream & in, const char * filename, const int n)

{

int j;
ConcatString line;
bool status = false;

   //
   //  make room for the new elements
   //

extend_grib2(N_grib2_elements + n);

   //
   //  read the new elements
   //

j = 0;

while ( line.read_line(in) )  {

   //
   //  skip blank lines
   //

   if ( line.empty() )  continue;

   //
   //  add newline in case it is missing from the last line of the file
   //

   line << "\n";

   g2e[N_grib2_elements + j] = new Grib2TableEntry;

   status = g2e[N_grib2_elements + j]->parse_line(line);

   if ( ! status )  {
     my_log("err #%s\n", "0x56f75d20");
     
     return false;
   }

   //
   //  increment counter
   //

   j++;

}  //  while

   //
   //  done
   //

N_grib2_elements += j;

return ( true );

}


////////////////////////////////////////////////////////////////////////

/* _bp_
#ifdef _WINDOWS
#include "posixstat.h"
#endif

int TableFlatFile::get_table_files(const char *dir, const char *prefix, const char *postfix, vector<ConcatString> &files)
{
   DIR *dp;
   struct dirent *dirp;
   size_t prefix_length = strlen(prefix);
   size_t postfix_length = strlen(postfix);
   size_t max_length = prefix_length;

   if(max_length < postfix_length)
   {
      max_length=postfix_length;
   }

   dp = opendir( dir );
   if(dp == NULL)
   {
     my_log("err #%s\n", "0x113e1f7f");

     return errno;
   }

   while ((dirp = readdir(dp)) != NULL)
   {
      char filename[512];
      struct stat st;
      snprintf(filename, sizeof(filename), "%s/%s", dir, dirp->d_name);
      /// lstat(filename, &st);
      stat(filename, &st);
      if( !S_ISDIR(st.st_mode) && strlen(dirp->d_name) > max_length && strncmp(prefix,dirp->d_name,prefix_length) == 0  && strcmp(dirp->d_name+ strlen(dirp->d_name) - postfix_length, postfix) == 0)
      {
         files.push_back(dirp->d_name);
      }
   }
   closedir(dp);
   
   ///_bp_
   if (!files.size())
   {
     my_log("err #%s\n", "0xed067043");
   }
   return 0;
}
*/

////////////////////////////////////////////////////////////////////////


bool TableFlatFile::lookup_grib1(int code, int table_number, Grib1TableEntry & e)

{

int j;

e.clear();

for (j=0; j<N_grib1_elements; ++j)  {

   if ( (g1e[j]->code == code) && (g1e[j]->table_number == table_number) )  {

      e = *(g1e[j]);

      return ( true );

   }

}

return ( false );

}

////////////////////////////////////////////////////////////////////////

bool TableFlatFile::lookup_grib1(int code, int table_number, int center, int subcenter, Grib1TableEntry & e)
{
   int j;

   e.clear();
   int matching_subsenter;

   for (j=0; j<N_grib1_elements; ++j)  {

      matching_subsenter = subcenter;
      if( g1e[j]->subcenter == -1){
         matching_subsenter = -1;
      }

      if ( (g1e[j]->code == code) && (g1e[j]->table_number == table_number)
              && (g1e[j]->center == center)  && (g1e[j]->subcenter == matching_subsenter))  {

         e = *(g1e[j]);

         return ( true );

      }
   }
   return ( false );
}


////////////////////////////////////////////////////////////////////////


bool TableFlatFile::lookup_grib1(int code, Grib1TableEntry & e)   //  assumes table_number = 2;

{

const int table_number = 2;
bool status = false;

status = lookup_grib1(code, table_number, e);

return ( status );

}


////////////////////////////////////////////////////////////////////////


bool TableFlatFile::lookup_grib1(const char * parm_name, int table_number, int code,
                                 Grib1TableEntry & e, int & n_matches)

{

   //  clear the by-reference arguments
   e.clear();
   n_matches = 0;

   //  build a list of matches
   vector<Grib1TableEntry*> matches;
   for(int j=0; j < N_grib1_elements; j++){

      if( g1e[j]->parm_name != parm_name ||
          (bad_data_int != table_number && g1e[j]->table_number != table_number) ||
          (bad_data_int != code         && g1e[j]->code         != code        ) )
         continue;

      if( n_matches++ == 0 ) e = *(g1e[j]);
      matches.push_back( g1e[j] );

   }

   //  if there are multiple matches, print a descriptive warning
   if( 1 < n_matches ){

      ConcatString msg;
      my_log("warn #%s\n", "0xa983525d");

      /*for(vector<Grib1TableEntry*>::iterator it = matches.begin();
          it < matches.end(); it++)*/
        /// my_log("warn #%s\n","0x11b36981");
   }

   return (n_matches > 0);

}

////////////////////////////////////////////////////////////////////////

bool TableFlatFile::lookup_grib1(const char * parm_name, int table_number, int code,int center, int subcenter,
                  Grib1TableEntry & e, int & n_matches)
{

   //  clear the by-reference arguments
   e.clear();
   n_matches = 0;

   //  build a list of matches
   vector<Grib1TableEntry*> matches;
   int matching_subsenter;
   for(int j=0; j < N_grib1_elements; j++){
      matching_subsenter = subcenter;
      if( g1e[j]->subcenter == -1){
         matching_subsenter = -1;
      }

      if( g1e[j]->parm_name != parm_name ||
          (bad_data_int != table_number && g1e[j]->table_number != table_number) ||
          (bad_data_int != code         && g1e[j]->code         != code        ) ||
          (bad_data_int != center         && g1e[j]->center != center        )   ||
          (bad_data_int != matching_subsenter         && g1e[j]->subcenter != matching_subsenter)   )
         continue;

      if( n_matches++ == 0 ) e = *(g1e[j]);
      matches.push_back( g1e[j] );

   }

   //  if there are multiple matches, print a descriptive warning
   if( 1 < n_matches ){

     my_log("warn #%s\n", "0x221ac49c");

      // _bp_ for(vector<Grib1TableEntry*>::iterator it = matches.begin(); it < matches.end(); it++)
   }

   return (n_matches > 0);

}


////////////////////////////////////////////////////////////////////////


bool TableFlatFile::lookup_grib1(const char * parm_name, Grib1TableEntry & e)   //  assumes table number is 2

{

   int n_matches = -1;
   return lookup_grib1(parm_name, 2, bad_data_int, e, n_matches);

}


////////////////////////////////////////////////////////////////////////


bool TableFlatFile::lookup_grib2(int a, int b, int c, Grib2TableEntry & e)

{

int j;

e.clear();

for (j=0; j<N_grib2_elements; ++j)  {

   if ( (g2e[j]->index_a == a) && (g2e[j]->index_b == b) && (g2e[j]->index_c == c) )  {

      e = *(g2e[j]);

      return ( true );

   }

}

return ( false );

}


////////////////////////////////////////////////////////////////////////


bool TableFlatFile::lookup_grib2(int a, int b, int c,
                                 int mtab, int cntr, int ltab,
                                 Grib2TableEntry & e)
{
   int j;

   e.clear();

   for (j=0; j<N_grib2_elements; ++j)  {

      // Check discipline, parm_cat, and cat
      if ( g2e[j]->index_a != a ||
           g2e[j]->index_b != b ||
           g2e[j]->index_c != c ) continue;

      // Check local table
      if ( g2e[j]->ltab > 0 && g2e[j]->ltab != ltab ) continue;

      // Check master table
      if ( g2e[j]->mtab_low > mtab || g2e[j]->mtab_high < mtab ) continue;

      // Check center
      if ( g2e[j]->cntr > 0 && g2e[j]->cntr != cntr ) continue;

      e = *(g2e[j]);

      return ( true );

   }

   return ( false );
}



////////////////////////////////////////////////////////////////////////


bool TableFlatFile::lookup_grib2(const char * parm_name, int a, int b, int c,
                                 Grib2TableEntry & e, int & n_matches)

{

   //  clear the by-reference arguments
   e.clear();
   n_matches = 0;

   //  build a list of matches
   vector<Grib2TableEntry*> matches;
   for(int j=0; j<N_grib2_elements; ++j){

      if( g2e[j]->parm_name != parm_name ||
          (bad_data_int != a && g2e[j]->index_a != a) ||
          (bad_data_int != b && g2e[j]->index_b != b) ||
          (bad_data_int != c && g2e[j]->index_c != c) )
         continue;

      if( n_matches++ == 0 ) e = *(g2e[j]);
      matches.push_back( g2e[j] );

   }

   //  if there are multiple matches, print a descriptive warning
   if( 1 < n_matches ){
     my_log("warn #%s\n", "0xc387d714");

      // _bp_ for(vector<Grib2TableEntry*>::iterator it = matches.begin(); it < matches.end(); it++)
   }

   return (n_matches > 0);

}


////////////////////////////////////////////////////////////////////////


bool TableFlatFile::lookup_grib2(const char * parm_name,
                                 int a, int b, int c,
                                 int mtab, int cntr, int ltab,
                                 Grib2TableEntry & e, int & n_matches)
{
   //  clear the by-reference arguments
   e.clear();
   n_matches = 0;

   //  build a list of matches
   vector<Grib2TableEntry*> matches;
   for(int j=0; j<N_grib2_elements; ++j){

      if( g2e[j]->parm_name != parm_name ||
          (bad_data_int != a    && g2e[j]->index_a != a) ||
          (bad_data_int != b    && g2e[j]->index_b != b) ||
          (bad_data_int != c    && g2e[j]->index_c != c) ||
          (bad_data_int != mtab && g2e[j]->mtab_low  > mtab) ||
          (bad_data_int != mtab && g2e[j]->mtab_high < mtab) ||
          (bad_data_int != cntr && g2e[j]->cntr > 0 && g2e[j]->cntr != cntr) ||
          (bad_data_int != ltab && g2e[j]->ltab > 0 && g2e[j]->ltab != ltab) )
         continue;

      if( n_matches++ == 0 ) e = *(g2e[j]);
      matches.push_back( g2e[j] );

   }

   //  if there are multiple matches, print a descriptive warning
   if( 1 < n_matches ){
     my_log("warn #%s\n", "0x2413a8f9");

      // _bp_ for(vector<Grib2TableEntry*>::iterator it = matches.begin(); it < matches.end(); it++)
   }

   return (n_matches > 0);
}



////////////////////////////////////////////////////////////////////////


bool TableFlatFile::lookup_grib2(const char * parm_name, Grib2TableEntry & e, int & n_matches)

{

   return lookup_grib2(parm_name, bad_data_int, bad_data_int, bad_data_int, e, n_matches);

}






////////////////////////////////////////////////////////////////////////
