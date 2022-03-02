#include "flags.hpp"

using namespace ns_flags;

int main(int argc, char const *argv[]) {
  /**
   * @brief try-catch is not necessary but it is strongly recommended,
   * because you can get a lot of advice when there are errors in your code
   */
  try {
    OptionParser parser;
    /**
     * @brief define some kinds of arguements
     * [int, std::string, bool, double]
     * std::vector<[int, std::string, bool, double]>
     */
    parser.addOption<IntArg>("id", 10, "the id of current thread");
    parser.addOption<StringArg>("usr", "null", "the name of usr");
    parser.addOption<BoolArg>("sex", true,
                              "the sex of usr [male: true, female: false]");
    parser.addOption<DoubleArg>("height", 1.7, "the height of usr", OptionProp::REQUIRED);
    parser.addOption<IntVecArg>("ids", {1, 2, 3}, "the ids of threads");
    parser.addOption<StringVecArg>("lans", {"cpp", "python"},
                                   "the used langusges of usr");
    parser.addOption<BoolVecArg>("choice", {true, false}, "the choice of usr");
    parser.addOption<DoubleVecArg>("scores", {2.3, 4.5}, "the score of usr");
    /**
     * @brief set version and help docs
     * @attention if you do not set the help docs, then the help docs
     * will generate automatically
     */
    parser.setVersion("2.0.0");
    // parser.set_help("");

    parser.setDefaultOption<StringVecArg>({""}, "the default option", OptionProp::REQUIRED);
    /**
     * @brief finally, you can set up the parser and then use these arguements
     */
    parser.setupParser(argc, argv);

    /**
     * @brief print the info of arguements
     */
    std::cout << parser << std::endl;
    std::cout << parser.getDefaultOptionInfo<StringVecArg>() << std::endl;

    /**
     * @brief use the arguements
     */
    auto id = parser.getOptionArgv<IntArg>("id");
    std::cout << "the 'id' I get is: " << id << std::endl;
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
  return 0;
}
