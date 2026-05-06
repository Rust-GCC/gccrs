/* Capturing the results of pretty_print for later playback.
   Copyright (C) 2023-2026 Free Software Foundation, Inc.
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
#define INCLUDE_STRING
#include "system.h"
#include "coretypes.h"
#include "intl.h"
#include "pretty-print.h"
#include "pretty-print-token-buffer.h"
#include "selftest.h"

/* A token_printer that makes a deep copy of the pp_token_list
   into another obstack.  */

class copying_token_printer : public token_printer
{
public:
  copying_token_printer (obstack &dst_obstack,
			 pp_token_list &dst_token_list)
  : m_dst_obstack (dst_obstack),
    m_dst_token_list (dst_token_list)
  {
  }

  void
  print_tokens (pretty_printer *,
		const pp_token_list &tokens) final override
  {
    for (auto iter = tokens.m_first; iter; iter = iter->m_next)
      switch (iter->m_kind)
	{
	default:
	  gcc_unreachable ();

	case pp_token::kind::text:
	  {
	    const pp_token_text *sub = as_a <const pp_token_text *> (iter);
	    /* Copy the text, with null terminator.  */
	    obstack_grow (&m_dst_obstack, sub->m_value.get (),
			  strlen (sub->m_value.get ()) + 1);
	    m_dst_token_list.push_back_text
	      (label_text::borrow (XOBFINISH (&m_dst_obstack,
					      const char *)));
	  }
	  break;

	case pp_token::kind::begin_color:
	  {
	    pp_token_begin_color *sub = as_a <pp_token_begin_color *> (iter);
	    /* Copy the color, with null terminator.  */
	    obstack_grow (&m_dst_obstack, sub->m_value.get (),
			  strlen (sub->m_value.get ()) + 1);
	    m_dst_token_list.push_back<pp_token_begin_color>
	      (label_text::borrow (XOBFINISH (&m_dst_obstack,
					      const char *)));
	  }
	  break;
	case pp_token::kind::end_color:
	  m_dst_token_list.push_back<pp_token_end_color> ();
	  break;

	case pp_token::kind::begin_quote:
	  m_dst_token_list.push_back<pp_token_begin_quote> ();
	  break;
	case pp_token::kind::end_quote:
	  m_dst_token_list.push_back<pp_token_end_quote> ();
	  break;

	case pp_token::kind::begin_url:
	  {
	    pp_token_begin_url *sub = as_a <pp_token_begin_url *> (iter);
	    /* Copy the URL, with null terminator.  */
	    obstack_grow (&m_dst_obstack, sub->m_value.get (),
			  strlen (sub->m_value.get ()) + 1);
	    m_dst_token_list.push_back<pp_token_begin_url>
	      (label_text::borrow (XOBFINISH (&m_dst_obstack,
					      const char *)));
	  }
	  break;
	case pp_token::kind::end_url:
	  m_dst_token_list.push_back<pp_token_end_url> ();
	  break;

	case pp_token::kind::event_id:
	  {
	    pp_token_event_id *sub = as_a <pp_token_event_id *> (iter);
	    m_dst_token_list.push_back<pp_token_event_id> (sub->m_event_id);
	  }
	  break;

	case pp_token::kind::custom_data:
	  /* These should have been eliminated by replace_custom_tokens.  */
	  gcc_unreachable ();
	  break;
	}
  }

private:
  obstack &m_dst_obstack;
  pp_token_list &m_dst_token_list;
};

pretty_print_token_buffer::pretty_print_token_buffer ()
: m_obstack (std::make_unique<auto_obstack> ()),
  m_tokens (*m_obstack.get ())
{
}

/* Capture GMSGID and ARGS as a sequence of pretty_print tokens.  */

pretty_print_token_buffer::pretty_print_token_buffer (const char *gmsgid,
						      va_list *args)
: m_obstack (std::make_unique<auto_obstack> ()),
  m_tokens (*m_obstack.get ())
{
  text_info text (gmsgid, args, errno);
  pretty_printer pp;
  pp.set_output_stream (nullptr);
  copying_token_printer tok_printer (*m_obstack.get (), m_tokens);
  pp.set_token_printer (&tok_printer);
  pp_format (&pp, &text);
  pp_output_formatted_text (&pp, nullptr);
}

pretty_print_token_buffer::
pretty_print_token_buffer (pretty_print_token_buffer &&other)
: m_obstack (std::move (other.m_obstack)),
  m_tokens (std::move (other.m_tokens))
{
}

/* Convert to text, dropping colorization, URLs, etc.  */

std::string
pretty_print_token_buffer::to_string () const
{
  std::string result;

  for (auto iter = m_tokens.m_first; iter; iter = iter->m_next)
    switch (iter->m_kind)
      {
      default:
	gcc_unreachable ();

      case pp_token::kind::text:
	{
	  pp_token_text *sub = as_a <pp_token_text *> (iter);
	  result += sub->m_value.get ();
	}
	break;

      case pp_token::kind::begin_color:
      case pp_token::kind::end_color:
	// Skip
	break;

      case pp_token::kind::begin_quote:
	result += open_quote;
	break;

      case pp_token::kind::end_quote:
	result += close_quote;
	break;

      case pp_token::kind::begin_url:
      case pp_token::kind::end_url:
	// Skip
	break;

      case pp_token::kind::event_id:
	{
	  pp_token_event_id *sub = as_a <pp_token_event_id *> (iter);
	  gcc_assert (sub->m_event_id.known_p ());
	  result += '(';
	  result += std::to_string (sub->m_event_id.one_based ());
	  result += ')';
	}
	break;

      case pp_token::kind::custom_data:
	/* We don't have a way of handling custom_data tokens here.  */
	gcc_unreachable ();
	break;
      }

  return result;
}

