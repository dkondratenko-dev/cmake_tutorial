#include <iostream>
#include "hello.h"

void say_hello() {
    std::cout << __FUNCTION__ << "() { Hello from 'src/' folder! }" << std::endl;
}
