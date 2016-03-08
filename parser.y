%{
// Dummy parser for scanner project.

#include "lyutils.h"
#include "astree.h"
#include "symtable.h"

%}

%debug
%defines
%error-verbose
%token-table
%verbose

%token TOK_VOID TOK_BOOL TOK_CHAR TOK_INT TOK_STRING
%token TOK_IF TOK_ELSE TOK_WHILE TOK_RETURN TOK_STRUCT
%token TOK_FALSE TOK_TRUE TOK_NULL TOK_NEW TOK_ARRAY
%token TOK_EQ TOK_NE TOK_LT TOK_LE TOK_GT TOK_GE
%token TOK_IDENT TOK_INTCON TOK_CHARCON TOK_STRINGCON

%token TOK_BLOCK TOK_CALL TOK_IFELSE TOK_INITDECL
%token TOK_POS TOK_NEG TOK_NEWARRAY TOK_TYPEID TOK_FIELD
%token TOK_ORD TOK_CHR TOK_ROOT TOK_FUNCTION TOK_PARAMLIST
%token TOK_NEWSTRING TOK_INDEX TOK_RETURNVOID TOK_STATEMENTS

%start program
%right TOK_IF
%right '='
%left TOK_EQ TOK_NE TOK_LT TOK_LE TOK_GT TOK_GE
%left '+' '-'
%left '*' '/' '%'
%right TOK_ELSE
%right TOK_POS TOK_NEG '!' TOK_NEW TOK_ORD TOK_CHR
%left '[' ']' '.' '('')' 


%%

start     : program {yyparse_astree = $1;}
          |
          ;
program   : program structdef {$$ = adopt1($1, $2);}
          | program function {$$ = adopt1($1, $2);}
          | program statement {$$ = adopt1($1, $2);}
          | program error '}' {$$ = $1;}
          | program error ';' {$$ = $1;}
          | {$$ = new_parseroot();}
          ;
structdef : TOK_STRUCT TOK_IDENT'{''}' {$$ = adopt1($1, $2); $2->symbol = TOK_TYPEID;struct_symbol($2);}
          | TOK_STRUCT TOK_IDENT '{'fields'}' {struct_fields_symbol($2, $4);
                                               $$ = adopt1($1, $2); $$ = adopt_children($$, $4);
                                               free_ast($4); $2->symbol = TOK_TYPEID;} 
          ;
fields    : fielddecl';' {$$ = new_node(); $$ = adopt1($$, $1);}
          | fields fielddecl';' {$$ = adopt1($1, $2);}
          ;
fielddecl : basetype TOK_IDENT { $$ = adopt1($1, $2); $2->symbol = TOK_FIELD;}
          | basetype TOK_ARRAY TOK_IDENT {$$ = adopt2($2, $1, $3); $3->symbol = TOK_FIELD;}
          ;
basetype  : TOK_VOID {$$ = $1;}
          | TOK_BOOL {$$ = $1;}
          | TOK_CHAR {$$ = $1;}
          | TOK_INT  {$$ = $1;}
          | TOK_STRING {$$ = $1;}
          ;
function  : identdecl '(' params ')'block { func_param_symbol($3);
                                            $$ = adopt_children($2, $3); 
                                            $$ = adopt2($2, $1, $5); 
                                            free_ast($3); 
                                            $$->symbol = TOK_FUNCTION;
                                            function_symbol($1);
                                            }
          | identdecl'('')'block {$$ = adopt2($2, $1, $4); $$->symbol = TOK_FUNCTION; function_symbol($1); }
          ;
params    : identdecl { $$ = new_node(); $$ = adopt1($$, $1);}
          | params ','identdecl { $$ = adopt1($$, $3);}
          ;
identdecl : basetype TOK_IDENT { $$ = adopt1($1, $2); $2->symbol = TOK_INITDECL; ident_symbol($2, $1);}
          | basetype TOK_ARRAY TOK_IDENT { $$ = adopt2($2, $1, $3); $3->symbol = TOK_INITDECL; array_symbol($3, $1);}
          ;
block     : '{'statements'}' {$$ = adopt_children($1, $2); free_ast($2); $$->symbol = TOK_BLOCK;}
          ;
statements: statement {$$ = new_node(); adopt1($$, $1);}
          |statements statement {$$ = adopt1($1, $2);}
          ;
