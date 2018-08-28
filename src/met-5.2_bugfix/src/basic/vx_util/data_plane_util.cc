// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

////////////////////////////////////////////////////////////////////////

using namespace std;

#include <cstdlib>
#include <iostream>
#include <math.h>
#include <string.h>
#include <unistd.h>

#ifdef _WINDOWS
#include <float.h>
#define isnan _isnan
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif
void my_log(const char *frmt, const char *s = NULL);

#include "data_plane_util.h"
#include "interp_util.h"
#include "two_to_one.h"

#include "vx_math.h"
#include "vx_log.h"

////////////////////////////////////////////////////////////////////////
//
// Utility functions operating on a DataPlane
//
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//
// Check the range of probability values and make sure it's either
// [0, 1] or [0, 100].  If it's [0, 100], rescale to [0, 1].
//
////////////////////////////////////////////////////////////////////////

void rescale_probability(DataPlane &dp) {
   double v, min_v, max_v;
   int x, y;

   //
   // Get the range of data values in the field
   //
   dp.data_range(min_v, max_v);

   //
   // Check for a valid range of probability values.
   //
   if(min_v < 0.0-loose_tol || max_v > 100.0+loose_tol) {
     my_log("err #%s\n", "0x1d3b5277");

     return;
   }

   //
   // If necessary, rescale data from [0, 100] to [0, 1]
   //
   if(max_v > 1.0) {
      //
      // Divide each value by 100
      //
      for(x=0; x<dp.nx(); x++) {
         for(y=0; y<dp.ny(); y++) {

            v = dp.get(x, y);
            if(!is_bad_data(v)) dp.set(v/100.0, x, y);

         } // end for y
      } // end for x

   }

   return;

}

////////////////////////////////////////////////////////////////////////
//
// Smooth the DataPlane values using the interpolation method and
// box width specified.
//
////////////////////////////////////////////////////////////////////////

void smooth_field(const DataPlane &dp, DataPlane &smooth_dp,
                  InterpMthd mthd, int wdth, double t) {
   double v;
   int x, y, x_ll, y_ll;
   
   // Initialize the smoothed field to the raw field
   smooth_dp = dp;

   // Check for a width value of 1 for which no smoothing is done
   if(wdth <= 1) return;

   // Otherwise, apply smoothing to each grid point
   for(x=0; x<dp.nx(); x++) {
      for(y=0; y<dp.ny(); y++) {

         // The neighborhood width must be odd, find the lower-left
         // corner of the neighborhood
         x_ll = x - (wdth - 1)/2;
         y_ll = y - (wdth - 1)/2;

         // Compute the smoothed value based on the interpolation method
         switch(mthd) {

            case(InterpMthd_Min):     // Minimum
               v = interp_min(dp, x_ll, y_ll, wdth, t);
               break;

            case(InterpMthd_Max):     // Maximum
               v = interp_max(dp, x_ll, y_ll, wdth, t);
               break;

            case(InterpMthd_Median):  // Median
               v = interp_median(dp, x_ll, y_ll, wdth, t);
               break;

            case(InterpMthd_UW_Mean): // Unweighted Mean
               v = interp_uw_mean(dp, x_ll, y_ll, wdth, t);
               break;

            // Distance-weighted mean, least-squares fit, and bilinear
            // interpolation are omitted here since they are not
            // options for gridded data

            default:
              my_log("err #%s\n", "0xdf29b96e");

               return;               
         }

         // Store the smoothed value
         smooth_dp.set(v, x, y);

      } // end for y
   } // end for x

   return;
}

////////////////////////////////////////////////////////////////////////
//
// Convert the DataPlane field to the corresponding fractional coverage
// using the threshold critea specified.
//
////////////////////////////////////////////////////////////////////////

