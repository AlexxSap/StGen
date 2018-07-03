#ifndef COMMON_H
#define COMMON_H

#include <QStringList>
#include <initializer_list>
#include <QVariantList>

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
QVariantList convVar(Args&& ... args)
{
    QVariantList lst_;
    for (auto&& arg : std::initializer_list<QVariant>{std::forward<Args>(args)...})
    {
        lst_.append(QVariant(arg));
    }
    return lst_;
}


#endif // COMMON_H
