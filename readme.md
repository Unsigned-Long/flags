# Flags-[V3]

>
>___name: shlchen | ULong2___

>___E-Mail: shlchen@whu.edu.cn___

[TOC]

## 1. Overview

this is a simple 'program-command-line-parameter-parsing' library with cpp-17.

## 2. Usage

test code

```cpp
#include "flags-v2.hpp"

int main(int argc, char const *argv[]) {
    try {
        using namespace ns_flags;

        // ...
        const auto &odds = parser.AddOption<IntVec>(
                "odds", {1, 3}, "the odd number(s)", OptionProp::OPTIONAL,
                [](const IntVec::data_type &vec) -> std::optional<std::string> {
                    for (const auto &item: vec) {
                        if (item % 2 == 0) {
                            return "not all numbers entered are odd";
                        }
                    }
                    return {};
                }
        );
        // ...
        const auto &name = parser.AddOption<String>(
                "name", "null", "the name string", OptionProp::REQUIRED,
                [](const String::data_type &str) -> std::optional<std::string> {
                    if (str.empty()) {
                        return "the name string cannot be an empty string";
                    }
                    return {};
                }
        );
        const auto &note = parser.AddDefaultOption<String>(
                "hello, world!", "a note", OptionProp::OPTIONAL
        );
        parser.SetupFlags(argc, argv);
        LOG_VAR(age, odds, height, note)
        LOG_VAR(parser)
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
```

before setup:

```cpp
{'optionName': "def-opt", 'defaultValue': hello, world!, 'value': hello, world!, 'desc': "a note", 'prop': Optional, 'argType': String, 'hasAssertor': false}
{'optionName': "name", 'defaultValue': null, 'value': null, 'desc': "the name string", 'prop': Required, 'argType': String, 'hasAssertor': true}
{'optionName': "height", 'defaultValue': 174.5, 'value': 174.5, 'desc': "the height", 'prop': Optional, 'argType': Float, 'hasAssertor': true}
{'optionName': "odds", 'defaultValue': [1, 3], 'value': [1, 3], 'desc': "the odd number(s)", 'prop': Optional, 'argType': IntVec, 'hasAssertor': true}
{'optionName': "age", 'defaultValue': 18, 'value': 18, 'desc': "the age of the student", 'prop': Optional, 'argType': Int, 'hasAssertor': true}
{'optionName': "help", 'defaultValue': "...", 'value': "...", 'desc': "display the help docs", 'prop': Optional, 'argType': Help, 'hasAssertor': false}
{'optionName': "version", 'defaultValue': "...", 'value': "...", 'desc': "display the version of this program", 'prop': Optional, 'argType': Version, 'hasAssertor': false}
```

run this command below to set up parser:

```sh
 ./flags-v3 "I'm ULong2" --name ULong2 --age 22 --height 174.0f --odds 9 11 97
```

will output:

```cpp
{'optionName': "def-opt", 'defaultValue': hello, world!, 'value': I'm ULong2, 'desc': "a note", 'prop': Optional, 'argType': String, 'hasAssertor': false}
{'optionName': "name", 'defaultValue': null, 'value': ULong2, 'desc': "the name string", 'prop': Required, 'argType': String, 'hasAssertor': true}
{'optionName': "height", 'defaultValue': 174.5, 'value': 174, 'desc': "the height", 'prop': Optional, 'argType': Float, 'hasAssertor': true}
{'optionName': "odds", 'defaultValue': [1, 3], 'value': [9, 11, 97], 'desc': "the odd number(s)", 'prop': Optional, 'argType': IntVec, 'hasAssertor': true}
{'optionName': "age", 'defaultValue': 18, 'value': 22, 'desc': "the age of the student", 'prop': Optional, 'argType': Int, 'hasAssertor': true}
{'optionName': "help", 'defaultValue': "...", 'value': "...", 'desc': "display the help docs", 'prop': Optional, 'argType': Help, 'hasAssertor': false}
{'optionName': "version", 'defaultValue': "...", 'value': "...", 'desc': "display the version of this program", 'prop': Optional, 'argType': Version, 'hasAssertor': false}
```

help option:

```sh
./flags-v3 --help
```

```cpp
Usage: ./flags-v3 [def-opt argv(s)] [--optName argv(s)] ...

    Options        Property       Type           Describes
--------------------------------------------------------------
  --def-opt        Optional       String         a note

  --name           Required       String         the name string
  --height         Optional       Float          the height
  --odds           Optional       IntVec         the odd number(s)
  --age            Optional       Int            the age of the student

  --help           Optional       Help           display the help docs
  --version        Optional       Version        display the version of this program

help docs for program "./flags-v3"
```

version option:

```sh
./flags-v3 --version
```

```cpp
./flags-v3: ['version': '1.0.0']
```

run the command:

```sh
./flags-v3 --name ""
```

will output:

```c++
[ error from 'lib-flags':'AssertOptionValue' ] the value(s) for option '--name' is(are) invalid: "the name string cannot be an empty string". (use option '--help' to get more info)
```

## 3. Details

```cpp
static void SetVersion(const std::string &version);
```

```cpp
static void SetHelp(const std::string &help);
```

```cpp
static void SetupFlags(int argc, char const *argv[]);
```

```cpp
/**
 * @tparam ArgumentType the type of the option's argument
 *
 * @param optionName the option name
 * @param defaultValue the default value of the option argument
 * @param description the description of this option
 * @param property the property of this option
 * @param assertor the assertor to judge whether the entered args are valid
 * @return the value address of the option argument
 *
 * @attention user should use reference to receive the return value
 */
template<class ArgumentType>
const typename ArgumentType::data_type &
AddOption(const std::string &optionName, const typename ArgumentType::data_type &defaultValue,
const std::string &description, const OptionProp &property,
assertor_type<ArgumentType> assertor = nullptr);
```

```cpp
/**
 * @tparam ArgumentType the type of the option's argument
 *
 * @param defaultValue the default value of the default option argument
 * @param description the description of this default option
 * @param property the property of this default option
 * @param assertor the assertor to judge whether the entered args are valid
 * @return the value address of the default option argument
 *
 * @attention user should use reference to receive the return value
 */
template<class ArgumentType>
const typename ArgumentType::data_type &
AddDefaultOption(const typename ArgumentType::data_type &defaultValue,
const std::string &description, const OptionProp &property,
assertor_type<ArgumentType> assertor = nullptr)
```

## 4. Self-define Option Example
firstly, you should write the code below:
```c++
struct Person {
    std::string name;
    float age;
    bool sex;

    friend std::ostream &operator<<(std::ostream &os, const Person &person) {
        os << std::boolalpha;
        os << "name: " << person.name << " age: " << person.age << " sex: " << person.sex;
        return os;
    }
};

FLAGS_ARGUMENT_TEMPLATE_GENERATOR_BEGIN(PersonOption, Person)

    std::optional<std::string> DataFromStringVector(const std::vector<std::string> &strVec) override {
        if (strVec.size() < 3) {
            return "3 elems are needed to create a person";
        }
        data.name = strVec.at(0);
        data.age = std::stof(strVec.at(1));
        data.sex = ns_flags::Utils::StrToBool(strVec.at(2));
        return {};
    }
FLAGS_ARGUMENT_TEMPLATE_GENERATOR_END
```
than:
```c++
int main(int argc, char const *argv[]) {
    try {
        using namespace ns_flags;
        const auto &p = parser.AddOption<PersonOption>("p", {}, "the person", OptionProp::REQUIRED);
        parser.SetupFlags(argc, argv);
        LOG_VAR(p)
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
```