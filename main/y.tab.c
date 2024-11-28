/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 2
#define YYMINOR 0
#define YYPATCH 20230521

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#undef YYBTYACC
#define YYBTYACC 0
#define YYDEBUGSTR YYPREFIX "debug"
#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "translator.y"
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

	/* Original function definition*/
	/* void yyerror(const char *msg)*/

	/* Updated function definition*/
	void yyerror(YYLTYPE *yylloc, const char *msg) {
		fprintf(stderr, "Error at line %d: %s\n", yylloc->first_line, msg);
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

			/* id assigning to zero*/
			printf("T%d = 0\n", opNode->nodeNo);
			int iteration_condition_temp_no = opNode->NextLevel[0]->NextLevel[0]->nodeNo;
			makeQ(makeStr(iteration_condition_temp_no, 1), "0", "-", "=");
			printf("%s = T%d\n", opNode->NextLevel[1]->id->name, iteration_condition_temp_no);
			char *id_name = opNode->NextLevel[1]->id->name;
			makeQ(opNode->NextLevel[1]->id->name, makeStr(iteration_condition_temp_no, 1), "-", "=");		

			/* evaluating boolean expression*/
			printf("L%d:\n",  lIndex);
			makeQ(makeStr(temp, 0), "-", "-", "Label");
			codeGenOp(opNode->NextLevel[0]);
			printf("If False T%d goto L%d\n", opNode->NextLevel[0]->nodeNo, lIndex+1);		
			makeQ(makeStr(temp+1, 0), makeStr(opNode->NextLevel[0]->nodeNo, 1), "-", "If False");
			
			lIndex+=2;
			/* start suite 			*/
			codeGenOp(opNode->NextLevel[3]);

			/* increment operation*/
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

			/* iteration*/
			printf("goto L%d\n", temp);
			makeQ(makeStr(temp, 0), "-", "-", "goto");

			/* end of iteration*/
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
		/*printf("FR: %d, %s\n", scope, name);*/
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
      /*ASCII value of 0 = 48, 9 = 57. So if value is outside of numeric range then fail*/
      /*Checking for negative sign "-" could be added: ASCII value 45.*/
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
		/*Abort if no string is passed*/
		if(String == NULL) return 0;
	
		while((*String) != '\0')
		{
			if(isalpha(*String)) return 1;
			String++;
		}
		return 0;
	}

	

	char* check_data_type(char* data){
		/* it's a number*/
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
			/* printf("%s-%s\n", records[i].name, name);*/
			if(!strcmp(records[i].name, name)){
				/* printf("type: %s\n", records[i].type);*/
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
	/* generate c output code*/
	void generateCCode(){
		FILE *file = fopen("output.c", "w");
		if (file == NULL){   
			printf("Error! Could not open file\n"); 
			exit(-1); /* must include stdlib.h */
		} 
		fprintf(file, "//Auto Generated C Code. Call the c_code() function inside your main() Function.\n//It can be included by either linking or defining.\n//Include all preprocessors into your code\n"); /* write to file*/
		/* write main*/
		fprintf(file, "#include <stdio.h>\n#define True 1\n#define False 0\n");
		/* Declarations*/
		fprintf(file, "void c_code();\n");
		/* Main*/
		fprintf(file, "int main(){\n");
		fprintf(file, "\tc_code();\n}\n");
		/* c_code() Definition*/
		printf("\n--------------------------------C-Code---------------------------------\n");
		fprintf(file, "void c_code(){\n");
		printf("void c_code(){\n");
		int i;
		for(i=0; i<qIndex; i++)
		{
			if(allQ[i].I > -1){
				/* right value is an id*/
				if(strstr(allQ[i].Op, "goto")){
					fprintf(file, "\tgoto %s;\n", allQ[i].R);
					printf("\tgoto %s;\n", allQ[i].R);
				}
				
				else if(strstr(allQ[i].Op, "Label")){
					/* dead label elimination*/
					if(check_label(allQ[i].R)){
						labels[label_counter] = (char *)malloc(strlen(allQ[i].R)+1);
						labels[label_counter] = allQ[i].R;
						label_counter++;
						fprintf(file, "%s: ;\n",  allQ[i].R);
						printf("%s: ;\n",  allQ[i].R);
					}
					/* fprintf(file, "%s:\n",  allQ[i].R);*/
					/* printf("%s:\n",  allQ[i].R);*/
				}
				else if(strstr(allQ[i].Op, "If")){
					if(!strcmp("If False", allQ[i].Op)){
						/* if(find_index_of_condition(allQ[i].A1)){*/
						/* 	int idx = find_index_of_condition(allQ[i].A1);*/
						/* 	// fprintf(file, "\t%s %s %s;\n", allQ[idx].A1, allQ[idx].Op, allQ[idx].A2);*/
						/* 	// printf("\t%s %s %s;\n", allQ[idx].A1, allQ[idx].Op, allQ[idx].A2);*/
						/* 	fprintf(file, "\tif(!(%s %s %s)){\n\t\tgoto %s;\n\t}\n", allQ[idx].A1, allQ[idx].Op, allQ[idx].A2,  allQ[i].R);*/
						/* 	printf("\tif(!(%s %s %s)){\n\t\tgoto %s;\n\t}\n", allQ[idx].A1, allQ[idx].Op, allQ[idx].A2,  allQ[i].R);*/
						/* }*/
						fprintf(file, "\tif(!%s){\n\t\tgoto %s;\n\t}\n", allQ[i].A1,  allQ[i].R);
						printf("\tif(!%s){\n\t\tgoto %s;\n\t}\n", allQ[i].A1,  allQ[i].R);		
					}
					else if(!strcmp("If True", allQ[i].Op)){
						fprintf(file, "\tif(%s){\n\t\tgoto %s;\n\t}\n", allQ[i].A1,  allQ[i].R);
						printf("\tif(%s){\n\t\tgoto %s;\n\t}\n", allQ[i].A1,  allQ[i].R);		
					}
				}
				/* other ops such as "=" */
				else{
					/* printf("\t%s\t%s\t%s\t%s\n",   allQ[i].Op, allQ[i].A1, allQ[i].A2, allQ[i].R);*/
					/* if rl has id*/
					if(check_data_type(allQ[i].A1) == "id"){
						/* one left value operation*/
						if(strchr(allQ[i].A2, '-')){
							char* type;
							if(find_id_data_type(allQ[i].A1) != NULL){
								
								type = find_id_data_type(allQ[i].A1);
								/* printf("type: %s\n", type);*/
							}
							/* create a record for left id if it is not existed */
							if(find_id_data_type(allQ[i].R)){
								/* reassign the variable*/
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
								/* printf("type: %s\n", lr.type);*/
								records[id_counter] = lr;
								id_counter++;
								/* define the variable*/
								fprintf(file, "\t%s %s %s %s;\n", type, allQ[i].R, allQ[i].Op, allQ[i].A1);
								printf("\t%s %s %s %s;\n", type, allQ[i].R, allQ[i].Op, allQ[i].A1);
							}	
						}
						else{
							char* type;
							if(find_id_data_type(allQ[i].A1)){
								type = find_id_data_type(allQ[i].A1);
							}
							/* create a record for left id if it is not existed */
							if(find_id_data_type(allQ[i].R)){
								/* reassign the variable*/
								fprintf(file, "\t%s = %s %s %s;\n", allQ[i].R, allQ[i].A1, allQ[i].Op, allQ[i].A2);
								printf("\t%s  = %s %s %s;\n", allQ[i].R, allQ[i].A1, allQ[i].Op, allQ[i].A2);
							}
							else{
								record lr;
								lr.name = allQ[i].R;
								lr.type = type;
								records[id_counter] = lr;
								id_counter++;
								/* define the variable*/
								fprintf(file, "\t%s %s = %s %s %s;\n", type, allQ[i].R, allQ[i].A1, allQ[i].Op, allQ[i].A2);
								printf("\t%s %s  = %s %s %s;\n", type, allQ[i].R, allQ[i].A1, allQ[i].Op, allQ[i].A2);
							}
						}
						/* fprintf("\t%s\t%s\t%s\t%s\n",   allQ[i].Op, allQ[i].A1, allQ[i].A2, allQ[i].R);*/
					}
					/* if rl is a number or a string*/
					else {
						
						if(strchr(allQ[i].A2, '-')){
							char* type = check_data_type(allQ[i].A1);						
							if(find_id_data_type(allQ[i].R) != NULL){
								/* reassign the variable*/
								fprintf(file, "\t%s %s %s;\n", allQ[i].R, allQ[i].Op, allQ[i].A1);
								printf("\t%s %s %s;\n", allQ[i].R, allQ[i].Op, allQ[i].A1);
							}
							else{								
								records[id_counter].name = (char *)malloc(strlen(allQ[i].R)+1);
								records[id_counter].type = (char *)malloc(strlen(type)+1);
								strcpy(records[id_counter].name, allQ[i].R);
								strcpy(records[id_counter].type, type);
								id_counter++;
								/* define the variable*/
								fprintf(file, "\t%s %s %s %s;\n", type, allQ[i].R, allQ[i].Op, allQ[i].A1);
								printf("\t%s %s %s %s;\n", type, allQ[i].R, allQ[i].Op, allQ[i].A1);
							}	
						}
						else{
							char* type;
							if(find_id_data_type(allQ[i].A1)){
								type = find_id_data_type(allQ[i].A1);
							}
							/* create a record for left id if it is not existed */
							if(find_id_data_type(allQ[i].R)){
								/* reassign the variable*/
								fprintf(file, "\t%s = %s %s %s;\n", allQ[i].R, allQ[i].A1, allQ[i].Op, allQ[i].A2);
								printf("\t%s  = %s %s %s;\n", allQ[i].R, allQ[i].A1, allQ[i].Op, allQ[i].A2);
							}
							else{
								/* printf("[DEBUG] saving into table");*/
								record lr;
								lr.name = allQ[i].R;
								lr.type = type;
								records[id_counter] = lr;
								id_counter++;
								/* define the variable*/
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
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#line 1025 "translator.y"
typedef union YYSTYPE { char *text; int depth; int scope_depth; struct ASTNode* node;} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 1055 "y.tab.c"

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
/* Default: YYLTYPE is the text position type. */
typedef struct YYLTYPE
{
    int first_line;
    int first_column;
    int last_line;
    int last_column;
    unsigned source;
} YYLTYPE;
#define YYLTYPE_IS_DECLARED 1
#endif
#define YYRHSLOC(rhs, k) ((rhs)[k])

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

#if !(defined(yylex) || defined(YYSTATE))
int YYLEX_DECL();
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(YYLTYPE *loc, const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(&yylloc, msg)
#endif

extern int YYPARSE_DECL();

#define T_EndOfFile 257
#define T_Return 258
#define T_Number 259
#define T_True 260
#define T_False 261
#define T_ID 262
#define T_Print 263
#define T_Range 264
#define T_Cln 265
#define T_NL 266
#define T_EQL 267
#define T_NEQ 268
#define T_EQ 269
#define T_GT 270
#define T_LT 271
#define T_EGT 272
#define T_ELT 273
#define T_Or 274
#define T_And 275
#define T_Not 276
#define T_In 277
#define ID 278
#define ND 279
#define DD 280
#define T_String 281
#define T_If 282
#define T_Elif 283
#define T_While 284
#define T_For 285
#define T_Else 286
#define T_Break 287
#define T_MN 288
#define T_PL 289
#define T_DV 290
#define T_ML 291
#define T_OP 292
#define T_CP 293
#define T_OB 294
#define T_CB 295
#define T_Def 296
#define T_Comma 297
#define T_List 298
#define YYERRCODE 256
typedef int YYINT;
static const YYINT yylhs[] = {                           -1,
   23,    0,    9,    8,    8,    4,   24,    4,    4,   10,
   10,   10,   10,   10,    6,    6,    6,    6,    6,    6,
    6,    7,    7,    7,    7,    7,    7,    7,   21,   21,
   21,   21,   21,   22,   22,   12,   20,   20,   13,    5,
    5,    5,   11,   11,   11,   14,   14,   15,   15,   16,
   17,   19,   19,   19,   18,    1,   25,    1,    2,    2,
   26,    3,   27,    3,    3,
};
static const YYINT yylen[] = {                            2,
    0,    3,    1,    1,    1,    2,    0,    4,    2,    1,
    1,    1,    1,    1,    1,    3,    3,    3,    3,    2,
    3,    3,    3,    3,    3,    3,    3,    1,    1,    3,
    3,    1,    1,    2,    3,    1,    3,    3,    4,    1,
    1,    2,    1,    1,    1,    4,    5,    1,    5,    3,
    4,    1,    3,    5,    9,    1,    0,    4,    3,    2,
    0,    3,    0,    2,    0,
};
static const YYINT yydefred[] = {                         1,
    0,    0,    0,    3,   32,   33,    0,    0,    0,    0,
    0,    0,    0,   36,    0,    0,    0,    0,    0,   13,
   15,    5,   40,   41,   10,   14,   43,   44,   45,   11,
    0,   29,   42,    0,    0,    6,    0,   34,    4,    0,
    0,    0,    0,    0,    0,    0,    0,    2,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   38,    0,    0,    0,    0,    0,   21,   35,    0,
    0,   31,    0,    0,    0,    0,    0,    0,   19,   18,
    0,   22,   24,   39,   57,    0,   56,   51,    0,    8,
    0,    0,    0,   47,   48,    0,    0,    0,    0,    0,
    0,    0,   58,    0,   50,    0,    0,    0,   60,    0,
    0,    0,    0,    0,   64,   59,   49,    0,   55,   62,
   54,
};
#if defined(YYDESTRUCT_CALL) || defined(YYSTYPE_TOSTRING)
static const YYINT yystos[] = {                           0,
  300,  323,  256,  259,  260,  261,  262,  263,  266,  276,
  282,  284,  285,  287,  288,  292,  304,  305,  306,  307,
  308,  309,  310,  311,  312,  313,  314,  317,  318,  320,
  321,  322,  266,  267,  292,  304,  292,  322,  262,  306,
  307,  307,  262,  292,  306,  306,  307,  257,  266,  269,
  270,  271,  272,  273,  288,  289,  290,  291,  274,  275,
  306,  307,  308,  265,  265,  277,  306,  293,  293,  324,
  306,  321,  306,  306,  306,  306,  306,  306,  306,  306,
  306,  321,  321,  293,  266,  301,  310,  301,  264,  304,
  325,  283,  286,  315,  316,  292,  305,  307,  265,  308,
  319,  266,  302,  265,  301,  297,  293,  295,  303,  305,
  301,  308,  265,  326,  327,  302,  315,  297,  301,  305,
  308,
};
#endif /* YYDESTRUCT_CALL || YYSTYPE_TOSTRING */
static const YYINT yydgoto[] = {                          1,
   86,  103,  109,   17,   18,   19,   20,   21,   22,   23,
   24,   25,   26,   27,   94,   95,   28,   29,  101,   30,
   31,   32,    2,   70,   91,  114,  115,
};
static const YYINT yysindex[] = {                         0,
    0,  -82, -244,    0,    0,    0, -229, -237,  -82, -255,
 -174, -174, -209,    0, -234, -174, -178, -196,   90,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -265,    0,    0, -174, -239,    0, -174,    0,    0,   90,
 -184, -166, -170, -234, -227, -258, -176,    0,    0, -174,
 -234, -234, -234, -234, -234, -234, -234, -234, -174, -174,
   90,    0, -173,  -30,  -30, -149, -197,    0,    0,  -82,
  -50,    0, -137, -137, -137, -137, -227, -227,    0,    0,
  -50,    0,    0,    0,    0, -224,    0,    0, -169,    0,
  -64, -174, -143,    0,    0, -239, -141, -138,  -30, -167,
 -145, -187,    0,  -30,    0, -239, -110,    0,    0, -141,
 -224, -148,  -30,  -64,    0,    0,    0, -239,    0,    0,
    0,
};
static const YYINT yyrindex[] = {                         0,
    0,    0,    0,    0,    0,    0,   85,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, -247,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -162,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  -22,    0,    0,    0, -116,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -206,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   44,    0,  -79,   22,   57,   59,    3,   28,    0,    0,
    0,    0,    0,    0,    0, -109,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -135,
    0, -127,    0,    0,    0,    0,    0, -150,    0,    0,
    0, -132,    0,    0,    0,    0,    0,    0,    0,    0,
    0,
};
#if YYBTYACC
static const YYINT yycindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,
};
#endif
static const YYINT yygindex[] = {                         0,
  -47,   52,    0,   -2,  -24,  -10,   -8,  -35,    0,  -48,
    0,    0,    0,    0,   53,    0,    0,    0,    0,    0,
   69,  153,    0,    0,    0,    0,    0,
};
#define YYTABLESIZE 381
static const YYINT yytable[] = {                         63,
   40,   40,   41,   42,   45,   46,   36,   47,   59,   60,
   50,   51,   52,   53,   54,   87,   87,   88,   12,    4,
   10,   33,   39,   61,    4,   62,   40,   39,   47,   55,
   56,   57,   58,   67,   68,   12,   37,   34,   12,   71,
   73,   74,   75,   76,   77,   78,   79,   80,   81,   81,
   87,  105,   43,   15,   35,   87,  111,   44,   92,   37,
  100,   93,   57,   58,   87,  119,   97,   90,    3,   49,
  112,    4,    5,    6,    7,    8,   37,  110,   48,   37,
   64,   40,  121,   98,    4,    5,    6,   39,   10,  120,
   55,   56,   57,   58,   11,   68,   12,   13,   65,   14,
   15,   10,   28,   28,   16,   61,   66,  108,   61,   61,
   61,   61,   61,   15,   89,   63,   69,   16,   72,   84,
   28,   99,   96,   28,  102,   61,  104,   82,   83,  106,
   28,   61,   63,   61,   61,   63,   61,   61,   65,    7,
    9,   61,    7,    7,    7,    7,    7,  107,  118,    7,
   55,   56,   57,   58,  113,   65,   46,   52,   65,    7,
   53,  116,   38,  117,    0,    7,    0,    7,    7,    0,
    7,    7,    0,    3,    0,    7,    4,    5,    6,    7,
    8,    0,    0,    9,    0,   25,   25,    0,    0,    0,
    0,    3,    0,   10,    4,    5,    6,    7,    8,   11,
    0,   12,   13,   25,   14,   15,   25,    0,    0,   16,
    0,   10,    0,   25,    0,    0,    0,   11,   50,   12,
   13,    0,   14,   15,    0,    0,    0,   16,    4,    5,
    6,    7,    8,    0,    0,   85,    0,   55,   56,   57,
   58,    0,   20,   20,    0,   10,   20,   20,   20,   20,
   20,   20,   20,    0,    0,    0,   14,   15,    0,    0,
   20,   16,    0,   20,    0,   20,   20,   17,   17,    0,
   20,   17,   17,   17,   17,   17,   17,   17,    0,    0,
    0,    0,    0,    0,    0,   17,   23,   23,   17,    0,
   17,   17,   16,   16,    0,   17,   16,   16,   16,   16,
   16,   16,   16,    0,   23,    0,    0,   23,   30,   30,
   16,    0,    0,   16,   23,   16,   16,   30,   30,    0,
   16,   27,   27,   26,   26,    0,   30,    0,    0,   30,
    0,    0,    0,    0,    0,    0,   30,    0,    0,   27,
    0,   26,   27,    0,   26,    0,    0,    0,    0,   27,
    4,   26,    0,    4,    4,    4,    4,    4,   50,   51,
   52,   53,   54,    0,    0,    0,    0,    4,    0,    0,
    4,    0,    4,    4,    4,    4,    0,   55,   56,   57,
   58,
};
static const YYINT yycheck[] = {                         35,
   11,   12,   11,   12,   15,   16,    9,   16,  274,  275,
  269,  270,  271,  272,  273,   64,   65,   65,  266,  259,
  276,  266,  262,   34,  259,   34,   37,  262,   37,  288,
  289,  290,  291,   44,  293,  283,  292,  267,  286,   50,
   51,   52,   53,   54,   55,   56,   57,   58,   59,   60,
   99,   99,  262,  288,  292,  104,  104,  292,  283,  266,
   96,  286,  290,  291,  113,  113,   91,   70,  256,  266,
  106,  259,  260,  261,  262,  263,  283,  102,  257,  286,
  265,   92,  118,   92,  259,  260,  261,  262,  276,  114,
  288,  289,  290,  291,  282,  293,  284,  285,  265,  287,
  288,  276,  265,  266,  292,  256,  277,  295,  259,  260,
  261,  262,  263,  288,  264,  266,  293,  292,   50,  293,
  283,  265,  292,  286,  266,  276,  265,   59,   60,  297,
  293,  282,  283,  284,  285,  286,  287,  288,  266,  256,
  257,  292,  259,  260,  261,  262,  263,  293,  297,  266,
  288,  289,  290,  291,  265,  283,  266,  293,  286,  276,
  293,  110,   10,  111,   -1,  282,   -1,  284,  285,   -1,
  287,  288,   -1,  256,   -1,  292,  259,  260,  261,  262,
  263,   -1,   -1,  266,   -1,  265,  266,   -1,   -1,   -1,
   -1,  256,   -1,  276,  259,  260,  261,  262,  263,  282,
   -1,  284,  285,  283,  287,  288,  286,   -1,   -1,  292,
   -1,  276,   -1,  293,   -1,   -1,   -1,  282,  269,  284,
  285,   -1,  287,  288,   -1,   -1,   -1,  292,  259,  260,
  261,  262,  263,   -1,   -1,  266,   -1,  288,  289,  290,
  291,   -1,  265,  266,   -1,  276,  269,  270,  271,  272,
  273,  274,  275,   -1,   -1,   -1,  287,  288,   -1,   -1,
  283,  292,   -1,  286,   -1,  288,  289,  265,  266,   -1,
  293,  269,  270,  271,  272,  273,  274,  275,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  283,  265,  266,  286,   -1,
  288,  289,  265,  266,   -1,  293,  269,  270,  271,  272,
  273,  274,  275,   -1,  283,   -1,   -1,  286,  265,  266,
  283,   -1,   -1,  286,  293,  288,  289,  274,  275,   -1,
  293,  265,  266,  265,  266,   -1,  283,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,   -1,  293,   -1,   -1,  283,
   -1,  283,  286,   -1,  286,   -1,   -1,   -1,   -1,  293,
  266,  293,   -1,  269,  270,  271,  272,  273,  269,  270,
  271,  272,  273,   -1,   -1,   -1,   -1,  283,   -1,   -1,
  286,   -1,  288,  289,  290,  291,   -1,  288,  289,  290,
  291,
};
#if YYBTYACC
static const YYINT yyctable[] = {                        -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
};
#endif
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 298
#define YYUNDFTOKEN 328
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const yyname[] = {

"$end",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"error","T_EndOfFile","T_Return",
"T_Number","T_True","T_False","T_ID","T_Print","T_Range","T_Cln","T_NL","T_EQL",
"T_NEQ","T_EQ","T_GT","T_LT","T_EGT","T_ELT","T_Or","T_And","T_Not","T_In","ID",
"ND","DD","T_String","T_If","T_Elif","T_While","T_For","T_Else","T_Break",
"T_MN","T_PL","T_DV","T_ML","T_OP","T_CP","T_OB","T_CB","T_Def","T_Comma",
"T_List","$accept","StartDebugger","start_suite","suite","end_suite",
"StartParse","finalStatements","arith_exp","bool_exp","term","constant",
"basic_stmt","cmpd_stmt","break_stmt","print_stmt","if_stmt","elif_stmts",
"else_stmt","while_stmt","for_stmt","myrange","assign_stmt","bool_term",
"bool_factor","$$1","$$2","$$3","$$4","$$5","illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : StartDebugger",
"$$1 :",
"StartDebugger : $$1 StartParse T_EndOfFile",
"constant : T_Number",
"term : T_ID",
"term : constant",
"StartParse : T_NL StartParse",
"$$2 :",
"StartParse : finalStatements T_NL $$2 StartParse",
"StartParse : finalStatements T_NL",
"basic_stmt : break_stmt",
"basic_stmt : assign_stmt",
"basic_stmt : arith_exp",
"basic_stmt : bool_exp",
"basic_stmt : print_stmt",
"arith_exp : term",
"arith_exp : arith_exp T_PL arith_exp",
"arith_exp : arith_exp T_MN arith_exp",
"arith_exp : arith_exp T_ML arith_exp",
"arith_exp : arith_exp T_DV arith_exp",
"arith_exp : T_MN arith_exp",
"arith_exp : T_OP arith_exp T_CP",
"bool_exp : bool_term T_Or bool_term",
"bool_exp : arith_exp T_LT arith_exp",
"bool_exp : bool_term T_And bool_term",
"bool_exp : arith_exp T_GT arith_exp",
"bool_exp : arith_exp T_ELT arith_exp",
"bool_exp : arith_exp T_EGT arith_exp",
"bool_exp : bool_term",
"bool_term : bool_factor",
"bool_term : arith_exp T_EQ arith_exp",
"bool_term : arith_exp T_EQ bool_term",
"bool_term : T_True",
"bool_term : T_False",
"bool_factor : T_Not bool_factor",
"bool_factor : T_OP bool_exp T_CP",
"break_stmt : T_Break",
"assign_stmt : T_ID T_EQL arith_exp",
"assign_stmt : T_ID T_EQL bool_exp",
"print_stmt : T_Print T_OP term T_CP",
"finalStatements : basic_stmt",
"finalStatements : cmpd_stmt",
"finalStatements : error T_NL",
"cmpd_stmt : if_stmt",
"cmpd_stmt : while_stmt",
"cmpd_stmt : for_stmt",
"if_stmt : T_If bool_exp T_Cln start_suite",
"if_stmt : T_If bool_exp T_Cln start_suite elif_stmts",
"elif_stmts : else_stmt",
"elif_stmts : T_Elif bool_exp T_Cln start_suite elif_stmts",
"else_stmt : T_Else T_Cln start_suite",
"while_stmt : T_While bool_exp T_Cln start_suite",
"myrange : term",
"myrange : term T_Comma term",
"myrange : term T_Comma term T_Comma term",
"for_stmt : T_For T_ID T_In T_Range T_OP myrange T_CP T_Cln start_suite",
"start_suite : basic_stmt",
"$$3 :",
"start_suite : T_NL $$3 finalStatements suite",
"suite : T_NL finalStatements suite",
"suite : T_NL end_suite",
"$$4 :",
"end_suite : T_CB $$4 finalStatements",
"$$5 :",
"end_suite : T_CB $$5",
"end_suite :",

};
#endif

#if YYDEBUG
int      yydebug;
#endif

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;
int      yynerrs;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
YYLTYPE  yyloc; /* position returned by actions */
YYLTYPE  yylloc; /* position from the lexer */
#endif

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(loc, rhs, n) \
do \
{ \
    if (n == 0) \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 0).last_line; \
        (loc).first_column = YYRHSLOC(rhs, 0).last_column; \
        (loc).last_line    = YYRHSLOC(rhs, 0).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, 0).last_column; \
    } \
    else \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 1).first_line; \
        (loc).first_column = YYRHSLOC(rhs, 1).first_column; \
        (loc).last_line    = YYRHSLOC(rhs, n).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, n).last_column; \
    } \
} while (0)
#endif /* YYLLOC_DEFAULT */
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#if YYBTYACC

#ifndef YYLVQUEUEGROWTH
#define YYLVQUEUEGROWTH 32
#endif
#endif /* YYBTYACC */

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#ifndef YYINITSTACKSIZE
#define YYINITSTACKSIZE 200
#endif

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  *p_base;
    YYLTYPE  *p_mark;
#endif
} YYSTACKDATA;
#if YYBTYACC

struct YYParseState_s
{
    struct YYParseState_s *save;    /* Previously saved parser state */
    YYSTACKDATA            yystack; /* saved parser stack */
    int                    state;   /* saved parser state */
    int                    errflag; /* saved error recovery status */
    int                    lexeme;  /* saved index of the conflict lexeme in the lexical queue */
    YYINT                  ctry;    /* saved index in yyctable[] for this conflict */
};
typedef struct YYParseState_s YYParseState;
#endif /* YYBTYACC */
/* variables for the parser stack */
static YYSTACKDATA yystack;
#if YYBTYACC

/* Current parser state */
static YYParseState *yyps = 0;

/* yypath != NULL: do the full parse, starting at *yypath parser state. */
static YYParseState *yypath = 0;

/* Base of the lexical value queue */
static YYSTYPE *yylvals = 0;

/* Current position at lexical value queue */
static YYSTYPE *yylvp = 0;

/* End position of lexical value queue */
static YYSTYPE *yylve = 0;

/* The last allocated position at the lexical value queue */
static YYSTYPE *yylvlim = 0;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
/* Base of the lexical position queue */
static YYLTYPE *yylpsns = 0;

/* Current position at lexical position queue */
static YYLTYPE *yylpp = 0;

/* End position of lexical position queue */
static YYLTYPE *yylpe = 0;

/* The last allocated position at the lexical position queue */
static YYLTYPE *yylplim = 0;
#endif

/* Current position at lexical token queue */
static YYINT  *yylexp = 0;

static YYINT  *yylexemes = 0;
#endif /* YYBTYACC */
#line 1135 "translator.y"



int main()
{
	//printf("Enter the Expression\n");
	yyparse();
	return 0;
}
#line 1617 "y.tab.c"

/* For use in generated program */
#define yydepth (int)(yystack.s_mark - yystack.s_base)
#if YYBTYACC
#define yytrial (yyps->save)
#endif /* YYBTYACC */

#if YYDEBUG
#include <stdio.h>	/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE *newps;
#endif

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    newps = (YYLTYPE *)realloc(data->p_base, newsize * sizeof(*newps));
    if (newps == 0)
        return YYENOMEM;

    data->p_base = newps;
    data->p_mark = newps + i;
#endif

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;

#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%sdebug: stack size increased to %d\n", YYPREFIX, newsize);
#endif
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    free(data->p_base);
#endif
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif /* YYPURE || defined(YY_NO_LEAKS) */
#if YYBTYACC

static YYParseState *
yyNewState(unsigned size)
{
    YYParseState *p = (YYParseState *) malloc(sizeof(YYParseState));
    if (p == NULL) return NULL;

    p->yystack.stacksize = size;
    if (size == 0)
    {
        p->yystack.s_base = NULL;
        p->yystack.l_base = NULL;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        p->yystack.p_base = NULL;
#endif
        return p;
    }
    p->yystack.s_base    = (YYINT *) malloc(size * sizeof(YYINT));
    if (p->yystack.s_base == NULL) return NULL;
    p->yystack.l_base    = (YYSTYPE *) malloc(size * sizeof(YYSTYPE));
    if (p->yystack.l_base == NULL) return NULL;
    memset(p->yystack.l_base, 0, size * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    p->yystack.p_base    = (YYLTYPE *) malloc(size * sizeof(YYLTYPE));
    if (p->yystack.p_base == NULL) return NULL;
    memset(p->yystack.p_base, 0, size * sizeof(YYLTYPE));
#endif

    return p;
}

static void
yyFreeState(YYParseState *p)
{
    yyfreestack(&p->yystack);
    free(p);
}
#endif /* YYBTYACC */

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab
#if YYBTYACC
#define YYVALID        do { if (yyps->save)            goto yyvalid; } while(0)
#define YYVALID_NESTED do { if (yyps->save && \
                                yyps->save->save == 0) goto yyvalid; } while(0)
#endif /* YYBTYACC */

int
YYPARSE_DECL()
{
    int yym, yyn, yystate, yyresult;
#if YYBTYACC
    int yynewerrflag;
    YYParseState *yyerrctx = NULL;
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  yyerror_loc_range[3]; /* position of error start/end (0 unused) */
#endif
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
    if (yydebug)
        fprintf(stderr, "%sdebug[<# of symbols on state stack>]\n", YYPREFIX);
#endif
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    memset(yyerror_loc_range, 0, sizeof(yyerror_loc_range));
#endif

#if YYBTYACC
    yyps = yyNewState(0); if (yyps == 0) goto yyenomem;
    yyps->save = 0;
#endif /* YYBTYACC */
    yym = 0;
    /* yyn is set below */
    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base;
#endif
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
#if YYBTYACC
        do {
        if (yylvp < yylve)
        {
            /* we're currently re-reading tokens */
            yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc = *yylpp++;
#endif
            yychar = *yylexp++;
            break;
        }
        if (yyps->save)
        {
            /* in trial mode; save scanner results for future parse attempts */
            if (yylvp == yylvlim)
            {   /* Enlarge lexical value queue */
                size_t p = (size_t) (yylvp - yylvals);
                size_t s = (size_t) (yylvlim - yylvals);

                s += YYLVQUEUEGROWTH;
                if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL) goto yyenomem;
                if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL) goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL) goto yyenomem;
#endif
                yylvp   = yylve = yylvals + p;
                yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp   = yylpe = yylpsns + p;
                yylplim = yylpsns + s;
#endif
                yylexp  = yylexemes + p;
            }
            *yylexp = (YYINT) YYLEX;
            *yylvp++ = yylval;
            yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *yylpp++ = yylloc;
            yylpe++;
#endif
            yychar = *yylexp++;
            break;
        }
        /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
        yychar = YYLEX;
