// { dg-do run { target c++20 } }

#include <chrono>
#include <sstream>
#include <testsuite_hooks.h>

using namespace std::chrono;

const std::string input = "2026-05-18 15:26:48 ";
const std::string fmt = "%F %T %";
sys_seconds ss;
minutes offset{};
std::string abbrev;

void
test_parse_z()
{
  for (auto suffix : {"Z", "+", "+Z", "-A", "+1Z", "+010", "+010Z"})
  {
    std::istringstream in{input + suffix};
    from_stream(in, (fmt + "z").c_str(), ss, &abbrev, &offset);

    VERIFY( in.fail() );
    VERIFY( ss == sys_seconds(0s) );
    VERIFY( offset == 0min );
    VERIFY( abbrev.empty() );
  }
}

void
test_parse_Ez()
{
  for (auto suffix : {"Z", "+", "-", "+01:", "+01:X"})
  {
    std::istringstream in{input + suffix};
    from_stream(in, (fmt + "Ez").c_str(), ss, &abbrev, &offset);

    VERIFY( in.fail() );
    VERIFY( ss == sys_seconds(0s) );
    VERIFY( offset == 0min );
    VERIFY( abbrev.empty() );
  }
}

void
test_parse_Oz()
{
  for (auto suffix : {"Z", "+", "-", "+01:", "+01:X"})
  {
    std::istringstream in{input + suffix};
    from_stream(in, (fmt + "Oz").c_str(), ss, &abbrev, &offset);

    VERIFY( in.fail() );
    VERIFY( ss == sys_seconds(0s) );
    VERIFY( offset == 0min );
    VERIFY( abbrev.empty() );
  }
}

int main()
{
  test_parse_z();
  test_parse_Ez();
  test_parse_Oz();
}
