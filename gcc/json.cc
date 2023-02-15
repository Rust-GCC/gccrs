/* JSON trees
   Copyright (C) 2017-2023 Free Software Foundation, Inc.
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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "json.h"
#include "pretty-print.h"
#include "math.h"
#include "selftest.h"

using namespace json;

/* class json::value.  */

/* Dump this json::value tree to OUTF.
   No formatting is done.  There are no guarantees about the order
   in which the key/value pairs of json::objects are printed.  */

void
value::dump (FILE *outf) const
{
  pretty_printer pp;
  pp_buffer (&pp)->stream = outf;
  print (&pp);
  pp_flush (&pp);
}

/* class json::object, a subclass of json::value, representing
   an unordered collection of key/value pairs.  */

/* json:object's dtor.  */

object::~object ()
{
  for (map_t::iterator it = m_map.begin (); it != m_map.end (); ++it)
    {
      free (const_cast <char *>((*it).first));
      delete ((*it).second);
    }
}

/* Implementation of json::value::print for json::object.  */

void
object::print (pretty_printer *pp) const
{
  /* Note that the order is not guaranteed.  */
  pp_character (pp, '{');
  for (map_t::iterator it = m_map.begin (); it != m_map.end (); ++it)
    {
      if (it != m_map.begin ())
	pp_string (pp, ", ");
      const char *key = const_cast <char *>((*it).first);
      value *value = (*it).second;
      pp_doublequote (pp);
      pp_string (pp, key); // FIXME: escaping?
      pp_doublequote (pp);
      pp_string (pp, ": ");
      value->print (pp);
    }
  pp_character (pp, '}');
}

/* Set the json::value * for KEY, taking ownership of V
   (and taking a copy of KEY if necessary).  */

void
object::set (const char *key, value *v)
{
  gcc_assert (key);
  gcc_assert (v);

  value **ptr = m_map.get (key);
  if (ptr)
    {
      /* If the key is already present, delete the existing value
	 and overwrite it.  */
      delete *ptr;
      *ptr = v;
    }
  else
    /* If the key wasn't already present, take a copy of the key,
       and store the value.  */
    m_map.put (xstrdup (key), v);
}

/* Get the json::value * for KEY.

   The object retains ownership of the value.  */

value *
object::get (const char *key) const
{
  gcc_assert (key);

  value **ptr = const_cast <map_t &> (m_map).get (key);
  if (ptr)
    return *ptr;
  else
    return NULL;
}

/* class json::array, a subclass of json::value, representing
   an ordered collection of values.  */

/* json::array's dtor.  */

array::~array ()
{
  unsigned i;
  value *v;
  FOR_EACH_VEC_ELT (m_elements, i, v)
    delete v;
}

/* Implementation of json::value::print for json::array.  */

void
array::print (pretty_printer *pp) const
{
  pp_character (pp, '[');
  unsigned i;
  value *v;
  FOR_EACH_VEC_ELT (m_elements, i, v)
    {
      if (i)
	pp_string (pp, ", ");
      v->print (pp);
    }
  pp_character (pp, ']');
}

/* Append non-NULL value V to a json::array, taking ownership of V.  */

void
array::append (value *v)
{
  gcc_assert (v);
  m_elements.safe_push (v);
}

/* class json::float_number, a subclass of json::value, wrapping a double.  */

/* Implementation of json::value::print for json::float_number.  */

void
float_number::print (pretty_printer *pp) const
{
  char tmp[1024];
  snprintf (tmp, sizeof (tmp), "%g", m_value);
  pp_string (pp, tmp);
}

/* class json::integer_number, a subclass of json::value, wrapping a long.  */

/* Implementation of json::value::print for json::integer_number.  */

void
integer_number::print (pretty_printer *pp) const
{
  char tmp[1024];
  snprintf (tmp, sizeof (tmp), "%ld", m_value);
  pp_string (pp, tmp);
}


/* class json::string, a subclass of json::value.  */

/* json::string's ctor.  */

string::string (const char *utf8)
{
  gcc_assert (utf8);
  m_utf8 = xstrdup (utf8);
  m_len = strlen (utf8);
}

string::string (const char *utf8, size_t len)
{
  gcc_assert (utf8);
  m_utf8 = XNEWVEC (char, len);
  m_len = len;
  memcpy (m_utf8, utf8, len);
}

/* Implementation of json::value::print for json::string.  */

