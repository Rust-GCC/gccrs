// { dg-module-do link }
// { dg-additional-options "-fmodules -finput-charset=UTF-8" }
// { dg-skip-if "" { ! ucn } }

export module 영혼;
// { dg-module-cmi }

export unsigned f(unsigned x) {
	return x + 3;
}
