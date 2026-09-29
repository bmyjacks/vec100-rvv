/****************************************************************************
 *
 *
 *  Project: stable-diffusion.cpp (revision c92d73c)
 *  Source files:
 *    ggml/src/ggml-cpu/vec.h
 *    ggml/src/ggml-cpu/ops.cpp
 *    ggml/src/ggml.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * ggml/src/ggml-cpu/vec.h
 *
 *   Vectorized functions for fundamental operations
 *
 * License origin: ggml/LICENSE
 *
 * MIT License
 *
 * Copyright (c) 2023-2026 The ggml authors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 * ggml/src/ggml-cpu/ops.cpp
 *
 * License origin: ggml/LICENSE
 *
 * MIT License
 *
 * Copyright (c) 2023-2026 The ggml authors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 * ggml/src/ggml.c
 *
 * License origin: ggml/LICENSE
 *
 * MIT License
 *
 * Copyright (c) 2023-2026 The ggml authors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "kernel.h"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

/*
 * ggml/src/ggml.c:40-44
 */
#if defined(__APPLE__)
#include <TargetConditionals.h>
#include <mach/mach.h>
#include <unistd.h>
#endif
/*
 * ggml/src/ggml.c:81-96
 */
#if defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) ||       \
    defined(__OpenBSD__) ||                                                    \
    (defined(__APPLE__) && !TARGET_OS_TV && !TARGET_OS_WATCH)
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#if defined(__linux__)
#include <sys/prctl.h>
#endif
#if defined(__ANDROID__)
#include <dlfcn.h>
#include <stdio.h>
#include <unwind.h>
#endif
#endif

/* ggml/src/ggml-cpu/vec.h:703-704,763-767 (scalar configuration) */
inline static void ggml_vec_scale_f32(const int n, float *y, const float v) {
    for (int i = 0; i < n; ++i) {
        y[i] *= v;
    }
}

/*
 * ggml/src/ggml-cpu/ops.cpp:4120-4193
 */
static void
ggml_compute_forward_group_norm_f32(const ggml_compute_params *params,
                                    ggml_tensor *dst) {
    const ggml_tensor *src0 = dst->src[0];

    GGML_ASSERT(ggml_are_same_shape(src0, dst));

    GGML_ASSERT(src0->nb[0] == sizeof(float));

    const int ith = params->ith;
    const int nth = params->nth;

    GGML_TENSOR_UNARY_OP_LOCALS

    float eps;
    memcpy(&eps, dst->op_params + 1, sizeof(float));

    int n_channels = src0->ne[2];
    int n_groups = dst->op_params[0];
    int n_channels_per_group = (n_channels + n_groups - 1) / n_groups;
    for (int i = ith; i < n_groups; i += nth) {
        int start = i * n_channels_per_group;
        int end = start + n_channels_per_group;
        if (end > n_channels) {
            end = n_channels;
        }
        int step = end - start;

        for (int64_t i03 = 0; i03 < ne03; i03++) {
            ggml_float sum = 0.0;
            for (int64_t i02 = start; i02 < end; i02++) {
                for (int64_t i01 = 0; i01 < ne01; i01++) {
                    const float *x = (float *)((char *)src0->data + i01 * nb01 +
                                               i02 * nb02 + i03 * nb03);

                    ggml_float sumr = 0.0;
                    for (int64_t i00 = 0; i00 < ne00; i00++) {
                        sumr += (ggml_float)x[i00];
                    }
                    sum += sumr;
                }
            }
            const float mean = sum / (ne00 * ne01 * step);

            ggml_float sum2 = 0.0;
            for (int64_t i02 = start; i02 < end; i02++) {
                for (int64_t i01 = 0; i01 < ne01; i01++) {
                    const float *x = (float *)((char *)src0->data + i01 * nb01 +
                                               i02 * nb02 + i03 * nb03);

                    float *y = (float *)((char *)dst->data + i01 * nb1 +
                                         i02 * nb2 + i03 * nb3);

                    ggml_float sumr = 0.0;
                    for (int64_t i00 = 0; i00 < ne00; i00++) {
                        float v = x[i00] - mean;
                        y[i00] = v;
                        sumr += (ggml_float)(v * v);
                    }
                    sum2 += sumr;
                }
            }
            const float variance = sum2 / (ne00 * ne01 * step);
            const float scale = 1.0f / sqrtf(variance + eps);

            for (int64_t i02 = start; i02 < end; i02++) {
                for (int64_t i01 = 0; i01 < ne01; i01++) {
                    float *y = (float *)((char *)dst->data + i01 * nb1 +
                                         i02 * nb2 + i03 * nb3);
                    ggml_vec_scale_f32(ne00, y, scale);
                }
            }
        }
    }
}

