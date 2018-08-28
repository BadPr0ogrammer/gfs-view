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
#include <cmath>

#ifdef _WINDOWS
#include <float.h>
#include <math.h>
#define isnan _isnan
#define strcasecmp strcmpi
#endif
void my_log(const char *frmt, const char *s = NULL);

#include "vx_log.h"
#include "indent.h"
#include "data_class.h"

////////////////////////////////////////////////////////////////////////


   //
   //  Code for class Met2dData
   //


////////////////////////////////////////////////////////////////////////


Met2dData::Met2dData()

{

mtdd_init_from_scratch();

}


////////////////////////////////////////////////////////////////////////


Met2dData::~Met2dData()

{


}


////////////////////////////////////////////////////////////////////////


void Met2dData::mtdd_init_from_scratch()

{


return;

}


////////////////////////////////////////////////////////////////////////


   //
   //  Code for class Met2dDataFile
   //


////////////////////////////////////////////////////////////////////////


Met2dDataFile::Met2dDataFile()

{

mtddf_init_from_scratch();

}


////////////////////////////////////////////////////////////////////////


Met2dDataFile::~Met2dDataFile()

{

mtddf_clear();

}


////////////////////////////////////////////////////////////////////////


void Met2dDataFile::mtddf_init_from_scratch()

{

Raw_Grid  = (Grid *) 0;
Dest_Grid = (Grid *) 0;

ShiftRight = 0;

return;

}


////////////////////////////////////////////////////////////////////////


void Met2dDataFile::mtddf_clear()

{

// mttd_clear();

if ( Raw_Grid  )  { delete Raw_Grid;   Raw_Grid  = (Grid *) 0; }
if ( Dest_Grid )  { delete Dest_Grid;  Dest_Grid = (Grid *) 0; }

Filename.clear();

ShiftRight = 0;

return;

}


////////////////////////////////////////////////////////////////////////

/* _bp_
void Met2dDataFile::dump(ostream & out, int depth) const

{

Indent prefix(depth);

out << prefix << "File = ";

if ( Filename.empty() )  out << "(nul)\n";
else                     out << '\"' << Filename << "\"\n";

out << prefix << "Raw Grid = ";

if ( Raw_Grid )  {

   out << '\n';

   Raw_Grid->dump(out, depth + 1);

} else out << "(nul)\n";


out << prefix << "Dest Grid = ";

if ( Dest_Grid )  {

   out << '\n';

   Dest_Grid->dump(out, depth + 1);

} else out << "(nul)\n";

out << prefix << "ShiftRight = " << ShiftRight << '\n';
   
   //
   //  done
   //

out.flush();

return;

}
*/

////////////////////////////////////////////////////////////////////////

const Grid & Met2dDataFile::grid() const

{

if ( ! Dest_Grid )  {
    my_log("err #%s\n", "0x710f568f");

   return nullptr;
}

return ( *Dest_Grid );

}


////////////////////////////////////////////////////////////////////////


const Grid & Met2dDataFile::raw_grid() const

{

if ( ! Raw_Grid )  {
  my_log("err #%s\n", "0xf687c10e");

   return nullptr;
}

return ( *Raw_Grid );

}


////////////////////////////////////////////////////////////////////////


void Met2dDataFile::set_shift_right(int N)

{

ShiftRight = N;

Dest_Grid->shift_right(ShiftRight);

return;

}


////////////////////////////////////////////////////////////////////////


void Met2dDataFile::copy_raw_grid_to_dest()

{

if ( ! Raw_Grid )  {
  my_log("err #%s\n", "0xf5e19dd4");

  return;
}

if ( Dest_Grid )  { delete Dest_Grid;  Dest_Grid = 0; }

Dest_Grid = new Grid;

(*Dest_Grid) = (*Raw_Grid);

return;

}


////////////////////////////////////////////////////////////////////////
