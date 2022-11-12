.. Copyright (C) 2015-2022 Free Software Foundation, Inc.
   Originally contributed by David Malcolm <dmalcolm@redhat.com>

   This is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see
   <https://www.gnu.org/licenses/>.

.. default-domain:: c

ABI and API compatibility
=========================

The libgccjit developers strive for ABI and API backward-compatibility:
programs built against libgccjit.so stand a good chance of running
without recompilation against newer versions of libgccjit.so, and
ought to recompile without modification against newer versions of
libgccjit.h.

.. note:: The libgccjit++.h C++ API is more experimental, and less
          locked-down at this time.

API compatibility is achieved by extending the API rather than changing
it.  For ABI compatiblity, we avoid bumping the SONAME, and instead use
symbol versioning to tag each symbol, so that a binary linked against
libgccjit.so is tagged according to the symbols that it uses.

For example, :func:`gcc_jit_context_add_command_line_option` was added in
``LIBGCCJIT_ABI_1``.  If a client program uses it, this can be detected
from metadata by using ``objdump``:

.. code-block:: bash

   $ objdump -p testsuite/jit/test-extra-options.c.exe | tail -n 8

   Version References:
     required from libgccjit.so.0:
       0x00824161 0x00 04 LIBGCCJIT_ABI_1
       0x00824160 0x00 03 LIBGCCJIT_ABI_0
     required from libc.so.6:

You can see the symbol tags provided by libgccjit.so using ``objdump``:

.. code-block:: bash

   $ objdump -p libgccjit.so | less
   [...snip...]
   Version definitions:
   1 0x01 0x0ff81f20 libgccjit.so.0
   2 0x00 0x00824160 LIBGCCJIT_ABI_0
   3 0x00 0x00824161 LIBGCCJIT_ABI_1
           LIBGCCJIT_ABI_0
   [...snip...]

Programmatically checking version
*********************************

Client code can programmatically check libgccjit version using:

.. function::  int gcc_jit_version_major (void)

   Return libgccjit major version.  This is analogous to __GNUC__ in C code.

.. function::  int gcc_jit_version_minor (void)

   Return libgccjit minor version.  This is analogous to
   __GNUC_MINOR__ in C code.

.. function::  int gcc_jit_version_patchlevel (void)

   Return libgccjit patchlevel version.  This is analogous to
   __GNUC_PATCHLEVEL__ in C code.

.. note:: These entry points has been added with ``LIBGCCJIT_ABI_13``
          (see below).

ABI symbol tags
***************

The initial release of libgccjit (in gcc 5.1) did not use symbol versioning.

Newer releases use the following tags.

.. _LIBGCCJIT_ABI_0:

``LIBGCCJIT_ABI_0``
-------------------

All entrypoints in the initial release of libgccjit are tagged with
``LIBGCCJIT_ABI_0``, to signify the transition to symbol versioning.

Binaries built against older copies of ``libgccjit.so`` should
continue to work, with this being handled transparently by the linker
(see `this post
<https://gcc.gnu.org/ml/gcc-patches/2015-06/msg02126.html>`_)

.. _LIBGCCJIT_ABI_1:

``LIBGCCJIT_ABI_1``
-------------------
``LIBGCCJIT_ABI_1`` covers the addition of
:func:`gcc_jit_context_add_command_line_option`

.. _LIBGCCJIT_ABI_2:

``LIBGCCJIT_ABI_2``
-------------------
``LIBGCCJIT_ABI_2`` covers the addition of
:func:`gcc_jit_context_set_bool_allow_unreachable_blocks`

.. _LIBGCCJIT_ABI_3:

``LIBGCCJIT_ABI_3``
-------------------
``LIBGCCJIT_ABI_3`` covers the addition of switch statements via API
entrypoints:

  * :func:`gcc_jit_block_end_with_switch`

  * :func:`gcc_jit_case_as_object`

  * :func:`gcc_jit_context_new_case`

.. _LIBGCCJIT_ABI_4:

