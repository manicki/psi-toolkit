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


#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
// #line 22 "grparser.y"
typedef union YYSTYPE {
    int i;
    std::string* s;
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
    std::list<GRuleTreeRecipe*>* tree_subrecipes_list;
    Lattice::Score score;
} YYSTYPE;
/* Line 1240 of yacc.c.  */
// #line 131 "grparser.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE grparserlval;
