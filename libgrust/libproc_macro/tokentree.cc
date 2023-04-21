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

#include "tokentree.h"

namespace TokenTree {

TokenTree
TokenTree::make_tokentree (Group::Group group)
{
  TokenTreePayload payload;
  payload.group = group;
  return {GROUP, payload};
}

TokenTree
TokenTree::make_tokentree (Ident::Ident ident)
{
  TokenTreePayload payload;
  payload.ident = ident;
  return {IDENT, payload};
}

TokenTree
TokenTree::make_tokentree (Punct::Punct punct)
{
  TokenTreePayload payload;
  payload.punct = punct;
  return {PUNCT, payload};
}

TokenTree
TokenTree::make_tokentree (Literal::Literal literal)
{
  TokenTreePayload payload;
  payload.literal = literal;
  return {LITERAL, payload};
}

} // namespace TokenTree
