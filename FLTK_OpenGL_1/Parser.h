//
// Created by MiBook on 25.03.2018.
//

#ifndef PARSER_MAIN2_H
#define PARSER_MAIN2_H

#include <vector>

struct Expression {
    Expression(std::string token) : token(token) {}
    Expression(std::string token, Expression a) : token(token), args{ a } {}
    Expression(std::string token, Expression a, Expression b) : token(token), args{ a, b } {}

    std::string token;
    std::vector<Expression> args;
};

class Parser {
public:
    explicit Parser(const char* input) : input(input) {}
    Expression parse();
	double eval(const Expression& e);
private:
    std::string parse_token();
    Expression parse_simple_expression();
    Expression parse_binary_expression(int min_priority);

    const char* input;
};

#endif //PARSER_MAIN2_H
