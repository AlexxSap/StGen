#ifndef COMMON_H
#define COMMON_H

#include <QStringList>
#include <initializer_list>
#include <QVariantList>

class VariantList : public QVariantList
{
public:
    QStringList toStringList() const
    {
        QStringList result;
        result.reserve(this->size());

        std::transform(constBegin(),
                       constEnd(),
                       std::back_inserter(result),
                       [](const QVariant& var){ return var.toString();});

        return result;
    }
};

template <typename... Args>
QStringList conv(Args&& ... args)
{
    QStringList lst_;
    for (auto&& arg : std::initializer_list<QString>{std::forward<Args>(args)...})
    {
        lst_.append(QString(arg));
    }
    return lst_;
}

template <typename... Args>
VariantList convVar(Args&& ... args)
{
    VariantList lst_;
    for (auto&& arg : std::initializer_list<QVariant>{std::forward<Args>(args)...})
    {
        lst_.append(QVariant(arg));
    }
    return lst_;
}


#endif // COMMON_H
