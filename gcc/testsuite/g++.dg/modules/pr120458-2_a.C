// { dg-module-do link }
// { dg-additional-options "-fmodules -fmodule-mapper=[srcdir]/pr120458-2.map " }
// { dg-additional-options "-finput-charset=UTF-8 " }
// { dg-skip-if "" { ! ucn } }
// { dg-additional-files "pr120458-2.map" }

export module 灵魂;
// { dg-module-cmi }

export unsigned f(unsigned x) {
	return x + 3;
}
