//
// Created by csl on 1/11/23.
//

#ifndef FLAGS_ARG_TYPE_HPP
#define FLAGS_ARG_TYPE_HPP

#include "memory"
#include "optional"
#include "vector"
#include "string"
#include "algorithm"
#include "sstream"
#include "iostream"

namespace ns_flags {
    template<typename ElemType>
    static std::ostream &operator<<(std::ostream &os, const std::vector<ElemType> &vec) {
        os << '[';
        if (vec.empty()) {
            os << ']';
        } else {
            for (int i = 0; i != vec.size() - 1; ++i) { os << vec.at(i) << ", "; }
            os << vec.back();
            os << ']';
        }
        return os;
    }


    struct Argument {
    public:
        Argument() = default;

        virtual ~Argument() = default;

        [[nodiscard]] virtual std::string ValueString() const = 0;

        [[nodiscard]] virtual std::string TypeNameString() const = 0;

    protected:

        virtual std::optional<std::string> DataFromStringVector(const std::vector<std::string> &strVec) = 0;
    };

#define ARGUMENT_TEMPLATE_GENERATOR_BEGIN(ArgType, DataType)                \
  struct ArgType : Argument {                                               \
  public:                                                                   \
    using data_type = DataType;                                             \
                                                                            \
  protected:                                                                \
    data_type data;                                                         \
                                                                            \
  public:                                                                   \
    explicit ArgType(const DataType &data = {})                             \
        : Argument(), data(data) {}                                         \
                                                                            \
    ~ArgType() override = default;                                          \
                                                                            \
    [[nodiscard]] std::string ValueString() const override {                \
      std::stringstream stream;                                             \
      stream << data;                                                       \
      return stream.str();                                                  \
    }                                                                       \
                                                                            \
  protected:                                                                \
                                                                            \
    friend std::ostream &operator<<(std::ostream &os, const ArgType &obj) { \
      os << obj.TypeNameString() << "{data: " << obj.data << '}';           \
      return os;                                                            \
    }                                                                       \
                                                                            \
    [[nodiscard]] std::string TypeNameString() const override {             \
      return #ArgType;                                                      \
    }

#define ARGUMENT_TEMPLATE_GENERATOR_END };


    /**
     * int type argument
     */
    ARGUMENT_TEMPLATE_GENERATOR_BEGIN(Int, int)

    protected:
        std::optional<std::string> DataFromStringVector(const std::vector<std::string> &strVec) override {
            if (strVec.empty()) {
                return "At least one parameter is required to initialize 'Int' type argument";
            } else {
                data = std::stoi(strVec.front());
                return {};
            }
        }
    ARGUMENT_TEMPLATE_GENERATOR_END

    /**
     * int vector type argument
     */
    ARGUMENT_TEMPLATE_GENERATOR_BEGIN(IntVec, std::vector<int>)

    protected:
        std::optional<std::string> DataFromStringVector(const std::vector<std::string> &strVec) override {
            if (strVec.empty()) {
                return "At least one parameter is required to initialize 'IntVec' type argument";
            } else {
                data.resize(strVec.size());
                std::transform(strVec.cbegin(), strVec.cend(), data.begin(), [](const std::string &str) {
                    return std::stoi(str);
                });
                return {};
            }
        }
    ARGUMENT_TEMPLATE_GENERATOR_END

}

#endif
