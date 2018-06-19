#ifndef QUERYRESULT_H
#define QUERYRESULT_H

#include <QSqlQuery>
#include <QVariant>

class QueryResult
{
public:
    QueryResult(QSqlQuery query);

    bool next() const;

    QVariant value(const QString &name) const;
    QVariant value(const int index) const;

    bool hasError() const;
    QString error() const;

private:
    mutable QSqlQuery query_;

};

#endif // QUERYRESULT_H
