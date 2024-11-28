/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "project.y"

    #include <stdio.h>
    #include <iostream>
    #include <string>
    #include <vector>
    #include <map>
    #include "y.tab.h"
    using namespace std;
    extern FILE *yyin;
    extern int yylex();
    void yyerror(string s);
    int numOfTabs = 0;
    int linenum = 1;
    
    #define INT_TYPE 1
    #define FLOAT_TYPE 2
    #define STRING_TYPE 3

    vector<bool> ifScopeVector;
    map<string, int> typeMap;
    
    string cppFile = "void main()\n{\n";
    string tmp = "";
    string assignment = "";
    string element;
    string intVars = "";
    string floatVars = "";
    string stringVars = "";
    string stringSign;

    int types[2];
    int requiredTabNum = 0;
    bool flag = false;
    bool isPlus;

    char *variable;

    // Function declarations
    void handleConditionStatements(string conditionWord);
    void tabCheck();
    void primitiveTypeDetect(int type, string element);
    void handleAssignment();
    void printParseTree(string nodeName, int depth);
    void printSymbolTable();

    struct TreeNode {
        string name;
        vector<TreeNode*> children;
    };

    TreeNode* root = nullptr;
    TreeNode* currentNode = nullptr;


#line 126 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
    VAR = 258,                     /* VAR  */
    STRING = 259,                  /* STRING  */
    PLUS = 260,                    /* PLUS  */
    MINUS = 261,                   /* MINUS  */
    MULT = 262,                    /* MULT  */
    DIV = 263,                     /* DIV  */
    INT = 264,                     /* INT  */
    FLOAT = 265,                   /* FLOAT  */
    EQUAL = 266,                   /* EQUAL  */
    IF = 267,                      /* IF  */
    ELIF = 268,                    /* ELIF  */
    ELSE = 269,                    /* ELSE  */
    COMPARISON = 270,              /* COMPARISON  */
    NWLN = 271,                    /* NWLN  */
    COLON = 272,                   /* COLON  */
    TAB = 273                      /* TAB  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define VAR 258
#define STRING 259
#define PLUS 260
#define MINUS 261
#define MULT 262
#define DIV 263
#define INT 264
#define FLOAT 265
#define EQUAL 266
#define IF 267
#define ELIF 268
#define ELSE 269
#define COMPARISON 270
#define NWLN 271
#define COLON 272
#define TAB 273

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 57 "project.y"

    char *str;

#line 219 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_VAR = 3,                        /* VAR  */
  YYSYMBOL_STRING = 4,                     /* STRING  */
  YYSYMBOL_PLUS = 5,                       /* PLUS  */
  YYSYMBOL_MINUS = 6,                      /* MINUS  */
  YYSYMBOL_MULT = 7,                       /* MULT  */
  YYSYMBOL_DIV = 8,                        /* DIV  */
  YYSYMBOL_INT = 9,                        /* INT  */
  YYSYMBOL_FLOAT = 10,                     /* FLOAT  */
  YYSYMBOL_EQUAL = 11,                     /* EQUAL  */
  YYSYMBOL_IF = 12,                        /* IF  */
  YYSYMBOL_ELIF = 13,                      /* ELIF  */
  YYSYMBOL_ELSE = 14,                      /* ELSE  */
  YYSYMBOL_COMPARISON = 15,                /* COMPARISON  */
  YYSYMBOL_NWLN = 16,                      /* NWLN  */
  YYSYMBOL_COLON = 17,                     /* COLON  */
  YYSYMBOL_TAB = 18,                       /* TAB  */
  YYSYMBOL_YYACCEPT = 19,                  /* $accept  */
  YYSYMBOL_program = 20,                   /* program  */
  YYSYMBOL_newline = 21,                   /* newline  */
  YYSYMBOL_newlines = 22,                  /* newlines  */
  YYSYMBOL_statements = 23,                /* statements  */
  YYSYMBOL_tab = 24,                       /* tab  */
  YYSYMBOL_comparison_expr = 25,           /* comparison_expr  */
  YYSYMBOL_if = 26,                        /* if  */
  YYSYMBOL_elif = 27,                      /* elif  */
  YYSYMBOL_else = 28,                      /* else  */
  YYSYMBOL_var = 29,                       /* var  */
  YYSYMBOL_statement = 30,                 /* statement  */
  YYSYMBOL_if_statement = 31,              /* if_statement  */
  YYSYMBOL_elif_statement = 32,            /* elif_statement  */
  YYSYMBOL_else_statement = 33,            /* else_statement  */
  YYSYMBOL_assign_statements = 34,         /* assign_statements  */
  YYSYMBOL_35_1 = 35,                      /* $@1  */
  YYSYMBOL_assign_statement = 36,          /* assign_statement  */
  YYSYMBOL_signed_operand = 37,            /* signed_operand  */
  YYSYMBOL_38_2 = 38,                      /* $@2  */
  YYSYMBOL_sec_signed_operand = 39,        /* sec_signed_operand  */
  YYSYMBOL_40_3 = 40,                      /* $@3  */
  YYSYMBOL_operand = 41,                   /* operand  */
  YYSYMBOL_sign = 42,                      /* sign  */
  YYSYMBOL_operator = 43                   /* operator  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  21
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   41

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  19
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  42
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  59

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   273


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    66,    66,    67,    71,    75,    76,    79,    80,    81,
      85,    89,    96,   102,   108,   114,   128,   129,   130,   131,
     132,   136,   165,   194,   217,   217,   226,   229,   240,   240,
     254,   254,   263,   277,   281,   285,   292,   293,   294,   298,
     299,   300,   301
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "VAR", "STRING",
  "PLUS", "MINUS", "MULT", "DIV", "INT", "FLOAT", "EQUAL", "IF", "ELIF",
  "ELSE", "COMPARISON", "NWLN", "COLON", "TAB", "$accept", "program",
  "newline", "newlines", "statements", "tab", "comparison_expr", "if",
  "elif", "else", "var", "statement", "if_statement", "elif_statement",
  "else_statement", "assign_statements", "$@1", "assign_statement",
  "signed_operand", "$@2", "sec_signed_operand", "$@3", "operand", "sign",
  "operator", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-45)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      -2,   -45,   -45,   -45,   -45,   -45,   -45,     3,    -7,     5,
     -45,     5,     0,     0,   -45,    13,    -7,   -45,   -45,   -45,
     -45,   -45,   -45,   -45,   -45,   -45,   -45,    14,   -45,    14,
       0,     5,   -45,     0,    17,     0,   -45,   -45,    18,   -45,
     -45,   -45,   -45,   -45,   -45,    19,    17,   -45,    17,   -45,
      26,   -45,   -45,   -45,   -45,   -45,   -45,    17,   -45
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       7,    15,    12,    13,    14,     4,    10,     0,     5,     7,
       2,     0,    36,    36,    23,     0,     8,    17,    18,    19,
      20,     1,     6,     3,    16,    37,    38,     0,    28,     0,
      36,     7,    11,    36,     0,    36,    24,     9,     0,    30,
      32,    35,    33,    34,    29,     0,     0,    21,     0,    22,
      25,    26,    31,    39,    40,    41,    42,     0,    27
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -45,   -45,   -45,    -1,    -9,   -45,     8,   -45,   -45,   -45,
     -45,    27,   -45,   -45,   -45,   -45,   -45,   -45,    28,   -45,
       4,   -45,   -44,    -5,   -45
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     7,     8,     9,    10,    11,    33,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    46,    50,    27,    34,
      38,    48,    44,    28,    57
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      23,     1,    51,    21,    52,    25,    26,    22,     1,     5,
       2,     3,     4,    58,     5,    31,     6,     2,     3,     4,
      40,    41,    37,     6,    30,    36,    42,    43,    39,    32,
      39,    53,    54,    55,    56,    47,    49,    35,    24,    45,
       0,    29
};

static const yytype_int8 yycheck[] =
{
       9,     3,    46,     0,    48,     5,     6,     8,     3,    16,
      12,    13,    14,    57,    16,    16,    18,    12,    13,    14,
       3,     4,    31,    18,    11,    30,     9,    10,    33,    15,
      35,     5,     6,     7,     8,    17,    17,    29,    11,    35,
      -1,    13
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    12,    13,    14,    16,    18,    20,    21,    22,
      23,    24,    26,    27,    28,    29,    30,    31,    32,    33,
      34,     0,    22,    23,    30,     5,     6,    37,    42,    37,
      11,    22,    15,    25,    38,    25,    42,    23,    39,    42,
       3,     4,     9,    10,    41,    39,    35,    17,    40,    17,
      36,    41,    41,     5,     6,     7,     8,    43,    41
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    19,    20,    20,    21,    22,    22,    23,    23,    23,
      24,    25,    26,    27,    28,    29,    30,    30,    30,    30,
      30,    31,    32,    33,    35,    34,    36,    36,    38,    37,
      40,    39,    41,    41,    41,    41,    42,    42,    42,    43,
      43,    43,    43
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     2,     0,     1,     3,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     5,     5,     1,     0,     5,     1,     3,     0,     3,
       0,     3,     1,     1,     1,     1,     0,     1,     1,     1,
       1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4: /* newline: NWLN  */
