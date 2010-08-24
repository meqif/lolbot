
#line 1 "src/state_machine.rl"
#include "state_machine.h"
#include "globals.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <bstring.h>

extern bstring bot_nickname;


#line 53 "src/state_machine.rl"



#line 19 "src/state_machine.c"
static const int irc_parser_start = 1;
static const int irc_parser_first_final = 68;
static const int irc_parser_error = 0;

static const int irc_parser_en_main = 1;


#line 56 "src/state_machine.rl"

irc_request *irc_parser(char *string)
{
    if (string == NULL)
        return NULL;

    int cs = 0;
    int len = strlen(string);
    const char *p = string;
    const char *pe = p+len;
    const char *eof = NULL;
    const char *mark = p;
    bstring botname = NULL, digits = NULL, remote_nick;
    irc_request *irc_req = calloc(1, sizeof(irc_request));
    irc_req->op = INVALID;

    
#line 45 "src/state_machine.c"
	{
	cs = irc_parser_start;
	}

#line 73 "src/state_machine.rl"

    pe = p + len;

    
#line 55 "src/state_machine.c"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 58: goto st2;
		case 80: goto st62;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr3;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr3;
	} else
		goto tr3;
	goto st0;
tr3:
#line 14 "src/state_machine.rl"
	{ mark = p; }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 91 "src/state_machine.c"
	if ( (*p) == 33 )
		goto tr4;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st3;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st3;
	} else
		goto st3;
	goto st0;
tr4:
#line 24 "src/state_machine.rl"
	{
        remote_nick = blk2bstr(mark, p-mark);
    }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 113 "src/state_machine.c"
	if ( (*p) == 126 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st6;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st6;
		} else if ( (*p) >= 64 )
			goto st6;
	} else
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 32 )
		goto st7;
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st6;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st6;
		} else if ( (*p) >= 64 )
			goto st6;
	} else
		goto st6;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 80 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 82 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 73 )
		goto st10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 86 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 77 )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 83 )
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 71 )
		goto st14;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 32 )
		goto st15;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr17;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr17;
	} else
		goto tr17;
	goto st0;
tr17:
#line 14 "src/state_machine.rl"
	{ mark = p; }
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 228 "src/state_machine.c"
	if ( (*p) == 32 )
		goto tr18;
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr18;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st16;
		} else if ( (*p) >= 65 )
			goto st16;
	} else
		goto st16;
	goto st0;
tr18:
#line 16 "src/state_machine.rl"
	{
        botname = blk2bstr(mark, p-mark);
    }
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 253 "src/state_machine.c"
	switch( (*p) ) {
		case 32: goto st17;
		case 58: goto st18;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st17;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	switch( (*p) ) {
		case 32: goto st18;
		case 97: goto st19;
		case 120: goto st40;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st18;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 100 )
		goto st20;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 109 )
		goto st21;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 105 )
		goto st22;
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 110 )
		goto st23;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 32 )
		goto st24;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st24;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	switch( (*p) ) {
		case 32: goto st24;
		case 48: goto st25;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st24;
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == 120 )
		goto st26;
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 49 )
		goto st27;
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 50 )
		goto st28;
	goto st0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 51 )
		goto st29;
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( (*p) == 52 )
		goto st30;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 53 )
		goto st31;
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( (*p) == 54 )
		goto st32;
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) == 55 )
		goto st33;
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	if ( (*p) == 56 )
		goto st34;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( (*p) == 57 )
		goto st35;
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( (*p) == 32 )
		goto st36;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st36;
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	switch( (*p) ) {
		case 32: goto st36;
		case 113: goto st37;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st36;
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) == 117 )
		goto st38;
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 105 )
		goto st39;
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 116 )
		goto tr44;
	goto st0;
tr80:
#line 24 "src/state_machine.rl"
	{
        remote_nick = blk2bstr(mark, p-mark);
    }
	goto st68;
tr44:
#line 47 "src/state_machine.rl"
	{ irc_req->op = QUIT; }
	goto st68;
tr61:
#line 40 "src/state_machine.rl"
	{ irc_req->op = LIST; }
	goto st68;
tr77:
#line 20 "src/state_machine.rl"
	{
        digits = blk2bstr(mark, p-mark);
    }
	goto st68;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