statement : block {$$ = $1;}
          | vardecl {$$ = $1;}
          | while {$$ = $1;}
          | ifelse {$$ = $1;}
          | expr';' {$$ = $1;}
          | return {$$ = $1;}
          ;
vardecl   : identdecl'='expr';' { $$ = adopt2($2, $1, $3); }
          ;
while     : TOK_WHILE'('expr')' statement {$$ = adopt2($1, $3, $5);}
          ;
ifelse    : TOK_IF'('expr')'statement {$$ = adopt2($1, $3, $5);}
          | TOK_IF'('expr')'statement  %prec TOK_ELSE statement {$$ = adopt3($1, $3, $5, $6); 
                                                          $$->symbol = TOK_IFELSE; }
          ;
return    : TOK_RETURN ';' {$$ = $1; $$->symbol = TOK_RETURNVOID;}
          | TOK_RETURN expr';' {$$ = adopt1($1, $2);}
          ;
exprs     : expr {$$ = new_node(); $$ = adopt1($$, $1);}
          | exprs ','expr {$$ = adopt1($$, $1);}
          ;
expr      : expr '=' expr {$$ = adopt2($2, $1, $3);}
          | expr '+' expr {$$ = adopt2($2, $1, $3);}
          | expr '-' expr {$$ = adopt2($2, $1, $3);}
          | expr '*' expr {$$ = adopt2($2, $1, $3);}
          | expr '/' expr {$$ = adopt2($2, $1, $3);}
          | expr '%' expr {$$ = adopt2($2, $1, $3);}
          | expr TOK_EQ expr {$$ = adopt2($2, $1, $3);}
          | expr TOK_NE expr {$$ = adopt2($2, $1, $3);}
          | expr TOK_LT expr {$$ = adopt2($2, $1, $3);}
          | expr TOK_LE expr {$$ = adopt2($2, $1, $3);}
          | expr TOK_GT expr {$$ = adopt2($2, $1, $3);}
          | expr TOK_GE expr {$$ = adopt2($2, $1, $3);}    
          | '-'expr %prec TOK_NEG {$$ = adopt1($1, $2);}
          | '!'expr {$$ = adopt1($1, $2);} 
          | allocator {$$ = $1;}
          | call {$$ = $1;}
          | '('expr')' {$$ = adopt1($1, $2);}
          | variable {$$ = $1;}
          | constant {$$ = $1;}
          ;
allocator : TOK_NEW TOK_IDENT'('')' {$$ = adopt1($1, $2); $2->symbol = TOK_TYPEID;}
          | TOK_NEW TOK_STRING '('expr')' {$$ = adopt1($1, $4); $$->symbol = TOK_NEWSTRING;}
          | TOK_NEW basetype'['expr']' {$$ = adopt2($1, $2, $4); $$->symbol = TOK_NEWARRAY}
          ;
call      : TOK_IDENT'('exprs')' {check_param($3);
                                  $$ = adopt_children($2, $3); $$ = adopt1($$, $1); 
                                  free_ast($3); $$->symbol = TOK_CALL;
                                  check_func($1); }
          ;
variable  : TOK_IDENT {$$ = $1;var_symbol($1);}
          | expr'['expr']' {$$ = adopt2($2, $1, $3); $$->symbol = TOK_INDEX;} 
          | expr'.'TOK_FIELD { $$ = adopt2($2, $3, $1); $$->symbol = TOK_FIELD;}
          ;
constant  : TOK_INTCON {$$ = $1; const_symbol($1);}
          | TOK_CHARCON {$$ = $1;const_symbol($1);} 
          | TOK_STRINGCON {$$ = $1;const_symbol($1);}
          | TOK_FALSE {$$ = $1;const_symbol($1);}
          | TOK_TRUE {$$ = $1;const_symbol($1);}
          | TOK_NULL {$$ = $1;const_symbol($1);}
          ;

%%

const char *get_yytname (int symbol) {
   return yytname [YYTRANSLATE (symbol)];
}


bool is_defined_token (int symbol) {
   return YYTRANSLATE (symbol) > YYUNDEFTOK;
}

/*static void* yycalloc (size_t size) {
   void* result = calloc (1, size);
   assert (result != NULL);
   return result;
}*/

