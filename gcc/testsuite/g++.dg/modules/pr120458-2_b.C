// { dg-module-do link }
// { dg-additional-options "-fmodules -fmodule-mapper=[srcdir]/pr120458-2.map " }
// { dg-additional-options "-finput-charset=UTF-8" }

import 灵魂;

#include <iostream>

int main(void) {
	std::cout << f(13) << '\n';
	return 0;
}
