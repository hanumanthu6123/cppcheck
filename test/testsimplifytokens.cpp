/*
 * Cppcheck - A tool for static C/C++ code analysis
 * Copyright (C) 2007-2022 Cppcheck team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "errortypes.h"
#include "platform.h"
#include "settings.h"
#include "standards.h"
#include "testsuite.h"
#include "token.h"
#include "tokenize.h"

#include <istream>
#include <string>


class TestSimplifyTokens : public TestFixture {
public:
    TestSimplifyTokens() : TestFixture("TestSimplifyTokens") {}


private:
    Settings settings0;
    Settings settings1;
    Settings settings_std;
    Settings settings_windows;

    void run() override {
        LOAD_LIB_2(settings_std.library, "std.cfg");
        LOAD_LIB_2(settings_windows.library, "windows.cfg");
        settings0.severity.enable(Severity::portability);
        settings1.severity.enable(Severity::style);
        settings_windows.severity.enable(Severity::portability);

        // If there are unused templates, keep those
        settings0.checkUnusedTemplates = true;
        settings1.checkUnusedTemplates = true;
        settings_std.checkUnusedTemplates = true;
        settings_windows.checkUnusedTemplates = true;

        TEST_CASE(combine_strings);
        TEST_CASE(combine_wstrings);
        TEST_CASE(combine_ustrings);
        TEST_CASE(combine_Ustrings);
        TEST_CASE(combine_u8strings);
        TEST_CASE(combine_mixedstrings);

        TEST_CASE(double_plus);
        TEST_CASE(redundant_plus);
        TEST_CASE(redundant_plus_numbers);
        TEST_CASE(declareVar);

        TEST_CASE(declareArray);

        TEST_CASE(dontRemoveIncrement);

        TEST_CASE(elseif1);

        TEST_CASE(namespaces);

        // Simplify "not" to "!" (#345)
        TEST_CASE(not1);

        // Simplify "and" to "&&" (#620)
        TEST_CASE(and1);

        // Simplify "or" to "||"
        TEST_CASE(or1);

        TEST_CASE(cAlternativeTokens);

        TEST_CASE(comma_keyword);
        TEST_CASE(remove_comma);

        // Simplify "?:"
        TEST_CASE(simplifyConditionOperator);

        TEST_CASE(simplifyOperator1);
        TEST_CASE(simplifyOperator2);

        TEST_CASE(simplifyArrayAccessSyntax);

        TEST_CASE(pointeralias1);
        TEST_CASE(pointeralias3);

        // struct ABC { } abc; => struct ABC { }; ABC abc;
        TEST_CASE(simplifyStructDecl1);
        TEST_CASE(simplifyStructDecl2); // ticket #2579
        TEST_CASE(simplifyStructDecl3);
        TEST_CASE(simplifyStructDecl4);
        TEST_CASE(simplifyStructDecl6); // ticket #3732
        TEST_CASE(simplifyStructDecl7); // ticket #476 (static anonymous struct array)
        TEST_CASE(simplifyStructDecl8); // ticket #7698

        // register int var; => int var;
        // inline int foo() {} => int foo() {}
        TEST_CASE(removeUnwantedKeywords);

        // remove calling convention __cdecl, __stdcall, ...
        TEST_CASE(simplifyCallingConvention);

        // remove __attribute, __attribute__
        TEST_CASE(simplifyAttribute);

        TEST_CASE(simplifyFunctorCall);

        TEST_CASE(simplifyFunctionPointer); // ticket #5339 (simplify function pointer after comma)

        TEST_CASE(simplifyFunctionReturn);

        TEST_CASE(consecutiveBraces);

        TEST_CASE(simplifyOverride); // ticket #5069
        TEST_CASE(simplifyNestedNamespace);
        TEST_CASE(simplifyNamespaceAliases1);
        TEST_CASE(simplifyNamespaceAliases2); // ticket #10281

        TEST_CASE(simplifyKnownVariables2);
        TEST_CASE(simplifyKnownVariables3);
        TEST_CASE(simplifyKnownVariables4);
        TEST_CASE(simplifyKnownVariables5);
        TEST_CASE(simplifyKnownVariables13);
        TEST_CASE(simplifyKnownVariables14);
        TEST_CASE(simplifyKnownVariables16);
        TEST_CASE(simplifyKnownVariables17);
        TEST_CASE(simplifyKnownVariables18);
        TEST_CASE(simplifyKnownVariables19);
        TEST_CASE(simplifyKnownVariables21);
        TEST_CASE(simplifyKnownVariables25);
        // FIXME Does expression id handle these? TEST_CASE(simplifyKnownVariables29);    // ticket #1811
        TEST_CASE(simplifyKnownVariables30);
        TEST_CASE(simplifyKnownVariables34);
        TEST_CASE(simplifyKnownVariables36);    // ticket #2304 - known value for strcpy parameter
        TEST_CASE(simplifyKnownVariables42);    // ticket #2031 - known string value after strcpy
        TEST_CASE(simplifyKnownVariables43);
        TEST_CASE(simplifyKnownVariables44);    // ticket #3117 - don't simplify static variables
        TEST_CASE(simplifyKnownVariables46);    // ticket #3587 - >>
        TEST_CASE(simplifyKnownVariables47);    // ticket #3627 - >>
        TEST_CASE(simplifyKnownVariables48);    // ticket #3754 - wrong simplification in for loop header
        TEST_CASE(simplifyKnownVariables49);    // #3691 - continue in switch
        TEST_CASE(simplifyKnownVariables50);    // #4066 sprintf changes
        TEST_CASE(simplifyKnownVariables51);    // #4409 hang
        TEST_CASE(simplifyKnownVariables54);    // #4913 'x' is not 0 after *--x=0;
        TEST_CASE(simplifyKnownVariables56);    // ticket #5301 - >>
        TEST_CASE(simplifyKnownVariables58);    // ticket #5268
        TEST_CASE(simplifyKnownVariables59);    // skip for header
        TEST_CASE(simplifyKnownVariables61);    // #7805
        TEST_CASE(simplifyKnownVariables62);    // #5666 - p=&str[0]
        TEST_CASE(simplifyKnownVariables63);    // #10798
        TEST_CASE(simplifyKnownVariablesBailOutAssign1);
        TEST_CASE(simplifyKnownVariablesBailOutAssign2);
        TEST_CASE(simplifyKnownVariablesBailOutFor1);
        TEST_CASE(simplifyKnownVariablesBailOutFor2);
        TEST_CASE(simplifyKnownVariablesBailOutFor3);
        TEST_CASE(simplifyKnownVariablesBailOutMemberFunction);
        TEST_CASE(simplifyKnownVariablesBailOutConditionalIncrement);
        TEST_CASE(simplifyKnownVariablesBailOutSwitchBreak); // ticket #2324
        TEST_CASE(simplifyKnownVariablesClassMember);  // #2815 - value of class member may be changed by function call
        TEST_CASE(simplifyKnownVariablesFunctionCalls); // Function calls (don't assume pass by reference)
        TEST_CASE(simplifyKnownVariablesGlobalVars);
        TEST_CASE(simplifyKnownVariablesNamespace); // #10059

        TEST_CASE(simplify_constants6);     // Ticket #5625: Ternary operator as template parameter
        TEST_CASE(simplifyVarDeclInitLists);
    }

#define tok(...) tok_(__FILE__, __LINE__, __VA_ARGS__)
    std::string tok_(const char* file, int line, const char code[], bool simplify = true, Settings::PlatformType type = Settings::Native) {
        errout.str("");

        settings0.platform(type);
        Tokenizer tokenizer(&settings0, this);

        std::istringstream istr(code);
        ASSERT_LOC(tokenizer.tokenize(istr, "test.cpp"), file, line);

        if (simplify)
            tokenizer.simplifyTokenList2();

        return tokenizer.tokens()->stringifyList(nullptr, !simplify);
    }

    std::string tok_(const char* file, int line, const char code[], const char filename[], bool simplify = true) {
        errout.str("");

        Tokenizer tokenizer(&settings0, this);

        std::istringstream istr(code);
        ASSERT_LOC(tokenizer.tokenize(istr, filename), file, line);
        if (simplify)
            tokenizer.simplifyTokenList2();

        return tokenizer.tokens()->stringifyList(nullptr, false);
    }

#define tokWithNewlines(code) tokWithNewlines_(code, __FILE__, __LINE__)
    std::string tokWithNewlines_(const char code[], const char* file, int line) {
        errout.str("");

        Tokenizer tokenizer(&settings0, this);

        std::istringstream istr(code);
        ASSERT_LOC(tokenizer.tokenize(istr, "test.cpp"), file, line);
        tokenizer.simplifyTokenList2();

        return tokenizer.tokens()->stringifyList(false, false, false, true, false);
    }

#define tokenizeAndStringify(...) tokenizeAndStringify_(__FILE__, __LINE__, __VA_ARGS__)
    std::string tokenizeAndStringify_(const char* file, int linenr, const char code[], bool simplify = false, bool expand = true, Settings::PlatformType platform = Settings::Native, const char* filename = "test.cpp", bool cpp11 = true) {
        errout.str("");

        settings1.debugwarnings = true;
        settings1.platform(platform);
        settings1.standards.cpp = cpp11 ? Standards::CPP11 : Standards::CPP03;

        // tokenize..
        Tokenizer tokenizer(&settings1, this);
        std::istringstream istr(code);
        ASSERT_LOC(tokenizer.tokenize(istr, filename), file, linenr);
        if (simplify)
            tokenizer.simplifyTokenList2();

        // filter out ValueFlow messages..
        const std::string debugwarnings = errout.str();
        errout.str("");
        std::istringstream istr2(debugwarnings);
        std::string line;
        while (std::getline(istr2,line)) {
            if (line.find("valueflow.cpp") == std::string::npos)
                errout << line << "\n";
        }

        if (tokenizer.tokens())
            return tokenizer.tokens()->stringifyList(false, expand, false, true, false, nullptr, nullptr);
        else
            return "";
    }

#define tokenizeDebugListing(...) tokenizeDebugListing_(__FILE__, __LINE__, __VA_ARGS__)
    std::string tokenizeDebugListing_(const char* file, int line, const char code[], bool simplify = false, const char filename[] = "test.cpp") {
        errout.str("");

        Tokenizer tokenizer(&settings0, this);
        std::istringstream istr(code);
        ASSERT_LOC(tokenizer.tokenize(istr, filename), file, line);

        if (simplify)
            tokenizer.simplifyTokenList2();

        // result..
        return tokenizer.tokens()->stringifyList(true);
    }


    void simplifyMathFunctions_erfc() {
        // verify erfc(), erfcf(), erfcl() - simplifcation
        const char code_erfc[] ="void f(int x) {\n"
                                 " std::cout << erfc(x);\n" // do not simplify
                                 " std::cout << erfc(0L);\n" // simplify to 1
                                 "}";
        const char expected_erfc[] = "void f ( int x ) {\n"
                                     "std :: cout << erfc ( x ) ;\n"
                                     "std :: cout << 1 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_erfc, tokWithNewlines(code_erfc));

        const char code_erfcf[] ="void f(float x) {\n"
                                  " std::cout << erfcf(x);\n" // do not simplify
                                  " std::cout << erfcf(0.0f);\n" // simplify to 1
                                  "}";
        const char expected_erfcf[] = "void f ( float x ) {\n"
                                      "std :: cout << erfcf ( x ) ;\n"
                                      "std :: cout << 1 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_erfcf, tokWithNewlines(code_erfcf));

        const char code_erfcl[] ="void f(long double x) {\n"
                                  " std::cout << erfcl(x);\n" // do not simplify
                                  " std::cout << erfcl(0.0f);\n" // simplify to 1
                                  "}";
        const char expected_erfcl[] = "void f ( double x ) {\n"
                                      "std :: cout << erfcl ( x ) ;\n"
                                      "std :: cout << 1 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_erfcl, tokWithNewlines(code_erfcl));
    }

    void simplifyMathFunctions_cos() {
        // verify cos(), cosf(), cosl() - simplifcation
        const char code_cos[] ="void f(int x) {\n"
                                " std::cout << cos(x);\n" // do not simplify
                                " std::cout << cos(0L);\n" // simplify to 1
                                "}";
        const char expected_cos[] = "void f ( int x ) {\n"
                                    "std :: cout << cos ( x ) ;\n"
                                    "std :: cout << 1 ;\n"
                                    "}";
        ASSERT_EQUALS(expected_cos, tokWithNewlines(code_cos));

        const char code_cosf[] ="void f(float x) {\n"
                                 " std::cout << cosf(x);\n" // do not simplify
                                 " std::cout << cosf(0.0f);\n" // simplify to 1
                                 "}";
        const char expected_cosf[] = "void f ( float x ) {\n"
                                     "std :: cout << cosf ( x ) ;\n"
                                     "std :: cout << 1 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_cosf, tokWithNewlines(code_cosf));

        const char code_cosl[] ="void f(long double x) {\n"
                                 " std::cout << cosl(x);\n" // do not simplify
                                 " std::cout << cosl(0.0f);\n" // simplify to 1
                                 "}";
        const char expected_cosl[] = "void f ( double x ) {\n"
                                     "std :: cout << cosl ( x ) ;\n"
                                     "std :: cout << 1 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_cosl, tokWithNewlines(code_cosl));
    }

    void simplifyMathFunctions_cosh() {
        // verify cosh(), coshf(), coshl() - simplifcation
        const char code_cosh[] ="void f(int x) {\n"
                                 " std::cout << cosh(x);\n" // do not simplify
                                 " std::cout << cosh(0L);\n" // simplify to 1
                                 "}";
        const char expected_cosh[] = "void f ( int x ) {\n"
                                     "std :: cout << cosh ( x ) ;\n"
                                     "std :: cout << 1 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_cosh, tokWithNewlines(code_cosh));

        const char code_coshf[] ="void f(float x) {\n"
                                  " std::cout << coshf(x);\n" // do not simplify
                                  " std::cout << coshf(0.0f);\n" // simplify to 1
                                  "}";
        const char expected_coshf[] = "void f ( float x ) {\n"
                                      "std :: cout << coshf ( x ) ;\n"
                                      "std :: cout << 1 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_coshf, tokWithNewlines(code_coshf));

        const char code_coshl[] ="void f(long double x) {\n"
                                  " std::cout << coshl(x);\n" // do not simplify
                                  " std::cout << coshl(0.0f);\n" // simplify to 1
                                  "}";
        const char expected_coshl[] = "void f ( double x ) {\n"
                                      "std :: cout << coshl ( x ) ;\n"
                                      "std :: cout << 1 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_coshl, tokWithNewlines(code_coshl));
    }

    void simplifyMathFunctions_acos() {
        // verify acos(), acosf(), acosl() - simplifcation
        const char code_acos[] ="void f(int x) {\n"
                                 " std::cout << acos(x);\n" // do not simplify
                                 " std::cout << acos(1L);\n" // simplify to 0
                                 "}";
        const char expected_acos[] = "void f ( int x ) {\n"
                                     "std :: cout << acos ( x ) ;\n"
                                     "std :: cout << 0 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_acos, tokWithNewlines(code_acos));

        const char code_acosf[] ="void f(float x) {\n"
                                  " std::cout << acosf(x);\n" // do not simplify
                                  " std::cout << acosf(1.0f);\n" // simplify to 0
                                  "}";
        const char expected_acosf[] = "void f ( float x ) {\n"
                                      "std :: cout << acosf ( x ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_acosf, tokWithNewlines(code_acosf));

        const char code_acosl[] ="void f(long double x) {\n"
                                  " std::cout << acosl(x);\n" // do not simplify
                                  " std::cout << acosl(1.0f);\n" // simplify to 0
                                  "}";
        const char expected_acosl[] = "void f ( double x ) {\n"
                                      "std :: cout << acosl ( x ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_acosl, tokWithNewlines(code_acosl));
    }

    void simplifyMathFunctions_acosh() {
        // verify acosh(), acoshf(), acoshl() - simplifcation
        const char code_acosh[] ="void f(int x) {\n"
                                  " std::cout << acosh(x);\n" // do not simplify
                                  " std::cout << acosh(1L);\n" // simplify to 0
                                  "}";
        const char expected_acosh[] = "void f ( int x ) {\n"
                                      "std :: cout << acosh ( x ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_acosh, tokWithNewlines(code_acosh));

        const char code_acoshf[] ="void f(float x) {\n"
                                   " std::cout << acoshf(x);\n" // do not simplify
                                   " std::cout << acoshf(1.0f);\n" // simplify to 0
                                   "}";
        const char expected_acoshf[] = "void f ( float x ) {\n"
                                       "std :: cout << acoshf ( x ) ;\n"
                                       "std :: cout << 0 ;\n"
                                       "}";
        ASSERT_EQUALS(expected_acoshf, tokWithNewlines(code_acoshf));

        const char code_acoshl[] ="void f(long double x) {\n"
                                   " std::cout << acoshl(x);\n" // do not simplify
                                   " std::cout << acoshl(1.0f);\n" // simplify to 0
                                   "}";
        const char expected_acoshl[] = "void f ( double x ) {\n"
                                       "std :: cout << acoshl ( x ) ;\n"
                                       "std :: cout << 0 ;\n"
                                       "}";
        ASSERT_EQUALS(expected_acoshl, tokWithNewlines(code_acoshl));
    }

    void simplifyMathFunctions_sqrt() {
        // verify sqrt(), sqrtf(), sqrtl() - simplifcation
        const char code_sqrt[] ="void f(int x) {\n"
                                 " std::cout << sqrt(x);\n" // do not simplify
                                 " std::cout << sqrt(-1);\n" // do not simplify
                                 " std::cout << sqrt(0L);\n" // simplify to 0
                                 " std::cout << sqrt(1L);\n" // simplify to 1
                                 "}";
        const char expected_sqrt[] = "void f ( int x ) {\n"
                                     "std :: cout << sqrt ( x ) ;\n"
                                     "std :: cout << sqrt ( -1 ) ;\n"
                                     "std :: cout << 0 ;\n"
                                     "std :: cout << 1 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_sqrt, tokWithNewlines(code_sqrt));

        const char code_sqrtf[] ="void f(float x) {\n"
                                  " std::cout << sqrtf(x);\n" // do not simplify
                                  " std::cout << sqrtf(-1.0f);\n" // do not simplify
                                  " std::cout << sqrtf(0.0f);\n" // simplify to 0
                                  " std::cout << sqrtf(1.0);\n" // simplify to 1
                                  "}";
        const char expected_sqrtf[] = "void f ( float x ) {\n"
                                      "std :: cout << sqrtf ( x ) ;\n"
                                      "std :: cout << sqrtf ( -1.0f ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "std :: cout << 1 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_sqrtf, tokWithNewlines(code_sqrtf));

        const char code_sqrtl[] ="void f(long double x) {\n"
                                  " std::cout << sqrtf(x);\n" // do not simplify
                                  " std::cout << sqrtf(-1.0);\n" // do not simplify
                                  " std::cout << sqrtf(0.0);\n" // simplify to 0
                                  " std::cout << sqrtf(1.0);\n" // simplify to 1
                                  "}";
        const char expected_sqrtl[] = "void f ( double x ) {\n"
                                      "std :: cout << sqrtf ( x ) ;\n"
                                      "std :: cout << sqrtf ( -1.0 ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "std :: cout << 1 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_sqrtl, tokWithNewlines(code_sqrtl));
    }

    void simplifyMathFunctions_cbrt() {
        // verify cbrt(), cbrtf(), cbrtl() - simplifcation
        const char code_cbrt[] ="void f(int x) {\n"
                                 " std::cout << cbrt(x);\n" // do not simplify
                                 " std::cout << cbrt(-1);\n" // do not simplify
                                 " std::cout << cbrt(0L);\n" // simplify to 0
                                 " std::cout << cbrt(1L);\n" // simplify to 1
                                 "}";
        const char expected_cbrt[] = "void f ( int x ) {\n"
                                     "std :: cout << cbrt ( x ) ;\n"
                                     "std :: cout << cbrt ( -1 ) ;\n"
                                     "std :: cout << 0 ;\n"
                                     "std :: cout << 1 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_cbrt, tokWithNewlines(code_cbrt));

        const char code_cbrtf[] ="void f(float x) {\n"
                                  " std::cout << cbrtf(x);\n" // do not simplify
                                  " std::cout << cbrtf(-1.0f);\n" // do not simplify
                                  " std::cout << cbrtf(0.0f);\n" // simplify to 0
                                  " std::cout << cbrtf(1.0);\n" // simplify to 1
                                  "}";
        const char expected_cbrtf[] = "void f ( float x ) {\n"
                                      "std :: cout << cbrtf ( x ) ;\n"
                                      "std :: cout << cbrtf ( -1.0f ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "std :: cout << 1 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_cbrtf, tokWithNewlines(code_cbrtf));

        const char code_cbrtl[] ="void f(long double x) {\n"
                                  " std::cout << cbrtl(x);\n" // do not simplify
                                  " std::cout << cbrtl(-1.0);\n" // do not simplify
                                  " std::cout << cbrtl(0.0);\n" // simplify to 0
                                  " std::cout << cbrtl(1.0);\n" // simplify to 1
                                  "}";
        const char expected_cbrtl[] = "void f ( double x ) {\n"
                                      "std :: cout << cbrtl ( x ) ;\n"
                                      "std :: cout << cbrtl ( -1.0 ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "std :: cout << 1 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_cbrtl, tokWithNewlines(code_cbrtl));
    }

    void simplifyMathFunctions_exp2() {
        // verify exp2(), exp2f(), exp2l() - simplifcation
        const char code_exp2[] ="void f(int x) {\n"
                                 " std::cout << exp2(x);\n" // do not simplify
                                 " std::cout << exp2(-1);\n" // do not simplify
                                 " std::cout << exp2(0L);\n" // simplify to 0
                                 " std::cout << exp2(1L);\n" // do not simplify
                                 "}";
        const char expected_exp2[] = "void f ( int x ) {\n"
                                     "std :: cout << exp2 ( x ) ;\n"
                                     "std :: cout << exp2 ( -1 ) ;\n"
                                     "std :: cout << 1 ;\n"
                                     "std :: cout << exp2 ( 1L ) ;\n"
                                     "}";
        ASSERT_EQUALS(expected_exp2, tokWithNewlines(code_exp2));

        const char code_exp2f[] ="void f(float x) {\n"
                                  " std::cout << exp2f(x);\n" // do not simplify
                                  " std::cout << exp2f(-1.0);\n" // do not simplify
                                  " std::cout << exp2f(0.0);\n" // simplify to 1
                                  " std::cout << exp2f(1.0);\n" // do not simplify
                                  "}";
        const char expected_exp2f[] = "void f ( float x ) {\n"
                                      "std :: cout << exp2f ( x ) ;\n"
                                      "std :: cout << exp2f ( -1.0 ) ;\n"
                                      "std :: cout << 1 ;\n"
                                      "std :: cout << exp2f ( 1.0 ) ;\n"
                                      "}";
        ASSERT_EQUALS(expected_exp2f, tokWithNewlines(code_exp2f));

        const char code_exp2l[] ="void f(long double x) {\n"
                                  " std::cout << exp2l(x);\n" // do not simplify
                                  " std::cout << exp2l(-1.0);\n" // do not simplify
                                  " std::cout << exp2l(0.0);\n" // simplify to 1
                                  " std::cout << exp2l(1.0);\n" // do not simplify
                                  "}";
        const char expected_exp2l[] = "void f ( double x ) {\n"
                                      "std :: cout << exp2l ( x ) ;\n"
                                      "std :: cout << exp2l ( -1.0 ) ;\n"
                                      "std :: cout << 1 ;\n"
                                      "std :: cout << exp2l ( 1.0 ) ;\n"
                                      "}";
        ASSERT_EQUALS(expected_exp2l, tokWithNewlines(code_exp2l));
    }

    void simplifyMathFunctions_exp() {
        // verify exp(), expf(), expl() - simplifcation
        const char code_exp[] ="void f(int x) {\n"
                                " std::cout << exp(x);\n" // do not simplify
                                " std::cout << exp(-1);\n" // do not simplify
                                " std::cout << exp(0L);\n" // simplify to 1
                                " std::cout << exp(1L);\n" // do not simplify
                                "}";
        const char expected_exp[] = "void f ( int x ) {\n"
                                    "std :: cout << exp ( x ) ;\n"
                                    "std :: cout << exp ( -1 ) ;\n"
                                    "std :: cout << 1 ;\n"
                                    "std :: cout << exp ( 1L ) ;\n"
                                    "}";
        ASSERT_EQUALS(expected_exp, tokWithNewlines(code_exp));

        const char code_expf[] ="void f(float x) {\n"
                                 " std::cout << expf(x);\n" // do not simplify
                                 " std::cout << expf(-1.0);\n" // do not simplify
                                 " std::cout << expf(0.0);\n" // simplify to 1
                                 " std::cout << expf(1.0);\n" // do not simplify
                                 "}";
        const char expected_expf[] = "void f ( float x ) {\n"
                                     "std :: cout << expf ( x ) ;\n"
                                     "std :: cout << expf ( -1.0 ) ;\n"
                                     "std :: cout << 1 ;\n"
                                     "std :: cout << expf ( 1.0 ) ;\n"
                                     "}";
        ASSERT_EQUALS(expected_expf, tokWithNewlines(code_expf));

        const char code_expl[] ="void f(long double x) {\n"
                                 " std::cout << expl(x);\n" // do not simplify
                                 " std::cout << expl(-1.0);\n" // do not simplify
                                 " std::cout << expl(0.0);\n" // simplify to 1
                                 " std::cout << expl(1.0);\n" // do not simplify
                                 "}";
        const char expected_expl[] = "void f ( double x ) {\n"
                                     "std :: cout << expl ( x ) ;\n"
                                     "std :: cout << expl ( -1.0 ) ;\n"
                                     "std :: cout << 1 ;\n"
                                     "std :: cout << expl ( 1.0 ) ;\n"
                                     "}";
        ASSERT_EQUALS(expected_expl, tokWithNewlines(code_expl));
    }

    void simplifyMathFunctions_erf() {
        // verify erf(), erff(), erfl() - simplifcation
        const char code_erf[] ="void f(int x) {\n"
                                " std::cout << erf(x);\n" // do not simplify
                                " std::cout << erf(10);\n" // do not simplify
                                " std::cout << erf(0L);\n" // simplify to 0
                                "}";
        const char expected_erf[] = "void f ( int x ) {\n"
                                    "std :: cout << erf ( x ) ;\n"
                                    "std :: cout << erf ( 10 ) ;\n"
                                    "std :: cout << 0 ;\n"
                                    "}";
        ASSERT_EQUALS(expected_erf, tokWithNewlines(code_erf));

        const char code_erff[] ="void f(float x) {\n"
                                 " std::cout << erff(x);\n" // do not simplify
                                 " std::cout << erff(10);\n" // do not simplify
                                 " std::cout << erff(0.0f);\n" // simplify to 0
                                 "}";
        const char expected_erff[] = "void f ( float x ) {\n"
                                     "std :: cout << erff ( x ) ;\n"
                                     "std :: cout << erff ( 10 ) ;\n"
                                     "std :: cout << 0 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_erff, tokWithNewlines(code_erff));

        const char code_erfl[] ="void f(long double x) {\n"
                                 " std::cout << erfl(x);\n" // do not simplify
                                 " std::cout << erfl(10.0f);\n" // do not simplify
                                 " std::cout << erfl(0.0f);\n" // simplify to 0
                                 "}";
        const char expected_erfl[] = "void f ( double x ) {\n"
                                     "std :: cout << erfl ( x ) ;\n"
                                     "std :: cout << erfl ( 10.0f ) ;\n"
                                     "std :: cout << 0 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_erfl, tokWithNewlines(code_erfl));
    }

    void simplifyMathFunctions_atanh() {
        // verify atanh(), atanhf(), atanhl() - simplifcation
        const char code_atanh[] ="void f(int x) {\n"
                                  " std::cout << atanh(x);\n" // do not simplify
                                  " std::cout << atanh(10);\n" // do not simplify
                                  " std::cout << atanh(0L);\n" // simplify to 0
                                  "}";
        const char expected_atanh[] = "void f ( int x ) {\n"
                                      "std :: cout << atanh ( x ) ;\n"
                                      "std :: cout << atanh ( 10 ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_atanh, tokWithNewlines(code_atanh));

        const char code_atanhf[] ="void f(float x) {\n"
                                   " std::cout << atanhf(x);\n" // do not simplify
                                   " std::cout << atanhf(10);\n" // do not simplify
                                   " std::cout << atanhf(0.0f);\n" // simplify to 0
                                   "}";
        const char expected_atanhf[] = "void f ( float x ) {\n"
                                       "std :: cout << atanhf ( x ) ;\n"
                                       "std :: cout << atanhf ( 10 ) ;\n"
                                       "std :: cout << 0 ;\n"
                                       "}";
        ASSERT_EQUALS(expected_atanhf, tokWithNewlines(code_atanhf));

        const char code_atanhl[] ="void f(long double x) {\n"
                                   " std::cout << atanhl(x);\n" // do not simplify
                                   " std::cout << atanhl(10.0f);\n" // do not simplify
                                   " std::cout << atanhl(0.0d);\n" // do not simplify - invalid number!
                                   " std::cout << atanhl(0.0f);\n" // simplify to 0
                                   "}";
        const char expected_atanhl[] = "void f ( double x ) {\n"
                                       "std :: cout << atanhl ( x ) ;\n"
                                       "std :: cout << atanhl ( 10.0f ) ;\n"
                                       "std :: cout << atanhl ( 0.0d ) ;\n"
                                       "std :: cout << 0 ;\n"
                                       "}";
        ASSERT_EQUALS(expected_atanhl, tokWithNewlines(code_atanhl));
    }

    void simplifyMathFunctions_atan() {
        // verify atan(), atanf(), atanl() - simplifcation
        const char code_atan[] ="void f(int x) {\n"
                                 " std::cout << atan(x);\n" // do not simplify
                                 " std::cout << atan(10);\n" // do not simplify
                                 " std::cout << atan(0L);\n" // simplify to 0
                                 "}";
        const char expected_atan[] = "void f ( int x ) {\n"
                                     "std :: cout << atan ( x ) ;\n"
                                     "std :: cout << atan ( 10 ) ;\n"
                                     "std :: cout << 0 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_atan, tokWithNewlines(code_atan));

        const char code_atanf[] ="void f(float x) {\n"
                                  " std::cout << atanf(x);\n" // do not simplify
                                  " std::cout << atanf(10);\n" // do not simplify
                                  " std::cout << atanf(0.0f);\n" // simplify to 0
                                  "}";
        const char expected_atanf[] = "void f ( float x ) {\n"
                                      "std :: cout << atanf ( x ) ;\n"
                                      "std :: cout << atanf ( 10 ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_atanf, tokWithNewlines(code_atanf));

        const char code_atanl[] ="void f(long double x) {\n"
                                  " std::cout << atanl(x);\n" // do not simplify
                                  " std::cout << atanl(10.0f);\n" // do not simplify
                                  " std::cout << atanl(0.0f);\n" // simplify to 0
                                  "}";
        const char expected_atanl[] = "void f ( double x ) {\n"
                                      "std :: cout << atanl ( x ) ;\n"
                                      "std :: cout << atanl ( 10.0f ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_atanl, tokWithNewlines(code_atanl));
    }

    void simplifyMathFunctions_tanh() {
        // verify tanh(), tanhf(), tanhl() - simplifcation
        const char code_tanh[] ="void f(int x) {\n"
                                 " std::cout << tanh(x);\n" // do not simplify
                                 " std::cout << tanh(10);\n" // do not simplify
                                 " std::cout << tanh(0L);\n" // simplify to 0
                                 "}";
        const char expected_tanh[] = "void f ( int x ) {\n"
                                     "std :: cout << tanh ( x ) ;\n"
                                     "std :: cout << tanh ( 10 ) ;\n"
                                     "std :: cout << 0 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_tanh, tokWithNewlines(code_tanh));

        const char code_tanhf[] ="void f(float x) {\n"
                                  " std::cout << tanhf(x);\n" // do not simplify
                                  " std::cout << tanhf(10);\n" // do not simplify
                                  " std::cout << tanhf(0.0f);\n" // simplify to 0
                                  "}";
        const char expected_tanhf[] = "void f ( float x ) {\n"
                                      "std :: cout << tanhf ( x ) ;\n"
                                      "std :: cout << tanhf ( 10 ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_tanhf, tokWithNewlines(code_tanhf));

        const char code_tanhl[] ="void f(long double x) {\n"
                                  " std::cout << tanhl(x);\n" // do not simplify
                                  " std::cout << tanhl(10.0f);\n" // do not simplify
                                  " std::cout << tanhl(0.0f);\n" // simplify to 0
                                  "}";
        const char expected_tanhl[] = "void f ( double x ) {\n"
                                      "std :: cout << tanhl ( x ) ;\n"
                                      "std :: cout << tanhl ( 10.0f ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_tanhl, tokWithNewlines(code_tanhl));
    }

    void simplifyMathFunctions_tan() {
        // verify tan(), tanf(), tanl() - simplifcation
        const char code_tan[] ="void f(int x) {\n"
                                " std::cout << tan(x);\n" // do not simplify
                                " std::cout << tan(10);\n" // do not simplify
                                " std::cout << tan(0L);\n" // simplify to 0
                                "}";
        const char expected_tan[] = "void f ( int x ) {\n"
                                    "std :: cout << tan ( x ) ;\n"
                                    "std :: cout << tan ( 10 ) ;\n"
                                    "std :: cout << 0 ;\n"
                                    "}";
        ASSERT_EQUALS(expected_tan, tokWithNewlines(code_tan));

        const char code_tanf[] ="void f(float x) {\n"
                                 " std::cout << tanf(x);\n" // do not simplify
                                 " std::cout << tanf(10);\n" // do not simplify
                                 " std::cout << tanf(0.0f);\n" // simplify to 0
                                 "}";
        const char expected_tanf[] = "void f ( float x ) {\n"
                                     "std :: cout << tanf ( x ) ;\n"
                                     "std :: cout << tanf ( 10 ) ;\n"
                                     "std :: cout << 0 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_tanf, tokWithNewlines(code_tanf));

        const char code_tanl[] ="void f(long double x) {\n"
                                 " std::cout << tanl(x);\n" // do not simplify
                                 " std::cout << tanl(10.0f);\n" // do not simplify
                                 " std::cout << tanl(0.0f);\n" // simplify to 0
                                 "}";
        const char expected_tanl[] = "void f ( double x ) {\n"
                                     "std :: cout << tanl ( x ) ;\n"
                                     "std :: cout << tanl ( 10.0f ) ;\n"
                                     "std :: cout << 0 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_tanl, tokWithNewlines(code_tanl));
    }

    void simplifyMathFunctions_expm1() {
        // verify expm1(), expm1f(), expm1l() - simplifcation
        const char code_expm1[] ="void f(int x) {\n"
                                  " std::cout << expm1(x);\n" // do not simplify
                                  " std::cout << expm1(10);\n" // do not simplify
                                  " std::cout << expm1(0L);\n" // simplify to 0
                                  "}";
        const char expected_expm1[] = "void f ( int x ) {\n"
                                      "std :: cout << expm1 ( x ) ;\n"
                                      "std :: cout << expm1 ( 10 ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_expm1, tokWithNewlines(code_expm1));

        const char code_expm1f[] ="void f(float x) {\n"
                                   " std::cout << expm1f(x);\n" // do not simplify
                                   " std::cout << expm1f(10);\n" // do not simplify
                                   " std::cout << expm1f(0.0f);\n" // simplify to 0
                                   "}";
        const char expected_expm1f[] = "void f ( float x ) {\n"
                                       "std :: cout << expm1f ( x ) ;\n"
                                       "std :: cout << expm1f ( 10 ) ;\n"
                                       "std :: cout << 0 ;\n"
                                       "}";
        ASSERT_EQUALS(expected_expm1f, tokWithNewlines(code_expm1f));

        const char code_expm1l[] ="void f(long double x) {\n"
                                   " std::cout << expm1l(x);\n" // do not simplify
                                   " std::cout << expm1l(10.0f);\n" // do not simplify
                                   " std::cout << expm1l(0.0f);\n" // simplify to 0
                                   "}";
        const char expected_expm1l[] = "void f ( double x ) {\n"
                                       "std :: cout << expm1l ( x ) ;\n"
                                       "std :: cout << expm1l ( 10.0f ) ;\n"
                                       "std :: cout << 0 ;\n"
                                       "}";
        ASSERT_EQUALS(expected_expm1l, tokWithNewlines(code_expm1l));
    }

    void simplifyMathFunctions_asinh() {
        // verify asinh(), asinhf(), asinhl() - simplifcation
        const char code_asinh[] ="void f(int x) {\n"
                                  " std::cout << asinh(x);\n" // do not simplify
                                  " std::cout << asinh(10);\n" // do not simplify
                                  " std::cout << asinh(0L);\n" // simplify to 0
                                  "}";
        const char expected_asinh[] = "void f ( int x ) {\n"
                                      "std :: cout << asinh ( x ) ;\n"
                                      "std :: cout << asinh ( 10 ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_asinh, tokWithNewlines(code_asinh));

        const char code_asinhf[] ="void f(float x) {\n"
                                   " std::cout << asinhf(x);\n" // do not simplify
                                   " std::cout << asinhf(10);\n" // do not simplify
                                   " std::cout << asinhf(0.0f);\n" // simplify to 0
                                   "}";
        const char expected_asinhf[] = "void f ( float x ) {\n"
                                       "std :: cout << asinhf ( x ) ;\n"
                                       "std :: cout << asinhf ( 10 ) ;\n"
                                       "std :: cout << 0 ;\n"
                                       "}";
        ASSERT_EQUALS(expected_asinhf, tokWithNewlines(code_asinhf));

        const char code_asinhl[] ="void f(long double x) {\n"
                                   " std::cout << asinhl(x);\n" // do not simplify
                                   " std::cout << asinhl(10.0f);\n" // do not simplify
                                   " std::cout << asinhl(0.0f);\n" // simplify to 0
                                   "}";
        const char expected_asinhl[] = "void f ( double x ) {\n"
                                       "std :: cout << asinhl ( x ) ;\n"
                                       "std :: cout << asinhl ( 10.0f ) ;\n"
                                       "std :: cout << 0 ;\n"
                                       "}";
        ASSERT_EQUALS(expected_asinhl, tokWithNewlines(code_asinhl));
    }

    void simplifyMathFunctions_asin() {
        // verify asin(), asinf(), asinl() - simplifcation
        const char code_asin[] ="void f(int x) {\n"
                                 " std::cout << asin(x);\n" // do not simplify
                                 " std::cout << asin(10);\n" // do not simplify
                                 " std::cout << asin(0L);\n" // simplify to 0
                                 "}";
        const char expected_asin[] = "void f ( int x ) {\n"
                                     "std :: cout << asin ( x ) ;\n"
                                     "std :: cout << asin ( 10 ) ;\n"
                                     "std :: cout << 0 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_asin, tokWithNewlines(code_asin));

        const char code_asinf[] ="void f(float x) {\n"
                                  " std::cout << asinf(x);\n" // do not simplify
                                  " std::cout << asinf(10);\n" // do not simplify
                                  " std::cout << asinf(0.0f);\n" // simplify to 0
                                  "}";
        const char expected_asinf[] = "void f ( float x ) {\n"
                                      "std :: cout << asinf ( x ) ;\n"
                                      "std :: cout << asinf ( 10 ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_asinf, tokWithNewlines(code_asinf));

        const char code_asinl[] ="void f(long double x) {\n"
                                  " std::cout << asinl(x);\n" // do not simplify
                                  " std::cout << asinl(10.0f);\n" // do not simplify
                                  " std::cout << asinl(0.0f);\n" // simplify to 0
                                  "}";
        const char expected_asinl[] = "void f ( double x ) {\n"
                                      "std :: cout << asinl ( x ) ;\n"
                                      "std :: cout << asinl ( 10.0f ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_asinl, tokWithNewlines(code_asinl));
    }

    void simplifyMathFunctions_sinh() {
        // verify sinh(), sinhf(), sinhl() - simplifcation
        const char code_sinh[] ="void f(int x) {\n"
                                 " std::cout << sinh(x);\n" // do not simplify
                                 " std::cout << sinh(10);\n" // do not simplify
                                 " std::cout << sinh(0L);\n" // simplify to 0
                                 "}";
        const char expected_sinh[] = "void f ( int x ) {\n"
                                     "std :: cout << sinh ( x ) ;\n"
                                     "std :: cout << sinh ( 10 ) ;\n"
                                     "std :: cout << 0 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_sinh, tokWithNewlines(code_sinh));

        const char code_sinhf[] ="void f(float x) {\n"
                                  " std::cout << sinhf(x);\n" // do not simplify
                                  " std::cout << sinhf(10);\n" // do not simplify
                                  " std::cout << sinhf(0.0f);\n" // simplify to 0
                                  "}";
        const char expected_sinhf[] = "void f ( float x ) {\n"
                                      "std :: cout << sinhf ( x ) ;\n"
                                      "std :: cout << sinhf ( 10 ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_sinhf, tokWithNewlines(code_sinhf));

        const char code_sinhl[] ="void f(long double x) {\n"
                                  " std::cout << sinhl(x);\n" // do not simplify
                                  " std::cout << sinhl(10.0f);\n" // do not simplify
                                  " std::cout << sinhl(0.0f);\n" // simplify to 0
                                  "}";
        const char expected_sinhl[] = "void f ( double x ) {\n"
                                      "std :: cout << sinhl ( x ) ;\n"
                                      "std :: cout << sinhl ( 10.0f ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_sinhl, tokWithNewlines(code_sinhl));
    }

    void simplifyMathFunctions_sin() {
        // verify sin(), sinf(), sinl() - simplifcation
        const char code_sin[] ="void f(int x) {\n"
                                " std::cout << sin(x);\n" // do not simplify
                                " std::cout << sin(10);\n" // do not simplify
                                " std::cout << sin(0L);\n" // simplify to 0
                                "}";
        const char expected_sin[] = "void f ( int x ) {\n"
                                    "std :: cout << sin ( x ) ;\n"
                                    "std :: cout << sin ( 10 ) ;\n"
                                    "std :: cout << 0 ;\n"
                                    "}";
        ASSERT_EQUALS(expected_sin, tokWithNewlines(code_sin));

        const char code_sinf[] ="void f(float x) {\n"
                                 " std::cout << sinf(x);\n" // do not simplify
                                 " std::cout << sinf(10);\n" // do not simplify
                                 " std::cout << sinf(0.0f);\n" // simplify to 0
                                 "}";
        const char expected_sinf[] = "void f ( float x ) {\n"
                                     "std :: cout << sinf ( x ) ;\n"
                                     "std :: cout << sinf ( 10 ) ;\n"
                                     "std :: cout << 0 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_sinf, tokWithNewlines(code_sinf));

        const char code_sinl[] ="void f(long double x) {\n"
                                 " std::cout << sinl(x);\n" // do not simplify
                                 " std::cout << sinl(10.0f);\n" // do not simplify
                                 " std::cout << sinl(0.0f);\n" // simplify to 0
                                 "}";
        const char expected_sinl[] = "void f ( double x ) {\n"
                                     "std :: cout << sinl ( x ) ;\n"
                                     "std :: cout << sinl ( 10.0f ) ;\n"
                                     "std :: cout << 0 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_sinl, tokWithNewlines(code_sinl));

        // #6629
        const char code[] = "class Foo { int sinf; Foo() : sinf(0) {} };";
        const char expected[] = "class Foo { int sinf ; Foo ( ) : sinf ( 0 ) { } } ;";
        ASSERT_EQUALS(expected, tokWithNewlines(code));
    }

    void simplifyMathFunctions_ilogb() {
        // verify ilogb(), ilogbf(), ilogbl() - simplifcation
        const char code_ilogb[] ="void f(int x) {\n"
                                  " std::cout << ilogb(x);\n" // do not simplify
                                  " std::cout << ilogb(10);\n" // do not simplify
                                  " std::cout << ilogb(1L);\n" // simplify to 0
                                  "}";
        const char expected_ilogb[] = "void f ( int x ) {\n"
                                      "std :: cout << ilogb ( x ) ;\n"
                                      "std :: cout << ilogb ( 10 ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_ilogb, tokWithNewlines(code_ilogb));

        const char code_ilogbf[] ="void f(float x) {\n"
                                   " std::cout << ilogbf(x);\n" // do not simplify
                                   " std::cout << ilogbf(10);\n" // do not simplify
                                   " std::cout << ilogbf(1.0f);\n" // simplify to 0
                                   "}";
        const char expected_ilogbf[] = "void f ( float x ) {\n"
                                       "std :: cout << ilogbf ( x ) ;\n"
                                       "std :: cout << ilogbf ( 10 ) ;\n"
                                       "std :: cout << 0 ;\n"
                                       "}";
        ASSERT_EQUALS(expected_ilogbf, tokWithNewlines(code_ilogbf));

        const char code_ilogbl[] ="void f(long double x) {\n"
                                   " std::cout << ilogbl(x);\n" // do not simplify
                                   " std::cout << ilogbl(10.0f);\n" // do not simplify
                                   " std::cout << ilogbl(1.0f);\n" // simplify to 0
                                   "}";
        const char expected_ilogbl[] = "void f ( double x ) {\n"
                                       "std :: cout << ilogbl ( x ) ;\n"
                                       "std :: cout << ilogbl ( 10.0f ) ;\n"
                                       "std :: cout << 0 ;\n"
                                       "}";
        ASSERT_EQUALS(expected_ilogbl, tokWithNewlines(code_ilogbl));
    }

    void simplifyMathFunctions_logb() {
        // verify logb(), logbf(), logbl() - simplifcation
        const char code_logb[] ="void f(int x) {\n"
                                 " std::cout << logb(x);\n" // do not simplify
                                 " std::cout << logb(10);\n" // do not simplify
                                 " std::cout << logb(1L);\n" // simplify to 0
                                 "}";
        const char expected_logb[] = "void f ( int x ) {\n"
                                     "std :: cout << logb ( x ) ;\n"
                                     "std :: cout << logb ( 10 ) ;\n"
                                     "std :: cout << 0 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_logb, tokWithNewlines(code_logb));

        const char code_logbf[] ="void f(float x) {\n"
                                  " std::cout << logbf(x);\n" // do not simplify
                                  " std::cout << logbf(10);\n" // do not simplify
                                  " std::cout << logbf(1.0f);\n" // simplify to 0
                                  "}";
        const char expected_logbf[] = "void f ( float x ) {\n"
                                      "std :: cout << logbf ( x ) ;\n"
                                      "std :: cout << logbf ( 10 ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_logbf, tokWithNewlines(code_logbf));

        const char code_logbl[] ="void f(long double x) {\n"
                                  " std::cout << logbl(x);\n" // do not simplify
                                  " std::cout << logbl(10.0f);\n" // do not simplify
                                  " std::cout << logbl(1.0f);\n" // simplify to 0
                                  "}";
        const char expected_logbl[] = "void f ( double x ) {\n"
                                      "std :: cout << logbl ( x ) ;\n"
                                      "std :: cout << logbl ( 10.0f ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_logbl, tokWithNewlines(code_logbl));
    }

    void simplifyMathFunctions_log1p() {
        // verify log1p(), log1pf(), log1pl() - simplifcation
        const char code_log1p[] ="void f(int x) {\n"
                                  " std::cout << log1p(x);\n" // do not simplify
                                  " std::cout << log1p(10);\n" // do not simplify
                                  " std::cout << log1p(0L);\n" // simplify to 0
                                  "}";
        const char expected_log1p[] = "void f ( int x ) {\n"
                                      "std :: cout << log1p ( x ) ;\n"
                                      "std :: cout << log1p ( 10 ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_log1p, tokWithNewlines(code_log1p));

        const char code_log1pf[] ="void f(float x) {\n"
                                   " std::cout << log1pf(x);\n" // do not simplify
                                   " std::cout << log1pf(10);\n" // do not simplify
                                   " std::cout << log1pf(0.0f);\n" // simplify to 0
                                   "}";
        const char expected_log1pf[] = "void f ( float x ) {\n"
                                       "std :: cout << log1pf ( x ) ;\n"
                                       "std :: cout << log1pf ( 10 ) ;\n"
                                       "std :: cout << 0 ;\n"
                                       "}";
        ASSERT_EQUALS(expected_log1pf, tokWithNewlines(code_log1pf));

        const char code_log1pl[] ="void f(long double x) {\n"
                                   " std::cout << log1pl(x);\n" // do not simplify
                                   " std::cout << log1pl(10.0f);\n" // do not simplify
                                   " std::cout << log1pl(0.0f);\n" // simplify to 0
                                   "}";
        const char expected_log1pl[] = "void f ( double x ) {\n"
                                       "std :: cout << log1pl ( x ) ;\n"
                                       "std :: cout << log1pl ( 10.0f ) ;\n"
                                       "std :: cout << 0 ;\n"
                                       "}";
        ASSERT_EQUALS(expected_log1pl, tokWithNewlines(code_log1pl));
    }

    void simplifyMathFunctions_log10() {
        // verify log10(), log10f(), log10l() - simplifcation
        const char code_log10[] ="void f(int x) {\n"
                                  " std::cout << log10(x);\n" // do not simplify
                                  " std::cout << log10(10);\n" // do not simplify
                                  " std::cout << log10(1L);\n" // simplify to 0
                                  "}";
        const char expected_log10[] = "void f ( int x ) {\n"
                                      "std :: cout << log10 ( x ) ;\n"
                                      "std :: cout << log10 ( 10 ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_log10, tokWithNewlines(code_log10));

        const char code_log10f[] ="void f(float x) {\n"
                                   " std::cout << log10f(x);\n" // do not simplify
                                   " std::cout << log10f(10);\n" // do not simplify
                                   " std::cout << log10f(1.0f);\n" // simplify to 0
                                   "}";
        const char expected_log10f[] = "void f ( float x ) {\n"
                                       "std :: cout << log10f ( x ) ;\n"
                                       "std :: cout << log10f ( 10 ) ;\n"
                                       "std :: cout << 0 ;\n"
                                       "}";
        ASSERT_EQUALS(expected_log10f, tokWithNewlines(code_log10f));

        const char code_log10l[] ="void f(long double x) {\n"
                                   " std::cout << log10l(x);\n" // do not simplify
                                   " std::cout << log10l(10.0f);\n" // do not simplify
                                   " std::cout << log10l(1.0f);\n" // simplify to 0
                                   "}";
        const char expected_log10l[] = "void f ( double x ) {\n"
                                       "std :: cout << log10l ( x ) ;\n"
                                       "std :: cout << log10l ( 10.0f ) ;\n"
                                       "std :: cout << 0 ;\n"
                                       "}";
        ASSERT_EQUALS(expected_log10l, tokWithNewlines(code_log10l));

    }
    void simplifyMathFunctions_log() {
        // verify log(), logf(), logl() - simplifcation
        const char code_log[] ="void f(int x) {\n"
                                " std::cout << log(x);\n" // do not simplify
                                " std::cout << log(10);\n" // do not simplify
                                " std::cout << log(1L);\n" // simplify to 0
                                "}";
        const char expected_log[] = "void f ( int x ) {\n"
                                    "std :: cout << log ( x ) ;\n"
                                    "std :: cout << log ( 10 ) ;\n"
                                    "std :: cout << 0 ;\n"
                                    "}";
        ASSERT_EQUALS(expected_log, tokWithNewlines(code_log));

        const char code_logf[] ="void f(float x) {\n"
                                 " std::cout << logf(x);\n" // do not simplify
                                 " std::cout << logf(10);\n" // do not simplify
                                 " std::cout << logf(1.0f);\n" // simplify to 0
                                 "}";
        const char expected_logf[] = "void f ( float x ) {\n"
                                     "std :: cout << logf ( x ) ;\n"
                                     "std :: cout << logf ( 10 ) ;\n"
                                     "std :: cout << 0 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_logf, tokWithNewlines(code_logf));

        const char code_logl[] ="void f(long double x) {\n"
                                 " std::cout << logl(x);\n" // do not simplify
                                 " std::cout << logl(10.0f);\n" // do not simplify
                                 " std::cout << logl(1.0f);\n" // simplify to 0
                                 "}";
        const char expected_logl[] = "void f ( double x ) {\n"
                                     "std :: cout << logl ( x ) ;\n"
                                     "std :: cout << logl ( 10.0f ) ;\n"
                                     "std :: cout << 0 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_logl, tokWithNewlines(code_logl));
    }

    void simplifyMathFunctions_log2() {
        // verify log2(), log2f(), log2l() - simplifcation
        const char code_log2[] ="void f(int x) {\n"
                                 " std::cout << log2(x);\n" // do not simplify
                                 " std::cout << log2(10);\n" // do not simplify
                                 " std::cout << log2(1L);\n" // simplify to 0
                                 "}";
        const char expected_log2[] = "void f ( int x ) {\n"
                                     "std :: cout << log2 ( x ) ;\n"
                                     "std :: cout << log2 ( 10 ) ;\n"
                                     "std :: cout << 0 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_log2, tokWithNewlines(code_log2));

        const char code_log2f[] ="void f(float x) {\n"
                                  " std::cout << log2f(x);\n" // do not simplify
                                  " std::cout << log2f(10);\n" // do not simplify
                                  " std::cout << log2f(1.0f);\n" // simplify to 0
                                  "}";
        const char expected_log2f[] = "void f ( float x ) {\n"
                                      "std :: cout << log2f ( x ) ;\n"
                                      "std :: cout << log2f ( 10 ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_log2f, tokWithNewlines(code_log2f));

        const char code_log2l[] ="void f(long double x) {\n"
                                  " std::cout << log2l(x);\n" // do not simplify
                                  " std::cout << log2l(10.0f);\n" // do not simplify
                                  " std::cout << log2l(1.0f);\n" // simplify to 0
                                  "}";
        const char expected_log2l[] = "void f ( double x ) {\n"
                                      "std :: cout << log2l ( x ) ;\n"
                                      "std :: cout << log2l ( 10.0f ) ;\n"
                                      "std :: cout << 0 ;\n"
                                      "}";
        ASSERT_EQUALS(expected_log2l, tokWithNewlines(code_log2l));
    }

    void simplifyMathFunctions_pow() {
        // verify pow(),pow(),powl() - simplifcation
        const char code_pow[] ="void f() {\n"
                                " std::cout << pow(-1.0,1);\n"
                                " std::cout << pow(1.0,1);\n"
                                " std::cout << pow(0,1);\n"
                                " std::cout << pow(1,-6);\n"
                                " std::cout << powf(-1.0,1.0f);\n"
                                " std::cout << powf(1.0,1.0f);\n"
                                " std::cout << powf(0,1.0f);\n"
                                " std::cout << powf(1.0,-6.0f);\n"
                                " std::cout << powl(-1.0,1.0);\n"
                                " std::cout << powl(1.0,1.0);\n"
                                " std::cout << powl(0,1.0);\n"
                                " std::cout << powl(1.0,-6.0d);\n"
                                "}";

        const char expected_pow[] = "void f ( ) {\n"
                                    "std :: cout << -1.0 ;\n"
                                    "std :: cout << 1 ;\n"
                                    "std :: cout << 0 ;\n"
                                    "std :: cout << 1 ;\n"
                                    "std :: cout << -1.0 ;\n"
                                    "std :: cout << 1 ;\n"
                                    "std :: cout << 0 ;\n"
                                    "std :: cout << 1 ;\n"
                                    "std :: cout << -1.0 ;\n"
                                    "std :: cout << 1 ;\n"
                                    "std :: cout << 0 ;\n"
                                    "std :: cout << 1 ;\n"
                                    "}";
        ASSERT_EQUALS(expected_pow, tokWithNewlines(code_pow));

        // verify if code is simplified correctly.
        // Do not simplify class members.
        const char code_pow1[] = "int f(const Fred &fred) {return fred.pow(12,3);}";
        const char expected_pow1[] = "int f ( const Fred & fred ) { return fred . pow ( 12 , 3 ) ; }";
        ASSERT_EQUALS(expected_pow1, tokWithNewlines(code_pow1));

        const char code_pow2[] = "int f() {return pow(0,0);}";
        const char expected_pow2[] = "int f ( ) { return 1 ; }";
        ASSERT_EQUALS(expected_pow2, tokWithNewlines(code_pow2));

        const char code_pow3[] = "int f() {return pow(0,1);}";
        const char expected_pow3[] = "int f ( ) { return 0 ; }";
        ASSERT_EQUALS(expected_pow3, tokWithNewlines(code_pow3));

        const char code_pow4[] = "int f() {return pow(1,0);}";
        const char expected_pow4[] = "int f ( ) { return 1 ; }";
        ASSERT_EQUALS(expected_pow4, tokWithNewlines(code_pow4));
    }

    void simplifyMathFunctions_fmin() {
        // verify fmin,fminl,fminl simplifcation
        const char code_fmin[] ="void f() {\n"
                                 " std::cout << fmin(-1.0,0);\n"
                                 " std::cout << fmin(1.0,0);\n"
                                 " std::cout << fmin(0,0);\n"
                                 " std::cout << fminf(-1.0,0);\n"
                                 " std::cout << fminf(1.0,0);\n"
                                 " std::cout << fminf(0,0);\n"
                                 " std::cout << fminl(-1.0,0);\n"
                                 " std::cout << fminl(1.0,0);\n"
                                 " std::cout << fminl(0,0);\n"
                                 "}";

        const char expected_fmin[] = "void f ( ) {\n"
                                     "std :: cout << -1.0 ;\n"
                                     "std :: cout << 0 ;\n"
                                     "std :: cout << 0 ;\n"
                                     "std :: cout << -1.0 ;\n"
                                     "std :: cout << 0 ;\n"
                                     "std :: cout << 0 ;\n"
                                     "std :: cout << -1.0 ;\n"
                                     "std :: cout << 0 ;\n"
                                     "std :: cout << 0 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_fmin, tokWithNewlines(code_fmin));

        // do not simplify this case
        const char code_fmin1[] = "float f(float f) { return fmin(f,0);}";
        const char expected_fmin1[] = "float f ( float f ) { return fmin ( f , 0 ) ; }";
        ASSERT_EQUALS(expected_fmin1, tokWithNewlines(code_fmin1));
    }

    void simplifyMathFunctions_fmax() {
        // verify fmax(),fmax(),fmaxl() simplifcation
        const char code_fmax[] ="void f() {\n"
                                 " std::cout << fmax(-1.0,0);\n"
                                 " std::cout << fmax(1.0,0);\n"
                                 " std::cout << fmax(0,0);\n"
                                 " std::cout << fmaxf(-1.0,0);\n"
                                 " std::cout << fmaxf(1.0,0);\n"
                                 " std::cout << fmaxf(0,0);\n"
                                 " std::cout << fmaxl(-1.0,0);\n"
                                 " std::cout << fmaxl(1.0,0);\n"
                                 " std::cout << fmaxl(0,0);\n"
                                 "}";

        const char expected_fmax[] = "void f ( ) {\n"
                                     "std :: cout << 0 ;\n"
                                     "std :: cout << 1.0 ;\n"
                                     "std :: cout << 0 ;\n"
                                     "std :: cout << 0 ;\n"
                                     "std :: cout << 1.0 ;\n"
                                     "std :: cout << 0 ;\n"
                                     "std :: cout << 0 ;\n"
                                     "std :: cout << 1.0 ;\n"
                                     "std :: cout << 0 ;\n"
                                     "}";
        ASSERT_EQUALS(expected_fmax, tokWithNewlines(code_fmax));

        // do not simplify this case
        const char code_fmax1[] = "float f(float f) { return fmax(f,0);}";
        const char expected_fmax1[] = "float f ( float f ) { return fmax ( f , 0 ) ; }";
        ASSERT_EQUALS(expected_fmax1, tokWithNewlines(code_fmax1));
    }

    void simplifyMathExpressions() { //#1620
        const char code1[] = "void foo() {\n"
                             "    std::cout<<pow(sin(x),2)+pow(cos(x),2);\n"
                             "    std::cout<<pow(sin(pow(sin(y),2)+pow(cos(y),2)),2)+pow(cos(pow(sin(y),2)+pow(cos(y),2)),2);\n"
                             "    std::cout<<pow(sin(x),2.0)+pow(cos(x),2.0);\n"
                             "    std::cout<<pow(sin(x*y+z),2.0)+pow(cos(x*y+z),2.0);\n"
                             "    std::cout<<pow(sin(x*y+z),2)+pow(cos(x*y+z),2);\n"
                             "    std::cout<<pow(cos(x),2)+pow(sin(x),2);\n"
                             "    std::cout<<pow(cos(x),2.0)+pow(sin(x),2.0);\n"
                             "    std::cout<<pow(cos(x*y+z),2.0)+pow(sin(x*y+z),2.0);\n"
                             "    std::cout<<pow(cos(x*y+z),2)+pow(sin(x*y+z),2);\n"
                             "    std::cout<<pow(sinh(x*y+z),2)-pow(cosh(x*y+z),2);\n"
                             "    std::cout<<pow(sinh(x),2)-pow(cosh(x),2);\n"
                             "    std::cout<<pow(sinh(x*y+z),2.0)-pow(cosh(x*y+z),2.0);\n"
                             "    std::cout<<pow(sinh(x),2.0)-pow(cosh(x),2.0);\n"
                             "    std::cout<<pow(cosh(x*y+z),2)-pow(sinh(x*y+z),2);\n"
                             "    std::cout<<pow(cosh(x),2)-pow(sinh(x),2);\n"
                             "    std::cout<<pow(cosh(x*y+z),2.0)-pow(sinh(x*y+z),2.0);\n"
                             "    std::cout<<pow(cosh(x),2.0)-pow(sinh(x),2.0);\n"
                             "    std::cout<<pow(cosh(pow(x,1)),2.0)-pow(sinh(pow(x,1)),2.0);\n"
                             "}";

        const char expected1[] = "void foo ( ) {\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "}";
        ASSERT_EQUALS(expected1, tokWithNewlines(code1));

        const char code2[] = "void f ( ) {\n"
                             "a = pow ( sin ( x ) , 2 ) + pow ( cos ( y ) , 2 ) ;\n"
                             "b = pow ( sinh ( x ) , 2 ) - pow ( cosh ( y ) , 2 ) ;\n"
                             "c = pow ( sin ( x ) , 2.0 ) + pow ( cos ( y ) , 2.0 ) ;\n"
                             "d = pow ( sinh ( x ) , 2.0 ) - pow ( cosh ( y ) , 2.0 ) ;\n"
                             "e = pow ( cos ( x ) , 2 ) + pow ( sin ( y ) , 2 ) ;\n"
                             "f = pow ( cosh ( x ) , 2 ) - pow ( sinh ( y ) , 2 ) ;\n"
                             "g = pow ( cos ( x ) , 2.0 ) + pow ( sin ( y ) , 2.0 ) ;\n"
                             "h = pow ( cosh ( x ) , 2.0 ) - pow ( sinh ( y ) , 2.0 ) ;\n"
                             "}";
        ASSERT_EQUALS(code2, tokWithNewlines(code2));

        const char code3[] = "void foo() {\n"
                             "    std::cout<<powf(sinf(x),2)+powf(cosf(x),2);\n"
                             "    std::cout<<powf(sinf(powf(sinf(y),2)+powf(cosf(y),2)),2)+powf(cosf(powf(sinf(y),2)+powf(cosf(y),2)),2);\n"
                             "    std::cout<<powf(sinf(x),2.0)+powf(cosf(x),2.0);\n"
                             "    std::cout<<powf(sinf(x*y+z),2.0)+powf(cosf(x*y+z),2.0);\n"
                             "    std::cout<<powf(sinf(x*y+z),2)+powf(cosf(x*y+z),2);\n"
                             "    std::cout<<powf(cosf(x),2)+powf(sinf(x),2);\n"
                             "    std::cout<<powf(cosf(x),2.0)+powf(sinf(x),2.0);\n"
                             "    std::cout<<powf(cosf(x*y+z),2.0)+powf(sinf(x*y+z),2.0);\n"
                             "    std::cout<<powf(cosf(x*y+z),2)+powf(sinf(x*y+z),2);\n"
                             "    std::cout<<powf(sinhf(x*y+z),2)-powf(coshf(x*y+z),2);\n"
                             "    std::cout<<powf(sinhf(x),2)-powf(coshf(x),2);\n"
                             "    std::cout<<powf(sinhf(x*y+z),2.0)-powf(coshf(x*y+z),2.0);\n"
                             "    std::cout<<powf(sinhf(x),2.0)-powf(coshf(x),2.0);\n"
                             "    std::cout<<powf(coshf(x*y+z),2)-powf(sinhf(x*y+z),2);\n"
                             "    std::cout<<powf(coshf(x),2)-powf(sinhf(x),2);\n"
                             "    std::cout<<powf(coshf(x*y+z),2.0)-powf(sinhf(x*y+z),2.0);\n"
                             "    std::cout<<powf(coshf(x),2.0)-powf(sinhf(x),2.0);\n"
                             "    std::cout<<powf(coshf(powf(x,1)),2.0)-powf(sinhf(powf(x,1)),2.0);\n"
                             "}";

        const char expected3[] = "void foo ( ) {\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "}";
        ASSERT_EQUALS(expected3, tokWithNewlines(code3));

        const char code4[] = "void f ( ) {\n"
                             "a = powf ( sinf ( x ) , 2 ) + powf ( cosf ( y ) , 2 ) ;\n"
                             "b = powf ( sinhf ( x ) , 2 ) - powf ( coshf ( y ) , 2 ) ;\n"
                             "c = powf ( sinf ( x ) , 2.0 ) + powf ( cosf ( y ) , 2.0 ) ;\n"
                             "d = powf ( sinhf ( x ) , 2.0 ) - powf ( coshf ( y ) , 2.0 ) ;\n"
                             "e = powf ( cosf ( x ) , 2 ) + powf ( sinf ( y ) , 2 ) ;\n"
                             "f = powf ( coshf ( x ) , 2 ) - powf ( sinhf ( y ) , 2 ) ;\n"
                             "g = powf ( cosf ( x ) , 2.0 ) + powf ( sinf ( y ) , 2.0 ) ;\n"
                             "h = powf ( coshf ( x ) , 2.0 ) - powf ( sinhf ( y ) , 2.0 ) ;\n"
                             "}";
        ASSERT_EQUALS(code4, tokWithNewlines(code4));

        const char code5[] = "void foo() {\n"
                             "    std::cout<<powf(sinl(x),2)+powl(cosl(x),2);\n"
                             "    std::cout<<pow(sinl(powl(sinl(y),2)+powl(cosl(y),2)),2)+powl(cosl(powl(sinl(y),2)+powl(cosl(y),2)),2);\n"
                             "    std::cout<<powl(sinl(x),2.0)+powl(cosl(x),2.0);\n"
                             "    std::cout<<powl(sinl(x*y+z),2.0)+powl(cosl(x*y+z),2.0);\n"
                             "    std::cout<<powl(sinl(x*y+z),2)+powl(cosl(x*y+z),2);\n"
                             "    std::cout<<powl(cosl(x),2)+powl(sinl(x),2);\n"
                             "    std::cout<<powl(cosl(x),2.0)+powl(sinl(x),2.0);\n"
                             "    std::cout<<powl(cosl(x*y+z),2.0)+powl(sinl(x*y+z),2.0);\n"
                             "    std::cout<<powl(cosl(x*y+z),2)+powl(sinl(x*y+z),2);\n"
                             "    std::cout<<powl(sinhl(x*y+z),2)-powl(coshl(x*y+z),2);\n"
                             "    std::cout<<powl(sinhl(x),2)-powl(coshl(x),2);\n"
                             "    std::cout<<powl(sinhl(x*y+z),2.0)-powl(coshl(x*y+z),2.0);\n"
                             "    std::cout<<powl(sinhl(x),2.0)-powl(coshl(x),2.0);\n"
                             "    std::cout<<powl(coshl(x*y+z),2)-powl(sinhl(x*y+z),2);\n"
                             "    std::cout<<powl(coshl(x),2)-powl(sinhl(x),2);\n"
                             "    std::cout<<powl(coshl(x*y+z),2.0)-powl(sinhl(x*y+z),2.0);\n"
                             "    std::cout<<powl(coshl(x),2.0)-powl(sinhl(x),2.0);\n"
                             "    std::cout<<powl(coshl(powl(x,1)),2.0)-powl(sinhl(powl(x,1)),2.0);\n"
                             "}";

        const char expected5[] = "void foo ( ) {\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << 1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "std :: cout << -1 ;\n"
                                 "}";
        ASSERT_EQUALS(expected5, tokWithNewlines(code5));


        const char code6[] = "void f ( ) {\n"
                             "a = powl ( sinl ( x ) , 2 ) + powl ( cosl ( y ) , 2 ) ;\n"
                             "b = powl ( sinhl ( x ) , 2 ) - powl ( coshl ( y ) , 2 ) ;\n"
                             "c = powl ( sinl ( x ) , 2.0 ) + powl ( cosl ( y ) , 2.0 ) ;\n"
                             "d = powl ( sinhl ( x ) , 2.0 ) - powl ( coshl ( y ) , 2.0 ) ;\n"
                             "e = powl ( cosl ( x ) , 2 ) + powl ( sinl ( y ) , 2 ) ;\n"
                             "f = powl ( coshl ( x ) , 2 ) - powl ( sinhl ( y ) , 2 ) ;\n"
                             "g = powl ( cosl ( x ) , 2.0 ) + powl ( sinl ( y ) , 2.0 ) ;\n"
                             "h = powl ( coshl ( x ) , 2.0 ) - powl ( sinhl ( y ) , 2.0 ) ;\n"
                             "}";
        ASSERT_EQUALS(code6, tokWithNewlines(code6));
    }


    void simplifyCompoundAssignment() {
        ASSERT_EQUALS("; x = x + y ;", tok("; x += y;"));
        ASSERT_EQUALS("; x = x - y ;", tok("; x -= y;"));
        ASSERT_EQUALS("; x = x * y ;", tok("; x *= y;"));
        ASSERT_EQUALS("; x = x / y ;", tok("; x /= y;"));
        ASSERT_EQUALS("; x = x % y ;", tok("; x %= y;"));
        ASSERT_EQUALS("; x = x & y ;", tok("; x &= y;"));
        ASSERT_EQUALS("; x = x | y ;", tok("; x |= y;"));
        ASSERT_EQUALS("; x = x ^ y ;", tok("; x ^= y;"));
        ASSERT_EQUALS("; x = x << y ;", tok("; x <<= y;"));
        ASSERT_EQUALS("; x = x >> y ;", tok("; x >>= y;"));

        ASSERT_EQUALS("{ x = x + y ; }", tok("{ x += y;}"));
        ASSERT_EQUALS("{ x = x - y ; }", tok("{ x -= y;}"));
        ASSERT_EQUALS("{ x = x * y ; }", tok("{ x *= y;}"));
        ASSERT_EQUALS("{ x = x / y ; }", tok("{ x /= y;}"));
        ASSERT_EQUALS("{ x = x % y ; }", tok("{ x %= y;}"));
        ASSERT_EQUALS("{ x = x & y ; }", tok("{ x &= y;}"));
        ASSERT_EQUALS("{ x = x | y ; }", tok("{ x |= y;}"));
        ASSERT_EQUALS("{ x = x ^ y ; }", tok("{ x ^= y;}"));
        ASSERT_EQUALS("{ x = x << y ; }", tok("{ x <<= y;}"));
        ASSERT_EQUALS("{ x = x >> y ; }", tok("{ x >>= y;}"));

        ASSERT_EQUALS("; * p = * p + y ;", tok("; *p += y;"));
        ASSERT_EQUALS("; ( * p ) = ( * p ) + y ;", tok("; (*p) += y;"));
        ASSERT_EQUALS("; * ( p [ 0 ] ) = * ( p [ 0 ] ) + y ;", tok("; *(p[0]) += y;"));
        ASSERT_EQUALS("; p [ { 1 , 2 } ] = p [ { 1 , 2 } ] + y ;", tok("; p[{1,2}] += y;"));

        ASSERT_EQUALS("void foo ( ) { switch ( n ) { case 0 : ; x = x + y ; break ; } }", tok("void foo() { switch (n) { case 0: x += y; break; } }"));

        ASSERT_EQUALS("; x . y = x . y + 1 ;", tok("; x.y += 1;"));

        ASSERT_EQUALS("; x [ 0 ] = x [ 0 ] + 1 ;", tok("; x[0] += 1;"));
        ASSERT_EQUALS("; x [ y - 1 ] = x [ y - 1 ] + 1 ;", tok("; x[y-1] += 1;"));
        ASSERT_EQUALS("; x [ y ] = x [ y ++ ] + 1 ;", tok("; x[y++] += 1;"));
        ASSERT_EQUALS("; x [ ++ y ] = x [ y ] + 1 ;", tok("; x[++y] += 1;"));

        ASSERT_EQUALS(";", tok(";x += 0;"));
        TODO_ASSERT_EQUALS(";", "; x = x + '\\0' ;", tok("; x += '\\0'; "));
        ASSERT_EQUALS(";", tok(";x -= 0;"));
        ASSERT_EQUALS(";", tok(";x |= 0;"));
        ASSERT_EQUALS(";", tok(";x *= 1;"));
        ASSERT_EQUALS(";", tok(";x /= 1;"));

        ASSERT_EQUALS("; a . x ( ) = a . x ( ) + 1 ;", tok("; a.x() += 1;"));
        ASSERT_EQUALS("; x ( 1 ) = x ( 1 ) + 1 ;", tok("; x(1) += 1;"));

        // #2368
        ASSERT_EQUALS("{ j = j - i ; }", tok("{if (false) {} else { j -= i; }}"));

        // #2714 - wrong simplification of "a += b?c:d;"
        ASSERT_EQUALS("; a = a + ( b ? c : d ) ;", tok("; a+=b?c:d;"));
        ASSERT_EQUALS("; a = a * ( b + 1 ) ;", tok("; a*=b+1;"));

        ASSERT_EQUALS("; a = a + ( b && c ) ;", tok("; a+=b&&c;"));
        ASSERT_EQUALS("; a = a * ( b || c ) ;", tok("; a*=b||c;"));
        ASSERT_EQUALS("; a = a | ( b == c ) ;", tok("; a|=b==c;"));

        // #3469
        ASSERT_EQUALS("; a = a + ( b = 1 ) ;", tok("; a += b = 1;"));

        // #7571
        ASSERT_EQUALS("; foo = foo + [ & ] ( ) { } ;", tok("; foo += [&]() {int i;};"));

        // #8796
        ASSERT_EQUALS("{ return ( a = b ) += c ; }", tok("{ return (a = b) += c; }"));
    }



    void combine_strings() {
        const char code1[] =  "void foo()\n"
                             "{\n"
                             "const char *a =\n"
                             "{\n"
                             "\"hello \"\n"
                             "\"world\"\n"
                             "};\n"
                             "}\n";

        const char code2[] =  "void foo()\n"
                             "{\n"
                             "const char *a =\n"
                             "{\n"
                             "\"hello world\"\n"
                             "};\n"
                             "}\n";
        ASSERT_EQUALS(tok(code2), tok(code1));

        const char code3[] = "x = L\"1\" TEXT(\"2\") L\"3\";";
        ASSERT_EQUALS("x = L\"123\" ;", tok(code3, false, Settings::Win64));

        const char code4[] = "x = TEXT(\"1\") L\"2\";";
        ASSERT_EQUALS("x = L\"1\" L\"2\" ;", tok(code4, false, Settings::Win64));
    }

    void combine_wstrings() {
        const char code[] =  "a = L\"hello \"  L\"world\" ;\n";

        const char expected[] =  "a = L\"hello world\" ;";

        Tokenizer tokenizer(&settings0, this);
        std::istringstream istr(code);
        ASSERT(tokenizer.tokenize(istr, "test.cpp"));

        ASSERT_EQUALS(expected, tokenizer.tokens()->stringifyList(nullptr, false));
    }

    void combine_ustrings() {
        const char code[] =  "abcd = u\"ab\" u\"cd\";";

        const char expected[] =  "abcd = u\"abcd\" ;";

        Tokenizer tokenizer(&settings0, this);
        std::istringstream istr(code);
        ASSERT(tokenizer.tokenize(istr, "test.cpp"));

        ASSERT_EQUALS(expected, tokenizer.tokens()->stringifyList(nullptr, false));
    }

    void combine_Ustrings() {
        const char code[] =  "abcd = U\"ab\" U\"cd\";";

        const char expected[] =  "abcd = U\"abcd\" ;";

        Tokenizer tokenizer(&settings0, this);
        std::istringstream istr(code);
        ASSERT(tokenizer.tokenize(istr, "test.cpp"));

        ASSERT_EQUALS(expected, tokenizer.tokens()->stringifyList(nullptr, false));
    }

    void combine_u8strings() {
        const char code[] =  "abcd = u8\"ab\" u8\"cd\";";

        const char expected[] =  "abcd = u8\"abcd\" ;";

        Tokenizer tokenizer(&settings0, this);
        std::istringstream istr(code);
        ASSERT(tokenizer.tokenize(istr, "test.cpp"));

        ASSERT_EQUALS(expected, tokenizer.tokens()->stringifyList(nullptr, false));
    }

    void combine_mixedstrings() {
        const char code[] = "abcdef = \"ab\" L\"cd\" \"ef\";";

        const char expected[] =  "abcdef = L\"abcdef\" ;";

        Tokenizer tokenizer(&settings0, this);
        std::istringstream istr(code);
        ASSERT(tokenizer.tokenize(istr, "test.cpp"));

        ASSERT_EQUALS(expected, tokenizer.tokens()->stringifyList(nullptr, false));
    }

    void double_plus() {
        {
            const char code1[] =  "void foo( int a )\n"
                                 "{\n"
                                 "a++;\n"
                                 "a--;\n"
                                 "++a;\n"
                                 "--a;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a ) { a ++ ; a -- ; ++ a ; -- a ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a )\n"
                                 "{\n"
                                 "a=a+a;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a ) { a = a + a ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                 "{\n"
                                 "a=a+++b;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a ++ + b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                 "{\n"
                                 "a=a---b;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a -- - b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                 "{\n"
                                 "a=a--+b;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a -- + b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                 "{\n"
                                 "a=a++-b;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a ++ - b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                 "{\n"
                                 "a=a+--b;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a + -- b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                 "{\n"
                                 "a=a-++b;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a - ++ b ; }", tok(code1));
        }
    }

    void redundant_plus() {
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                 "{\n"
                                 "a=a + + b;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a + b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                 "{\n"
                                 "a=a + + + b;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a + b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                 "{\n"
                                 "a=a + - b;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a - b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                 "{\n"
                                 "a=a - + b;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a - b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                 "{\n"
                                 "a=a - - b;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a + b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                 "{\n"
                                 "a=a - + - b;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a + b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                 "{\n"
                                 "a=a - - - b;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a - b ; }", tok(code1));
        }
    }

    void redundant_plus_numbers() {
        {
            const char code1[] =  "void foo( int a )\n"
                                 "{\n"
                                 "a=a + + 1;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a ) { a = a + 1 ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a )\n"
                                 "{\n"
                                 "a=a + + + 1;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a ) { a = a + 1 ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a )\n"
                                 "{\n"
                                 "a=a + - 1;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a ) { a = a - 1 ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a )\n"
                                 "{\n"
                                 "a=a - + 1;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a ) { a = a - 1 ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a )\n"
                                 "{\n"
                                 "a=a - - 1;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a ) { a = a + 1 ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a )\n"
                                 "{\n"
                                 "a=a - + - 1;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a ) { a = a + 1 ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a )\n"
                                 "{\n"
                                 "a=a - - - 1;\n"
                                 "}\n";
            ASSERT_EQUALS("void foo ( int a ) { a = a - 1 ; }", tok(code1));
        }
    }


    void declareVar() {
        const char code[] = "void f ( ) { char str [ 100 ] = \"100\" ; }";
        ASSERT_EQUALS(code, tok(code));
    }

    void declareArray() {
        const char code1[] = "void f ( ) { char str [ ] = \"100\" ; }";
        const char expected1[] = "void f ( ) { char str [ 4 ] = \"100\" ; }";
        ASSERT_EQUALS(expected1, tok(code1));

        const char code2[] = "char str [ ] = \"\\x00\";";
        const char expected2[] = "char str [ 2 ] = \"\\0\" ;";
        ASSERT_EQUALS(expected2, tok(code2));

        const char code3[] = "char str [ ] = \"\\0\";";
        const char expected3[] = "char str [ 2 ] = \"\\0\" ;";
        ASSERT_EQUALS(expected3, tok(code3));

        const char code4[] = "char str [ ] = \"\\n\\n\";";
        const char expected4[] = "char str [ 3 ] = \"\\n\\n\" ;";
        ASSERT_EQUALS(expected4, tok(code4));
    }

    void dontRemoveIncrement() {
        {
            const char code[] = "void f(int a)\n"
                                "{\n"
                                "    if (a > 10)\n"
                                "        a = 5;\n"
                                "    else\n"
                                "        a = 10;\n"
                                "    a++;\n"
                                "}\n";
            ASSERT_EQUALS("void f ( int a ) { if ( a > 10 ) { a = 5 ; } else { a = 10 ; } a ++ ; }", tok(code));
        }

        {
            const char code[] = "void f(int a)\n"
                                "{\n"
                                "    if (a > 10)\n"
                                "        a = 5;\n"
                                "    else\n"
                                "        a = 10;\n"
                                "    ++a;\n"
                                "}\n";
            ASSERT_EQUALS("void f ( int a ) { if ( a > 10 ) { a = 5 ; } else { a = 10 ; } ++ a ; }", tok(code));
        }
    }


    void elseif1() {
        const char code[] = "void f(){ if(x) {} else if(ab) { cd } else { ef }gh; }";
        ASSERT_EQUALS("\n\n##file 0\n1: void f ( ) { if ( x ) { } else { if ( ab ) { cd } else { ef } } gh ; }\n", tokenizeDebugListing(code));

        // syntax error: assert there is no segmentation fault
        ASSERT_EQUALS("\n\n##file 0\n1: void f ( ) { if ( x ) { } else { if ( x ) { } } }\n", tokenizeDebugListing("void f(){ if(x) {} else if (x) { } }"));

        {
            const char src[] =  "void f(int g,int f) {\n"
                               "if(g==1) {poo();}\n"
                               "else if( g == 2 )\n"
                               "{\n"
                               " if( f == 0 ){coo();}\n"
                               " else if( f==1)\n"
                               "  goo();\n"
                               "}\n"
                               "}";

            const char expected[] = "void f ( int g , int f ) "
                                    "{ "
                                    "if ( g == 1 ) { poo ( ) ; } "
                                    "else { "
                                    "if ( g == 2 ) "
                                    "{ "
                                    "if ( f == 0 ) { coo ( ) ; } "
                                    "else { "
                                    "if ( f == 1 ) "
                                    "{ "
                                    "goo ( ) ; "
                                    "} "
                                    "} "
                                    "} "
                                    "} "
                                    "}";
            ASSERT_EQUALS(tok(expected), tok(src));
        }

        // Ticket #6860 - lambdas
        {
            const char src[] = "( []{if (ab) {cd}else if(ef) { gh } else { ij }kl}() );";
            const char expected[] = "\n\n##file 0\n1: ( [ ] { if ( ab ) { cd } else { if ( ef ) { gh } else { ij } } kl } ( ) ) ;\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(src));
        }
        {
            const char src[] = "[ []{if (ab) {cd}else if(ef) { gh } else { ij }kl}() ];";
            const char expected[] = "\n\n##file 0\n1: [ [ ] { if ( ab ) { cd } else { if ( ef ) { gh } else { ij } } kl } ( ) ] ;\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(src));
        }
        {
            const char src[] = "= { []{if (ab) {cd}else if(ef) { gh } else { ij }kl}() }";
            const char expected[] = "\n\n##file 0\n1: = { [ ] { if ( ab ) { cd } else { if ( ef ) { gh } else { ij } } kl } ( ) }\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(src));
        }
    }


    void namespaces() {
        {
            const char code[] = "namespace std { }";

            ASSERT_EQUALS(";", tok(code));
        }

        {
            const char code[] = "; namespace std { }";

            ASSERT_EQUALS(";", tok(code));
        }

        {
            const char code[] = "using namespace std; namespace a{ namespace b{ void f(){} } }";

            const char expected[] = "namespace a { namespace b { void f ( ) { } } }";

            ASSERT_EQUALS(expected, tok(code));
        }

        {
            const char code[] = "namespace b{ void f(){} }";

            const char expected[] = "namespace b { void f ( ) { } }";

            ASSERT_EQUALS(expected, tok(code));
        }

        {
            const char code[] = "void f(int namespace) { }";

            const char expected[] = "void f ( int namespace ) { }";

            ASSERT_EQUALS(expected, tok(code));
        }
    }

    void not1() {
        ASSERT_EQUALS("void f ( ) { if ( ! p ) { ; } }", tok("void f() { if (not p); }", "test.c", false));
        ASSERT_EQUALS("void f ( ) { if ( p && ! q ) { ; } }", tok("void f() { if (p && not q); }", "test.c", false));
        ASSERT_EQUALS("void f ( ) { a = ! ( p && q ) ; }", tok("void f() { a = not(p && q); }", "test.c", false));
        // Don't simplify 'not' or 'compl' if they are defined as a type;
        // in variable declaration and in function declaration/definition
        ASSERT_EQUALS("struct not { int x ; } ;", tok("struct not { int x; };", "test.c", false));
        ASSERT_EQUALS("void f ( ) { not p ; compl c ; }", tok(" void f() { not p; compl c; }", "test.c", false));
        ASSERT_EQUALS("void foo ( not i ) ;", tok("void foo(not i);", "test.c", false));
        ASSERT_EQUALS("int foo ( not i ) { return g ( i ) ; }", tok("int foo(not i) { return g(i); }", "test.c", false));
    }

    void and1() {
        ASSERT_EQUALS("void f ( ) { if ( p && q ) { ; } }",
                      tok("void f() { if (p and q) ; }", "test.c", false));

        ASSERT_EQUALS("void f ( ) { if ( foo ( ) && q ) { ; } }",
                      tok("void f() { if (foo() and q) ; }", "test.c", false));

        ASSERT_EQUALS("void f ( ) { if ( foo ( ) && bar ( ) ) { ; } }",
                      tok("void f() { if (foo() and bar()) ; }", "test.c", false));

        ASSERT_EQUALS("void f ( ) { if ( p && bar ( ) ) { ; } }",
                      tok("void f() { if (p and bar()) ; }", "test.c", false));

        ASSERT_EQUALS("void f ( ) { if ( p && ! q ) { ; } }",
                      tok("void f() { if (p and not q) ; }", "test.c", false));

        ASSERT_EQUALS("void f ( ) { r = a && b ; }",
                      tok("void f() { r = a and b; }", "test.c", false));

        ASSERT_EQUALS("void f ( ) { r = ( a || b ) && ( c || d ) ; }",
                      tok("void f() { r = (a || b) and (c || d); }", "test.c", false));

        ASSERT_EQUALS("void f ( ) { if ( test1 [ i ] == 'A' && test2 [ i ] == 'C' ) { } }",
                      tok("void f() { if (test1[i] == 'A' and test2[i] == 'C') {} }", "test.c", false));
    }

    void or1() {
        ASSERT_EQUALS("void f ( ) { if ( p || q ) { ; } }",
                      tok("void f() { if (p or q) ; }", "test.c", false));

        ASSERT_EQUALS("void f ( ) { if ( foo ( ) || q ) { ; } }",
                      tok("void f() { if (foo() or q) ; }", "test.c", false));

        ASSERT_EQUALS("void f ( ) { if ( foo ( ) || bar ( ) ) { ; } }",
                      tok("void f() { if (foo() or bar()) ; }", "test.c", false));

        ASSERT_EQUALS("void f ( ) { if ( p || bar ( ) ) { ; } }",
                      tok("void f() { if (p or bar()) ; }", "test.c", false));

        ASSERT_EQUALS("void f ( ) { if ( p || ! q ) { ; } }",
                      tok("void f() { if (p or not q) ; }", "test.c", false));

        ASSERT_EQUALS("void f ( ) { r = a || b ; }",
                      tok("void f() { r = a or b; }", "test.c", false));

        ASSERT_EQUALS("void f ( ) { r = ( a && b ) || ( c && d ) ; }",
                      tok("void f() { r = (a && b) or (c && d); }", "test.c", false));
    }

    void cAlternativeTokens() {
        ASSERT_EQUALS("void f ( ) { err |= ( ( r & s ) && ! t ) ; }",
                      tok("void f() { err or_eq ((r bitand s) and not t); }", "test.c", false));
        ASSERT_EQUALS("void f ( ) const { r = f ( a [ 4 ] | 0x0F , ~ c , ! d ) ; }",
                      tok("void f() const { r = f(a[4] bitor 0x0F, compl c, not d) ; }", "test.c", false));

    }

    void comma_keyword() {
        {
            const char code[] = "void foo()\n"
                                "{\n"
                                "    char *a, *b;\n"
                                "    delete a, delete b;\n"
                                "}\n";
            ASSERT_EQUALS("void foo ( ) { char * a ; char * b ; delete a ; delete b ; }", tok(code));
        }

        {
            const char code[] = "void foo()\n"
                                "{\n"
                                "    struct A *a, *b;\n"
                                "}\n";
            ASSERT_EQUALS("void foo ( ) { struct A * a ; struct A * b ; }", tok(code));
        }

        {
            const char code[] = "void foo()\n"
                                "{\n"
                                "    struct A **a, **b;\n"
                                "}\n";
            ASSERT_EQUALS("void foo ( ) { struct A * * a ; struct A * * b ; }", tok(code));
        }

        {
            const char code[] = "void foo()\n"
                                "{\n"
                                "    char *a, *b;\n"
                                "    delete a, b;\n"
                                "}\n";
            ASSERT_EQUALS("void foo ( ) { char * a ; char * b ; delete a ; b ; }", tok(code));
        }

        {
            const char code[] = "void foo()\n"
                                "{\n"
                                "    char *a, *b, *c;\n"
                                "    delete a, b, c;\n"
                                "}\n";
            // delete a; b; c; would be better but this will do too
            ASSERT_EQUALS("void foo ( ) { char * a ; char * b ; char * c ; delete a ; b , c ; }", tok(code));
        }

        {
            const char code[] = "void foo()\n"
                                "{\n"
                                "    char *a, *b;\n"
                                "    if (x)\n"
                                "        delete a, b;\n"
                                "}\n";
            ASSERT_EQUALS("void foo ( ) { char * a ; char * b ; if ( x ) { delete a ; b ; } }", tok(code));
        }

        {
            const char code[] = "void foo()\n"
                                "{\n"
                                "    char *a, *b, *c;\n"
                                "    if (x) \n"
                                "        delete a, b, c;\n"
                                "}\n";
            // delete a; b; c; would be better but this will do too
            ASSERT_EQUALS("void foo ( ) { char * a ; char * b ; char * c ; if ( x ) { delete a ; b , c ; } }", tok(code));
        }

        {
            const char code[] = "void foo()\n"
                                "{\n"
                                "    char **a, **b, **c;\n"
                                "}\n";
            ASSERT_EQUALS("void foo ( ) { char * * a ; char * * b ; char * * c ; }", tok(code));
        }

        {
            const char code[] = "int f()\n"
                                "{\n"
                                "    if (something)\n"
                                "        return a(2, c(3, 4)), b(3), 10;\n"
                                "    return a(), b(0, 0, 0), 10;\n"
                                "}\n";
            ASSERT_EQUALS("int f ( )"
                          " {"
                          " if ( something )"
                          " {"
                          " a ( 2 , c ( 3 , 4 ) ) ;"
                          " b ( 3 ) ;"
                          " return 10 ;"
                          " }"
                          " a ( ) ;"
                          " b ( 0 , 0 , 0 ) ;"
                          " return 10 ; "
                          "}", tok(code));
        }

        {
            const char code[] = "void foo()\n"
                                "{\n"
                                "    delete [] a, a = 0;\n"
                                "}\n";
            ASSERT_EQUALS("void foo ( ) { delete [ ] a ; a = 0 ; }", tok(code));
        }

        {
            const char code[] = "void foo()\n"
                                "{\n"
                                "    delete a, a = 0;\n"
                                "}\n";
            ASSERT_EQUALS("void foo ( ) { delete a ; a = 0 ; }", tok(code));
        }

        {
            const char code[] = "void foo()\n"
                                "{\n"
                                "    if( x ) delete a, a = 0;\n"
                                "}\n";
            ASSERT_EQUALS("void foo ( ) { if ( x ) { delete a ; a = 0 ; } }", tok(code));
        }

        {
            const char code[] = "void f()\n"
                                "{\n"
                                "    for(int a,b; a < 10; a = a + 1, b = b + 1);\n"
                                "}\n";
            ASSERT_EQUALS("void f ( ) { for ( int a , b ; a < 10 ; a = a + 1 , b = b + 1 ) { ; } }", tok(code));
        }

        {
            const char code[] = "void f()\n"
                                "{\n"
                                "    char buf[BUFSIZ], **p;\n"
                                "    char *ptrs[BUFSIZ], **pp;\n"
                                "}\n";
            ASSERT_EQUALS("void f ( ) { char buf [ BUFSIZ ] ; char * * p ; char * ptrs [ BUFSIZ ] ; char * * pp ; }", tok(code));
        }

        {
            // #4786 - don't replace , with ; in ".. : public B, C .." code
            const char code[] = "template < class T = X > class A : public B , C { } ;";
            ASSERT_EQUALS(code, tok(code));
        }
    }

    void remove_comma() {
        {
            const char code[] = "void f()\n"
                                "{\n"
                                "  int a,b;\n"
                                "  if( a )\n"
                                "  a=0,\n"
                                "  b=0;\n"
                                "}\n";
            ASSERT_EQUALS("void f ( ) { int a ; int b ; if ( a ) { a = 0 ; b = 0 ; } }", tok(code));
        }

        {
            ASSERT_EQUALS("a ? ( b = c , d ) : e ;", tok("a ? b = c , d : e ;")); // Keep comma
        }

        {
            ASSERT_EQUALS("{ return a ? ( b = c , d ) : e ; }", tok("{ return a ? b = c , d : e ; }")); // Keep comma
        }

        {
            const char code[] = "void f()\n"
                                "{\n"
                                "  A a,b;\n"
                                "  if( a.f )\n"
                                "  a.f=b.f,\n"
                                "  a.g=b.g;\n"
                                "}\n";
            ASSERT_EQUALS("void f ( ) { A a ; A b ; if ( a . f ) { a . f = b . f ; a . g = b . g ; } }", tok(code));
        }

        // keep the comma in template specifiers..
        {
            const char code[] = "void f()\n"
                                "{\n"
                                "  int a = b<T<char,3>, int>();\n"
                                "}\n";
            ASSERT_EQUALS("void f ( ) { int a ; a = b < T < char , 3 > , int > ( ) ; }", tok(code));
        }

        {
            const char code[] = "void f() {\n"
                                "  a = new std::map<std::string, std::string>;\n"
                                "}\n";
            ASSERT_EQUALS("void f ( ) { a = new std :: map < std :: string , std :: string > ; }", tok(code));
        }

        {
            // ticket #1327
            const char code[] = "const C<1,2,3> foo ()\n"
                                "{\n"
                                "    return C<1,2,3>(x,y);\n"
                                "}\n";
            const char expected[]  = "const C < 1 , 2 , 3 > foo ( ) "
                                     "{"
                                     " return C < 1 , 2 , 3 > ( x , y ) ; "
                                     "}";
            ASSERT_EQUALS(expected, tok(code));
        }

        {
            const char code[] = "int foo ()\n"
                                "{\n"
                                "    return doSomething(), 0;\n"
                                "}\n";
            const char expected[]  = "int foo ( ) "
                                     "{"
                                     " doSomething ( ) ; return 0 ; "
                                     "}";
            ASSERT_EQUALS(expected, tok(code));
        }

        {
            const char code[] = "int foo ()\n"
                                "{\n"
                                "    return a=1, b=2;\n"
                                "}\n";
            const char expected[]  = "int foo ( ) "
                                     "{"
                                     " a = 1 ; return b = 2 ; "
                                     "}";
            ASSERT_EQUALS(expected, tok(code));
        }

        {
            const char code[]     = "tr = (struct reg){ .a = (1), .c = (2) };";
            const char expected[] = "tr = ( struct reg ) { . a = 1 , . c = ( 2 ) } ;";
            ASSERT_EQUALS(expected, tok(code));
        }
    }

    void simplifyConditionOperator() {
        {
            const char code[] = "(0?(false?1:2):3);";
            ASSERT_EQUALS("( 3 ) ;", tok(code));
        }

        {
            const char code[] = "(1?(false?1:2):3);";
            ASSERT_EQUALS("( ( 2 ) ) ;", tok(code));
        }

        {
            const char code[] = "int a = (1?0:1 == 1?0:1);";
            ASSERT_EQUALS("int a ; a = 0 ;", tok(code));
        }

        {
            const char code[] = "(1?0:foo());";
            ASSERT_EQUALS("( 0 ) ;", tok(code));
        }

        {
            const char code[] = "void f () { switch(n) { case 1?0:foo(): break; }}";
            // TODO Do not throw AST validation exception
            TODO_ASSERT_THROW(tok(code), InternalError);
            //ASSERT_EQUALS("void f ( ) { switch ( n ) { case 0 : ; break ; } }", tok(code));
        }

        {
            const char code[] = "void f () { switch(n) { case 1?0?1:0:foo(): break; }}";
            // TODO Do not throw AST validation exception
            TODO_ASSERT_THROW(tok(code), InternalError);
        }

        {
            const char code[] = "void f () { switch(n) { case 0?foo():1: break; }}";
            // TODO Do not throw AST validation exception
            TODO_ASSERT_THROW(tok(code), InternalError);
        }

        {
            const char code[] = "( true ? a ( ) : b ( ) );";
            ASSERT_EQUALS("( a ( ) ) ;", tok(code));
        }

        {
            const char code[] = "( true ? abc . a : abc . b );";
            ASSERT_EQUALS("( abc . a ) ;", tok(code));
        }


        //GNU extension: "x ?: y" <-> "x ? x : y"
        {
            const char code[] = "; a = 1 ? : x; b = 0 ? : 2;";
            ASSERT_EQUALS("; a = 1 ; b = 2 ;", tok(code));
        }

        // Ticket #3572 (segmentation fault)
        ASSERT_EQUALS("0 ; x = { ? y : z ; }", tok("0; x = { ? y : z; }"));

        {
            // #3922 - (true)
            ASSERT_EQUALS("; x = 2 ;", tok("; x = (true)?2:4;"));
            ASSERT_EQUALS("; x = 4 ;", tok("; x = (false)?2:4;"));
            ASSERT_EQUALS("; x = * a ;", tok("; x = (true)?*a:*b;"));
            ASSERT_EQUALS("; x = * b ;", tok("; x = (false)?*a:*b;"));
        }

        {
            // TODO Do not throw AST validation exception
            TODO_ASSERT_THROW(tok("; type = decay_t<decltype(true ? declval<T>() : declval<U>())>;"), InternalError);
            TODO_ASSERT_THROW(tok("; type = decay_t<decltype(false ? declval<T>() : declval<U>())>;"), InternalError);
        }
    }

    void simplifyOperator1() {
        // #3237 - error merging namespaces with operators
        const char code[] = "class c {\n"
                            "public:\n"
                            "    operator std::string() const;\n"
                            "    operator string() const;\n"
                            "};\n";
        const char expected[] = "class c { "
                                "public: "
                                "operatorstd::string ( ) const ; "
                                "operatorstring ( ) const ; "
                                "} ;";
        ASSERT_EQUALS(expected, tok(code));
    }

    void simplifyOperator2() {
        // #6576
        ASSERT_EQUALS("template < class T > class SharedPtr { "
                      "SharedPtr & operator= ( const SharedPtr < Y > & r ) ; "
                      "} ; "
                      "class TClass { "
                      "public: TClass & operator= ( const TClass & rhs ) ; "
                      "} ; "
                      "TClass :: TClass ( const TClass & other ) { operator= ( other ) ; }",
                      tok("template<class T>\n"
                          "    class SharedPtr {\n"
                          "    SharedPtr& operator=(SharedPtr<Y> const & r);\n"
                          "};\n"
                          "class TClass {\n"
                          "public:\n"
                          "    TClass& operator=(const TClass& rhs);\n"
                          "};\n"
                          "TClass::TClass(const TClass &other) {\n"
                          "    operator=(other);\n"
                          "}"));
    }

    void simplifyArrayAccessSyntax() {
        ASSERT_EQUALS("\n\n##file 0\n"
                      "1: int a@1 ; a@1 [ 13 ] ;\n", tokenizeDebugListing("int a; 13[a];"));
    }


    void pointeralias1() {
        {
            const char code[] = "void f(char *p1)\n"
                                "{\n"
                                "    char *p = p1;\n"
                                "    p1 = 0;"
                                "    x(p);\n"
                                "}\n";

            const char expected[] = "void f ( char * p1 ) "
                                    "{ "
                                    "char * p ; p = p1 ; "
                                    "p1 = 0 ; "
                                    "x ( p ) ; "
                                    "}";

            ASSERT_EQUALS(expected, tok(code));
        }

        {
            const char code[] = "void foo(Result* ptr)\n"
                                "{\n"
                                "    Result* obj = ptr;\n"
                                "    ++obj->total;\n"
                                "}\n";

            const char expected[] = "void foo ( Result * ptr ) "
                                    "{ "
                                    "Result * obj ; obj = ptr ; "
                                    "++ obj . total ; "
                                    "}";

            ASSERT_EQUALS(expected, tok(code));
        }
    }

    void pointeralias3() {
        const char code[] = "void f()\n"
                            "{\n"
                            "    int i, j, *p;\n"
                            "    if (ab) p = &i;\n"
                            "    else p = &j;\n"
                            "    *p = 0;\n"
                            "}\n";
        const char expected[] = "void f ( ) "
                                "{"
                                " int i ; int j ; int * p ;"
                                " if ( ab ) { p = & i ; }"
                                " else { p = & j ; }"
                                " * p = 0 ; "
                                "}";
        ASSERT_EQUALS(expected, tok(code));
    }

    void simplifyStructDecl1() {
        {
            const char code[] = "struct ABC { } abc;";
            const char expected[] = "struct ABC { } ; struct ABC abc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct ABC { } * pabc;";
            const char expected[] = "struct ABC { } ; struct ABC * pabc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct ABC { } abc[4];";
            const char expected[] = "struct ABC { } ; struct ABC abc [ 4 ] ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct ABC { } abc, def;";
            const char expected[] = "struct ABC { } ; struct ABC abc ; struct ABC def ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct ABC { } abc, * pabc;";
            const char expected[] = "struct ABC { } ; struct ABC abc ; struct ABC * pabc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct ABC { struct DEF {} def; } abc;";
            const char expected[] = "struct ABC { struct DEF { } ; struct DEF def ; } ; struct ABC abc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct { } abc;";
            const char expected[] = "struct Anonymous0 { } ; struct Anonymous0 abc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct { } * pabc;";
            const char expected[] = "struct Anonymous0 { } ; struct Anonymous0 * pabc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct { } abc[4];";
            const char expected[] = "struct Anonymous0 { } ; struct Anonymous0 abc [ 4 ] ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct {int a;} const array[3] = {0};";
            const char expected[] = "struct Anonymous0 { int a ; } ; struct Anonymous0 const array [ 3 ] = { 0 } ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "static struct {int a;} const array[3] = {0};";
            const char expected[] = "struct Anonymous0 { int a ; } ; static struct Anonymous0 const array [ 3 ] = { 0 } ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct { } abc, def;";
            const char expected[] = "struct Anonymous0 { } ; struct Anonymous0 abc ; struct Anonymous0 def ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct { } abc, * pabc;";
            const char expected[] = "struct Anonymous0 { } ; struct Anonymous0 abc ; struct Anonymous0 * pabc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct { struct DEF {} def; } abc;";
            const char expected[] = "struct Anonymous0 { struct DEF { } ; struct DEF def ; } ; struct Anonymous0 abc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct ABC { struct {} def; } abc;";
            const char expected[] = "struct ABC { struct Anonymous0 { } ; struct Anonymous0 def ; } ; struct ABC abc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct { struct {} def; } abc;";
            const char expected[] = "struct Anonymous0 { struct Anonymous1 { } ; struct Anonymous1 def ; } ; struct Anonymous0 abc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "union ABC { int i; float f; } abc;";
            const char expected[] = "union ABC { int i ; float f ; } ; union ABC abc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct ABC { struct {} def; };";
            const char expected[] = "struct ABC { struct Anonymous0 { } ; struct Anonymous0 def ; } ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct ABC : public XYZ { struct {} def; };";
            const char expected[] = "struct ABC : public XYZ { struct Anonymous0 { } ; struct Anonymous0 def ; } ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct { int x; }; int y;";
            const char expected[] = "int x ; int y ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct { int x; };";
            const char expected[] = "int x ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct { };";
            const char expected[] = ";";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct { struct { struct { } ; } ; };";
            const char expected[] = ";";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        // ticket 2464
        {
            const char code[] = "static struct ABC { } abc ;";
            const char expected[] = "struct ABC { } ; static struct ABC abc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        // ticket #980
        {
            const char code[] = "void f() { int A(1),B(2),C=3,D,E(5),F=6; }";
            const char expected[] = "void f ( ) { int A ; A = 1 ; int B ; B = 2 ; int C ; C = 3 ; int D ; int E ; E = 5 ; int F ; F = 6 ; }";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        // ticket #8284
        {
            const char code[] = "void f() { class : foo<int> { } abc; }";
            const char expected[] = "void f ( ) { class Anonymous0 : foo < int > { } ; Anonymous0 abc ; }";
            ASSERT_EQUALS(expected, tok(code, false));
        }
    }

    void simplifyStructDecl2() { // ticket #2479 (segmentation fault)
        const char code[] = "struct { char c; }";
        const char expected[] = "struct { char c ; }";
        ASSERT_EQUALS(expected, tok(code, false));
    }

    void simplifyStructDecl3() {
        {
            const char code[] = "class ABC { } abc;";
            const char expected[] = "class ABC { } ; ABC abc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "class ABC { } * pabc;";
            const char expected[] = "class ABC { } ; ABC * pabc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "class ABC { } abc[4];";
            const char expected[] = "class ABC { } ; ABC abc [ 4 ] ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "class ABC { } abc, def;";
            const char expected[] = "class ABC { } ; ABC abc ; ABC def ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "class ABC { } abc, * pabc;";
            const char expected[] = "class ABC { } ; ABC abc ; ABC * pabc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "class ABC { class DEF {} def; } abc;";
            const char expected[] = "class ABC { class DEF { } ; DEF def ; } ; ABC abc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "class { } abc;";
            const char expected[] = "class { } abc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "class { } * pabc;";
            const char expected[] = "class { } * pabc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "class { } abc[4];";
            const char expected[] = "class { } abc [ 4 ] ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "class { } abc, def;";
            const char expected[] = "class { } abc , def ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "class { } abc, * pabc;";
            const char expected[] = "class { } abc , * pabc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "struct { class DEF {} def; } abc;";
            const char expected[] = "struct Anonymous0 { class DEF { } ; DEF def ; } ; struct Anonymous0 abc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "class ABC { struct {} def; } abc;";
            const char expected[] = "class ABC { struct Anonymous0 { } ; struct Anonymous0 def ; } ; ABC abc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "class { class {} def; } abc;";
            const char expected[] = "class { class { } def ; } abc ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "class ABC { struct {} def; };";
            const char expected[] = "class ABC { struct Anonymous0 { } ; struct Anonymous0 def ; } ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "class ABC : public XYZ { struct {} def; };";
            const char expected[] = "class ABC : public XYZ { struct Anonymous0 { } ; struct Anonymous0 def ; } ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "class { int x; }; int y;";
            const char expected[] = "class { int x ; } ; int y ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "class { int x; };";
            const char expected[] = "class { int x ; } ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "class { };";
            const char expected[] = "class { } ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }

        {
            const char code[] = "class { struct { struct { } ; } ; };";
            const char expected[] = "class { } ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }
    }

    void simplifyStructDecl4() {
        const char code[] = "class ABC {\n"
                            "    void foo() {\n"
                            "        union {\n"
                            "            int i;\n"
                            "            float f;\n"
                            "        };\n"
                            "        struct Fee { } fee;\n"
                            "    }\n"
                            "    union {\n"
                            "        long long ll;\n"
                            "        double d;\n"
                            "    };\n"
                            "} abc;\n";
        const char expected[] = "class ABC { "
                                "void foo ( ) { "
                                "int i ; "
                                "float & f = i ; "
                                "struct Fee { } ; struct Fee fee ; "
                                "} "
                                "union { "
                                "long long ll ; "
                                "double d ; "
                                "} ; "
                                "} ; ABC abc ;";
        ASSERT_EQUALS(expected, tok(code, false));
    }

    void simplifyStructDecl6() {
        ASSERT_EQUALS("struct A { "
                      "char integers [ X ] ; "
                      "} ; struct A arrays ; arrays = { { 0 } } ;",
                      tok("struct A {\n"
                          "    char integers[X];\n"
                          "} arrays = {{0}};", false));
    }

    void simplifyStructDecl7() {
        ASSERT_EQUALS("struct Anonymous0 { char x ; } ; struct Anonymous0 a [ 2 ] ;",
                      tok("struct { char x; } a[2];", false));
        ASSERT_EQUALS("struct Anonymous0 { char x ; } ; static struct Anonymous0 a [ 2 ] ;",
                      tok("static struct { char x; } a[2];", false));
    }

    void simplifyStructDecl8() {
        ASSERT_EQUALS("enum A { x , y , z } ; enum A a ; a = x ;", tok("enum A { x, y, z } a(x);", false));
        ASSERT_EQUALS("enum B { x , y , z } ; enum B b ; b = x ;", tok("enum B { x , y, z } b{x};", false));
        ASSERT_EQUALS("struct C { int i ; } ; struct C c ; c = { 0 } ;", tok("struct C { int i; } c{0};", false));
        ASSERT_EQUALS("enum Anonymous0 { x , y , z } ; enum Anonymous0 d ; d = x ;", tok("enum { x, y, z } d(x);", false));
        ASSERT_EQUALS("enum Anonymous0 { x , y , z } ; enum Anonymous0 e ; e = x ;", tok("enum { x, y, z } e{x};", false));
        ASSERT_EQUALS("struct Anonymous0 { int i ; } ; struct Anonymous0 f ; f = { 0 } ;", tok("struct { int i; } f{0};", false));
        ASSERT_EQUALS("struct Anonymous0 { } ; struct Anonymous0 x ; x = { 0 } ;", tok("struct {} x = {0};", false));
        ASSERT_EQUALS("enum G : short { x , y , z } ; enum G g ; g = x ;", tok("enum G : short { x, y, z } g(x);", false));
        ASSERT_EQUALS("enum H : short { x , y , z } ; enum H h ; h = x ;", tok("enum H : short { x, y, z } h{x};", false));
        ASSERT_EQUALS("enum class I : short { x , y , z } ; enum I i ; i = x ;", tok("enum class I : short { x, y, z } i(x);", false));
        ASSERT_EQUALS("enum class J : short { x , y , z } ; enum J j ; j = x ;", tok("enum class J : short { x, y, z } j{x};", false));
    }

    void removeUnwantedKeywords() {
        ASSERT_EQUALS("int var ;", tok("register int var ;", true));
        ASSERT_EQUALS("short var ;", tok("register short int var ;", true));
        ASSERT_EQUALS("int foo ( ) { }", tok("inline int foo ( ) { }", true));
        ASSERT_EQUALS("int foo ( ) { }", tok("__inline int foo ( ) { }", true));
        ASSERT_EQUALS("int foo ( ) { }", tok("__forceinline int foo ( ) { }", true));
        ASSERT_EQUALS("constexpr int foo ( ) { }", tok("constexpr int foo() { }", true));
        ASSERT_EQUALS("constexpr int foo ( ) { }", tok("consteval int foo() { }", true));
        ASSERT_EQUALS("int x ; x = 0 ;", tok("constinit int x = 0;", true));
        ASSERT_EQUALS("void f ( ) { int final [ 10 ] ; }", tok("void f() { int final[10]; }", true));
        ASSERT_EQUALS("int * p ;", tok("int * __restrict p;", "test.c"));
        ASSERT_EQUALS("int * * p ;", tok("int * __restrict__ * p;", "test.c"));
        ASSERT_EQUALS("void foo ( float * a , float * b ) ;", tok("void foo(float * __restrict__ a, float * __restrict__ b);", "test.c"));
        ASSERT_EQUALS("int * p ;", tok("int * restrict p;", "test.c"));
        ASSERT_EQUALS("int * * p ;", tok("int * restrict * p;", "test.c"));
        ASSERT_EQUALS("void foo ( float * a , float * b ) ;", tok("void foo(float * restrict a, float * restrict b);", "test.c"));
        ASSERT_EQUALS("void foo ( int restrict ) ;", tok("void foo(int restrict);"));
        ASSERT_EQUALS("int * p ;", tok("typedef int * __restrict__ rint; rint p;", "test.c"));

        // don't remove struct members:
        ASSERT_EQUALS("a = b . _inline ;", tok("a = b._inline;", true));

        ASSERT_EQUALS("int i ; i = 0 ;", tok("auto int i = 0;", "test.c"));
        ASSERT_EQUALS("auto i ; i = 0 ;", tok("auto i = 0;", "test.cpp"));
    }

    void simplifyCallingConvention() {
        ASSERT_EQUALS("int f ( ) ;", tok("int __cdecl f();", true));
        ASSERT_EQUALS("int f ( ) ;", tok("int __stdcall f();", true));
        ASSERT_EQUALS("int f ( ) ;", tok("int __fastcall f();", true));
        ASSERT_EQUALS("int f ( ) ;", tok("int __clrcall f();", true));
        ASSERT_EQUALS("int f ( ) ;", tok("int __thiscall f();", true));
        ASSERT_EQUALS("int f ( ) ;", tok("int __syscall f();", true));
        ASSERT_EQUALS("int f ( ) ;", tok("int __pascal f();", true));
        ASSERT_EQUALS("int f ( ) ;", tok("int __fortran f();", true));
        ASSERT_EQUALS("int f ( ) ;", tok("int __far __cdecl f();", true));
        ASSERT_EQUALS("int f ( ) ;", tok("int __far __stdcall f();", true));
        ASSERT_EQUALS("int f ( ) ;", tok("int __far __fastcall f();", true));
        ASSERT_EQUALS("int f ( ) ;", tok("int __far __clrcall f();", true));
        ASSERT_EQUALS("int f ( ) ;", tok("int __far __thiscall f();", true));
        ASSERT_EQUALS("int f ( ) ;", tok("int __far __syscall f();", true));
        ASSERT_EQUALS("int f ( ) ;", tok("int __far __pascal f();", true));
        ASSERT_EQUALS("int f ( ) ;", tok("int __far __fortran f();", true));
        ASSERT_EQUALS("int f ( ) ;", tok("int WINAPI f();", true, Settings::Win32A));
        ASSERT_EQUALS("int f ( ) ;", tok("int APIENTRY f();", true, Settings::Win32A));
        ASSERT_EQUALS("int f ( ) ;", tok("int CALLBACK f();", true, Settings::Win32A));

        // don't simplify Microsoft defines in unix code (#7554)
        ASSERT_EQUALS("enum E { CALLBACK } ;", tok("enum E { CALLBACK } ;", true, Settings::Unix32));
    }

    void simplifyAttribute() {
        ASSERT_EQUALS("int f ( ) ;", tok("__attribute__ ((visibility(\"default\"))) int f();", true));
        ASSERT_EQUALS("int f ( ) ;", tok("__attribute__((visibility(\"default\"))) int f();", true));
        ASSERT_EQUALS("int f ( ) ;", tok("__attribute ((visibility(\"default\"))) int f();", true));
        ASSERT_EQUALS("int f ( ) ;", tok("__attribute__ ((visibility(\"default\"))) __attribute__ ((warn_unused_result)) int f();", true));
        ASSERT_EQUALS("blah :: blah f ( ) ;", tok("__attribute__ ((visibility(\"default\"))) blah::blah f();", true));
        ASSERT_EQUALS("template < T > Result < T > f ( ) ;", tok("template<T> __attribute__ ((warn_unused_result)) Result<T> f();", true));
        ASSERT_EQUALS("template < T , U > Result < T , U > f ( ) ;", tok("template<T, U> __attribute__ ((warn_unused_result)) Result<T, U> f();", true));
    }

    void simplifyFunctorCall() {
        ASSERT_EQUALS("IncrementFunctor ( ) ( a ) ;", tok("IncrementFunctor()(a);", true));
    }

    // #ticket #5339 (simplify function pointer after comma)
    void simplifyFunctionPointer() {
        ASSERT_EQUALS("f ( double x , double ( * y ) ( ) ) ;", tok("f (double x, double (*y) ());", true));
    }

    void simplifyFunctionReturn() {
        {
            const char code[] = "typedef void (*testfp)();\n"
                                "struct Fred\n"
                                "{\n"
                                "    testfp get1() { return 0; }\n"
                                "    void ( * get2 ( ) ) ( ) { return 0 ; }\n"
                                "    testfp get3();\n"
                                "    void ( * get4 ( ) ) ( );\n"
                                "};";
            const char expected[] = "struct Fred "
                                    "{ "
                                    "void * get1 ( ) { return 0 ; } "
                                    "void * get2 ( ) { return 0 ; } "
                                    "void * get3 ( ) ; "
                                    "void * get4 ( ) ; "
                                    "} ;";
            ASSERT_EQUALS(expected, tok(code, false));
        }
        {
            const char code[] = "class Fred {\n"
                                "    std::string s;\n"
                                "    const std::string & foo();\n"
                                "};\n"
                                "const std::string & Fred::foo() { return \"\"; }";
            const char expected[] = "class Fred { "
                                    "std :: string s ; "
                                    "const std :: string & foo ( ) ; "
                                    "} ; "
                                    "const std :: string & Fred :: foo ( ) { return \"\" ; }";
            ASSERT_EQUALS(expected, tok(code, false));
        }
        {
            // Ticket #7916
            // Tokenization would include "int fact < 2 > ( ) { return 2 > ( ) ; }" and generate
            // a parse error (and use after free)
            const char code[] = "extern \"C\" void abort ();\n"
                                "template <int a> inline int fact2 ();\n"
                                "template <int a> inline int fact () {\n"
                                "  return a * fact2<a-1> ();\n"
                                "}\n"
                                "template <> inline int fact<1> () {\n"
                                "  return 1;\n"
                                "}\n"
                                "template <int a> inline int fact2 () {\n"
                                "  return a * fact<a-1>();\n"
                                "}\n"
                                "template <> inline int fact2<1> () {\n"
                                "  return 1;\n"
                                "}\n"
                                "int main() {\n"
                                "  fact2<3> ();\n"
                                "  fact2<2> ();\n"
                                "}";
            tok(code);
        }
    }

    void consecutiveBraces() {
        ASSERT_EQUALS("void f ( ) { }", tok("void f(){{}}", true));
        ASSERT_EQUALS("void f ( ) { }", tok("void f(){{{}}}", true));
        ASSERT_EQUALS("void f ( ) { for ( ; ; ) { } }", tok("void f () { for(;;){} }", true));
        ASSERT_EQUALS("void f ( ) { { scope_lock lock ; foo ( ) ; } { scope_lock lock ; bar ( ) ; } }", tok("void f () { {scope_lock lock; foo();} {scope_lock lock; bar();} }", true));
    }

    void simplifyOverride() { // ticket #5069
        const char code[] = "void fun() {\n"
                            "    unsigned char override[] = {0x01, 0x02};\n"
                            "    doSomething(override, sizeof(override));\n"
                            "}\n";
        ASSERT_EQUALS("void fun ( ) { char override [ 2 ] = { 0x01 , 0x02 } ; doSomething ( override , sizeof ( override ) ) ; }",
                      tok(code, true));
    }

    void simplifyNestedNamespace() {
        ASSERT_EQUALS("namespace A { namespace B { namespace C { int i ; } } }", tok("namespace A::B::C { int i; }"));
    }

    void simplifyNamespaceAliases1() {
        ASSERT_EQUALS(";",
                      tok("namespace ios = boost::iostreams;"));
        ASSERT_EQUALS("boost :: iostreams :: istream foo ( \"foo\" ) ;",
                      tok("namespace ios = boost::iostreams; ios::istream foo(\"foo\");"));
        ASSERT_EQUALS("boost :: iostreams :: istream foo ( \"foo\" ) ;",
                      tok("using namespace std; namespace ios = boost::iostreams; ios::istream foo(\"foo\");"));
        ASSERT_EQUALS(";",
                      tok("using namespace std; namespace ios = boost::iostreams;"));
        ASSERT_EQUALS("namespace NS { boost :: iostreams :: istream foo ( \"foo\" ) ; }",
                      tok("namespace NS { using namespace std; namespace ios = boost::iostreams; ios::istream foo(\"foo\"); }"));

        // duplicate namespace aliases
        ASSERT_EQUALS(";",
                      tok("namespace ios = boost::iostreams;\nnamespace ios = boost::iostreams;"));
        ASSERT_EQUALS(";",
                      tok("namespace ios = boost::iostreams;\nnamespace ios = boost::iostreams;\nnamespace ios = boost::iostreams;"));
        ASSERT_EQUALS("namespace A { namespace B { void foo ( ) { bar ( A :: B :: ab ( ) ) ; } } }",
                      tok("namespace A::B {"
                          "namespace AB = A::B;"
                          "void foo() {"
                          "    namespace AB = A::B;" // duplicate declaration
                          "    bar(AB::ab());"
                          "}"
                          "namespace AB = A::B;" //duplicate declaration
                          "}"));

        // redeclared nested namespace aliases
        TODO_ASSERT_EQUALS("namespace A { namespace B { void foo ( ) { bar ( A :: B :: ab ( ) ) ; { baz ( A :: a ( ) ) ; } bar ( A :: B :: ab ( ) ) ; } } }",
                           "namespace A { namespace B { void foo ( ) { bar ( A :: B :: ab ( ) ) ; { baz ( A :: B :: a ( ) ) ; } bar ( A :: B :: ab ( ) ) ; } } }",
                           tok("namespace A::B {"
                               "namespace AB = A::B;"
                               "void foo() {"
                               "    namespace AB = A::B;" // duplicate declaration
                               "    bar(AB::ab());"
                               "    {"
                               "         namespace AB = A;"
                               "         baz(AB::a());" // redeclaration OK
                               "    }"
                               "    bar(AB::ab());"
                               "}"
                               "namespace AB = A::B;" //duplicate declaration
                               "}"));

        // variable and namespace alias with same name
        ASSERT_EQUALS("namespace external { namespace ns { "
                      "class A { "
                      "public: "
                      "static void f ( const std :: string & json ) ; "
                      "} ; "
                      "} } "
                      "namespace external { namespace ns { "
                      "void A :: f ( const std :: string & json ) { } "
                      "} }",
                      tok("namespace external::ns {"
                          "    class A {"
                          "    public:"
                          "        static void f(const std::string& json);"
                          "    };"
                          "}"
                          "namespace json = rapidjson;"
                          "namespace external::ns {"
                          "    void A::f(const std::string& json) { }"
                          "}"));
    }

    void simplifyNamespaceAliases2() {
        ASSERT_EQUALS("void foo ( ) "
                      "{ "
                      "int maxResults ; maxResults = :: a :: b :: c :: d :: ef :: MAX ; "
                      "}",
                      tok("namespace ef = ::a::b::c::d::ef;"
                          "void foo()"
                          "{"
                          "  int maxResults = ::a::b::c::d::ef::MAX;"
                          "}"));
    }

#define simplifyKnownVariables(code) simplifyKnownVariables_(code, __FILE__, __LINE__)
    std::string simplifyKnownVariables_(const char code[], const char* file, int line) {
        errout.str("");

        Tokenizer tokenizer(&settings0, this);
        std::istringstream istr(code);
        ASSERT_LOC(tokenizer.tokenize(istr, "test.cpp"), file, line);

        return tokenizer.tokens()->stringifyList(nullptr, false);
    }

    void simplifyKnownVariables2() {
        const char code[] = "void f()\n"
                            "{\n"
                            "    int a = 10;\n"
                            "    a = g();\n"
                            "    if (a);\n"
                            "}\n";

        ASSERT_EQUALS(
            "void f ( ) { int a ; a = 10 ; a = g ( ) ; if ( a ) { ; } }",
            simplifyKnownVariables(code));
    }

    void simplifyKnownVariables3() {
        const char code[] = "void f()\n"
                            "{\n"
                            "    int a = 4;\n"
                            "    while(true){\n"
                            "    break;\n"
                            "    a = 10;\n"
                            "    }\n"
                            "    if (a);\n"
                            "}\n";

        ASSERT_EQUALS(
            "void f ( ) { int a ; a = 4 ; while ( true ) { break ; a = 10 ; } if ( a ) { ; } }",
            simplifyKnownVariables(code));
    }

    void simplifyKnownVariables4() {
        const char code[] = "void f()\n"
                            "{\n"
                            "    int a = 4;\n"
                            "    if ( g(a));\n"
                            "}\n";

        // TODO: if a is passed by value is is ok to simplify..
        ASSERT_EQUALS(
            "void f ( ) { int a ; a = 4 ; if ( g ( a ) ) { ; } }",
            simplifyKnownVariables(code));
    }

    void simplifyKnownVariables5() {
        const char code[] = "void f()\n"
                            "{\n"
                            "    int a = 4;\n"
                            "    if ( a = 5 );\n"
                            "}\n";

        ASSERT_EQUALS(
            "void f ( ) { int a ; a = 4 ; if ( a = 5 ) { ; } }",
            simplifyKnownVariables(code));
    }

    void simplifyKnownVariables13() {
        const char code[] = "void f()\n"
                            "{\n"
                            "    int i = 10;\n"
                            "    while(--i) {}\n"
                            "}\n";

        ASSERT_EQUALS(
            "void f ( ) { int i ; i = 10 ; while ( -- i ) { } }",
            simplifyKnownVariables(code));
    }

    void simplifyKnownVariables14() {
        // ticket #753
        const char code[] = "void f ( ) { int n ; n = 1 ; do { ++ n ; } while ( n < 10 ) ; }";
        ASSERT_EQUALS(code, simplifyKnownVariables(code));
    }

    void simplifyKnownVariables16() {
        // ticket #807 - segmentation fault when macro isn't found
        const char code[] = "void f ( ) { int n = 1; DISPATCH(while); }";
        ASSERT_THROW(simplifyKnownVariables(code), InternalError);
    }

    void simplifyKnownVariables17() {
        // ticket #807 - segmentation fault when macro isn't found
        const char code[] = "void f ( ) { char *s = malloc(100);mp_ptr p = s; p++; }";
        ASSERT_EQUALS(
            "void f ( ) { char * s ; s = malloc ( 100 ) ; mp_ptr p ; p = s ; p ++ ; }",
            simplifyKnownVariables(code));
    }

    void simplifyKnownVariables18() {
        const char code[] = "void f ( ) { char *s = malloc(100);mp_ptr p = s; ++p; }";
        ASSERT_EQUALS(
            "void f ( ) { char * s ; s = malloc ( 100 ) ; mp_ptr p ; p = s ; ++ p ; }",
            simplifyKnownVariables(code));
    }

    void simplifyKnownVariables19() {
        const char code[] = "void f ( ) { int i=0; do { if (i>0) { a(); } i=b(); } while (i != 12); }";
        ASSERT_EQUALS(
            "void f ( ) { int i ; i = 0 ; do { if ( i > 0 ) { a ( ) ; } i = b ( ) ; } while ( i != 12 ) ; }",
            simplifyKnownVariables(code));
    }

    void simplifyKnownVariables21() {
        ASSERT_EQUALS(
            "void foo ( int i ) { int n ; n = i ; for ( i = 0 ; i < n ; ++ i ) { } }",
            simplifyKnownVariables("void foo(int i) { int n = i; for (i = 0; i < n; ++i) { } }"));
    }

    void simplifyKnownVariables25() {
        {
            // This testcase is related to ticket #1646
            const char code[] = "void foo(char *str)\n"
                                "{\n"
                                "    int i;\n"
                                "    for (i=0;i<10;++i) {\n"
                                "        if (*str == 0) goto label;\n"
                                "    }\n"
                                "    return;\n"
                                "label:\n"
                                "    str[i] = 0;\n"
                                "}\n";

            // Current result
            ASSERT_EQUALS(
                "void foo ( char * str ) "
                "{"
                " int i ;"
                " for ( i = 0 ; i < 10 ; ++ i ) {"
                " if ( * str == 0 ) { goto label ; }"
                " }"
                " return ;"
                " label : ;"
                " str [ i ] = 0 ; "
                "}",
                simplifyKnownVariables(code));
        }

        {
            // This testcase is related to ticket #1646
            const char code[] = "void foo(char *str)\n"
                                "{\n"
                                "    int i;\n"
                                "    for (i=0;i<10;++i) { }\n"
                                "    return;\n"
                                "    str[i] = 0;\n"
                                "}\n";

            // Current result
            ASSERT_EQUALS(
                "void foo ( char * str ) "
                "{"
                " int i ;"
                " for ( i = 0 ; i < 10 ; ++ i ) { }"
                " return ;"
                " str [ i ] = 0 ; "
                "}",
                simplifyKnownVariables(code));
        }
    }

    void simplifyKnownVariables29() { // ticket #1811
        {
            const char code[] = "int foo(int u, int v)\n"
                                "{\n"
                                "  int h = u;\n"
                                "  int i = v;\n"
                                "  return h + i;\n"
                                "}\n";
            const char expected[] = "\n\n"
                                    "##file 0\n"
                                    "1: int foo ( int u@1 , int v@2 )\n"
                                    "2: {\n"
                                    "3:\n"
                                    "4:\n"
                                    "5: return u@1 + v@2 ;\n"
                                    "6: }\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(code, true));
        }

        {
            const char code[] = "int foo(int u, int v)\n"
                                "{\n"
                                "  int h = u;\n"
                                "  int i = v;\n"
                                "  return h - i;\n"
                                "}\n";
            const char expected[] = "\n\n"
                                    "##file 0\n"
                                    "1: int foo ( int u@1 , int v@2 )\n"
                                    "2: {\n"
                                    "3:\n"
                                    "4:\n"
                                    "5: return u@1 - v@2 ;\n"
                                    "6: }\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(code, true));
        }

        {
            const char code[] = "int foo(int u, int v)\n"
                                "{\n"
                                "  int h = u;\n"
                                "  int i = v;\n"
                                "  return h * i;\n"
                                "}\n";
            const char expected[] = "\n\n"
                                    "##file 0\n"
                                    "1: int foo ( int u@1 , int v@2 )\n"
                                    "2: {\n"
                                    "3:\n"
                                    "4:\n"
                                    "5: return u@1 * v@2 ;\n"
                                    "6: }\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(code, true));
        }

        {
            const char code[] = "int foo(int u, int v)\n"
                                "{\n"
                                "  int h = u;\n"
                                "  int i = v;\n"
                                "  return h / i;\n"
                                "}\n";
            const char expected[] = "\n\n"
                                    "##file 0\n"
                                    "1: int foo ( int u@1 , int v@2 )\n"
                                    "2: {\n"
                                    "3:\n"
                                    "4:\n"
                                    "5: return u@1 / v@2 ;\n"
                                    "6: }\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(code, true));
        }

        {
            const char code[] = "int foo(int u, int v)\n"
                                "{\n"
                                "  int h = u;\n"
                                "  int i = v;\n"
                                "  return h & i;\n"
                                "}\n";
            const char expected[] = "\n\n"
                                    "##file 0\n"
                                    "1: int foo ( int u@1 , int v@2 )\n"
                                    "2: {\n"
                                    "3:\n"
                                    "4:\n"
                                    "5: return u@1 & v@2 ;\n"
                                    "6: }\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(code, true));
        }

        {
            const char code[] = "int foo(int u, int v)\n"
                                "{\n"
                                "  int h = u;\n"
                                "  int i = v;\n"
                                "  return h | i;\n"
                                "}\n";
            const char expected[] = "\n\n"
                                    "##file 0\n"
                                    "1: int foo ( int u@1 , int v@2 )\n"
                                    "2: {\n"
                                    "3:\n"
                                    "4:\n"
                                    "5: return u@1 | v@2 ;\n"
                                    "6: }\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(code, true));
        }

        {
            const char code[] = "int foo(int u, int v)\n"
                                "{\n"
                                "  int h = u;\n"
                                "  int i = v;\n"
                                "  return h ^ i;\n"
                                "}\n";
            const char expected[] = "\n\n"
                                    "##file 0\n"
                                    "1: int foo ( int u@1 , int v@2 )\n"
                                    "2: {\n"
                                    "3:\n"
                                    "4:\n"
                                    "5: return u@1 ^ v@2 ;\n"
                                    "6: }\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(code, true));
        }

        {
            const char code[] = "int foo(int u, int v)\n"
                                "{\n"
                                "  int h = u;\n"
                                "  int i = v;\n"
                                "  return h % i;\n"
                                "}\n";
            const char expected[] = "\n\n"
                                    "##file 0\n"
                                    "1: int foo ( int u@1 , int v@2 )\n"
                                    "2: {\n"
                                    "3:\n"
                                    "4:\n"
                                    "5: return u@1 % v@2 ;\n"
                                    "6: }\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(code, true));
        }

        {
            const char code[] = "int foo(int u, int v)\n"
                                "{\n"
                                "  int h = u;\n"
                                "  int i = v;\n"
                                "  return h >> i;\n"
                                "}\n";
            const char expected[] = "\n\n"
                                    "##file 0\n"
                                    "1: int foo ( int u@1 , int v@2 )\n"
                                    "2: {\n"
                                    "3:\n"
                                    "4:\n"
                                    "5: return u@1 >> v@2 ;\n"
                                    "6: }\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(code, true));
        }

        {
            const char code[] = "int foo(int u, int v)\n"
                                "{\n"
                                "  int h = u;\n"
                                "  int i = v;\n"
                                "  return h << i;\n"
                                "}\n";
            const char expected[] = "\n\n"
                                    "##file 0\n"
                                    "1: int foo ( int u@1 , int v@2 )\n"
                                    "2: {\n"
                                    "3:\n"
                                    "4:\n"
                                    "5: return u@1 << v@2 ;\n"
                                    "6: }\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(code, true));
        }

        {
            const char code[] = "bool foo(int u, int v)\n"
                                "{\n"
                                "  int h = u;\n"
                                "  int i = v;\n"
                                "  return h == i;\n"
                                "}\n";
            const char expected[] = "\n\n"
                                    "##file 0\n"
                                    "1: bool foo ( int u@1 , int v@2 )\n"
                                    "2: {\n"
                                    "3:\n"
                                    "4:\n"
                                    "5: return u@1 == v@2 ;\n"
                                    "6: }\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(code, true));
        }

        {
            const char code[] = "bool foo(int u, int v)\n"
                                "{\n"
                                "  int h = u;\n"
                                "  int i = v;\n"
                                "  return h != i;\n"
                                "}\n";
            const char expected[] = "\n\n"
                                    "##file 0\n"
                                    "1: bool foo ( int u@1 , int v@2 )\n"
                                    "2: {\n"
                                    "3:\n"
                                    "4:\n"
                                    "5: return u@1 != v@2 ;\n"
                                    "6: }\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(code, true));
        }

        {
            const char code[] = "bool foo(int u, int v)\n"
                                "{\n"
                                "  int h = u;\n"
                                "  int i = v;\n"
                                "  return h > i;\n"
                                "}\n";
            const char expected[] = "\n\n"
                                    "##file 0\n"
                                    "1: bool foo ( int u@1 , int v@2 )\n"
                                    "2: {\n"
                                    "3:\n"
                                    "4:\n"
                                    "5: return u@1 > v@2 ;\n"
                                    "6: }\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(code, true));
        }

        {
            const char code[] = "bool foo(int u, int v)\n"
                                "{\n"
                                "  int h = u;\n"
                                "  int i = v;\n"
                                "  return h >= i;\n"
                                "}\n";
            const char expected[] = "\n\n"
                                    "##file 0\n"
                                    "1: bool foo ( int u@1 , int v@2 )\n"
                                    "2: {\n"
                                    "3:\n"
                                    "4:\n"
                                    "5: return u@1 >= v@2 ;\n"
                                    "6: }\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(code, true));
        }

        {
            const char code[] = "bool foo(int u, int v)\n"
                                "{\n"
                                "  int h = u;\n"
                                "  int i = v;\n"
                                "  return h < i;\n"
                                "}\n";
            const char expected[] = "\n\n"
                                    "##file 0\n"
                                    "1: bool foo ( int u@1 , int v@2 )\n"
                                    "2: {\n"
                                    "3:\n"
                                    "4:\n"
                                    "5: return u@1 < v@2 ;\n"
                                    "6: }\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(code, true));
        }

        {
            const char code[] = "bool foo(int u, int v)\n"
                                "{\n"
                                "  int h = u;\n"
                                "  int i = v;\n"
                                "  return h <= i;\n"
                                "}\n";
            const char expected[] = "\n\n"
                                    "##file 0\n"
                                    "1: bool foo ( int u@1 , int v@2 )\n"
                                    "2: {\n"
                                    "3:\n"
                                    "4:\n"
                                    "5: return u@1 <= v@2 ;\n"
                                    "6: }\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(code, true));
        }

        {
            const char code[] = "bool foo(int u, int v)\n"
                                "{\n"
                                "  int h = u;\n"
                                "  int i = v;\n"
                                "  return h && i;\n"
                                "}\n";
            const char expected[] = "\n\n"
                                    "##file 0\n"
                                    "1: bool foo ( int u@1 , int v@2 )\n"
                                    "2: {\n"
                                    "3:\n"
                                    "4:\n"
                                    "5: return u@1 && v@2 ;\n"
                                    "6: }\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(code, true));
        }

        {
            const char code[] = "bool foo(int u, int v)\n"
                                "{\n"
                                "  int h = u;\n"
                                "  int i = v;\n"
                                "  return h || i;\n"
                                "}\n";
            const char expected[] = "\n\n"
                                    "##file 0\n"
                                    "1: bool foo ( int u@1 , int v@2 )\n"
                                    "2: {\n"
                                    "3:\n"
                                    "4:\n"
                                    "5: return u@1 || v@2 ;\n"
                                    "6: }\n";
            ASSERT_EQUALS(expected, tokenizeDebugListing(code, true));
        }
    }

    void simplifyKnownVariables30() {
        const char code[] = "int foo() {\n"
                            "  iterator it1 = ints.begin();\n"
                            "  iterator it2 = it1;\n"
                            "  for (++it2;it2!=ints.end();++it2);\n"
                            "}\n";
        const char expected[] = "int foo ( ) {\n"
                                "iterator it1 ; it1 = ints . begin ( ) ;\n"
                                "iterator it2 ; it2 = it1 ;\n"
                                "for ( ++ it2 ; it2 != ints . end ( ) ; ++ it2 ) { ; }\n"
                                "}";
        ASSERT_EQUALS(expected, tokenizeAndStringify(code, true));
    }

    void simplifyKnownVariables34() {
        const char code[] = "void f() {\n"
                            "    int x = 10;\n"
                            "    do { cin >> x; } while (x > 5);\n"
                            "    a[x] = 0;\n"
                            "}\n";
        const char expected[] = "void f ( ) {\n"
                                "int x ; x = 10 ;\n"
                                "do { cin >> x ; } while ( x > 5 ) ;\n"
                                "a [ x ] = 0 ;\n"
                                "}";
        ASSERT_EQUALS(expected, tokenizeAndStringify(code, true));
    }

    void simplifyKnownVariables36() {

        // Ticket #5972
        const char code2[] = "void f() {"
                             "  char buf[10] = \"ab\";"
                             "    memset(buf, 0, 10);"
                             "}";
        const char expected2[] = "void f ( ) { char buf [ 10 ] = \"ab\" ; memset ( buf , 0 , 10 ) ; }";
        ASSERT_EQUALS(expected2, tokenizeAndStringify(code2, true));
    }

    void simplifyKnownVariables42() {
        {
            const char code[] = "void f() {\n"
                                "   char a[10];\n"
                                "   strcpy(a, \"hello\");\n"
                                "   strcat(a, \"!\");\n"
                                "}";
            const char expected[] = "void f ( ) {\n"
                                    "char a [ 10 ] ;\n"
                                    "strcpy ( a , \"hello\" ) ;\n"
                                    "strcat ( a , \"!\" ) ;\n"
                                    "}";
            ASSERT_EQUALS(expected, tokenizeAndStringify(code, true, true, Settings::Native, "test.c"));
        }

        {
            const char code[] = "void f() {"
                                "    char *s = malloc(10);"
                                "    strcpy(s, \"\");"
                                "    free(s);"
                                "}";
            const char expected[] = "void f ( ) {"
                                    " char * s ; s = malloc ( 10 ) ;"
                                    " strcpy ( s , \"\" ) ;"
                                    " free ( s ) ; "
                                    "}";
            ASSERT_EQUALS(expected, tokenizeAndStringify(code, true));
        }

        {
            const char code[] = "void f(char *p, char *q) {"
                                "    strcpy(p, \"abc\");"
                                "    q = p;"
                                "}";
            const char expected[] = "void f ( char * p , char * q ) {"
                                    " strcpy ( p , \"abc\" ) ;"
                                    " q = p ; "
                                    "}";
            ASSERT_EQUALS(expected, tokenizeAndStringify(code, true));
        }

        // 3538
        {
            const char code[] = "void f() {\n"
                                "    char s[10];\n"
                                "    strcpy(s, \"123\");\n"
                                "    if (s[6] == ' ');\n"
                                "}";
            const char expected[] = "void f ( ) {\n"
                                    "char s [ 10 ] ;\n"
                                    "strcpy ( s , \"123\" ) ;\n"
                                    "if ( s [ 6 ] == ' ' ) { ; }\n"
                                    "}";
            ASSERT_EQUALS(expected, tokenizeAndStringify(code,true));
        }
    }

    void simplifyKnownVariables43() {
        {
            const char code[] = "void f() {\n"
                                "    int a, *p; p = &a;\n"
                                "    { int a = *p; }\n"
                                "}";
            const char expected[] = "void f ( ) {\n"
                                    "int a ; int * p ; p = & a ;\n"
                                    "{ int a ; a = * p ; }\n"
                                    "}";
            ASSERT_EQUALS(expected, tokenizeAndStringify(code, true));
        }

        {
            const char code[] = "void f() {\n"
                                "    int *a, **p; p = &a;\n"
                                "    { int *a = *p; }\n"
                                "}";
            const char expected[] = "void f ( ) {\n"
                                    "int * a ; int * * p ; p = & a ;\n"
                                    "{ int * a ; a = * p ; }\n"
                                    "}";
            ASSERT_EQUALS(expected, tokenizeAndStringify(code, true));
        }
    }

    void simplifyKnownVariables44() {
        const char code[] = "void a() {\n"
                            "    static int i = 10;\n"
                            "    b(i++);\n"
                            "}";
        const char expected[] = "void a ( ) {\n"
                                "static int i = 10 ;\n"
                                "b ( i ++ ) ;\n"
                                "}";
        ASSERT_EQUALS(expected, tokenizeAndStringify(code, true));
    }

    void simplifyKnownVariables46() {
        const char code[] = "void f() {\n"
                            "    int x = 0;\n"
                            "    cin >> x;\n"
                            "    return x;\n"
                            "}";

        {
            const char expected[] = "void f ( ) {\n"
                                    "int x ; x = 0 ;\n"
                                    "cin >> x ;\n"
                                    "return x ;\n"
                                    "}";
            ASSERT_EQUALS(expected, tokenizeAndStringify(code, true, true, Settings::Native, "test.cpp"));
        }
    }

    void simplifyKnownVariables47() {
        // #3621
        const char code[] = "void f() {\n"
                            "    int x = 0;\n"
                            "    cin >> std::hex >> x;\n"
                            "}";
        const char expected[] = "void f ( ) {\n"
                                "int x ; x = 0 ;\n"
                                "cin >> std :: hex >> x ;\n"
                                "}";
        ASSERT_EQUALS(expected, tokenizeAndStringify(code, true, true, Settings::Native, "test.cpp"));
    }

    void simplifyKnownVariables48() {
        // #3754
        const char code[] = "void f(int sz) {\n"
                            "    int i;\n"
                            "    for (i = 0; ((i<sz) && (sz>3)); ++i) { }\n"
                            "}";
        const char expected[] = "void f ( int sz ) {\n"
                                "int i ;\n"
                                "for ( i = 0 ; ( i < sz ) && ( sz > 3 ) ; ++ i ) { }\n"
                                "}";
        ASSERT_EQUALS(expected, tokenizeAndStringify(code, true, true, Settings::Native, "test.c"));
    }

    void simplifyKnownVariables49() { // #3691
        const char code[] = "void f(int sz) {\n"
                            "    switch (x) {\n"
                            "    case 1: sz = 2; continue;\n"
                            "    case 2: x = sz; break;\n"
                            "    }\n"
                            "}";
        const char expected[] = "void f ( int sz ) {\n"
                                "switch ( x ) {\n"
                                "case 1 : ; sz = 2 ; continue ;\n"
                                "case 2 : ; x = sz ; break ;\n"
                                "}\n"
                                "}";
        ASSERT_EQUALS(expected, tokenizeAndStringify(code, true, true, Settings::Native, "test.c"));
    }

    void simplifyKnownVariables50() { // #4066
        {
            //don't simplify '&x'!
            const char code[] = "const char * foo ( ) {\n"
                                "const char x1 = 'b' ;\n"
                                "f ( & x1 ) ;\n"
                                "const char x2 = 'b' ;\n"
                                "f ( y , & x2 ) ;\n"
                                "const char x3 = 'b' ;\n"
                                "t = & x3 ;\n"
                                "const char x4 = 'b' ;\n"
                                "t = y + & x4 ;\n"
                                "const char x5 = 'b' ;\n"
                                "z [ & x5 ] = y ;\n"
                                "const char x6 = 'b' ;\n"
                                "v = { & x6 } ;\n"
                                "const char x7 = 'b' ;\n"
                                "return & x7 ;\n"
                                "}";
            ASSERT_EQUALS(code, tokenizeAndStringify(code, true));
        }
        {
            //don't simplify '&x'!
            const char code[] = "const int * foo ( ) {\n"
                                "const int x1 = 1 ;\n"
                                "f ( & x1 ) ;\n"
                                "const int x2 = 1 ;\n"
                                "f ( y , & x2 ) ;\n"
                                "const int x3 = 1 ;\n"
                                "t = & x3 ;\n"
                                "const int x4 = 1 ;\n"
                                "t = y + & x4 ;\n"
                                "const int x5 = 1 ;\n"
                                "z [ & x5 ] = y ;\n"
                                "const int x6 = 1 ;\n"
                                "v = { & x6 } ;\n"
                                "const int x7 = 1 ;\n"
                                "return & x7 ;\n"
                                "}";
            ASSERT_EQUALS(code, tokenizeAndStringify(code, true));
        }
    }

    void simplifyKnownVariables51() { // #4409 hang
        const char code[] = "void mhz_M(int enough) {\n"
                            "  TYPE *x=&x, **p=x, **q = NULL;\n"
                            "  BENCH1(q = _mhz_M(n); n = 1;)\n"
                            "  use_pointer(q);\n"
                            "}";
        ASSERT_THROW(tokenizeAndStringify(code, true), InternalError);
    }

    void simplifyKnownVariables54() { // #4913
        ASSERT_EQUALS("void f ( int * p ) { * -- p = 0 ; * p = 0 ; }", tokenizeAndStringify("void f(int*p) { *--p=0; *p=0; }", true));
    }

    void simplifyKnownVariables56() { // ticket #5301 - >>
        ASSERT_EQUALS("void f ( ) { int a ; a = 0 ; int b ; b = 0 ; * p >> a >> b ; return a / b ; }",
                      tokenizeAndStringify("void f() { int a=0,b=0; *p>>a>>b; return a/b; }", true));
    }

    void simplifyKnownVariables58() { // #5268
        const char code[] = "enum e { VAL1 = 1, VAL2 }; "
                            "typedef char arr_t[VAL2]; "
                            "int foo(int) ; "
                            "void bar () { "
                            "  throw foo (VAL1); "
                            "} "
                            "int baz() { "
                            "  return sizeof(arr_t); "
                            "}";
        ASSERT_EQUALS("enum e { VAL1 = 1 , VAL2 } ; "
                      "int foo ( int ) ; "
                      "void bar ( ) { "
                      "throw foo ( VAL1 ) ; "
                      "} "
                      "int baz ( ) { "
                      "return sizeof ( char [ VAL2 ] ) ; "
                      "}", tokenizeAndStringify(code, true));
    }

    void simplifyKnownVariables59() { // #5062 - for head
        const char code[] = "void f() {\n"
                            "  int a[3], i, j;\n"
                            "  for(i = 0, j = 1; i < 3, j < 12; i++,j++) {\n"
                            "    a[i] = 0;\n"
                            "  }\n"
                            "}";
        ASSERT_EQUALS("void f ( ) {\n"
                      "int a [ 3 ] ; int i ; int j ;\n"
                      "for ( i = 0 , j = 1 ; i < 3 , j < 12 ; i ++ , j ++ ) {\n"
                      "a [ i ] = 0 ;\n"
                      "}\n"
                      "}", tokenizeAndStringify(code, true));
    }

    void simplifyKnownVariables61() { // #7805
        tokenizeAndStringify("static const int XX = 0;\n"
                             "enum E { XX };\n"
                             "struct s {\n"
                             "  enum Bar {\n"
                             "    XX,\n"
                             "    Other\n"
                             "  };\n"
                             "  enum { XX };\n"
                             "};", /*simplify=*/ true);
        ASSERT_EQUALS("", errout.str());
    }

    void simplifyKnownVariables62() { // #5666
        ASSERT_EQUALS("void foo ( std :: string str ) {\n"
                      "char * p ; p = & str [ 0 ] ;\n"
                      "* p = 0 ;\n"
                      "}",
                      tokenizeAndStringify("void foo(std::string str) {\n"
                                           "  char *p = &str[0];\n"
                                           "  *p = 0;\n"
                                           "}", /*simplify=*/ true));
    }

    void simplifyKnownVariables63() { // #10798
        tokenizeAndStringify("typedef void (*a)();\n"
                             "enum class E { a };\n");
        ASSERT_EQUALS("", errout.str()); // don't throw
    }

    void simplifyKnownVariablesBailOutAssign1() {
        const char code[] = "int foo() {\n"
                            "    int i; i = 0;\n"
                            "    if (x) { i = 10; }\n"
                            "    return i;\n"
                            "}\n";
        const char expected[] = "int foo ( ) {\n"
                                "int i ; i = 0 ;\n"
                                "if ( x ) { i = 10 ; }\n"
                                "return i ;\n"
                                "}";
        ASSERT_EQUALS(expected, tokenizeAndStringify(code, true));
    }

    void simplifyKnownVariablesBailOutAssign2() {
        // ticket #3032 - assignment in condition
        const char code[] = "void f(struct ABC *list) {\n"
                            "    struct ABC *last = NULL;\n"
                            "    nr = (last = list->prev)->nr;\n"  // <- don't replace "last" with 0
                            "}\n";
        const char expected[] = "void f ( struct ABC * list ) {\n"
                                "struct ABC * last ; last = NULL ;\n"
                                "nr = ( last = list . prev ) . nr ;\n"
                                "}";
        ASSERT_EQUALS(expected, tokenizeAndStringify(code, true));
    }

    void simplifyKnownVariablesBailOutFor1() {
        const char code[] = "void foo() {\n"
                            "    for (int i = 0; i < 10; ++i) { }\n"
                            "}\n";
        const char expected[] = "void foo ( ) {\n"
                                "for ( int i = 0 ; i < 10 ; ++ i ) { }\n"
                                "}";
        ASSERT_EQUALS(expected, tokenizeAndStringify(code, true));
        ASSERT_EQUALS("", errout.str());    // debug warnings
    }

    void simplifyKnownVariablesBailOutFor2() {
        const char code[] = "void foo() {\n"
                            "    int i = 0;\n"
                            "    while (i < 10) { ++i; }\n"
                            "}\n";
        const char expected[] = "void foo ( ) {\n"
                                "int i ; i = 0 ;\n"
                                "while ( i < 10 ) { ++ i ; }\n"
                                "}";
        ASSERT_EQUALS(expected, tokenizeAndStringify(code, true));
        ASSERT_EQUALS("", errout.str());    // debug warnings
    }

    void simplifyKnownVariablesBailOutFor3() {
        const char code[] = "void foo() {\n"
                            "    for (std::string::size_type pos = 0; pos < 10; ++pos)\n"
                            "    { }\n"
                            "}\n";
        const char expected[] = "void foo ( ) {\n"
                                "for ( std :: string :: size_type pos = 0 ; pos < 10 ; ++ pos )\n"
                                "{ }\n"
                                "}";
        ASSERT_EQUALS(expected, tokenizeAndStringify(code, true));
        ASSERT_EQUALS("", errout.str());    // debug warnings
    }

    void simplifyKnownVariablesBailOutMemberFunction() {
        const char code[] = "void foo(obj a) {\n"
                            "    obj b = a;\n"
                            "    b.f();\n"
                            "}\n";
        const char expected[] = "void foo ( obj a ) {\n"
                                "obj b ; b = a ;\n"
                                "b . f ( ) ;\n"
                                "}";
        ASSERT_EQUALS(expected, tokenizeAndStringify(code, true));
    }

    void simplifyKnownVariablesBailOutConditionalIncrement() {
        const char code[] = "int f() {\n"
                            "    int a = 0;\n"
                            "    if (x) {\n"
                            "        ++a;\n" // conditional increment
                            "    }\n"
                            "    return a;\n"
                            "}\n";
        tokenizeAndStringify(code,true);
        ASSERT_EQUALS("", errout.str());     // no debug warnings
    }

    void simplifyKnownVariablesBailOutSwitchBreak() {
        // Ticket #2324
        const char code[] = "int f(char *x) {\n"
                            "    char *p;\n"
                            "    char *q;\n"
                            "\n"
                            "    switch (x & 0x3)\n"
                            "    {\n"
                            "        case 1:\n"
                            "            p = x;\n"
                            "            x = p;\n"
                            "            break;\n"
                            "        case 2:\n"
                            "            q = x;\n" // x is not equal with p
                            "            x = q;\n"
                            "            break;\n"
                            "    }\n"
                            "}\n";

        const char expected[] = "int f ( char * x ) {\n"
                                "char * p ;\n"
                                "char * q ;\n"
                                "\n"
                                "switch ( x & 0x3 )\n"
                                "{\n"
                                "case 1 : ;\n"
                                "p = x ;\n"
                                "x = p ;\n"
                                "break ;\n"
                                "case 2 : ;\n"
                                "q = x ;\n"
                                "x = q ;\n"
                                "break ;\n"
                                "}\n"
                                "}";

        ASSERT_EQUALS(expected, tokenizeAndStringify(code,true));
    }

    void simplifyKnownVariablesFunctionCalls() {
        {
            const char code[] = "void a(int &x);" // <- x is passed by reference
                                "void b() {"
                                "    int x = 123;"
                                "    a(x);"       // <- don't replace with a(123);
                                "}";
            const char expected[] = "void a ( int & x ) ; void b ( ) { int x ; x = 123 ; a ( x ) ; }";
            ASSERT_EQUALS(expected, tokenizeAndStringify(code,true));
        }
    }

    void simplifyKnownVariablesGlobalVars() {
        // #8054
        const char code[] = "static int x;"
                            "void f() {"
                            "    x = 123;"
                            "    while (!x) { dostuff(); }"
                            "}";
        ASSERT_EQUALS("static int x ; void f ( ) { x = 123 ; while ( ! x ) { dostuff ( ) ; } }", tokenizeAndStringify(code,true));
    }

    void simplifyKnownVariablesNamespace() {
        { // #10059
            const char code[] = "namespace N {\n"
                                "    const int n = 0;\n"
                                "    namespace M { const int m = 0; }\n"
                                "}\n"
                                "using namespace N;\n"
                                "int i(n);\n"
                                "int j(M::m);\n"
                                "using namespace N::M;\n"
                                "int k(m);\n"
                                "int l(N::M::m);\n";
            const char exp[]  = "\n\n##file 0\n"
                                "1: namespace N {\n"
                                "2: const int n@1 = 0 ;\n"
                                "3: namespace M { const int m@2 = 0 ; }\n"
                                "4: }\n"
                                "5: using namespace N ;\n"
                                "6: int i ; i = n@1 ;\n"
                                "7: int j ( M :: m@2 ) ;\n"
                                "8: using namespace N :: M ;\n"
                                "9: int k ; k = m@2 ;\n"
                                "10: int l ( N :: M :: m@2 ) ;\n";
            ASSERT_EQUALS(exp, tokenizeDebugListing(code));
        }
        { // #10835
            const char code[] = "using namespace X;\n"
                                "namespace N {\n"
                                "    struct A {\n"
                                "        static int i;\n"
                                "        struct B {\n"
                                "            double x;\n"
                                "            void f();\n"
                                "        };\n"
                                "    };\n"
                                "}\n"
                                "namespace N {\n"
                                "    int A::i = 0;\n"
                                "    void A::B::f() {\n"
                                "        x = 0;\n"
                                "    }\n"
                                "}\n";
            const char exp[]  = "\n\n##file 0\n"
                                "1: using namespace X ;\n"
                                "2: namespace N {\n"
                                "3: struct A {\n"
                                "4: static int i@1 ;\n"
                                "5: struct B {\n"
                                "6: double x@2 ;\n"
                                "7: void f ( ) ;\n"
                                "8: } ;\n"
                                "9: } ;\n"
                                "10: }\n"
                                "11: namespace N {\n"
                                "12: int A :: i@1 = 0 ;\n"
                                "13: void A :: B :: f ( ) {\n"
                                "14: x@2 = 0 ;\n"
                                "15: }\n"
                                "16: }\n";
            ASSERT_EQUALS(exp, tokenizeDebugListing(code));
        }
    }

    void simplifyKnownVariablesClassMember() {
        // Ticket #2815
        {
            const char code[] = "char *a;\n"
                                "void f(const char *s) {\n"
                                "    a = NULL;\n"
                                "    x();\n"
                                "    memcpy(a, s, 10);\n"   // <- don't simplify "a" here
                                "}\n";

            const std::string s(tokenizeAndStringify(code, true));
            ASSERT_EQUALS(true, s.find("memcpy ( a , s , 10 ) ;") != std::string::npos);
        }

        // If the variable is local then perform simplification..
        {
            const char code[] = "void f(const char *s) {\n"
                                "    char *a = NULL;\n"
                                "    x();\n"
                                "    memcpy(a, s, 10);\n"   // <- simplify "a"
                                "}\n";

            const std::string s(tokenizeAndStringify(code, true));
            TODO_ASSERT_EQUALS(true, false, s.find("memcpy ( 0 , s , 10 ) ;") != std::string::npos);
        }
    }

    void simplify_constants6() { // Ticket #5625
        {
            const char code[] = "template < class T > struct foo ;\n"
                                "void bar ( ) {\n"
                                "foo < 1 ? 0 ? 1 : 6 : 2 > x ;\n"
                                "foo < 1 ? 0 : 2 > y ;\n"
                                "}";
            const char exp[] = "template < class T > struct foo ; "
                               "void bar ( ) { "
                               "foo < 6 > x ; "
                               "foo < 0 > y ; "
                               "}";
            ASSERT_EQUALS(exp, tok(code));
        }
        {
            const char code[] = "bool b = true ? false : 1 > 2 ;";
            const char exp[] = "bool b ; b = false ;";
            ASSERT_EQUALS(exp, tok(code));
        }
    }

    void simplifyVarDeclInitLists()
    {
        const char code[] = "std::vector<int> v{a * b, 1};";
        const char exp[] = "std :: vector < int > v { a * b , 1 } ;";
        ASSERT_EQUALS(exp, tok(code));
    }
};

REGISTER_TEST(TestSimplifyTokens)
