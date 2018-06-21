#include "TCommon.h"

#include <QtTest>
#include "../Core/StGen.h"

TCommon::TCommon(QObject *parent)
    : QObject(parent)
{}

void TCommon::TestConv()
{
    QCOMPARE(conv("a1"), QStringList() << "a1");
    QCOMPARE(conv(QString("a1")), QStringList() << "a1");
    QCOMPARE(conv("b1", "b2", "b3"), QStringList() << "b1" << "b2" << "b3");
    QCOMPARE(conv("c1", "c2", "c3", "c3", "c2", "c1"),
             QStringList() << "c1" << "c2" << "c3" << "c3" << "c2" << "c1");
}
