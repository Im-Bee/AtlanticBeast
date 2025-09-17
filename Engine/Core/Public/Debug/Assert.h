#ifndef AB_DEBUG_ASSERT_H
#define AB_DEBUG_ASSERT_H

#ifdef _DEBUG
#   define AB_ASSERT(expr)                                                          \
        do { if (!(expr)) {                                                           \
            throw AB_EXCEPT("Assertion failed! ('" #expr  "').");                   \
        } } while (0)                                                               

#   define AB_ASSERT_MSG(expr, msg)                                                 \
        do { if (!(expr)) {                                                           \
            throw AB_EXCEPT("Assertion failed! ('" #expr  "'). " msg);              \
        } } while (0)                                                               
#else
#   define AB_ASSERT(expr)
#   define AB_ASSERT_MSG(expr, msg)
#endif

#endif // !AB_DEBUG_ASSERT_H
