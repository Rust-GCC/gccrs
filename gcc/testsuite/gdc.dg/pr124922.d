// { dg-do compile }
// { dg-additional-options "-fno-assert -fdump-tree-original" }
module object;
alias noreturn = typeof(*null);
void main()
{
    void function(noreturn) fun;
    noreturn bar;
    fun(bar);
}
// { dg-final { scan-tree-dump-times "__builtin_trap" 1 "original" } }
