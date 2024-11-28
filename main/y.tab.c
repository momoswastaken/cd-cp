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
#line 1 "translator.y"

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdarg.h>
	#include <ctype.h>
	#include "y.tab.h"

	#define MAXRECST 200
	#define MAXST 100
	#define MAXCHILDREN 100
	#define MAXLEVELS 20
	#define MAXQUADS 1000
	
	
	extern int yylex();
	extern char* yytext;
	extern int yylineno;
	extern int depth;
	extern int scope_depth;
	extern int top();
	extern int pop();
	int currentScope = 1, previousScope = 1;
	
	int *arrayScope = NULL;

	void yyerror(const char *msg)
	{
		printf("\nSyntax Error at Line %d, Column : %d\n",  yylineno, yylloc.last_column);
		exit(0);
	}

	typedef struct record
	{
		char *type;
		char *name;
		int decLineNo;
		int lastUseLine;
	} record;

	typedef struct STable
	{
		int no;
		int noOfElems;
		int scope;
		record *Elements;
		int Parent;
		
	} STable;
	
	typedef struct ASTNode
	{
		int nodeNo;
    /*Operator*/
    char *NType;
    int noOps;
    struct ASTNode** NextLevel;
    
    /*Identifier or Const*/
    record *id;
	
	} node;
  
	typedef struct Quad
	{
		char *R;
		char *A1;
		char *A2;
		char *Op;
		int I;
	} Quad;
	
	STable *symbolTables = NULL;
	int sIndex = -1, aIndex = -1, tabCount = 0, tIndex = 0 , lIndex = 0, qIndex = 0, nodeCount = 0, id_counter = 0, label_counter = 0;
	node *rootNode;
	char *tString = NULL, *lString = NULL;
	Quad *allQ = NULL;
	record records[10000];
	char *labels[10000];
	node ***Tree = NULL;
	int *levelIndices = NULL;
	
	/*-----------------------------Declarations----------------------------------*/
	
	record* findRecord(const char *name, const char *type, int scope);
  	node *createID_Const(char *value, char *type, int scope);
  	int power(int base, int exp);
  	void updateCScope(int scope);
  	void resetDepth();
	int scopeBasedTableSearch(int scope);
	void initNewTable(int scope);
	void init();
	int searchRecordInScope(const char* type, const char *name, int index);
	void insertRecord(const char* type, const char *name, int lineNo, int scope);
	int searchRecordInScope(const char* type, const char *name, int index);
	void printSTable();
	void freeAll();
	int checkIfBinOperator(char *Op);
	
	/*------------------------------------------------------------------------------*/
	
	void Xitoa(int num, char *str)
	{
		if(str == NULL)
		{
			 printf("Allocate Memory\n");
		   return;
		}
		sprintf(str, "%d", num);
	}

	
	char *makeStr(int no, int flag)
	{
		char A[10];
		Xitoa(no, A);
		
		if(flag==1)
		{
				strcpy(tString, "T");
				strcat(tString, A);
				insertRecord("ICGTempVar", tString, -1, 0);
				return tString;
		}
		else
		{
				strcpy(lString, "L");
				strcat(lString, A);
				insertRecord("ICGTempLabel", lString, -1, 0);
				return lString;
		}

	}
	
	void makeQ(char *R, char *A1, char *A2, char *Op)
	{
		
		allQ[qIndex].R = (char*)malloc(strlen(R)+1);
		allQ[qIndex].Op = (char*)malloc(strlen(Op)+1);
		allQ[qIndex].A1 = (char*)malloc(strlen(A1)+1);
		allQ[qIndex].A2 = (char*)malloc(strlen(A2)+1);
		
		strcpy(allQ[qIndex].R, R);
		strcpy(allQ[qIndex].A1, A1);
		strcpy(allQ[qIndex].A2, A2);
		strcpy(allQ[qIndex].Op, Op);
		allQ[qIndex].I = qIndex;
 
		qIndex++;
		
		return;
	}
	
	int checkIfBinOperator(char *Op)
	{
		if((!strcmp(Op, "+")) || (!strcmp(Op, "*")) || (!strcmp(Op, "/")) || (!strcmp(Op, ">=")) || (!strcmp(Op, "<=")) || (!strcmp(Op, "<")) || (!strcmp(Op, ">")) || 
			 (!strcmp(Op, "in")) || (!strcmp(Op, "==")) || (!strcmp(Op, "and")) || (!strcmp(Op, "or")))
			{
				return 1;
			}
			
			else 
			{
				return 0;
			}
	}
	
	void codeGenOp(node *opNode){
		if(opNode == NULL)
		{
			return;
		}
		
		if(opNode->NType == NULL)
		{
			if((!strcmp(opNode->id->type, "Identifier")) || (!strcmp(opNode->id->type, "Constant")))
			{
				printf("T%d = %s\n", opNode->nodeNo, opNode->id->name);
				makeQ(makeStr(opNode->nodeNo, 1), opNode->id->name, "-", "=");
			}
			return;
		}
		
		if((!strcmp(opNode->NType, "If")) || (!strcmp(opNode->NType, "Elif")))
		{			
			switch(opNode->noOps)
			{
				case 2 : 
				{
					int temp = lIndex;
					codeGenOp(opNode->NextLevel[0]);
					printf("If False T%d goto L%d\n", opNode->NextLevel[0]->nodeNo, lIndex);
					makeQ(makeStr(temp, 0), makeStr(opNode->NextLevel[0]->nodeNo, 1), "-", "If False");
					lIndex++;
					codeGenOp(opNode->NextLevel[1]);
					lIndex--;
					printf("L%d: ", temp);
					makeQ(makeStr(temp, 0), "-", "-", "Label");
					break;
				}
				case 3 : 
				{
					int temp = lIndex;
					codeGenOp(opNode->NextLevel[0]);
					printf("If False T%d goto L%d\n", opNode->NextLevel[0]->nodeNo, lIndex);
					makeQ(makeStr(temp, 0), makeStr(opNode->NextLevel[0]->nodeNo, 1), "-", "If False");					
					codeGenOp(opNode->NextLevel[1]);
					printf("goto L%d\n", temp+1);
					makeQ(makeStr(temp+1, 0), "-", "-", "goto");
					printf("L%d: ", temp);
					makeQ(makeStr(temp, 0), "-", "-", "Label");
					codeGenOp(opNode->NextLevel[2]);
					printf("L%d: ", temp+1);
					makeQ(makeStr(temp+1, 0), "-", "-", "Label");
					lIndex+=2;
					break;
				}
			}
			return;
		}
		
		if(!strcmp(opNode->NType, "Else"))
		{
			codeGenOp(opNode->NextLevel[0]);
			return;
		}
		
		if(!strcmp(opNode->NType, "While"))
		{
			int temp = lIndex;
			printf("L%d:\n",  lIndex);
			makeQ(makeStr(temp, 0), "-", "-", "Label");
			codeGenOp(opNode->NextLevel[0]);
			printf("If False T%d goto L%d\n", opNode->NextLevel[0]->nodeNo, lIndex+1);		
			makeQ(makeStr(temp+1, 0), makeStr(opNode->NextLevel[0]->nodeNo, 1), "-", "If False");								
			lIndex+=2;			
			codeGenOp(opNode->NextLevel[1]);
			printf("goto L%d\n", temp);
			makeQ(makeStr(temp, 0), "-", "-", "goto");
			printf("L%d: ", temp+1);
			makeQ(makeStr(temp+1, 0), "-", "-", "Label"); 
			lIndex = lIndex+2;
			return;
		}
		if(!strcmp(opNode->NType, "For")){
			int temp = lIndex;

			// id assigning to zero
			printf("T%d = 0\n", opNode->nodeNo);
			int iteration_condition_temp_no = opNode->NextLevel[0]->NextLevel[0]->nodeNo;
			makeQ(makeStr(iteration_condition_temp_no, 1), "0", "-", "=");
			printf("%s = T%d\n", opNode->NextLevel[1]->id->name, iteration_condition_temp_no);
			char *id_name = opNode->NextLevel[1]->id->name;
			makeQ(opNode->NextLevel[1]->id->name, makeStr(iteration_condition_temp_no, 1), "-", "=");		

			// evaluating boolean expression
			printf("L%d:\n",  lIndex);
			makeQ(makeStr(temp, 0), "-", "-", "Label");
			codeGenOp(opNode->NextLevel[0]);
			printf("If False T%d goto L%d\n", opNode->NextLevel[0]->nodeNo, lIndex+1);		
			makeQ(makeStr(temp+1, 0), makeStr(opNode->NextLevel[0]->nodeNo, 1), "-", "If False");
			
			lIndex+=2;
			// start suite 			
			codeGenOp(opNode->NextLevel[3]);

			// increment operation
			char *X1 = (char*)malloc(10);
			char *X2 = (char*)malloc(10);
			
			strcpy(X1, makeStr(iteration_condition_temp_no, 1));
			strcpy(X2, makeStr(iteration_condition_temp_no, 1));

			printf("%s = T%d %s 1\n", id_name, iteration_condition_temp_no, "+");
			makeQ(X1, X2, "1", "+");
			free(X1);
			free(X2);
			printf("%s = T%d\n", id_name, opNode->nodeNo);
			makeQ( id_name, makeStr(iteration_condition_temp_no, 1), "-", "=");

			// iteration
			printf("goto L%d\n", temp);
			makeQ(makeStr(temp, 0), "-", "-", "goto");

			// end of iteration
			printf("L%d: ", temp+1);
			makeQ(makeStr(temp+1, 0), "-", "-", "Label"); 
			lIndex = lIndex+2;
			return;

		}
		if(!strcmp(opNode->NType, "Next"))
		{
			codeGenOp(opNode->NextLevel[0]);
			codeGenOp(opNode->NextLevel[1]);
			return;
		}
		
		if(!strcmp(opNode->NType, "BeginBlock"))
		{
			codeGenOp(opNode->NextLevel[0]);
			codeGenOp(opNode->NextLevel[1]);		
			return;	
		}
		
		if(!strcmp(opNode->NType, "EndBlock"))
		{
			switch(opNode->noOps)
			{
				case 0 : 
				{
					break;
				}
				case 1 : 
				{
					codeGenOp(opNode->NextLevel[0]);
					break;
				}
			}
			return;
		}
		
		if(!strcmp(opNode->NType, "ListIndex"))
		{
			printf("T%d = %s[%s]\n", opNode->nodeNo, opNode->NextLevel[0]->id->name, opNode->NextLevel[1]->id->name);
			makeQ(makeStr(opNode->nodeNo, 1), opNode->NextLevel[0]->id->name, opNode->NextLevel[1]->id->name, "=[]");
			return;
		}
		
		if(checkIfBinOperator(opNode->NType)==1)
		{
			codeGenOp(opNode->NextLevel[0]);
			codeGenOp(opNode->NextLevel[1]);
			char *X1 = (char*)malloc(10);
			char *X2 = (char*)malloc(10);
			char *X3 = (char*)malloc(10);
			
			strcpy(X1, makeStr(opNode->nodeNo, 1));
			strcpy(X2, makeStr(opNode->NextLevel[0]->nodeNo, 1));
			strcpy(X3, makeStr(opNode->NextLevel[1]->nodeNo, 1));

			printf("T%d = T%d %s T%d\n", opNode->nodeNo, opNode->NextLevel[0]->nodeNo, opNode->NType, opNode->NextLevel[1]->nodeNo);
			makeQ(X1, X2, X3, opNode->NType);
			free(X1);
			free(X2);
			free(X3);
			return;
		}
		
		if(!strcmp(opNode->NType, "-"))
		{
			if(opNode->noOps == 1)
			{
				codeGenOp(opNode->NextLevel[0]);
				char *X1 = (char*)malloc(10);
				char *X2 = (char*)malloc(10);
				strcpy(X1, makeStr(opNode->nodeNo, 1));
				strcpy(X2, makeStr(opNode->NextLevel[0]->nodeNo, 1));
				printf("T%d = %s T%d\n", opNode->nodeNo, opNode->NType, opNode->NextLevel[0]->nodeNo);
				makeQ(X1, X2, "-", opNode->NType);	
			}
			
			else
			{
				codeGenOp(opNode->NextLevel[0]);
				codeGenOp(opNode->NextLevel[1]);
				char *X1 = (char*)malloc(10);
				char *X2 = (char*)malloc(10);
				char *X3 = (char*)malloc(10);
			
				strcpy(X1, makeStr(opNode->nodeNo, 1));
				strcpy(X2, makeStr(opNode->NextLevel[0]->nodeNo, 1));
				strcpy(X3, makeStr(opNode->NextLevel[1]->nodeNo, 1));

				printf("T%d = T%d %s T%d\n", opNode->nodeNo, opNode->NextLevel[0]->nodeNo, opNode->NType, opNode->NextLevel[1]->nodeNo);
				makeQ(X1, X2, X3, opNode->NType);
				free(X1);
				free(X2);
				free(X3);
				return;
			
			}
		}
		
		if(!strcmp(opNode->NType, "NewLine"))
		{
			codeGenOp(opNode->NextLevel[0]);
			codeGenOp(opNode->NextLevel[1]);
			return;
		}
		
		if(!strcmp(opNode->NType, "="))
		{
			codeGenOp(opNode->NextLevel[1]);
			printf("%s = T%d\n", opNode->NextLevel[0]->id->name, opNode->NextLevel[1]->nodeNo);
			makeQ(opNode->NextLevel[0]->id->name, makeStr(opNode->NextLevel[1]->nodeNo, 1), "-", opNode->NType);
			return;
		}
		
		
		if(!(strcmp(opNode->NType, "Print")))
		{
			codeGenOp(opNode->NextLevel[0]);
			printf("Print T%d\n", opNode->NextLevel[0]->nodeNo);
			makeQ("-", makeStr(opNode->nodeNo, 1), "-", "Print");
		}
		if(opNode->noOps == 0)
		{
			if(!strcmp(opNode->NType, "break"))
			{
				printf("goto L%d\n", lIndex);
				makeQ(makeStr(lIndex, 0), "-", "-", "goto");
			}

			
			if(!strcmp(opNode->NType, "return"))
			{
				printf("return\n");
				makeQ("-", "-", "-", "return");
			}
		}
		
		
	}
	
  node *createID_Const(char *type, char *value, int scope)
  {
    node *newNode;
    newNode = (node*)calloc(1, sizeof(node));
    newNode->NType = NULL;
    newNode->noOps = -1;
    newNode->id = findRecord(value, type, scope);
    newNode->nodeNo = nodeCount++;
    return newNode;
  }

  node *createOp(char *oper, int noOps, ...)
  {
    va_list params;
    node *newNode;
    int i;
    newNode = (node*)calloc(1, sizeof(node));
    
    newNode->NextLevel = (node**)calloc(noOps, sizeof(node*));
    
    newNode->NType = (char*)malloc(strlen(oper)+1);
    strcpy(newNode->NType, oper);
    newNode->noOps = noOps;
    va_start(params, noOps);
    for (i = 0; i < noOps; i++)
    	newNode->NextLevel[i] = va_arg(params, node*);
		
    va_end(params);
    newNode->nodeNo = nodeCount++;
    return newNode;
  }
  
  
  
	int power(int base, int exp)
	{
		int i =0, res = 1;
		for(i; i<exp; i++)
		{
			res *= base;
		}
		return res;
	}
	
	void updateCScope(int scope)
	{
		currentScope = scope;
	}
	
	void resetDepth()
	{
		while(top()) pop();
		depth = 10;
	}
	
	int scopeBasedTableSearch(int scope)
	{
		int i = sIndex;
		for(i; i > -1; i--)
		{
			if(symbolTables[i].scope == scope)
			{
				return i;
			}
		}
		return -1;
	}
	
	void initNewTable(int scope)
	{
		arrayScope[scope]++;
		sIndex++;
		symbolTables[sIndex].no = sIndex;
		symbolTables[sIndex].scope = power(scope, arrayScope[scope]);
		symbolTables[sIndex].noOfElems = 0;		
		symbolTables[sIndex].Elements = (record*)calloc(MAXRECST, sizeof(record));
		
		symbolTables[sIndex].Parent = scopeBasedTableSearch(currentScope); 
	}
	
	void init()
	{
		int i = 0;
		symbolTables = (STable*)calloc(MAXST, sizeof(STable));
		arrayScope = (int*)calloc(100, sizeof(int));
		initNewTable(1);
		tString = (char*)calloc(10, sizeof(char));
		lString = (char*)calloc(10, sizeof(char));
		allQ = (Quad*)calloc(MAXQUADS, sizeof(Quad));
		
		levelIndices = (int*)calloc(MAXLEVELS, sizeof(int));
		Tree = (node***)calloc(MAXLEVELS, sizeof(node**));
		for(i = 0; i<MAXLEVELS; i++)
		{
			Tree[i] = (node**)calloc(MAXCHILDREN, sizeof(node*));
		}
	}

	int searchRecordInScope(const char* type, const char *name, int index)
	{
		int i =0;
		for(i=0; i<symbolTables[index].noOfElems; i++)
		{
			if((strcmp(symbolTables[index].Elements[i].type, type)==0) && (strcmp(symbolTables[index].Elements[i].name, name)==0))
			{
				return i;
			}	
		}
		return -1;
	}
		
	void modifyRecordID(const char *type, const char *name, int lineNo, int scope)
	{
		int i =0;
		int index = scopeBasedTableSearch(scope);
		if(index==0)
		{
			for(i=0; i<symbolTables[index].noOfElems; i++)
			{
				
				if(strcmp(symbolTables[index].Elements[i].type, type)==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
				{
					symbolTables[index].Elements[i].lastUseLine = lineNo;
					return;
				}	
			}
			printf("%s '%s' at line %d Not Declared\n", type, name, yylineno);
			exit(1);
		}
		
		for(i=0; i<symbolTables[index].noOfElems; i++)
		{
			if(strcmp(symbolTables[index].Elements[i].type, type)==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
			{
				symbolTables[index].Elements[i].lastUseLine = lineNo;
				return;
			}	
		}
		return modifyRecordID(type, name, lineNo, symbolTables[symbolTables[index].Parent].scope);
	}
	
	void insertRecord(const char* type, const char *name, int lineNo, int scope)
	{ 
		int FScope = power(scope, arrayScope[scope]);
		int index = scopeBasedTableSearch(FScope);
		int recordIndex = searchRecordInScope(type, name, index);
		if(recordIndex==-1)
		{
			
			symbolTables[index].Elements[symbolTables[index].noOfElems].type = (char*)calloc(30, sizeof(char));
			symbolTables[index].Elements[symbolTables[index].noOfElems].name = (char*)calloc(20, sizeof(char));
		
			strcpy(symbolTables[index].Elements[symbolTables[index].noOfElems].type, type);	
			strcpy(symbolTables[index].Elements[symbolTables[index].noOfElems].name, name);
			symbolTables[index].Elements[symbolTables[index].noOfElems].decLineNo = lineNo;
			symbolTables[index].Elements[symbolTables[index].noOfElems].lastUseLine = lineNo;
			symbolTables[index].noOfElems++;

		}
		else
		{
			symbolTables[index].Elements[recordIndex].lastUseLine = lineNo;
		}
	}
	
	
	record* findRecord(const char *name, const char *type, int scope)
	{
		int i =0;
		int index = scopeBasedTableSearch(scope);
		//printf("FR: %d, %s\n", scope, name);
		if(index==0)
		{
			for(i=0; i<symbolTables[index].noOfElems; i++)
			{
				
				if(strcmp(symbolTables[index].Elements[i].type, type)==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
				{
					return &(symbolTables[index].Elements[i]);
				}	
			}
			printf("\n%s '%s' at line %d Not Found in Symbol Table\n", type, name, yylineno);
			exit(1);
		}
		
		for(i=0; i<symbolTables[index].noOfElems; i++)
		{
			if(strcmp(symbolTables[index].Elements[i].type, type)==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
			{
				return &(symbolTables[index].Elements[i]);
			}	
		}
		return findRecord(name, type, symbolTables[symbolTables[index].Parent].scope);
	}

	void printSTable()
	{
		int i = 0, j = 0;
		
		printf("\n------------------------Symbol Tables---------------------------");
		printf("\nScope\tName/Value\tType\t\tDeclaration\tLast Used Line\n");
		for(i=0; i<=sIndex; i++)
		{
			for(j=0; j<symbolTables[i].noOfElems; j++)
			{	
				if(!strcmp(symbolTables[i].Elements[j].type,"ICGTempLabel"))
					printf("%d\t%s\t\t%s\t\n", symbolTables[i].scope, symbolTables[i].Elements[j].name, "Label");
				else if(!strcmp(symbolTables[i].Elements[j].type,"ICGTempVar"))
					printf("%d\t%s\t\t%s\t\n", symbolTables[i].scope, symbolTables[i].Elements[j].name, "Temp");	
				else
					printf("%d\t%s\t\t%s\t\t%d\t\t%d\n", symbolTables[i].scope, symbolTables[i].Elements[j].name, symbolTables[i].Elements[j].type, symbolTables[i].Elements[j].decLineNo,  symbolTables[i].Elements[j].lastUseLine);
			}
		}
		
		printf("-------------------------------------------------------------------\n");
		
	}
	
	void ASTToArray(node *root, int level)
	{
	  if(root == NULL )
	  {
	    return;
	  }
	  
	  if(root->noOps <= 0)
	  {
	  	Tree[level][levelIndices[level]] = root;
	  	levelIndices[level]++;
	  }
	  
	  if(root->noOps > 0)
	  {
	 		int j;
	 		Tree[level][levelIndices[level]] = root;
	 		levelIndices[level]++; 
	    for(j=0; j<root->noOps; j++)
	    {
	    	ASTToArray(root->NextLevel[j], level+1);
	    }
	  }
	}
	
	void printAST(node *root)
	{
		printf("\n-----------------------Syntax Tree--------------------------\n");
		ASTToArray(root, 0);
		int j = 0, p, q, maxLevel = 0, lCount = 0;
		
		while(levelIndices[maxLevel] > 0) maxLevel++;
		
		while(levelIndices[j] > 0)
		{
			for(q=0; q<lCount; q++)
			{
				printf(" ");
			
			}
			for(p=0; p<levelIndices[j] ; p++)
			{
				if(Tree[j][p]->noOps == -1)
				{
					printf("%s  ", Tree[j][p]->id->name);
					lCount+=strlen(Tree[j][p]->id->name);
				}
				else if(Tree[j][p]->noOps == 0)
				{
					printf("%s  ", Tree[j][p]->NType);
					lCount+=strlen(Tree[j][p]->NType);
				}
				else
				{
					printf("%s(%d) ", Tree[j][p]->NType, Tree[j][p]->noOps);
				}
			}
			j++;
			printf("\n");
		}
	}
	
	int IsValidNumber(char * string)
	{
   for(int i = 0; i < strlen( string ); i ++)
   {
      //ASCII value of 0 = 48, 9 = 57. So if value is outside of numeric range then fail
      //Checking for negative sign "-" could be added: ASCII value 45.
      if (string[i] < 48 || string[i] > 57)
         return 0;
   }
 
   return 1;
	}
	
	int deadCodeElimination()
	{
		int i = 0, j = 0, flag = 1, XF=0;
		while(flag==1)
		{
			flag=0;
			for(i=0; i<qIndex; i++)
			{
				XF=0;
				if(!((strcmp(allQ[i].R, "-")==0) | (strcmp(allQ[i].Op, "Call")==0) | (strcmp(allQ[i].Op, "Label")==0) | (strcmp(allQ[i].Op, "goto")==0) | (strcmp(allQ[i].Op, "If False")==0)))
				{
					for(j=0; j<qIndex; j++)
					{
							if(((strcmp(allQ[i].R, allQ[j].A1)==0) && (allQ[j].I!=-1)) | ((strcmp(allQ[i].R, allQ[j].A2)==0) && (allQ[j].I!=-1)))
							{
								XF=1;
							}
					}
				
					if((XF==0) & (allQ[i].I != -1))
					{
						allQ[i].I = -1;
						flag=1;
					}
				}
			}
		}
		return flag;
	}
	
	
	void printQuads(){
		printf("\n------------------------Three-address code---------------------------\n");
		int i = 0;
		qIndex;
		
		for(i=0; i<qIndex; i++)
		{
			if(allQ[i].I > -1){
				if(!strcmp(allQ[i].A2 ,"-")){
					printf("%d\t%s\t%s\t%s\n", allQ[i].I,allQ[i].R,  allQ[i].Op, allQ[i].A1);
				}else	
				printf("%d\t%s\t%s\t%s\t%s\n", allQ[i].I,allQ[i].R,  allQ[i].Op, allQ[i].A1,  allQ[i].A2);
			}
		}
		printf("\n--------------------------------------------------------------------------\n");
		
	}

	int ContainsLetter(const char* String){
		//Abort if no string is passed
		if(String == NULL) return 0;
	
		while((*String) != '\0')
		{
			if(isalpha(*String)) return 1;
			String++;
		}
		return 0;
	}

	

	char* check_data_type(char* data){
		// it's a number
		if(ContainsLetter(data) == 0){
			if(strchr(data, '.')){
				return "double";
			}
			else return "int";	
		}
		else if(strchr(data, '"')) return "char*";
		else if(!strcmp(data,"True")) return "int";
		else if(!strcmp(data,"False")) return "int";
		else return "id";
	}

	char* find_id_data_type(char* name){
		int i = 0;
		if(id_counter == 0){
			return NULL;
		}
		for(i=0; i<id_counter; i++){
			// printf("%s-%s\n", records[i].name, name);
			if(!strcmp(records[i].name, name)){
				// printf("type: %s\n", records[i].type);
				return records[i].type;
			}
		}
		return NULL;
	}

	int check_label(char *label){
		int i;
		for(int i = 0; i < label_counter;i++){
			if(!strcmp(labels[i], label)){
				return 0;
			}
		}
		return 1;
	}
	int find_index_of_condition(char* lv_name){
		for(int i=0; i<qIndex; i++){
			if(!strcmp(lv_name, allQ[i].R)){
				return i;
			}
		}
		return -1;
	}
	// generate c output code
	void generateCCode(){
		FILE *file = fopen("output.c", "w");
		if (file == NULL){   
			printf("Error! Could not open file\n"); 
			exit(-1); // must include stdlib.h 
		} 
		fprintf(file, "//Auto Generated C Code. Call the c_code() function inside your main() Function.\n//It can be included by either linking or defining.\n//Include all preprocessors into your code\n"); // write to file
		// write main
		fprintf(file, "#include <stdio.h>\n#define True 1\n#define False 0\n");
		// Declarations
		fprintf(file, "void c_code();\n");
		// Main
		fprintf(file, "int main(){\n");
		fprintf(file, "\tc_code();\n}\n");
		// c_code() Definition
		printf("\n--------------------------------C-Code---------------------------------\n");
		fprintf(file, "void c_code(){\n");
		printf("void c_code(){\n");
		int i;
		for(i=0; i<qIndex; i++)
		{
			if(allQ[i].I > -1){
				// right value is an id
				if(strstr(allQ[i].Op, "goto")){
					fprintf(file, "\tgoto %s;\n", allQ[i].R);
					printf("\tgoto %s;\n", allQ[i].R);
				}
				
				else if(strstr(allQ[i].Op, "Label")){
					// dead label elimination
					if(check_label(allQ[i].R)){
						labels[label_counter] = (char *)malloc(strlen(allQ[i].R)+1);
						labels[label_counter] = allQ[i].R;
						label_counter++;
						fprintf(file, "%s: ;\n",  allQ[i].R);
						printf("%s: ;\n",  allQ[i].R);
					}
					// fprintf(file, "%s:\n",  allQ[i].R);
					// printf("%s:\n",  allQ[i].R);
				}
				else if(strstr(allQ[i].Op, "If")){
					if(!strcmp("If False", allQ[i].Op)){
						// if(find_index_of_condition(allQ[i].A1)){
						// 	int idx = find_index_of_condition(allQ[i].A1);
						// 	// fprintf(file, "\t%s %s %s;\n", allQ[idx].A1, allQ[idx].Op, allQ[idx].A2);
						// 	// printf("\t%s %s %s;\n", allQ[idx].A1, allQ[idx].Op, allQ[idx].A2);
						// 	fprintf(file, "\tif(!(%s %s %s)){\n\t\tgoto %s;\n\t}\n", allQ[idx].A1, allQ[idx].Op, allQ[idx].A2,  allQ[i].R);
						// 	printf("\tif(!(%s %s %s)){\n\t\tgoto %s;\n\t}\n", allQ[idx].A1, allQ[idx].Op, allQ[idx].A2,  allQ[i].R);
						// }
						fprintf(file, "\tif(!%s){\n\t\tgoto %s;\n\t}\n", allQ[i].A1,  allQ[i].R);
						printf("\tif(!%s){\n\t\tgoto %s;\n\t}\n", allQ[i].A1,  allQ[i].R);		
					}
					else if(!strcmp("If True", allQ[i].Op)){
						fprintf(file, "\tif(%s){\n\t\tgoto %s;\n\t}\n", allQ[i].A1,  allQ[i].R);
						printf("\tif(%s){\n\t\tgoto %s;\n\t}\n", allQ[i].A1,  allQ[i].R);		
					}
				}
				// other ops such as "=" 
				else{
					// printf("\t%s\t%s\t%s\t%s\n",   allQ[i].Op, allQ[i].A1, allQ[i].A2, allQ[i].R);
					// if rl has id
					if(check_data_type(allQ[i].A1) == "id"){
						// one left value operation
						if(strchr(allQ[i].A2, '-')){
							char* type;
							if(find_id_data_type(allQ[i].A1) != NULL){
								
								type = find_id_data_type(allQ[i].A1);
								// printf("type: %s\n", type);
							}
							// create a record for left id if it is not existed 
							if(find_id_data_type(allQ[i].R)){
								// reassign the variable
								fprintf(file, "\t%s %s %s;\n", allQ[i].R, allQ[i].Op, allQ[i].A1);
								printf("\t%s %s %s;\n", allQ[i].R, allQ[i].Op, allQ[i].A1);
							}
							else{
								record lr;
								records[id_counter] = lr;
								lr.name = allQ[i].R;
								lr.type = type;
								lr.decLineNo = 0;
								lr.lastUseLine = 0;
								// printf("type: %s\n", lr.type);
								records[id_counter] = lr;
								id_counter++;
								// define the variable
								fprintf(file, "\t%s %s %s %s;\n", type, allQ[i].R, allQ[i].Op, allQ[i].A1);
								printf("\t%s %s %s %s;\n", type, allQ[i].R, allQ[i].Op, allQ[i].A1);
							}	
						}
						else{
							char* type;
							if(find_id_data_type(allQ[i].A1)){
								type = find_id_data_type(allQ[i].A1);
							}
							// create a record for left id if it is not existed 
							if(find_id_data_type(allQ[i].R)){
								// reassign the variable
								fprintf(file, "\t%s = %s %s %s;\n", allQ[i].R, allQ[i].A1, allQ[i].Op, allQ[i].A2);
								printf("\t%s  = %s %s %s;\n", allQ[i].R, allQ[i].A1, allQ[i].Op, allQ[i].A2);
							}
							else{
								record lr;
								lr.name = allQ[i].R;
								lr.type = type;
								records[id_counter] = lr;
								id_counter++;
								// define the variable
								fprintf(file, "\t%s %s = %s %s %s;\n", type, allQ[i].R, allQ[i].A1, allQ[i].Op, allQ[i].A2);
								printf("\t%s %s  = %s %s %s;\n", type, allQ[i].R, allQ[i].A1, allQ[i].Op, allQ[i].A2);
							}
						}
						// fprintf("\t%s\t%s\t%s\t%s\n",   allQ[i].Op, allQ[i].A1, allQ[i].A2, allQ[i].R);
					}
					// if rl is a number or a string
					else {
						
						if(strchr(allQ[i].A2, '-')){
							char* type = check_data_type(allQ[i].A1);						
							if(find_id_data_type(allQ[i].R) != NULL){
								// reassign the variable
								fprintf(file, "\t%s %s %s;\n", allQ[i].R, allQ[i].Op, allQ[i].A1);
								printf("\t%s %s %s;\n", allQ[i].R, allQ[i].Op, allQ[i].A1);
							}
							else{								
								records[id_counter].name = (char *)malloc(strlen(allQ[i].R)+1);
								records[id_counter].type = (char *)malloc(strlen(type)+1);
								strcpy(records[id_counter].name, allQ[i].R);
								strcpy(records[id_counter].type, type);
								id_counter++;
								// define the variable
								fprintf(file, "\t%s %s %s %s;\n", type, allQ[i].R, allQ[i].Op, allQ[i].A1);
								printf("\t%s %s %s %s;\n", type, allQ[i].R, allQ[i].Op, allQ[i].A1);
							}	
						}
						else{
							char* type;
							if(find_id_data_type(allQ[i].A1)){
								type = find_id_data_type(allQ[i].A1);
							}
							// create a record for left id if it is not existed 
							if(find_id_data_type(allQ[i].R)){
								// reassign the variable
								fprintf(file, "\t%s = %s %s %s;\n", allQ[i].R, allQ[i].A1, allQ[i].Op, allQ[i].A2);
								printf("\t%s  = %s %s %s;\n", allQ[i].R, allQ[i].A1, allQ[i].Op, allQ[i].A2);
							}
							else{
								// printf("[DEBUG] saving into table");
								record lr;
								lr.name = allQ[i].R;
								lr.type = type;
								records[id_counter] = lr;
								id_counter++;
								// define the variable
								fprintf(file, "\t%s %s = %s %s %s;\n", type, allQ[i].R, allQ[i].A1, allQ[i].Op, allQ[i].A2);
								printf("\t%s %s  = %s %s %s;\n", type, allQ[i].R, allQ[i].A1, allQ[i].Op, allQ[i].A2);
							}
						}
					}
				}
				
			}
				
		}

		fprintf(file, "}\n");
		fclose(file);
		printf("}\n");
		printf("\n--------------------------------------------------------------------------\n");
	}
	
	void freeAll()
	{		
		printf("Remove Unused Variables");
		deadCodeElimination();
		printQuads();
		generateCCode();
		printf("\n");
		int i = 0, j = 0;
		for(i=0; i<=sIndex; i++)
		{
			for(j=0; j<symbolTables[i].noOfElems; j++)
			{
				free(symbolTables[i].Elements[j].name);
				free(symbolTables[i].Elements[j].type);	
			}
			free(symbolTables[i].Elements);
		}
		free(symbolTables);
		free(allQ);
	}

#line 1093 "y.tab.c"

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
    T_EndOfFile = 258,             /* T_EndOfFile  */
    T_Return = 259,                /* T_Return  */
    T_Number = 260,                /* T_Number  */
    T_True = 261,                  /* T_True  */
    T_False = 262,                 /* T_False  */
    T_ID = 263,                    /* T_ID  */
    T_Print = 264,                 /* T_Print  */
    T_Range = 265,                 /* T_Range  */
    T_Cln = 266,                   /* T_Cln  */
    T_NL = 267,                    /* T_NL  */
    T_EQL = 268,                   /* T_EQL  */
    T_NEQ = 269,                   /* T_NEQ  */
    T_EQ = 270,                    /* T_EQ  */
    T_GT = 271,                    /* T_GT  */
    T_LT = 272,                    /* T_LT  */
    T_EGT = 273,                   /* T_EGT  */
    T_ELT = 274,                   /* T_ELT  */
    T_Or = 275,                    /* T_Or  */
    T_And = 276,                   /* T_And  */
    T_Not = 277,                   /* T_Not  */
    T_In = 278,                    /* T_In  */
    ID = 279,                      /* ID  */
    ND = 280,                      /* ND  */
    DD = 281,                      /* DD  */
    T_String = 282,                /* T_String  */
    T_If = 283,                    /* T_If  */
    T_Elif = 284,                  /* T_Elif  */
    T_While = 285,                 /* T_While  */
    T_For = 286,                   /* T_For  */
    T_Else = 287,                  /* T_Else  */
    T_Break = 288,                 /* T_Break  */
    T_MN = 289,                    /* T_MN  */
    T_PL = 290,                    /* T_PL  */
    T_DV = 291,                    /* T_DV  */
    T_ML = 292,                    /* T_ML  */
    T_OP = 293,                    /* T_OP  */
    T_CP = 294,                    /* T_CP  */
    T_OB = 295,                    /* T_OB  */
    T_CB = 296,                    /* T_CB  */
    T_Def = 297,                   /* T_Def  */
    T_Comma = 298,                 /* T_Comma  */
    T_List = 299                   /* T_List  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define T_EndOfFile 258
#define T_Return 259
#define T_Number 260
#define T_True 261
#define T_False 262
#define T_ID 263
#define T_Print 264
#define T_Range 265
#define T_Cln 266
#define T_NL 267
#define T_EQL 268
#define T_NEQ 269
#define T_EQ 270
#define T_GT 271
#define T_LT 272
#define T_EGT 273
#define T_ELT 274
#define T_Or 275
#define T_And 276
#define T_Not 277
#define T_In 278
#define ID 279
#define ND 280
#define DD 281
#define T_String 282
#define T_If 283
#define T_Elif 284
#define T_While 285
#define T_For 286
#define T_Else 287
#define T_Break 288
#define T_MN 289
#define T_PL 290
#define T_DV 291
#define T_ML 292
#define T_OP 293
#define T_CP 294
#define T_OB 295
#define T_CB 296
#define T_Def 297
#define T_Comma 298
#define T_List 299

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 1023 "translator.y"
 char *text; int depth; int scope_depth; struct ASTNode* node;

#line 1237 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_T_EndOfFile = 3,                /* T_EndOfFile  */
  YYSYMBOL_T_Return = 4,                   /* T_Return  */
  YYSYMBOL_T_Number = 5,                   /* T_Number  */
  YYSYMBOL_T_True = 6,                     /* T_True  */
  YYSYMBOL_T_False = 7,                    /* T_False  */
  YYSYMBOL_T_ID = 8,                       /* T_ID  */
  YYSYMBOL_T_Print = 9,                    /* T_Print  */
  YYSYMBOL_T_Range = 10,                   /* T_Range  */
  YYSYMBOL_T_Cln = 11,                     /* T_Cln  */
  YYSYMBOL_T_NL = 12,                      /* T_NL  */
  YYSYMBOL_T_EQL = 13,                     /* T_EQL  */
  YYSYMBOL_T_NEQ = 14,                     /* T_NEQ  */
  YYSYMBOL_T_EQ = 15,                      /* T_EQ  */
  YYSYMBOL_T_GT = 16,                      /* T_GT  */
  YYSYMBOL_T_LT = 17,                      /* T_LT  */
  YYSYMBOL_T_EGT = 18,                     /* T_EGT  */
  YYSYMBOL_T_ELT = 19,                     /* T_ELT  */
  YYSYMBOL_T_Or = 20,                      /* T_Or  */
  YYSYMBOL_T_And = 21,                     /* T_And  */
  YYSYMBOL_T_Not = 22,                     /* T_Not  */
  YYSYMBOL_T_In = 23,                      /* T_In  */
  YYSYMBOL_ID = 24,                        /* ID  */
  YYSYMBOL_ND = 25,                        /* ND  */
  YYSYMBOL_DD = 26,                        /* DD  */
  YYSYMBOL_T_String = 27,                  /* T_String  */
  YYSYMBOL_T_If = 28,                      /* T_If  */
  YYSYMBOL_T_Elif = 29,                    /* T_Elif  */
  YYSYMBOL_T_While = 30,                   /* T_While  */
  YYSYMBOL_T_For = 31,                     /* T_For  */
  YYSYMBOL_T_Else = 32,                    /* T_Else  */
  YYSYMBOL_T_Break = 33,                   /* T_Break  */
  YYSYMBOL_T_MN = 34,                      /* T_MN  */
  YYSYMBOL_T_PL = 35,                      /* T_PL  */
  YYSYMBOL_T_DV = 36,                      /* T_DV  */
  YYSYMBOL_T_ML = 37,                      /* T_ML  */
  YYSYMBOL_T_OP = 38,                      /* T_OP  */
  YYSYMBOL_T_CP = 39,                      /* T_CP  */
  YYSYMBOL_T_OB = 40,                      /* T_OB  */
  YYSYMBOL_T_CB = 41,                      /* T_CB  */
  YYSYMBOL_T_Def = 42,                     /* T_Def  */
  YYSYMBOL_T_Comma = 43,                   /* T_Comma  */
  YYSYMBOL_T_List = 44,                    /* T_List  */
  YYSYMBOL_YYACCEPT = 45,                  /* $accept  */
  YYSYMBOL_StartDebugger = 46,             /* StartDebugger  */
  YYSYMBOL_47_1 = 47,                      /* $@1  */
  YYSYMBOL_constant = 48,                  /* constant  */
  YYSYMBOL_term = 49,                      /* term  */
  YYSYMBOL_StartParse = 50,                /* StartParse  */
  YYSYMBOL_51_2 = 51,                      /* $@2  */
  YYSYMBOL_basic_stmt = 52,                /* basic_stmt  */
  YYSYMBOL_arith_exp = 53,                 /* arith_exp  */
  YYSYMBOL_bool_exp = 54,                  /* bool_exp  */
  YYSYMBOL_bool_term = 55,                 /* bool_term  */
  YYSYMBOL_bool_factor = 56,               /* bool_factor  */
  YYSYMBOL_break_stmt = 57,                /* break_stmt  */
  YYSYMBOL_assign_stmt = 58,               /* assign_stmt  */
  YYSYMBOL_print_stmt = 59,                /* print_stmt  */
  YYSYMBOL_finalStatements = 60,           /* finalStatements  */
  YYSYMBOL_cmpd_stmt = 61,                 /* cmpd_stmt  */
  YYSYMBOL_if_stmt = 62,                   /* if_stmt  */
  YYSYMBOL_elif_stmts = 63,                /* elif_stmts  */
  YYSYMBOL_else_stmt = 64,                 /* else_stmt  */
  YYSYMBOL_while_stmt = 65,                /* while_stmt  */
  YYSYMBOL_myrange = 66,                   /* myrange  */
  YYSYMBOL_for_stmt = 67,                  /* for_stmt  */
  YYSYMBOL_start_suite = 68,               /* start_suite  */
  YYSYMBOL_69_3 = 69,                      /* $@3  */
  YYSYMBOL_suite = 70,                     /* suite  */
  YYSYMBOL_end_suite = 71,                 /* end_suite  */
  YYSYMBOL_72_4 = 72,                      /* $@4  */
  YYSYMBOL_73_5 = 73                       /* $@5  */
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
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   227

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  123

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   299


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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,  1039,  1039,  1039,  1043,  1045,  1046,  1050,  1050,  1050,
    1050,  1052,  1053,  1054,  1055,  1056,  1058,  1059,  1060,  1061,
    1062,  1063,  1064,  1067,  1068,  1069,  1070,  1071,  1072,  1073,
    1075,  1076,  1077,  1078,  1079,  1081,  1082,  1084,  1086,  1087,
    1089,  1091,  1092,  1093,  1095,  1096,  1097,  1100,  1101,  1103,
    1104,  1106,  1108,  1110,  1111,  1112,  1114,  1120,  1121,  1121,
    1124,  1125,  1127,  1127,  1128,  1128,  1129
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
  "\"end of file\"", "error", "\"invalid token\"", "T_EndOfFile",
  "T_Return", "T_Number", "T_True", "T_False", "T_ID", "T_Print",
  "T_Range", "T_Cln", "T_NL", "T_EQL", "T_NEQ", "T_EQ", "T_GT", "T_LT",
  "T_EGT", "T_ELT", "T_Or", "T_And", "T_Not", "T_In", "ID", "ND", "DD",
  "T_String", "T_If", "T_Elif", "T_While", "T_For", "T_Else", "T_Break",
  "T_MN", "T_PL", "T_DV", "T_ML", "T_OP", "T_CP", "T_OB", "T_CB", "T_Def",
  "T_Comma", "T_List", "$accept", "StartDebugger", "$@1", "constant",
  "term", "StartParse", "$@2", "basic_stmt", "arith_exp", "bool_exp",
  "bool_term", "bool_factor", "break_stmt", "assign_stmt", "print_stmt",
  "finalStatements", "cmpd_stmt", "if_stmt", "elif_stmts", "else_stmt",
  "while_stmt", "myrange", "for_stmt", "start_suite", "$@3", "suite",
  "end_suite", "$@4", "$@5", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-84)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-67)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -84,    10,   105,   -84,     0,   -84,   -84,   -84,     5,   -15,
     105,    -1,    85,    85,    30,   -84,    20,    85,   -84,   -84,
      28,   -84,   190,   -84,    15,   -84,   -84,   -84,   -84,    40,
     -84,   -84,   -84,   -84,   -84,    85,     6,   -84,    85,   -84,
     -84,   190,    46,    56,    47,    20,    23,   165,    38,   -84,
      85,    20,    20,    20,    20,    20,    20,    20,    20,    85,
      85,    66,   190,   -84,    39,   157,   157,    70,   113,   -84,
     -84,    51,   -84,   136,   136,   136,   136,    23,    23,   -84,
     -84,    51,   -84,   -84,   105,   -84,   -84,   -84,   -10,   -84,
      64,   -84,   123,    85,    83,   -84,   -84,     6,    91,    93,
     157,    72,    77,    67,   -84,   157,   -84,     6,    98,     1,
      91,   -84,   -10,    75,   157,   123,   -84,   -84,   -84,     6,
     -84,   -84,   -84
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     0,     1,     0,     4,    33,    34,     5,     0,
       0,     0,     0,     0,     0,    37,     0,     0,     6,    16,
       0,    41,    13,    14,    29,    30,    11,    12,    15,     0,
      42,    44,    45,    46,    43,     0,     0,     7,     0,    35,
       5,     0,     0,     0,     0,     0,    21,     0,     0,     3,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     8,    38,    39,     0,     0,     0,     0,     0,    22,
      36,    31,    32,    26,    24,    28,    27,    18,    17,    20,
      19,     0,    23,    25,     0,    40,    58,    57,    47,    52,
       0,     9,     0,     0,     0,    48,    49,     0,     0,     0,
       0,    53,     0,     0,    59,     0,    51,     0,     0,    62,
       0,    61,     0,    54,     0,     0,    65,    60,    50,     0,
      56,    63,    55
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -84,   -84,   -84,   -84,   -36,    -3,   -84,   -50,   -11,    -9,
       3,   109,   -84,   -84,   -84,   -83,   -84,   -84,     9,   -84,
     -84,   -84,   -84,   -49,   -84,    12,   -84,   -84,   -84
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,    18,    19,    20,    84,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    95,    96,
      32,   102,    33,    88,    92,   104,   111,   115,   116
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      64,    41,    41,    42,    43,    46,    47,    37,    48,    98,
       3,     5,    34,   -64,    40,    87,    87,    89,    35,    93,
     110,    11,    94,    36,    62,     5,    63,    41,    40,    48,
     -64,    49,   121,   -64,    68,    59,    60,    38,    44,    71,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    81,
      87,   106,    61,    72,    16,    87,   112,    65,    45,    57,
      58,   101,    82,    83,    87,   120,    50,    66,     4,   -10,
      67,   113,     5,     6,     7,     8,     9,    70,    85,   -66,
      90,    91,    41,   122,    99,    55,    56,    57,    58,    11,
       5,     6,     7,    40,   100,    12,   -66,    13,    14,   -66,
      15,    16,    97,   103,   105,    17,     4,    11,   109,   114,
       5,     6,     7,     8,     9,   107,   108,    10,   119,    16,
      39,   118,   117,    17,     4,     0,     0,    11,     5,     6,
       7,     8,     9,    12,     0,    13,    14,     0,    15,    16,
       0,     0,     0,    17,     0,    11,     0,    55,    56,    57,
      58,    12,    69,    13,    14,     0,    15,    16,     0,     0,
       0,    17,     5,     6,     7,     8,     9,     0,     0,    86,
      55,    56,    57,    58,     0,     0,     0,     0,     0,    11,
      50,    51,    52,    53,    54,     0,     0,     0,     0,     0,
      15,    16,     0,     0,     0,    17,     0,     0,     0,    55,
      56,    57,    58,     0,    69,    50,    51,    52,    53,    54,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,    56,    57,    58
};

static const yytype_int8 yycheck[] =
{
      36,    12,    13,    12,    13,    16,    17,    10,    17,    92,
       0,     5,    12,    12,     8,    65,    66,    66,    13,    29,
     103,    22,    32,    38,    35,     5,    35,    38,     8,    38,
      29,     3,   115,    32,    45,    20,    21,    38,     8,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
     100,   100,    12,    50,    34,   105,   105,    11,    38,    36,
      37,    97,    59,    60,   114,   114,    15,    11,     1,     3,
      23,   107,     5,     6,     7,     8,     9,    39,    39,    12,
      10,    84,    93,   119,    93,    34,    35,    36,    37,    22,
       5,     6,     7,     8,    11,    28,    29,    30,    31,    32,
      33,    34,    38,    12,    11,    38,     1,    22,    41,    11,
       5,     6,     7,     8,     9,    43,    39,    12,    43,    34,
      11,   112,   110,    38,     1,    -1,    -1,    22,     5,     6,
       7,     8,     9,    28,    -1,    30,    31,    -1,    33,    34,
      -1,    -1,    -1,    38,    -1,    22,    -1,    34,    35,    36,
      37,    28,    39,    30,    31,    -1,    33,    34,    -1,    -1,
      -1,    38,     5,     6,     7,     8,     9,    -1,    -1,    12,
      34,    35,    36,    37,    -1,    -1,    -1,    -1,    -1,    22,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    -1,    -1,    38,    -1,    -1,    -1,    34,
      35,    36,    37,    -1,    39,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    35,    36,    37
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    46,    47,     0,     1,     5,     6,     7,     8,     9,
      12,    22,    28,    30,    31,    33,    34,    38,    48,    49,
      50,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    65,    67,    12,    13,    38,    50,    38,    56,
       8,    53,    54,    54,     8,    38,    53,    53,    54,     3,
      15,    16,    17,    18,    19,    34,    35,    36,    37,    20,
      21,    12,    53,    54,    49,    11,    11,    23,    53,    39,
      39,    53,    55,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    55,    55,    51,    39,    12,    52,    68,    68,
      10,    50,    69,    29,    32,    63,    64,    38,    60,    54,
      11,    49,    66,    12,    70,    11,    68,    43,    39,    41,
      60,    71,    68,    49,    11,    72,    73,    70,    63,    43,
      68,    60,    49
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    45,    47,    46,    48,    49,    49,    50,    51,    50,
      50,    52,    52,    52,    52,    52,    53,    53,    53,    53,
      53,    53,    53,    54,    54,    54,    54,    54,    54,    54,
      55,    55,    55,    55,    55,    56,    56,    57,    58,    58,
      59,    60,    60,    60,    61,    61,    61,    62,    62,    63,
      63,    64,    65,    66,    66,    66,    67,    68,    69,    68,
      70,    70,    72,    71,    73,    71,    71
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     3,     1,     1,     1,     2,     0,     4,
       2,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     2,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     3,     3,     1,     1,     2,     3,     1,     3,     3,
       4,     1,     1,     2,     1,     1,     1,     4,     5,     1,
       5,     3,     4,     1,     3,     5,     9,     1,     0,     4,
       3,     2,     0,     3,     0,     2,     0
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

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
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
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
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

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yyerror_range[1] = yylloc;
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
  *++yylsp = yylloc;

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* $@1: %empty  */
#line 1039 "translator.y"
                {init();}
#line 2521 "y.tab.c"
    break;

  case 3: /* StartDebugger: $@1 StartParse T_EndOfFile  */
#line 1039 "translator.y"
                                                 {printf("\nValid Python Syntax\n"); printAST((yyvsp[-1].node));
				printf("\n-------------Three-address code---------------\n");
 				codeGenOp((yyvsp[-1].node)); printSTable(); freeAll(); exit(0);}
#line 2529 "y.tab.c"
    break;

  case 4: /* constant: T_Number  */
#line 1043 "translator.y"
                    {insertRecord("Constant", (yyvsp[0].text), (yylsp[0]).first_line, currentScope); (yyval.node) = createID_Const("Constant", (yyvsp[0].text), currentScope);}
#line 2535 "y.tab.c"
    break;

  case 5: /* term: T_ID  */
#line 1045 "translator.y"
            {modifyRecordID("Identifier", (yyvsp[0].text), (yylsp[0]).first_line, currentScope); (yyval.node) = createID_Const("Identifier", (yyvsp[0].text), currentScope);}
#line 2541 "y.tab.c"
    break;

  case 6: /* term: constant  */
#line 1046 "translator.y"
                {(yyval.node) = (yyvsp[0].node);}
#line 2547 "y.tab.c"
    break;

  case 7: /* StartParse: T_NL StartParse  */
#line 1050 "translator.y"
                             {(yyval.node)=(yyvsp[0].node);}
#line 2553 "y.tab.c"
    break;

  case 8: /* $@2: %empty  */
#line 1050 "translator.y"
                                                            {resetDepth();}
#line 2559 "y.tab.c"
    break;

  case 9: /* StartParse: finalStatements T_NL $@2 StartParse  */
#line 1050 "translator.y"
                                                                                       {(yyval.node) = createOp("NewLine", 2, (yyvsp[-3].node), (yyvsp[0].node));}
#line 2565 "y.tab.c"
    break;

  case 10: /* StartParse: finalStatements T_NL  */
#line 1050 "translator.y"
                                                                                                                                                    {(yyval.node)=(yyvsp[-1].node);}
#line 2571 "y.tab.c"
    break;

  case 11: /* basic_stmt: break_stmt  */
#line 1052 "translator.y"
                        {(yyval.node)=(yyvsp[0].node);}
#line 2577 "y.tab.c"
    break;

  case 12: /* basic_stmt: assign_stmt  */
#line 1053 "translator.y"
                         {(yyval.node)=(yyvsp[0].node);}
#line 2583 "y.tab.c"
    break;

  case 13: /* basic_stmt: arith_exp  */
#line 1054 "translator.y"
                       {(yyval.node)=(yyvsp[0].node);}
#line 2589 "y.tab.c"
    break;

  case 14: /* basic_stmt: bool_exp  */
#line 1055 "translator.y"
                      {(yyval.node)=(yyvsp[0].node);}
#line 2595 "y.tab.c"
    break;

  case 15: /* basic_stmt: print_stmt  */
#line 1056 "translator.y"
                        {(yyval.node)=(yyvsp[0].node);}
#line 2601 "y.tab.c"
    break;

  case 16: /* arith_exp: term  */
#line 1058 "translator.y"
                 {(yyval.node)=(yyvsp[0].node);}
#line 2607 "y.tab.c"
    break;

  case 17: /* arith_exp: arith_exp T_PL arith_exp  */
#line 1059 "translator.y"
                                       {(yyval.node) = createOp("+", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2613 "y.tab.c"
    break;

  case 18: /* arith_exp: arith_exp T_MN arith_exp  */
#line 1060 "translator.y"
                                       {(yyval.node) = createOp("-", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2619 "y.tab.c"
    break;

  case 19: /* arith_exp: arith_exp T_ML arith_exp  */
#line 1061 "translator.y"
                                       {(yyval.node) = createOp("*", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2625 "y.tab.c"
    break;

  case 20: /* arith_exp: arith_exp T_DV arith_exp  */
#line 1062 "translator.y"
                                       {(yyval.node) = createOp("/", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2631 "y.tab.c"
    break;

  case 21: /* arith_exp: T_MN arith_exp  */
#line 1063 "translator.y"
                           {(yyval.node) = createOp("-", 1, (yyvsp[0].node));}
#line 2637 "y.tab.c"
    break;

  case 22: /* arith_exp: T_OP arith_exp T_CP  */
#line 1064 "translator.y"
                                {(yyval.node) = (yyvsp[-1].node);}
#line 2643 "y.tab.c"
    break;

  case 23: /* bool_exp: bool_term T_Or bool_term  */
#line 1067 "translator.y"
                                    {(yyval.node) = createOp("or", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2649 "y.tab.c"
    break;

  case 24: /* bool_exp: arith_exp T_LT arith_exp  */
#line 1068 "translator.y"
                                    {(yyval.node) = createOp("<", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2655 "y.tab.c"
    break;

  case 25: /* bool_exp: bool_term T_And bool_term  */
#line 1069 "translator.y"
                                     {(yyval.node) = createOp("and", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2661 "y.tab.c"
    break;

  case 26: /* bool_exp: arith_exp T_GT arith_exp  */
#line 1070 "translator.y"
                                    {(yyval.node) = createOp(">", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2667 "y.tab.c"
    break;

  case 27: /* bool_exp: arith_exp T_ELT arith_exp  */
#line 1071 "translator.y"
                                     {(yyval.node) = createOp("<=", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2673 "y.tab.c"
    break;

  case 28: /* bool_exp: arith_exp T_EGT arith_exp  */
#line 1072 "translator.y"
                                     {(yyval.node) = createOp(">=", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2679 "y.tab.c"
    break;

  case 29: /* bool_exp: bool_term  */
#line 1073 "translator.y"
                     {(yyval.node)=(yyvsp[0].node);}
#line 2685 "y.tab.c"
    break;

  case 30: /* bool_term: bool_factor  */
#line 1075 "translator.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 2691 "y.tab.c"
    break;

  case 31: /* bool_term: arith_exp T_EQ arith_exp  */
#line 1076 "translator.y"
                                     {(yyval.node) = createOp("==", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2697 "y.tab.c"
    break;

  case 32: /* bool_term: arith_exp T_EQ bool_term  */
#line 1077 "translator.y"
                                             {(yyval.node) = createOp("==", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2703 "y.tab.c"
    break;

  case 33: /* bool_term: T_True  */
#line 1078 "translator.y"
                   {insertRecord("Constant", "True", (yylsp[0]).first_line, currentScope); (yyval.node) = createID_Const("Constant", "True", currentScope);}
#line 2709 "y.tab.c"
    break;

  case 34: /* bool_term: T_False  */
#line 1079 "translator.y"
                    {insertRecord("Constant", "False", (yylsp[0]).first_line, currentScope); (yyval.node) = createID_Const("Constant", "False", currentScope);}
#line 2715 "y.tab.c"
    break;

  case 35: /* bool_factor: T_Not bool_factor  */
#line 1081 "translator.y"
                                {(yyval.node) = createOp("!", 1, (yyvsp[0].node));}
#line 2721 "y.tab.c"
    break;

  case 36: /* bool_factor: T_OP bool_exp T_CP  */
#line 1082 "translator.y"
                                 {(yyval.node) = (yyvsp[-1].node);}
#line 2727 "y.tab.c"
    break;

  case 37: /* break_stmt: T_Break  */
#line 1084 "translator.y"
                     {(yyval.node) = createOp("break", 0);}
#line 2733 "y.tab.c"
    break;

  case 38: /* assign_stmt: T_ID T_EQL arith_exp  */
#line 1086 "translator.y"
                                   {insertRecord("Identifier", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope); (yyval.node) = createOp("=", 2, createID_Const("Identifier", (yyvsp[-2].text), currentScope), (yyvsp[0].node));}
#line 2739 "y.tab.c"
    break;

  case 39: /* assign_stmt: T_ID T_EQL bool_exp  */
#line 1087 "translator.y"
                                  {insertRecord("Identifier", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope);(yyval.node) = createOp("=", 2, createID_Const("Identifier", (yyvsp[-2].text), currentScope), (yyvsp[0].node));}
#line 2745 "y.tab.c"
    break;

  case 40: /* print_stmt: T_Print T_OP term T_CP  */
#line 1089 "translator.y"
                                    {(yyval.node) = createOp("Print", 1, (yyvsp[-1].node));}
#line 2751 "y.tab.c"
    break;

  case 41: /* finalStatements: basic_stmt  */
#line 1091 "translator.y"
                             {(yyval.node) = (yyvsp[0].node);}
#line 2757 "y.tab.c"
    break;

  case 42: /* finalStatements: cmpd_stmt  */
#line 1092 "translator.y"
                            {(yyval.node) = (yyvsp[0].node);}
#line 2763 "y.tab.c"
    break;

  case 43: /* finalStatements: error T_NL  */
#line 1093 "translator.y"
                             {yyerrok; yyclearin; (yyval.node)=createOp("SyntaxError", 0);}
#line 2769 "y.tab.c"
    break;

  case 44: /* cmpd_stmt: if_stmt  */
#line 1095 "translator.y"
                    {(yyval.node) = (yyvsp[0].node);}
#line 2775 "y.tab.c"
    break;

  case 45: /* cmpd_stmt: while_stmt  */
#line 1096 "translator.y"
                       {(yyval.node) = (yyvsp[0].node);}
#line 2781 "y.tab.c"
    break;

  case 46: /* cmpd_stmt: for_stmt  */
#line 1097 "translator.y"
                             {(yyval.node) = (yyvsp[0].node);}
#line 2787 "y.tab.c"
    break;

  case 47: /* if_stmt: T_If bool_exp T_Cln start_suite  */
#line 1100 "translator.y"
                                          {(yyval.node) = createOp("If", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2793 "y.tab.c"
    break;

  case 48: /* if_stmt: T_If bool_exp T_Cln start_suite elif_stmts  */
#line 1101 "translator.y"
                                                     {(yyval.node) = createOp("If", 3, (yyvsp[-3].node), (yyvsp[-1].node), (yyvsp[0].node));}
#line 2799 "y.tab.c"
    break;

  case 49: /* elif_stmts: else_stmt  */
#line 1103 "translator.y"
                       {(yyval.node)= (yyvsp[0].node);}
#line 2805 "y.tab.c"
    break;

  case 50: /* elif_stmts: T_Elif bool_exp T_Cln start_suite elif_stmts  */
#line 1104 "translator.y"
                                                          {(yyval.node)= createOp("Elif", 3, (yyvsp[-3].node), (yyvsp[-1].node), (yyvsp[0].node));}
#line 2811 "y.tab.c"
    break;

  case 51: /* else_stmt: T_Else T_Cln start_suite  */
#line 1106 "translator.y"
                                     {(yyval.node) = createOp("Else", 1, (yyvsp[0].node));}
#line 2817 "y.tab.c"
    break;

  case 52: /* while_stmt: T_While bool_exp T_Cln start_suite  */
#line 1108 "translator.y"
                                                {(yyval.node) = createOp("While", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2823 "y.tab.c"
    break;

  case 56: /* for_stmt: T_For T_ID T_In T_Range T_OP myrange T_CP T_Cln start_suite  */
#line 1114 "translator.y"
                                                                       {node* bool_node;bool_node=(node*)calloc(1, sizeof(node));
																		insertRecord("Identifier", (yyvsp[-7].text), (yylsp[-8]).first_line, currentScope);
																		node* id = createID_Const("Identifier", (yyvsp[-7].text), currentScope);
																		bool_node=createOp("<", 2, id, (yyvsp[-3].node));
																		(yyval.node) = createOp("For", 4, createOp("<", 2, id, (yyvsp[-3].node)), id, (yyvsp[-3].node), (yyvsp[0].node));}
#line 2833 "y.tab.c"
    break;

  case 57: /* start_suite: basic_stmt  */
#line 1120 "translator.y"
                         {(yyval.node) = (yyvsp[0].node);}
#line 2839 "y.tab.c"
    break;

  case 58: /* $@3: %empty  */
#line 1121 "translator.y"
                   {initNewTable(scope_depth); updateCScope(scope_depth);}
#line 2845 "y.tab.c"
    break;

  case 59: /* start_suite: T_NL $@3 finalStatements suite  */
#line 1121 "translator.y"
                                                                                                 {(yyval.node) = createOp("BeginBlock", 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 2851 "y.tab.c"
    break;

  case 60: /* suite: T_NL finalStatements suite  */
#line 1124 "translator.y"
                                   {(yyval.node) = createOp("Next", 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 2857 "y.tab.c"
    break;

  case 61: /* suite: T_NL end_suite  */
#line 1125 "translator.y"
                       {(yyval.node) = (yyvsp[0].node);}
#line 2863 "y.tab.c"
    break;

  case 62: /* $@4: %empty  */
#line 1127 "translator.y"
                 {updateCScope(scope_depth);}
#line 2869 "y.tab.c"
    break;

  case 63: /* end_suite: T_CB $@4 finalStatements  */
#line 1127 "translator.y"
                                                              {(yyval.node) = createOp("EndBlock", 1, (yyvsp[0].node));}
#line 2875 "y.tab.c"
    break;

  case 64: /* $@5: %empty  */
#line 1128 "translator.y"
                 {updateCScope(scope_depth);}
#line 2881 "y.tab.c"
    break;

  case 65: /* end_suite: T_CB $@5  */
#line 1128 "translator.y"
                                              {(yyval.node) = createOp("EndBlock", 0);}
#line 2887 "y.tab.c"
    break;

  case 66: /* end_suite: %empty  */
#line 1129 "translator.y"
            {(yyval.node) = createOp("EndBlock", 0); resetDepth();}
#line 2893 "y.tab.c"
    break;


#line 2897 "y.tab.c"

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
  *++yylsp = yyloc;

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

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc);
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1132 "translator.y"




int main()
{
	//printf("Enter the Expression\n");
	yyparse();
	return 0;
}
