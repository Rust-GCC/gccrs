#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libgccjit.h"

#include "harness.h"

void
create_code (gcc_jit_context *ctxt, void *user_data)
{
  /* Let's try to inject the equivalent of:
int
func ()
{
    _Float32 float32_val = 2.3;
    float float = 4.5;
    if (float == float32_val)
        return 1;
    return 0;
}
   */
  gcc_jit_type *int_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_INT);
  gcc_jit_type *float_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_FLOAT);
  gcc_jit_type *float32_type;

  gcc_jit_context *info_ctxt = gcc_jit_context_acquire ();
  gcc_jit_target_info* target_info = gcc_jit_context_get_target_info (info_ctxt);
  if (gcc_jit_target_info_supports_target_dependent_type (target_info,
							  GCC_JIT_TYPE_FLOAT32))
  {
    gcc_jit_type *sized_float_type =
      gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_FLOAT32);
    float32_type = gcc_jit_type_get_aligned (sized_float_type, 4);
  }
  else
    float32_type = gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_FLOAT);

  gcc_jit_function *func =
    gcc_jit_context_new_function (ctxt,
				  NULL,
				  GCC_JIT_FUNCTION_EXPORTED,
				  int_type,
				  "func",
				  0, NULL, 0);

  gcc_jit_block *initial =
    gcc_jit_function_new_block (func, "initial");
  gcc_jit_block *if_block =
    gcc_jit_function_new_block (func, "if");
  gcc_jit_block *else_block =
    gcc_jit_function_new_block (func, "else");

  gcc_jit_rvalue *zero = gcc_jit_context_zero (ctxt, int_type);
  gcc_jit_rvalue *one = gcc_jit_context_one (ctxt, int_type);

  gcc_jit_rvalue *float32_val =
    gcc_jit_context_new_rvalue_from_double (ctxt, float32_type, 2.3);
  gcc_jit_rvalue *float_val =
    gcc_jit_context_new_rvalue_from_double (ctxt, float_type, 4.5);

  gcc_jit_rvalue *comparison = gcc_jit_context_new_comparison (ctxt,
    NULL, GCC_JIT_COMPARISON_EQ, float_val, float32_val);

  gcc_jit_block_end_with_conditional (initial,
    NULL, comparison, if_block, else_block);

  gcc_jit_block_end_with_return(if_block, NULL, one);

  gcc_jit_block_end_with_return(else_block, NULL, zero);
}

void
verify_code (gcc_jit_context *ctxt, gcc_jit_result *result)
{
  CHECK_NON_NULL (result);
}
