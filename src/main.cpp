#include <iostream>
#include "Headers/FarmApp.h"

int main(int, char**){
   

    FarmApp app;

    try
    {
        app.Run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}
