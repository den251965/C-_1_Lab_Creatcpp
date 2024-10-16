#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "setting.h"

enum class Operator {
    Invalid = 0,
    Plus,
    Minus,
    Umnojenie,
    Delenie,
};

std::map<Operator, char> op_ {
    {  Operator::Plus      , '+' },
    {  Operator::Minus     , '-' },
    {  Operator::Umnojenie , '*' },
    {  Operator::Delenie   , '/' },
};

std::vector<Operator> operators(Setting& sett) {
    size_t n;
    std::stringstream in_count(sett.value(Setting::CountOperators));
    in_count >> n;
    std::stringstream in_operator(sett.value(Setting::Operator));
    int16_t op;
    std::vector<Operator> t;
    t.reserve(n);
    for (size_t i = 1; i <= n; ++i) {
        in_operator >> op;
        switch (op) {
        case 1: {
            t.push_back(Operator::Plus);
            break;
        }
        case 2: {
            t.push_back(Operator::Minus);
            break;
        }
        case 3: {
            t.push_back(Operator::Umnojenie);
            break;
        }
        case 4: {
            t.push_back(Operator::Delenie);
            break;
        }
        default: {
            t.push_back(Operator::Invalid);
            break;
        }
        }
    }
    return t;
}

int input_operand(int i, Operator m) {
    int b;
    std::string pred =  i == -1 ? "saterted " : "";
    std::string name =  i == -1 ? "b" : "b_" + std::to_string(i);
    std::string condition =  m == Operator::Delenie ? "(unequal 0)" : "" ;
    bool prov = false;
    while (!prov) {
        std::cout << "Input " <<  pred << "operand " << name << condition << " :";
        std::cin >> b;
        prov = m == Operator::Delenie && b == 0 ? false : true;
    }
    return b;
}

void generator(const std::vector<Operator>& operators) {
    std::string patch_cpp("creat_cpp.cpp");
    std::ofstream write(patch_cpp);
    write << "#include <iostream>" << "\n";
    write << "\n";
    write << "int main(int argc, char* argv[]) {" << "\n";
    int x = input_operand(-1, Operator::Invalid);
    write << "  int b = " << x << ";\n";
    for (size_t i = 0; i < operators.size(); ++i) {
        x = input_operand(i + 2, operators[i]);
        write << "  int b_"<< i + 2 << " = " << x << ";\n";
    }
    write << "  int x = b";
    for (size_t i = 0; i < operators.size(); ++i) {
       write << " " << op_[operators[i]] << " b_" << i + 2 ;
    }
    write << ";\n";
    write << "  std::cout << x;" << "\n";
    write << "}" << "\n";

}

int main(int argc, char* argv[]) {
    std::string pathsett = "";
    if (argc > 1) {
        pathsett = argv[0];
    }
    Setting sett(pathsett);
    std::vector<Operator> query = operators(sett);
    generator(query);
    return 0;
}
