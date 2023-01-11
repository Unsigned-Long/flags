//
// Created by csl on 1/11/23.
//
#include "flags.hpp"
#include "artwork/logger/logger.h"

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

