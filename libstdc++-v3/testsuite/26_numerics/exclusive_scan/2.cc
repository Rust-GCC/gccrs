// { dg-do run { target c++17 } }

// C++17 29.8.7 [exclusive.scan]

#include <numeric>
#include <testsuite_hooks.h>

struct Mint
{
  Mint(int i = 0) : val(i) { }
  Mint(Mint&&) = default;
  Mint& operator=(Mint&&) = default;

  operator int() const { return val; }

private:
  int val;
};

void
test_move_only()
{
  const int input[]{10, 20, 30};
  int output[3];
  std::exclusive_scan(input, input+3, output, Mint(5), std::plus<int>{});
  VERIFY( output[0] == 5 );
  VERIFY( output[1] == 15 );
  VERIFY( output[2] == 35 );
}

void
test_pr108236()
{
  int vals[]{1, 2, 3};
  // Output range is the same as the input range:
  std::exclusive_scan(vals, vals+3, vals, 99);
  VERIFY( vals[0] == 99 );
  VERIFY( vals[1] == 100 );
  VERIFY( vals[2] == 102 );
}

int main()
{
  test_move_only();
  test_pr108236();
}
