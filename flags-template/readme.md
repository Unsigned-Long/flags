># Flags-Template
>
>___name: csl___

>___E-Mail: 3079625093@qq.com___

```cpp
    _|_|  _|                                    
  _|      _|    _|_|_|    _|_|_|    _|_|_|      
_|_|_|_|  _|  _|    _|  _|    _|  _|_|          
  _|      _|  _|    _|  _|    _|      _|_|      
  _|      _|    _|_|_|    _|_|_|  _|_|_|    _|  
                              _|                
                          _|_|                  
                                                                      
  _|                                          _|              _|      
_|_|_|_|    _|_|    _|_|_|  _|_|    _|_|_|    _|    _|_|_|  _|_|_|_|    _|_|    
  _|      _|_|_|_|  _|    _|    _|  _|    _|  _|  _|    _|    _|      _|_|_|_|  
  _|      _|        _|    _|    _|  _|    _|  _|  _|    _|    _|      _|        
    _|_|    _|_|_|  _|    _|    _|  _|_|_|    _|    _|_|_|      _|_|    _|_|_| 
                                    _|                                
                                    _|                                
```

## OverView

this is a simple 'program-command-line-parameter-parsing' library using cpp-template.

the main functions:

+ Add command line parameters to the specified program and set the relevant properties of the command line parameters;
+ Parse the passed in parameters based on the set command line parameters;
+ During parsing, identify and check the command line parameters (such as wrong type, wrong option name, inconsistent selectability);

## Usage

### Example for Source Code

```cpp
#include "flags.hpp"

using namespace ns_flags;

int main(int argc, char const* argv[]) {
  /**
   * @brief try-catch is not necessary but it is strongly recommended,
   * because you can get a lot of advice when there are errors in your code
   */
  try {
    ns_flags::ArgParser parser;
    /**
     * @brief define some kinds of arguements
     * [int, std::string, bool, double]
     * std::vector<[int, std::string, bool, double]>
     */
    parser.add_opt<ArgType::INT>("id", 0, "the id of current thread");
    parser.add_opt<ArgType::STRING>("usr", "null", "the name of usr");
    parser.add_opt<ArgType::BOOL>("sex", true,
                                  "the sex of usr [male: true, female: false]");
    parser.add_opt<ArgType::DOUBLE>("height", 1.7, "the height of usr",
                                    OptProp::REQUIRED);
    parser.add_opt<ArgType::INT_VEC>("ids", {1, 2, 3}, "the ids of threads");
    parser.add_opt<ArgType::STRING_VEC>("lans", {"cpp", "python"},
                                        "the used langusges of usr");
    parser.add_opt<ArgType::BOOL_VEC>("choice", {true, false},
                                      "the choice of usr");
    parser.add_opt<ArgType::DOUBLE_VEC>("scores", {2.3, 4.5},
                                        "the score of usr");
    /**
     * @brief set version and help docs
     * @attention if you do not set the help docs, then the help docs
     * will generate automatically
     */
    parser.set_version("2.0");
    // parser.set_help("");

    parser.set_nopt<ArgType::STRING_VEC>({""}, OptProp::REQUIRED);
    /**
     * @brief finally, you can set up the parser and then use these arguements
     */
    parser.setup_parser(argc, argv);

    /**
     * @brief print the info of arguements
     */
    std::cout << parser.get_nopti() << std::endl;
    for (const auto& [key, value] : parser.get_opts())
      std::cout << value << std::endl;

    /**
     * @brief use the arguements
     */
    auto id = parser.get_argv<ArgType::INT>("id");
    std::cout << "the 'id' I get is: " << id << std::endl;
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
  return 0;
}
```

### Output

_if you want to over view the example log file for command lines and outputs, please click [the log file](./output/log.log)._

if run command line:

```shell
./flags hello "I'm" flags!
```

will output:

```cpp
[ error from 'ArgParser::setup_parser' ] the property of the option named '--height' is 'OptProp::required', but you didn't pass the arguement(s)
```



if run command line:

```shell
/flags hello "I'm" flags!  --height 98.8 --sex true --usr csl --id 12 --choice true false true --ids 12 34 123 --scores 12.3 45.6 78.9 --lans cpp java python html
```

will output:

```cpp
{'name': no-opt, 'prop': required, 'argv': [hello, I'm, flags!], 'defult': [], 'desc': pass arguement(s) without any option}
{'name': choice, 'prop': optional, 'argv': [true, false, true], 'defult': [true, false], 'desc': the choice of usr}
{'name': ids, 'prop': optional, 'argv': [12, 34, 123], 'defult': [1, 2, 3], 'desc': the ids of threads}
{'name': scores, 'prop': optional, 'argv': [12.3, 45.6, 78.9], 'defult': [2.3, 4.5], 'desc': the score of usr}
{'name': lans, 'prop': optional, 'argv': [cpp, java, python, html], 'defult': [cpp, python], 'desc': the used langusges of usr}
{'name': height, 'prop': required, 'argv': 98.800000, 'defult': 1.700000, 'desc': the height of usr}
{'name': sex, 'prop': optional, 'argv': true, 'defult': true, 'desc': the sex of usr [male: true, female: false]}
{'name': usr, 'prop': optional, 'argv': csl, 'defult': null, 'desc': the name of usr}
{'name': id, 'prop': optional, 'argv': 12, 'defult': 0, 'desc': the id of current thread}
the 'id' I get is: 12
```

