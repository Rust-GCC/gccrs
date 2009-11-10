// dwarf.h -- DWARF2 constants  -*- C++ -*-

// Copyright 2006, 2007, Free Software Foundation, Inc.
// Written by Ian Lance Taylor <iant@google.com>.

// This file is part of elfcpp.

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public License
// as published by the Free Software Foundation; either version 2, or
// (at your option) any later version.

// In addition to the permissions in the GNU Library General Public
// License, the Free Software Foundation gives you unlimited
// permission to link the compiled version of this file into
// combinations with other programs, and to distribute those
// combinations without any restriction coming from the use of this
// file.  (The Library Public License restrictions do apply in other
// respects; for example, they cover modification of the file, and
/// distribution when not linked into a combined executable.)

// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.

// You should have received a copy of the GNU Library General Public
// License along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA
// 02110-1301, USA.

#ifndef ELFCPP_DWARF_H
#define ELFCPP_DWARF_H

namespace elfcpp
{

// DWARF2 codes.

enum DW_TAG
{
  DW_TAG_padding = 0x00,
  DW_TAG_array_type = 0x01,
  DW_TAG_class_type = 0x02,
  DW_TAG_entry_point = 0x03,
  DW_TAG_enumeration_type = 0x04,
  DW_TAG_formal_parameter = 0x05,
  DW_TAG_imported_declaration = 0x08,
  DW_TAG_label = 0x0a,
  DW_TAG_lexical_block = 0x0b,
  DW_TAG_member = 0x0d,
  DW_TAG_pointer_type = 0x0f,
  DW_TAG_reference_type = 0x10,
  DW_TAG_compile_unit = 0x11,
  DW_TAG_string_type = 0x12,
  DW_TAG_structure_type = 0x13,
  DW_TAG_subroutine_type = 0x15,
  DW_TAG_typedef = 0x16,
  DW_TAG_union_type = 0x17,
  DW_TAG_unspecified_parameters = 0x18,
  DW_TAG_variant = 0x19,
  DW_TAG_common_block = 0x1a,
  DW_TAG_common_inclusion = 0x1b,
  DW_TAG_inheritance = 0x1c,
  DW_TAG_inlined_subroutine = 0x1d,
  DW_TAG_module = 0x1e,
  DW_TAG_ptr_to_member_type = 0x1f,
  DW_TAG_set_type = 0x20,
  DW_TAG_subrange_type = 0x21,
  DW_TAG_with_stmt = 0x22,
  DW_TAG_access_declaration = 0x23,
  DW_TAG_base_type = 0x24,
  DW_TAG_catch_block = 0x25,
  DW_TAG_const_type = 0x26,
  DW_TAG_constant = 0x27,
  DW_TAG_enumerator = 0x28,
  DW_TAG_file_type = 0x29,
  DW_TAG_friend = 0x2a,
  DW_TAG_namelist = 0x2b,
  DW_TAG_namelist_item = 0x2c,
  DW_TAG_packed_type = 0x2d,
  DW_TAG_subprogram = 0x2e,
  DW_TAG_template_type_param = 0x2f,
  DW_TAG_template_value_param = 0x30,
  DW_TAG_thrown_type = 0x31,
  DW_TAG_try_block = 0x32,
  DW_TAG_variant_part = 0x33,
  DW_TAG_variable = 0x34,
  DW_TAG_volatile_type = 0x35,

  // DWARF3.
  DW_TAG_dwarf_procedure = 0x36,
  DW_TAG_restrict_type = 0x37,
  DW_TAG_interface_type = 0x38,
  DW_TAG_namespace = 0x39,
  DW_TAG_imported_module = 0x3a,
  DW_TAG_unspecified_type = 0x3b,
  DW_TAG_partial_unit = 0x3c,
  DW_TAG_imported_unit = 0x3d,
  DW_TAG_condition = 0x3f,
  DW_TAG_shared_type = 0x40,

  // SGI/MIPS extensions.
  DW_TAG_MIPS_loop = 0x4081,

  // HP extensions.
  // See: ftp://ftp.hp.com/pub/lang/tools/WDB/wdb-4.0.tar.gz.
  DW_TAG_HP_array_descriptor = 0x4090,

