/* Verify that x86_64 mingw can emit SEH metadata for a sysv_abi wrapper
   trampoline that references a throwing callee.  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-std=gnu++17 -fexceptions" } */

#define SYSV __attribute__ ((sysv_abi))
#define NOINLINE __attribute__ ((noinline))

template <auto F>
struct wrapper;

template <typename R, typename... Args, SYSV R (*Func) (Args...)>
struct wrapper<Func>
{
  static R SYSV NOINLINE
  call (Args... args) noexcept
  {
    return Func (args...);
  }
};

using entry_fn = int SYSV (*) ();

volatile int throw_flag;
volatile unsigned long long sink;

int SYSV NOINLINE
stub0 ()
{
  try
    {
      if (throw_flag)
        throw 0;
    }
  catch (...)
    {
    }

  return 1;
}

static entry_fn wrapped[] = { wrapper<stub0>::call };

int
main ()
{
  entry_fn fn = wrapped[0];
  sink += (unsigned long long) (void *) fn;
  return fn () == 0;
}
