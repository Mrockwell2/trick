/*************************************************************************
PURPOSE: (Starter class)
LIBRARY DEPENDENCY:
    (
    (test.cpp)
    )
**************************************************************************/

#pragma once

#include <string>
#include <vector>

struct SomeStruct
{
    double x{5.0};
    std::string name{"Default Name"};
};

class BTestClass
{
    public:
    BTestClass();
    
    ~BTestClass(){};

    public:
    std::vector<SomeStruct*> classes_dyn;

    

};