%{
    #include <stdio.h>
    #include <iostream>
    #include <string>
    #include <vector>
    #include <map>
    #include "y.tab.h"
    using namespace std;
    extern FILE *yyin;
    extern int yylex();
    void yyerror(string s);
    int numOfTabs = 0;
    int linenum = 1;
    
    #define INT_TYPE 1
    #define FLOAT_TYPE 2
    #define STRING_TYPE 3

    vector<bool> ifScopeVector;
    map<string, int> typeMap;
    
    string cppFile = "void main()\n{\n";
    string tmp = "";
    string assignment = "";
    string element;
    string intVars = "";
    string floatVars = "";
    string stringVars = "";
    string stringSign;

    int types[2];
    int requiredTabNum = 0;
    bool flag = false;
    bool isPlus;

    char *variable;

    // Function declarations
    void handleConditionStatements(string conditionWord);
    void tabCheck();
    void primitiveTypeDetect(int type, string element);
    void handleAssignment();
    void printParseTree(string nodeName, int depth);
    void printSymbolTable();

    struct TreeNode {
        string name;
        vector<TreeNode*> children;
    };

    TreeNode* root = nullptr;
    TreeNode* currentNode = nullptr;

%}

%union
{
    char *str;
}

%token <str> VAR STRING PLUS MINUS MULT DIV INT FLOAT EQUAL IF ELIF ELSE COMPARISON NWLN COLON TAB

%%

program:
    statements
    | newlines statements
    ;

newline:
    NWLN { linenum++; }
    ;

newlines:
    newline
    | newline newlines
    ;

statements:
    | statement
    | statement newlines statements
    ;

tab:
    TAB { numOfTabs++; }
    ;

comparison_expr:
    COMPARISON {
        string element($1);
        tmp += " " + element;
    }
    ;

if:
    IF {
        handleConditionStatements("if");
    }
    ;

elif:
    ELIF {
        handleConditionStatements("else if");
    }
    ;

else:
    ELSE {
        handleConditionStatements("else");
    }
    ;

var:
    VAR {
        variable = $1;
        for (int i = requiredTabNum; i >= numOfTabs + 1; i--) {
            for (int j = 0; j < i; j++)
                tmp += "\t";
            tmp += "}\n";
        }
        for (int i = 0; i <= numOfTabs; i++)
            tmp += "\t";
        assignment = "";
    }
    ;

statement:
    tab statement
    | if_statement
    | elif_statement
    | else_statement
    | assign_statements
    ;

if_statement:
    if signed_operand comparison_expr sec_signed_operand COLON {
        for (int i = 0; i <= numOfTabs; i++)
            tmp += "\t";
        tmp += "{\n";
        
        tabCheck();
        
        if (numOfTabs >= ifScopeVector.size())
            ifScopeVector.push_back(true);
        else
            ifScopeVector[numOfTabs] = true;
        if (numOfTabs < requiredTabNum) {
            while (ifScopeVector.size() != numOfTabs + 1)
                ifScopeVector.pop_back();
            requiredTabNum = numOfTabs;
        }
        requiredTabNum++;
        flag = true;
        numOfTabs = 0;
        if ((types[0] == 3 || types[1] == 3) && ((types[0] != 3 || types[1] != 3) || (stringSign != "" && types[1] == 3))) {
            cout << "comparison type mismatch in line " << linenum << endl;
            exit(1);
        }
        if (types[0] == 1 && types[1] == 2)
            types[0] = 2;
    }
    ;

