// src/main.cpp
#include <iostream>
#include <vector>
#include <memory> // For std::unique_ptr

#include "../include/Trade.hpp" // Include Trade struct declaration
#include "../include/TradeProcessor.hpp" // Include TradeProcessor class declaration

// --- Memory Leak Examples ---

// 1. Simple Memory Leak: Forgetting to delete a dynamically allocated array.
void simpleMemoryLeak() {
    std::cout << "\n--- Running Simple Memory Leak Example ---" << std::endl;
    int* data = new int[10]; // Allocate memory for 10 integers
    std::cout << "  Allocated 10 ints. Forgetting to free it." << std::endl;
    data[0] = 100; // Use it
    // PROBLEM: No `delete[] data;` here. Memory leaks!

    // FIX (Uncomment to fix):
    // delete[] data;
    // data = nullptr;
    // std::cout << "  Freed 10 ints." << std::endl;
}

// 2. Advanced Memory Leak (Fintech Context):
//    - Multiple raw pointers returned from a function and not deleted.
//    - Overwriting a raw pointer without deleting the previously pointed-to memory.
void advancedMemoryLeak_problematic() {
    std::cout << "\n--- Running Advanced Memory Leak (Problematic) Example ---" << std::endl;
    TradeProcessor processor;

    // Simulate processing multiple trades without proper cleanup
    for (int i = 0; i < 3; ++i) {
        std::string tradeId = "PROB_TRADE_" + std::to_string(100 + i);
        Trade* t = processor.createLeakyTrade(tradeId, "AAPL", 175.50 + i, 100 + i*10);
        // PROBLEM: 't' is allocated on the heap but never deleted!
        // This is a common error where a factory function returns a raw pointer
        // and the caller doesn't take ownership or forgets to delete.
        t->display(); // Use the trade for a bit, then pointer is lost
    }

    // Another potential leak: re-assigning a raw pointer without freeing the old one
    Trade* anotherLeakyTrade = new Trade("LEAK_001", "MSFT", 400.0, 50);
    anotherLeakyTrade->setSettlementInstructions("Fast settlement required.");
    std::cout << "  [MAIN] Created another trade for potential re-assignment (LEAK_001)." << std::endl;
    delete anotherLeakyTrade;

    // PROBLEM: Overwriting `anotherLeakyTrade` pointer. The object `LEAK_001` is now leaked.
    anotherLeakyTrade = new Trade("LEAK_002", "GOOG", 150.0, 75);
    anotherLeakyTrade->setSettlementInstructions("Regular settlement.");
    std::cout << "  [MAIN] Overwritten pointer, LEAK_001 is now leaked. (Created LEAK_002)." << std::endl;

    // PROBLEM: We also forget to delete `anotherLeakyTrade` (now pointing to LEAK_002) here.
}

// Fixed version of the advanced memory leak using smart pointers.
void advancedMemoryLeak_fixed() {
    std::cout << "\n--- Running Advanced Memory Leak (FIXED) Example ---" << std::endl;
    TradeProcessor processor;

    // Using smart pointers in a vector for automatic memory management
    std::vector<std::unique_ptr<Trade>> trades;
    for (int i = 0; i < 3; ++i) {
        std::string tradeId = "FIXED_TRADE_" + std::to_string(200 + i);
        // Use the smart pointer factory method. unique_ptr handles deletion automatically.
        trades.push_back(processor.createSmartTrade(tradeId, "IBM", 200.0 + i, 50 + i*5));
        trades.back()->display();
    }
    // When 'trades' vector goes out of scope, each unique_ptr's destructor is called,
    // which in turn deletes the Trade objects. No manual deletion needed.

    // Fixing the re-assignment leak with unique_ptr
    std::unique_ptr<Trade> goodTrade = std::make_unique<Trade>("FIXED_REASSIGN_001", "AMZN", 180.0, 120);
    goodTrade->setSettlementInstructions("Smart pointer handles re-assignment.");
    std::cout << "  [MAIN] Created smart trade for re-assignment (FIXED_REASSIGN_001)." << std::endl;

    // When a unique_ptr is re-assigned, the old object it managed is automatically deleted.
    goodTrade = std::make_unique<Trade>("FIXED_REASSIGN_002", "NVDA", 1000.0, 10);
    goodTrade->setSettlementInstructions("New smart pointer assigned (FIXED_REASSIGN_002).");
    std::cout << "  [MAIN] Re-assigned smart pointer, old object properly deleted." << std::endl;

    // No explicit delete needed for `goodTrade`, as unique_ptr handles it on exit.
}

int main() {
    // Run the examples with memory leaks first
    simpleMemoryLeak();
    advancedMemoryLeak_problematic();

    std::cout << "\n--- All problematic examples finished. Memory leaks occurred. ---" << std::endl;
    std::cout << "--- Now running fixed examples to show no leaks. ---" << std::endl;

    // Run the fixed example (uncomment the line below after fixing the issues
    // and rebuilding to verify no leaks)
    advancedMemoryLeak_fixed();

    std::cout << "\n--- All examples finished. Check ASan output for leak reports. ---" << std::endl;

    return 0;
}