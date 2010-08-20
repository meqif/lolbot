#include "minunit.h"
#include <globals.h>
#include <state_machine.h>
#include <stdlib.h>
#include <stdio.h>
#include <bstring.h>

int tests_run = 0;
int assertion = 0;
const char *last_test = "";
int line = 0;
int strdup_fail = 0;

static char *dummy  = ":nick!~user@127.0.0.1";
static char *dummy2 = ":nick!~user@localhost.com";
static char *dummy3 = ":nick!~user@127.0.0.1.localhost.com";

static
char *test_xdcc_list()
{
    struct irc_request *irc_req;
    bstring msg;

    msg = bformat("%s PRIVMSG loldrop :xdcc list\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Invalid address format", irc_req->op == LIST);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :xdcc list\r\n", dummy2);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Invalid address format", irc_req->op == LIST);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :xdcc list\r\n", dummy3);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Invalid address format", irc_req->op == LIST);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :                 xdcc list\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Many spaces after colon", irc_req->op == LIST);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop:xdcc list\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Missing space before colon", irc_req->op == INVALID);
    bdestroy(msg);
    free(irc_req);

    return NULL;
}

static
char *test_xdcc_send()
{
    struct irc_request *irc_req;
    bstring msg;

    msg = bformat("%s PRIVMSG loldrop :xdcc send #1\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == SEND);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :xdcc send 1\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == SEND);
    bdestroy(msg);
    free(irc_req);

    return NULL;
}

static
char *test_stupidity()
{
    struct irc_request *irc_req;
    bstring msg;

    msg = bfromcstr("omgwtfbbq\r\n");
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Absurd message", irc_req->op == INVALID);
    bdestroy(msg);
    free(irc_req);

    return NULL;
}

static
char *all_tests() {
    mu_run_test(test_xdcc_list);
    mu_run_test(test_xdcc_send);
    mu_run_test(test_stupidity);
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
