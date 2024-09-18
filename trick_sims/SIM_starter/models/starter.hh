/*************************************************************************
PURPOSE: (Starter class)
LIBRARY DEPENDENCY:
    (
    (starter.cpp)
    (test.cpp)
    )
**************************************************************************/

#include "test.hh"

class Starter {
public:
    Starter();
    int default_data();
    int init();
    int scheduled();
    int deriv();
    int integ();
    int shutdown();

    BTestClass test;

};