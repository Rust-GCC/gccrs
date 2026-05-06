// { dg-do compile }
// { dg-require-effective-target split_stack }
// { dg-options "-fsplit-stack -fno-moduleinfo" }

import gcc.attributes;

void use(ref int[100]);

void split()
{
    int[100] arr;
    use(arr);
}

@attribute("no_split_stack")
void nosplit()
{
    int[100] arr;
    use(arr);
}
// { dg-final { scan-assembler-times "__morestack" 1 } }
