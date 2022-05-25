/* -*-mode:c; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */

/*
  Copyright (c) 2018 Rian Hunter et. al, see AUTHORS file.

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
 */

#include <sgxwasm/emscripten_runtime_sys.h>

#include <sgxwasm/ast.h>
#include <sgxwasm/runtime.h>
#include <sgxwasm/util.h>

#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define __KDECL(to, n, t) t _##n
#define __KA(to, n, t) _##n

#define KWSCx(x, name, ...)                                                    \
  long sys_##name(__KMAP(x, __KDECL, __VA_ARGS__))                             \
  {                                                                            \
    long ret;                                                                  \
    ret = name(__KMAP(x, __KA, __VA_ARGS__));                                  \
    if (ret == -1) {                                                           \
      ret = -errno;                                                            \
    }                                                                          \
    return ret;                                                                \
  }

#define KWSC1(name, ...) KWSCx(1, name, __VA_ARGS__)
#define KWSC2(name, ...) KWSCx(2, name, __VA_ARGS__)
#define KWSC3(name, ...) KWSCx(3, name, __VA_ARGS__)
#define KWSC5(name, ...) KWSCx(5, name, __VA_ARGS__)
#define KWSC6(name, ...) KWSCx(6, name, __VA_ARGS__)

#include <sgxwasm/emscripten_runtime_sys_def.h>

struct Memory*
sgxwasm_emscripten_get_mem(struct Function* func)
{
  return func->module->mems.data[0];
}

void
sgxwasm_emscripten_internal_abort(const char* msg)
{
  fprintf(stderr, "%s\n", msg);
  sgxwasm_trap(SGXWASM_TRAP_ABORT);
}
