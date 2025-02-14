/*
 * Copyright (C) 2011-2018 Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#include <stdarg.h>
#include <stdio.h>      /* vsnprintf */

#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "ocall_type.h"
typedef void (*sighandler_t)(int);

#include "sgx_trts.h"

#include "Enclave.h"
#include "Enclave_t.h"  /* print_string */


#include "ocall_stub.c"

#if 0
void enclave_main()
{
  int fd = open("/dev/null", O_RDWR, 0);
  unsigned long t1, t2;
  int i, loop = 1000;

  printf("fd: %d\n", fd);
  ocall_sgx_rdtsc(&t1);
  for (i = 0; i < loop; i++)
    write(fd, "test", 4);
  ocall_sgx_rdtsc(&t2);
  printf("base t: %lu\n", t2 - t1);

  printf("fd: %d\n", fd);
  ocall_sgx_rdtsc(&t1);
  for (i = 0; i < loop; i++)
    write_opt(fd, "test", 4);
  ocall_sgx_rdtsc(&t2);
  printf("opt  t: %lu\n", t2 - t1);

  return;
}
#endif
