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
    irc_request *irc_req;
    bstring msg;

    msg = bformat("%s PRIVMSG loldrop :xdcc list\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Invalid address format", irc_req->op == LIST);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :xdcc list\r\n", dummy2);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Invalid address format", irc_req->op == LIST);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :xdcc list\r\n", dummy3);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Invalid address format", irc_req->op == LIST);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :                 xdcc list\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Many spaces after colon", irc_req->op == LIST);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
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
    irc_request *irc_req;
    bstring msg;

    msg = bformat("%s PRIVMSG loldrop :xdcc send #1\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == SEND);
    mu_assert("", irc_req->number == 1);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :xdcc send 1\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == SEND);
    mu_assert("", irc_req->number == 1);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :xdcc send 1+1\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == INVALID);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :xdcc send potatoes\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == INVALID);
    bdestroy(msg);
    free(irc_req);

    return NULL;
}

static
char *test_xdcc_info()
{
    irc_request *irc_req;
    bstring msg;

    msg = bformat("%s PRIVMSG loldrop :xdcc info #1\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == INFO);
    mu_assert("", irc_req->number == 1);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :xdcc info 1\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == INFO);
    mu_assert("", irc_req->number == 1);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :xdcc info 2+5\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == INVALID);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :xdcc info potatoes\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == INVALID);
    bdestroy(msg);
    free(irc_req);

    return NULL;
}

static
char *test_xdcc_remove()
{
    irc_request *irc_req;
    bstring msg;

    msg = bformat("%s PRIVMSG loldrop :xdcc remove #1\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == REMOVE);
    mu_assert("", irc_req->number == 1);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :xdcc remove 1\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == REMOVE);
    mu_assert("", irc_req->number == 1);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :xdcc remove\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == REMOVE);
    mu_assert("", irc_req->number == -1);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :xdcc remove potatoes\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == INVALID);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :xdcc remove -1\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == INVALID);
    bdestroy(msg);
    free(irc_req);

    return NULL;
}

static
char *test_ping_pong()
{
    irc_request *irc_req;
    bstring msg;

    msg = bfromcstr("PING :remoteserver.net\r\n");
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Valid PING", irc_req->op == PING);
    mu_assert("", strcmp("remoteserver.net", irc_req->remote_nick) == 0);
    bdestroy(msg);
    free(irc_req);

    msg = bfromcstr("PING remoteserver.net\r\n");
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Valid PING", irc_req->op == PING);
    mu_assert("", strcmp("remoteserver.net", irc_req->remote_nick) == 0);
    bdestroy(msg);
    free(irc_req);

    msg = bfromcstr("PING invalid@address\r\n");
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Invalid PING address", irc_req->op == INVALID);
    bdestroy(msg);
    free(irc_req);

    return NULL;
}

static
char *test_stupidity()
{
    irc_request *irc_req;
    bstring msg;

    irc_req = irc_parser(NULL);
    mu_assert("NULL message", irc_req == NULL);

    irc_req = irc_parser("");
    mu_assert("", irc_req != NULL);
    mu_assert("Empty message", irc_req->op == INVALID);
    free(irc_req);

    irc_req = irc_parser("omgwtfbbq\r\n");
    mu_assert("", irc_req != NULL);
    mu_assert("Absurd message", irc_req->op == INVALID);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :xdcc invalid #1337\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Invalid xdcc operation requested", irc_req->op == INVALID);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :Hello there, handsome!\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Invalid request/silent bot", irc_req->op == INVALID);
    bdestroy(msg);
    free(irc_req);

    return NULL;
}

static
char *test_admin()
{
    irc_request *irc_req;
    bstring msg;

    msg = bformat("%s PRIVMSG loldrop :admin 0x123456789 quit\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Valid quit admin command", irc_req->op == QUIT);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    free(irc_req);

    msg = bformat("%s PRIVMSG loldrop :xdcc admin 0x123456789 quit\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Stupid, invalid xdcc and admin mixup", irc_req->op == INVALID);
    bdestroy(msg);
    free(irc_req);

    return NULL;
}

static
char *all_tests() {
    mu_run_test(test_xdcc_list);
    mu_run_test(test_xdcc_send);
    mu_run_test(test_xdcc_info);
    mu_run_test(test_xdcc_remove);
    mu_run_test(test_ping_pong);
    mu_run_test(test_stupidity);
    mu_run_test(test_admin);
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
