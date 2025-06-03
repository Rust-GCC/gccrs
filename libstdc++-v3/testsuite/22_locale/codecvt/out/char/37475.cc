#include <locale>
#include <assert.h>

void
test_pr37475()
{
  typedef std::codecvt<char, char, std::mbstate_t> test_type;
  const test_type& cvt = std::use_facet<test_type>(std::locale::classic());
  const char from = 'a';
  const char* from_next;
  char to;
  char* to_next;
  std::mbstate_t st = std::mbstate_t();
  std::codecvt_base::result res
    = cvt.out(st, &from, &from+1, from_next, &to, &to, to_next);

  assert( res == std::codecvt_base::noconv );
}

int main()
{
  test_pr37475();
}
