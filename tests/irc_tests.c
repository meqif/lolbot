#include "minunit.h"
#include <stdio.h>
#include <globals.h>
#include <irc.h>
#include <bstring.h>

#define PORT "6667"
#define SOCK 0

int tests_run = 0;
int assertion = 0;
const char *last_test = "";
int line = 0;
int strdup_fail = 0;

bstring sent;

/* Override send in order to be able to intercept what would be sent */
int send(int socket, const void *buffer, size_t length, int flags)
{
    sent = bfromcstr(buffer);
    return 0;
}

static
char *test_irc_user() {
    bstring expected;

    expected = bfromcstr("USER loldrop x x :loldrop\r\n");
    irc_user(SOCK, "loldrop", "loldrop");
    mu_assert("Empty message", blength(sent) != 0);
    mu_assert("Message differs from expected", bstrcmp(expected, sent) == 0);
    bdestroy(expected);
    bdestroy(sent);

    return NULL;
}

static
char *test_irc_nick() {
    bstring expected;

    expected = bfromcstr("NICK loldrop\r\n");
    irc_nick(SOCK, "loldrop");
    mu_assert("Empty message", blength(sent) != 0);
    mu_assert("Message differs from expected", bstrcmp(expected, sent) == 0);
    bdestroy(expected);
    bdestroy(sent);

    return NULL;
}

static
char *test_irc_join() {
    bstring expected;

    expected = bfromcstr("JOIN #loldrop\r\n");
    irc_join(SOCK, "loldrop");
    mu_assert("Empty message", blength(sent) != 0);
    mu_assert("Message differs from expected", bstrcmp(expected, sent) == 0);
    bdestroy(expected);
    bdestroy(sent);

    return NULL;
}

static
char *test_irc_quit() {
    bstring expected;

    expected = bfromcstr("QUIT\r\n");
    irc_quit(0);
    mu_assert("Empty message", blength(sent) != 0);
    mu_assert("Message differs from expected", bstrcmp(expected, sent) == 0);
    bdestroy(expected);
    bdestroy(sent);

    return NULL;
}

static
char *test_irc_pong() {
    bstring expected;

    expected = bfromcstr("PONG server.somewhere.net\r\n");
    irc_pong(SOCK, "server.somewhere.net");
    mu_assert("Empty message", blength(sent) != 0);
    mu_assert("Message differs from expected", bstrcmp(expected, sent) == 0);
    bdestroy(expected);
    bdestroy(sent);

    return NULL;
}

static
char *test_irc_privmsg() {
    bstring expected;

    expected = bfromcstr("PRIVMSG someBloke :Message for someBloke!\r\n");
    irc_privmsg(SOCK, "someBloke", "Message for someBloke!");
    mu_assert("Empty message", blength(sent) != 0);
    mu_assert("Message differs from expected", bstrcmp(expected, sent) == 0);
    bdestroy(expected);
    bdestroy(sent);

    return NULL;
}

static
char *test_irc_notice() {
    bstring expected;

    expected = bfromcstr("NOTICE someBloke :Message for someBloke!\r\n");
    irc_notice(SOCK, "someBloke", "Message for someBloke!");
    mu_assert("Empty message", blength(sent) != 0);
    mu_assert("Message differs from expected", bstrcmp(expected, sent) == 0);
    bdestroy(expected);
    bdestroy(sent);

    return NULL;
}

static
char *test_irc_dcc_send() {
    bstring expected;

    expected = bfromcstr("PRIVMSG someBloke :\01DCC SEND somefile.ext 92929292 8888 123456789\01\r\n");
    irc_dcc_send(SOCK, "someBloke", "somefile.ext", 123456789, 92929292, 8888);
    mu_assert("Empty message", blength(sent) != 0);
    mu_assert("Message differs from expected", bstrcmp(expected, sent) == 0);
    bdestroy(expected);
    bdestroy(sent);

    return NULL;
}

static
char *test_irc_dcc_accept() {
    bstring expected;

    expected = bfromcstr("PRIVMSG someBloke :\01DCC ACCEPT somefile.ext 8888 123456789\01\r\n");
    irc_dcc_accept(SOCK, "someBloke", "somefile.ext", 8888, 123456789);
    mu_assert("Empty message", blength(sent) != 0);
    mu_assert("Message differs from expected", bstrcmp(expected, sent) == 0);
    bdestroy(expected);
    bdestroy(sent);

    return NULL;
}

static
char *all_tests() {
    mu_run_test(test_irc_user);
    mu_run_test(test_irc_nick);
    mu_run_test(test_irc_join);
    mu_run_test(test_irc_quit);
    mu_run_test(test_irc_pong);
    mu_run_test(test_irc_privmsg);
    mu_run_test(test_irc_notice);
    mu_run_test(test_irc_dcc_send);
    mu_run_test(test_irc_dcc_accept);
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
