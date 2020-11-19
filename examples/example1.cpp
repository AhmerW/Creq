#include <iostream> 
#include "creq.h"

// COMPILEC COMMAND (gnu compiler)
// g++ example1.cpp C:\Users\Ahmer\Documents\GitHub\Creq\creq\creq.cpp -IC:\Users\Ahmer\Documents\GitHub\Creq\creq -lws2_32


int main(int argc, char* argv[]){
    using namespace creq;
    Creq obj = Creq();
    Response res = obj.get("http://example.com");
    

    return 0;
}
