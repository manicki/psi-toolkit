%{
#define YYERROR_VERBOSE

#include "grules.h"
#include <stdio.h>

    int grlexlex();
    extern int grlexlineno;

int grparserlex()
{
   return grlexlex();
}



void grparsererror(const char* s);

GRuleList* grparser_all = NULL;
char grerror_message[128] = "GRULES UNKNOWN ERROR";
%}

%union{
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
    Lattice::Score score;
}

%token <s> RTOKEN_ERROR
%token <s> RTOKEN_ASSIGN
%token <s> RTOKEN_ATOM
%token <s> RTOKEN_ATTRIBUTE
%token <s> RTOKEN_CARET
%token <s> RTOKEN_COMMA
%token <s> RTOKEN_DOT
%token <s> RTOKEN_LT
%token <s> RTOKEN_GT
%token <s> RTOKEN_EQUAL
%token <s> RTOKEN_LEFT_CURLY
%token <s> RTOKEN_LEFT_PAREN
%token <s> RTOKEN_LEFT_SQUARE
%token <s> RTOKEN_NOT_EQUAL
%token <i> RTOKEN_NUMBER
%token <s> RTOKEN_PIPE
%token <s> RTOKEN_RIGHT_CURLY
%token <s> RTOKEN_RIGHT_LAPKI
%token <s> RTOKEN_RIGHT_PAREN
%token <s> RTOKEN_RIGHT_SQUARE
%token <s> RTOKEN_SEMICOLON
%token <s> RTOKEN_STAR
%token <s> RTOKEN_UASSIGN
%token <s> RTOKEN_UEQUAL
%token <s> RTOKEN_QMARK
%token <s> RTOKEN_COLON
%token <s> RTOKEN_ELSE
%token <s> RTOKEN_IMPLICATION
%token <s> RTOKEN_SLASH
%token <s> RTOKEN_PERCENT
%token <s> RTOKEN_PLUS
%token <s> RTOKEN_AMPERSAND
%token <s> RTOKEN_BINARG

%token <s> RTOKEN_FIRST_NEWLINE
%token <s> RTOKEN_MULTIPLE_NEWLINE

%token <i> RTOKEN_SETSCORE
%token <score> RTOKEN_SCORE

%nonassoc RTOKEN_IMPLICATION
%nonassoc RTOKEN_ELSE
%right RTOKEN_SEMICOLON
%right RTOKEN_COMMA
%nonassoc RTOKEN_UEQUAL
%nonassoc RTOKEN_NOT_EQUAL
%nonassoc RTOKEN_EQUAL
%nonassoc RTOKEN_UASSIGN
%nonassoc RTOKEN_ASSIGN
%nonassoc RTOKEN_AMPERSAND
%left RTOKEN_PLUS
%left RTOKEN_CARET

%type <grule_seq> grules_file
%type <grule_seq> grules_seq
%type <grule> grule
%type <grule_expression> condition
%type <right_symbols_seq> right_symbols_seq
%type <right_symbol_alts_seq> right_symbol_alts_seq
%type <right_symbol_alts_seq> right_symbol_alt
%type <i> quantifier
%type <i> starred
%type <grule_expression> expr
%type <grule_expression> symbol_ref
%type <s> parse_category
%type <s> parse_label
%type <tree_recipe_path_node> tree_recipe_path_node
%type <tree_recipe_path> tree_recipe_path
%type <tree_subrecipes_list> tree_subrecipes_list
%type <tree_recipe> tree_recipe
%type <tree_recipe> tree_recipe_head
%type <tree_recipe> tree_recipe_head_main
%type <tree_recipe> tree_recipe_head_pre_path
%type <tree_recipe> tree_recipe_expr
%type <tree_recipe> grule_recipe
%type <b> p_plus
%type <score> score_number
%type <s> rule_id

%type <search_type_spec> search_type_specifier
%%

grules_file: grules_seq
{
    grparser_all = $1;
}
   | grules_seq RTOKEN_MULTIPLE_NEWLINE
{
    grparser_all = $1;
}
;

grules_seq: grule
{
    $$ = new GRuleList;
    $$->push_back($1);
}
   | grules_seq RTOKEN_MULTIPLE_NEWLINE grule
{
    $$->push_back($3);
}
;