#if YYBTYACC
        } while (0);
#endif /* YYBTYACC */
        if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, " <%s>", YYSTYPE_TOSTRING(yychar, yylval));
#endif
            fputc('\n', stderr);
        }
#endif
    }
#if YYBTYACC

    /* Do we have a conflict? */
    if (((yyn = yycindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
        yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        YYINT ctry;

        if (yypath)
        {
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: CONFLICT in state %d: following successful trial parse\n",
                                YYDEBUGSTR, yydepth, yystate);
#endif
            /* Switch to the next conflict context */
            save = yypath;
            yypath = save->save;
            save->save = NULL;
            ctry = save->ctry;
            if (save->state != yystate) YYABORT;
            yyFreeState(save);

        }
        else
        {

            /* Unresolved conflict - start/continue trial parse */
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
            {
                fprintf(stderr, "%s[%d]: CONFLICT in state %d. ", YYDEBUGSTR, yydepth, yystate);
                if (yyps->save)
                    fputs("ALREADY in conflict, continuing trial parse.\n", stderr);
                else
                    fputs("Starting trial parse.\n", stderr);
            }
#endif
            save                  = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (save == NULL) goto yyenomem;
            save->save            = yyps->save;
            save->state           = yystate;
            save->errflag         = yyerrflag;
            save->yystack.s_mark  = save->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (save->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            save->yystack.l_mark  = save->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (save->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            save->yystack.p_mark  = save->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (save->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            ctry                  = yytable[yyn];
            if (yyctable[ctry] == -1)
            {
#if YYDEBUG
                if (yydebug && yychar >= YYEOF)
                    fprintf(stderr, "%s[%d]: backtracking 1 token\n", YYDEBUGSTR, yydepth);
#endif
                ctry++;
            }
            save->ctry = ctry;
            if (yyps->save == NULL)
            {
                /* If this is a first conflict in the stack, start saving lexemes */
                if (!yylexemes)
                {
                    yylexemes = (YYINT *) malloc((YYLVQUEUEGROWTH) * sizeof(YYINT));
                    if (yylexemes == NULL) goto yyenomem;
                    yylvals   = (YYSTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYSTYPE));
                    if (yylvals == NULL) goto yyenomem;
                    yylvlim   = yylvals + YYLVQUEUEGROWTH;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpsns   = (YYLTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYLTYPE));
                    if (yylpsns == NULL) goto yyenomem;
                    yylplim   = yylpsns + YYLVQUEUEGROWTH;
#endif
                }
                if (yylvp == yylve)
                {
                    yylvp  = yylve = yylvals;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp  = yylpe = yylpsns;
#endif
                    yylexp = yylexemes;
                    if (yychar >= YYEOF)
                    {
                        *yylve++ = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                        *yylpe++ = yylloc;
#endif
                        *yylexp  = (YYINT) yychar;
                        yychar   = YYEMPTY;
                    }
                }
            }
            if (yychar >= YYEOF)
            {
                yylvp--;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp--;
#endif
                yylexp--;
                yychar = YYEMPTY;
            }
            save->lexeme = (int) (yylvp - yylvals);
            yyps->save   = save;
        }
        if (yytable[yyn] == ctry)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                                YYDEBUGSTR, yydepth, yystate, yyctable[ctry]);
#endif
            if (yychar < 0)
            {
                yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp++;
#endif
                yylexp++;
            }
            if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
                goto yyoverflow;
            yystate = yyctable[ctry];
            *++yystack.s_mark = (YYINT) yystate;
            *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *++yystack.p_mark = yylloc;
#endif
            yychar  = YYEMPTY;
            if (yyerrflag > 0) --yyerrflag;
            goto yyloop;
        }
        else
        {
            yyn = yyctable[ctry];
            goto yyreduce;
        }
    } /* End of code dealing with conflicts */
