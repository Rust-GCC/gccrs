/* { dg-require-effective-target ptr32plus } */
/* { dg-skip-if "The array is too big" { "avr-*-*" "pdp11-*-*" } } */ 
/* { dg-require-stack-size "250*250" } */

char paths[1024];
static void x264_slicetype_path(char (*best_paths)[250], int n, int length)
{
    __builtin_memcpy (best_paths[n], paths, length);
}
void x264_slicetype_analyse(int n, int length)
{
    char best_paths[250][250];
    x264_slicetype_path (best_paths, n, length);
}
