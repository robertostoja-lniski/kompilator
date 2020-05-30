//
// Created by robert on 06.05.2020.
//

#ifndef TKOM_VISITOR_H
#define TKOM_VISITOR_H

#include <memory>
#include <stack>
#include <queue>
#include <variant>
#include <string>
#include <cassert>
#include <map>
struct FieldReferenceExpression;
struct VarDeclarationExpression;
struct TypeSpecifierExpression;
struct PutExpression;
// lexical expression
struct VarNameExpression;
struct RootExpression;
// numeric expressions
struct IntExpression;
struct FloatExpression;

// Secial words
struct IfExpression;
struct ElseExpression;
struct ForExpression;
struct WhileExpression;

// generic structure for double args expressions
struct DoubleArgsExpression;
// numeric operations expressions
struct AdditionExpression;
struct BooleanAndExpression;
struct BooleanOrExpression;
struct BooleanOperatorExpression;
struct MultiplyExpression;
struct DivideExpression;
// other
struct AssignExpression;
struct DeclarationExpression;
// functions
struct FunctionArgExpression;
struct FunctionExpression;
struct FunctionCallExpression;
struct NoArgFunctionExpression;
// file organisation
struct NewLineExpression;
struct BodyExpression;
struct DoExpression;
struct FileExpression;

struct Visitor {
    // visit root
    virtual void visit(RootExpression* rootExpression) = 0;
    virtual void visit(FileExpression* fileExpression) = 0;

    // numeric and lexical storage
    virtual void visit(IntExpression* intExpression) = 0;
    virtual void visit(FloatExpression* floatExpression) = 0;
    virtual void visit(VarNameExpression* varNameExpression) = 0;
    virtual void visit(DoubleArgsExpression* doubleArgsExpression) = 0;

    // numeric operations
    virtual void visit(AdditionExpression* additionExpression) = 0;
    virtual void visit(MultiplyExpression* multiplyExpression) = 0;
    virtual void visit(DivideExpression* divideExpression) = 0;

    // other
    virtual void visit(FieldReferenceExpression* fieldReferenceExpression) = 0;
    virtual void visit(AssignExpression* assignExpression) = 0;
    virtual void visit(VarDeclarationExpression* varDeclarationExpression) = 0;
    virtual void visit(TypeSpecifierExpression* typeSpecifierExpression) = 0;

    // boolean
    virtual void visit(BooleanAndExpression* booleanAndExpression) = 0;
    virtual void visit(BooleanOrExpression* booleanOrExpression) = 0;
    virtual void visit(BooleanOperatorExpression* booleanOrExpression) = 0;

    // functions
    virtual void visit(FunctionCallExpression* functionCallExpression) = 0;
    virtual void visit(FunctionArgExpression* functionArgExpression) = 0;
    virtual void visit(FunctionExpression* functionExpression) = 0;
    virtual void visit(NoArgFunctionExpression* noArgFunctionExpression) = 0;
    virtual void visit(PutExpression* putExpression) = 0;

    // file organisation
    virtual void visit(NewLineExpression* newLineExpression) = 0;
    virtual void visit(BodyExpression* bodyExpression) = 0;
    virtual void visit(DoExpression* doExpression) = 0;

    //keywords
    virtual void visit(IfExpression* ifExpression) = 0;
    virtual void visit(ElseExpression* elseExpression) = 0;
    virtual void visit(ForExpression* forExpression) = 0;
    virtual void visit(WhileExpression* whileExpression) = 0;

};

struct ExpressionVisitor : Visitor {
    void visit(IntExpression* intExpression) override;
    void visit(FloatExpression* floatExpression) override;
    void visit(VarNameExpression* varNameExpression)  override;
    void visit(DoubleArgsExpression* doubleArgsExpression) override;
    void visit(AdditionExpression* additionExpression) override;
    void visit(MultiplyExpression* multiplyExpression)  override;
    void visit(DivideExpression* divideExpression) override;
    void visit(AssignExpression* assignExpression) override;
    void visit(RootExpression* rootExpression) override;
    void visit(VarDeclarationExpression* varDeclarationExpression) override;
    void visit(TypeSpecifierExpression* typeSpecifierExpression) override;
    void visit(BooleanAndExpression* booleanAndExpression) override;
    void visit(BooleanOrExpression* booleanOrExpression) override;
    void visit(BooleanOperatorExpression* booleanOrExpression) override;
    void visit(FunctionArgExpression* functionArgExpression) override;
    void visit(FunctionExpression* functionExpression) override;
    void visit(FunctionCallExpression* functionCallExpression) override;
    void visit(NoArgFunctionExpression* noArgFunctionExpression) override;
    void visit(NewLineExpression* newLineExpression) override;
    void visit(BodyExpression* bodyExpression) override;
    void visit(IfExpression* ifExpression) override;
    void visit(ElseExpression* elseExpression) override;
    void visit(ForExpression* forExpression) override;
    void visit(WhileExpression* whileExpression) override;
    void visit(DoExpression* doExpression) override;
    void visit(FileExpression* fileExpression) override;
    void visit(FieldReferenceExpression* fieldReferenceExpression) override;
    void visit(PutExpression* putExpression) override;
};


struct EvaluationVisitor : Visitor {
    struct PrimitiveValue {
        PrimitiveValue() = default;
        virtual ~PrimitiveValue() = default;
    };
    struct IntValue : PrimitiveValue {
        int value {0};
        IntValue(int value) : value(value) {}
    };
    struct RealValue : PrimitiveValue {
        double value {0};
        RealValue(double value) : value(value) {}
    };
    struct StrValue : PrimitiveValue {
        std::string value;
        StrValue(std::string value) : value(value) {}
    };
    struct FunctionDeclaration {
        std::string specifier;
        struct FunctionArg {
            std::string specifier;
            std::string name;