  // GNU extensions.
  DW_TAG_format_label = 0x4101,		// For FORTRAN 77 and Fortran 90.
  DW_TAG_function_template = 0x4102,	// For C++.
  DW_TAG_class_template = 0x4103,	// For C++.
  DW_TAG_GNU_BINCL = 0x4104,
  DW_TAG_GNU_EINCL = 0x4105,

  // Extensions for UPC.  See: http://upc.gwu.edu/~upc.
  DW_TAG_upc_shared_type = 0x8765,
  DW_TAG_upc_strict_type = 0x8766,
  DW_TAG_upc_relaxed_type = 0x8767,

  // PGI (STMicroelectronics) extensions.  No documentation available.
  DW_TAG_PGI_kanji_type      = 0xA000,
  DW_TAG_PGI_interface_block = 0xA020
};

enum DW_FORM
{
  DW_FORM_null = 0x00,
  DW_FORM_addr = 0x01,
  DW_FORM_block2 = 0x03,
  DW_FORM_block4 = 0x04,
  DW_FORM_data2 = 0x05,
  DW_FORM_data4 = 0x06,
  DW_FORM_data8 = 0x07,
  DW_FORM_string = 0x08,
  DW_FORM_block = 0x09,
  DW_FORM_block1 = 0x0a,
  DW_FORM_data1 = 0x0b,
  DW_FORM_flag = 0x0c,
  DW_FORM_sdata = 0x0d,
  DW_FORM_strp = 0x0e,
  DW_FORM_udata = 0x0f,
  DW_FORM_ref_addr = 0x10,
  DW_FORM_ref1 = 0x11,
  DW_FORM_ref2 = 0x12,
  DW_FORM_ref4 = 0x13,
  DW_FORM_ref8 = 0x14,
  DW_FORM_ref_udata = 0x15,
  DW_FORM_indirect = 0x16
};

// Frame unwind information.

enum DW_EH_PE
{
  DW_EH_PE_absptr = 0x00,
  DW_EH_PE_omit = 0xff,

  DW_EH_PE_uleb128 = 0x01,
  DW_EH_PE_udata2 = 0x02,
  DW_EH_PE_udata4 = 0x03,
  DW_EH_PE_udata8 = 0x04,
  DW_EH_PE_signed = 0x08,
  DW_EH_PE_sleb128 = 0x09,
  DW_EH_PE_sdata2 = 0x0a,
  DW_EH_PE_sdata4 = 0x0b,
  DW_EH_PE_sdata8 = 0x0c,

  DW_EH_PE_pcrel = 0x10,
  DW_EH_PE_textrel = 0x20,
  DW_EH_PE_datarel = 0x30,
  DW_EH_PE_funcrel = 0x40,
  DW_EH_PE_aligned = 0x50,

