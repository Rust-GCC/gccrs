..
  Copyright 1988-2022 Free Software Foundation, Inc.
  This is part of the GCC manual.
  For copying conditions, see the copyright.rst file.

.. index:: LGT

.. index:: lexical comparison of strings

.. index:: string, comparison

.. _lgt:

LGT --- Lexical greater than
****************************

.. function:: LGT(STRING_A, STRING_B)

  Determines whether one string is lexically greater than another string,
  where the two strings are interpreted as containing ASCII character
  codes.  If the String A and String B are not the same length, the
  shorter is compared as if spaces were appended to it to form a value
  that has the same length as the longer.

  :param STRING_A:
    Shall be of default ``CHARACTER`` type.

  :param STRING_B:
    Shall be of default ``CHARACTER`` type.

  :return:
    Returns ``.TRUE.`` if ``STRING_A > STRING_B``, and ``.FALSE.``
    otherwise, based on the ASCII ordering.

  Standard:
    Fortran 77 and later

  Class:
    Elemental function

  Syntax:
    .. code-block:: fortran

      RESULT = LGT(STRING_A, STRING_B)

  Specific names:
    .. list-table::
       :header-rows: 1

       * - Name
         - Argument
         - Return type
         - Standard

       * - ``LGT(STRING_A,STRING_B)``
         - ``CHARACTER``
         - ``LOGICAL``
         - Fortran 77 and later

  See also:
    :ref:`LGE`,
    :ref:`LLE`,
    :ref:`LLT`
