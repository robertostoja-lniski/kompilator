//
// Created by robert on 06.05.2020.
//

#include "../include/Parser.h"

#include <utility>

void Parser::parseToken(Token tokenToParse) {
    if(tokenToParse.getType() == T_NOT_DEFINED_YET) {
        return;
    }
    try {
        std::cout << "parsing " <<  tokenToParse << '\n';
        if(tryToBuildDeclaration(tokenToParse)
            || tryToBuildExpression(tokenToParse)) {
            return;
        }
    } catch(std::exception& e) {
        std::cerr << "Error in syntax\n";
    }
}
bool Parser::tryToBuildExpression(Token tokenToParse) {
    return converter->generatePostfixRepresentation(tokenToParse);
}
void Parser::createIntExpression(Token token) {
    int numericValue = std::stoi(token.getValue());
    recentExpressions.push(std::make_shared<IntExpression>(numericValue));
}
void Parser::createFloatExpression(Token token) {
    double realValue = std::stod(token.getValue());
    recentExpressions.push(std::make_shared<FloatExpression>(realValue));
}
void Parser::createVarNameExpression(Token token) {
    std::string name = token.getValue();
    recentExpressions.push(std::make_shared<VarNameExpression>(name));
}
void Parser::createForExpression(Token token) {
    auto forExpr = std::make_unique<ForExpression>();

    auto collection = recentExpressions.top();
    recentExpressions.pop();
    auto iter = recentExpressions.top();
    recentExpressions.pop();

    forExpr->left = iter;
    forExpr->collectionName = collection;
    recentExpressions.push(std::move(forExpr));
}

void Parser::createElseExpression(Token token) {
    recentExpressions.push(std::move(std::make_shared<ElseExpression>()));
}

void Parser::createIfExpression(Token token) {
    auto ifExpr = std::make_shared<IfExpression>();
    auto cond = recentExpressions.top();
    recentExpressions.pop();

    ifExpr->left = cond;
    recentExpressions.size();
    recentExpressions.push(std::move(ifExpr));
}

void Parser::createWhileExpression(Token token) {
    auto whileExpr = std::make_shared<WhileExpression>();
    auto cond = recentExpressions.top();
    recentExpressions.pop();

    whileExpr->left = cond;
    recentExpressions.size();
    recentExpressions.push(std::move(whileExpr));
}
void Parser::setDoubleArgsExpr(std::shared_ptr<DoubleArgsExpression> doubleArgsExpression) {
    if(recentExpressions.size() < 2) {
        throw std::runtime_error("Not enough args for operation");
    }
    auto right = recentExpressions.top();
    recentExpressions.pop();
    auto left = recentExpressions.top();
    recentExpressions.pop();

    doubleArgsExpression->left = left;
    doubleArgsExpression->right = right;

    recentExpressions.push(std::move(doubleArgsExpression));
}
void Parser::createAdditionExpression(Token token) {
    setDoubleArgsExpr(std::make_shared<AdditionExpression>(token.getValue()));
}
void Parser::createMultExpression(Token token) {
    setDoubleArgsExpr(std::make_shared<MultiplyExpression>());
}
void Parser::createBooleanOperatorExpression(Token token) {
    auto value = token.getValue();
    setDoubleArgsExpr(std::make_shared<BooleanOperatorExpression>(value));
}
void Parser::createAssignExpression(Token token) {
    setDoubleArgsExpr(std::make_unique<AssignExpression>());
}
void Parser::createBooleanAndExpression(Token token) {
    setDoubleArgsExpr(std::make_unique<BooleanAndExpression>());
}
void Parser::createBooleanOrExpression(Token token) {
    setDoubleArgsExpr(std::make_unique<BooleanOrExpression>());
}
void Parser::createNextLineExpression(Token token) {
    // if if/while/for body is at stack top
    // do not take it out.
    assignTreeToRoot();
}
void Parser::createFunctionCallExpression(Token token) {
    auto funcExpr = std::make_unique<FunctionExpression>();
    funcExpr->left = std::make_shared<VarNameExpression>(token.getValue());

    if(recentExpressions.empty()) {
        funcExpr->right = nullptr;
    } else {
        auto nextArg = recentExpressions.top();
        recentExpressions.pop();
        funcExpr->right = nextArg;
    }
    recentExpressions.push(std::move(funcExpr));
}
void Parser::generateTree() {
    auto postfixCopy = converter->getPostfixRepresentation();
    while(!postfixCopy.empty()) {
        auto currentToken = postfixCopy.front();
        transformTokenIntoTreeNode(currentToken);
        postfixCopy.pop_front();
    }
    assignTreeToRoot();
}
void Parser::analyzeTree() {
    ExpressionVisitor expressionVisitor;
    while(!roots.empty()) {
        auto currentRoot = roots.front();
        expressionVisitor.visit(currentRoot.get());
        roots.pop_front();
    }
}

