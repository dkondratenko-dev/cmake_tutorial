# Static Code Analysis in C++ for Fintech

## Why Static Code Analysis?

Static code analysis tools help detect potential bugs, security vulnerabilities, or performance issues *before* running the code. In the fintech domain, correctness, precision, and performance are critical.

## Recommended Tools

### ✅ Clang-Tidy

#### How to Run `clang-tidy` on the Whole Project

##### Step 1: Configure the Project

```bash
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

This generates `build/compile_commands.json`, which `clang-tidy` requires to understand include paths and compiler options.

##### Step 2: Run Automatically During Build

If `clang-tidy` is installed, it runs **automatically** when you build:

```bash
cmake --build build
```

You will see tidy output during compilation.

##### Step 3: Run Manually via Dedicated Target

If you want to run tidy *without* recompiling:

```bash
cmake --build build --target clang-tidy-all
```

This runs `clang-tidy` explicitly over `main.cpp`, `FinanceCalculator.cpp`, and your test file.

### ✅ Cppcheck

From the root folder - **13-static-code-analisys** run:

```bash
cppcheck --enable=all --inconclusive --std=c++17 -Iinclude src include
```

or to supress missing system includes:

```bash
cppcheck --enable=all --inconclusive --std=c++17 -Iinclude --suppress=missingIncludeSystem src include
```

## Generationg report from cppcheck

From your `cmake-examples/13-static-code-analisys` directory:

### Generate XML

```bash
cppcheck --enable=all --inconclusive --std=c++17 -Iinclude src include --xml-version=2 2> cppcheck_report.xml
```

### Generate HTML

(First, find your `cppcheck-htmlreport` script. Let's assume it's in `/usr/share/cppcheck/htmlreport/`)

```bash
cppcheck-htmlreport --file=cppcheck_report.xml --report-dir=cppcheck_html_report --source-dir=. --title="My CMake Example Project"
```

### Then, open the report

```bash
xdg-open cppcheck_html_report/index.html # On Linux
# or
open cppcheck_html_report/index.html # On macOS
# or navigate manually in a file explorer
```

This two-step process is the most common and effective way to get meaningful `cppcheck` reports.

### ✅ Include-What-You-Use

```bash
iwyu_tool.py -p build
```

## Best Practices

1. **Enable All Warnings**
   - Use  in your compiler flags.
2. **Treat Warnings as Errors**
   - helps enforce discipline.
3. **Use Modern C++**
   - Prefer , range-for, smart pointers (, )
4. **Code Reviews with Analysis**
   - Run static analysis before every pull request.
5. **Integrate in CI**
   - Add clang-tidy/cppcheck in your CI pipeline (GitHub Actions, GitLab CI).
6. **Custom Rules for Finance**
   - E.g., forbid use of  in monetary calculations.

## Sample .clang-tidy

```yaml
Checks: '-*,bugprone-*,modernize-*,performance-*,readability-*'
WarningsAsErrors: ['bugprone-*']
HeaderFilterRegex: '.*'
FormatStyle: file
```
