🟢 Phase 0: Concept Lock & Scope Freeze

Goal: Decide what you are building (no code yet).

Lock compiler idea: habit-aware, threshold-based self-correction

Fix threshold value: 3

Decide supported errors:

Missing semicolon

Missing closing brace

Decide language subset (simple C-like)

Decide memory-only auto-fix rule (no source modification)

Decide error tracking method (file-based)

✅ Output:
Clear mental model of compiler behavior.

🟢 Phase 1: Environment & Dependency Setup (Your Change)

Goal: Ensure everything needed is installed and working so coding is uninterrupted later.

What to Set Up

C compiler (GCC / Clang)

Code editor / IDE

Build tools (Make / basic compile command)

Folder structure

Basic test runner setup

OS compatibility check

Tasks

Install and verify:

gcc or clang

Standard C libraries

Create project directory structure:

hasc/
├── src/
├── include/
├── tests/
└── build/


Create a basic Makefile or compile script

Create a dummy main.c and compile it

Confirm file I/O works (read/write test file)

✅ Output:
You can compile and run a basic C program without errors.

🟢 Phase 2: Lexer (Tokenization)

Goal: Start actual compiler development.

Read source file

Tokenize keywords, identifiers, numbers, symbols

Track line and column numbers

Print token stream for debugging

✅ Output:
Correct token stream with positions.

🟢 Phase 3: Parser (Syntax Checking)

Goal: Validate program structure.

Implement recursive descent parser

Detect missing semicolons and braces

Generate structured error information

Stop compilation on syntax error (initially)

✅ Output:
Parser accepts valid code and rejects invalid code.

🟢 Phase 4: Error Codes & Central Error Handler

Goal: Standardize error reporting.

Assign unique error codes

Store error metadata:

Code

Line

Column

Centralize error printing logic

✅ Output:
Errors are consistent and machine-trackable.

🟢 Phase 5: Error History Tracking

Goal: Persist user behavior across runs.

Implement user_profile.dat

Load error counts at startup

Increment counts on error

Save counts on exit

✅ Output:
Error count increases correctly across runs.

🟢 Phase 6: Threshold Decision Logic (Core Innovation)

Goal: Change compiler behavior based on repetition.

Compare error count with threshold

Switch behavior:

Normal error

Habitual error mode

Route control flow accordingly

✅ Output:
Same error behaves differently on 3rd occurrence.

🟢 Phase 7: Temporary Auto-Correction (Memory-Only)

Goal: Allow execution without modifying source.

Clone token stream / AST

Apply fixes internally:

Insert missing ;

Insert missing }

Ensure source file is untouched

✅ Output:
Code executes successfully despite habitual error.

🟢 Phase 8: Code Highlighting

Goal: Show user exactly where the mistake is.

Print source line with error

Highlight error using ^

Display line and column number

✅ Output:
Clear visual indication of mistake.

🟢 Phase 9: User Notification & Messaging

Goal: Make compiler behavior explicit.

Print habitual mistake warning

Explain temporary correction

Ask user to manually fix code

✅ Output:
User understands what happened and why.

🟢 Phase 10: Execution Engine

Goal: Run corrected code.

Execute corrected internal representation

Print program output

Show post-execution disclaimer

✅ Output:
Program runs, source remains unchanged.

🟢 Phase 11: Stability & Cleanup

Goal: Make it solid.

Remove debug logs

Add comments

Test edge cases

Validate no source writes occur

✅ Output:
Stable, clean, demo-ready compiler.

🧠 Final Build Rule (IMPORTANT)

From Phase 2 onward, you should:

“Only write compiler logic — no environment distractions.”

Your revised Phase 1 guarantees that.