grule: RTOKEN_ATOM  RTOKEN_EQUAL right_symbols_seq rule_id grule_recipe condition
{
    $$ = new GRule;
    $$->left_symbol = $1;
    $$->right_symbols = $3;
    $$->recipe = $5;
    $$->expr = $6;

#ifdef WITH_ACCOUNTING
    $$->rule_id = $4;
#endif
}
| RTOKEN_ATOM  RTOKEN_EQUAL right_symbols_seq rule_id grule_recipe condition RTOKEN_RIGHT_LAPKI expr
{
    $$ = new GRule;
    $$->left_symbol = $1;
    $$->right_symbols = $3;
    $$->recipe = $5;
    $$->expr = $6;
    $$->extra_expr = $8;

#ifdef WITH_ACCOUNTING
    $$->rule_id = $4;
#endif
}
;

rule_id: /* empty */
{
    $$ = NULL;
}
| RTOKEN_LT RTOKEN_ATOM RTOKEN_GT { $$ = $2; }
| RTOKEN_LT RTOKEN_ATTRIBUTE RTOKEN_GT { $$ = $2; }
;

grule_recipe: /* empty */
{
    $$ = NULL;
}
| tree_recipe_expr
;


/* ===== */

tree_recipe_expr: RTOKEN_PERCENT tree_recipe RTOKEN_PERCENT
{
    $$ = $2;
}
;

tree_recipe: p_plus tree_recipe_head p_plus
{
    $$ = $2;
    if($1) $$->how_to_insert = GRuleTreeRecipe::INSERT_AFTER;
    if($3) $$->how_to_insert = GRuleTreeRecipe::INSERT_BEFORE;
}
| p_plus tree_recipe_head RTOKEN_LEFT_SQUARE tree_subrecipes_list RTOKEN_RIGHT_SQUARE p_plus
{
    $$ = $2;
    $$->subrecipes = $4;
    if($1) $$->how_to_insert = GRuleTreeRecipe::INSERT_AFTER;
    if($6) $$->how_to_insert = GRuleTreeRecipe::INSERT_BEFORE;
}
;

p_plus: /* EMPTY */
{
    $$ = false;
}
| RTOKEN_PLUS
{
    $$ = true;
}
;

tree_recipe_head: tree_recipe_path
{
    $$ = new GRuleTreeRecipe;
    $$->path = $1;
    $$->how_to_insert = GRuleTreeRecipe::ONLY_PUT;
}
;

tree_recipe_head: tree_recipe_head_pre_path
| tree_recipe_head_pre_path tree_recipe_path
{
    $$ = $1;
    $$->path = $2;
}
;

tree_recipe_head_pre_path: tree_recipe_head_main
| tree_recipe_head_main RTOKEN_COLON parse_label
{
    $$ = $1;
    $$->label = $3;
}
;

tree_recipe_head_main: parse_category
{
    $$ = new GRuleTreeRecipe;
    $$->category = $1;
}
| RTOKEN_STAR RTOKEN_ATOM
{
    $$ = new GRuleTreeRecipe;
    $$->symbol = $2;
    $$->is_contents_ref = true;
}
| RTOKEN_ATOM
{
    $$ = new GRuleTreeRecipe;
    $$->symbol = $1;
}
| RTOKEN_STAR RTOKEN_ATOM RTOKEN_LEFT_CURLY RTOKEN_NUMBER RTOKEN_RIGHT_CURLY
{
    $$ = new GRuleTreeRecipe;
    $$->symbol = $2;
    $$->number = $4;
    $$->is_contents_ref = true;
}
| RTOKEN_ATOM RTOKEN_LEFT_CURLY RTOKEN_NUMBER RTOKEN_RIGHT_CURLY
{
    $$ = new GRuleTreeRecipe;
    $$->symbol = $1;
    $$->number = $3;
}
;

tree_subrecipes_list: tree_recipe
{
    $$ = new list<GRuleTreeRecipe*>;
    $$->push_back($1);
}
| tree_subrecipes_list RTOKEN_COMMA tree_recipe
{
    $$ = $1;
    $$->push_back($3);
}
;

tree_recipe_path: tree_recipe_path_node
{
    $$ = new GRuleTreeRecipePath;
    $$->push_back($1);
}
| tree_recipe_path tree_recipe_path_node
{
    $$ = $1;
    $$->push_back($2);
}
;

