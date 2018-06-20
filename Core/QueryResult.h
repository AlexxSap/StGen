#ifndef QUERYRESULT_H
#define QUERYRESULT_H

#include <QVariant>
#include "DataBaseInterface.h"

class QueryResult
{
public:
    QueryResult(SqlQuery query);

    bool next() const;

    QVariant value(const QString &name) const;
    QVariant value(const int index) const;

    bool hasError() const;
    QString error() const;

private:
    SqlQuery query_;

};

#endif // QUERYRESULT_H
