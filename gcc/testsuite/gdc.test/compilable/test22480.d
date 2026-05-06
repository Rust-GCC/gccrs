// EXTRA_FILES: imports/test22480b.d
import imports.test22480b;

@nogc nothrow:
enum a = parseAA();

extern(C) int main()
{
	return 0;
}
