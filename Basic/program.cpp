/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"



Program::Program() = default;

Program::~Program()
{
    clear();
} 

void Program::clear() 
{
    lines.clear();
    sourceLine.clear();
    for(auto& it : pr)
    {
        delete it.second;
    }
    pr.clear();
}

void Program::addSourceLine(int lineNumber, const std::string &line) 
{
    // if(lines.find(lineNumber) == lines.end())
    // {
    //     lines.insert(lineNumber);
    //     sourceLine[lineNumber] = line;
    // }
    // else
    // {
    //     sourceLine[lineNumber] = line;
    //     delete pr[lineNumber];
    //     pr.erase(lineNumber);
    // }
    
    TokenScanner scanner(line);
    scanner.ignoreWhitespace();
    std::string token = scanner.nextToken();
    if(scanner.hasMoreTokens())
    {
        token = scanner.nextToken();
        lines.insert(lineNumber);
        sourceLine[lineNumber] = line;

        if(lines.find(lineNumber) != lines.end())
        {
            delete pr[lineNumber];
        }
        
        if(token == "LET")
        {
            setParsedStatement(lineNumber, new LetStmt(line));
        }
        if(token == "REM")
        {
            setParsedStatement(lineNumber, new RemStmt);
        }
        if(token == "PRINT")
        {
            setParsedStatement(lineNumber, new PrintStmt(line));
        }
        if(token == "INPUT")
        {  
            setParsedStatement(lineNumber, new InputStmt(line));       
        }
        if(token == "IF")
        {
            setParsedStatement(lineNumber, new IfStmt(line));
        }    
        if(token == "GOTO")
        {
            setParsedStatement(lineNumber, new GotoStmt(line));
        }
        if(token == "END")
        {
            setParsedStatement(lineNumber, new EndStmt);
        }
    }
    else
    {
        delete pr[lineNumber];
        lines.erase(lineNumber);
        sourceLine.erase(lineNumber);
        pr.erase(lineNumber);
    }
}

void Program::removeSourceLine(int lineNumber) 
{
    if(lines.find(lineNumber) != lines.end())
    {
        lines.erase(lineNumber);
        sourceLine.erase(lineNumber);
        delete pr[lineNumber];
        pr.erase(lineNumber);
    }
}

std::string Program::getSourceLine(int lineNumber) 
{
    if(lines.find(lineNumber) != lines.end())
    {
        return sourceLine[lineNumber];
    }
    else
    {
        return nullptr;
    }
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) 
{
    // if(lines.find(lineNumber) != lines.end())
    // {
    //     if(pr[lineNumber] == nullptr)
    //     {
    //         pr[lineNumber] = stmt;
    //     }
    //     else
    //     {
    //         delete pr[lineNumber];
    //         pr[lineNumber] = stmt;
    //     }
    // }
    // else
    // {
    //     throw "error";
    // }
    if(lines.find(lineNumber) != lines.end())
    {
        // if(pr[lineNumber] != nullptr)
        // {
        //     delete pr[lineNumber];
        // }
        pr[lineNumber] = stmt;
    }
    else
    {
        error("LINE NUMBER ERROR");
    }
}

//void Program::removeSourceLine(int lineNumber) {

Statement *Program::getParsedStatement(int lineNumber) 
{
    if(lines.find(lineNumber) != lines.end() && pr[lineNumber] != nullptr)
    {
        return pr[lineNumber];
    }
    else
    {
        return NULL;
    }
}

int Program::getFirstLineNumber() 
{
    if(lines.empty())
    {
        return -1;
    }
    else
    {
        return *(lines.begin());
    }
    
}

int Program::getNextLineNumber(int lineNumber) 
{
    auto it = lines.find(lineNumber);
    if(it != lines.end())
    {
        ++it;
        if(it != lines.end())
        {
            return *(it);
        }
    }
    return -1;
}

void Program::list()
{
    // for(int i = 0; i < lines.size(); ++i)
    // {
    //     auto it = lines.begin();
    //     int lineNumber = *it;
    //     std::cout <<  sourceLine[lineNumber] << "\n";
    //     ++it;
    // }
    for (int lineNumber : lines) 
    {
       std::cout << sourceLine[lineNumber] << "\n";
    }

}

void Program::run(EvalState &state)
{
    runLine = getFirstLineNumber();
    while(runLine != -1 && runLine <= *(lines.rbegin()))
    {
        int now = runLine;
        //if(pr[runLine] != nullptr)
        if(pr.count(runLine) > 0)
        {
            pr[runLine]->execute(state, *this);
        }
        
        if(now == runLine)
        {
            runLine = getNextLineNumber(runLine);
        }
    }
}



