#include "Common.h"


QString variantToString(const QVariant &value)
{
    if(value.type() == QVariant::Date)
    {
        return "'" + value.toDate().toString("yyyy-MM-dd") + "'";
    }
    else if(value.type() == QVariant::String)
    {
        return "'" + value.toString() + "'";
    }

    return value.toString();
}
