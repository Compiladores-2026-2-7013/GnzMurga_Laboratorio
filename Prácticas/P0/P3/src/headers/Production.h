#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <vector>
using namespace std;

#include "Symbol.h"

class Production
{
private:
    int head;
    vector<int> body;

public:
    Production();
    Production(int head);
    ~Production();
    
    void addSymbol(int s);
    int getHead();
    vector<int> getBody();
    int size();
    bool isEmpty();
};

#endif