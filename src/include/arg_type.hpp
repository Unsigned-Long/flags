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
    struct Utils {
    public:
        /**
         * @brief Converts a string to lowercase
         *
         * @param str the string
         * @return std::string
         */
        static std::string Tolower(const std::string &str) {
            std::string lowerStr;
            for (const auto i: str) {
                lowerStr.push_back(static_cast<char>(std::tolower(static_cast<int>(i))));
            }
            return lowerStr;
        }

        static bool StrToBool(const std::string &str) {
            std::string val = Tolower(str);
            bool b;
            if (val == "on" || val == "1" || val == "true" || val.empty()) {
                b = true;
            } else if (val == "off" || val == "0" || val == "false") {
                b = false;
            } else {
                b = std::stoi(str);
            }
            return b;
        }
    };

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

        template<class BoostType>
        BoostType *Boost() { return dynamic_cast<BoostType *>(this); }

        virtual void DataFromStringVector(const std::vector<std::string> &strVec) = 0;
    };

#define ARGUMENT_TEMPLATE_GENERATOR_BEGIN(ArgType, DataType)                \
  struct ArgType : Argument {                                               \
  public:                                                                   \
    using data_type = DataType;                                             \
                                                                            \
  protected:                                                                \
    DataType data;                                                          \
                                                                            \
  public:                                                                   \
    explicit ArgType(const DataType &data = {}) : Argument(), data(data) {} \
                                                                            \
    ~ArgType() override = default;                                          \
                                                                            \
    [[nodiscard]] std::string TypeNameString() const override {             \
      return #ArgType;                                                      \
    }                                                                       \
                                                                            \
    [[nodiscard]] std::string ValueString() const override {                \
      std::stringstream stream;                                             \
      stream << std::boolalpha << data;                                     \
      return stream.str();                                                  \
    }                                                                       \
                                                                            \
    [[nodiscard]] const DataType &GetData() const {                         \
      return data;                                                          \
    }                                                                       \
                                                                            \
    friend std::ostream &operator<<(std::ostream &os, const ArgType &obj) { \
      os << obj.TypeNameString() << "{data: " << obj.data << '}';           \
      return os;                                                            \
    }
#define ARGUMENT_TEMPLATE_GENERATOR_END };

    /**
     * int type argument
     */
    ARGUMENT_TEMPLATE_GENERATOR_BEGIN(Int, int)

        void DataFromStringVector(const std::vector<std::string> &strVec) override {
            if (!strVec.empty()) {
                data = std::stoi(strVec.front());
            }
        }
    ARGUMENT_TEMPLATE_GENERATOR_END

    /**
     * int vector type argument
     */
    ARGUMENT_TEMPLATE_GENERATOR_BEGIN(IntVec, std::vector<int>)

        void DataFromStringVector(const std::vector<std::string> &strVec) override {
            data.resize(strVec.size());
            std::transform(strVec.cbegin(), strVec.cend(), data.begin(), [](const std::string &str) {
                return std::stoi(str);
            });
        }
    ARGUMENT_TEMPLATE_GENERATOR_END

    /**
     * bool type argument
     */
    ARGUMENT_TEMPLATE_GENERATOR_BEGIN(Bool, bool)

        void DataFromStringVector(const std::vector<std::string> &strVec) override {
            if (!strVec.empty()) {
                data = Utils::StrToBool(strVec.front());
            }
        }
    ARGUMENT_TEMPLATE_GENERATOR_END

    /**
     * bool vector type argument
     */
    ARGUMENT_TEMPLATE_GENERATOR_BEGIN(BoolVec, std::vector<bool>)

        void DataFromStringVector(const std::vector<std::string> &strVec) override {
            data.resize(strVec.size());
            std::transform(strVec.cbegin(), strVec.cend(), data.begin(), [](const std::string &str) {
                return Utils::StrToBool(str);
            });
        }
    ARGUMENT_TEMPLATE_GENERATOR_END

    /**
     * float type argument
     */
    ARGUMENT_TEMPLATE_GENERATOR_BEGIN(Float, float)

        void DataFromStringVector(const std::vector<std::string> &strVec) override {
            if (!strVec.empty()) {
                data = std::stof(strVec.front());
            }
        }
    ARGUMENT_TEMPLATE_GENERATOR_END

    /**
     * float vector type argument
     */
    ARGUMENT_TEMPLATE_GENERATOR_BEGIN(FloatVec, std::vector<float>)

        void DataFromStringVector(const std::vector<std::string> &strVec) override {
            data.resize(strVec.size());
            std::transform(strVec.cbegin(), strVec.cend(), data.begin(), [](const std::string &str) {
                return std::stof(str);
            });
        }
    ARGUMENT_TEMPLATE_GENERATOR_END

    /**
     * double type argument
     */
    ARGUMENT_TEMPLATE_GENERATOR_BEGIN(Double, double)

        void DataFromStringVector(const std::vector<std::string> &strVec) override {
            if (!strVec.empty()) {
                data = std::stod(strVec.front());
            }
        }
    ARGUMENT_TEMPLATE_GENERATOR_END

    /**
     * double vector type argument
     */
    ARGUMENT_TEMPLATE_GENERATOR_BEGIN(DoubleVec, std::vector<double>)

        void DataFromStringVector(const std::vector<std::string> &strVec) override {
            data.resize(strVec.size());
            std::transform(strVec.cbegin(), strVec.cend(), data.begin(), [](const std::string &str) {
                return std::stod(str);
            });
        }
    ARGUMENT_TEMPLATE_GENERATOR_END

    /**
     * string type argument
     */
    ARGUMENT_TEMPLATE_GENERATOR_BEGIN(String, std::string)

        void DataFromStringVector(const std::vector<std::string> &strVec) override {
            if (!strVec.empty()) {
                data = strVec.front();
            }
        }
    ARGUMENT_TEMPLATE_GENERATOR_END

    /**
     * string vector type argument
     */
    ARGUMENT_TEMPLATE_GENERATOR_BEGIN(StringVec, std::vector<std::string>)

        void DataFromStringVector(const std::vector<std::string> &strVec) override {
            data = strVec;
        }
    ARGUMENT_TEMPLATE_GENERATOR_END

    /**
     * help type argument
     */
    ARGUMENT_TEMPLATE_GENERATOR_BEGIN(Help, std::string)

        void DataFromStringVector(const std::vector<std::string> &strVec) override {
            if (!strVec.empty()) {
                data = strVec.front();
            }
        }
    ARGUMENT_TEMPLATE_GENERATOR_END

    /**
     * version type argument
     */
    ARGUMENT_TEMPLATE_GENERATOR_BEGIN(Version, std::string)

        void DataFromStringVector(const std::vector<std::string> &strVec) override {
            if (!strVec.empty()) {
                data = strVec.front();
            }
        }
    ARGUMENT_TEMPLATE_GENERATOR_END

}

#endif
