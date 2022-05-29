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
{'optName': "likes", 'varName': "flags_likes", 'varDefVal': [eat, sleep], 'varVal': [eat, sleep], 'desc': "the likes", 'prop': Optional, 'argType': STRING_VEC}
{'optName': "name", 'varName': "flags_name", 'varDefVal': "", 'varVal': "", 'desc': "the name", 'prop': Required, 'argType': STRING}
{'optName': "bvs", 'varName': "flags_boolVals", 'varDefVal': [1, 0], 'varVal': [1, 0], 'desc': "the bool-type parameter(s)", 'prop': Required, 'argType': BOOL_VEC}
{'optName': "sex", 'varName': "flags_sex", 'varDefVal': 1, 'varVal': 1, 'desc': "the sex(male[1], female[0])", 'prop': Required, 'argType': BOOL}
{'optName': "weight", 'varName': "flags_weight", 'varDefVal': 60, 'varVal': 60, 'desc': "the weight", 'prop': Optional, 'argType': DOUBLE}
{'optName': "fvs", 'varName': "flags_floatVals", 'varDefVal': [3.14, 2.71, 1.414], 'varVal': [3.14, 2.71, 1.414], 'desc': "the float-type parameter(s)", 'prop': Optional, 'argType': FLOAT_VEC}
{'optName': "height", 'varName': "flags_height", 'varDefVal': 174.5, 'varVal': 174.5, 'desc': "the height", 'prop': Optional, 'argType': FLOAT}
{'optName': "odds", 'varName': "flags_odds", 'varDefVal': [1, 3], 'varVal': [1, 3], 'desc': "the odd number(s)", 'prop': Optional, 'argType': INT_VEC}
{'optName': "version", 'varName': "null", 'value': "", 'desc': "display the version of this program", 'prop': Optional, 'argType': VERSION}
{'optName': "__NOPT__", 'varName': "flags_note", 'varDefVal': "hello, world", 'varVal': "hello, world", 'desc': "a note", 'prop': Optional, 'argType': STRING}
{'optName': "dvs", 'varName': "flags_doubleVals", 'varDefVal': [1.1, 2.2], 'varVal': [1.1, 2.2], 'desc': "the double-type parameter(s)", 'prop': Optional, 'argType': DOUBLE_VEC}
{'optName': "age", 'varName': "flags_age", 'varDefVal': 18, 'varVal': 18, 'desc': "the age of the student", 'prop': Optional, 'argType': INT}
{'optName': "help", 'varName': "null", 'value': "...", 'desc': "display the help docs", 'prop': Optional, 'argType': HELP}
```

run this command below to set up parser:

```sh
./flags-v2 "That's all!" --likes running swimming --name shlChen --bvs 0 1 ON off OfF oN --sex 1 --weight 59.9 --fvs 1.0 2.0 3.0 --height 173.3 --odds 5 7 9 --dvs 2.12 2.13 --age 14
```

will output:

```cpp
{'optName': "likes", 'varName': "flags_likes", 'varDefVal': [eat, sleep], 'varVal': [running, swimming], 'desc': "the likes", 'prop': Optional, 'argType': STRING_VEC}
{'optName': "name", 'varName': "flags_name", 'varDefVal': "", 'varVal': "shlChen", 'desc': "the name", 'prop': Required, 'argType': STRING}
{'optName': "bvs", 'varName': "flags_boolVals", 'varDefVal': [1, 0], 'varVal': [0, 1, 1, 0, 0, 1], 'desc': "the bool-type parameter(s)", 'prop': Required, 'argType': BOOL_VEC}
{'optName': "sex", 'varName': "flags_sex", 'varDefVal': 1, 'varVal': 1, 'desc': "the sex(male[1], female[0])", 'prop': Required, 'argType': BOOL}
{'optName': "weight", 'varName': "flags_weight", 'varDefVal': 60, 'varVal': 59.9, 'desc': "the weight", 'prop': Optional, 'argType': DOUBLE}
{'optName': "fvs", 'varName': "flags_floatVals", 'varDefVal': [3.14, 2.71, 1.414], 'varVal': [1, 2, 3], 'desc': "the float-type parameter(s)", 'prop': Optional, 'argType': FLOAT_VEC}
{'optName': "height", 'varName': "flags_height", 'varDefVal': 174.5, 'varVal': 173.3, 'desc': "the height", 'prop': Optional, 'argType': FLOAT}
{'optName': "odds", 'varName': "flags_odds", 'varDefVal': [1, 3], 'varVal': [5, 7, 9], 'desc': "the odd number(s)", 'prop': Optional, 'argType': INT_VEC}
{'optName': "version", 'varName': "null", 'value': "1.0.0", 'desc': "display the version of this program", 'prop': Optional, 'argType': VERSION}
{'optName': "__NOPT__", 'varName': "flags_note", 'varDefVal': "hello, world", 'varVal': "That's all!", 'desc': "a note", 'prop': Optional, 'argType': STRING}
{'optName': "dvs", 'varName': "flags_doubleVals", 'varDefVal': [1.1, 2.2], 'varVal': [2.12, 2.13], 'desc': "the double-type parameter(s)", 'prop': Optional, 'argType': DOUBLE_VEC}
{'optName': "age", 'varName': "flags_age", 'varDefVal': 18, 'varVal': 14, 'desc': "the age of the student", 'prop': Optional, 'argType': INT}
{'optName': "help", 'varName': "null", 'value': "...", 'desc': "display the help docs", 'prop': Optional, 'argType': HELP}
```

help option:

```sh
./flags-v2 --help
```

```cpp
Usage: ./flags-v2 [no-opt argv(s)] [--optName argv(s)] ...

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
#define FLAGS_DEF_INT(varName, optName, desc, prop, ...)
```

```cpp
#define FLAGS_DEF_INT_VEC(varName, optName, desc, prop, ...)
```

```cpp
#define FLAGS_DEF_FLOAT(varName, optName, desc, prop, ...)
```

```cpp
#define FLAGS_DEF_FLOAT_VEC(varName, optName, desc, prop, ...)
```

```cpp
#define FLAGS_DEF_DOUBLE(varName, optName, desc, prop, ...)
```

```cpp
#define FLAGS_DEF_DOUBLE_VEC(varName, optName, desc, prop, ...)
```

```cpp
#define FLAGS_DEF_BOOL(varName, optName, desc, prop, ...)
```

```cpp
#define FLAGS_DEF_BOOL_VEC(varName, optName, desc, prop, ...)
```

```cpp
#define FLAGS_DEF_STRING(varName, optName, desc, prop, ...)
```

```cpp
#define FLAGS_DEF_STRING_VEC(varName, optName, desc, prop, ...)
```



**Define macros that do not require option**:

```cpp
#define FLAGS_DEF_NO_OPTION(type, varName, desc, prop, ...)
```



**Define assertion function macros for different type:**

```cpp
#define FLAGS_ASSERT_INT(optName, invaildMsg, fun)
```

```cpp
#define FLAGS_ASSERT_INT_VEC(optName, invaildMsg, fun)
```

```cpp
#define FLAGS_ASSERT_FLOAT(optName, invaildMsg, fun)
```

```cpp
#define FLAGS_ASSERT_FLOAT_VEC(optName, invaildMsg, fun)
```

```cpp
#define FLAGS_ASSERT_DOUBLE(optName, invaildMsg, fun)
```

```cpp
#define FLAGS_ASSERT_DOUBLE_VEC(optName, invaildMsg, fun)
```

```cpp
#define FLAGS_ASSERT_BOOL(optName, invaildMsg, fun)
```

```cpp
#define FLAGS_ASSERT_BOOL_VEC(optName, invaildMsg, fun)
```

```cpp
#define FLAGS_ASSERT_STRING(optName, invaildMsg, fun)
```

```cpp
#define FLAGS_ASSERT_STRING_VEC(optName, invaildMsg, fun)
```

