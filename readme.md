# Flags-[V2]

>
>___name: shlChen___

>___E-Mail: 3079625093@qq.com___

[TOC]

## 1. Overview

this is a simple 'program-command-line-parameter-parsing' library with cpp-17.

## 2. Usage

test code

```cpp
#include "flags-v2.hpp"

int main(int argc, char const *argv[]) {
  try {
    FLAGS_DEF_INT(age, age, "the age of the student", ns_flags_v2::OptionProp::OPTIONAL, 18);
    FLAGS_ASSERT_INT(age, "age must be greater than 0", [](int age) {
      return age > 0;
    });

    FLAGS_DEF_INT_VEC(odds, odds, "the odd number(s)", ns_flags_v2::OptionProp::OPTIONAL, 1, 3);
    FLAGS_ASSERT_INT_VEC(odds, "not all numbers entered are odd", [](const std::vector<int> &vec) {
      for (const auto &elem : vec) {
        if (elem % 2 == 0) {
          return false;
        }
      }
      return true;
    });

    FLAGS_DEF_FLOAT(height, height, "the height", ns_flags_v2::OptionProp::OPTIONAL, 174.5f);
    FLAGS_ASSERT_FLOAT(height, "the value of height must be positive", [](float val) {
      return val > 0.0f;
    });
      
    // ...

    FLAGS_DEF_STRING(name, name, "the name", ns_flags_v2::OptionProp::REQUIRED);
    FLAGS_ASSERT_STRING(name, "the name string cannot be an empty string", [](const std::string &str) {
      return !str.empty();
    });

    // ...

    FLAGS_DEF_NO_OPTION(STRING, note, "a note", ns_flags_v2::OptionProp::OPTIONAL, "hello, world");

    // std::cout << ns_flags_v2::ns_priv::parser << std::endl;

    ns_flags_v2::setupFlags(argc, argv);

    // std::cout << ns_flags_v2::ns_priv::parser << std::endl;

  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }

  return 0;
}
```

before setup:

```cpp
{'optionName': "likes", 'variableName': "flags_likes", 'varDefVal': [eat, sleep], 'varVal': [eat, sleep], 'description': "the likes", 'property': Optional, 'ArgType': STRING_VEC}
{'optionName': "name", 'variableName': "flags_name", 'varDefVal': "", 'varVal': "", 'description': "the name", 'property': Required, 'ArgType': STRING}
{'optionName': "bvs", 'variableName': "flags_boolVals", 'varDefVal': [1, 0], 'varVal': [1, 0], 'description': "the bool-type parameter(s)", 'property': Required, 'ArgType': BOOL_VEC}
{'optionName': "sex", 'variableName': "flags_sex", 'varDefVal': 1, 'varVal': 1, 'description': "the sex(male[1], female[0])", 'property': Required, 'ArgType': BOOL}
{'optionName': "weight", 'variableName': "flags_weight", 'varDefVal': 60, 'varVal': 60, 'description': "the weight", 'property': Optional, 'ArgType': DOUBLE}
{'optionName': "fvs", 'variableName': "flags_floatVals", 'varDefVal': [3.14, 2.71, 1.414], 'varVal': [3.14, 2.71, 1.414], 'description': "the float-type parameter(s)", 'property': Optional, 'ArgType': FLOAT_VEC}
{'optionName': "height", 'variableName': "flags_height", 'varDefVal': 174.5, 'varVal': 174.5, 'description': "the height", 'property': Optional, 'ArgType': FLOAT}
{'optionName': "odds", 'variableName': "flags_odds", 'varDefVal': [1, 3], 'varVal': [1, 3], 'description': "the odd number(s)", 'property': Optional, 'ArgType': INT_VEC}
{'optionName': "version", 'variableName': "null", 'value': "", 'description': "display the version of this program", 'property': Optional, 'ArgType': VERSION}
{'optionName': "__NOPT__", 'variableName': "flags_note", 'varDefVal': "hello, world", 'varVal': "hello, world", 'description': "a note", 'property': Optional, 'ArgType': STRING}
{'optionName': "dvs", 'variableName': "flags_doubleVals", 'varDefVal': [1.1, 2.2], 'varVal': [1.1, 2.2], 'description': "the double-type parameter(s)", 'property': Optional, 'ArgType': DOUBLE_VEC}
{'optionName': "age", 'variableName': "flags_age", 'varDefVal': 18, 'varVal': 18, 'description': "the age of the student", 'property': Optional, 'ArgType': INT}
{'optionName': "help", 'variableName': "null", 'value': "...", 'description': "display the help docs", 'property': Optional, 'ArgType': HELP}
```