            FunctionArg(std::string specifier, std::string name) :
                    specifier(specifier), name(name) {}
        };

        std::vector<FunctionArg> args;
        std::shared_ptr<BodyExpression> body;
    };

    struct Context {
        enum class Specifiers {INT, FLOAT, STRING, SYSTEM_HANDLER};
        std::map<std::string, std::shared_ptr<PrimitiveValue>> variableAssignmentMap;
        std::map<std::string, std::string> declarationMap;
        std::map<std::string, FunctionDeclaration> functionDeclarationMap;
        std::queue<std::shared_ptr<PrimitiveValue>> operands;
        Context() = default;
        auto moveLocalOperand() {
            auto ret = operands.front();
            operands.pop();
            return ret;
        }
    };

    auto moveLocalOperandFromNearestContext() {
        return ctx.back().moveLocalOperand();
    }
    // front is most global
    // back is most local
    std::deque<Context> ctx;

    void visit(IntExpression* intExpression) override;
    void visit(FloatExpression* floatExpression) override;
    void visit(VarNameExpression* varNameExpression)  override;
    void visit(DoubleArgsExpression* doubleArgsExpression) override;
    void visit(AdditionExpression* additionExpression) override;
    void visit(MultiplyExpression* multiplyExpression)  override;
    void visit(DivideExpression* divideExpression) override;
    void visit(AssignExpression* assignExpression) override;
    void visit(RootExpression* rootExpression) override;
    void visit(VarDeclarationExpression* varDeclarationExpression) override;
    void visit(TypeSpecifierExpression* typeSpecifierExpression) override;
    void visit(BooleanAndExpression* booleanAndExpression) override;
    void visit(BooleanOrExpression* booleanOrExpression) override;
    void visit(BooleanOperatorExpression* booleanOrExpression) override;
    void visit(FunctionArgExpression* functionArgExpression) override;
    void visit(FunctionExpression* functionExpression) override;
    void visit(NoArgFunctionExpression* noArgFunctionExpression) override;
    void visit(NewLineExpression* newLineExpression) override;
    void visit(BodyExpression* bodyExpression) override;
    void visit(IfExpression* ifExpression) override;
    void visit(ElseExpression* elseExpression) override;
    void visit(ForExpression* forExpression) override;
    void visit(WhileExpression* whileExpression) override;
    void visit(DoExpression* doExpression) override;
    void visit(FileExpression* fileExpression) override;
    void visit(FieldReferenceExpression* fieldReferenceExpression) override;
    void visit(FunctionCallExpression* functionCallExpression) override;
    void visit(PutExpression* putExpression) override;
};

struct Expression {
    virtual void accept(Visitor* visitor) = 0;
};
struct DoExpression : Expression {
    // simple boundary for done.
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct FileExpression : Expression {
    std::deque <std::shared_ptr<RootExpression>> roots;
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct NoArgFunctionExpression : Expression {
    std::string name;
    NoArgFunctionExpression(std::string name) : name(name) {}
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

struct IntExpression : Expression {
    int value{};
    IntExpression(int value) : value(value) {}
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct FloatExpression : Expression {
    double value{};
    FloatExpression(double value) : value(value) {}
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct VarNameExpression : Expression {
    std::string value;
    VarNameExpression(std::string value) : value(value) {}
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct PutExpression : Expression {
    std::shared_ptr<Expression> toPrint {};
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct DoubleArgsExpression : Expression {
    std::shared_ptr<Expression> left{}, right{};
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

struct WhileExpression : DoubleArgsExpression {
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

struct IfExpression : DoubleArgsExpression {
    std::shared_ptr<BodyExpression> elseCondition {nullptr};
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

struct ForExpression : DoubleArgsExpression {
    std::shared_ptr<Expression> collectionName;
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct TypeSpecifierExpression : DoubleArgsExpression {
    std::string value;
    TypeSpecifierExpression(std::string value) : value(value) {}
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

struct RootExpression : Expression {
    std::shared_ptr<Expression> expr;
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct BodyExpression:Expression {
    std::deque<std::shared_ptr<Expression>> statements;
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct ElseExpression : Expression {
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct NewLineExpression : DoubleArgsExpression {
    std::shared_ptr<Expression> left{}, right{};
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct BooleanOperatorExpression : DoubleArgsExpression {
    std::string value;
    BooleanOperatorExpression(std::string value) :
        value(value) {}
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct FunctionExpression : DoubleArgsExpression {
    std::string value;
    std::shared_ptr<BodyExpression> body;
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct FunctionCallExpression : DoubleArgsExpression {
    std::string value;
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct AdditionExpression : DoubleArgsExpression {
    std::string operation;
    AdditionExpression(std::string operation) : operation(operation) {}
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct VarDeclarationExpression : DoubleArgsExpression {
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct DivideExpression : DoubleArgsExpression {
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct MultiplyExpression : DoubleArgsExpression {
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct AssignExpression : DoubleArgsExpression {
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct BooleanOrExpression : DoubleArgsExpression {
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct BooleanAndExpression : DoubleArgsExpression {
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct FunctionArgExpression : DoubleArgsExpression {
    FunctionArgExpression() {
        right = nullptr;
    }
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
struct FieldReferenceExpression : DoubleArgsExpression {
    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};
#endif //TKOM_VISITOR_H
