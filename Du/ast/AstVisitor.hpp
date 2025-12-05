#pragma once
#include "AstExpr.hpp"
#include <iostream>
#include <cassert>

// Helper dla std::visit
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

// Przykład wizytatora drukującego AST (tylko do demonstracji)
// ... reszta pliku ...

struct AstPrinterVisitor {
    int indentLevel = 0;

    void printIndent() const {
        for (int i = 0; i < indentLevel; ++i) std::cout << "  ";
    }

    // --- Obsługa konkretnych typów ---

    void operator()(const AstNodes::BinaryExpr& node) {
        printIndent();
        std::cout << "BinaryOp\n";
        
        indentLevel++;
        if(node.left) node.left->accept(*this);
        if(node.right) node.right->accept(*this);
        indentLevel--;
    }

    void operator()(const AstNodes::CmpExpr& node) {
        printIndent();
        std::cout << "CompareOp\n";

        indentLevel++;
        if (node.left) node.left->accept(*this);
        if (node.right) node.right->accept(*this);
        indentLevel--;
    }

    void operator()(const AstNodes::UnaryExpr& node) {
        printIndent();
        std::cout << "UnaryOp\n";
        
        indentLevel++;
        if (node.operand) node.operand->accept(*this);
        indentLevel--;
    }

    void operator()(const AstNodes::LiteralExpr& node) {
        printIndent();
        std::cout << "Literal: ";
        std::visit([](auto&& val) { std::cout << val; }, node.value);
        std::cout << "\n";
    }

    void operator()(const AstNodes::VariableRefExpr& node) {
        printIndent();
        std::cout << "VarRef: " << node.name << "\n";
    }

    void operator()(const AstNodes::FunctionCallExpr& node) {
        printIndent();
        std::cout << "Call: " << node.funcName << "\n";
        
        indentLevel++;
        for (const auto& arg : node.args) {
            arg->accept(*this);
        }
        indentLevel--;
    }

    void operator()(const AstNodes::ArrayIndexingExpr& node) {
        printIndent();
        std::cout << "ArrayIndex\n";
        indentLevel++;
        if(node.arrayExpr) node.arrayExpr->accept(*this);
        for(const auto& idx : node.indices) {
            idx->accept(*this);
        }
        indentLevel--;
    }

    void operator()(const AstNodes::CastExpr& node) {
        printIndent();
        std::cout << "Cast\n";
        indentLevel++;
        if (node.expr) node.expr->accept(*this);
        indentLevel--;
    }
};