#endif /* YYBTYACC */
    if (((yyn = yysindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                            YYDEBUGSTR, yydepth, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yylloc;
#endif
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if (((yyn = yyrindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag != 0) goto yyinrecovery;
#if YYBTYACC

    yynewerrflag = 1;
    goto yyerrhandler;
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */

yyerrlab:
    /* explicit YYERROR from an action -- pop the rhs of the rule reduced
     * before looking for error recovery */
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif

    yynewerrflag = 0;
yyerrhandler:
    while (yyps->save)
    {
        int ctry;
        YYParseState *save = yyps->save;
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: ERROR in state %d, CONFLICT BACKTRACKING to state %d, %d tokens\n",
                            YYDEBUGSTR, yydepth, yystate, yyps->save->state,
                    (int)(yylvp - yylvals - yyps->save->lexeme));
#endif
        /* Memorize most forward-looking error state in case it's really an error. */
        if (yyerrctx == NULL || yyerrctx->lexeme < yylvp - yylvals)
        {
            /* Free old saved error context state */
            if (yyerrctx) yyFreeState(yyerrctx);
            /* Create and fill out new saved error context state */
            yyerrctx                 = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (yyerrctx == NULL) goto yyenomem;
            yyerrctx->save           = yyps->save;
            yyerrctx->state          = yystate;
            yyerrctx->errflag        = yyerrflag;
            yyerrctx->yystack.s_mark = yyerrctx->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (yyerrctx->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yyerrctx->yystack.l_mark = yyerrctx->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (yyerrctx->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yyerrctx->yystack.p_mark = yyerrctx->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (yyerrctx->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yyerrctx->lexeme         = (int) (yylvp - yylvals);
        }
        yylvp          = yylvals   + save->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yylpp          = yylpsns   + save->lexeme;
#endif
        yylexp         = yylexemes + save->lexeme;
        yychar         = YYEMPTY;
        yystack.s_mark = yystack.s_base + (save->yystack.s_mark - save->yystack.s_base);
        memcpy (yystack.s_base, save->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
        yystack.l_mark = yystack.l_base + (save->yystack.l_mark - save->yystack.l_base);
        memcpy (yystack.l_base, save->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yystack.p_mark = yystack.p_base + (save->yystack.p_mark - save->yystack.p_base);
        memcpy (yystack.p_base, save->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
        ctry           = ++save->ctry;
        yystate        = save->state;
        /* We tried shift, try reduce now */
        if ((yyn = yyctable[ctry]) >= 0) goto yyreduce;
        yyps->save     = save->save;
        save->save     = NULL;
        yyFreeState(save);

        /* Nothing left on the stack -- error */
        if (!yyps->save)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%sdebug[%d,trial]: trial parse FAILED, entering ERROR mode\n",
                                YYPREFIX, yydepth);
#endif
            /* Restore state as it was in the most forward-advanced error */
            yylvp          = yylvals   + yyerrctx->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylpp          = yylpsns   + yyerrctx->lexeme;
#endif
            yylexp         = yylexemes + yyerrctx->lexeme;
            yychar         = yylexp[-1];
            yylval         = yylvp[-1];
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc         = yylpp[-1];
#endif
            yystack.s_mark = yystack.s_base + (yyerrctx->yystack.s_mark - yyerrctx->yystack.s_base);
            memcpy (yystack.s_base, yyerrctx->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yystack.l_mark = yystack.l_base + (yyerrctx->yystack.l_mark - yyerrctx->yystack.l_base);
            memcpy (yystack.l_base, yyerrctx->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yystack.p_mark = yystack.p_base + (yyerrctx->yystack.p_mark - yyerrctx->yystack.p_base);
            memcpy (yystack.p_base, yyerrctx->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yystate        = yyerrctx->state;
            yyFreeState(yyerrctx);
            yyerrctx       = NULL;
        }
        yynewerrflag = 1;
    }
    if (yynewerrflag == 0) goto yyinrecovery;
#endif /* YYBTYACC */

    YYERROR_CALL("syntax error");
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yyerror_loc_range[1] = yylloc; /* lookahead position is error start position */
#endif

#if !YYBTYACC
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */
yyerrlab:
#endif
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if (((yyn = yysindex[*yystack.s_mark]) != 0) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: state %d, error recovery shifting to state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* lookahead position is error end position */
                yyerror_loc_range[2] = yylloc;
                YYLLOC_DEFAULT(yyloc, yyerror_loc_range, 2); /* position of error span */
                *++yystack.p_mark = yyloc;
#endif
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: error recovery discarding state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* the current TOS position is the error start position */
                yyerror_loc_range[1] = *yystack.p_mark;
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
                if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark, yystack.p_mark);
#else
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
                --yystack.s_mark;
                --yystack.l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                --yystack.p_mark;
#endif
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, error recovery discarding token %d (%s)\n",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
        }
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval, &yylloc);
#else
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
    yym = yylen[yyn];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: state %d, reducing by rule %d (%s)",
                        YYDEBUGSTR, yydepth, yystate, yyn, yyrule[yyn]);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            if (yym > 0)
            {
                int i;
                fputc('<', stderr);
                for (i = yym; i > 0; i--)
                {
                    if (i != yym) fputs(", ", stderr);
                    fputs(YYSTYPE_TOSTRING(yystos[yystack.s_mark[1-i]],
                                           yystack.l_mark[1-i]), stderr);
                }
                fputc('>', stderr);
            }
#endif
        fputc('\n', stderr);
    }
#endif
    if (yym > 0)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)

    /* Perform position reduction */
    memset(&yyloc, 0, sizeof(yyloc));
#if YYBTYACC
    if (!yytrial)
#endif /* YYBTYACC */
    {
        YYLLOC_DEFAULT(yyloc, &yystack.p_mark[-yym], yym);
        /* just in case YYERROR is invoked within the action, save
           the start of the rhs as the error start position */
        yyerror_loc_range[1] = yystack.p_mark[1-yym];
    }
#endif

    switch (yyn)
    {
case 1:
#line 1041 "translator.y"
	{init();}
#line 2290 "y.tab.c"
break;
case 2:
#line 1041 "translator.y"
	{printf("\nValid Python Syntax\n"); printAST(yystack.l_mark[-1].node);
				printf("\n-------------Three-address code---------------\n");
 				codeGenOp(yystack.l_mark[-1].node); printSTable(); freeAll(); exit(0);}
#line 2297 "y.tab.c"
break;
case 3:
#line 1045 "translator.y"
	{insertRecord("Constant", yystack.l_mark[0].text, yystack.p_mark[0].first_line, currentScope); yyval.node = createID_Const("Constant", yystack.l_mark[0].text, currentScope);}
#line 2302 "y.tab.c"
break;
case 4:
#line 1047 "translator.y"
	{modifyRecordID("Identifier", yystack.l_mark[0].text, yystack.p_mark[0].first_line, currentScope); yyval.node = createID_Const("Identifier", yystack.l_mark[0].text, currentScope);}
#line 2307 "y.tab.c"
break;
case 5:
#line 1048 "translator.y"
	{yyval.node = yystack.l_mark[0].node;}
#line 2312 "y.tab.c"
break;
case 6:
#line 1052 "translator.y"
	{yyval.node=yystack.l_mark[0].node;}
#line 2317 "y.tab.c"
break;
case 7:
#line 1052 "translator.y"
	{resetDepth();}
#line 2322 "y.tab.c"
break;
case 8:
#line 1052 "translator.y"
	{yyval.node = createOp("NewLine", 2, yystack.l_mark[-3].node, yystack.l_mark[0].node);}
#line 2327 "y.tab.c"
break;
case 9:
#line 1052 "translator.y"
	{yyval.node=yystack.l_mark[-1].node;}
#line 2332 "y.tab.c"
break;
case 10:
#line 1054 "translator.y"
	{yyval.node=yystack.l_mark[0].node;}
#line 2337 "y.tab.c"
break;
case 11:
#line 1055 "translator.y"
	{yyval.node=yystack.l_mark[0].node;}
#line 2342 "y.tab.c"
break;
case 12:
#line 1056 "translator.y"
	{yyval.node=yystack.l_mark[0].node;}
#line 2347 "y.tab.c"
break;
case 13:
#line 1057 "translator.y"
	{yyval.node=yystack.l_mark[0].node;}
#line 2352 "y.tab.c"
break;
case 14:
#line 1058 "translator.y"
	{yyval.node=yystack.l_mark[0].node;}
#line 2357 "y.tab.c"
break;
case 15:
#line 1060 "translator.y"
	{yyval.node=yystack.l_mark[0].node;}
#line 2362 "y.tab.c"
break;
case 16:
#line 1061 "translator.y"
	{yyval.node = createOp("+", 2, yystack.l_mark[-2].node, yystack.l_mark[0].node);}
#line 2367 "y.tab.c"
break;
case 17:
#line 1062 "translator.y"
	{yyval.node = createOp("-", 2, yystack.l_mark[-2].node, yystack.l_mark[0].node);}
#line 2372 "y.tab.c"
break;
case 18:
#line 1063 "translator.y"
	{yyval.node = createOp("*", 2, yystack.l_mark[-2].node, yystack.l_mark[0].node);}
#line 2377 "y.tab.c"
break;
case 19:
#line 1064 "translator.y"
	{yyval.node = createOp("/", 2, yystack.l_mark[-2].node, yystack.l_mark[0].node);}
#line 2382 "y.tab.c"
break;
case 20:
#line 1065 "translator.y"
	{yyval.node = createOp("-", 1, yystack.l_mark[0].node);}
#line 2387 "y.tab.c"
break;
case 21:
#line 1066 "translator.y"
	{yyval.node = yystack.l_mark[-1].node;}
#line 2392 "y.tab.c"
break;
case 22:
#line 1069 "translator.y"
	{yyval.node = createOp("or", 2, yystack.l_mark[-2].node, yystack.l_mark[0].node);}
#line 2397 "y.tab.c"
break;
case 23:
#line 1070 "translator.y"
	{yyval.node = createOp("<", 2, yystack.l_mark[-2].node, yystack.l_mark[0].node);}
#line 2402 "y.tab.c"
break;
case 24:
#line 1071 "translator.y"
	{yyval.node = createOp("and", 2, yystack.l_mark[-2].node, yystack.l_mark[0].node);}
#line 2407 "y.tab.c"
break;
case 25:
#line 1072 "translator.y"
	{yyval.node = createOp(">", 2, yystack.l_mark[-2].node, yystack.l_mark[0].node);}
#line 2412 "y.tab.c"
break;
case 26:
#line 1073 "translator.y"
	{yyval.node = createOp("<=", 2, yystack.l_mark[-2].node, yystack.l_mark[0].node);}
#line 2417 "y.tab.c"
break;
case 27:
#line 1074 "translator.y"
	{yyval.node = createOp(">=", 2, yystack.l_mark[-2].node, yystack.l_mark[0].node);}
#line 2422 "y.tab.c"
break;
case 28:
#line 1075 "translator.y"
	{yyval.node=yystack.l_mark[0].node;}
#line 2427 "y.tab.c"
break;
case 29:
#line 1077 "translator.y"
	{yyval.node = yystack.l_mark[0].node;}
#line 2432 "y.tab.c"
break;
case 30:
#line 1078 "translator.y"
	{yyval.node = createOp("==", 2, yystack.l_mark[-2].node, yystack.l_mark[0].node);}
#line 2437 "y.tab.c"
break;
case 31:
#line 1079 "translator.y"
	{yyval.node = createOp("==", 2, yystack.l_mark[-2].node, yystack.l_mark[0].node);}
#line 2442 "y.tab.c"
break;
case 32:
#line 1080 "translator.y"
	{insertRecord("Constant", "True", yystack.p_mark[0].first_line, currentScope); yyval.node = createID_Const("Constant", "True", currentScope);}
#line 2447 "y.tab.c"
break;
case 33:
#line 1081 "translator.y"
	{insertRecord("Constant", "False", yystack.p_mark[0].first_line, currentScope); yyval.node = createID_Const("Constant", "False", currentScope);}
#line 2452 "y.tab.c"
break;
case 34:
#line 1083 "translator.y"
	{yyval.node = createOp("!", 1, yystack.l_mark[0].node);}
#line 2457 "y.tab.c"
break;
case 35:
#line 1084 "translator.y"
	{yyval.node = yystack.l_mark[-1].node;}
#line 2462 "y.tab.c"
break;
case 36:
#line 1086 "translator.y"
	{yyval.node = createOp("break", 0);}
#line 2467 "y.tab.c"
break;
case 37:
#line 1088 "translator.y"
	{insertRecord("Identifier", yystack.l_mark[-2].text, yystack.p_mark[-2].first_line, currentScope); yyval.node = createOp("=", 2, createID_Const("Identifier", yystack.l_mark[-2].text, currentScope), yystack.l_mark[0].node);}
#line 2472 "y.tab.c"
break;
case 38:
#line 1089 "translator.y"
	{insertRecord("Identifier", yystack.l_mark[-2].text, yystack.p_mark[-2].first_line, currentScope);yyval.node = createOp("=", 2, createID_Const("Identifier", yystack.l_mark[-2].text, currentScope), yystack.l_mark[0].node);}
#line 2477 "y.tab.c"
break;
case 39:
#line 1091 "translator.y"
	{yyval.node = createOp("Print", 1, yystack.l_mark[-1].node);}
#line 2482 "y.tab.c"
break;
case 40:
#line 1093 "translator.y"
	{yyval.node = yystack.l_mark[0].node;}
#line 2487 "y.tab.c"
break;
case 41:
#line 1094 "translator.y"
	{yyval.node = yystack.l_mark[0].node;}
#line 2492 "y.tab.c"
break;
case 42:
#line 1095 "translator.y"
	{yyerrok; yyclearin; yyval.node=createOp("SyntaxError", 0);}
#line 2497 "y.tab.c"
break;
case 43:
#line 1097 "translator.y"
	{yyval.node = yystack.l_mark[0].node;}
#line 2502 "y.tab.c"
break;
case 44:
#line 1098 "translator.y"
	{yyval.node = yystack.l_mark[0].node;}
#line 2507 "y.tab.c"
break;
case 45:
#line 1099 "translator.y"
	{yyval.node = yystack.l_mark[0].node;}
#line 2512 "y.tab.c"
break;
case 46:
#line 1102 "translator.y"
	{yyval.node = createOp("If", 2, yystack.l_mark[-2].node, yystack.l_mark[0].node);}
#line 2517 "y.tab.c"
break;
case 47:
#line 1103 "translator.y"
	{yyval.node = createOp("If", 3, yystack.l_mark[-3].node, yystack.l_mark[-1].node, yystack.l_mark[0].node);}
#line 2522 "y.tab.c"
break;
case 48:
#line 1105 "translator.y"
	{yyval.node= yystack.l_mark[0].node;}
#line 2527 "y.tab.c"
break;
case 49:
#line 1106 "translator.y"
	{yyval.node= createOp("Elif", 3, yystack.l_mark[-3].node, yystack.l_mark[-1].node, yystack.l_mark[0].node);}
#line 2532 "y.tab.c"
break;
case 50:
#line 1108 "translator.y"
	{yyval.node = createOp("Else", 1, yystack.l_mark[0].node);}
#line 2537 "y.tab.c"
break;
case 51:
#line 1110 "translator.y"
	{yyval.node = createOp("While", 2, yystack.l_mark[-2].node, yystack.l_mark[0].node);}
#line 2542 "y.tab.c"
break;
case 55:
#line 1116 "translator.y"
	{node* bool_node;bool_node=(node*)calloc(1, sizeof(node));
																		insertRecord("Identifier", yystack.l_mark[-7].text, yystack.p_mark[-8].first_line, currentScope);
																		node* id = createID_Const("Identifier", yystack.l_mark[-7].text, currentScope);
																		bool_node=createOp("<", 2, id, yystack.l_mark[-3].node);
																		yyval.node = createOp("For", 4, createOp("<", 2, id, yystack.l_mark[-3].node), id, yystack.l_mark[-3].node, yystack.l_mark[0].node);}
#line 2551 "y.tab.c"
break;
case 56:
#line 1122 "translator.y"
	{yyval.node = yystack.l_mark[0].node;}
#line 2556 "y.tab.c"
break;
case 57:
#line 1123 "translator.y"
	{initNewTable(scope_depth); updateCScope(scope_depth);}
#line 2561 "y.tab.c"
break;
case 58:
#line 1123 "translator.y"
	{yyval.node = createOp("BeginBlock", 2, yystack.l_mark[-1].node, yystack.l_mark[0].node);}
#line 2566 "y.tab.c"
break;
case 59:
#line 1126 "translator.y"
	{yyval.node = createOp("Next", 2, yystack.l_mark[-1].node, yystack.l_mark[0].node);}
#line 2571 "y.tab.c"
break;
case 60:
#line 1127 "translator.y"
	{yyval.node = yystack.l_mark[0].node;}
#line 2576 "y.tab.c"
break;
case 61:
#line 1129 "translator.y"
	{updateCScope(scope_depth);}
#line 2581 "y.tab.c"
break;
case 62:
#line 1129 "translator.y"
	{yyval.node = createOp("EndBlock", 1, yystack.l_mark[0].node);}
#line 2586 "y.tab.c"
break;
case 63:
#line 1130 "translator.y"
	{updateCScope(scope_depth);}
#line 2591 "y.tab.c"
break;
case 64:
#line 1130 "translator.y"
	{yyval.node = createOp("EndBlock", 0);}
#line 2596 "y.tab.c"
break;
case 65:
#line 1131 "translator.y"
	{yyval.node = createOp("EndBlock", 0); resetDepth();}
#line 2601 "y.tab.c"
break;
#line 2603 "y.tab.c"
    default:
        break;
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
        {
            fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[YYFINAL], yyval));
#endif
            fprintf(stderr, "shifting from state 0 to final state %d\n", YYFINAL);
        }
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yyloc;
#endif
        if (yychar < 0)
        {
#if YYBTYACC
            do {
            if (yylvp < yylve)
            {
                /* we're currently re-reading tokens */
                yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylloc = *yylpp++;
#endif
                yychar = *yylexp++;
                break;
            }
            if (yyps->save)
            {
                /* in trial mode; save scanner results for future parse attempts */
                if (yylvp == yylvlim)
                {   /* Enlarge lexical value queue */
                    size_t p = (size_t) (yylvp - yylvals);
                    size_t s = (size_t) (yylvlim - yylvals);

                    s += YYLVQUEUEGROWTH;
                    if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL)
                        goto yyenomem;
                    if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL)
                        goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL)
                        goto yyenomem;
#endif
                    yylvp   = yylve = yylvals + p;
                    yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp   = yylpe = yylpsns + p;
                    yylplim = yylpsns + s;
#endif
                    yylexp  = yylexemes + p;
                }
                *yylexp = (YYINT) YYLEX;
                *yylvp++ = yylval;
                yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                *yylpp++ = yylloc;
                yylpe++;
#endif
                yychar = *yylexp++;
                break;
            }
            /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
            yychar = YYLEX;
#if YYBTYACC
            } while (0);
#endif /* YYBTYACC */
            if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
                fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)\n",
                                YYDEBUGSTR, yydepth, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if (((yyn = yygindex[yym]) != 0) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[yystate], yyval));
#endif
        fprintf(stderr, "shifting from state %d to state %d\n", *yystack.s_mark, yystate);
    }
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    *++yystack.p_mark = yyloc;
#endif
    goto yyloop;