#line 456 "src/state_machine.c"
	if ( (*p) == 32 )
		goto st68;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st68;
	goto st0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	if ( (*p) == 100 )
		goto st41;
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 99 )
		goto st42;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( (*p) == 99 )
		goto st43;
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	if ( (*p) == 32 )
		goto st44;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st44;
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	switch( (*p) ) {
		case 32: goto st44;
		case 105: goto st45;
		case 108: goto st51;
		case 114: goto st54;
		case 115: goto st59;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st44;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( (*p) == 110 )
		goto st46;
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) == 102 )
		goto st47;
	goto st0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	if ( (*p) == 111 )
		goto tr55;
	goto st0;
tr55:
#line 34 "src/state_machine.rl"
	{ irc_req->op = INFO; }
	goto st48;
tr69:
#line 33 "src/state_machine.rl"
	{ irc_req->op = SEND; }
	goto st48;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
#line 539 "src/state_machine.c"
	if ( (*p) == 32 )
		goto st49;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st49;
	goto st0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	switch( (*p) ) {
		case 32: goto st49;
		case 35: goto st50;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr58;
	} else if ( (*p) >= 9 )
		goto st49;
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr58;
	goto st0;
tr58:
#line 14 "src/state_machine.rl"
	{ mark = p; }
	goto st69;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
#line 574 "src/state_machine.c"
	if ( (*p) == 32 )
		goto tr77;
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st69;
	} else if ( (*p) >= 9 )
		goto tr77;
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( (*p) == 105 )
		goto st52;
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	if ( (*p) == 115 )
		goto st53;
	goto st0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	if ( (*p) == 116 )
		goto tr61;
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	if ( (*p) == 101 )
		goto st55;
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	if ( (*p) == 109 )
		goto st56;
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	if ( (*p) == 111 )
		goto st57;
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	if ( (*p) == 118 )
		goto st58;
	goto st0;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	if ( (*p) == 101 )
		goto tr66;
	goto st0;
tr66:
#line 41 "src/state_machine.rl"
	{ irc_req->op = REMOVE; }
#line 35 "src/state_machine.rl"
	{ irc_req->op = REMOVE; }
	goto st70;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
#line 649 "src/state_machine.c"
	if ( (*p) == 32 )
		goto st71;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st71;
	goto st0;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	switch( (*p) ) {
		case 32: goto st71;
		case 35: goto st50;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr58;
	} else if ( (*p) >= 9 )
		goto st71;
	goto st0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	if ( (*p) == 101 )
		goto st60;
	goto st0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	if ( (*p) == 110 )
		goto st61;
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	if ( (*p) == 100 )
		goto tr69;
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) == 73 )
		goto st63;
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	if ( (*p) == 78 )
		goto st64;
	goto st0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	if ( (*p) == 71 )
		goto tr72;
	goto st0;
tr72:
#line 46 "src/state_machine.rl"
	{ irc_req->op = PING; }
	goto st65;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
#line 719 "src/state_machine.c"
	if ( (*p) == 32 )
		goto st66;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st66;
	goto st0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	switch( (*p) ) {
		case 32: goto st66;
		case 46: goto tr74;
		case 58: goto st67;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st66;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr74;
		} else if ( (*p) >= 65 )
			goto tr74;
	} else
		goto tr74;
	goto st0;
tr74:
#line 14 "src/state_machine.rl"
	{ mark = p; }
	goto st72;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
#line 754 "src/state_machine.c"
	switch( (*p) ) {
		case 32: goto tr80;
		case 46: goto st72;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr80;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st72;
		} else if ( (*p) >= 65 )
			goto st72;
	} else
		goto st72;
	goto st0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	if ( (*p) == 46 )
		goto tr74;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr74;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr74;
	} else
		goto tr74;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 69: 
#line 20 "src/state_machine.rl"
	{
        digits = blk2bstr(mark, p-mark);
    }
	break;
	case 72: 
#line 24 "src/state_machine.rl"
	{
        remote_nick = blk2bstr(mark, p-mark);
    }
	break;
#line 875 "src/state_machine.c"
	}
	}

	_out: {}
	}

#line 77 "src/state_machine.rl"

    if (botname && bstrcmp(botname, bot_nickname) != 0) {
        bdestroy(botname);
        irc_req->op = INVALID;
        return irc_req;
    }
    bdestroy(botname);

    if ( cs < 
#line 892 "src/state_machine.c"
68
#line 85 "src/state_machine.rl"
 ) {
        irc_req->op = INVALID;
        return irc_req;
    }

    irc_req->remote_nick = bdata(remote_nick);

    if (digits) {
        irc_req->number = atoi(bdata(digits));
        bdestroy(digits);
    } else
        irc_req->number = -1;

    return irc_req;
}

