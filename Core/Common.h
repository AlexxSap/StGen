#ifndef COMMON_H
#define COMMON_H

#include <QStringList>
#include <initializer_list>

template <typename... Args>
QStringList conv(Args&& ... args)
{
    QStringList lst_;
    for (auto&& arg : std::initializer_list<QString>{std::forward<Args>(args)...})
    {
        lst_.append(arg);
    }
    return lst_;
}


#endif // COMMON_H