#if YYBTYACC

    /* Reduction declares that this path is valid. Set yypath and do a full parse */
yyvalid:
    if (yypath) YYABORT;
    while (yyps->save)
    {
        YYParseState *save = yyps->save;
        yyps->save = save->save;
        save->save = yypath;
        yypath = save;
    }
#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%s[%d]: state %d, CONFLICT trial successful, backtracking to state %d, %d tokens\n",
                        YYDEBUGSTR, yydepth, yystate, yypath->state, (int)(yylvp - yylvals - yypath->lexeme));
#endif
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    yylvp          = yylvals + yypath->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yylpp          = yylpsns + yypath->lexeme;
#endif
    yylexp         = yylexemes + yypath->lexeme;
    yychar         = YYEMPTY;
    yystack.s_mark = yystack.s_base + (yypath->yystack.s_mark - yypath->yystack.s_base);
    memcpy (yystack.s_base, yypath->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
    yystack.l_mark = yystack.l_base + (yypath->yystack.l_mark - yypath->yystack.l_base);
    memcpy (yystack.l_base, yypath->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base + (yypath->yystack.p_mark - yypath->yystack.p_base);
    memcpy (yystack.p_base, yypath->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
    yystate        = yypath->state;
    goto yyloop;
#endif /* YYBTYACC */

yyoverflow:
    YYERROR_CALL("yacc stack overflow");
#if YYBTYACC
    goto yyabort_nomem;
yyenomem:
    YYERROR_CALL("memory exhausted");
yyabort_nomem:
#endif /* YYBTYACC */
    yyresult = 2;
    goto yyreturn;

yyabort:
    yyresult = 1;
    goto yyreturn;

yyaccept:
#if YYBTYACC
    if (yyps->save) goto yyvalid;
#endif /* YYBTYACC */
    yyresult = 0;

yyreturn:
#if defined(YYDESTRUCT_CALL)
    if (yychar != YYEOF && yychar != YYEMPTY)
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval, &yylloc);
#else
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */

    {
        YYSTYPE *pv;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYLTYPE *pp;

        for (pv = yystack.l_base, pp = yystack.p_base; pv <= yystack.l_mark; ++pv, ++pp)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv, pp);
#else
        for (pv = yystack.l_base; pv <= yystack.l_mark; ++pv)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
    }
#endif /* defined(YYDESTRUCT_CALL) */

#if YYBTYACC
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    while (yyps)
    {
        YYParseState *save = yyps;
        yyps = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
    while (yypath)
    {
        YYParseState *save = yypath;
        yypath = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
#endif /* YYBTYACC */
    yyfreestack(&yystack);
    return (yyresult);
}