#line 71 "project.y"
         { linenum++; }
#line 1274 "y.tab.c"
    break;

  case 10: /* tab: TAB  */
#line 85 "project.y"
        { numOfTabs++; }
#line 1280 "y.tab.c"
    break;

  case 11: /* comparison_expr: COMPARISON  */
#line 89 "project.y"
               {
        string element((yyvsp[0].str));
        tmp += " " + element;
    }
#line 1289 "y.tab.c"
    break;

  case 12: /* if: IF  */
#line 96 "project.y"
       {
        handleConditionStatements("if");
    }
#line 1297 "y.tab.c"
    break;

  case 13: /* elif: ELIF  */
#line 102 "project.y"
         {
        handleConditionStatements("else if");
    }
#line 1305 "y.tab.c"
    break;

  case 14: /* else: ELSE  */
#line 108 "project.y"
         {
        handleConditionStatements("else");
    }
#line 1313 "y.tab.c"
    break;

  case 15: /* var: VAR  */
#line 114 "project.y"
        {
        variable = (yyvsp[0].str);
        for (int i = requiredTabNum; i >= numOfTabs + 1; i--) {
            for (int j = 0; j < i; j++)
                tmp += "\t";
            tmp += "}\n";
        }
        for (int i = 0; i <= numOfTabs; i++)
            tmp += "\t";
        assignment = "";
    }
