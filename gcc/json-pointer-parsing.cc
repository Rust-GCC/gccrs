/* JSON Pointer parsing (RFC 6901).
   Copyright (C) 2026 Free Software Foundation, Inc.
   Contributed by David Malcolm <dmalcolm@redhat.com>.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#define INCLUDE_MAP
#define INCLUDE_STRING
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "json-pointer-parsing.h"
#include "pretty-print.h"
#include "pretty-print-markup-json.h"
#include "selftest.h"

/* Implementation of json::pointer parsing.  */

namespace {

class json_pointer_parser
{
public:
  json::pointer::parser_result_t
  parse_utf8_string (const char *utf8_json_pointer,
		     const json::value *root_val);

private:
  std::unique_ptr<json::pointer::error>
  make_error (const char *fmt, ...);

  json::result<size_t, std::unique_ptr<json::pointer::error>>
  parse_array_index (const std::string &reftoken);
};

} // anonymous namespace

/* Parse JSON pointer.  */

json::pointer::parser_result_t
json_pointer_parser::parse_utf8_string (const char *utf8_json_pointer,
					const json::value *root_val)
{
  const char *ch_iter = utf8_json_pointer;
  const json::value *cur_val = root_val;

  while (*ch_iter)
    {
      // Try to consume a reference-token
      if (*ch_iter != '/')
	return make_error ("malformed JSON Pointer: expected %qs; got %qc",
			   "/", *ch_iter);
      ch_iter++;
      std::string reftoken;
      while (char ch = *ch_iter)
	{
	  /* End at end of string, or unescaped '/'.  */
	  if (ch == '/' || ch == '\0')
	    break;
	  ch_iter++;
	  if (ch == '~')
	    {
	      switch (*ch_iter)
		{
		case '0':
		  reftoken += '~';
		  ch_iter++;
		  break;
		case '1':
		  reftoken += '/';
		  ch_iter++;
		  break;
		default:
		  if (*ch_iter)
		    return make_error(("malformed JSON Pointer:"
				       " expected %qs or %qs after %qs;"
				       " got %qc"),
				      "0", "1", "~", *ch_iter);
		  else
		    return make_error (("malformed JSON Pointer:"
					" expected %qs or %qs after %qs"),
				       "0", "1", "~");
		}
	    }
	  else
	    reftoken += ch;
	}
      switch (cur_val->get_kind ())
	{
	default:
	  gcc_unreachable ();

	case json::JSON_OBJECT:
	  {
	    const json::object *cur_obj
	      = static_cast<const json::object *> (cur_val);
	    if (const json::value *child = cur_obj->get (reftoken.c_str ()))
	      cur_val = child;
	    else
	      {
		pp_markup::quoted_json_pointer obj_pointer (*cur_val);
		return make_error ("unknown member %qs within object %e",
				   reftoken.c_str (), &obj_pointer);
	      }
	  }
	  break;
	case json::JSON_ARRAY:
	  {
	    auto array_idx_res = parse_array_index (reftoken);
	    if (array_idx_res.m_err)
	      return std::move (array_idx_res.m_err);

	    const json::array *cur_arr
	      = static_cast<const json::array *> (cur_val);
	    if (array_idx_res.m_val < cur_arr->size ())
	      cur_val = (*cur_arr)[array_idx_res.m_val];
	    else
	      {
		pp_markup::quoted_json_pointer array_pointer (*cur_val);
		return make_error
		  ("array index %li out of range for array %e",
		   array_idx_res.m_val,
		   &array_pointer);
	      }
	  }
	  break;

	case json::JSON_INTEGER:
	case json::JSON_FLOAT:
	  {
	    pp_markup::quoted_json_pointer cur_val_ptr (*cur_val);
	    return make_error
	      (("expected object or array for reference token %qs;"
		" %e is a number"),
	       reftoken.c_str (),
	       &cur_val_ptr);
	  }
	case json::JSON_STRING:
	  {
	    pp_markup::quoted_json_pointer cur_val_ptr (*cur_val);
	    return make_error
	      (("expected object or array for reference token %qs;"
		" %e is a string"),
	       reftoken.c_str (),
	       &cur_val_ptr);
	  }
	case json::JSON_TRUE:
	case json::JSON_FALSE:
	case json::JSON_NULL:
	  {
	    pp_markup::quoted_json_pointer cur_val_ptr (*cur_val);
	    return make_error
	      (("expected object or array for reference token %qs;"
		" %e is a JSON literal"),
	       reftoken.c_str (),
	       &cur_val_ptr);
	  }
	}
    }

  return cur_val;
}

std::unique_ptr<json::pointer::error>
json_pointer_parser::make_error (const char *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);
  auto err = std::make_unique<json::pointer::error>
    (pretty_print_token_buffer (fmt, &ap));
  va_end (ap);
  return err;
}

/* Parse array-index: '0', or decimal digits without a leading '0'.  */

