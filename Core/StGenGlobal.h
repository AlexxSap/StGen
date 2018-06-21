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
}

#endif // STGENGLOBAL_H
