%option noyywrap
/* ^ macos thing */

%{
#include "parser.tab.h"
%}

ID              [a-zA-Z_][a-zA-Z0-9_]*
NUMBER          [0-9]+
BOOL            (true|false)
COMMENT         "/*"((("*"[^/])?)|[^*])*"*/"
LINE_COMMENT    "//".*

%%
"if"            return IF;
"else"          return ELSE;
"elif"          return ELIF;
"while"         return WHILE;
"for"           return FOR;
"print"         return PRINT;
{BOOL}          { yylval.boolval = !strcmp(yytext, "true"); return BOOL_LITERAL; }
{NUMBER}        { yylval.intval = atoi(yytext); return NUMBER; }
{ID}            { yylval.id = strdup(yytext); return ID; }
"=="            return EQ;
"!="            return NE;
"<="            return LE;
">="            return GE;
"<"             return '<';
">"             return '>';
"+"             return '+';
"-"             return '-';
"*"             return '*';
"/"             return '/';
"="             return '=';
"&&"            return AND;
"||"            return OR;
"!"             return NOT;
"("             return '(';
")"             return ')';
"{"             return '{';
"}"             return '}';
";"             return ';';
{COMMENT}       /* skip comments */;
{LINE_COMMENT}  /* skip one-line comments*/;
[ \t\n]+        /* skip whitespace */;
.               return yytext[0];
%%

