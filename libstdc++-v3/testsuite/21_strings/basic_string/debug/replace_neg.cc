// { dg-do run { xfail *-*-* } }
// { dg-require-debug-mode "" }

#include <vector>
#include <string>

void test01()
{
  std::vector<char> v1(10, 'a');
  std::string s2 = "bbbbbbbbbb";

  s2.replace(s2.begin(), s2.begin() + 5,
	     v1.begin() + 7, v1.begin() + 2);
}

int main()
{
  test01();
  return 0;
}
