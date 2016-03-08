# 33 "<stdin>"
#ifndef YY_YY_YYPARSE_H_INCLUDED
#define YY_YY_YYPARSE_H_INCLUDED 

#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif


#ifndef YYTOKENTYPE
#define YYTOKENTYPE 


   enum yytokentype {
     TOK_VOID = 258,
     TOK_BOOL = 259,
     TOK_CHAR = 260,
     TOK_INT = 261,
     TOK_STRING = 262,
     TOK_IF = 263,
     TOK_ELSE = 264,
     TOK_WHILE = 265,
     TOK_RETURN = 266,
     TOK_STRUCT = 267,
     TOK_FALSE = 268,
     TOK_TRUE = 269,
     TOK_NULL = 270,
     TOK_NEW = 271,
     TOK_ARRAY = 272,
     TOK_EQ = 273,
     TOK_NE = 274,
     TOK_LT = 275,
     TOK_LE = 276,
     TOK_GT = 277,
     TOK_GE = 278,
     TOK_IDENT = 279,
     TOK_INTCON = 280,
     TOK_CHARCON = 281,
     TOK_STRINGCON = 282,
     TOK_BLOCK = 283,
     TOK_CALL = 284,
     TOK_IFELSE = 285,
     TOK_INITDECL = 286,
     TOK_POS = 287,
     TOK_NEG = 288,
     TOK_NEWARRAY = 289,
     TOK_TYPEID = 290,
     TOK_FIELD = 291,
     TOK_ORD = 292,
     TOK_CHR = 293,
     TOK_ROOT = 294,
     TOK_FUNCTION = 295,
     TOK_PARAMLIST = 296,
     TOK_NEWSTRING = 297,
     TOK_INDEX = 298,
     TOK_RETURNVOID = 299,
     TOK_STATEMENTS = 300
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
#define YYSTYPE_IS_TRIVIAL 1
#define yystype YYSTYPE
#define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif

#endif
