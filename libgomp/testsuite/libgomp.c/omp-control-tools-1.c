#include <omp.h>

int
main ()
{
  omp_control_tool_t cmd = omp_control_tool_start;
  omp_control_tool_result_t res;
  res = omp_control_tool (omp_control_tool_start, 1, nullptr);
  if (res != omp_control_tool_notool)
    __builtin_abort ();
  if (omp_control_tool_start != 1
      || omp_control_tool_pause != 2
      || omp_control_tool_flush != 3
      || omp_control_tool_end != 4
      || omp_control_tool_max != 2147483647 /* INT32_MAX */
      || omp_control_tool_notool != -2
      || omp_control_tool_nocallback != -1
      || omp_control_tool_success != 0
      || omp_control_tool_ignored != 1)
    __builtin_abort ();
}