// class pp_token_buffer_element : public pp_element

void
pp_token_buffer_element::add_to_phase_2 (pp_markup::context &ctxt)
{
  for (auto iter = m_token_buf.m_tokens.m_first; iter; iter = iter->m_next)
    switch (iter->m_kind)
      {
      default:
	gcc_unreachable ();

      case pp_token::kind::text:
	{
	  const pp_token_text *sub = as_a <const pp_token_text *> (iter);
	  pp_string (&ctxt.m_pp, sub->m_value.get ());
	  ctxt.push_back_any_text ();
	}
	break;

      case pp_token::kind::begin_color:
	{
	  const pp_token_begin_color *sub
	    = as_a <const pp_token_begin_color *> (iter);
	  gcc_assert (sub->m_value.get ());
	  ctxt.begin_highlight_color (sub->m_value.get ());
	}
	break;

      case pp_token::kind::end_color:
	ctxt.end_highlight_color ();
	break;

      case pp_token::kind::begin_quote:
	ctxt.begin_quote ();
	break;

      case pp_token::kind::end_quote:
	ctxt.end_quote ();
	break;

      case pp_token::kind::begin_url:
	{
	  const pp_token_begin_url *sub
	    = as_a <const pp_token_begin_url *> (iter);
	  gcc_assert (sub->m_value.get ());
	  ctxt.begin_url (sub->m_value.get ());
	}
	break;

      case pp_token::kind::end_url:
	ctxt.end_url ();
	break;

      case pp_token::kind::event_id:
	{
	  const pp_token_event_id *sub
	    = as_a <const pp_token_event_id *> (iter);
	  gcc_assert (sub->m_event_id.known_p ());
	  ctxt.add_event_id (sub->m_event_id);
	}
	break;

      case pp_token::kind::custom_data:
	/* We don't have a way of handling custom_data tokens here.  */
	gcc_unreachable ();
	break;
      }
}

#if CHECKING_P

namespace selftest {

static pretty_print_token_buffer
pp_printf_to_buf (const char *fmt, ...)
{
  va_list args;
  va_start (args, fmt);

  pretty_print_token_buffer buf (fmt, &args);

  va_end (args);

  return buf;
}

static void
test_empty ()
{
  pretty_print_token_buffer buf;
  pp_token_buffer_element e (buf);
  pretty_printer pp;
  pp_printf (&pp, "before %e after", &e);
  ASSERT_STREQ (pp_formatted_text (&pp), "before  after");
}

static void
test_print ()
{
  pretty_print_token_buffer buf
    = pp_printf_to_buf ("x: %qs y: %qs", "foo", "bar");

  // Check that the individual tokens are captured in "buf".
  pp_token *tok0 = buf.m_tokens.m_first;
  ASSERT_EQ (tok0->m_kind, pp_token::kind::text);
  ASSERT_STREQ (((pp_token_text *)tok0)->m_value.get (),
		"x: ");

  pp_token *tok1 = tok0->m_next;
  ASSERT_EQ (tok1->m_kind, pp_token::kind::begin_quote);

  pp_token *tok2 = tok1->m_next;
  ASSERT_EQ (tok2->m_kind, pp_token::kind::text);
  ASSERT_STREQ (((pp_token_text *)tok2)->m_value.get (),
		"foo");

  pp_token *tok3 = tok2->m_next;
  ASSERT_EQ (tok3->m_kind, pp_token::kind::end_quote);

  pp_token *tok4 = tok3->m_next;
  ASSERT_EQ (tok4->m_kind, pp_token::kind::text);
  ASSERT_STREQ (((pp_token_text *)tok4)->m_value.get (),
		" y: ");

  pp_token *tok5 = tok4->m_next;
  ASSERT_EQ (tok5->m_kind, pp_token::kind::begin_quote);

  pp_token *tok6 = tok5->m_next;
  ASSERT_EQ (tok6->m_kind, pp_token::kind::text);
  ASSERT_STREQ (((pp_token_text *)tok6)->m_value.get (),
		"bar");

  pp_token *tok7 = tok6->m_next;
  ASSERT_EQ (tok7->m_kind, pp_token::kind::end_quote);
  ASSERT_EQ (tok7->m_next, nullptr);


  // Check that we can replay buf via pp_token_buffer_element
  pp_token_buffer_element e (buf);
  pretty_printer pp;
  pp_printf (&pp, "before %e after", &e);
  ASSERT_STREQ (pp_formatted_text (&pp), "before x: 'foo' y: 'bar' after");
}

/* Run all of the selftests within this file.  */

void
pretty_print_token_buffer_cc_tests ()
{
  test_empty ();
  test_print ();
}

} // namespace selftest

#endif /* CHECKING_P */
