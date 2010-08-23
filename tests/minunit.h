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

#define RUN_TESTS(name) int main(int argc, char *argv[]) {\
    char *result = name(); \
    if (result != 0) { \
        printf("Failed: %s:%d\n", last_test, line); \
        printf("%s\n", result); \
    } else { \
        printf("ALL TESTS PASSED\n"); \
        printf("  Tests run : %d\n", tests_run); \
        printf("  Assertion : %d\n", assertion); \
    } \
    return result != 0; \
}

extern int tests_run;
extern int assertion;
extern const char *last_test;
extern int line;