if run command line:

```shell
./flags --help
```

will output:

```cpp
Usage: ./flags [no-opt] [--option target(s)] ...

    Options        property       Default Value       Describes
-------------------------------------------------------------------
  --no-opt         required       []                  pass arguement(s) without any option

  --choice         optional       [true, false]       the choice of usr
  --ids            optional       [1, 2, 3]           the ids of threads
  --scores         optional       [2.3, 4.5]          the score of usr
  --lans           optional       [cpp, python]       the used langusges of usr
  --height         required       1.700000            the height of usr
  --sex            optional       true                the sex of usr [male: true, female: false]
  --usr            optional       null                the name of usr
  --id             optional       0                   the id of current thread

  --help           optional       help docs           get the help docs of this program
  --version        optional       0.0.1               get the version of this program

help docs for program "./flags"
```

if run command line:

```shell
./flags --version
```

will output:

```cpp
./flags version: 2.0
```

if run command line:

```shell
./flags --nema 12
```

will output:

```cpp
some error(s) happened in the command line:
[ error from 'ArgParser::setup_parser' ] the option named '--nema' is invalid, use '--help' option for help
```

## Apis

### Arguement Types

Here are the types you can use in the 'arguement-parser':

```cpp
  using INT = int;
  using DOUBLE = double;
  using BOOL = bool;
  using STRING = std::string;
  using INT_VEC = std::vector<int>;
  using DOUBLE_VEC = std::vector<double>;
  using BOOL_VEC = std::vector<bool>;
  using STRING_VEC = std::vector<std::string>;
```

### Option Property

```cpp
enum class OptProp {
  /**
   * @brief options
   */
  OPTIONAL,
  REQUIRED
};
```

### Arguement Info

These members are config objects in an 'arguement-info' object:

```cpp
  std::string _name;

  OptProp _prop;

  std::any _value;
  std::any _defult_value;

  std::string _desc;
```

### Apis in the ArgParser

#### constructor

+ ___ArgParser()___

```cpp
  /**
   * @brief the default and only constructor for ArgParser
   */
```

#### main methods

+ ___template <typename Type> void add_opt(const std::string &name, const Type &defult_value, const std::string &desc, OptProp prop = OptProp::OPTIONAL)___

```cpp
  /**
   * @brief add an option to the parser
   *
   * @tparam Type the type of the option's arguemrnt(s)
   * @param name the name of the option
   * @param defult_value the default value of the option's arguement(s)
   * @param desc the describe of the option
   * @param prop the property of this option
   */
```

+ ___void setup_parser(int argc, char const *argv[])___

```cpp
  /**
   * @brief Set the up the parser
   *
   * @param argc the count of the arguements
   * @param argv the values of the arguements
   */
```

#### methods for 'no-opt'

+ ___template <typename Type> inline void set_nopt___

```cpp
  /**
   * @brief Set the 'no-opt'
   *
   * @tparam Type the type of 'no-opt'
   * @param default_value the default value of the 'no-opt'
   * @param prop the property of 'no-opt'
   * @param desc the describe of 'no-opt'
   */
```

+ ___template <typename Type> inline const Type &get_noptv() const___

```cpp
  /**
   * @brief Get the value of 'no-opt' arguement(s)
   *
   * @tparam Type the type of 'no-opt'
   * @return const Type&
   */
```

#### 'get' methods for 'opts'

+ ___inline std::size_t get_optc() const___

```cpp
  /**
   * @brief get the count of the options in the parser
   *
   * @return std::size_t
   */
```

+ ___inline const OptInfo &get_opti(const std::string &name) const___

```cpp
  /**
   * @brief Get the option's info named 'name'
   *
   * @param name the name of the option
   * @return const OptInfo&
   */
```

+ ___inline const auto &get_opts() const___

```cpp
  /**
   * @brief Get the all options in the parser
   *
   * @return const auto&
   */
```

+ ___template <typename Type> inline const Type &get_argv(const std::string &name) const___

```cpp
  /**
   * @brief Get the value of an option according to name
   *
   * @tparam Type the type of this option's arguement(s)
   * @param name the name of this option
   * @return const Type&
   */
```

+ ___template <typename Type> inline const Type &get_argdv(const std::string &name) const___

```cpp
  /**
   * @brief Get the default value of an option according to name
   *
   * @tparam Type the type of this option's arguement(s)
   * @param name the name of this option
   * @return const Type&
   */
```

+ ___inline const std::string &get_argdc(const std::string &name) const___

```cpp
  /**
   * @brief Get the describe of the option named 'name'
   *
   * @param name the name of the option
   * @return const std::string&
   */
```

#### 'set' methods for 'help' and 'version' options

+ ___inline void set_help(const std::string &str)___

```cpp
  /**
   * @brief Set the help docs string for the parser
   *
   * @param str the help str to set
   */
```

+ ___inline void set_version(const std::string &str)___

```cpp
  /**
   * @brief Set the version of the program
   *
   * @param str the version str
   */
```