json::result<size_t, std::unique_ptr<json::pointer::error>>
json_pointer_parser::parse_array_index (const std::string &reftoken)
{
  if (reftoken == "0")
    return 0;

  /* Decimal digits without a leading '0'.  */
  if (reftoken[0] < '1' || reftoken[0] > '9')
    return make_error ("malformed JSON Pointer: bad array index: %qs",
		       reftoken.c_str ());

  size_t result = 0;
  for (auto digit : reftoken)
    {
      result *= 10;
      if (digit < '0' || digit > '9')
	return make_error ("malformed JSON Pointer: bad array index: %qs",
			   reftoken.c_str ());
      result += digit - '0';
    }
  return result;
}

json::pointer::parser_result_t
json::pointer::parse_utf8_string (const char *utf8_json_pointer,
				  const json::value *root_val)
{
  json_pointer_parser p;
  return p.parse_utf8_string (utf8_json_pointer, root_val);
}


#if CHECKING_P

namespace selftest {

/* Implementation detail of ASSERT_PARSE_JSON_POINTER_EQ.  */

static void
assert_parse_json_pointer_eq (const location &loc,
			      const char *utf8_json_pointer,
			      const json::value *root_val,
			      const json::value *expected_jv)
{
  auto res = json::pointer::parse_utf8_string (utf8_json_pointer, root_val);
  ASSERT_EQ_AT (loc, res.m_err, nullptr);
  ASSERT_EQ_AT (loc, res.m_val, expected_jv);
}

/* Assert that JSON_POINTER, a const char *, is a valid JSON pointer into
   ROOT_VAL, and equals EXPECTED_JV.  */
#define ASSERT_PARSE_JSON_POINTER_EQ(JSON_POINTER, ROOT_VAL, EXPECTED_JV) \
  assert_parse_json_pointer_eq ((SELFTEST_LOCATION), (JSON_POINTER), \
				(ROOT_VAL), (EXPECTED_JV))

/* Implementation detail of ASSERT_DUMP_FROM_JSON_POINTER_STREQ.  */

static void
assert_dump_from_json_pointer_streq (const location &loc,
				     const json::value *root_val,
				     const char *utf8_json_pointer,
				     const char *expected_dump)
{
  auto res = json::pointer::parse_utf8_string (utf8_json_pointer, root_val);
  ASSERT_EQ_AT (loc, res.m_err, nullptr);

  pretty_printer pp;
  res.m_val->print (&pp, false);
  ASSERT_STREQ_AT (loc, pp_formatted_text (&pp), expected_dump);
}

/* Assert that JSON_POINTER (a const char *) successfully looks up a value
   relative to ROOT_VAL, and that the resulting value dumps as
   EXPECTED_DUMP.  */
#define ASSERT_DUMP_FROM_JSON_POINTER_STREQ(ROOT_VAL, JSON_POINTER, EXPECTED_DUMP) \
  assert_dump_from_json_pointer_streq ((SELFTEST_LOCATION), (ROOT_VAL),	\
				       (JSON_POINTER), (EXPECTED_DUMP))

/* Implementation detail of ASSERT_JSON_POINTER_ERR.  */

static void
assert_json_pointer_err (const location &loc,
			 const char *utf8_json_pointer,
			 const json::value *root_val,
			 const char *expected_err)
{
  auto res = json::pointer::parse_utf8_string (utf8_json_pointer, root_val);
  ASSERT_EQ_AT (loc, res.m_val, nullptr);
  ASSERT_NE_AT (loc, res.m_err, nullptr);
  std::string err_str = res.m_err->m_tokens.to_string ();
  ASSERT_STREQ_AT (loc, err_str.c_str (), expected_err);
}

/* Assert that JSON_POINTER (a const char *) fails to look up a value
   relative to ROOT_VAL, and that the resulting error expressed as a string
   is EXPECTED_ERR.  */
#define ASSERT_JSON_POINTER_ERR(JSON_POINTER, ROOT_VAL, EXPECTED_ERR) \
  assert_json_pointer_err ((SELFTEST_LOCATION), (JSON_POINTER),       \
			   (ROOT_VAL), (EXPECTED_ERR))

/* Selftests.  */

static void
test_simple ()
{
  json::object obj;
  auto js_bar = obj.set_string ("foo", "bar");
  auto baz = std::make_unique<json::array> ();
  json::array *js_baz = baz.get ();
  auto js_str0 = baz->append_string ("x");
  auto js_str1 = baz->append_string ("y");
  obj.set ("baz", std::move (baz));

  ASSERT_PARSE_JSON_POINTER_EQ ("", &obj, &obj);
  ASSERT_PARSE_JSON_POINTER_EQ ("/foo", &obj, js_bar);
  ASSERT_PARSE_JSON_POINTER_EQ ("/baz", &obj, js_baz);
  ASSERT_PARSE_JSON_POINTER_EQ ("/baz/0", &obj, js_str0);
  ASSERT_PARSE_JSON_POINTER_EQ ("/baz/1", &obj, js_str1);
}

/* Verify that JSON Pointers are correctly escaped.  */

static void
test_escaping_1 ()
{
  json::object obj;
  auto js_a_slash_b = obj.set_integer ("a/b", 1);
  auto js_m_tilde_n = obj.set_integer ("m~n", 8);
  auto js_tilde_1 = obj.set_integer ("~1", 9);

  ASSERT_PARSE_JSON_POINTER_EQ ("/a~1b", &obj, js_a_slash_b);
  ASSERT_PARSE_JSON_POINTER_EQ ("/m~0n", &obj, js_m_tilde_n);
  ASSERT_PARSE_JSON_POINTER_EQ ("/~01", &obj, js_tilde_1);
}

static void
test_escaping_2 ()
{
  /* The example from RFC 6901 section 5.  */
  char *path = locate_file ("json-pointer.json");
  char *js_doc = selftest::read_file (SELFTEST_LOCATION, path);
  free (path);
  auto res = json::parse_utf8_string (js_doc, true, nullptr);
  ASSERT_EQ (res.m_err, nullptr);
  free (js_doc);

  auto root = res.m_val.get ();
  ASSERT_PARSE_JSON_POINTER_EQ ("", root, root);
  ASSERT_DUMP_FROM_JSON_POINTER_STREQ (root, "/foo",   "[\"bar\", \"baz\"]");
  ASSERT_DUMP_FROM_JSON_POINTER_STREQ (root, "/foo/0", "\"bar\"");
  ASSERT_DUMP_FROM_JSON_POINTER_STREQ (root, "/",      "0");
  ASSERT_DUMP_FROM_JSON_POINTER_STREQ (root, "/a~1b",  "1");
  ASSERT_DUMP_FROM_JSON_POINTER_STREQ (root, "/c%d" ,  "2");
  ASSERT_DUMP_FROM_JSON_POINTER_STREQ (root, "/e^f" ,  "3");
  ASSERT_DUMP_FROM_JSON_POINTER_STREQ (root, "/g|h" ,  "4");
  ASSERT_DUMP_FROM_JSON_POINTER_STREQ (root, "/i\\j",  "5");
  ASSERT_DUMP_FROM_JSON_POINTER_STREQ (root, "/k\"l",  "6");
  ASSERT_DUMP_FROM_JSON_POINTER_STREQ (root, "/ "   ,  "7");
  ASSERT_DUMP_FROM_JSON_POINTER_STREQ (root, "/m~0n",  "8");
}

static void
test_errors ()
{
  json::object obj;
  json::array arr;
  ASSERT_JSON_POINTER_ERR ("foo", &obj,
			   "malformed JSON Pointer: expected '/'; got 'f'");
  ASSERT_JSON_POINTER_ERR ("/~", &obj,
			   "malformed JSON Pointer: expected '0' or '1' after '~'");
  ASSERT_JSON_POINTER_ERR ("/~~", &obj,
			   "malformed JSON Pointer: expected '0' or '1' after '~';"
			   " got '~'");
  ASSERT_JSON_POINTER_ERR ("/foo", &obj,
			   "unknown member 'foo' within object ''");
  ASSERT_JSON_POINTER_ERR ("/0", &obj,
			   "unknown member '0' within object ''");
  ASSERT_JSON_POINTER_ERR ("/0", &arr,
			   "array index 0 out of range for array ''");
  ASSERT_JSON_POINTER_ERR ("/-1", &arr,
			   "malformed JSON Pointer: bad array index: '-1'");
  ASSERT_JSON_POINTER_ERR ("/8a", &arr,
			   "malformed JSON Pointer: bad array index: '8a'");

  {
    json::integer_number js_int (42);
    json::float_number js_float (42);
    json::string js_str ("foo");
    json::literal js_true (json::JSON_TRUE);
    ASSERT_JSON_POINTER_ERR
      ("/foo", &js_int,
       "expected object or array for reference token 'foo';"
       " '' is a number");
    ASSERT_JSON_POINTER_ERR
      ("/foo", &js_float,
       "expected object or array for reference token 'foo';"
       " '' is a number");
    ASSERT_JSON_POINTER_ERR
      ("/foo", &js_str,
       "expected object or array for reference token 'foo';"
       " '' is a string");
    ASSERT_JSON_POINTER_ERR
      ("/foo", &js_true,
       "expected object or array for reference token 'foo';"
       " '' is a JSON literal");
  }

  /* RFC 6901 section 4 "Evaluation" has:
     "the string '~01' correctly becomes '~1' after transformation".  */
  ASSERT_JSON_POINTER_ERR ("/~01", &obj,
			   "unknown member '~1' within object ''");
}

/* Run all of the selftests within this file.  */

void
json_pointer_parsing_cc_tests ()
{
  test_simple ();
  test_escaping_1 ();
  test_escaping_2 ();
  test_errors ();
}

} // namespace selftest

#endif /* #if CHECKING_P */
