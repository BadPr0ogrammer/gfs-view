// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_CONFIG_CONFIG_TAB_H_INCLUDED
# define YY_CONFIG_CONFIG_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int configdebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IDENTIFIER = 258,
    QUOTED_STRING = 259,
    INTEGER = 260,
    FLOAT = 261,
    BOOLEAN = 262,
    COMPARISON = 263,
    NA_COMPARISON = 264,
    LOGICAL_OP_NOT = 265,
    LOGICAL_OP_AND = 266,
    LOGICAL_OP_OR = 267,
    FORTRAN_THRESHOLD = 268,
    UNARY_MINUS = 269
  };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define QUOTED_STRING 259
#define INTEGER 260
#define FLOAT 261
#define BOOLEAN 262
#define COMPARISON 263
#define NA_COMPARISON 264
#define LOGICAL_OP_NOT 265
#define LOGICAL_OP_AND 266
#define LOGICAL_OP_OR 267
#define FORTRAN_THRESHOLD 268
#define UNARY_MINUS 269

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 154 "config.tab.yy" /* yacc.c:1909  */


   char text[max_id_length];

   Number nval;

   bool bval;

   ThreshType cval;

   ThreshNode * node;


#line 96 "config.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE configlval;

int configparse (void);

#endif /* !YY_CONFIG_CONFIG_TAB_H_INCLUDED  */
