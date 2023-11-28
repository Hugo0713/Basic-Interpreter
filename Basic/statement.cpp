/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"


/* Implementation of the Statement class */

int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;


PrintStmt::PrintStmt(std::string str)
{
    TokenScanner scanner(str);
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    
    std::string token = scanner.nextToken();
    token = scanner.nextToken();
    token = scanner.nextToken();
    scanner.setInput(token);
    exp = readE(scanner); 
}

PrintStmt::~PrintStmt()
{
    delete exp;
}

void PrintStmt::execute(EvalState &state, Program &program)
{
    std::cout << exp->eval(state) << "\n";
}


InputStmt::InputStmt(std::string str)
{
    TokenScanner scanner(str);
    scanner.ignoreWhitespace();

    var = scanner.nextToken();
    var = scanner.nextToken();
    var = scanner.nextToken();
}

void InputStmt::execute(EvalState &state, Program &program)
{
    std::cout << " ? ";
    std::string str1;

    TokenScanner scanner1(str1);
    Expression *exp1 = nullptr;

    bool flag_num = false;
    while(!flag_num)
    {
        flag_num = true;
        std::getline(std::cin, str1);
        for(int i = 0; i < str1.size(); ++i) 
        {
            if(!(((str1[i] >= '0') && (str1[i] <= '9')) || ((str1[i] == '-') && (i == 0)))) 
            {
                flag_num = false;
                std::cout << "INVALID NUMBER" << '\n';
                std::cout << " ? "; 
                break;
            }
        }

        if(flag_num)
        {
            scanner1.setInput(str1);
            scanner1.ignoreWhitespace();
            scanner1.scanNumbers();

            exp1 = readE(scanner1);
        }
    }
    state.setValue(var, exp1->eval(state));
    delete exp1;
}


LetStmt::LetStmt(std::string str)
{
    TokenScanner scanner(str);
    scanner.ignoreWhitespace();
    var = scanner.nextToken();
    var = scanner.nextToken();
    var = scanner.nextToken();
    scanner.nextToken();
    right_value = readE(scanner);
}

LetStmt::~LetStmt()
{
    delete right_value;
}

void LetStmt::execute(EvalState &state, Program &program)
{
    if(var == "LET" || var == "QUIT" || var == "CLEAR" || var == "REM" || var == "PRINT" || var == "END" || var == "LIST" || var == "IF" || var == "GOTO" || var == "RUN")
    {
        error("SYNTAX ERROR");
    }
    state.setValue(var, right_value->eval(state));
}


void RemStmt::execute(EvalState &state, Program &program)
{
    return;
}


IfStmt::IfStmt(std::string str)
{
    TokenScanner scanner(str);
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    
    std::string token;
    token = scanner.nextToken();
    token = scanner.nextToken();
    exp1 = readE(scanner, 1);
    cmp = "";
    while(cmp != "=" && cmp != "<" && cmp != ">")
    {
        cmp = scanner.nextToken();
    }
    exp2 = readE(scanner);
    scanner.nextToken();
    lineNumber = stoi(scanner.nextToken());
}

void IfStmt::execute(EvalState &state, Program &program)
{
    if(cmp == "=")
    {
        if(exp1->eval(state) == exp2->eval(state))
        {
            program.runLine = lineNumber;
        }
    }
    if(cmp == ">")
    {
        if(exp1->eval(state) > exp2->eval(state))
        {
            program.runLine = lineNumber;
        }
    }
    if(cmp == "<")
    {
        if(exp1->eval(state) < exp2->eval(state))
        {
            program.runLine = lineNumber;
        }
    }
}

IfStmt::~IfStmt()
{
    delete exp1;
    delete exp2;
}


GotoStmt::GotoStmt(std::string str)
{
    TokenScanner scanner(str);
    scanner.ignoreWhitespace();
    
    scanner.nextToken();
    scanner.nextToken();
    lineNumber = stoi(scanner.nextToken());
}

void GotoStmt::execute(EvalState &state, Program &program)
{
    program.runLine = lineNumber;
    if(program.lines.find(lineNumber) == program.lines.end())
    {
        error("LINE NUMBER ERROR");
    }
}


void EndStmt::execute(EvalState &state, Program &program)
{
    program.runLine = -1;
}

