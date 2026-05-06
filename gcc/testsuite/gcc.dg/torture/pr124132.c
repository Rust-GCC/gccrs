/* { dg-do compile } */
/* { dg-additional-options "-w" } */

char slow_mul1_dst1, code_some_shards___gf_mulc__0, code_some_shards_lim;
long slow_mul1_dst_max;
long long code_some_shards_pos;
void code_some_shards()
{
  char *dst = &slow_mul1_dst1;
  for (; dst < &code_some_shards_lim; dst++)
    {
      if (code_some_shards_pos < slow_mul1_dst_max)
        *dst = code_some_shards___gf_mulc__0;
      code_some_shards_pos += 1;
    }
}
