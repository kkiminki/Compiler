
%{
//Kyler Kiminki
//kkiminki
//cmps104a
//lexer.l
//Builds the flex scanner


#include "lyutils.h"

#define YY_USER_ACTION  { lexer_useraction(); }

#define RETURN(SYMBOL) { \
   yylval_token(SYMBOL); \
   return SYMBOL; \
}

%}

%option 8bit
%option debug
%option nodefault
%option noinput
%option nounput
%option noyywrap
%option verbose
%option warn

LETTER          [A-Za-z_]
DIGIT           [0-9]
MANTISSA        ({DIGIT}+\.?{DIGIT}*|\.{DIGIT}+)
EXPONENT        ([Ee][+-]?{DIGIT}+)
NUMBER          ({MANTISSA}{EXPONENT}?)
NOTNUMBER       ({MANTISSA}[Ee][+-]?)
IDENT           ({LETTER}({LETTER}|{DIGIT})*)
INTCON          ({DIGIT}{DIGIT}*)
CHARCON         ('([^\\'\n]|\\[\\'"0nt])')
STRCON          (\"([^\\'\n]|\\[\\'"0nt])*\")

%%

"#".*           { lexer_include(); }
[ \t]+          { }
\n              { lexer_newline(); }

"="             { RETURN ('='); }
"+"             { RETURN ('+'); }
"-"             { RETURN ('-'); }
"*"             { RETURN ('*'); }
"/"             { RETURN ('/'); }
"^"             { RETURN ('^'); }
"("             { RETURN ('('); }
")"             { RETURN (')'); }
";"             { RETURN (';'); }
"["		{ RETURN ('['); }
"]"		{ RETURN (']'); }
"{"             { RETURN ('{'); }
"}"             { RETURN ('}'); }
"void"          { RETURN (TOK_VOID) }
"bool"          { RETURN (TOK_BOOL) }
"char"          { RETURN (TOK_CHAR) }
"int"           { RETURN (TOK_INT) }
"string"        { RETURN (TOK_STRING) }
"if"            { RETURN (TOK_IF) }
"else"          { RETURN (TOK_ELSE) }
"while"         { RETURN (TOK_WHILE) }
"return"        { RETURN (TOK_RETURN) }
"struct"        { RETURN (TOK_STRUCT) }
"false"         { RETURN (TOK_FALSE) }
"true"          { RETURN (TOK_TRUE) }
"null"          { RETURN (TOK_NULL) }
"new"           { RETURN (TOK_NEW) }
"[]"            { RETURN (TOK_ARRAY) }
"=="            { RETURN (TOK_EQ) }
"!="            { RETURN (TOK_NE) }
"<"             { RETURN (TOK_LT) }
"<="            { RETURN (TOK_LE) }
">"             { RETURN (TOK_GT) }
">="            { RETURN (TOK_GE) }
{IDENT}         { RETURN (TOK_IDENT) }
{INTCON}        { RETURN (TOK_INTCON) }
{CHARCON}       { RETURN (TOK_CHARCON) }
{STRCON}        { RETURN (TOK_STRINGCON) }


{NOTNUMBER}     { lexer_badtoken (yytext); RETURN (TOK_INTCON); }
.               { lexer_badchar (*yytext); }

%%

