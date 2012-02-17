/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* If NAME_PREFIX is specified substitute the variables and functions
   names.  */
#define yyparse grparserparse
#define yylex   grparserlex
#define yyerror grparsererror
#define yylval  grparserlval
#define yychar  grparserchar
#define yydebug grparserdebug
#define yynerrs grparsernerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     RTOKEN_ERROR = 258,
     RTOKEN_ASSIGN = 259,
     RTOKEN_ATOM = 260,
     RTOKEN_ATTRIBUTE = 261,
     RTOKEN_CARET = 262,
     RTOKEN_COMMA = 263,
     RTOKEN_DOT = 264,
     RTOKEN_LT = 265,
     RTOKEN_GT = 266,
     RTOKEN_EQUAL = 267,
     RTOKEN_LEFT_CURLY = 268,
     RTOKEN_LEFT_PAREN = 269,
     RTOKEN_LEFT_SQUARE = 270,
     RTOKEN_NOT_EQUAL = 271,
     RTOKEN_NUMBER = 272,
     RTOKEN_PIPE = 273,
     RTOKEN_RIGHT_CURLY = 274,
     RTOKEN_RIGHT_LAPKI = 275,
     RTOKEN_RIGHT_PAREN = 276,
     RTOKEN_RIGHT_SQUARE = 277,
     RTOKEN_SEMICOLON = 278,
     RTOKEN_STAR = 279,
     RTOKEN_UASSIGN = 280,
     RTOKEN_UEQUAL = 281,
     RTOKEN_QMARK = 282,
     RTOKEN_COLON = 283,
     RTOKEN_ELSE = 284,
     RTOKEN_IMPLICATION = 285,
     RTOKEN_SLASH = 286,
     RTOKEN_PERCENT = 287,
     RTOKEN_PLUS = 288,
     RTOKEN_AMPERSAND = 289,
     RTOKEN_BINARG = 290,
     RTOKEN_FIRST_NEWLINE = 291,
     RTOKEN_MULTIPLE_NEWLINE = 292,
     RTOKEN_SETSCORE = 293,
     RTOKEN_SCORE = 294
   };
#endif
#define RTOKEN_ERROR 258
#define RTOKEN_ASSIGN 259
#define RTOKEN_ATOM 260
#define RTOKEN_ATTRIBUTE 261
#define RTOKEN_CARET 262
#define RTOKEN_COMMA 263
#define RTOKEN_DOT 264
#define RTOKEN_LT 265
#define RTOKEN_GT 266
#define RTOKEN_EQUAL 267
#define RTOKEN_LEFT_CURLY 268
#define RTOKEN_LEFT_PAREN 269
#define RTOKEN_LEFT_SQUARE 270
#define RTOKEN_NOT_EQUAL 271
#define RTOKEN_NUMBER 272
#define RTOKEN_PIPE 273
#define RTOKEN_RIGHT_CURLY 274
#define RTOKEN_RIGHT_LAPKI 275
#define RTOKEN_RIGHT_PAREN 276
#define RTOKEN_RIGHT_SQUARE 277
#define RTOKEN_SEMICOLON 278
#define RTOKEN_STAR 279
#define RTOKEN_UASSIGN 280
#define RTOKEN_UEQUAL 281
#define RTOKEN_QMARK 282
#define RTOKEN_COLON 283
#define RTOKEN_ELSE 284
#define RTOKEN_IMPLICATION 285
#define RTOKEN_SLASH 286
#define RTOKEN_PERCENT 287
#define RTOKEN_PLUS 288
#define RTOKEN_AMPERSAND 289
#define RTOKEN_BINARG 290
#define RTOKEN_FIRST_NEWLINE 291
#define RTOKEN_MULTIPLE_NEWLINE 292
#define RTOKEN_SETSCORE 293
#define RTOKEN_SCORE 294




/* Copy the first part of user declarations.  */
#line 1 "grparser.y"

#define YYERROR_VERBOSE

#include "grules.h"
#include <stdio.h>

    int grlexlex();
    extern int grlexlineno;

int grparserlex()
{
   return grlexlex();
}



void grparsererror(char* s);