``LIBGCCJIT_ABI_4``
-------------------
``LIBGCCJIT_ABI_4`` covers the addition of timers via API
entrypoints:

  * :func:`gcc_jit_context_get_timer`

  * :func:`gcc_jit_context_set_timer`

  * :func:`gcc_jit_timer_new`

  * :func:`gcc_jit_timer_release`

  * :func:`gcc_jit_timer_push`

  * :func:`gcc_jit_timer_pop`

  * :func:`gcc_jit_timer_print`

.. _LIBGCCJIT_ABI_5:

``LIBGCCJIT_ABI_5``
-------------------
``LIBGCCJIT_ABI_5`` covers the addition of
:func:`gcc_jit_context_set_bool_use_external_driver`

.. _LIBGCCJIT_ABI_6:

``LIBGCCJIT_ABI_6``
-------------------
``LIBGCCJIT_ABI_6`` covers the addition of
:func:`gcc_jit_rvalue_set_bool_require_tail_call`

.. _LIBGCCJIT_ABI_7:

``LIBGCCJIT_ABI_7``
-------------------
``LIBGCCJIT_ABI_7`` covers the addition of
:func:`gcc_jit_type_get_aligned`

.. _LIBGCCJIT_ABI_8:

``LIBGCCJIT_ABI_8``
-------------------
``LIBGCCJIT_ABI_8`` covers the addition of
:func:`gcc_jit_type_get_vector`

.. _LIBGCCJIT_ABI_9:

``LIBGCCJIT_ABI_9``
-------------------
``LIBGCCJIT_ABI_9`` covers the addition of
:func:`gcc_jit_function_get_address`

.. _LIBGCCJIT_ABI_10:

``LIBGCCJIT_ABI_10``
--------------------
``LIBGCCJIT_ABI_10`` covers the addition of
:func:`gcc_jit_context_new_rvalue_from_vector`

.. _LIBGCCJIT_ABI_11:

``LIBGCCJIT_ABI_11``
--------------------
``LIBGCCJIT_ABI_11`` covers the addition of
:func:`gcc_jit_context_add_driver_option`

.. _LIBGCCJIT_ABI_12:

``LIBGCCJIT_ABI_12``
--------------------
``LIBGCCJIT_ABI_12`` covers the addition of
:func:`gcc_jit_context_new_bitfield`

.. _LIBGCCJIT_ABI_13:

``LIBGCCJIT_ABI_13``
--------------------
``LIBGCCJIT_ABI_13`` covers the addition of version functions via API
entrypoints:

  * :func:`gcc_jit_version_major`

  * :func:`gcc_jit_version_minor`

  * :func:`gcc_jit_version_patchlevel`

.. _LIBGCCJIT_ABI_14:

``LIBGCCJIT_ABI_14``
--------------------
``LIBGCCJIT_ABI_14`` covers the addition of
:func:`gcc_jit_global_set_initializer`

.. _LIBGCCJIT_ABI_15:

``LIBGCCJIT_ABI_15``
--------------------
``LIBGCCJIT_ABI_15`` covers the addition of API entrypoints for directly
embedding assembler instructions:

  * :func:`gcc_jit_block_add_extended_asm`
  * :func:`gcc_jit_block_end_with_extended_asm_goto`
  * :func:`gcc_jit_extended_asm_as_object`
  * :func:`gcc_jit_extended_asm_set_volatile_flag`
  * :func:`gcc_jit_extended_asm_set_inline_flag`
  * :func:`gcc_jit_extended_asm_add_output_operand`
  * :func:`gcc_jit_extended_asm_add_input_operand`
  * :func:`gcc_jit_extended_asm_add_clobber`
  * :func:`gcc_jit_context_add_top_level_asm`

.. _LIBGCCJIT_ABI_16:

