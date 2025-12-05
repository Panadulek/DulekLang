#pragma once

#include <variant>
#include <memory>
#include <vector>
#include <string>
#include <optional>
#include "AstType.hpp"
#include "VariableDecorator.hpp"
#include "CastGraph.hpp" 
#include "AstElement.hpp"

class AstExpr;

namespace AstNodes {

    enum class BinaryOpType : uint8_t {
        Addition,
        Subtraction,
        Multiplication,
        Division,
    };

    enum class CmpOpType : uint8_t {
        Equal,
        GreaterThan,
        LessThan,
        GreaterOrEq,
        LessOrEq,
        NotEqual
    };

    enum class UnaryOpType : uint8_t {
        Negation, 
    };

    // --- Node Structures ---

    struct BinaryExpr {
        BinaryOpType op;
        std::unique_ptr<AstExpr> left;
        std::unique_ptr<AstExpr> right;
    };

    struct CmpExpr {
        CmpOpType op;
        std::unique_ptr<AstExpr> left;
        std::unique_ptr<AstExpr> right;
    };

    struct UnaryExpr {
        UnaryOpType op;
        std::unique_ptr<AstExpr> operand;
    };

    struct LiteralExpr {
        using ValueType = std::variant<long long, unsigned long long, double, bool, std::string>;
        ValueType value;
    };

    struct VariableRefExpr {
        std::string name;
        AstElement* declaration = nullptr; // Pointer to declaration
    };

    struct FunctionCallExpr {
        std::string funcName;
        std::vector<std::unique_ptr<AstExpr>> args;
    };

    struct ArrayIndexingExpr {
        std::unique_ptr<AstExpr> arrayExpr;
        std::vector<std::unique_ptr<AstExpr>> indices;
    };
    
    struct CastExpr {
        CastOp castOp; 
        std::unique_ptr<AstExpr> expr;
    };

} // namespace AstNodes

class AstExpr : public AstElement {
public:
    // Backward compatibility enums
    enum class CMP_OPERATION : uint8_t
    {
        EQUAL = 0,
        GREATER_THAN,
        LESS_THAN,
        GREATER_OR_EQ,
        LESS_OR_EQ,
        NOT_EQUAL,
    };
    
    // Main variant definition
    using ExprVariant = std::variant<
        AstNodes::BinaryExpr,
        AstNodes::CmpExpr,
        AstNodes::UnaryExpr,
        AstNodes::LiteralExpr,
        AstNodes::VariableRefExpr,
        AstNodes::FunctionCallExpr,
        AstNodes::ArrayIndexingExpr,
        AstNodes::CastExpr
    >;

private:
    ExprVariant m_expression;
    
    // Metadata
    std::unique_ptr<AstType> m_type; 

public:
    // Implicit constructors
    template <typename T, typename = std::enable_if_t<!std::is_same_v<std::decay_t<T>, AstExpr>>>
    AstExpr(T&& expr) 
        : AstElement("expr", AstElement::ElementType::EXPR), 
          m_expression(std::forward<T>(expr)),
          m_type(nullptr) 
    {}

    const ExprVariant& getExpression() const { return m_expression; }
    ExprVariant& getExpression() { return m_expression; }

    void setType(std::unique_ptr<AstType> type) { m_type = std::move(type); }
    AstType* getType() const { return m_type.get(); }

    template <typename Visitor>
    auto accept(Visitor&& visitor) const {
        return std::visit(std::forward<Visitor>(visitor), m_expression);
    }

    template <typename Visitor>
    auto accept(Visitor&& visitor) {
        return std::visit(std::forward<Visitor>(visitor), m_expression);
    }

    virtual ~AstExpr() = default;
};