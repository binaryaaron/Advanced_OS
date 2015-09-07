#include "minunit.h"
#include "dbg.h"
#include <dlfcn.h>

typedef int (*lib_function)(const char *data);
char *lib_file = "build/libunmthread.so";
void *lib = NULL;


char *test_failures()
{
    mu_assert(check_function("fail_on_purpose", "Hello", 1), "fail_on_purpose should fail.");
    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_failures);

    return NULL;
}

RUN_TESTS(all_tests);