GRuleList* grparser_all = NULL;
char grerror_message[128] = "GRULES UNKNOWN ERROR";


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 22 "grparser.y"
typedef union YYSTYPE {
    int i;
    string* s;
    bool b;
    GRule* grule;
    GRuleList* grule_seq;
    GRuleRightSymbolAltsSeq* right_symbol_alts_seq;
    GRuleRightSymbolsSeq* right_symbols_seq;
    GRuleExpression* grule_expression;
    GRuleTreeRecipePathNode* tree_recipe_path_node;
    GRuleTreeRecipePathNode::ESearchType search_type_spec;
    GRuleTreeRecipePath* tree_recipe_path;
    GRuleTreeRecipe* tree_recipe;
    list<GRuleTreeRecipe*>* tree_subrecipes_list;
    score_type score;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 200 "grparser.bis.cpp"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 212 "grparser.bis.cpp"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   189

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  27
/* YYNRULES -- Number of rules. */
#define YYNRULES  75
/* YYNRULES -- Number of states. */
#define YYNSTATES  127

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    14,    21,    30,    31,
      35,    39,    40,    42,    46,    50,    57,    58,    60,    62,
      64,    67,    69,    73,    75,    78,    80,    86,    91,    93,
      97,    99,   102,   105,   110,   112,   114,   116,   118,   120,
     123,   127,   132,   136,   140,   141,   143,   144,   146,   147,
     150,   154,   158,   162,   168,   172,   176,   180,   184,   188,
     192,   196,   200,   204,   208,   212,   215,   217,   221,   223,
     225,   227,   229,   231,   234,   236
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      41,     0,    -1,    42,    -1,    42,    37,    -1,    43,    -1,
      42,    37,    43,    -1,     5,    12,    58,    44,    45,    63,
      -1,     5,    12,    58,    44,    45,    63,    20,    64,    -1,
      -1,    10,     5,    11,    -1,    10,     6,    11,    -1,    -1,
      46,    -1,    32,    47,    32,    -1,    48,    49,    48,    -1,
      48,    49,    15,    52,    22,    48,    -1,    -1,    33,    -1,
      53,    -1,    50,    -1,    50,    53,    -1,    51,    -1,    51,
      28,    57,    -1,    56,    -1,    24,     5,    -1,     5,    -1,
      24,     5,    13,    17,    19,    -1,     5,    13,    17,    19,
      -1,    47,    -1,    52,     8,    47,    -1,    54,    -1,    53,
      54,    -1,    55,    56,    -1,    55,    56,    28,    57,    -1,
      18,    -1,    31,    -1,     6,    -1,     5,    -1,    59,    -1,
      58,    59,    -1,     5,    61,    62,    -1,    14,    60,    21,
      62,    -1,    58,    18,    58,    -1,    60,    18,    58,    -1,
      -1,    24,    -1,    -1,    27,    -1,    -1,    36,    64,    -1,
      64,    23,    64,    -1,    64,     8,    64,    -1,    64,    26,
      64,    -1,    64,    30,    64,    29,    64,    -1,    64,    30,
      64,    -1,    64,    16,    64,    -1,    64,    34,    64,    -1,
      64,    12,    64,    -1,     6,    25,    64,    -1,     6,     4,
      64,    -1,    64,     7,    64,    -1,    64,    33,    64,    -1,
      64,    35,    64,    -1,    14,    64,    21,    -1,    13,    64,
      19,    -1,    27,    64,    -1,     6,    -1,    66,     9,     6,
      -1,     5,    -1,    17,    -1,    46,    -1,    39,    -1,    17,
      -1,    38,    65,    -1,     5,    -1,     5,    15,    17,    22,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   121,   121,   125,   131,   136,   143,   155,   171,   174,
     175,   179,   182,   188,   194,   201,   212,   215,   221,   229,
     230,   237,   238,   245,   250,   256,   261,   268,   276,   281,
     288,   293,   300,   308,   316,   322,   328,   331,   336,   341,
     348,   355,   362,   368,   375,   376,   378,   379,   384,   385,
     389,   398,   407,   416,   428,   436,   445,   455,   464,   474,
     484,   493,   502,   511,   515,   523,   531,   538,   546,   553,
     560,   566,   571,   576,   584,   589
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "RTOKEN_ERROR", "RTOKEN_ASSIGN", 
  "RTOKEN_ATOM", "RTOKEN_ATTRIBUTE", "RTOKEN_CARET", "RTOKEN_COMMA", 
  "RTOKEN_DOT", "RTOKEN_LT", "RTOKEN_GT", "RTOKEN_EQUAL", 
  "RTOKEN_LEFT_CURLY", "RTOKEN_LEFT_PAREN", "RTOKEN_LEFT_SQUARE", 
  "RTOKEN_NOT_EQUAL", "RTOKEN_NUMBER", "RTOKEN_PIPE", 
  "RTOKEN_RIGHT_CURLY", "RTOKEN_RIGHT_LAPKI", "RTOKEN_RIGHT_PAREN", 
  "RTOKEN_RIGHT_SQUARE", "RTOKEN_SEMICOLON", "RTOKEN_STAR", 
  "RTOKEN_UASSIGN", "RTOKEN_UEQUAL", "RTOKEN_QMARK", "RTOKEN_COLON", 
  "RTOKEN_ELSE", "RTOKEN_IMPLICATION", "RTOKEN_SLASH", "RTOKEN_PERCENT", 
  "RTOKEN_PLUS", "RTOKEN_AMPERSAND", "RTOKEN_BINARG", 
  "RTOKEN_FIRST_NEWLINE", "RTOKEN_MULTIPLE_NEWLINE", "RTOKEN_SETSCORE", 
  "RTOKEN_SCORE", "$accept", "grules_file", "grules_seq", "grule", 
  "rule_id", "grule_recipe", "tree_recipe_expr", "tree_recipe", "p_plus", 
  "tree_recipe_head", "tree_recipe_head_pre_path", 
  "tree_recipe_head_main", "tree_subrecipes_list", "tree_recipe_path", 
  "tree_recipe_path_node", "search_type_specifier", "parse_category", 
  "parse_label", "right_symbols_seq", "right_symbol_alt", 
  "right_symbol_alts_seq", "starred", "quantifier", "condition", "expr", 
  "score_number", "symbol_ref", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    40,    41,    41,    42,    42,    43,    43,    44,    44,
      44,    45,    45,    46,    47,    47,    48,    48,    49,    49,
      49,    50,    50,    51,    51,    51,    51,    51,    52,    52,
      53,    53,    54,    54,    55,    55,    56,    57,    58,    58,
      59,    59,    60,    60,    61,    61,    62,    62,    63,    63,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    65,    65,    64,    66,    66
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     2,     1,     3,     6,     8,     0,     3,
       3,     0,     1,     3,     3,     6,     0,     1,     1,     1,
       2,     1,     3,     1,     2,     1,     5,     4,     1,     3,
       1,     2,     2,     4,     1,     1,     1,     1,     1,     2,
       3,     4,     3,     3,     0,     1,     0,     1,     0,     2,
       3,     3,     3,     5,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     1,     3,     1,     1,
       1,     1,     1,     2,     1,     4
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     0,     0,     2,     4,     0,     1,     3,    44,     0,
       8,    38,     5,    45,    46,     0,     0,     0,    11,    39,
      47,    40,     0,     0,    46,     0,     0,    16,    48,    12,
      42,    43,    41,     9,    10,    17,     0,     0,     0,     6,
      13,    25,    36,    34,     0,    35,    16,    19,    21,    18,
      30,     0,    23,    68,    66,     0,     0,    69,     0,     0,
      70,    49,     0,     0,     0,    24,    16,    14,    20,     0,
      31,    32,     0,     0,     0,     0,     0,    65,    72,    71,
      73,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     7,     0,     0,    28,     0,    37,    22,     0,
       0,    59,    58,    64,    63,    60,    51,    57,    55,    50,
      52,    54,    61,    56,    62,    67,    27,     0,    16,    16,
      33,    75,     0,    26,    29,    15,    53
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     2,     3,     4,    18,    28,    60,    36,    37,    46,
      47,    48,    96,    49,    50,    51,    52,    98,    10,    11,
      16,    14,    21,    39,    61,    80,    62
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -64
static const short yypact[] =
{
       2,     0,    14,   -14,   -64,     3,   -64,     2,    37,     3,
       1,   -64,   -64,   -64,    44,   149,    42,   105,    60,   -64,
     -64,   -64,     3,     3,    44,    88,   116,    61,    92,   -64,
       3,     3,   -64,   -64,   -64,   -64,   100,    83,    32,   117,
     -64,   125,   -64,   -64,   137,   -64,    41,   103,   120,   103,
     -64,   146,   -64,   132,    62,    32,    32,   -64,    32,    73,
     -64,   110,   144,    32,   145,   147,    61,   -64,   103,   159,
     -64,   140,   148,    32,    32,    46,    70,   110,   -64,   -64,
     -64,    32,    32,    32,    32,    32,    32,    32,    32,    32,
      32,   165,   110,   153,   156,   -64,   107,   -64,   -64,   159,
     152,    17,    17,   -64,   -64,   150,   143,     9,     6,   123,
     154,    90,    40,    50,   110,   -64,   -64,   160,    61,    61,
     -64,   -64,    32,   -64,   -64,   -64,   123
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
     -64,   -64,   -64,   168,   -64,   -64,   163,   -63,   -46,   -64,
     -64,   -64,   -64,   135,    -1,   -64,   133,    84,    86,    -5,
     -64,   -64,   162,   -64,   -54,   -64,   -64
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -75
static const yysigned_char yytable[] =
{
      67,    75,    76,    95,    77,    19,     8,     1,     8,    92,
      19,    17,     5,    81,     6,     9,    81,     9,    83,   101,
     102,   -75,   -75,     7,    81,    19,    19,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    53,    54,    88,
      89,    90,    88,    89,    90,    55,    56,    81,    70,    57,
      88,    89,    90,    81,    82,   124,    66,    81,    83,    58,
      23,    13,    84,    24,    27,   103,    73,    70,   126,    85,
      59,    20,    86,   125,    35,    90,    87,    81,    82,    88,
      89,    90,    83,    88,   -75,    90,    84,    74,    41,    42,
      78,   104,    27,    85,    35,    15,    86,    81,    82,    33,
      87,    43,    83,    88,    89,    90,    84,    44,    30,    31,
      25,    26,    79,    85,    45,   118,    86,    81,    82,   122,
     -75,    43,    83,    88,    89,    90,    84,    34,    38,   119,
      81,    82,    40,    85,    45,    83,    86,    63,    64,    84,
      87,   -74,    65,    88,    89,    90,    85,    72,    69,    86,
      81,    82,    42,    91,     8,    83,    88,    89,    90,    84,
      94,    81,    93,     9,    97,   100,    83,    22,    99,    86,
      84,   115,   116,   117,   121,    12,    88,    89,    90,   123,
     -75,    29,    68,   120,    71,    90,    32,    88,    89,    90
};

static const unsigned char yycheck[] =
{
      46,    55,    56,    66,    58,    10,     5,     5,     5,    63,
      15,    10,    12,     7,     0,    14,     7,    14,    12,    73,
      74,    12,    16,    37,     7,    30,    31,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,     5,     6,    33,
      34,    35,    33,    34,    35,    13,    14,     7,    49,    17,
      33,    34,    35,     7,     8,   118,    15,     7,    12,    27,
      18,    24,    16,    21,    32,    19,     4,    68,   122,    23,
      38,    27,    26,   119,    33,    35,    30,     7,     8,    33,
      34,    35,    12,    33,    34,    35,    16,    25,     5,     6,
      17,    21,    32,    23,    33,     9,    26,     7,     8,    11,
      30,    18,    12,    33,    34,    35,    16,    24,    22,    23,
       5,     6,    39,    23,    31,     8,    26,     7,     8,    29,
      30,    18,    12,    33,    34,    35,    16,    11,    36,    22,
       7,     8,    32,    23,    31,    12,    26,    20,    13,    16,
      30,     9,     5,    33,    34,    35,    23,    15,    28,    26,
       7,     8,     6,     9,     5,    12,    33,    34,    35,    16,
      13,     7,    17,    14,     5,    17,    12,    18,    28,    26,
      16,     6,    19,    17,    22,     7,    33,    34,    35,    19,
      26,    18,    47,    99,    51,    35,    24,    33,    34,    35
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     5,    41,    42,    43,    12,     0,    37,     5,    14,
      58,    59,    43,    24,    61,    58,    60,    10,    44,    59,
      27,    62,    18,    18,    21,     5,     6,    32,    45,    46,
      58,    58,    62,    11,    11,    33,    47,    48,    36,    63,
      32,     5,     6,    18,    24,    31,    49,    50,    51,    53,
      54,    55,    56,     5,     6,    13,    14,    17,    27,    38,
      46,    64,    66,    20,    13,     5,    15,    48,    53,    28,
      54,    56,    15,     4,    25,    64,    64,    64,    17,    39,
      65,     7,     8,    12,    16,    23,    26,    30,    33,    34,
      35,     9,    64,    17,    13,    47,    52,     5,    57,    28,
      17,    64,    64,    19,    21,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,     6,    19,    17,     8,    22,
      57,    22,    29,    19,    47,    48,    64
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 122 "grparser.y"
    {
    grparser_all = yyvsp[0].grule_seq;
;}
    break;

  case 3:
#line 126 "grparser.y"
    {
    grparser_all = yyvsp[-1].grule_seq;
;}
    break;

  case 4:
#line 132 "grparser.y"
    {
    yyval.grule_seq = new GRuleList;
    yyval.grule_seq->push_back(yyvsp[0].grule);
;}
    break;

  case 5:
#line 137 "grparser.y"
    {
    yyval.grule_seq->push_back(yyvsp[0].grule);
;}
    break;

  case 6:
#line 144 "grparser.y"
    { 
    yyval.grule = new GRule;
    yyval.grule->left_symbol = yyvsp[-5].s;
    yyval.grule->right_symbols = yyvsp[-3].right_symbols_seq;
    yyval.grule->recipe = yyvsp[-1].tree_recipe;
    yyval.grule->expr = yyvsp[0].grule_expression;

#ifdef WITH_ACCOUNTING
    yyval.grule->rule_id = yyvsp[-2].s;
#endif
;}
    break;

  case 7:
#line 156 "grparser.y"
    { 
    yyval.grule = new GRule;
    yyval.grule->left_symbol = yyvsp[-7].s;
    yyval.grule->right_symbols = yyvsp[-5].right_symbols_seq;
    yyval.grule->recipe = yyvsp[-3].tree_recipe;
    yyval.grule->expr = yyvsp[-2].grule_expression;
    yyval.grule->extra_expr = yyvsp[0].grule_expression;

#ifdef WITH_ACCOUNTING
    yyval.grule->rule_id = yyvsp[-4].s;
#endif
;}
    break;

  case 8:
#line 171 "grparser.y"
    {
    yyval.s = NULL;
;}
    break;

  case 9:
#line 174 "grparser.y"
    { yyval.s = yyvsp[-1].s; ;}
    break;

  case 10:
#line 175 "grparser.y"
    { yyval.s = yyvsp[-1].s; ;}
    break;

  case 11:
#line 179 "grparser.y"
    {
    yyval.tree_recipe = NULL;
;}
    break;

  case 13:
#line 189 "grparser.y"
    {
    yyval.tree_recipe = yyvsp[-1].tree_recipe;
;}
    break;

  case 14:
#line 195 "grparser.y"
    {
    yyval.tree_recipe = yyvsp[-1].tree_recipe;
    if(yyvsp[-2].b) yyval.tree_recipe->how_to_insert = GRuleTreeRecipe::INSERT_AFTER;
    if(yyvsp[0].b) yyval.tree_recipe->how_to_insert = GRuleTreeRecipe::INSERT_BEFORE;
    PE_ASSERT(!yyvsp[-2].b || !yyvsp[0].b);
;}
    break;

  case 15:
#line 202 "grparser.y"
    {
    yyval.tree_recipe = yyvsp[-4].tree_recipe;
    yyval.tree_recipe->subrecipes = yyvsp[-2].tree_subrecipes_list;
    if(yyvsp[-5].b) yyval.tree_recipe->how_to_insert = GRuleTreeRecipe::INSERT_AFTER;
    if(yyvsp[0].b) yyval.tree_recipe->how_to_insert = GRuleTreeRecipe::INSERT_BEFORE;
    PE_ASSERT(!yyvsp[-5].b || !yyvsp[0].b);
;}
    break;

  case 16:
#line 212 "grparser.y"
    {
    yyval.b = false;
;}
    break;

  case 17:
#line 216 "grparser.y"
    {
    yyval.b = true;
;}
    break;

  case 18:
#line 222 "grparser.y"
    {
    yyval.tree_recipe = new GRuleTreeRecipe;
    yyval.tree_recipe->path = yyvsp[0].tree_recipe_path;
    yyval.tree_recipe->how_to_insert = GRuleTreeRecipe::ONLY_PUT;
;}
    break;

  case 20:
#line 231 "grparser.y"
    {
    yyval.tree_recipe = yyvsp[-1].tree_recipe;
    yyval.tree_recipe->path = yyvsp[0].tree_recipe_path;
;}
    break;

  case 22:
#line 239 "grparser.y"
    {
    yyval.tree_recipe = yyvsp[-2].tree_recipe;
    yyval.tree_recipe->label = yyvsp[0].s;
;}
    break;

  case 23:
#line 246 "grparser.y"
    {
    yyval.tree_recipe = new GRuleTreeRecipe;
    yyval.tree_recipe->category = yyvsp[0].s;
;}
    break;

  case 24:
#line 251 "grparser.y"
    {
    yyval.tree_recipe = new GRuleTreeRecipe;
    yyval.tree_recipe->symbol = yyvsp[0].s;
    yyval.tree_recipe->is_contents_ref = true;
;}
    break;

  case 25:
#line 257 "grparser.y"
    {
    yyval.tree_recipe = new GRuleTreeRecipe;
    yyval.tree_recipe->symbol = yyvsp[0].s;
;}
    break;

  case 26:
#line 262 "grparser.y"
    {
    yyval.tree_recipe = new GRuleTreeRecipe;
    yyval.tree_recipe->symbol = yyvsp[-3].s;
    yyval.tree_recipe->number = yyvsp[-1].i;
    yyval.tree_recipe->is_contents_ref = true;
;}
    break;

  case 27:
#line 269 "grparser.y"
    {
    yyval.tree_recipe = new GRuleTreeRecipe;
    yyval.tree_recipe->symbol = yyvsp[-3].s;
    yyval.tree_recipe->number = yyvsp[-1].i;
;}
    break;

  case 28:
#line 277 "grparser.y"
    {
    yyval.tree_subrecipes_list = new list<GRuleTreeRecipe*>;
    yyval.tree_subrecipes_list->push_back(yyvsp[0].tree_recipe);
;}
    break;

  case 29:
#line 282 "grparser.y"
    {
    yyval.tree_subrecipes_list = yyvsp[-2].tree_subrecipes_list;
    yyval.tree_subrecipes_list->push_back(yyvsp[0].tree_recipe);
;}
    break;

  case 30:
#line 289 "grparser.y"
    {
    yyval.tree_recipe_path = new GRuleTreeRecipePath;
    yyval.tree_recipe_path->push_back(yyvsp[0].tree_recipe_path_node);
;}
    break;

  case 31:
#line 294 "grparser.y"
    {
    yyval.tree_recipe_path = yyvsp[-1].tree_recipe_path;
    yyval.tree_recipe_path->push_back(yyvsp[0].tree_recipe_path_node);
;}
    break;

  case 32:
#line 301 "grparser.y"
    {
    yyval.tree_recipe_path_node = new GRuleTreeRecipePathNode;
    yyval.tree_recipe_path_node->search_type = yyvsp[-1].search_type_spec;
    yyval.tree_recipe_path_node->category = yyvsp[0].s;
;}
    break;

  case 33:
#line 309 "grparser.y"
    {
    yyval.tree_recipe_path_node = new GRuleTreeRecipePathNode;
    yyval.tree_recipe_path_node->search_type = yyvsp[-3].search_type_spec;
    yyval.tree_recipe_path_node->category = yyvsp[-2].s;
    yyval.tree_recipe_path_node->label = yyvsp[0].s;
;}
    break;

  case 34:
#line 317 "grparser.y"
    {
    yyval.search_type_spec = GRuleTreeRecipePathNode::SEARCH_AND_CREATE;
;}
    break;

  case 35:
#line 323 "grparser.y"
    {
    yyval.search_type_spec = GRuleTreeRecipePathNode::ONLY_SEARCH;
;}
    break;

  case 38:
#line 337 "grparser.y"
    {
    yyval.right_symbols_seq = new GRuleRightSymbolsSeq;
    yyval.right_symbols_seq->push_back(yyvsp[0].right_symbol_alts_seq);
;}
    break;

  case 39:
#line 342 "grparser.y"
    {
    yyval.right_symbols_seq = yyvsp[-1].right_symbols_seq;
    yyval.right_symbols_seq->push_back(yyvsp[0].right_symbol_alts_seq);
;}
    break;

  case 40:
#line 349 "grparser.y"
    {
    yyval.right_symbol_alts_seq = new GRuleRightSymbolAltsSeq;
    yyval.right_symbol_alts_seq->atom = yyvsp[-2].s;
    yyval.right_symbol_alts_seq->is_starred = static_cast<bool>(yyvsp[-1].i);
    yyval.right_symbol_alts_seq->is_optional = static_cast<bool>(yyvsp[0].i);
;}
    break;

  case 41:
#line 356 "grparser.y"
    { 
    yyval.right_symbol_alts_seq = yyvsp[-2].right_symbol_alts_seq;
    yyval.right_symbol_alts_seq->is_optional = static_cast<bool>(yyvsp[0].i);
;}
    break;

  case 42:
#line 363 "grparser.y"
    {
    yyval.right_symbol_alts_seq = new GRuleRightSymbolAltsSeq;
    yyval.right_symbol_alts_seq->alts.push_back(yyvsp[-2].right_symbols_seq);
    yyval.right_symbol_alts_seq->alts.push_back(yyvsp[0].right_symbols_seq);
;}
    break;

  case 43:
#line 369 "grparser.y"
    {
    yyval.right_symbol_alts_seq = yyvsp[-2].right_symbol_alts_seq;
    yyval.right_symbol_alts_seq->alts.push_back(yyvsp[0].right_symbols_seq);
;}
    break;

  case 44:
#line 375 "grparser.y"
    { yyval.i = 0;;}
    break;

  case 45:
#line 376 "grparser.y"
    { yyval.i = 1;;}
    break;

  case 46:
#line 378 "grparser.y"
    { yyval.i = 0; ;}
    break;

  case 47:
#line 379 "grparser.y"
    { yyval.i = 1;;}
    break;

  case 48:
#line 384 "grparser.y"
    { yyval.grule_expression = NULL; ;}
    break;

  case 49:
#line 386 "grparser.y"
    { yyval.grule_expression = yyvsp[0].grule_expression;;}
    break;

  case 50:
#line 390 "grparser.y"
    { 
    yyval.grule_expression = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    yyval.grule_expression->expr_operator = GRuleExpression::OPERATOR_OR;
    yyval.grule_expression->left_subexpr = yyvsp[-2].grule_expression;
    yyval.grule_expression->right_subexpr = yyvsp[0].grule_expression;
;}
    break;

  case 51:
#line 399 "grparser.y"
    { 
    yyval.grule_expression = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    yyval.grule_expression->expr_operator = GRuleExpression::OPERATOR_AND;
    yyval.grule_expression->left_subexpr = yyvsp[-2].grule_expression;
    yyval.grule_expression->right_subexpr = yyvsp[0].grule_expression;
;}
    break;

  case 52:
#line 408 "grparser.y"
    { 
    yyval.grule_expression = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    yyval.grule_expression->expr_operator = GRuleExpression::OPERATOR_UEQUAL;
    yyval.grule_expression->left_subexpr = yyvsp[-2].grule_expression;
    yyval.grule_expression->right_subexpr = yyvsp[0].grule_expression;
;}
    break;

  case 53:
#line 417 "grparser.y"
    {
    yyval.grule_expression = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    yyval.grule_expression->expr_operator = GRuleExpression::OPERATOR_IMPLICATION;
    yyval.grule_expression->left_subexpr = yyvsp[-4].grule_expression;
    yyval.grule_expression->right_subexpr = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    yyval.grule_expression->right_subexpr->expr_operator = GRuleExpression::OPERATOR_ELSE;
    yyval.grule_expression->right_subexpr->left_subexpr = yyvsp[-2].grule_expression;
    yyval.grule_expression->right_subexpr->right_subexpr = yyvsp[0].grule_expression;
;}
    break;

  case 54:
#line 429 "grparser.y"
    {
    yyval.grule_expression = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    yyval.grule_expression->expr_operator = GRuleExpression::OPERATOR_IMPLICATION;
    yyval.grule_expression->left_subexpr = yyvsp[-2].grule_expression;
    yyval.grule_expression->right_subexpr = yyvsp[0].grule_expression;
;}
    break;

  case 55:
#line 437 "grparser.y"
    { 
    yyval.grule_expression = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    yyval.grule_expression->expr_operator = GRuleExpression::OPERATOR_NOT_EQUAL;
    yyval.grule_expression->left_subexpr = yyvsp[-2].grule_expression;
    yyval.grule_expression->right_subexpr = yyvsp[0].grule_expression;
;}
    break;

  case 56:
#line 446 "grparser.y"
    { 
    yyval.grule_expression = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    yyval.grule_expression->expr_operator = GRuleExpression::OPERATOR_HOOKING;
    yyval.grule_expression->left_subexpr = yyvsp[-2].grule_expression;
    yyval.grule_expression->right_subexpr = yyvsp[0].grule_expression;
;}
    break;

  case 57:
#line 456 "grparser.y"
    { 
    yyval.grule_expression = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    yyval.grule_expression->expr_operator = GRuleExpression::OPERATOR_EQUAL;
    yyval.grule_expression->left_subexpr = yyvsp[-2].grule_expression;
    yyval.grule_expression->right_subexpr = yyvsp[0].grule_expression;
;}
    break;

  case 58:
#line 465 "grparser.y"
    { 
    yyval.grule_expression = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    yyval.grule_expression->expr_operator = GRuleExpression::OPERATOR_UASSIGN;
    yyval.grule_expression->left_subexpr = new GRuleExpression(GRuleExpression::ATTRIBUTE);
    yyval.grule_expression->left_subexpr->attribute = yyvsp[-2].s;
    yyval.grule_expression->right_subexpr = yyvsp[0].grule_expression;
;}
    break;

  case 59:
#line 475 "grparser.y"
    { 
    yyval.grule_expression = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    yyval.grule_expression->expr_operator = GRuleExpression::OPERATOR_ASSIGN;
    yyval.grule_expression->left_subexpr = new GRuleExpression(GRuleExpression::ATTRIBUTE);
    yyval.grule_expression->left_subexpr->attribute = yyvsp[-2].s;
    yyval.grule_expression->right_subexpr = yyvsp[0].grule_expression;
;}
    break;

  case 60:
#line 485 "grparser.y"
    { 
    yyval.grule_expression = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    yyval.grule_expression->expr_operator = GRuleExpression::OPERATOR_SEMANTICS_INTERSECTION;
    yyval.grule_expression->left_subexpr = yyvsp[-2].grule_expression;
    yyval.grule_expression->right_subexpr = yyvsp[0].grule_expression;
;}
    break;

  case 61:
#line 494 "grparser.y"
    { 
    yyval.grule_expression = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    yyval.grule_expression->expr_operator = GRuleExpression::OPERATOR_SUM;
    yyval.grule_expression->left_subexpr = yyvsp[-2].grule_expression;
    yyval.grule_expression->right_subexpr = yyvsp[0].grule_expression;
;}
    break;

  case 62:
#line 503 "grparser.y"
    { 
    yyval.grule_expression = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    yyval.grule_expression->expr_operator = GRuleExpression::OPERATOR_BINARG;
    yyval.grule_expression->left_subexpr = yyvsp[-2].grule_expression;
    yyval.grule_expression->right_subexpr = yyvsp[0].grule_expression;
;}
    break;

  case 63:
#line 512 "grparser.y"
    { yyval.grule_expression = yyvsp[-1].grule_expression;;}
    break;

  case 64:
#line 516 "grparser.y"
    { 
    yyval.grule_expression = new GRuleExpression(GRuleExpression::OPERATOR_MONO);
    yyval.grule_expression->expr_operator = GRuleExpression::OPERATOR_ALWAYS_TRUE;
    yyval.grule_expression->left_subexpr = yyvsp[-1].grule_expression;
;}
    break;

  case 65:
#line 524 "grparser.y"
    { 
    yyval.grule_expression = new GRuleExpression(GRuleExpression::OPERATOR_MONO);
    yyval.grule_expression->expr_operator = GRuleExpression::OPERATOR_PRINT;
    yyval.grule_expression->left_subexpr = yyvsp[0].grule_expression;
;}
    break;

  case 66:
#line 532 "grparser.y"
    { 
    yyval.grule_expression = new GRuleExpression(GRuleExpression::ATTRIBUTE);
    yyval.grule_expression->attribute = yyvsp[0].s;
;}
    break;

  case 67:
#line 539 "grparser.y"
    { 
    yyval.grule_expression = yyvsp[-2].grule_expression;
    yyval.grule_expression->attribute = yyvsp[0].s;
;}
    break;

  case 68:
#line 547 "grparser.y"
    {
    yyval.grule_expression = new GRuleExpression(GRuleExpression::ATOM);
    yyval.grule_expression->atom = yyvsp[0].s;
;}
    break;

  case 69:
#line 554 "grparser.y"
    {
    yyval.grule_expression = new GRuleExpression(GRuleExpression::NUMBER);
    yyval.grule_expression->number = yyvsp[0].i;  
;}
    break;

  case 70:
#line 561 "grparser.y"
    {
    yyval.grule_expression = new GRuleExpression(GRuleExpression::TREE_CHOICE);
    yyval.grule_expression->tree_choice = yyvsp[0].tree_recipe;
;}
    break;

  case 71:
#line 567 "grparser.y"
    {
    yyval.score = yyvsp[0].score;
;}
    break;

  case 72:
#line 572 "grparser.y"
    {
    yyval.score = static_cast<score_type>(yyvsp[0].i);
;}
    break;

  case 73:
#line 577 "grparser.y"
    {
    yyval.grule_expression = new GRuleExpression(GRuleExpression::SETSCORE);
    yyval.grule_expression->score = yyvsp[0].score;
;}
    break;

  case 74:
#line 585 "grparser.y"
    {
    yyval.grule_expression = new GRuleExpression(GRuleExpression::ATTRIBUTE);
    yyval.grule_expression->atom = yyvsp[0].s;
;}
    break;

  case 75:
#line 590 "grparser.y"
    {
    yyval.grule_expression = new GRuleExpression(GRuleExpression::ATTRIBUTE);
    yyval.grule_expression->atom = yyvsp[-3].s;
    yyval.grule_expression->number = yyvsp[-1].i;
;}
    break;


    }

/* Line 999 of yacc.c.  */
#line 1803 "grparser.bis.cpp"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 121 "grparser.y"

void grparsererror(char* s)
    {
	printf("b³±d: %s %d\n", s, grlexlineno);
	sprintf(grerror_message,"%d::%s",grlexlineno,s);
   }


