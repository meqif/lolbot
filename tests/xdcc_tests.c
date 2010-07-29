#include "minunit.h"
#include <xdcc.h>
#include <stdlib.h>
#include <stdio.h>

int tests_run = 0;
int assertion = 0;
const char *last_test = "";
int line = 0;
int strdup_fail = 0;

static
char *test_xdcc_list()
{
    char *response;

    response = xdcc_process("xdcc list");
    mu_assert("xdcc list", response != NULL);

    response = xdcc_process("xdcc list somefile.txt");
    mu_assert("xdcc list somefile.txt", response == NULL);

    return NULL;
}

static
char *test_xdcc_send()
{
    char *response;

    response = xdcc_process("xdcc send #1");
    mu_assert("xdcc send #number", response != NULL);

    response = xdcc_process("xdcc send");
    mu_assert("xdcc send", response == NULL);

    return NULL;
}

static
char *test_xdcc_info()
{
    char *response;

    response = xdcc_process("xdcc info #1");
    mu_assert("xdcc info #number", response != NULL);

    response = xdcc_process("xdcc info");
    mu_assert("xdcc info", response == NULL);

    return NULL;
}

static
char *test_xdcc_remove()
{
    char *response;

    response = xdcc_process("xdcc remove #1");
    mu_assert("xdcc remove #number", response != NULL);

    response = xdcc_process("xdcc remove");
    mu_assert("xdcc remove", response != NULL);

    return NULL;
}

static
char *all_tests() {
    mu_run_test(test_xdcc_list);
    mu_run_test(test_xdcc_send);
    mu_run_test(test_xdcc_info);
    mu_run_test(test_xdcc_remove);
    return NULL;
}

int main(int argc, char *argv[])
{
    char *result = all_tests();
    if (result != 0) {
        printf("Failed: %s:%d\n", last_test, line);
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
        printf("  Tests run : %d\n", tests_run);
        printf("  Assertion : %d\n", assertion);
    }

    return result != 0;
}


// vim: et ts=4 sw=4 sts=4
