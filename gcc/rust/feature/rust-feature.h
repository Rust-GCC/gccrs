// Copyright (C) 2020-2022 Free Software Foundation, Inc.

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

namespace Rust {

struct Feature
{
  enum State
  {
    ACCEPTED,
    ACTIVE,
    REMOVED,
    STABILIZED,
  };
  union Edition
  {
    CompileOptions::Edition edition;
    uint8_t none;
    Edition (CompileOptions::Edition e) : edition (e) {}
    Edition (uint8_t n) : none (n) {}
  };
  Feature (State S, const char *N, const char *RS, const char *GS, uint64_t I,
	   Edition E, const char *D)
    : state (S), name (N), rustc_since (RS), gccrs_since (GS), issue (I),
      edition (E), description (D)
  {
    Feature::NUM++;
  }
  State state;
  const char *name;
  const char *rustc_since;
  const char *gccrs_since;
  uint64_t issue;
  Edition edition;
  const char *description;

  static std::map<std::string, const Feature *> &load_hash_map ();

private:
  static uint32_t NUM;
  static std::map<std::string, const Feature *> feature_hash_map;
};

#define DECLARE_FEATURE(ST, N, RS, GS, I, E, D)                                \
  {                                                                            \
    ST, (N), (RS), (GS), (I), (E), (D)                                         \
  }

#define DECLARE_ACCEPTED_FEATURE(N, RS, GS, I, E, D)                           \
  DECLARE_FEATURE (Feature::State::ACCEPTED, (N), (RS), (GS), (I), (E), (D))

#define DECLARE_ACTIVE_FEATURE(N, RS, GS, I, E, D)                             \
  DECLARE_FEATURE (Feature::State::ACTIVE, (N), (RS), (GS), (I), (E), (D))

#define DECLARE_REMOVED_FEATURE(N, RS, GS, I, E, D)                            \
  DECLARE_FEATURE (Feature::State::REMOVED, (N), (RS), (GS), (I), (E), (D))

#define DECLARE_STABILIZED_FEATURE(N, RS, GS, I, E, D)                         \
  DECLARE_FEATURE (Feature::State::STABILIZED, (N), (RS), (GS), (I), (E), (D))

} // namespace Rust