void Parser::createSemiconExpression(Token token) {
    auto newArgs = std::make_shared<FunctionArgExpression>();

    if(recentExpressions.size() < 2) {
        throw std::runtime_error("Not enough function args");
    }
    newArgs->right = recentExpressions.top();
    recentExpressions.pop();
    newArgs->left = recentExpressions.top();
    recentExpressions.pop();

    recentExpressions.push(newArgs);
}

void Parser::createNoArgFunctionExpression(Token token) {
    std::string name = token.getValue();
    recentExpressions.push(std::make_shared<NoArgFunctionExpression>(name));
}

void Parser::transformTokenIntoTreeNode(std::shared_ptr<Token> token) {
    auto tokenType = token->getType();
    auto handler = tokensToNode[tokenType];
    handler(*token);
}

void Parser::assignTreeToRoot() {
    while(!recentExpressions.empty()) {
        auto newRoot = std::make_shared<RootExpression>();
        newRoot->expr = recentExpressions.top();
        roots.push_back(newRoot);
        recentExpressions.pop();
    }
}

void Parser::createDoExpression(Token token) {
    auto condBody = std::make_shared<DoExpression>();
    recentExpressions.push(condBody);
}

void Parser::joinUpperStatementsUntilDoFound(std::shared_ptr<BodyExpression> condBody){
    auto upperRoot = roots.back()->expr;
    while(!std::dynamic_pointer_cast<DoExpression>(upperRoot)) {
        condBody->statements.push_front(upperRoot);
        roots.pop_back();
        upperRoot = roots.back()->expr;
    }
    roots.pop_back();
}
void Parser::assignBodyToUpperElse(std::shared_ptr<BodyExpression> condBody) {
    roots.pop_back();
    auto condExpr = roots.back()->expr;
    auto condExprAsDoubleArg = std::dynamic_pointer_cast<IfExpression>(condExpr);
    condExprAsDoubleArg->elseCondition = condBody;
}
void Parser::assignBodyToUpperDeclaration(std::shared_ptr<BodyExpression> condBody, std::shared_ptr<Expression> condExpr) {
    auto declaration = std::dynamic_pointer_cast<TypeSpecifierExpression>(condExpr);
    auto declarationStatements = std::dynamic_pointer_cast<BodyExpression>(declaration->right);
    declarationStatements->statements.push_back(condBody);
}
void Parser::assignBodyToUpperAnyExpression(std::shared_ptr<BodyExpression> condBody, std::shared_ptr<Expression> condExpr) {
    auto condExprAsDoubleArg = std::dynamic_pointer_cast<DoubleArgsExpression>(condExpr);
    condExprAsDoubleArg->right = condBody;
}
void Parser::assignBodyToUpperExpression(std::shared_ptr<BodyExpression> condBody) {
    auto condExpr = roots.back()->expr;
    auto isElse = std::dynamic_pointer_cast<ElseExpression>(condExpr);
    auto isDeclaration = std::dynamic_pointer_cast<TypeSpecifierExpression>(condExpr);
    if(isElse) {
        assignBodyToUpperElse(condBody);
    } else if (isDeclaration) {
        assignBodyToUpperDeclaration(condBody, condExpr);
    } else {
        assignBodyToUpperAnyExpression(condBody, condExpr);
    }
}
void Parser::createDoneExpression(Token token) {
    auto condBody = std::make_shared<BodyExpression>();
    joinUpperStatementsUntilDoFound(condBody);
    assignBodyToUpperExpression(condBody);
}

void Parser::addDeclarationsToTree(std::shared_ptr<RootExpression> declaration) {
    roots.push_back(std::move(declaration));
}

bool Parser::tryToBuildDeclaration(Token token) {
    auto buildDeclaration = declarationReader->tryToBuildDeclaration(token);
    if(buildDeclaration) {
        addDeclarationsToTree(buildDeclaration);
    }
}