#line 1329 "y.tab.c"
    break;

  case 21: /* if_statement: if signed_operand comparison_expr sec_signed_operand COLON  */
#line 136 "project.y"
                                                               {
        for (int i = 0; i <= numOfTabs; i++)
            tmp += "\t";
        tmp += "{\n";
        
        tabCheck();
        
        if (numOfTabs >= ifScopeVector.size())
            ifScopeVector.push_back(true);
        else
            ifScopeVector[numOfTabs] = true;
        if (numOfTabs < requiredTabNum) {
            while (ifScopeVector.size() != numOfTabs + 1)
                ifScopeVector.pop_back();
            requiredTabNum = numOfTabs;
        }
        requiredTabNum++;
        flag = true;
        numOfTabs = 0;
        if ((types[0] == 3 || types[1] == 3) && ((types[0] != 3 || types[1] != 3) || (stringSign != "" && types[1] == 3))) {
            cout << "comparison type mismatch in line " << linenum << endl;
            exit(1);
        }
        if (types[0] == 1 && types[1] == 2)
            types[0] = 2;
    }
#line 1360 "y.tab.c"
    break;

  case 22: /* elif_statement: elif signed_operand comparison_expr sec_signed_operand COLON  */
#line 165 "project.y"
                                                                 {
        for (int i = 0; i <= numOfTabs; i++)
            tmp += "\t";
        tmp += "{\n";
        
        tabCheck();

        if (numOfTabs < requiredTabNum) {
            while (ifScopeVector.size() != numOfTabs + 1)
                ifScopeVector.pop_back();
            requiredTabNum = numOfTabs;
        }
        if (numOfTabs >= ifScopeVector.size() || !ifScopeVector[numOfTabs]) {
            cout << "elif after else in line " << linenum << endl;
            exit(1);
        }
        requiredTabNum++;
        flag = true;
        numOfTabs = 0;
        if ((types[0] == 3 || types[1] == 3) && ((types[0] != 3 || types[1] != 3) || (stringSign != "" && types[1] == 3))) {
            cout << "comparison type mismatch in line " << linenum << endl;
            exit(1);
        }
        if (types[0] == 1 && types[1] == 2)
            types[0] = 2;
    }
#line 1391 "y.tab.c"
    break;

  case 23: /* else_statement: else  */
#line 194 "project.y"
         {
        tabCheck();

        if (numOfTabs < requiredTabNum) {
            while (ifScopeVector.size() != numOfTabs + 1)
                ifScopeVector.pop_back();
            requiredTabNum = numOfTabs;
        }
        for (int i = 0; i <= numOfTabs; i++)
            tmp += "\t";
        tmp += "{\n";
        if (numOfTabs >= ifScopeVector.size() || !ifScopeVector[numOfTabs]) {
            cout << "else without if in line " << linenum << endl;
            exit(1);
        }
        ifScopeVector[numOfTabs] = false;
        requiredTabNum++;
        flag = true;
        numOfTabs = 0;
    }
