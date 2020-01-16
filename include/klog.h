#ifndef KLOG_H

#define KLOG_H

#define stringize(x) _stringize(x)
#define _stringize(x) #x

// log level:
//     0: fatal (halt)
//     1: warning
//     2: info
//     3: debug
#define KMSG_COLOR 0x74

#define _kmsg(msg, level) \
    do { \
        disp_color_str("[kernel] " level ": ", KMSG_COLOR); \
        disp_color_str(msg, KMSG_COLOR); \
        disp_color_str("\n", KMSG_COLOR); \
    } while (0)

#define _kmsg_halt(msg, level) \
    do { \
        disp_color_str("[kernel] " level ": ", KMSG_COLOR); \
        disp_color_str(msg, KMSG_COLOR); \
        disp_color_str("\n", KMSG_COLOR); \
        while (1); \
    } while (0) // TODO halt system

#define kdebug(msg) \
    _kmsg(msg, "debug")

#define kfatal(msg) \
    _kmsg_halt(msg, "fatal")

// fatal error if not succeed
#define kassert(cond) \
    kassertm(cond, #cond)

// fatal error if not succeed
#define kassertm(cond, msg) \
    do { \
        if (!(cond)) { \
            _kmsg_halt(msg, \
                    "assert failed (in " \
                    stringize(__FILE__) ":" stringize(__LINE__) \
                    ")"); \
        } \
    } while (0)

#endif /* end of include guard: KLOG_H */
