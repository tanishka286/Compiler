📄PRODUCT REQUIREMENTS DOCUMENT (PRD)
Product Name

HASC – Habit-Aware Self-Correcting Compiler

1. Purpose & Vision
1.1 Purpose

The purpose of this project is to design and implement a habit-aware compiler in the C programming language that dynamically adapts its error-handling behavior based on a user’s repetitive programming mistakes. The compiler identifies habitual errors, temporarily corrects them for execution after a defined threshold, and explicitly instructs the user to manually correct the source code.

1.2 Vision

Traditional compilers treat every error identically, regardless of repetition or user behavior. This project aims to introduce personalized compilation, where the compiler learns from user mistakes and assists execution while reinforcing correct coding practices.

2. Problem Statement

Beginner and intermediate programmers frequently commit repetitive syntax errors such as missing semicolons or braces. Existing compilers:

Repeatedly report the same errors without adaptation

Halt execution for trivial mistakes

Provide no learning-oriented feedback

There is currently no compiler system that:

Tracks user-specific error history

Identifies habitual mistakes

Applies temporary, non-destructive corrections after repeated occurrences

Highlights errors and instructs users to correct them

3. Proposed Solution

Develop a Habit-Aware Self-Correcting Compiler (HASC) that introduces threshold-based adaptive error handling.

Core Behavioral Rule
Error Occurrence Count	Compiler Behavior
1st occurrence	Normal error message
2nd occurrence	Normal error message
3rd and subsequent occurrences	Temporary internal correction + execution + highlighting + user instruction

⚠ Critical Constraint:
The original source code must never be modified.

4. Target Users
Primary Users

Programming students

Beginner and intermediate C programmers

Secondary Users

Educational institutions

Compiler research and systems students

5. Functional Requirements (FR)
FR-1: Source Code Input

The compiler shall accept a .c source file written in a restricted C-like language.

Input shall be provided via command line.

FR-2: Lexical Analysis

The compiler shall tokenize the source code.

Supported token categories include:

Keywords

Identifiers

Operators

Delimiters

Literals

FR-3: Syntax Analysis

The compiler shall perform syntax analysis using recursive descent parsing.

It shall detect syntax errors including:

Missing semicolons

Missing closing braces

Invalid statement structures

FR-4: Error Detection & Classification

Each detected error shall be assigned a unique error code.

Example error codes:

E_MISSING_SEMICOLON

E_MISSING_BRACE

FR-5: Error History Tracking

The compiler shall maintain a persistent error history for the user.

Error history shall record:

Error code

Occurrence count

Data shall be stored in a local file (e.g., user_profile.dat).

FR-6: Threshold-Based Decision Logic

Upon detecting an error, the compiler shall:

Retrieve its occurrence count

Compare it against a fixed threshold (3)

If count < 3 → normal error handling

If count ≥ 3 → habitual error handling

FR-7: Temporary Auto-Correction

For habitual errors, the compiler shall:

Apply corrections only in memory

Modify the token stream or AST copy

The source file on disk shall remain unchanged.

FR-8: Code Highlighting

When a habitual error is detected, the compiler shall:

Display the line number of the error

Print the exact line of source code

Visually highlight the erroneous token using markers (e.g., ^)

Example:

Line 4:
int a = 10
        ^ Missing semicolon

FR-9: Habitual Error Notification & User Instruction

The compiler shall explicitly notify the user that:

The error is habitual

A temporary internal correction has been applied

The source code is still incorrect

The user must manually correct the mistake

FR-10: Before/After Representation

The compiler shall display:

The original erroneous code

The internally corrected version used for execution

FR-11: Program Execution

The compiler shall execute the program using the corrected internal representation.

Execution shall succeed despite habitual errors.

FR-12: Post-Execution Disclaimer

After execution, the compiler shall display:

“The source code was not modified. Please correct the highlighted habitual mistake.”

6. Non-Functional Requirements (NFR)
NFR-1: Language Constraint

The entire compiler shall be implemented strictly in C language.

NFR-2: Performance

Error history lookup shall be completed in O(n) time or better.

Additional compilation overhead shall be minimal.

NFR-3: Reliability

The compiler shall not crash on malformed input.

Auto-correction shall apply only to predefined error types.

NFR-4: Portability

The compiler shall run on Linux and Windows systems.

NFR-5: Security

No unsafe system calls shall be executed.

File I/O shall be limited to the project directory.

7. Supported Language Features (Initial Scope)
Supported Constructs

Variable declarations

Assignment statements

Arithmetic expressions

if-else

while loops

print statements

Supported Habitual Errors

Missing semicolon (;)

Missing closing brace (})

Undeclared variable (optional)

8. System Architecture
High-Level Module Flow
Source Code
     ↓
Lexical Analyzer
     ↓
Parser
     ↓
Error Detection
     ↓
Error History Tracker
     ↓
Threshold Decision Logic
     ↓
Temporary Auto-Fix Engine
     ↓
Code Highlighter & Notifier
     ↓
Executor

9. Data Design
Error Record Structure
typedef struct {
    char error_code[50];
    int count;
} ErrorRecord;

User Profile File Format
E_MISSING_SEMICOLON 3
E_MISSING_BRACE 1

10. User Experience (CLI Output)
Normal Error (1st & 2nd Occurrence)
Syntax Error: Missing semicolon at line 4

Habitual Error (3rd Occurrence and Beyond)
⚠ Habitual Mistake Detected

Error Type: Missing Semicolon
Occurrences: 3
Line: 4

Code:
int a = 10
        ^ Missing semicolon

Temporary internal correction applied:
int a = 10;

The program is running using the corrected version.
⚠ Please correct this mistake in your source code.

11. Constraints & Assumptions
Constraints

No GUI

No external libraries

No source file modification

Assumptions

Single-user environment

Educational usage

Controlled input programs

12. Risks & Mitigation
Risk	Mitigation
Patent eligibility risk	File provisional patent before publishing
Auto-fix complexity	Limit to simple syntax errors
Scope expansion	Strict language subset
13. Success Criteria

The project is considered successful if:

Repetitive errors are tracked correctly

Auto-correction triggers only after threshold

Errors are highlighted clearly

User is instructed to fix the source code

Program executes without modifying the source file

14. Future Enhancements (Out of Scope)

Machine learning-based habit detection

IDE integration

Multi-user profiles

Natural language explanations

15. Patent Alignment Statement

This project introduces a novel compiler system that:

Detects habitual programming errors

Applies threshold-based adaptive compilation behavior

Temporarily corrects errors without modifying source code

Visually highlights mistakes and instructs users to correct them

These elements collectively define a patent-eligible method and system for behavior-aware compilation