#line 1416 "y.tab.c"
    break;

  case 24: /* $@1: %empty  */
#line 217 "project.y"
                   {
        assignment += stringSign;
    }
#line 1424 "y.tab.c"
    break;

  case 25: /* assign_statements: var EQUAL sign $@1 assign_statement  */
#line 220 "project.y"
                     {
        handleAssignment();
    }
#line 1432 "y.tab.c"
    break;

  case 26: /* assign_statement: operand  */
#line 226 "project.y"
            {
        types[0] = types[1];
    }
#line 1440 "y.tab.c"
    break;

  case 27: /* assign_statement: assign_statement operator operand  */
#line 229 "project.y"
                                        {
        if ((types[0] == 3 || types[1] == 3) && (types[0] != 3 || types[1] != 3 || !isPlus)) {
            cout << "type mismatch in line " << linenum << endl;
            exit(1);
        }
        if (types[0] == 1 && types[1] == 2)
            types[0] = 2;
    }
#line 1453 "y.tab.c"
    break;

  case 28: /* $@2: %empty  */
#line 240 "project.y"
         {
        tmp += stringSign;
    }
#line 1461 "y.tab.c"
    break;

  case 29: /* signed_operand: sign $@2 operand  */
#line 243 "project.y"
            {
        types[0] = types[1];
        tmp += element;
        if (stringSign != "" && types[0] == 3) {
            cout << "type inconsistency in line " << linenum << endl;
            exit(1);
        }
    }
#line 1474 "y.tab.c"
    break;

  case 30: /* $@3: %empty  */
#line 254 "project.y"
         {
        tmp += stringSign;
    }
#line 1482 "y.tab.c"
    break;

  case 31: /* sec_signed_operand: sign $@3 operand  */
#line 257 "project.y"
            {
        tmp += element + ")\n";
    }
#line 1490 "y.tab.c"
    break;

  case 32: /* operand: VAR  */
#line 263 "project.y"
        {
        types[1] = typeMap[(yyvsp[0].str)];
        if (!types[1]) {
            cout << "name '" << (yyvsp[0].str) << "' is not defined in line " << linenum << endl;
            exit(1);
        } else if (types[1] == 1) {
            element = " " + string((yyvsp[0].str)) + "_int";
        } else if (types[1] == 2) {
            element = " " + string((yyvsp[0].str)) + "_flt";
        } else if (types[1] == 3) {
            element = " " + string((yyvsp[0].str)) + "_str";
        }
        assignment += element;
    }
#line 1509 "y.tab.c"
    break;

  case 33: /* operand: INT  */
#line 277 "project.y"
          {
        element = " " + string((yyvsp[0].str));
        primitiveTypeDetect(1, element);
    }
#line 1518 "y.tab.c"
    break;

  case 34: /* operand: FLOAT  */
#line 281 "project.y"
            {
        element = " " + string((yyvsp[0].str));
        primitiveTypeDetect(2, element);
    }
#line 1527 "y.tab.c"
    break;

  case 35: /* operand: STRING  */
#line 285 "project.y"
             {
        element = " " + string((yyvsp[0].str));
        primitiveTypeDetect(3, element);
    }
#line 1536 "y.tab.c"
    break;

  case 36: /* sign: %empty  */
#line 292 "project.y"
    { stringSign = ""; }
#line 1542 "y.tab.c"
    break;

  case 37: /* sign: PLUS  */
#line 293 "project.y"
           { stringSign = " +"; }
#line 1548 "y.tab.c"
    break;

  case 38: /* sign: MINUS  */
#line 294 "project.y"
            { stringSign = " -"; }
#line 1554 "y.tab.c"
    break;

  case 39: /* operator: PLUS  */
#line 298 "project.y"
         { isPlus = true; assignment += " +"; }
#line 1560 "y.tab.c"
    break;

  case 40: /* operator: MINUS  */
#line 299 "project.y"
            { isPlus = false; assignment += " -"; }
#line 1566 "y.tab.c"
    break;

  case 41: /* operator: MULT  */
#line 300 "project.y"
           { isPlus = false; assignment += " *"; }
#line 1572 "y.tab.c"
    break;

  case 42: /* operator: DIV  */
#line 301 "project.y"
          { isPlus = false; assignment += " /"; }
