# Interface Injection in C++ – Fintech Example

## What Is Interface Injection?

**Interface Injection** is a form of *Dependency Injection* where the dependency is passed through a setter method or an initialization method defined in an interface. The injected service implements a known interface, and the client class does not create or own the dependency—it simply uses it.

This pattern allows for loose coupling and easier testing.

---

## Fintech Context: Trade Risk Assessment

We are building a `TradeProcessor` that calculates the risk of a financial trade. The actual `RiskModel` (e.g., historical VaR, Monte Carlo simulation, etc.) is injected using interface injection.

---

## Components

* `IRiskModel` – Abstract interface for risk calculation.
* `HistoricalVaRModel` – A concrete implementation of the risk model.
* `MonteCarloRiskModel` – Another implementation.
* `Trade` – A simple trade data structure.
* `TradeProcessor` – A component that uses an `IRiskModel` to compute risk.

---

## Code Example

### `IRiskModel.h`

```cpp
#ifndef __IRiskModel_h__
#define __IRiskModel_h__

#include "Trade.h"

class IRiskModel {
public:
    virtual double calculateRisk(const Trade& trade) const = 0;
    virtual ~IRiskModel() = default;
};

#endif
```

---

### `Trade.h`

```cpp
#ifndef __Trade_h__
#define __Trade_h__

#include <string> // Required for std::string

struct Trade {
    std::string instrument;
    double notional;
    double volatility;
};

#endif
```

---

### `HistoricalVaRModel.h`

```cpp
#ifndef __HistoricalVaRModel_h__
#define __HistoricalVaRModel_h__

#include "IRiskModel.h"

class HistoricalVaRModel : public IRiskModel {
public:
    double calculateRisk(const Trade& trade) const override {
        // Simple calculation for demonstration purposes
        return trade.notional * trade.volatility * 0.01;
    }
};

#endif
```

---

### `MonteCarloRiskModel.h`

```cpp
#ifndef __MonteCarloRiskModel_h__
#define __MonteCarloRiskModel_h__

#include "IRiskModel.h"

class MonteCarloRiskModel : public IRiskModel {
public:
    double calculateRisk(const Trade& trade) const override {
        // Simple calculation for demonstration purposes
        return trade.notional * trade.volatility * 0.015;
    }
};

#endif
```

---

### `TradeProcessor.h`

```cpp
#ifndef __TradeProcessor_h__
#define __TradeProcessor_h__

#include "IRiskModel.h"
#include "Trade.h"
#include <memory> // For std::shared_ptr
#include <stdexcept> // For std::runtime_error

class TradeProcessor {
private:
    std::shared_ptr<IRiskModel> riskModel;

public:
    // Setter method for dependency injection
    void setRiskModel(std::shared_ptr<IRiskModel> model) {
        riskModel = std::move(model); // Use std::move for efficient transfer of ownership
    }

    // Method to assess risk using the injected model
    double assessRisk(const Trade& trade) const {
        if (!riskModel) {
            throw std::runtime_error("Error: Risk model not set in TradeProcessor.");
        }
        return riskModel->calculateRisk(trade);
    }
};

#endif
```

---

### `main.cpp`

```cpp
#include <iostream>
#include <string> // For std::string in Trade initialization
#include "TradeProcessor.h"
#include "HistoricalVaRModel.h"
#include "MonteCarloRiskModel.h"

int main() {
    TradeProcessor processor;
    // Initialize a Trade object
    Trade trade{"AAPL", 1000000.0, 0.2}; // Using double literals for clarity

    // Inject Historical VaR Model and calculate risk
    std::cout << "Injecting HistoricalVaRModel...\n";
    processor.setRiskModel(std::make_shared<HistoricalVaRModel>());
    try {
        std::cout << "Historical VaR Risk: " << processor.assessRisk(trade) << "\n";
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "\n"; // Newline for better readability

    // Inject Monte Carlo Model and calculate risk
    std::cout << "Injecting MonteCarloRiskModel...\n";
    processor.setRiskModel(std::make_shared<MonteCarloRiskModel>());
    try {
        std::cout << "Monte Carlo Risk: " << processor.assessRisk(trade) << "\n";
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
```

---

## Benefits of Interface Injection

* **Loosely Coupled Design**: Components are independent of concrete implementations, relying only on interfaces. This reduces dependencies and makes the system more flexible.
* **Ease of Testing**: It allows for easy substitution of dependencies with mock or stub implementations during unit testing, isolating the component under test from its real dependencies.
* **Runtime Algorithm Switching**: Different algorithms or strategies (e.g., risk models, pricing engines) can be injected at runtime without modifying the client class's code.

---

## Testability

A significant advantage of Interface Injection is improved testability. For instance, when unit testing `TradeProcessor`, you can create a `MockRiskModel` that implements `IRiskModel` and returns predefined values. This allows you to verify the behavior of `TradeProcessor` without the complexities or external dependencies of actual risk model calculations.

**Example of a Mock Risk Model for testing:**

```cpp
// MockRiskModel.h (for testing purposes)
#ifndef __MockRiskModel_h__
#define __MockRiskModel_h__

#include "IRiskModel.h"
#include "Trade.h"
#include <iostream>

class MockRiskModel : public IRiskModel {
private:
    double mockReturnValue;
public:
    MockRiskModel(double value) : mockReturnValue(value) {}

    double calculateRisk(const Trade& trade) const override {
        std::cout << "  (MOCK) Calculating risk for trade " << trade.instrument << "\n";
        return mockReturnValue;
    }
};

#endif
```

By injecting `MockRiskModel` into `TradeProcessor` during tests, you can precisely control the risk calculation outcome and assert `TradeProcessor`'s reactions.