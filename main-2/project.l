LETTER [a-zA-Z]
DIGIT  [0-9]

%{
    #include <stdio.h>
    #include <string.h>
    #include "y.tab.h"
%}
%%

"\+" { printf("PLUS "); return PLUS; }
"\-" { printf("MINUS "); return MINUS; }
"\/" { printf("DIV "); return DIV; }
"\*" { printf("MULT "); return MULT; }
"=" { printf("EQUAL "); return EQUAL; }
":" { printf("COLON "); return COLON; }

"if " { printf("IF "); return IF; }
"elif " { printf("ELIF "); return ELIF; }
"else:" { printf("ELSE "); return ELSE; }

{LETTER}({LETTER}|{DIGIT}|"_")* {
  yylval.str=strdup(yytext);
  printf("VAR(%s) ", yytext);
  return VAR;
}

("0"|[1-9]{DIGIT}*) {
  yylval.str=strdup(yytext);
  printf("INT(%s) ", yytext);
  return INT;
}

(("0"|[1-9]{DIGIT}*)"."{DIGIT}+|("0"|[1-9]{DIGIT}*)"."|"."{DIGIT}+) {
  yylval.str=strdup(yytext);
  printf("FLOAT(%s) ", yytext);
  return FLOAT;
}

\"([#-~]|"\\\""|"!"|" ")*\" {
  yylval.str=strdup(yytext);
  printf("STRING(%s) ", yytext);
  return STRING;
}

"=="|"!="|"<="|"<"|">="|">" {
  yylval.str=strdup(yytext);
  printf("COMPARISON(%s) ", yytext);
  return COMPARISON;
}

\t|"    " { printf("TAB "); return TAB; }

\n { printf("NWLN "); return NWLN; }
. { printf("UNKNOWN(%s) ", yytext); }
%%
