// Copyright (C) 2023 Free Software Foundation, Inc.
//
// This file is part of the GNU Proc Macro Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

#ifndef TOKENSTREAM_H
#define TOKENSTREAM_H

#include <cstdint>
#include <vector>

namespace TokenTree {
struct TokenTree;
}

namespace TokenStream {

const std::int64_t DEFAULT_CAPACITY = 0;

struct TokenStream
{
  TokenTree::TokenTree *data;
  std::uint64_t size;
  std::uint64_t capacity;

public:
  void grow (std::uint64_t delta);
  void push (TokenTree::TokenTree tree);

  TokenStream clone () const;

  static TokenStream make_tokenstream (std::vector<TokenTree::TokenTree> vec);
  static TokenStream make_tokenstream (std::uint64_t capacity
				       = DEFAULT_CAPACITY);
};

extern "C" TokenStream
TokenStream__new ();

extern "C" TokenStream
TokenStream__with_capacity (std::uint64_t capacity);

extern "C" void
TokenSream__push (TokenStream *stream, TokenTree::TokenTree tree);

extern "C" bool
TokenStream__from_string (unsigned char *str, std::uint64_t len,
			  TokenStream *ts);

extern "C" TokenStream
TokenStream__clone (const TokenStream *ts);

extern "C" void
TokenStream__drop (TokenStream *stream);

} // namespace TokenStream

#endif /* ! TOKENSTREAM_H */
