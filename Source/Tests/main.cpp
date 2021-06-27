#include <QDebug>
#include <QTest>
#include <Tests/RNG/LCRNG64Test.hpp>
#include <Tests/RNG/LCRNGTest.hpp>
#include <Tests/RNG/MTTest.hpp>
#include <Tests/RNG/RNGCacheTest.hpp>
#include <Tests/RNG/RNGEuclideanTest.hpp>
#include <Tests/RNG/SFMTTest.hpp>
#include <Tests/RNG/SHA1Test.hpp>
#include <Tests/RNG/TinyMTTest.hpp>

template <class Testname>
int runTest(QStringList &fails)
{
    Testname test;

    int ret = QTest::qExec(&test);
    if (ret != 0)
    {
        fails.append(QString("%1: %2 test(s) failed").arg(test.metaObject()->className()).arg(ret));
    }

    return ret;
}

int main()
{
    int status = 0;
    QStringList fails;

    // RNG Tests
    status += runTest<LCRNGTest>(fails);
    status += runTest<LCRNG64Test>(fails);
    status += runTest<MTTest>(fails);
    status += runTest<RNGCacheTest>(fails);
    status += runTest<RNGEuclideanTest>(fails);
    status += runTest<SFMTTest>(fails);
    status += runTest<SHA1Test>(fails);
    status += runTest<TinyMTTest>(fails);

    qDebug() << "";
    // Summary of failures at end for easy viewing
    for (const QString &fail : fails)
    {
        qDebug() << fail;
    }

    return status;
}
