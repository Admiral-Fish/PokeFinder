#include <QTest>
#include <RNG/LCRNG64Test.hpp>
#include <RNG/LCRNGTest.hpp>
#include <RNG/MTTest.hpp>
#include <RNG/RNGCacheTest.hpp>
#include <RNG/RNGEuclideanTest.hpp>
#include <RNG/SFMTTest.hpp>
#include <RNG/SHA1Test.hpp>
#include <RNG/TinyMTTest.hpp>

// Register types to meta system
#include <Core/Enum/Method.hpp>
#include <Core/Gen5/Profile5.hpp>
Q_DECLARE_METATYPE(Method)
Q_DECLARE_METATYPE(Profile5)

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
