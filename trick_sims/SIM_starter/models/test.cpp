#include "test.hh"
#include "trick/MemoryManager.hh"
#include <string>

extern Trick::MemoryManager* trick_MM;

BTestClass::BTestClass()
{
   
    for (size_t i = 0; i < 5; i++)
    {
        classes_dyn.emplace_back(new SomeStruct());
        std::string name = "SomeStruct trick_anon_extern_";
        name = name + std::to_string(i);

        name = "SomeStruct";

        trick_MM->declare_extern_var(classes_dyn[i], name.c_str());
    }


    std::cout << "Hello?\n";
}