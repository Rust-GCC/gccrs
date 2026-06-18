/*
 * Copyright (c) 2021-2026 Symas Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the Symas Corporation nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
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
 */
#ifndef _GENUTIL_H_
#define _GENUTIL_H_

extern bool exception_location_active;
extern bool skip_exception_processing;

extern bool suppress_dest_depends;

extern std::vector<std::string>current_filename;

extern tree var_decl_exception_code;         // int         __gg__exception_code;
extern tree var_decl_exception_file_status;  // int         __gg__exception_file_status;
extern tree var_decl_exception_file_name;    // const char *__gg__exception_file_name;
extern tree var_decl_exception_statement;    // const char *__gg__exception_statement;
extern tree var_decl_exception_source_file;  // const char *__gg__exception_source_file;
extern tree var_decl_exception_line_number;  // int         __gg__exception_line_number;
extern tree var_decl_exception_program_id;   // const char *__gg__exception_program_id;
extern tree var_decl_exception_section;      // const char *__gg__exception_section;
extern tree var_decl_exception_paragraph;    // const char *__gg__exception_paragraph;

extern tree var_decl_default_compute_error;  // int         __gg__default_compute_error;
extern tree var_decl_rdigits;                // int         __gg__rdigits;
extern tree var_decl_unique_prog_id;         // size_t      __gg__unique_prog_id;

extern tree var_decl_exit_address;           // This is for implementing pseudo_return_pop

extern tree var_decl_call_parameter_signature; // char   *__gg__call_parameter_signature
extern tree var_decl_call_parameter_count;     // int __gg__call_parameter_count
extern tree var_decl_call_parameter_lengths;   // size_t *var_decl_call_parameter_lengths

extern tree var_decl_nop;         // int __gg__nop
extern tree var_decl_main_called; // int __gg__main_called
extern tree var_decl_entry_index; // void* __gg__entry_index
extern tree var_decl_dialects;    // void* __gg__dialects

int       get_scaled_rdigits(cbl_field_t *field);
int       get_scaled_digits(cbl_field_t *field);

void      get_binary_value( tree value,
                            tree rdigits,
                            cbl_field_t *field,
                            tree         field_offset,
                            tree         hilo = NULL);
tree      get_binary_value_tree(tree return_type,
                                tree rdigits,
                                cbl_field_t *field,
                                tree         field_offset,
                                tree         hilo = NULL);
tree      get_binary_value_tree(tree return_type,
                                tree rdigits,
                                const cbl_refer_t &refer,
                                tree         hilo = NULL);
tree      get_data_address( cbl_field_t *field,
                            tree         offset);

FIXED_WIDE_INT(128) get_power_of_ten(int n);
void      scale_by_power_of_ten_N(tree value,
                                int N,
                                bool check_for_fractional = false);
tree      scale_by_power_of_ten(tree value,
                                tree N,
                                bool check_for_fractional = false);
void      scale_and_round(tree value,
                          int  value_rdigits,
                          bool target_is_signable,
                          int  target_rdigits,
                          cbl_round_t rounded);
void      hex_dump(tree data, size_t bytes);
void      set_exception_code_func(ec_type_t ec,
                                  int line,
                                  int from_raise_statement=0);
#define set_exception_code(ec) set_exception_code_func(ec, __LINE__)
bool      process_this_exception(const ec_type_t ec);
#define   CHECK_FOR_FRACTIONAL_DIGITS true
void      get_integer_value(tree value,  // This is always a LONG
                            cbl_field_t *field,
                            tree         offset=NULL,  // size_t
                            bool check_for_fractional_digits=false);
void      rt_error(const char *msg);
tree      build_array_of_size_t( size_t  N,
                                 const size_t *values);
void      parser_display_internal_field(tree file_descriptor,
                                        cbl_field_t *field,
                                        bool advance=DISPLAY_NO_ADVANCE);
char     *get_literal_string(cbl_field_t *field);

bool      refer_is_clean(const cbl_refer_t &refer);
bool      refer_is_super_clean(const cbl_refer_t &refer);
bool      refer_is_working_storage(const cbl_refer_t &refer);

tree      refer_offset(const cbl_refer_t &refer, int *pflags=NULL);
tree      refer_size_source(const cbl_refer_t &refer);
tree      refer_size_dest(const cbl_refer_t &refer);

tree      qualified_data_location(const cbl_refer_t &refer);

tree      build_array_of_referlets( size_t N,
                                    cbl_refer_t *refers);

tree      build_array_of_refers(size_t N,
                                cbl_refer_t *refers);
void      get_depending_on_value_from_odo(tree retval, cbl_field_t *odo);
uint64_t  get_time_nanoseconds();

bool      is_pure_integer(const cbl_field_t *field);

tree      tree_type_from_field(const cbl_field_t *field);
tree      tree_type_from_refer(const cbl_refer_t &refer);

bool      get_binary_value(tree &value, 
                     const cbl_refer_t &refer,
                           tree type = NULL_TREE);
void      get_location(tree &retval, const cbl_refer_t &refer);
void      get_length(tree &retval, const cbl_refer_t &refer);

void treeplet_fill_source(TREEPLET &treeplet, const cbl_refer_t &refer);

tree data_decl_type_for(cbl_field_t *field);

void attribute_bit_clear(struct cbl_field_t *var, cbl_field_attr_t bits);
tree attribute_bit_get(struct cbl_field_t *var, cbl_field_attr_t bits);
void attribute_bit_set(struct cbl_field_t *var, cbl_field_attr_t bits);

#endif