tree_recipe_path_node: search_type_specifier parse_category
{
    $$ = new GRuleTreeRecipePathNode;
    $$->search_type = $1;
    $$->category = $2;
}
;

tree_recipe_path_node: search_type_specifier parse_category RTOKEN_COLON parse_label
{
    $$ = new GRuleTreeRecipePathNode;
    $$->search_type = $1;
    $$->category = $2;
    $$->label = $4;
}

search_type_specifier: RTOKEN_PIPE
{
    $$ = GRuleTreeRecipePathNode::SEARCH_AND_CREATE;
}
;

search_type_specifier: RTOKEN_SLASH
{
    $$ = GRuleTreeRecipePathNode::ONLY_SEARCH;
}
;

parse_category: RTOKEN_ATTRIBUTE
;

parse_label: RTOKEN_ATOM
;

/* ===== */

right_symbols_seq: right_symbol_alt
{
    $$ = new GRuleRightSymbolsSeq;
    $$->push_back($1);
}
| right_symbols_seq  right_symbol_alt
{
    $$ = $1;
    $$->push_back($2);
}
;

right_symbol_alt: RTOKEN_ATOM starred quantifier
{
    $$ = new GRuleRightSymbolAltsSeq;
    $$->atom = $1;
    $$->is_starred = static_cast<bool>($2);
    $$->is_optional = static_cast<bool>($3);
}
  |  RTOKEN_LEFT_PAREN  right_symbol_alts_seq  RTOKEN_RIGHT_PAREN quantifier
{
    $$ = $2;
    $$->is_optional = static_cast<bool>($4);
}
;

right_symbol_alts_seq: right_symbols_seq  RTOKEN_PIPE  right_symbols_seq
{
    $$ = new GRuleRightSymbolAltsSeq;
    $$->alts.push_back($1);
    $$->alts.push_back($3);
}
 |  right_symbol_alts_seq  RTOKEN_PIPE  right_symbols_seq
{
    $$ = $1;
    $$->alts.push_back($3);
}
;

starred: /* empty */ { $$ = 0;}
| RTOKEN_STAR { $$ = 1;}

quantifier: /* empty */ { $$ = 0; }
| RTOKEN_QMARK { $$ = 1;}
 ;

/* ===== */

condition: /* empty */ { $$ = NULL; }
| RTOKEN_FIRST_NEWLINE expr
{ $$ = $2;}
;

expr: expr  RTOKEN_SEMICOLON  expr
{
    $$ = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    $$->expr_operator = GRuleExpression::OPERATOR_OR;
    $$->left_subexpr = $1;
    $$->right_subexpr = $3;
}
;

expr: expr  RTOKEN_COMMA  expr
{
    $$ = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    $$->expr_operator = GRuleExpression::OPERATOR_AND;
    $$->left_subexpr = $1;
    $$->right_subexpr = $3;
}
;

expr: expr  RTOKEN_UEQUAL  expr
{
    $$ = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    $$->expr_operator = GRuleExpression::OPERATOR_UEQUAL;
    $$->left_subexpr = $1;
    $$->right_subexpr = $3;
}
;

expr: expr RTOKEN_IMPLICATION expr RTOKEN_ELSE expr
{
    $$ = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    $$->expr_operator = GRuleExpression::OPERATOR_IMPLICATION;
    $$->left_subexpr = $1;
    $$->right_subexpr = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    $$->right_subexpr->expr_operator = GRuleExpression::OPERATOR_ELSE;
    $$->right_subexpr->left_subexpr = $3;
    $$->right_subexpr->right_subexpr = $5;
}


expr: expr RTOKEN_IMPLICATION expr
{
    $$ = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    $$->expr_operator = GRuleExpression::OPERATOR_IMPLICATION;
    $$->left_subexpr = $1;
    $$->right_subexpr = $3;
}

expr: expr  RTOKEN_NOT_EQUAL  expr
{
    $$ = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    $$->expr_operator = GRuleExpression::OPERATOR_NOT_EQUAL;
    $$->left_subexpr = $1;
    $$->right_subexpr = $3;
}
;

expr: expr  RTOKEN_AMPERSAND  expr
{
    $$ = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    $$->expr_operator = GRuleExpression::OPERATOR_HOOKING;
    $$->left_subexpr = $1;
    $$->right_subexpr = $3;
}
;


