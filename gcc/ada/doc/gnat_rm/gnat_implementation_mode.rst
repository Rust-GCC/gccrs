
.. _GNAT_Implementation_Mode:

**************************************************
GNAT Implementation Mode (the ``-gnatg`` switch)
**************************************************

``-gnatg`` enables **GNAT implementation mode**, the internal compilation mode
used when building GNAT itself and its runtime libraries. It is not intended
for user application programs.

It implies ``-gnatw.ge`` (extended warning set, treated as errors) and
``-gnatyg`` (GNAT style checks).

Language & Semantic Changes
===========================

Ada Version and Identifier Rules
---------------------------------

* **Ada version is fixed at Ada 2012**, regardless of other switches.
* **Identifier character set is restricted to 7-bit ASCII**;
  extended characters are disallowed.
* **Identifiers are allowed to start with underscore** to avoid conflicts
  between global symbols in the runtime and those defined in the user code.

Limited Types
-------------

* Copying limited objects is **permitted** (normally illegal).
* Initializing limited types emits a **warning** rather than an error.
* Returning objects of limited types is **permitted**.

Preelaboration and Categorization
----------------------------------

* Categorization errors (pure/preelaborated unit violations) are downgraded
  to **warnings**.
* Statements in preelaborated units emit a **warning** rather than an error.
* Private objects and non-static constants/calls in preelaborated units emit
  **warnings** rather than errors.
* Calls to non-preelaborable subprograms in preelaborated packages are
  demoted to **suppressible warnings** (RM 10.2.1), allowing the runtime to
  include elaboration code that would otherwise be forbidden.
* Assignment statements to library-level variables are **excluded** from
  access-before-elaboration (ABE) analysis, on the assumption that the
  predefined runtime manages its own elaboration order correctly.

Overflow
--------

Overflow checks are **suppressed** but the overflow checking mode is set to
**Strict** (for assertion purposes).

Miscellaneous
-------------

* ``Scalar_Storage_Order`` attribute applies to **generic types** (normally
  it does not).
* Only the ``--!`` special comment sequence is recognized; all others are
  disallowed.
* The ``Priority`` aspect/attribute on protected types accepts the
  unconstrained type ``Any_Priority`` rather than the subtype
  ``System.Priority`` (a subrange of ``Integer``), giving GNAT's own
  protected objects access to the full implementation-defined priority range.
* ``pragma Extend_System`` is **ignored** (prevents circularities in System).
  Exception: predefined renaming units (``Text_IO``, ``Direct_IO``, etc.) are
  compiled without GNAT implementation mode so that ``Extend_System`` still
  works for them.
* Importing a non-pure external function (``pragma Import``) into a ``Pure``
  unit does **not** produce a purity warning, on the assumption that the
  runtime knows the purity properties of the C functions it imports.
* ``pragma Style_Checks ("All_Checks")`` activates the stricter ``-gnatyg``
  rule set rather than the default ``-gnaty`` set.

ALI Files and Recompilation
----------------------------

* ALI files are always generated for predefined generic packages.
* Compiling a unit whose top-level name is ``Ada``, ``Interfaces``, or
  ``System`` as the main unit is permitted; the RM reserves these as
  hierarchical library-unit name prefixes, but the diagnostic is suppressed
  to let GNAT compile its own predefined library (``Ada.Text_IO``,
  ``System.Storage_Elements``, etc.) as main units.
