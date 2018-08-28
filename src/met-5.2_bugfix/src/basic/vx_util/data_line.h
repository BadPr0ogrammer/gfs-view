// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*



////////////////////////////////////////////////////////////////////////


#ifndef  __DATA_LINE_H__
#define  __DATA_LINE_H__


////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <fstream>

#include "vx_log.h"


////////////////////////////////////////////////////////////////////////


static const int dataline_charextend_alloc_inc = 512;

static const int dataline_intextend_alloc_inc  = 100;


////////////////////////////////////////////////////////////////////////


class LineDataFile;  //  forward reference


////////////////////////////////////////////////////////////////////////


class DataLine {

      friend class LineDataFile;
      friend ostream & operator<<(ostream &, const DataLine &);
      // _bp_ friend Logger & operator<<(Logger &, const DataLine &);

   protected:

      void init_from_scratch();

      void assign(const DataLine &);

      void extend_char  (int);
      void extend_int   (int);

      char * Line;

      int N_chars;


      int * Offset;

      int N_ints;


      int N_items;

      int LineNumber;

      char *Delimiter;

      LineDataFile * File;  //  not allocated
      
   public:

      DataLine();
      virtual ~DataLine();
      DataLine(const DataLine &);
      DataLine & operator=(const DataLine &);


      void clear();

      // _bp_ void dump(ostream &, int depth = 0) const;

         //
         //  retrieve stuff
         //

      const char * get_line() const;

      const char * get_item(int) const;

      const char * get_delimiter() const;

      const LineDataFile * get_file() const;

      int n_items() const;

      const char * operator [] (int) const;

      int line_number() const;

      int max_item_width() const;

         //
         //  read line from file
         //

      virtual int read_line(LineDataFile *);

      virtual int read_fwf_line(LineDataFile *, const int *wdth, int n_wdth);

      virtual int is_ok() const;

      virtual int is_header() const;

      virtual void set_delimiter(const char *delimiter);
      
};


////////////////////////////////////////////////////////////////////////


inline  int           DataLine::n_items      () const { return ( N_items ); }

inline  int           DataLine::line_number  () const { return ( LineNumber ); }

inline  const char *  DataLine::get_line     () const { return ( Line ); }

inline  const char *  DataLine::get_delimiter() const { return ( Delimiter ); }


////////////////////////////////////////////////////////////////////////


class LineDataFile {

   private:

      void init_from_scratch();

      LineDataFile(const LineDataFile &);
      LineDataFile & operator=(const LineDataFile &);

      char * Filename;

      char * ShortFilename;

      int Last_Line_Number;

      StringArray Header;

      void set_header(DataLine &);

   public:

      LineDataFile();
     ~LineDataFile();

      ifstream * in;

      int open(const char *);

      void close();

      void rewind();

      int ok() const;

      int operator>>(DataLine &);

      int read_fwf_line(DataLine &, const int *wdth, int n_wdth);

      const char * filename() const;
      const char * short_filename() const;

      int last_line_number() const;

      const StringArray & header() const;

};


////////////////////////////////////////////////////////////////////////


inline const char * LineDataFile::filename() const { return ( Filename ); }

inline const char * LineDataFile::short_filename() const { return ( ShortFilename ); }

inline int LineDataFile::last_line_number() const { return ( Last_Line_Number ); }

inline const StringArray & LineDataFile::header() const { return ( Header ); }


////////////////////////////////////////////////////////////////////////


extern ostream & operator<<(ostream &, const DataLine &);


////////////////////////////////////////////////////////////////////////


// _bp_ extern Logger & operator<<(Logger &, const DataLine &);


////////////////////////////////////////////////////////////////////////


#endif   /*  __DATA_LINE_H__  */


////////////////////////////////////////////////////////////////////////


