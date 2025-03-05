/**
 * @file mod_test.c
 * @author ruslan (valeev.rl@phystech.edu)
 * @brief 
 * @version 0.1
 * @date 2025-03-05
 * @note based on https://github.com/BBazard/cunit-example
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <CUnit/Basic.h>
#include "../src/func.h"


int init_suite_func(void)
{  return 0;  }

int clean_suite_func(void)
{  return 0;  }

/**
 * @brief test for mod()
 */
void testmod()
{
    /* positive integer cases */
    CU_ASSERT_EQUAL(mod(5, 4), 1);
    CU_ASSERT_EQUAL(mod(3, 1), 0);
}


int main()
{
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
    {  return CU_get_error();  }

    pSuite = CU_add_suite("Suite func", init_suite_func, clean_suite_func);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "test of simpleDivision() in classic cases", testmod))) 
    {
        CU_cleanup_registry();
        return CU_get_error();
    }


    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
