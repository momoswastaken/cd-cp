#ifndef _yy_defines_h_
#define _yy_defines_h_

#define T_EndOfFile 257
#define T_Return 258
#define T_Number 259
#define T_True 260
#define T_False 261
#define T_ID 262
#define T_Print 263
#define T_Range 264
#define T_Cln 265
#define T_NL 266
#define T_EQL 267
#define T_NEQ 268
#define T_EQ 269
#define T_GT 270
#define T_LT 271
#define T_EGT 272
#define T_ELT 273
#define T_Or 274
#define T_And 275
#define T_Not 276
#define T_In 277
#define ID 278
#define ND 279
#define DD 280
#define T_String 281
#define T_If 282
#define T_Elif 283
#define T_While 284
#define T_For 285
#define T_Else 286
#define T_Break 287
#define T_MN 288
#define T_PL 289
#define T_DV 290
#define T_ML 291
#define T_OP 292
#define T_CP 293
#define T_OB 294
#define T_CB 295
#define T_Def 296
#define T_Comma 297
#define T_List 298
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union YYSTYPE { char *text; int depth; int scope_depth; struct ASTNode* node;} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
/* Default: YYLTYPE is the text position type. */
typedef struct YYLTYPE
{
    int first_line;
    int first_column;
    int last_line;
    int last_column;
    unsigned source;
} YYLTYPE;
#define YYLTYPE_IS_DECLARED 1
#endif
#define YYRHSLOC(rhs, k) ((rhs)[k])
extern YYLTYPE yylloc;

#endif /* _yy_defines_h_ */
