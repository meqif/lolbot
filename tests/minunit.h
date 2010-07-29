/* http://www.jera.com/techinfo/jtns/jtn002.html */

#define xstr(s) str(s)
#define str(s) #s

#define mu_assert(message, test) do { \
    if (!(test)) \
        return __FILE__ ":" xstr(__LINE__) " " message " (" #test ")"; \
    else \
        assertion++; \
} while (0)

#define mu_run_test(test) do { \
    char *message; \
    last_test = #test; \
    line = __LINE__; \
    message = test(); \
    tests_run++; \
    if (message) \
        return message; \
} while (0)

#define mu_numelem(array) (sizeof(array)/sizeof(*array))

extern int tests_run;
extern int assertion;
extern const char *last_test;
extern int line;

