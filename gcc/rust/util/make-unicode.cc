// Copyright (C) 2020-2023 Free Software Foundation, Inc.

// This file is part of GCC.

// GCC is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3, or (at your option) any later
// version.

// GCC is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.

// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

/* Run this program as
	./make-rust-unicode UnicodeData.txt > rust-unicode.cc
*/

#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#define NUM_CODE_POINTS 0x110000

const std::string COPYRIGHT = {
  R"(// Copyright (C) 2020-2023 Free Software Foundation, Inc.

// This file is part of GCC.

// GCC is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3, or (at your option) any later
// version.

// GCC is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.

// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.)"};

static unsigned int decomposed[NUM_CODE_POINTS][2];

// this contains mapping B->A iff Codepoint A decomposes to B
static std::map<unsigned int, unsigned int> compose_map1{};

// this contains mapping B->[(A, C), (D, E), ...]
// iff Codepoint A decomposes to B+C, and D decomposes to B+E ...
static std::map<unsigned int,
		std::vector<std::pair<unsigned int, unsigned int>>>
  compose_map2{};

void
fail (std::string s)
{
  std::cerr << s << std::endl;
  exit (1);
}

// Utility to split string with delimiters
std::vector<std::string>
split (const std::string &s, char delim)
{
  std::vector<std::string> ret{};
  std::string current{};
  for (char c : s)
    {
      if (c == delim)
	{
	  ret.push_back (current);
	  current.clear ();
	}
      else
	{
	  current += c;
	}
    }
  ret.push_back (current);
  return ret;
}

void
read_unicode_data_txt (char *fname)
{
  std::ifstream file ("UnicodeData.txt");

  std::string line;
  for (std::string line; getline (file, line);)
    {
      std::vector<std::string> col = split (line, ';');
      if (col.size () == 0)
	break;
      if (col.size () != 15)
	fail ("file might be broken");

      // 1st row : code value
      unsigned long codepoint = stoi (col[0], nullptr, 16);

      // 5th row : character decomposition mapping
      if (col[5].length () != 0
	  && /* Starting with '<'. Compatibility mapping */
	  !(col[5].length () > 0 && col[5][0] == '<'))
	{
	  std::vector<std::string> decomposed_codepoints_hex
	    = split (col[5], ' ');
	  // At most two decomposed characters
	  if (decomposed_codepoints_hex.size () > 2)
	    fail ("file might be broken");

	  for (int i = 0; i < decomposed_codepoints_hex.size (); i++)
	    {
	      decomposed[codepoint][i]
		= stoi (decomposed_codepoints_hex[i], nullptr, 16);
	    }
	  unsigned int cp1 = decomposed[codepoint][0];
	  unsigned int cp2 = decomposed[codepoint][1];
	  if (cp1 && !cp2)
	    {
	      // compose1
	      compose_map1.insert ({cp1, codepoint});
	    }
	  else if (cp1 && cp2)
	    {
	      // compose2
	      auto it = compose_map2.find (cp1);
	      std::vector<std::pair<unsigned int, unsigned int>> v;
	      if (it == compose_map2.end ())
		{
		  v = {};
		}
	      else
		{
		  v = it->second;
		}
	      v.push_back ({codepoint, cp2});
	      compose_map2.insert ({cp1, v});
	    }
	}
    }
}

void
write_newline ()
{
  std::cout << std::endl;
}

void
write_decompose ()
{
  std::cout << "std::pair<uint32_t, uint32_t> " << std::endl
	    << "decompose(uint32_t codepoint)" << std::endl;
  std::cout << "{" << std::endl;

  std::cout << "  switch(codepoint)" << std::endl;
  std::cout << "  {" << std::endl;
  for (int cp = 0; cp < NUM_CODE_POINTS; cp++)
    {
      if (decomposed[cp][0] != 0)
	{
	  std::cout << std::hex << "  case 0x" << cp << ":" << std::endl;
	  std::cout << std::hex << "    "
		    << "return {0x" << decomposed[cp][0] << ", 0x"
		    << decomposed[cp][1] << "};" << std::endl;
	}
    }
  std::cout << "  default:" << std::endl;
  std::cout << "    return {0, 0};" << std::endl;
  std::cout << "  }" << std::endl;
  std::cout << "}" << std::endl;
}

void
write_compose_one_char ()
{
  std::cout << "Optional<uint32_t> " << std::endl
	    << "compose_one_char(uint32_t cp)" << std::endl;
  std::cout << "{" << std::endl;

  std::cout << "  switch(cp)" << std::endl;
  std::cout << "  {" << std::endl;

  for (auto p : compose_map1)
    {
      std::cout << "  case 0x" << std::hex << p.first << ":" << std::endl;
      std::cout << "    return {0x" << std::hex << p.second << "};"
		<< std::endl;
    }

  std::cout << "  default:" << std::endl;
  std::cout << "    return Optional<uint32_t>::none();" << std::endl;
  std::cout << "  }" << std::endl;
  std::cout << "}" << std::endl;
}

void
write_compose_two_char ()
{
  std::cout << "Optional<uint32_t> " << std::endl
	    << "compose_two_char(uint32_t cp1, uint32_t cp2)" << std::endl;
  std::cout << "{" << std::endl;

  std::cout << "  switch(cp1)" << std::endl;
  std::cout << "  {" << std::endl;

  for (auto p : compose_map2)
    {
      std::cout << "  case 0x" << std::hex << p.first << ":" << std::endl;

      std::cout << "    switch(cp2)" << std::endl;
      std::cout << "     {" << std::endl;
      for (auto p2 : p.second)
	{
	  std::cout << "    case 0x" << std::hex << p2.second << ":"
		    << std::endl;
	  std::cout << "      return {0x" << std::hex << p2.first << "};"
		    << std::endl;
	}
      std::cout << "    default:" << std::endl;
      std::cout << "      return Optional<uint32_t>::none();" << std::endl;
      std::cout << "    } break;" << std::endl;
    }

  std::cout << "  default:" << std::endl;
  std::cout << "    return Optional<uint32_t>::none();" << std::endl;
  std::cout << "  }" << std::endl;
  std::cout << "}" << std::endl;
}

int
main (int argc, char **argv)
{
  if (argc != 2)
    fail ("too few arguments");

  read_unicode_data_txt (argv[1]);

  std::cout << COPYRIGHT << std::endl;
  write_newline ();

  std::cout << "#include \"rust-optional.h\"" << std::endl;
  std::cout << "#include \"rust-system.h\"" << std::endl;
  write_newline ();

  std::cout << "namespace Rust {" << std::endl;
  write_newline ();

  write_decompose ();
  write_newline ();

  write_compose_one_char ();
  write_newline ();

  write_compose_two_char ();
  write_newline ();

  std::cout << "} // namespace Rust" << std::endl;
}