elif_statement:
    elif signed_operand comparison_expr sec_signed_operand COLON {
        for (int i = 0; i <= numOfTabs; i++)
            tmp += "\t";
        tmp += "{\n";
        
        tabCheck();

        if (numOfTabs < requiredTabNum) {
            while (ifScopeVector.size() != numOfTabs + 1)
                ifScopeVector.pop_back();
            requiredTabNum = numOfTabs;
        }
        if (numOfTabs >= ifScopeVector.size() || !ifScopeVector[numOfTabs]) {
            cout << "elif after else in line " << linenum << endl;
            exit(1);
        }
        requiredTabNum++;
        flag = true;
        numOfTabs = 0;
        if ((types[0] == 3 || types[1] == 3) && ((types[0] != 3 || types[1] != 3) || (stringSign != "" && types[1] == 3))) {
            cout << "comparison type mismatch in line " << linenum << endl;
            exit(1);
        }
        if (types[0] == 1 && types[1] == 2)
            types[0] = 2;
    }
    ;

else_statement:
    else {
        tabCheck();

        if (numOfTabs < requiredTabNum) {
            while (ifScopeVector.size() != numOfTabs + 1)
                ifScopeVector.pop_back();
            requiredTabNum = numOfTabs;
        }
        for (int i = 0; i <= numOfTabs; i++)
            tmp += "\t";
        tmp += "{\n";
        if (numOfTabs >= ifScopeVector.size() || !ifScopeVector[numOfTabs]) {
            cout << "else without if in line " << linenum << endl;
            exit(1);
        }
        ifScopeVector[numOfTabs] = false;
        requiredTabNum++;
        flag = true;
        numOfTabs = 0;
    }
    ;

assign_statements:
    var EQUAL sign {
        assignment += stringSign;
    }
    assign_statement {
        handleAssignment();
    }
    ;

assign_statement:
    operand {
        types[0] = types[1];
    }
    | assign_statement operator operand {
        if ((types[0] == 3 || types[1] == 3) && (types[0] != 3 || types[1] != 3 || !isPlus)) {
            cout << "type mismatch in line " << linenum << endl;
            exit(1);
        }
        if (types[0] == 1 && types[1] == 2)
            types[0] = 2;
    }
    ;

signed_operand:
    sign {
        tmp += stringSign;
    }
    operand {
        types[0] = types[1];
        tmp += element;
        if (stringSign != "" && types[0] == 3) {
            cout << "type inconsistency in line " << linenum << endl;
            exit(1);
        }
    }
    ;

sec_signed_operand:
    sign {
        tmp += stringSign;
    }
    operand {
        tmp += element + ")\n";
    }
    ;

operand:
    VAR {
        types[1] = typeMap[$1];
        if (!types[1]) {
            cout << "name '" << $1 << "' is not defined in line " << linenum << endl;
            exit(1);
        } else if (types[1] == 1) {
            element = " " + string($1) + "_int";
        } else if (types[1] == 2) {
            element = " " + string($1) + "_flt";
        } else if (types[1] == 3) {
            element = " " + string($1) + "_str";
        }
        assignment += element;
    }
    | INT {
        element = " " + string($1);
        primitiveTypeDetect(1, element);
    }
    | FLOAT {
        element = " " + string($1);
        primitiveTypeDetect(2, element);
    }
    | STRING {
        element = " " + string($1);
        primitiveTypeDetect(3, element);
    }
    ;

sign:
    { stringSign = ""; }
    | PLUS { stringSign = " +"; }
    | MINUS { stringSign = " -"; }
    ;

operator:
    PLUS { isPlus = true; assignment += " +"; }
    | MINUS { isPlus = false; assignment += " -"; }
    | MULT { isPlus = false; assignment += " *"; }
    | DIV { isPlus = false; assignment += " /"; }
    ;
%%

void primitiveTypeDetect(int type, string element) {
    assignment += element;
    types[1] = type;
}

void tabCheck() {
    if ((flag && numOfTabs != requiredTabNum) || numOfTabs > requiredTabNum) {
        if (!ifScopeVector.empty()) {
            cout << "error in line " << linenum << ": at least one line should be inside if/elif/else block" << endl;
            exit(1);
        } else {
            cout << "tab inconsistency in line " << linenum << endl;
            exit(1);
        }
    }
}