/*
 * ggml/src/ggml.c:98-114
 */
#if defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) ||       \
    defined(__OpenBSD__) ||                                                    \
    (defined(__APPLE__) && !TARGET_OS_TV && !TARGET_OS_WATCH)
#if defined(__ANDROID__)
struct backtrace_state {
    void **current;
    void **end;
};

static _Unwind_Reason_Code unwind_callback(struct _Unwind_Context *context,
                                           void *arg) {
    struct backtrace_state *state = (struct backtrace_state *)arg;
    uintptr_t pc = _Unwind_GetIP(context);
    if (pc) {
        if (state->current == state->end) {
            return _URC_END_OF_STACK;
        } else {
            *state->current++ = (void *)pc;
        }
    }
    return _URC_NO_REASON;
}

/*
 * ggml/src/ggml.c:116-136
 */
static void ggml_print_backtrace_symbols(void) {
    const int max = 100;
    void *buffer[max];

    struct backtrace_state state = {buffer, buffer + max};
    _Unwind_Backtrace(unwind_callback, &state);

    int count = state.current - buffer;

    for (int idx = 0; idx < count; ++idx) {
        const void *addr = buffer[idx];
        const char *symbol = "";

        Dl_info info;
        if (dladdr(addr, &info) && info.dli_sname) {
            symbol = info.dli_sname;
        }

        fprintf(stderr, "%d: %p %s\n", idx, addr, symbol);
    }
}
#elif defined(__linux__) && defined(__GLIBC__)
#include <execinfo.h>
/*
 * ggml/src/ggml.c:139-143
 */
static void ggml_print_backtrace_symbols(void) {
    void *trace[100];
    int nptrs = backtrace(trace, sizeof(trace) / sizeof(trace[0]));
    backtrace_symbols_fd(trace, nptrs, STDERR_FILENO);
}
#elif defined(__APPLE__)
#include <execinfo.h>
/*
 * ggml/src/ggml.c:146-150
 */
static void ggml_print_backtrace_symbols(void) {
    void *trace[100];
    int nptrs = backtrace(trace, sizeof(trace) / sizeof(trace[0]));
    backtrace_symbols_fd(trace, nptrs, STDERR_FILENO);
}
#else
/*
 * ggml/src/ggml.c:152-155
 */
static void ggml_print_backtrace_symbols(void) {}
#endif

/*
 * ggml/src/ggml.c:157-241
 */
void ggml_print_backtrace(void) {
    const char *GGML_NO_BACKTRACE = getenv("GGML_NO_BACKTRACE");
    if (GGML_NO_BACKTRACE) {
        return;
    }
#if defined(__APPLE__)
    const char *GGML_BACKTRACE_LLDB = getenv("GGML_BACKTRACE_LLDB");
    if (!GGML_BACKTRACE_LLDB) {
        fprintf(stderr, "WARNING: Using native backtrace. Set "
                        "GGML_BACKTRACE_LLDB for more info.\n");
        fprintf(stderr, "WARNING: GGML_BACKTRACE_LLDB may cause native MacOS "
                        "Terminal.app to crash.\n");
        fprintf(stderr,
                "See: https://github.com/ggml-org/llama.cpp/pull/17869\n");
        ggml_print_backtrace_symbols();
        return;
    }
#endif
#if defined(__linux__)
    FILE *f = fopen("/proc/self/status", "r");
    size_t size = 0;
    char *line = NULL;
    ssize_t length = 0;
    while ((length = getline(&line, &size, f)) > 0) {
        if (!strncmp(line, "TracerPid:", sizeof("TracerPid:") - 1) &&
            (length != sizeof("TracerPid:\t0\n") - 1 ||
             line[length - 2] != '0')) {
            free(line);
            fclose(f);
            return;
        }
    }
    free(line);
    fclose(f);
    int lock[2] = {-1, -1};
    (void)!pipe(lock);
#endif
    const int parent_pid = getpid();
    const int child_pid = fork();
    if (child_pid < 0) {
#if defined(__linux__)
        close(lock[1]);
        close(lock[0]);
#endif
        return;
    } else if (child_pid == 0) {
        char attach[32];
        snprintf(attach, sizeof(attach), "attach %d", parent_pid);
#if defined(__linux__)
        close(lock[1]);
        (void)!read(lock[0], lock, 1);
        close(lock[0]);
#endif
        execlp("gdb", "gdb", "--batch", "-ex", "set style enabled on", "-ex",
               attach, "-ex", "bt -frame-info source-and-location", "-ex",
               "detach", "-ex", "quit", (char *)NULL);
        execlp("lldb", "lldb", "--batch", "-o", "bt", "-o", "quit", "-p",
               &attach[sizeof("attach ") - 1], (char *)NULL);
        ggml_print_backtrace_symbols();
        _Exit(0);
    } else {
#if defined(__linux__)
        prctl(PR_SET_PTRACER, child_pid);
        close(lock[1]);
        close(lock[0]);
#endif
        waitpid(child_pid, NULL, 0);
    }
}
#else
/*
 * ggml/src/ggml.c:237-241
 */