  DW_EH_PE_indirect = 0x80
};

// Line number opcodes.

enum DW_LINE_OPS
{
  DW_LNS_extended_op = 0,
  DW_LNS_copy = 1,
  DW_LNS_advance_pc = 2,
  DW_LNS_advance_line = 3,
  DW_LNS_set_file = 4,
  DW_LNS_set_column = 5,
  DW_LNS_negate_stmt = 6,
  DW_LNS_set_basic_block = 7,
  DW_LNS_const_add_pc = 8,
  DW_LNS_fixed_advance_pc = 9,
  // DWARF 3.
  DW_LNS_set_prologue_end = 10,
  DW_LNS_set_epilogue_begin = 11,
  DW_LNS_set_isa = 12
};

// Line number extended opcodes.

enum DW_LINE_EXTENDED_OPS
{
  DW_LNE_end_sequence = 1,
  DW_LNE_set_address = 2,
  DW_LNE_define_file = 3,
  // HP extensions.
  DW_LNE_HP_negate_is_UV_update      = 0x11,
  DW_LNE_HP_push_context             = 0x12,
  DW_LNE_HP_pop_context              = 0x13,
  DW_LNE_HP_set_file_line_column     = 0x14,
  DW_LNE_HP_set_routine_name         = 0x15,
  DW_LNE_HP_set_sequence             = 0x16,
  DW_LNE_HP_negate_post_semantics    = 0x17,
  DW_LNE_HP_negate_function_exit     = 0x18,
  DW_LNE_HP_negate_front_end_logical = 0x19,
  DW_LNE_HP_define_proc              = 0x20
};

// Type encoding names and codes

enum DW_ENCODING
{
  DW_ATE_address                     =0x1,
  DW_ATE_boolean                     =0x2,
  DW_ATE_complex_float               =0x3,
  DW_ATE_float                       =0x4,
  DW_ATE_signed                      =0x5,
  DW_ATE_signed_char                 =0x6,
  DW_ATE_unsigned                    =0x7,
  DW_ATE_unsigned_char               =0x8,
  // DWARF3/DWARF3f
  DW_ATE_imaginary_float             =0x9,
  DW_ATE_packed_decimal              =0xa,
  DW_ATE_numeric_string              =0xb,
  DW_ATE_edited                      =0xc,
  DW_ATE_signed_fixed                =0xd,
  DW_ATE_unsigned_fixed              =0xe,
  DW_ATE_decimal_float               =0xf,
  DW_ATE_lo_user                     =0x80,
  DW_ATE_hi_user                     =0xff
};

// Location virtual machine opcodes

enum DW_OP
{
  DW_OP_addr                         =0x03,
  DW_OP_deref                        =0x06,
  DW_OP_const1u                      =0x08,
  DW_OP_const1s                      =0x09,
  DW_OP_const2u                      =0x0a,
  DW_OP_const2s                      =0x0b,
  DW_OP_const4u                      =0x0c,
  DW_OP_const4s                      =0x0d,
  DW_OP_const8u                      =0x0e,
  DW_OP_const8s                      =0x0f,
  DW_OP_constu                       =0x10,
  DW_OP_consts                       =0x11,
  DW_OP_dup                          =0x12,
  DW_OP_drop                         =0x13,
  DW_OP_over                         =0x14,
  DW_OP_pick                         =0x15,
  DW_OP_swap                         =0x16,
  DW_OP_rot                          =0x17,
  DW_OP_xderef                       =0x18,
  DW_OP_abs                          =0x19,
  DW_OP_and                          =0x1a,
  DW_OP_div                          =0x1b,
  DW_OP_minus                        =0x1c,
  DW_OP_mod                          =0x1d,
  DW_OP_mul                          =0x1e,
  DW_OP_neg                          =0x1f,
  DW_OP_not                          =0x20,
  DW_OP_or                           =0x21,
  DW_OP_plus                         =0x22,
  DW_OP_plus_uconst                  =0x23,
  DW_OP_shl                          =0x24,
  DW_OP_shr                          =0x25,
  DW_OP_shra                         =0x26,
  DW_OP_xor                          =0x27,
  DW_OP_bra                          =0x28,
  DW_OP_eq                           =0x29,
  DW_OP_ge                           =0x2a,
  DW_OP_gt                           =0x2b,
  DW_OP_le                           =0x2c,
  DW_OP_lt                           =0x2d,
  DW_OP_ne                           =0x2e,
  DW_OP_skip                         =0x2f,
  DW_OP_lit0                         =0x30,
  DW_OP_lit1                         =0x31,
  DW_OP_lit2                         =0x32,
  DW_OP_lit3                         =0x33,
  DW_OP_lit4                         =0x34,
  DW_OP_lit5                         =0x35,
  DW_OP_lit6                         =0x36,
  DW_OP_lit7                         =0x37,
  DW_OP_lit8                         =0x38,
  DW_OP_lit9                         =0x39,
  DW_OP_lit10                        =0x3a,
  DW_OP_lit11                        =0x3b,
  DW_OP_lit12                        =0x3c,
  DW_OP_lit13                        =0x3d,
  DW_OP_lit14                        =0x3e,
  DW_OP_lit15                        =0x3f,
  DW_OP_lit16                        =0x40,
  DW_OP_lit17                        =0x41,
  DW_OP_lit18                        =0x42,
  DW_OP_lit19                        =0x43,
  DW_OP_lit20                        =0x44,
  DW_OP_lit21                        =0x45,
  DW_OP_lit22                        =0x46,
  DW_OP_lit23                        =0x47,
  DW_OP_lit24                        =0x48,
  DW_OP_lit25                        =0x49,
  DW_OP_lit26                        =0x4a,
  DW_OP_lit27                        =0x4b,
  DW_OP_lit28                        =0x4c,
  DW_OP_lit29                        =0x4d,
  DW_OP_lit30                        =0x4e,
  DW_OP_lit31                        =0x4f,
  DW_OP_reg0                         =0x50,
  DW_OP_reg1                         =0x51,
  DW_OP_reg2                         =0x52,
  DW_OP_reg3                         =0x53,
  DW_OP_reg4                         =0x54,
  DW_OP_reg5                         =0x55,
  DW_OP_reg6                         =0x56,
  DW_OP_reg7                         =0x57,
  DW_OP_reg8                         =0x58,
  DW_OP_reg9                         =0x59,
  DW_OP_reg10                        =0x5a,
  DW_OP_reg11                        =0x5b,
  DW_OP_reg12                        =0x5c,
  DW_OP_reg13                        =0x5d,
  DW_OP_reg14                        =0x5e,
  DW_OP_reg15                        =0x5f,
  DW_OP_reg16                        =0x60,
  DW_OP_reg17                        =0x61,
  DW_OP_reg18                        =0x62,
  DW_OP_reg19                        =0x63,
  DW_OP_reg20                        =0x64,
  DW_OP_reg21                        =0x65,
  DW_OP_reg22                        =0x66,
  DW_OP_reg23                        =0x67,
  DW_OP_reg24                        =0x68,
  DW_OP_reg25                        =0x69,
  DW_OP_reg26                        =0x6a,
  DW_OP_reg27                        =0x6b,
  DW_OP_reg28                        =0x6c,
  DW_OP_reg29                        =0x6d,
  DW_OP_reg30                        =0x6e,
  DW_OP_reg31                        =0x6f,
  DW_OP_breg0                        =0x70,
  DW_OP_breg1                        =0x71,
  DW_OP_breg2                        =0x72,
  DW_OP_breg3                        =0x73,
  DW_OP_breg4                        =0x74,
  DW_OP_breg5                        =0x75,
  DW_OP_breg6                        =0x76,
  DW_OP_breg7                        =0x77,
  DW_OP_breg8                        =0x78,
  DW_OP_breg9                        =0x79,
  DW_OP_breg10                       =0x7a,
  DW_OP_breg11                       =0x7b,
  DW_OP_breg12                       =0x7c,
  DW_OP_breg13                       =0x7d,
  DW_OP_breg14                       =0x7e,
  DW_OP_breg15                       =0x7f,
  DW_OP_breg16                       =0x80,
  DW_OP_breg17                       =0x81,
  DW_OP_breg18                       =0x82,
  DW_OP_breg19                       =0x83,
  DW_OP_breg20                       =0x84,
  DW_OP_breg21                       =0x85,
  DW_OP_breg22                       =0x86,
  DW_OP_breg23                       =0x87,
  DW_OP_breg24                       =0x88,
  DW_OP_breg25                       =0x89,
  DW_OP_breg26                       =0x8a,
  DW_OP_breg27                       =0x8b,
  DW_OP_breg28                       =0x8c,
  DW_OP_breg29                       =0x8d,
  DW_OP_breg30                       =0x8e,
  DW_OP_breg31                       =0x8f,
  DW_OP_regX                         =0x90,
  DW_OP_fbreg                        =0x91,
  DW_OP_bregX                        =0x92,
  DW_OP_piece                        =0x93,
  DW_OP_deref_size                   =0x94,
  DW_OP_xderef_size                  =0x95,
  DW_OP_nop                          =0x96,
  // DWARF3/DWARF3f
  DW_OP_push_object_address          =0x97,
  DW_OP_call2                        =0x98,
  DW_OP_call4                        =0x99,
  DW_OP_call_ref                     =0x9a,
  DW_OP_form_tls_address             =0x9b,
  DW_OP_call_frame_cfa               =0x9c,
  DW_OP_bit_piece                    =0x9d,
  DW_OP_lo_user                      =0xe0,
  DW_OP_hi_user                      =0xff,
  // GNU extensions
  DW_OP_GNU_push_tls_address         =0xe0
};

} // End namespace elfcpp.

#endif // !defined(ELFCPP_DWARF_H)
