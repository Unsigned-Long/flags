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