void
string::print (pretty_printer *pp) const
{
  pp_character (pp, '"');
  for (size_t i = 0; i != m_len; ++i)
    {
      char ch = m_utf8[i];
      switch (ch)
	{
	case '"':
	  pp_string (pp, "\\\"");
	  break;
	case '\\':
	  pp_string (pp, "\\\\");
	  break;
	case '\b':
	  pp_string (pp, "\\b");
	  break;
	case '\f':
	  pp_string (pp, "\\f");
	  break;
	case '\n':
	  pp_string (pp, "\\n");
	  break;
	case '\r':
	  pp_string (pp, "\\r");
	  break;
	case '\t':
	  pp_string (pp, "\\t");
	  break;
	case '\0':
	  pp_string (pp, "\\0");
	  break;
	default:
	  pp_character (pp, ch);
	}
    }
  pp_character (pp, '"');
}

/* class json::literal, a subclass of json::value.  */

/* Implementation of json::value::print for json::literal.  */

void
literal::print (pretty_printer *pp) const
{
  switch (m_kind)
    {
    case JSON_TRUE:
      pp_string (pp, "true");
      break;
    case JSON_FALSE:
      pp_string (pp, "false");
      break;
    case JSON_NULL:
      pp_string (pp, "null");
      break;
    default:
      gcc_unreachable ();
    }
}


#if CHECKING_P

namespace selftest {

/* Selftests.  */

/* Verify that JV->print () prints EXPECTED_JSON.  */

static void
assert_print_eq (const json::value &jv, const char *expected_json)
{
  pretty_printer pp;
  jv.print (&pp);
  ASSERT_STREQ (expected_json, pp_formatted_text (&pp));
}

/* Verify that object::get works as expected.  */

static void
test_object_get ()
{
  object obj;
  value *val = new json::string ("value");
  obj.set ("foo", val);
  ASSERT_EQ (obj.get ("foo"), val);
  ASSERT_EQ (obj.get ("not-present"), NULL);
}

/* Verify that JSON objects are written correctly.  We can't test more than
   one key/value pair, as we don't impose a guaranteed ordering.  */

static void
test_writing_objects ()
{
  object obj;
  obj.set ("foo", new json::string ("bar"));
  assert_print_eq (obj, "{\"foo\": \"bar\"}");
}

/* Verify that JSON arrays are written correctly.  */

static void
test_writing_arrays ()
{
  array arr;
  assert_print_eq (arr, "[]");

  arr.append (new json::string ("foo"));
  assert_print_eq (arr, "[\"foo\"]");

  arr.append (new json::string ("bar"));
  assert_print_eq (arr, "[\"foo\", \"bar\"]");
}

/* Verify that JSON numbers are written correctly.  */

static void
test_writing_float_numbers ()
{
  assert_print_eq (float_number (0), "0");
  assert_print_eq (float_number (42), "42");
  assert_print_eq (float_number (-100), "-100");
  assert_print_eq (float_number (123456789), "1.23457e+08");
}

static void
test_writing_integer_numbers ()
{
  assert_print_eq (integer_number (0), "0");
  assert_print_eq (integer_number (42), "42");
  assert_print_eq (integer_number (-100), "-100");
  assert_print_eq (integer_number (123456789), "123456789");
  assert_print_eq (integer_number (-123456789), "-123456789");
}

/* Verify that JSON strings are written correctly.  */

static void
test_writing_strings ()
{
  string foo ("foo");
  assert_print_eq (foo, "\"foo\"");

  string contains_quotes ("before \"quoted\" after");
  assert_print_eq (contains_quotes, "\"before \\\"quoted\\\" after\"");

  const char data[] = {'a', 'b', 'c', 'd', '\0', 'e', 'f'};
  string not_terminated (data, 3);
  assert_print_eq (not_terminated, "\"abc\"");
  string embedded_null (data, sizeof data);
  assert_print_eq (embedded_null, "\"abcd\\0ef\"");
}

/* Verify that JSON literals are written correctly.  */

static void
test_writing_literals ()
{
  assert_print_eq (literal (JSON_TRUE), "true");
  assert_print_eq (literal (JSON_FALSE), "false");
  assert_print_eq (literal (JSON_NULL), "null");

  assert_print_eq (literal (true), "true");
  assert_print_eq (literal (false), "false");
}

/* Run all of the selftests within this file.  */

void
json_cc_tests ()
{
  test_object_get ();
  test_writing_objects ();
  test_writing_arrays ();
  test_writing_float_numbers ();
  test_writing_integer_numbers ();
  test_writing_strings ();
  test_writing_literals ();
}

} // namespace selftest

#endif /* #if CHECKING_P */
