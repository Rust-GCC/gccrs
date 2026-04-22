// PR c++/124973
// { dg-do compile { target c++20 } }
// { dg-additional-options "-fmodules" }

export module left;		// { dg-module-cmi left }
import right;

export auto xx(environment&) -> void;