``LIBGCCJIT_ABI_16``
--------------------
``LIBGCCJIT_ABI_16`` covers the addition of reflection functions via API
entrypoints:

  * :func:`gcc_jit_function_get_return_type`

  * :func:`gcc_jit_function_get_param_count`

  * :func:`gcc_jit_type_dyncast_array`

  * :func:`gcc_jit_type_is_bool`

  * :func:`gcc_jit_type_is_integral`

  * :func:`gcc_jit_type_is_pointer`

  * :func:`gcc_jit_type_is_struct`

  * :func:`gcc_jit_type_dyncast_vector`

  * :func:`gcc_jit_type_unqualified`

  * :func:`gcc_jit_type_dyncast_function_ptr_type`

  * :func:`gcc_jit_function_type_get_return_type`

  * :func:`gcc_jit_function_type_get_param_count`

  * :func:`gcc_jit_function_type_get_param_type`

  * :func:`gcc_jit_vector_type_get_num_units`

  * :func:`gcc_jit_vector_type_get_element_type`

  * :func:`gcc_jit_struct_get_field`

  * :func:`gcc_jit_struct_get_field_count`

.. _LIBGCCJIT_ABI_17:

``LIBGCCJIT_ABI_17``
-----------------------
``LIBGCCJIT_ABI_17`` covers the addition of an API entrypoint to set the
thread-local storage model of a variable:

  * :func:`gcc_jit_lvalue_set_tls_model`

.. _LIBGCCJIT_ABI_18:

``LIBGCCJIT_ABI_18``
-----------------------
``LIBGCCJIT_ABI_18`` covers the addition of an API entrypoint to set the link
section of a variable:

  * :func:`gcc_jit_lvalue_set_link_section`

.. _LIBGCCJIT_ABI_19:

``LIBGCCJIT_ABI_19``
-----------------------
``LIBGCCJIT_ABI_19`` covers the addition of API entrypoints to set the initial value
of a global with an rvalue and to use constructors:

  * :func:`gcc_jit_context_new_array_constructor`
  * :func:`gcc_jit_context_new_struct_constructor`
  * :func:`gcc_jit_context_new_union_constructor`
  * :func:`gcc_jit_global_set_initializer_rvalue`

.. _LIBGCCJIT_ABI_20:

``LIBGCCJIT_ABI_20``
-----------------------
``LIBGCCJIT_ABI_20`` covers the addition of sized integer types, including
128-bit integers and helper functions for types:

  * :func:`gcc_jit_compatible_types`
  * :func:`gcc_jit_type_get_size`
  * :c:macro:`GCC_JIT_TYPE_UINT8_T`
  * :c:macro:`GCC_JIT_TYPE_UINT16_T`
  * :c:macro:`GCC_JIT_TYPE_UINT32_T`
  * :c:macro:`GCC_JIT_TYPE_UINT64_T`
  * :c:macro:`GCC_JIT_TYPE_UINT128_T`
  * :c:macro:`GCC_JIT_TYPE_INT8_T`
  * :c:macro:`GCC_JIT_TYPE_INT16_T`
  * :c:macro:`GCC_JIT_TYPE_INT32_T`
  * :c:macro:`GCC_JIT_TYPE_INT64_T`
  * :c:macro:`GCC_JIT_TYPE_INT128_T`

.. _LIBGCCJIT_ABI_21:

``LIBGCCJIT_ABI_21``
--------------------
``LIBGCCJIT_ABI_21`` covers the addition of an API entrypoint to bitcast a
value from one type to another:

  * :func:`gcc_jit_context_new_bitcast`

.. _LIBGCCJIT_ABI_22:

``LIBGCCJIT_ABI_22``
--------------------
``LIBGCCJIT_ABI_22`` covers the addition of an API entrypoint to set the
register name of a variable:

  * :func:`gcc_jit_lvalue_set_register_name`

.. _LIBGCCJIT_ABI_23:

``LIBGCCJIT_ABI_23``
--------------------
``LIBGCCJIT_ABI_23`` covers the addition of an API entrypoint to hide stderr
logs:

  * :func:`gcc_jit_context_set_bool_print_errors_to_stderr`

.. _LIBGCCJIT_ABI_24:

``LIBGCCJIT_ABI_24``
--------------------
``LIBGCCJIT_ABI_24`` covers the addition of functions to get and set the
alignment of a variable:

  * :func:`gcc_jit_lvalue_set_alignment`
  * :func:`gcc_jit_lvalue_get_alignment`
