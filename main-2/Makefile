all: lex yacc
	g++ lex.yy.c y.tab.c -ll -o project

yacc: project.y
	yacc -d project.y

lex: project.l
	lex project.l