void handleConditionStatements(string conditionWord) {
    for (int i = numOfTabs + 1; i <= requiredTabNum; i++) {
        for (int j = 0; j < i; j++)
            tmp += "\t";
        tmp += "}\n";
    }
    for (int i = 0; i <= numOfTabs; i++) tmp += "\t";
    if (conditionWord != "else") conditionWord = conditionWord + " (";
    else if (conditionWord == "else") conditionWord += "\n";
    tmp += conditionWord;
}

void handleTypeAssignment(const string& element, const string& suffix, string& vars, int type) {
    string varName = element + suffix;

    bool isThere = false;
    int len = 0;
    for (int i = 0; i < vars.length(); ++i) {
        if (vars[i] == varName[len]) {
            ++len;
            if (len == varName.length() && (i + 1 == vars.length() || vars[i + 1] == ',')) {
                isThere = true;
            }
        } else if (vars[i] == ',') {
            len = 0;
        }
    }

    if (len == varName.length()) {
        isThere = true;
    }

    if (!isThere) {
        if (vars.length())
            vars += ",";
        vars += varName;
    }

    tmp += varName + " =";
    typeMap[element] = type;
}

void handleAssignment() {
    string element(variable);

    tabCheck();

    if (numOfTabs < requiredTabNum) {
        while (ifScopeVector.size() != numOfTabs)
            ifScopeVector.pop_back();
        requiredTabNum = numOfTabs;
    }
    flag = false;
    numOfTabs = 0;

    switch (types[0]) {
        case 1:
            handleTypeAssignment(element, "_int", intVars, 1);
            break;
        case 2:
            handleTypeAssignment(element, "_flt", floatVars, 2);
            break;
        case 3:
            if (stringSign != "") {
                cout << "type inconsistency in line " << linenum << endl;
                exit(1);
            }

            handleTypeAssignment(element, "_str", stringVars, 3);
            break;
    }
    tmp += assignment + ";\n";
}

void appendVariableDeclaration(const string& type, const string& vars) {
    if (vars.length())
        cppFile += "\t" + type + " " + vars + ";\n";
}

void appendTabsAndClosingBrace(int tabs) {
    for (int j = 0; j < tabs; ++j)
        cppFile += "\t";

    cppFile += "}\n";
}

void writeToCpp() {
    appendVariableDeclaration("int", intVars);
    appendVariableDeclaration("float", floatVars);
    appendVariableDeclaration("string", stringVars);

    cppFile += "\n" + tmp;

    for (int i = requiredTabNum; i >= numOfTabs + 1; --i) {
        appendTabsAndClosingBrace(i);
    }
    cppFile += "\n}";
}

void printParseTree(TreeNode* node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; ++i) cout << "  ";
    cout << node->name << endl;
    for (auto child : node->children) {
        printParseTree(child, depth + 1);
    }
}

void printSymbolTable() {
    cout << "Symbol Table:" << endl;
    cout << "| Variable | Type  |" << endl;
    cout << "|----------|-------|" << endl;
    for (const auto& entry : typeMap) {
        cout << "| " << entry.first << " | ";
        switch (entry.second) {
            case INT_TYPE:
                cout << "int";
                break;
            case FLOAT_TYPE:
                cout << "float";
                break;
            case STRING_TYPE:
                cout << "string";
                break;
        }
        cout << " |" << endl;
    }
}

void yyerror(string s) {
    cerr << "Error at line: " << linenum << endl;
    exit(1);
}

int yywrap() {
    return 1;
}

int main(int argc, char *argv[]) {
    /* Call the lexer, then quit. */
    yyin = fopen(argv[1], "r");
    yyparse();
    fclose(yyin);

    writeToCpp();

    cout << cppFile << endl;

    // Print the parse tree
    cout << "Parse Tree:" << endl;
    printParseTree(root, 0);

    // Print the symbol table
    printSymbolTable();

    return 0;
}