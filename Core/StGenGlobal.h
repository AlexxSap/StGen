#ifndef STGENGLOBAL_H
#define STGENGLOBAL_H

#include "StGen.h"
#include "stable.h"

#define SimpleTypeOperation(name) template <typename A, typename B> \
AbsExprPointer name(A a, B b, NOT_IS(AbstractExpression, A), NOT_IS(AbstractExpression, B)) \
{ \
    return ExprPointer::create(AbstractExpression::Operation::name, std::move(a), std::move(b)); \
}

namespace StGenGlobal
{
    AbstractBuilder sqlBuilder = nullptr;

    template <typename... Args>
    SelectQuery select(Args ... args)
    {
        return sqlBuilder->select(ColumnsQuery(conv(args...)));
    }

    QString bind(const QString &id)
    {
        return ":" + id;
    }

    SimpleTypeOperation(equal)
    SimpleTypeOperation(notEqual)
    SimpleTypeOperation(less)
    SimpleTypeOperation(lessEqual)
    SimpleTypeOperation(more)
    SimpleTypeOperation(moreEqual)

    AbsExprPointer andE(AbsExprPointer a, AbsExprPointer b)
    {
        return ExprPointer::create(AbstractExpression::Operation::andE, std::move(a), std::move(b));
    }

    AbsExprPointer orE(AbsExprPointer a, AbsExprPointer b)
    {
        return ExprPointer::create(AbstractExpression::Operation::orE, std::move(a), std::move(b));
    }
}

#endif // STGENGLOBAL_H
