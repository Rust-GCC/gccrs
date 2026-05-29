# GCC COBOL Compatibility Functions

## Purpose

It seems every COBOL compiler includes a library of functions intended
to make the COBOL programer's life easier.  All of them, as we
demonstrate here, can be written in COBOL.  They are supplied in COBOL
form, not as a library. The user is free to compile them into a
utility library.

Some of the functions defined here require runtime support from libgcobol. 

## Fri Oct 10 16:01:58 2025

At the time of this writing, the functions of greatest concern are
those that are defined by Rocket Software (formerly MicroFocus) and
emulated by GnuCOBOL. Those are implemented in
`libgcobol/compat/gnu/lib`.  Any calls they would otherwise make to
the C library are effected through COBOL POSIX bindings supplied by
`libgcobol/posix/udf/posix-close.cbl`.

As an aid to the developer, a simple example of how these functions
are used is found in `libgcobol/compat/t/smoke.cbl`. It may by
compiled using `libgcobol/compat/t/Makefile`.

## Thu Nov 13 17:34:43 2025
### Naming conventions

- For each POSIX function, we have a COBOL UDF posix-FUNC
- posix/udf/posix-FUNC.cbl defines posix-FUNC
- posix/cpy/psx-FUNC.cpy is a copybook for posix-FUNC
  - the abbreviated prefix prevents conflicts and confusion 
  - I'm not sure this is a good idea --jkl
- posix/shim/FUNC.cc provides a C function posix_FUNC (note underscore)
- posix/shim/FUNC.h  provides a C version of psx-FUNC.cpy
 
If posix-FUNC.cbl calls a shim function, it must `COPY psx-FUNC` to
get the required definitions.  We ensure the names and values defined
in the copybook match those used by the shim.

### Status

Of the MF functions, those needed immediately are 

Done, not tested:
- CBL_ALLOC_MEM
- CBL_CHECK_FILE_EXISTS
- CBL_DELETE_FILE
- CBL_FREE_MEM

Not written:
- CBL_GET_PROGRAM_INFO (functions 0 + 2)

### Tests

We want to write DejaGnu tests that will be activated by 

    $ make -C gcc check-cobol
    

## Wed Apr 29 15:01:40 2026

The library has been renamed from `libcompat` to `libgcobol_compat_gnu`.
It is now automatically injected by the `gcobol` and `gcobc` front-ends based
on the path to the installation prefix.

Tests have been implemented for the following functions:

- `CBL_ALLOC_MEM`
- `CBL_FREE_MEM`
- `CBL_CHECK_FILE_EXIST`
- `CBL_CREATE_FILE`
- `CBL_DELETE_FILE`
- `CBL_OPEN_FILE`
- `CBL_CLOSE_FILE`
- `CBL_WRITE_FILE`
- `CBL_READ_FILE`

`cbltypes.cpy` and `cblproto.cpy` can be now `COPY`ed by programs.
As of today, `cbltypes.cpy` only defines the `cblt-fileexist-buf` data type.
On the other hand, `cblproto.cpy` provides function prototypes for all of the
MF functions currently supported by the library.