expr: expr  RTOKEN_EQUAL  expr
{
    $$ = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    $$->expr_operator = GRuleExpression::OPERATOR_EQUAL;
    $$->left_subexpr = $1;
    $$->right_subexpr = $3;
}
;

expr: RTOKEN_ATTRIBUTE  RTOKEN_UASSIGN  expr
{
    $$ = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    $$->expr_operator = GRuleExpression::OPERATOR_UASSIGN;
    $$->left_subexpr = new GRuleExpression(GRuleExpression::ATTRIBUTE);
    $$->left_subexpr->attribute = $1;
    $$->right_subexpr = $3;
}
;

expr: RTOKEN_ATTRIBUTE  RTOKEN_ASSIGN  expr
{
    $$ = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    $$->expr_operator = GRuleExpression::OPERATOR_ASSIGN;
    $$->left_subexpr = new GRuleExpression(GRuleExpression::ATTRIBUTE);
    $$->left_subexpr->attribute = $1;
    $$->right_subexpr = $3;
}
;

expr: expr  RTOKEN_CARET  expr
{
    $$ = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    $$->expr_operator = GRuleExpression::OPERATOR_SEMANTICS_INTERSECTION;
    $$->left_subexpr = $1;
    $$->right_subexpr = $3;
}
;

expr: expr  RTOKEN_PLUS  expr
{
    $$ = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    $$->expr_operator = GRuleExpression::OPERATOR_SUM;
    $$->left_subexpr = $1;
    $$->right_subexpr = $3;
}
;

expr: expr  RTOKEN_BINARG  expr
{
    $$ = new GRuleExpression(GRuleExpression::OPERATOR_BI);
    $$->expr_operator = GRuleExpression::OPERATOR_BINARG;
    $$->left_subexpr = $1;
    $$->right_subexpr = $3;
}
;

expr: RTOKEN_LEFT_PAREN  expr  RTOKEN_RIGHT_PAREN
{ $$ = $2;}
;

expr: RTOKEN_LEFT_CURLY  expr  RTOKEN_RIGHT_CURLY
{
    $$ = new GRuleExpression(GRuleExpression::OPERATOR_MONO);
    $$->expr_operator = GRuleExpression::OPERATOR_ALWAYS_TRUE;
    $$->left_subexpr = $2;
}
;

expr: RTOKEN_QMARK  expr
{
    $$ = new GRuleExpression(GRuleExpression::OPERATOR_MONO);
    $$->expr_operator = GRuleExpression::OPERATOR_PRINT;
    $$->left_subexpr = $2;
}
;

expr: RTOKEN_ATTRIBUTE
{
    $$ = new GRuleExpression(GRuleExpression::ATTRIBUTE);
    $$->attribute = $1;
}
;

expr: symbol_ref RTOKEN_DOT  RTOKEN_ATTRIBUTE
{
    $$ = $1;
    $$->attribute = $3;
}
;


expr: RTOKEN_ATOM
{
    $$ = new GRuleExpression(GRuleExpression::ATOM);
    $$->atom = $1;
}
;

expr: RTOKEN_NUMBER
{
    $$ = new GRuleExpression(GRuleExpression::NUMBER);
    $$->number = $1;
}
;

expr: tree_recipe_expr
{
    $$ = new GRuleExpression(GRuleExpression::TREE_CHOICE);
    $$->tree_choice = $1;
}

score_number: RTOKEN_SCORE
{
    $$ = $1;
}

score_number: RTOKEN_NUMBER
{
    $$ = static_cast<Lattice::Score>($1);
}

expr: RTOKEN_SETSCORE score_number
{
    $$ = new GRuleExpression(GRuleExpression::SETSCORE);
    $$->score = $2;
}



symbol_ref: RTOKEN_ATOM
{
    $$ = new GRuleExpression(GRuleExpression::ATTRIBUTE);
    $$->atom = $1;
}
| RTOKEN_ATOM RTOKEN_LEFT_SQUARE RTOKEN_NUMBER RTOKEN_RIGHT_SQUARE
{
    $$ = new GRuleExpression(GRuleExpression::ATTRIBUTE);
    $$->atom = $1;
    $$->number = $3;
}
;

%%
void grparsererror(const char* s)
    {
    printf("grparse error (%s) at line %d\n", s, grlexlineno);
    snprintf(grerror_message,127,"%d::%s",grlexlineno,s);
   }
