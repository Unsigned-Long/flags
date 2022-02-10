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

## Usage

### example source code

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
    parser.add_arg<ArgType::INT>("id", 0, "the id of current thread");
    parser.add_arg<ArgType::STRING>("usr", "null", "the name of usr");
    parser.add_arg<ArgType::BOOL>("sex", true,
                                  "the sex of usr [male: true, female: false]");
    parser.add_arg<ArgType::DOUBLE>("height", 1.7, "the height of usr");
    parser.add_arg<ArgType::INT_VEC>("ids", {1, 2, 3}, "the ids of threads");
    parser.add_arg<ArgType::STRING_VEC>("lans", {"cpp", "python"},
                                        "the used langusges of usr");
    parser.add_arg<ArgType::BOOL_VEC>("choice", {true, false},
                                      "the choice of usr");
    parser.add_arg<ArgType::DOUBLE_VEC>("scores", {2.3, 4.5},
                                        "the score of usr");
    /**
     * @brief set version and help docs
     * @attention if you do not set the help docs, then the help docs
     * will generate automatically
     */
    parser.set_version("2.0");
    // parser.set_help("");

    parser.set_nopt_arg<ArgType::STRING_VEC>({""});
    /**
     * @brief finally, you can set up the parser and then use these arguements
     */
    parser.setup_parser(argc, argv);

    /**
     * @brief print the info of arguements
     */
    std::cout << parser.get_nopt_argi() << std::endl;
    for (const auto& [key, value] : parser.get_args())
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

### output

_if you want to over view the example command lines and outputs, please click [the log file](./output/log.log)._

if run command line:

```shell
/flags hello "I'm" flags!  --height 98.8 --sex true --usr csl --id 12 --choice true false true --ids 12 34 123 --scores 12.3 45.6 78.9 --lans cpp java python html
```

will output:

```cpp
{'name': nopt_arg, 'value': [hello, I'm, flags!], 'defult': [], 'desc': arguement(s) without any option}
{'name': choice, 'value': [true, false, true], 'defult': [true, false], 'desc': the choice of usr}
{'name': ids, 'value': [12, 34, 123], 'defult': [1, 2, 3], 'desc': the ids of threads}
{'name': scores, 'value': [12.3, 45.6, 78.9], 'defult': [2.3, 4.5], 'desc': the score of usr}
{'name': lans, 'value': [cpp, java, python, html], 'defult': [cpp, python], 'desc': the used langusges of usr}
{'name': height, 'value': 98.800000, 'defult': 1.700000, 'desc': the height of usr}
{'name': sex, 'value': true, 'defult': true, 'desc': the sex of usr [male: true, female: false]}
{'name': usr, 'value': csl, 'defult': null, 'desc': the name of usr}
{'name': id, 'value': 12, 'defult': 0, 'desc': the id of current thread}
{'name': help, 'value': false, 'defult': false, 'desc': get help docs of this program}
{'name': version, 'value': , 'defult': 1.0, 'desc': the version of this program}
the 'id' I get is: 12
```

if run command line:

```shell
./flags --help
```

will output:

```cpp
Usage: ./flags [nopt-arg(s)] [--option target(s)] ...

    Options        Default Value       Describes
----------------------------------------------------
  --nopt-arg(s)    []                  arguement(s) without any option

  --choice         [true, false]       the choice of usr
  --ids            [1, 2, 3]           the ids of threads
  --scores         [2.3, 4.5]          the score of usr
  --lans           [cpp, python]       the used langusges of usr
  --height         1.700000            the height of usr
  --sex            true                the sex of usr [male: true, female: false]
  --usr            null                the name of usr
  --id             0                   the id of current thread
  --help           false               get help docs of this program
  --version        1.0                 the version of this program

program help docs
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
[ error from lib-flags ] the option named '--nema' is invalid, use '--help' option for help.
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

### Arguement Info

These members are config objects in an 'arguement-info' object:

```cpp
  std::string _name;
  std::any _value;
  std::any _defult_value;
  std::string _desc;
```

### Apis in the ArgParser

___ArgParser()___

```cpp
  /**
   * @brief the default and only constructor for ArgParser
   */
```

___template <typename Type> void add_arg(const std::string &name, const Type &defult_value, const std::string &desc)___

```cpp
  /**
   * @brief add a arguement to the parser
   *
   * @tparam Type the type of the arguement
   * @param name the name of the arguement
   * @param defult_value the default value of the arguement
   * @param desc the describe of the arguement
   */
```

___template <typename Type> void set_nopt_arg___

```cpp
  /**
   * @brief Set the no-option arguement
   *
   * @tparam Type the type of arguement
   * @param default_value the default value of the no-option arguement
   * @param desc
   */
```

___template <typename Type> inline const Type &get_nopt_argv() const___

```cpp
  /**
   * @brief Get the no-option arguement's value
   *
   * @tparam Type the vaule type
   * @return const Type&
   */
```

___inline const ArgInfo get_nopt_argi() const___

```cpp
  /**
   * @brief Get the no-option arguement info object
   *
   * @return const ArgInfo
   */
```

___inline std::size_t get_argc() const___

```cpp
  /**
   * @brief get the count of the arguements in the parser
   *
   * @return size_t
   */
```

___inline const ArgInfo &get_argi(const std::string &name) const___

```cpp
  /**
   * @brief Get the arg info object in the parser according to the name
   *
   * @param name the name of the arguement
   * @return const ArgInfo&
   */
```

___inline const auto &get_args() const___

```cpp
  /**
   * @brief Get the all arguements in the parser
   *
   * @return const auto&
   */
```

___template <typename Type> inline const Type &get_argv(const std::string &name) const___

```cpp
  /**
   * @brief Get the value of an arguement according to name
   *
   * @tparam Type the type of this arguement
   * @param name the name of this arguement
   * @return Type&
   */
```

___template <typename Type> inline const Type &get_argdv(const std::string &name) const___

```cpp
  /**
   * @brief Get the default value of an arguement according to name
   *
   * @tparam Type the type of this arguement
   * @param name the name of this arguement
   * @return const Type&
   */
```

___inline const std::string &get_argdc(const std::string &name) const___

```cpp
  /**
   * @brief Get the describe of the arguement named 'name' 
   *
   * @param name
   * @return const std::string&
   */
```

___void setup_parser(int argc, char const *argv[])___

```cpp
  /**
   * @brief Set the up the parser
   *
   * @param argc the count of the arguements
   * @param argv the values of the arguements
   */
```

___inline void set_help(const std::string &str)___

```cpp
  /**
   * @brief Set the help docs string for the parser
   *
   * @param str the help str to set
   */
```

___inline void set_version(const std::string &str)___

```cpp
  /**
   * @brief Set the version of the program
   *
   * @param str the version str
   */
```

