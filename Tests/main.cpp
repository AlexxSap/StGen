#include <QCoreApplication>
#include <QtTest/QTest>
#include <QtTest/qtestcase.h>
#include "TSelect.h"

template <class TestClass>
int runTest(const QStringList &args = QStringList())
{
    TestClass test;
    return QTest::qExec(&test,
                        args);
}

int main(int argc, char *argv[])
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    int tests = 0;

    tests += runTest<TSelect>();

    return tests;
}
