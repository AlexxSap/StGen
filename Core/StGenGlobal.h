#ifndef STGENGLOBAL_H
#define STGENGLOBAL_H

#include "StGen.h"
#include "stable.h"

namespace StGenGlobal
{
    AbstractBuilder sqlBuilder = nullptr;

    template <typename... Args>
    SelectQuery select(Args ... args)
    {
        return sqlBuilder->select(ColumnsQuery(conv(args...)));
    }

    template <typename A, typename B>
    AbsExpr equal(A a, B b, NOT_IS(AbstractExpression, A), NOT_IS(AbstractExpression, B))
    {
        return Expr::create(AbstractExpression::Operation::Equal, std::move(a), std::move(b));
    }
}

#endif // STGENGLOBAL_H
