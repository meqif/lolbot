#include <stdio.h>
#include <string.h>
#include <stdlib.h>

%%{
	machine clang;

	whitespace = space+;

	main := "xdcc" whitespace ("send"|"info") whitespace "#"? digit+ whitespace*;
}%%

%% write data;

#define BUFSIZE 128

void process(char *string, int len)
{
	int cs;
	char *p = string, *pe;

	%% write init;

	pe = p + len;

	%% write exec;

	if ( cs < %%{ write first_final; }%% ) {
		fprintf(stderr, "PARSE ERROR\n" );
		return;
	}

	printf("Received: \"%s\"\n", string);
}

void scanner()
{
	static char buf[BUFSIZE];
	int len, space = BUFSIZE;

	len = fread(buf, 1, space, stdin );

	process(buf, len);
}
