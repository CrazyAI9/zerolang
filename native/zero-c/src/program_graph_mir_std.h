#ifndef ZERO_PROGRAM_GRAPH_MIR_STD_H
#define ZERO_PROGRAM_GRAPH_MIR_STD_H

#include "zero.h"

#include <stddef.h>

bool z_program_graph_append_source_std_functions(Program *program, size_t *appended, ZDiag *diag);
bool z_program_graph_ir_add_readonly_data(IrProgram *ir, const unsigned char *bytes, unsigned len, int line, int column, unsigned *out_offset);
bool z_program_graph_steal_source_std_helpers(IrProgram *dest, IrProgram *source);

#endif