#line 1578 "y.tab.c"
    break;


#line 1582 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 303 "project.y"


void primitiveTypeDetect(int type, string element) {
    assignment += element;
    types[1] = type;
}

void tabCheck() {
    if ((flag && numOfTabs != requiredTabNum) || numOfTabs > requiredTabNum) {
        if (!ifScopeVector.empty()) {
            cout << "error in line " << linenum << ": at least one line should be inside if/elif/else block" << endl;
            exit(1);
        } else {
            cout << "tab inconsistency in line " << linenum << endl;
            exit(1);
        }
    }
}

void handleConditionStatements(string conditionWord) {
    for (int i = numOfTabs + 1; i <= requiredTabNum; i++) {
        for (int j = 0; j < i; j++)
            tmp += "\t";
        tmp += "}\n";
    }
    for (int i = 0; i <= numOfTabs; i++) tmp += "\t";
    if (conditionWord != "else") conditionWord = conditionWord + " (";
    else if (conditionWord == "else") conditionWord += "\n";
    tmp += conditionWord;
}

void handleTypeAssignment(const string& element, const string& suffix, string& vars, int type) {
    string varName = element + suffix;

    bool isThere = false;
    int len = 0;
    for (int i = 0; i < vars.length(); ++i) {
        if (vars[i] == varName[len]) {
            ++len;
            if (len == varName.length() && (i + 1 == vars.length() || vars[i + 1] == ',')) {
                isThere = true;
            }
        } else if (vars[i] == ',') {
            len = 0;
        }
    }

    if (len == varName.length()) {
        isThere = true;
    }

    if (!isThere) {
        if (vars.length())
            vars += ",";
        vars += varName;
    }

    tmp += varName + " =";
    typeMap[element] = type;
}

void handleAssignment() {
    string element(variable);

    tabCheck();

    if (numOfTabs < requiredTabNum) {
        while (ifScopeVector.size() != numOfTabs)
            ifScopeVector.pop_back();
        requiredTabNum = numOfTabs;
    }
    flag = false;
    numOfTabs = 0;

    switch (types[0]) {
        case 1:
            handleTypeAssignment(element, "_int", intVars, 1);
            break;
        case 2:
            handleTypeAssignment(element, "_flt", floatVars, 2);
            break;
        case 3:
            if (stringSign != "") {
                cout << "type inconsistency in line " << linenum << endl;
                exit(1);
            }

            handleTypeAssignment(element, "_str", stringVars, 3);
            break;
    }
    tmp += assignment + ";\n";
}

void appendVariableDeclaration(const string& type, const string& vars) {
    if (vars.length())
        cppFile += "\t" + type + " " + vars + ";\n";
}

void appendTabsAndClosingBrace(int tabs) {
    for (int j = 0; j < tabs; ++j)
        cppFile += "\t";

    cppFile += "}\n";
}

void writeToCpp() {
    appendVariableDeclaration("int", intVars);
    appendVariableDeclaration("float", floatVars);
    appendVariableDeclaration("string", stringVars);

    cppFile += "\n" + tmp;

    for (int i = requiredTabNum; i >= numOfTabs + 1; --i) {
        appendTabsAndClosingBrace(i);
    }
    cppFile += "\n}";
}

void printParseTree(TreeNode* node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; ++i) cout << "  ";
    cout << node->name << endl;
    for (auto child : node->children) {
        printParseTree(child, depth + 1);
    }
}

void printSymbolTable() {
    cout << "Symbol Table:" << endl;
    cout << "| Variable | Type  |" << endl;
    cout << "|----------|-------|" << endl;
    for (const auto& entry : typeMap) {
        cout << "| " << entry.first << " | ";
        switch (entry.second) {
            case INT_TYPE:
                cout << "int";
                break;
            case FLOAT_TYPE:
                cout << "float";
                break;
            case STRING_TYPE:
                cout << "string";
                break;
        }
        cout << " |" << endl;
    }
}

void yyerror(string s) {
    cerr << "Error at line: " << linenum << endl;
    exit(1);
}

int yywrap() {
    return 1;
}

int main(int argc, char *argv[]) {
    /* Call the lexer, then quit. */
    yyin = fopen(argv[1], "r");
    yyparse();
    fclose(yyin);

    writeToCpp();

    cout << cppFile << endl;

    // Print the parse tree
    cout << "Parse Tree:" << endl;
    printParseTree(root, 0);

    // Print the symbol table
    printSymbolTable();

    return 0;
}
