//
//  main.cpp
//  Auditor
//


#include <iostream>
#include "MainConfigParser.h"

int main(int argc, char * argv[])
{

    // Read Config
    MainConfigParser mainConfigParser(argc, argv);
    if(!mainConfigParser.parse()){
        std::cout << "Configuration Parser Error" << std::endl;
    }
    
    // Startup Network
        // Contact Transaction Server (Optional)
        // Echo to BitMessage (or other communication layer) Network
    
    
    
        // https://bitmessage.org/wiki/API_Reference
    // Receive Hello from transaction server intended for Audit Server (Bitmessage message?)
}

