#include "minunit.h"
#include <globals.h>
#include <state_machine.h>
#include <irc_request.h>
#include <stdlib.h>
#include <stdio.h>
#include <bstring.h>

int tests_run = 0;
int assertion = 0;
const char *last_test = "";
int line = 0;
extern bstring bot_nickname;
static char *dummy  = ":nick!~user@127.0.0.1";
static char *dummy2 = ":nick!~user@localhost.com";
static char *dummy3 = ":nick!~user@127.0.0.1.localhost.com";

static
char *test_xdcc_list()
{
    irc_request *irc_req;
    bstring msg;

    msg = bformat("%s PRIVMSG lolbot :xdcc list\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Invalid address format", irc_req->op == LIST);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    msg = bformat("%s PRIVMSG otherbot :xdcc list\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Message to another nick, shouldn't care", irc_req->op != LIST);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    char *botname = "zomgbot";
    bdestroy(bot_nickname);
    bot_nickname = bfromcstr(botname);
    msg = bformat("%s PRIVMSG %s :xdcc list\r\n", dummy, botname);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Bot nick and nick in message not equal", irc_req->op == LIST);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    IrcRequest_free(irc_req);
    bdestroy(bot_nickname);
    bot_nickname = bfromcstr("lolbot");

    msg = bformat("%s PRIVMSG lolbot :xdcc list\r\n", dummy2);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Invalid address format", irc_req->op == LIST);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    msg = bformat("%s PRIVMSG lolbot :xdcc list\r\n", dummy3);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Invalid address format", irc_req->op == LIST);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    msg = bformat("%s PRIVMSG lolbot :                 xdcc list\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Many spaces after colon", irc_req->op == LIST);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    msg = bformat("%s PRIVMSG lolbot:xdcc list\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Missing space before colon", irc_req->op == INVALID);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    return NULL;
}

static
char *test_xdcc_send()
{
    irc_request *irc_req;
    bstring msg;

    msg = bformat("%s PRIVMSG lolbot :xdcc send #1\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == SEND);
    mu_assert("", irc_req->number == 1);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    msg = bformat("%s PRIVMSG lolbot :xdcc send 1\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == SEND);
    mu_assert("", irc_req->number == 1);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    msg = bformat("%s PRIVMSG lolbot :xdcc send 1+1\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == INVALID);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    msg = bformat("%s PRIVMSG lolbot :xdcc send potatoes\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == INVALID);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    return NULL;
}

static
char *test_xdcc_info()
{
    irc_request *irc_req;
    bstring msg;

    msg = bformat("%s PRIVMSG lolbot :xdcc info #1\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == INFO);
    mu_assert("", irc_req->number == 1);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    msg = bformat("%s PRIVMSG lolbot :xdcc info 1\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == INFO);
    mu_assert("", irc_req->number == 1);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    msg = bformat("%s PRIVMSG lolbot :xdcc info 2+5\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == INVALID);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    msg = bformat("%s PRIVMSG lolbot :xdcc info potatoes\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == INVALID);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    return NULL;
}

static
char *test_xdcc_remove()
{
    irc_request *irc_req;
    bstring msg;

    msg = bformat("%s PRIVMSG lolbot :xdcc remove #1\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == REMOVE);
    mu_assert("", irc_req->number == 1);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    msg = bformat("%s PRIVMSG lolbot :xdcc remove 1\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == REMOVE);
    mu_assert("", irc_req->number == 1);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    msg = bformat("%s PRIVMSG lolbot :xdcc remove\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == REMOVE);
    mu_assert("", irc_req->number == -1);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    msg = bformat("%s PRIVMSG lolbot :xdcc remove potatoes\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == INVALID);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    msg = bformat("%s PRIVMSG lolbot :xdcc remove -1\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("", irc_req->op == INVALID);
    bdestroy(msg);
    IrcRequest_free(irc_req);

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
    IrcRequest_free(irc_req);

    msg = bfromcstr("PING remoteserver.net\r\n");
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Valid PING", irc_req->op == PING);
    mu_assert("", strcmp("remoteserver.net", irc_req->remote_nick) == 0);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    msg = bfromcstr("PING invalid@address\r\n");
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Invalid PING address", irc_req->op == INVALID);
    bdestroy(msg);
    IrcRequest_free(irc_req);

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
    IrcRequest_free(irc_req);

    irc_req = irc_parser("omgwtfbbq\r\n");
    mu_assert("", irc_req != NULL);
    mu_assert("Absurd message", irc_req->op == INVALID);
    IrcRequest_free(irc_req);

    msg = bformat("%s PRIVMSG lolbot :xdcc invalid #1337\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Invalid xdcc operation requested", irc_req->op == INVALID);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    msg = bformat("%s PRIVMSG lolbot :Hello there, handsome!\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Invalid request/silent bot", irc_req->op == INVALID);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    return NULL;
}

static
char *test_admin()
{
    irc_request *irc_req;
    bstring msg;

    msg = bformat("%s PRIVMSG lolbot :admin 0x123456789 quit\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Valid quit admin command", irc_req->op == QUIT);
    mu_assert("", strcmp("nick", irc_req->remote_nick) == 0);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    msg = bformat("%s PRIVMSG lolbot :xdcc admin 0x123456789 quit\r\n", dummy);
    irc_req = irc_parser(bdata(msg));
    mu_assert("", irc_req != NULL);
    mu_assert("Stupid, invalid xdcc and admin mixup", irc_req->op == INVALID);
    bdestroy(msg);
    IrcRequest_free(irc_req);

    return NULL;
}

static
char *all_tests() {
    bot_nickname = bfromcstr("lolbot");

    mu_run_test(test_xdcc_list);
    mu_run_test(test_xdcc_send);
    mu_run_test(test_xdcc_info);
    mu_run_test(test_xdcc_remove);
    mu_run_test(test_ping_pong);
    mu_run_test(test_stupidity);
    mu_run_test(test_admin);

    bdestroy(bot_nickname);
    return NULL;
}

RUN_TESTS(all_tests);

// vim: et ts=4 sw=4 sts=4
