#include "headers/Production.h"

Production::Production()
{
    // Constructor por defecto
}

Production::Production(int head)
{
    this->head = head;
}


Production::~Production()
{
}

void Production::addSymbol(int s)
{
    body.push_back(s);
}

int Production::getHead()
{
    return head;
}

vector<int> Production::getBody()
{
    return body;
}

int Production::size()
{
    return body.size();
}

bool Production::isEmpty()
{
    return body.empty();
}