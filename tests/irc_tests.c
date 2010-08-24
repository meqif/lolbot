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

bstring sent;

/* Override send in order to be able to intercept what would be sent */
ssize_t send(int s, const void *buffer, size_t length, int flags)
{
    sent = bfromcstr(buffer);
    return 0;
}

static
char *test_irc_user() {
    bstring expected;

    expected = bfromcstr("USER lolbot x x :lolbot\r\n");
    irc_user(SOCK, "lolbot", "lolbot");
    mu_assert("Empty message", blength(sent) != 0);
    mu_assert("Message differs from expected", bstrcmp(expected, sent) == 0);
    bdestroy(expected);
    bdestroy(sent);

    return NULL;
}

static
char *test_irc_nick() {
    bstring expected;

    expected = bfromcstr("NICK lolbot\r\n");
    irc_nick(SOCK, "lolbot");
    mu_assert("Empty message", blength(sent) != 0);
    mu_assert("Message differs from expected", bstrcmp(expected, sent) == 0);
    bdestroy(expected);
    bdestroy(sent);

    return NULL;
}

static
char *test_irc_join() {
    bstring expected;

    expected = bfromcstr("JOIN #lolbot\r\n");
    irc_join(SOCK, "lolbot");
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

RUN_TESTS(all_tests);

// vim: et ts=4 sw=4 sts=4
