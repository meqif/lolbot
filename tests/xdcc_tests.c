#include "minunit.h"
#include <globals.h>
#include <xdcc.h>
#include <stdlib.h>
#include <stdio.h>
#include <bstring.h>

int tests_run = 0;
int assertion = 0;
const char *last_test = "";
int line = 0;

static
char *test_ipv4_conversion()
{
    bstring strIP;
    int ip;

    strIP = bfromcstr("8.8.8.8");
    ip = (8 | 8 << 8 | 8 << 16 | 8 << 24);
    mu_assert("", ipv4_str_to_int(bdata(strIP)) == ip);
    bdestroy(strIP);

    strIP = bfromcstr("127.0.0.1");
    ip = (127 | 0 << 8 | 0 << 16 | 1 << 24);
    mu_assert("", ipv4_str_to_int(bdata(strIP)) == ip);
    bdestroy(strIP);

    strIP = bfromcstr("192.168.2.1");
    ip = (192 | 168 << 8 | 2 << 16 | 1 << 24);
    mu_assert("", ipv4_str_to_int(bdata(strIP)) == ip);
    bdestroy(strIP);

    return NULL;
}

static
char *all_tests() {
    mu_run_test(test_ipv4_conversion);
    return NULL;
}

RUN_TESTS(all_tests);

// vim: et ts=4 sw=4 sts=4