run this command below to set up parser:

```sh
./flags-v2 "That's all!" --likes running swimming --name shlChen --bvs 0 1 ON off OfF oN --sex 1 --weight 59.9 --fvs 1.0 2.0 3.0 --height 173.3 --odds 5 7 9 --dvs 2.12 2.13 --age 14
```

will output:

```cpp
{'optionName': "likes", 'variableName': "flags_likes", 'varDefVal': [eat, sleep], 'varVal': [running, swimming], 'description': "the likes", 'property': Optional, 'ArgType': STRING_VEC}
{'optionName': "name", 'variableName': "flags_name", 'varDefVal': "", 'varVal': "shlChen", 'description': "the name", 'property': Required, 'ArgType': STRING}
{'optionName': "bvs", 'variableName': "flags_boolVals", 'varDefVal': [1, 0], 'varVal': [0, 1, 1, 0, 0, 1], 'description': "the bool-type parameter(s)", 'property': Required, 'ArgType': BOOL_VEC}
{'optionName': "sex", 'variableName': "flags_sex", 'varDefVal': 1, 'varVal': 1, 'description': "the sex(male[1], female[0])", 'property': Required, 'ArgType': BOOL}
{'optionName': "weight", 'variableName': "flags_weight", 'varDefVal': 60, 'varVal': 59.9, 'description': "the weight", 'property': Optional, 'ArgType': DOUBLE}
{'optionName': "fvs", 'variableName': "flags_floatVals", 'varDefVal': [3.14, 2.71, 1.414], 'varVal': [1, 2, 3], 'description': "the float-type parameter(s)", 'property': Optional, 'ArgType': FLOAT_VEC}
{'optionName': "height", 'variableName': "flags_height", 'varDefVal': 174.5, 'varVal': 173.3, 'description': "the height", 'property': Optional, 'ArgType': FLOAT}
{'optionName': "odds", 'variableName': "flags_odds", 'varDefVal': [1, 3], 'varVal': [5, 7, 9], 'description': "the odd number(s)", 'property': Optional, 'ArgType': INT_VEC}
{'optionName': "version", 'variableName': "null", 'value': "1.0.0", 'description': "display the version of this program", 'property': Optional, 'ArgType': VERSION}
{'optionName': "__NOPT__", 'variableName': "flags_note", 'varDefVal': "hello, world", 'varVal': "That's all!", 'description': "a note", 'property': Optional, 'ArgType': STRING}
{'optionName': "dvs", 'variableName': "flags_doubleVals", 'varDefVal': [1.1, 2.2], 'varVal': [2.12, 2.13], 'description': "the double-type parameter(s)", 'property': Optional, 'ArgType': DOUBLE_VEC}
{'optionName': "age", 'variableName': "flags_age", 'varDefVal': 18, 'varVal': 14, 'description': "the age of the student", 'property': Optional, 'ArgType': INT}
{'optionName': "help", 'variableName': "null", 'value': "...", 'description': "display the help docs", 'property': Optional, 'ArgType': HELP}
```

help option:

```sh
./flags-v2 --help
```