void fractional_coverage(const DataPlane &dp, DataPlane &frac_dp,
                         int wdth, SingleThresh t, double vld_t) {
   int i, j, k, n, x, y, x_ll, y_ll, y_ur, xx, yy, half_width;
   double v;
   int count_vld, count_thr;
   NumArray box_na;

   // Check that width is set to 1 or greater
   if(wdth < 1) {
     my_log("err #%s\n", "0x56b0839d");

     return;
   }

   // Initialize the fractional coverage field
   frac_dp = dp;
   frac_dp.set_constant(bad_data_double);

   // Compute the box half-width
   half_width = (wdth - 1)/2;

   // Initialize the box
   for(i=0; i<wdth*wdth; i++) box_na.add(bad_data_int);

   // Compute the fractional coverage meeting the threshold criteria
   for(x=0; x<dp.nx(); x++) {

      // Find the lower-left x-coordinate of the neighborhood
      x_ll = x - half_width;

      for(y=0; y<dp.ny(); y++) {

         // Find the lower-left y-coordinate of the neighborhood
         y_ll = y - half_width;
         y_ur = y + half_width;

         // Initialize the box for this new column
         if(y == 0) {

            // Initialize counts
            count_vld = count_thr = 0;

            for(i=0; i<wdth; i++) {

               xx = x_ll + i;

               for(j=0; j<wdth; j++) {

                  yy = y_ll + j;

                  n = DefaultTO.two_to_one(wdth, wdth, i, j);

                  // Check for being off the grid
                  if(xx < 0 || xx >= dp.nx() ||
                     yy < 0 || yy >= dp.ny()) {
                     k = bad_data_int;
                  }
                  // Check the value of v to see if it meets the threshold criteria
                  else {
                     v = dp.get(xx, yy);
                     if(is_bad_data(v))  k = bad_data_int;
                     else if(t.check(v)) k = 1;
                     else                k = 0;
                  }
                  box_na.set(n, k);

                  // Increment the counts
                  if(!is_bad_data(k)) {
                     count_vld += 1;
                     count_thr += k;
                  }

               } // end for j
            } // end for i
         } // end if

         // Otherwise, update one row of the box
         else {

            // Compute the row of the neighborhood box to be updated
            j = (y - 1) % wdth;

            for(i=0; i<wdth; i++) {

               // Index into the box
               n = DefaultTO.two_to_one(wdth, wdth, i, j);

               // Get x and y values to be checked
               xx = x_ll + i;
               yy = y_ur;

               // Decrement counts for data to be replaced
               k = nint(box_na[n]);
               if(!is_bad_data(k)) {
                  count_vld -= 1;
                  count_thr -= k;
               }

               // Check for being off the grid
               if(xx < 0 || xx >= dp.nx() ||
                  yy < 0 || yy >= dp.ny()) {
                  k = bad_data_int;
               }
               // Check the value of v to see if it meets the threshold criteria
               else {
                  v = dp.get(xx, yy);
                  if(is_bad_data(v))  k = bad_data_int;
                  else if(t.check(v)) k = 1;
                  else                k = 0;
               }
               box_na.set(n, k);

               // Increment the counts
               if(!is_bad_data(k)) {
                  count_vld += 1;
                  count_thr += k;
               }

            } // end for i
         } // end else

         // Check whether enough valid grid points were found
         if((double) count_vld/(wdth*wdth) < vld_t ||
            count_vld == 0) {
            v = bad_data_double;
         }
         // Compute the fractional coverage
         else {
            v = (double) count_thr/count_vld;
         }

         // Store the fractional coverage value
         frac_dp.set(v, x, y);

      } // end for y
   } // end for x

   return;

}

////////////////////////////////////////////////////////////////////////

void apply_mask(const DataPlane &dp, const DataPlane &mask_dp,
                NumArray &na) {
   int x, y;
   double v;

   // Initialize the NumArray object
   na.empty();

   // Loop through the mask data points
   for(x=0; x<mask_dp.nx(); x++) {
      for(y=0; y<mask_dp.ny(); y++) {

         // Skip points where the mask is off
         if(is_bad_data(mask_dp.get(x, y)) ||
            !mask_dp.s_is_on(x, y)) continue;

         // Get the current value
         v = (dp.nx() == 0 && dp.ny() == 0 ?
              bad_data_double : dp.get(x, y));

         // Store the value
         na.add(v);
      } // end for y
   } // end for x

   return;
}

///////////////////////////////////////////////////////////////////////////////

void apply_mask(DataPlane &dp, const DataPlane &mask_dp) {
   int x, y;

   if(dp.nx() != mask_dp.nx() ||
      dp.ny() != mask_dp.ny() ) {

     my_log("err #%s\n", "0xbad0dfcb");

      return;
   }

   for(x=0; x<dp.nx(); x++) {
      for(y=0; y<dp.ny(); y++) {

         // Put bad data everywhere the mask is bad data or turned off
         if(is_bad_data(mask_dp(x, y)) || !mask_dp(x, y)) dp.set(bad_data_double, x, y);
      }
   }

   return;
}

////////////////////////////////////////////////////////////////////////

void mask_bad_data(DataPlane &dp, const DataPlane &mask_dp, double v) {
   int x, y;

   if(dp.nx() != mask_dp.nx() ||
      dp.ny() != mask_dp.ny() ) {

     my_log("err #%s\n", "0xea538695");

     return;
   }

   for(x=0; x<dp.nx(); x++) {
      for(y=0; y<dp.ny(); y++) {

         if(is_bad_data(mask_dp.get(x, y)))
            dp.set(v, x, y);
      }
   }

   return;
}

////////////////////////////////////////////////////////////////////////
