//
// Created by robert on 19.04.2020.
//

#ifndef TKOM_TOKEN_H
#define TKOM_TOKEN_H

#include <string>
#include <iostream>
#include "Interfaces.h"

enum Type {
    T_ANY = 0,
    T_USER_DEFINED_NAME = 1,
    T_WHILE = 2,
    T_IF = 3,
    T_EXIT = 4,
    T_REAL_NUM = 5,
    T_INT_NUM = 6,
    T_DO = 7,
    T_DONE = 8,
    T_ADD_OPERATOR = 9,
    T_MULT_OPERATOR = 10,
    T_BOOLEAN_OPERATOR = 11,
    T_ASSIGN_OPERATOR = 12,
    T_SPECIFIER = 13,
    T_OPENING_PARENTHESIS = 14,
    T_CLOSING_PARENTHESIS = 15,
    T_SEMICON = 16,
    T_CON = 17,
    T_OPENING_BRACKET = 18,
    T_CLOSING_BRACKET = 19,
    T_END = 20,
    T_NOT_DEFINED_YET = 21,
    T_STRING = 22,
    T_EOF = 23,
    T_BOOLEAN_AND = 24,
    T_BOOLEAN_OR = 25,
    T_DOT = 26,
    T_FUNCTION_NAME = 27,
    T_NO_ARG_FUNCTION_NAME = 28,
    T_NEXT_LINE = 29,
    T_FOR = 30,
    T_ELSE = 31,
    T_DUMMY_ARG = 32,
    T_PUT = 33,
    T_FUNCTION_CALL = 34,
    T_SYSTEM_HANDLER = 35,
    T_REGISTER = 36,
    T_SEND_RAPORT = 37,
    T_BACKUP = 38,
    T_RUN = 39,
    T_CHECK_SYSTEM = 40,
    T_RUN_SCRIPT = 41,
    T_PATH = 42,
    T_MAIL = 43,
    T_RAPORT_TYPE = 44,
    T_RAPORT_DIR = 45,
    T_RET = 46,
};

class Token {

private:
    std::string value;
    Type type;
    Position position;

public:
    Token() = default;
    // position is not necessary, since we can have the same tokens with different positions
    Token(std::string value, Type type, Position position = {0,0}) : value(value), type(type), position(position) {}
    Token(char sign, Type type, Position position = {0,0}) : type(type), position(position) {
        value = sign;
    }
    void setType(Type type) {
        this->type=type;
    }
    std::string getValue() { return value; }
    Type getType() { return type; }
    Position getPosition() { return position; }
    bool isOperand() {
        return type == T_INT_NUM || type == T_USER_DEFINED_NAME ||
                type == T_REAL_NUM || type == T_STRING || type == T_SEND_RAPORT ||
                type == T_BACKUP || type == T_RUN_SCRIPT || type == T_CHECK_SYSTEM;
    }

    friend bool operator==(const Token& lhs, const Token& rhs);
    friend bool operator!=(const Token& lhs, const Token& rhs);
    friend std::ostream& operator<<(std::ostream& out, const Token& t);

    bool isOperator() {
        return type == T_MULT_OPERATOR || type == T_BOOLEAN_AND || type == T_ADD_OPERATOR ||
               type == T_BOOLEAN_OPERATOR || type == T_BOOLEAN_OR || type == T_OPENING_PARENTHESIS
                || type == T_ASSIGN_OPERATOR || type == T_SEMICON || type == T_DOT || type == T_DO
                || type == T_CON;
    }

    bool isCondition() {
        return type == T_WHILE || type == T_FOR || type == T_IF || type == T_ELSE;
    }

    bool isFunction() {
        return type == T_FUNCTION_NAME || type == T_NO_ARG_FUNCTION_NAME;
    }
    bool isClosingParenthesis();

    bool isOpeningParenthesis();
};

#endif //TKOM_TOKEN_H
