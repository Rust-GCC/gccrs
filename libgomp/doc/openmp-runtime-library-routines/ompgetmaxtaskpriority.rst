..
  Copyright 1988-2022 Free Software Foundation, Inc.
  This is part of the GCC manual.
  For copying conditions, see the copyright.rst file.

.. _omp_get_max_task_priority:

omp_get_max_task_priority -- Maximum priority value
***************************************************

that can be set for tasks.

Description:
  This function obtains the maximum allowed priority number for tasks.

C/C++:
  .. list-table::

     * - *Prototype*:
       - ``int omp_get_max_task_priority(void);``

Fortran:
  .. list-table::

     * - *Interface*:
       - ``integer function omp_get_max_task_priority()``

Reference:
  :openmp:`4.5`, Section 3.2.29.
