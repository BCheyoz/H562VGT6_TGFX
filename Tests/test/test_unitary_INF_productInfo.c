#ifdef TEST

#include "unity.h"

#include "calc.h"

void setUp(void)
{
}

void tearDown(void)
{
}

/**********************************************************************
* 								tests
***********************************************************************/

void test_calc(void)
{
    TEST_ASSERT_EQUAL_INT(2, calc(1));
    TEST_ASSERT_EQUAL_INT(8, calc(4));

}

#endif // TEST
