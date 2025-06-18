// src/main.cpp
#include <iostream>
#include <vector>
#include <string> // For std::to_string
#include <memory> // For std::unique_ptr, std::shared_ptr, std::weak_ptr

// Include our custom classes/structs
#include "../include/Trade.hpp"
#include "../include/TradeProcessor.hpp"
#include "../include/TradingAccount.hpp"   // New
#include "../include/TradingStrategy.hpp"  // New

// --- Existing Memory Leak Examples ---

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
    std::cout << "\n--- Running Advanced Memory Leak (Problematic Raw Pointers) Example ---" << std::endl;
    TradeProcessor processor;

    // Simulate processing multiple trades without proper cleanup
    for (int i = 0; i < 3; ++i) {
        std::string tradeId = "PROB_TRADE_" + std::to_string(100 + i);
        Trade* t = processor.createLeakyTrade(tradeId, "AAPL", 175.50 + i, 100 + i*10);
        // PROBLEM: 't' is allocated on the heap but never deleted!
        t->display(); // Use the trade for a bit, then pointer is lost
    }

    // Another potential leak: re-assigning a raw pointer without freeing the old one
    Trade* anotherLeakyTrade = new Trade("LEAK_001", "MSFT", 400.0, 50);
    anotherLeakyTrade->setSettlementInstructions("Fast settlement required.");
    std::cout << "  [MAIN] Created another trade for potential re-assignment (LEAK_001)." << std::endl;

    // PROBLEM: Overwriting `anotherLeakyTrade` pointer. The object `LEAK_001` is now leaked.
    anotherLeakyTrade = new Trade("LEAK_002", "GOOG", 150.0, 75);
    anotherLeakyTrade->setSettlementInstructions("Regular settlement.");
    std::cout << "  [MAIN] Overwritten pointer, LEAK_001 is now leaked. (Created LEAK_002)." << std::endl;

    // PROBLEM: We also forget to delete `anotherLeakyTrade` (now pointing to LEAK_002) here.
}

// 2. Advanced Memory Leak (Raw Pointers Fixed)
void advancedMemoryLeak_fixed() {
    std::cout << "\n--- Running Advanced Memory Leak (Fixed Raw Pointers) Example ---" << std::endl;
    TradeProcessor processor;

    // Using smart pointers in a vector for automatic memory management
    std::vector<std::unique_ptr<Trade>> trades;
    for (int i = 0; i < 3; ++i) {
        std::string tradeId = "FIXED_TRADE_" + std::to_string(200 + i);
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
}

// --- NEW ADVANCED LEAK: std::shared_ptr Circular Reference ---

// Problematic example: Creates a circular shared_ptr dependency
void sharedPtrCircularLeak_problematic() {
    std::cout << "\n--- Running Shared Pointer Circular Leak (Problematic) Example ---" << std::endl;

    // Create shared pointers for an account and a strategy
    std::shared_ptr<TradingAccount> account = std::make_shared<TradingAccount>("ACC_SP_001");
    std::shared_ptr<TradingStrategy> strategy = std::make_shared<TradingStrategy>("STRAT_SP_A");

    // Form the cycle:
    // account -> strategy
    // strategy -> account
    account->setStrategy(strategy);                     // Account holds shared_ptr to Strategy
    strategy->linkAccountProblematic(account);          // Strategy holds shared_ptr to Account

    // Both 'account' and 'strategy' will go out of scope here.
    // However, because they hold shared_ptrs to each other, their reference counts
    // will never drop to zero (they will both be 1), preventing their deletion.
    // This leads to a memory leak.
    std::cout << "  Shared pointers for ACC_SP_001 and STRAT_SP_A going out of scope." << std::endl;
    std::cout << "  Reference count for account (before exit): " << account.use_count() << std::endl;
    std::cout << "  Reference count for strategy (before exit): " << strategy.use_count() << std::endl;
}

// Fixed example: Breaks the shared_ptr cycle using std::weak_ptr
void sharedPtrCircularLeak_fixed() {
    std::cout << "\n--- Running Shared Pointer Circular Leak (FIXED) Example ---" << std::endl;

    // Create shared pointers for an account and a strategy
    std::shared_ptr<TradingAccount> account = std::make_shared<TradingAccount>("ACC_WP_001");
    std::shared_ptr<TradingStrategy> strategy = std::make_shared<TradingStrategy>("STRAT_WP_B");

    // Form the link:
    // account -> shared_ptr to Strategy
    // strategy -> weak_ptr to Account (does NOT increase reference count)
    account->setStrategy(strategy);                     // Account holds shared_ptr to Strategy (ref count +1 for strategy)
    strategy->linkAccountFixed(account);                // Strategy holds weak_ptr to Account (ref count for account does NOT change)

    // Now, when 'account' goes out of scope, its reference count will drop to zero
    // (since only 'account' itself holds a strong reference). 'account' will be destroyed.
    // Then 'strategy's reference count will drop to zero and it will be destroyed.
    std::cout << "  Shared pointers for ACC_WP_001 and STRAT_WP_B going out of scope." << std::endl;
    std::cout << "  Reference count for account (before exit): " << account.use_count() << std::endl;
    std::cout << "  Reference count for strategy (before exit): " << strategy.use_count() << std::endl;
}


int main() {
    // Run the examples with memory leaks first
    simpleMemoryLeak();
    advancedMemoryLeak_problematic();
    sharedPtrCircularLeak_problematic(); // The new problematic example

    std::cout << "\n--- All problematic examples finished. Memory leaks occurred. ---" << std::endl;
    std::cout << "--- Now running fixed examples to show no leaks. ---" << std::endl;

    // Run the fixed examples (uncomment after fixing the simple leak
    // and rebuilding to verify no leaks)
    // simpleMemoryLeak(); // Uncomment if you applied the fix inside the function
    advancedMemoryLeak_fixed();
    sharedPtrCircularLeak_fixed(); // The new fixed example

    std::cout << "\n--- All examples finished. Check ASan output for leak reports. ---" << std::endl;

    return 0;
}