void ggml_print_backtrace(void) {}
#endif

/*
 * ggml/src/ggml.c:243
 */
static ggml_abort_callback_t g_abort_callback = NULL;

/*
 * ggml/src/ggml.c:252-272
 */
extern "C" void ggml_abort(const char *file, int line, const char *fmt, ...) {
    fflush(stdout);
    char message[2048];
    int offset = snprintf(message, sizeof(message), "%s:%d: ", file, line);
    va_list args;
    va_start(args, fmt);
    vsnprintf(message + offset, sizeof(message) - offset, fmt, args);
    va_end(args);
    if (g_abort_callback) {
        g_abort_callback(message);
    } else {
        fprintf(stderr, "%s\n", message);
        ggml_print_backtrace();
    }
    abort();
}

/*
 * ggml/src/ggml.c:1585-1593
 */
extern "C" bool ggml_are_same_shape(const struct ggml_tensor *t0,
                                    const struct ggml_tensor *t1) {
    static_assert(GGML_MAX_DIMS == 4,
                  "GGML_MAX_DIMS is not 4 - update this function");
    return (t0->ne[0] == t1->ne[0]) && (t0->ne[1] == t1->ne[1]) &&
           (t0->ne[2] == t1->ne[2]) && (t0->ne[3] == t1->ne[3]);
}

/*
 * Wrapper for invoking the extracted kernel: fills the caller-owned
 * ggml_tensor and ggml_compute_params structures from flat arguments and
 * forwards to the extracted static function so the candidate loops are
 * emitted. The upstream caller was ggml_compute_forward_group_norm
 * (ops.cpp:4196) after checking src0->type == GGML_TYPE_F32.
 */
void ggml_compute_forward_group_norm_f32_bench(const float *input,
                                               float *output, int64_t ne00,
                                               int64_t ne01, int n_channels,
                                               int n_groups, float eps) {
    ggml_tensor src0 = {};
    src0.ne[0] = ne00;
    src0.ne[1] = ne01;
    src0.ne[2] = n_channels;
    src0.ne[3] = 1;
    src0.nb[0] = sizeof(float);
    src0.nb[1] = (size_t)ne00 * sizeof(float);
    src0.nb[2] = (size_t)ne00 * ne01 * sizeof(float);
    src0.nb[3] = (size_t)ne00 * ne01 * n_channels * sizeof(float);
    src0.data = (void *)input;

    ggml_tensor dst = {};
    dst.ne[0] = ne00;
    dst.ne[1] = ne01;
    dst.ne[2] = n_channels;
    dst.ne[3] = 1;
    dst.nb[0] = sizeof(float);
    dst.nb[1] = (size_t)ne00 * sizeof(float);
    dst.nb[2] = (size_t)ne00 * ne01 * sizeof(float);
    dst.nb[3] = (size_t)ne00 * ne01 * n_channels * sizeof(float);
    dst.data = output;
    dst.src[0] = &src0;
    dst.op_params[0] = n_groups;
    memcpy(dst.op_params + 1, &eps, sizeof(float));

    ggml_compute_params params = {};
    params.ith = 0;
    params.nth = 1;

    ggml_compute_forward_group_norm_f32(&params, &dst);
}
