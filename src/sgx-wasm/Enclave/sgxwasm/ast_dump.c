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

#include <sgxwasm/ast.h>

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void
dump_type_section(struct TypeSection* type_section)
{
  size_t i, j;
  for (i = 0; i < type_section->n_types; ++i) {
    struct TypeSectionType* type = &type_section->types[i];

    printf("(");
    for (j = 0; j < type->n_inputs; ++j) {
      printf("%d,", type->input_types[j]);
    }
    printf(") -> (");
    for (j = 0; j < FUNC_TYPE_N_OUTPUTS(type); ++j) {
      printf("%d,", FUNC_TYPE_OUTPUT_TYPES(type)[j]);
    }
    printf(")\n");
  }
}

void
dump_global_section(struct GlobalSection* global_section)
{
  uint32_t i;

  for (i = 0; i < global_section->n_globals; ++i) {
    struct GlobalSectionGlobal* global = &global_section->globals[i];
    printf("Global Section %" PRIu32 " type: %" PRIu8 " can mute: %" PRIu8 "\n",
           i,
           global->type.valtype,
           global->type.mut);
  }
}

void
dump_instructions(const struct Instr* instructions,
                  size_t n_instructions,
                  int indent);

void
dump_instruction(const struct Instr* instruction, int indent)
{
  int sps = indent * 2;
  const char* name;
  switch (instruction->opcode) {
    case OPCODE_UNREACHABLE:
      printf("%*sunreachable\n", sps, "");
      break;
    case OPCODE_NOP:
      printf("%*snop\n", sps, "");
    case OPCODE_BLOCK:
      printf("%*sblock 0x%02" PRIx8 "\n",
             sps,
             "",
             instruction->data.block.blocktype);
      /*dump_instructions(instruction->data.block.instructions,
                        instruction->data.block.n_instructions,
                        indent + 1);*/
      break;
    case OPCODE_LOOP:
      printf(
        "%*sloop 0x%02" PRIx8 "\n", sps, "", instruction->data.loop.blocktype);
      /*dump_instructions(instruction->data.loop.instructions,
                        instruction->data.loop.n_instructions,
                        indent + 1);*/
      break;
    case OPCODE_IF:
      printf(
        "%*sif 0x%02" PRIx8 "\n", sps, "", instruction->data.if_.blocktype);
      /*dump_instructions(instruction->data.if_.instructions_then,
                        instruction->data.if_.n_instructions_then,
                        indent + 1);*/
      break;
    case OPCODE_ELSE:
      printf("%*selse\n", sps, "");
      /*dump_instructions(instruction->data.if_.instructions_else,
                        instruction->data.if_.n_instructions_else,
                        indent + 1);*/
      break;
    case OPCODE_BR:
      printf("%*sbr 0x%" PRIx32 "\n", sps, "", instruction->data.br.labelidx);
      break;
    case OPCODE_BR_IF:
      printf(
        "%*sbr_if 0x%" PRIx32 "\n", sps, "", instruction->data.br_if.labelidx);
      break;
    case OPCODE_BR_TABLE: {
      uint32_t table_count = instruction->data.br_table.n_labelidxs;
      uint32_t* table = instruction->data.br_table.labelidxs;
      uint32_t default_index = instruction->data.br_table.labelidx;
      printf("%*sbr_table ", sps, "");
      for (uint32_t i = 0; i < table_count; i++) {
        printf("0x%" PRIx32 " ", table[i]);
      }
      printf("0x%" PRIx32 "\n", default_index);
      break;
    }
    case OPCODE_RETURN:
      printf("%*sreturn\n", sps, "");
      break;
    case OPCODE_END:
      printf("%*send\n", sps, "");
      break;
    case OPCODE_CALL:
      printf(
        "%*scall 0x%" PRIx32 "\n", sps, "", instruction->data.call.funcidx);
      break;
    case OPCODE_CALL_INDIRECT:
      printf("%*scall_indirect 0x%" PRIx32 "\n",
             sps,
             "",
             instruction->data.call_indirect.typeidx);
      break;
    case OPCODE_DROP:
      printf("%*sdrop\n", sps, "");
      break;
    case OPCODE_GET_LOCAL:
      printf("%*sget_local %" PRIu32 "\n",
             sps,
             "",
             instruction->data.get_local.localidx);
      break;
    case OPCODE_SET_LOCAL:
      printf("%*sset_local %" PRIu32 "\n",
             sps,
             "",
             instruction->data.set_local.localidx);
      break;
    case OPCODE_TEE_LOCAL:
      printf("%*stee_local %" PRIu32 "\n",
             sps,
             "",
             instruction->data.tee_local.localidx);
      break;
    case OPCODE_GET_GLOBAL:
      printf("%*sget_global %" PRIu32 "\n",
             sps,
             "",
             instruction->data.get_global.globalidx);
      break;
    case OPCODE_SET_GLOBAL:
      printf("%*sset_global %" PRIu32 "\n",
             sps,
             "",
             instruction->data.set_global.globalidx);
      break;
    case OPCODE_I32_LOAD:
      name = "i32.load";
      printf("%*s%s align: 0x%" PRIx32 " offset: 0x%" PRIx32 "\n",
             sps,
             "",
             name,
             instruction->data.i32_load.align,
             instruction->data.i32_load.offset);
      break;
    case OPCODE_I64_LOAD:
      name = "i64.load";
      printf("%*s%s align: 0x%" PRIx32 " offset: 0x%" PRIx32 "\n",
             sps,
             "",
             name,
             instruction->data.i64_load.align,
             instruction->data.i64_load.offset);
      break;
    case OPCODE_F32_LOAD:
      name = "f32.load";
      printf("%*s%s align: 0x%" PRIx32 " offset: 0x%" PRIx32 "\n",
             sps,
             "",
             name,
             instruction->data.f32_load.align,
             instruction->data.f32_load.offset);
      break;
    case OPCODE_F64_LOAD:
      name = "f64.load";
      printf("%*s%s align: 0x%" PRIx32 " offset: 0x%" PRIx32 "\n",
             sps,
             "",
             name,
             instruction->data.f64_load.align,
             instruction->data.f64_load.offset);
      break;
    case OPCODE_I32_STORE:
      name = "i32.store";
      printf("%*s%s align: 0x%" PRIx32 " offset: 0x%" PRIx32 "\n",
             sps,
             "",
             name,
             instruction->data.i64_load.align,
             instruction->data.i64_load.offset);
      break;
    case OPCODE_I64_STORE:
      name = "i64.store";
      printf("%*s%s align: 0x%" PRIx32 " offset: 0x%" PRIx32 "\n",
             sps,
             "",
             name,
             instruction->data.i64_load.align,
             instruction->data.i64_load.offset);
      break;
    case OPCODE_I32_CONST:
      printf("%*si32.const %" PRIu32 "\n",
             sps,
             "",
             instruction->data.i32_const.value);
      break;
    case OPCODE_I64_CONST:
      printf("%*si64.const %" PRIu64 "\n",
             sps,
             "",
             instruction->data.i64_const.value);
      break;
    case OPCODE_I32_LT_S:
      printf("%*si32.lt_s\n", sps, "");
      break;
    case OPCODE_I32_GE_S:
      printf("%*si32.ge_s\n", sps, "");
      break;
    case OPCODE_I64_EQZ:
      printf("%*si64.eqz\n", sps, "");
      break;
    case OPCODE_I32_ADD:
      printf("%*si32.add\n", sps, "");
      break;
    case OPCODE_I32_SUB:
      printf("%*si32.sub\n", sps, "");
      break;
    case OPCODE_I32_MUL:
      printf("%*si32.mul\n", sps, "");
      break;
    case OPCODE_I32_AND:
      printf("%*si32.and\n", sps, "");
      break;
    case OPCODE_I32_OR:
      printf("%*si32.or\n", sps, "");
      break;
    case OPCODE_I32_XOR:
      printf("%*si32.xor\n", sps, "");
      break;
    case OPCODE_I64_ADD:
      printf("%*si64.add\n", sps, "");
      break;
    case OPCODE_I64_SUB:
      printf("%*si64.sub\n", sps, "");
      break;
    case OPCODE_I64_MUL:
      printf("%*si64.mul\n", sps, "");
      break;
    case OPCODE_I64_AND:
      printf("%*si64.and\n", sps, "");
      break;
    case OPCODE_I64_OR:
      printf("%*si64.or\n", sps, "");
      break;
    case OPCODE_I64_XOR:
      printf("%*si64.xor\n", sps, "");
      break;
    case OPCODE_F32_ADD:
      printf("%*sf32.add\n", sps, "");
      break;
    case OPCODE_F32_SUB:
      printf("%*sf32.sub\n", sps, "");
      break;
    case OPCODE_F32_MUL:
      printf("%*sf32.mul\n", sps, "");
      break;
    case OPCODE_F32_DIV:
      printf("%*sf32.div\n", sps, "");
      break;
    case OPCODE_F64_ADD:
      printf("%*sf64.add\n", sps, "");
      break;
    case OPCODE_F64_SUB:
      printf("%*sf64.sub\n", sps, "");
      break;
    case OPCODE_F64_MUL:
      printf("%*sf64.mul\n", sps, "");
      break;
    case OPCODE_F64_DIV:
      printf("%*sf64.div\n", sps, "");
      break;
    default:
      printf("%*sBAD 0x%02" PRIx8 "\n", sps, "", instruction->opcode);
      break;
  }
}

void
dump_instructions(const struct Instr* instructions,
                  size_t n_instructions,
                  int indent)
{
  uint32_t i;

  for (i = 0; i < n_instructions; ++i) {
    dump_instruction(&instructions[i], indent);
  }
}
