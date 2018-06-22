#ifndef STABLE_H
#define STABLE_H

#include <QVariant>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QSharedPointer>
#include <QDate>

#include <type_traits>

#include "Common.h"

#define NOT_IS(A, B) typename std::enable_if<!std::is_base_of<A, B>::value, nullptr_t>::type = nullptr
#define IS(A, B) typename std::enable_if<std::is_base_of<A, B>::value, nullptr_t>::type = nullptr

#endif // STABLE_H
