#pragma once
#include <iostream>

namespace DiamondProblem {

// A simple demonstration of the memory layout in virtual inheritance (the "diamond problem").
// This is where v-base offsets are used by the compiler.

struct Base {
    // This adds a vptr to Base and all derived classes, making the layout more explicit.
    virtual ~Base() = default; 
    long b_data = 0xAAAAAAAAAAAAAAAA;
};

struct Derived1 : virtual Base { long d1_data = 0xBEEFBEEFBEEFBEEF; };
struct Derived2 : virtual Base { long d2_data = 0xCAFECAFECAFECAFE; };

// Diamond: Final inherits from two classes that share a virtual base.
struct Final : Derived1, Derived2 { long f_data = 0xDEADBEEFDEADBEEF; };

void run_demo() {
    std::cout << "\n--- V-Base Offset Demonstration ---\n";
    std::cout << "Size of Base: " << sizeof(Base) << " (vptr + long)\n";
    std::cout << "Size of Derived1: " << sizeof(Derived1) << " (vptr + long + vbase overhead)\n";
    std::cout << "Size of Derived2: " << sizeof(Derived2) << " (vptr + long + vbase overhead)\n";
    
    // The size of Final is NOT sizeof(D1) + sizeof(D2).
    // It contains one Base, one D1, one D2, and one Final part.
    // The compiler adds pointers/offsets to manage the single shared `Base` subobject.
    std::cout << "Size of Final: " << sizeof(Final) << "\n";
    
    Final f;
    Base* b_ptr = &f; // This upcast requires a runtime calculation
                      // using the v-base offset to find the location of the `Base` subobject.
    
    std::cout << "Address of final object: " << &f << "\n";
    std::cout << "Address of Base subobject via cast: " << b_ptr << "\n";
    std::cout << "The offset between them is managed via the v-table/v-base-offset mechanism.\n";
}

} // namespace VbaseDemo