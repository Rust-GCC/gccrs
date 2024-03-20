#ifndef RUST_BIR_FREE_REGION_H
#define RUST_BIR_FREE_REGION_H

#include <polonius/rust-polonius-ffi.h>

namespace Rust {

using FreeRegion = size_t;

class FreeRegions
{
  std::vector<FreeRegion> regions;

public:
  WARN_UNUSED_RESULT bool has_regions () const { return !regions.empty (); }
  decltype (regions)::const_iterator begin () const { return regions.begin (); }
  decltype (regions)::const_iterator end () const { return regions.end (); }
  size_t size () const { return regions.size (); }
  FreeRegion &operator[] (size_t i) { return regions.at (i); }
  const FreeRegion &operator[] (size_t i) const { return regions.at (i); }
  const std::vector<FreeRegion> &get_regions () const { return regions; }
  void set_from (std::vector<Rust::Polonius::Origin> &&regions)
  {
    this->regions.clear ();
    for (auto &region : regions)
      {
	this->regions.push_back ({region});
      }
  }

  WARN_UNUSED_RESULT FreeRegions prepend (FreeRegion region) const
  {
    std::vector<FreeRegion> new_regions = {region};
    new_regions.insert (new_regions.end (), regions.begin (), regions.end ());
    return FreeRegions (std::move (new_regions));
  }

  FreeRegions (std::vector<FreeRegion> &&regions) : regions (regions) {}

  WARN_UNUSED_RESULT std::string to_string () const
  {
    std::string result = "";
    for (auto &region : regions)
      {
	result += std::to_string (region);
	result += ", ";
      }
    // Remove the last ", " from the string.
    if (result.size () > 2)
      {
	result.pop_back ();
	result.pop_back ();
      }
    return result;
  }
};
} // namespace Rust

#endif // RUST_BIR_FREE_REGION_H
