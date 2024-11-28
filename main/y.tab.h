/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    T_EndOfFile = 258,             /* T_EndOfFile  */
    T_Return = 259,                /* T_Return  */
    T_Number = 260,                /* T_Number  */
    T_True = 261,                  /* T_True  */
    T_False = 262,                 /* T_False  */
    T_ID = 263,                    /* T_ID  */
    T_Print = 264,                 /* T_Print  */
    T_Range = 265,                 /* T_Range  */
    T_Cln = 266,                   /* T_Cln  */
    T_NL = 267,                    /* T_NL  */
    T_EQL = 268,                   /* T_EQL  */
    T_NEQ = 269,                   /* T_NEQ  */
    T_EQ = 270,                    /* T_EQ  */
    T_GT = 271,                    /* T_GT  */
    T_LT = 272,                    /* T_LT  */
    T_EGT = 273,                   /* T_EGT  */
    T_ELT = 274,                   /* T_ELT  */
    T_Or = 275,                    /* T_Or  */
    T_And = 276,                   /* T_And  */
    T_Not = 277,                   /* T_Not  */
    T_In = 278,                    /* T_In  */
    ID = 279,                      /* ID  */
    ND = 280,                      /* ND  */
    DD = 281,                      /* DD  */
    T_String = 282,                /* T_String  */
    T_If = 283,                    /* T_If  */
    T_Elif = 284,                  /* T_Elif  */
    T_While = 285,                 /* T_While  */
    T_For = 286,                   /* T_For  */
    T_Else = 287,                  /* T_Else  */
    T_Break = 288,                 /* T_Break  */
    T_MN = 289,                    /* T_MN  */
    T_PL = 290,                    /* T_PL  */
    T_DV = 291,                    /* T_DV  */
    T_ML = 292,                    /* T_ML  */
    T_OP = 293,                    /* T_OP  */
    T_CP = 294,                    /* T_CP  */
    T_OB = 295,                    /* T_OB  */
    T_CB = 296,                    /* T_CB  */
    T_Def = 297,                   /* T_Def  */
    T_Comma = 298,                 /* T_Comma  */
    T_List = 299                   /* T_List  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define T_EndOfFile 258
#define T_Return 259
#define T_Number 260
#define T_True 261
#define T_False 262
#define T_ID 263
#define T_Print 264
#define T_Range 265
#define T_Cln 266
#define T_NL 267
#define T_EQL 268
#define T_NEQ 269
#define T_EQ 270
#define T_GT 271
#define T_LT 272
#define T_EGT 273
#define T_ELT 274
#define T_Or 275
#define T_And 276
#define T_Not 277
#define T_In 278
#define ID 279
#define ND 280
#define DD 281
#define T_String 282
#define T_If 283
#define T_Elif 284
#define T_While 285
#define T_For 286
#define T_Else 287
#define T_Break 288
#define T_MN 289
#define T_PL 290
#define T_DV 291
#define T_ML 292
#define T_OP 293
#define T_CP 294
#define T_OB 295
#define T_CB 296
#define T_Def 297
#define T_Comma 298
#define T_List 299

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 1023 "translator.y"
 char *text; int depth; int scope_depth; struct ASTNode* node;

#line 158 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
