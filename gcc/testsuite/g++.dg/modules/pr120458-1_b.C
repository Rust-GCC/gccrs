// { dg-module-do link }
// { dg-additional-options "-fmodules -finput-charset=UTF-8" }

#include <iostream>

import 영혼;

int main(void) {
	std::cout << f(13) << '\n';
	return 0;
}
