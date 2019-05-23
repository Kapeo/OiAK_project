#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello
#include <boost/test/unit_test.hpp>
#include "my_fixedpoint.h"


BOOST_AUTO_TEST_CASE( test0001)
{
	myfixedpoint mf1(10264,64);
	myfixedpoint mf2(10264,64);
	mf1.setValue("0.1");
	mf2.setValue("0.1");
	myfixedpoint mf3(10264,64);
	mf3.setValue("0.2");
	
	BOOST_CHECK( mf1+mf2 == mf3);

}
BOOST_AUTO_TEST_CASE( test0002)
{
	myfixedpoint mf1(10264,64);
	myfixedpoint mf2(10264,64);
	mf1.setValue("0.1");
	mf2.setValue("0.1");
	myfixedpoint mf3(10264,64);
	mf3.setValue("0.01");
	
	BOOST_CHECK( mf1*mf2 == mf3);

}
BOOST_AUTO_TEST_CASE( test0003)
{
	myfixedpoint mf1(10264,64);
	myfixedpoint mf2(10264,64);
	mf1.setValue("0.1");
	mf2.setValue("0.1");
	myfixedpoint mf3(10264,64);
	mf3.setValue("1");
	
	BOOST_CHECK( mf1/mf2 == mf3);

}
BOOST_AUTO_TEST_CASE( test0004)
{
	myfixedpoint mf1(10264,64);
	myfixedpoint mf2(10264,64);
	mf1.setValue("0.1");
	mf2.setValue("0.1");
	myfixedpoint mf3(10264,64);
	mf3.setValue("0");
	
	BOOST_CHECK( mf1-mf2 == mf3);

}


//TEST DLA LICZB WYMIERNYCH
BOOST_AUTO_TEST_CASE( test001)
{
	myfixedpoint mf1(10264,64);
	myfixedpoint mf2(10264,64);
	mf1.setValue("100");
	mf2.setValue("0.1");
	myfixedpoint mf3(10264,64);
	mf3.setValue("10");
	
	BOOST_CHECK( mf1*mf2 == mf3);

}

BOOST_AUTO_TEST_CASE( test002)
{
	myfixedpoint mf1(10264,64);
	myfixedpoint mf2(10264,64);
	mf1.setValue("100");
	mf2.setValue("0.1");
	myfixedpoint mf3(10264,64);
	mf3.setValue("100.1");
	
	BOOST_CHECK( mf1+mf2 == mf3);

}

BOOST_AUTO_TEST_CASE( test003)
{
	myfixedpoint mf1(10264,64);
	myfixedpoint mf2(10264,64);
	mf1.setValue("100");
	mf2.setValue("0.1");
	myfixedpoint mf3(10264,64);
	mf3.setValue("99.9");
	
	BOOST_CHECK( mf1-mf2 == mf3);

}

BOOST_AUTO_TEST_CASE( test004)
{
	myfixedpoint mf1(10264,64);
	myfixedpoint mf2(10264,64);
	mf1.setValue("100");
	mf2.setValue("0.1");
	myfixedpoint mf3(10264,64);
	mf3.setValue("1000");
	
	BOOST_CHECK( mf1/mf2 == mf3);

}





BOOST_AUTO_TEST_CASE( test0)
{
	myfixedpoint mf1(10264,64);
	myfixedpoint mf2(10264,64);
	mf1.setValue("123456789123456789123456789333.913123123");
	mf2.setValue("1829310125812591247124912491.31239129912");
	myfixedpoint mf3(10264,64);
	mf3.setValue("125286099249269380370581701825.2255144221199999999237506553839693879126571118831634521484375");
	
	BOOST_CHECK( mf1+mf2 == mf3);

}
BOOST_AUTO_TEST_CASE( test01)
{
	myfixedpoint mf1(10264,64);
	myfixedpoint mf2(10264,64);
	mf1.setValue("123456789123456789123456789333.913123123");
	mf2.setValue("1829310125812591247124912491.31239129912");
	myfixedpoint mf3(10264,64);
	mf3.setValue("121627478997644197876331876842.60073182388000000001861133913649837268167175352573394775390625");
	
	BOOST_CHECK( mf1-mf2 == mf3);

}
BOOST_AUTO_TEST_CASE( test02)
{
	myfixedpoint mf1(10264,64);
	myfixedpoint mf2(10264,64);
	mf1.setValue("123456789123456789123456789333.913123123");
	mf2.setValue("1829310125812591247124912491.31239129912");
	myfixedpoint mf3(10264,64);
	mf3.setValue("225840754443849285583396464051804592194469530983296659244.4897871667512451909938520078213741726358421146869659423828125");
	
	BOOST_CHECK( mf1*mf2 == mf3);

}
BOOST_AUTO_TEST_CASE( test03)
{
	myfixedpoint mf1(10264,64);
	myfixedpoint mf2(10264,64);
	mf1.setValue("123456789123456789123456789333.913123123");
	mf2.setValue("1829310125812591247124912491.31239129912");
	myfixedpoint mf3(10264,64);
	mf3.setValue("67.4881680155881138576394635830713042423667502589523792266845703125");
	
	BOOST_CHECK( mf1/mf2 == mf3);

}

BOOST_AUTO_TEST_CASE( test1)
{
	myfixedpoint mf1(10264,64);
	myfixedpoint mf2(10264,64);
	mf1.setValue("11111.111");
	mf2.setValue("11111.111");
	myfixedpoint mf3(10264,64);
	mf3.setValue("22222.221999999999999999959233998314545033281319774687290191650390625");
	
	BOOST_CHECK( mf1+mf2 == mf3);

}
BOOST_AUTO_TEST_CASE( test2)
{
	myfixedpoint mf1(10264,64);
	myfixedpoint mf2(10264,64);
	mf1.setValue("11111.111");
	mf2.setValue("11111.111");
	myfixedpoint mf3(10264,64);
	mf3.setValue("123456787.6543209999999995470239787243027507201986736617982387542724609375");
	
	BOOST_CHECK( mf1*mf2 == mf3);

}

BOOST_AUTO_TEST_CASE( test3)
{
	myfixedpoint mf1(10264,64);
	myfixedpoint mf2(10264,64);
	mf1.setValue("11111.111");
	mf2.setValue("11111.111");
	myfixedpoint mf3(10264,64);
	mf3.setValue("0.0");
	
	BOOST_CHECK( mf1-mf2 == mf3);

}
BOOST_AUTO_TEST_CASE( test4)
{
	myfixedpoint mf1(10264,64);
	myfixedpoint mf2(10264,64);
	mf1.setValue("11111.111");
	mf2.setValue("11111.111");
	myfixedpoint mf3(10264,64);
	mf3.setValue("1.0");
	
	BOOST_CHECK( mf1/mf2 == mf3);

}