```cpp
Usage: ./flags-v2 [no-opt argv(s)] [--optionName argv(s)] ...

    Options        Property       Type           Describes
--------------------------------------------------------------
  --likes          Optional       STRING_VEC     the likes
  --name           Required       STRING         the name
  --bvs            Required       BOOL_VEC       the bool-type parameter(s)
  --sex            Required       BOOL           the sex(male[1], female[0])
  --weight         Optional       DOUBLE         the weight
  --fvs            Optional       FLOAT_VEC      the float-type parameter(s)
  --height         Optional       FLOAT          the height
  --odds           Optional       INT_VEC        the odd number(s)
  --dvs            Optional       DOUBLE_VEC     the double-type parameter(s)
  --age            Optional       INT            the age of the student

  --help           Optional       HELP           display the help docs
  --version        Optional       VERSION        display the version of this program

help docs for program "./flags-v2"
```

version option:

```sh
./flags-v2 --version
```

```cpp
./flags-v2: ['version': '1.0.0']
```

run the command:

```sh
./flags-v2 "That's all!" --likes running swimming --name shlChen --bvs 0 1 ON off OfF oN --sex 1 --weight 59.9 --fvs 1.0 2.0 3.0 --height 173.3 --odds 5 7 9 --dvs 2.12 2.13 --age 14
```

will output:

```c++
[ error from 'lib-flags':'FLAGS_SET_ASSERT' ] the value for option '--age' is invalid: "age must be greater than 0". (use option '--help' to get more info)
```

## 3. Details

```cpp
static void setVersion(const std::string &version);
```

```cpp
static void setHelpDocs(const std::string &help);
```

```cpp
static void setupFlags(int argc, char const *argv[]);
```



**Macros for adding options for different parameter types:**

```cpp
#define FLAGS_DEF_INT(variableName, optionName, description, property, ...)
```

```cpp
#define FLAGS_DEF_INT_VEC(variableName, optionName, description, property, ...)
```

```cpp
#define FLAGS_DEF_FLOAT(variableName, optionName, description, property, ...)
```

```cpp
#define FLAGS_DEF_FLOAT_VEC(variableName, optionName, description, property, ...)
```

```cpp
#define FLAGS_DEF_DOUBLE(variableName, optionName, description, property, ...)
```

```cpp
#define FLAGS_DEF_DOUBLE_VEC(variableName, optionName, description, property, ...)
```

```cpp
#define FLAGS_DEF_BOOL(variableName, optionName, description, property, ...)
```

```cpp
#define FLAGS_DEF_BOOL_VEC(variableName, optionName, description, property, ...)
```

```cpp
#define FLAGS_DEF_STRING(variableName, optionName, description, property, ...)
```

```cpp
#define FLAGS_DEF_STRING_VEC(variableName, optionName, description, property, ...)
```



**Define macros that do not require option**:

```cpp
#define FLAGS_DEF_NO_OPTION(type, variableName, description, property, ...)
```



**Define assertion function macros for different type:**

```cpp
#define FLAGS_ASSERT_INT(optionName, invaildMsg, fun)
```

```cpp
#define FLAGS_ASSERT_INT_VEC(optionName, invaildMsg, fun)
```

```cpp
#define FLAGS_ASSERT_FLOAT(optionName, invaildMsg, fun)
```

```cpp
#define FLAGS_ASSERT_FLOAT_VEC(optionName, invaildMsg, fun)
```

```cpp
#define FLAGS_ASSERT_DOUBLE(optionName, invaildMsg, fun)
```

```cpp
#define FLAGS_ASSERT_DOUBLE_VEC(optionName, invaildMsg, fun)
```

```cpp
#define FLAGS_ASSERT_BOOL(optionName, invaildMsg, fun)
```

```cpp
#define FLAGS_ASSERT_BOOL_VEC(optionName, invaildMsg, fun)
```

```cpp
#define FLAGS_ASSERT_STRING(optionName, invaildMsg, fun)
```

```cpp
#define FLAGS_ASSERT_STRING_VEC(optionName, invaildMsg, fun)
```

