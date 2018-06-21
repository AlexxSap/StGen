#ifndef STGENGLOBAL_H
#define STGENGLOBAL_H

#include "StGen.h"

namespace StGenGlobal
{
    AbstractBuilder sqlBuilder = nullptr;

    template <typename... Args>
    SelectQuery select(Args ... args)
    {
        return sqlBuilder->select(ColumnsQuery(conv(args...)));
    }

    template <typename A, typename B>
    Expr equal(A a, B b)
    {
        return EqualExpr::create(std::move(a), std::move(b));
    }
}

#endif // STGENGLOBAL_H
