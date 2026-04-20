// { dg-do compile }
module object;
enum E
{
    unknown
}
enum : E
{
    E_UNKNOWN
}

E test()
{
    return E.unknown;
}
