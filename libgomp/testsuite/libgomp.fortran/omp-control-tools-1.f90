       use omp_lib
       implicit none
       !include "omp_lib.h"
       if (omp_control_tool(omp_control_tool_start, 1)                          &
     &    /= omp_control_tool_notool) stop 1
       if (omp_control_tool_start /= 1                                          &
     &    .or. omp_control_tool_pause /= 2                                      &
     &    .or. omp_control_tool_flush /= 3                                      &
     &    .or. omp_control_tool_end /= 4                                        &
     &    .or. omp_control_tool_max /= 2147483647                               & ! INT32_MAX
     &    .or. omp_control_tool_notool /= -2                                    &
     &    .or. omp_control_tool_nocallback /= -1                                &
     &    .or. omp_control_tool_success /= 0                                    &
     &    .or. omp_control_tool_ignored /= 1) stop 2
       if (kind(omp_control_tool_result_kind) /= 4                              &
     &    .or. omp_control_tool_kind /= 4) stop 3
       end
