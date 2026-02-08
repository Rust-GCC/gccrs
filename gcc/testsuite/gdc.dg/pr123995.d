// { dg-do compile }
int fn123995(const typeof(*null) val)
{
    return 0;
}

int pr123995a()
{
    return fn123995(assert(0));
}

int pr123995b()
{
    static Exception ex;
    return fn123995(throw ex);
}
