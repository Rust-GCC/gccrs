/* { dg-do compile } */
/* { dg-options "-march=rv64gc_zba_zbb -mabi=lp64d -O3" } */
/* Verify that minu/maxu/min/max si3 expansions set SUBREG_PROMOTED,
   so that no redundant sext.w is emitted after the min/max.  */

unsigned int umin_loop(const unsigned char *cur, const unsigned char *buf,
		       unsigned int *nodes, unsigned int cur_match,
		       unsigned int len_limit, unsigned int depth)
{
    unsigned int len0 = 0, len1 = 0;
    unsigned int len_best = 0;

    while (depth--) {
        const unsigned char *pb = buf + cur_match;
        unsigned int len = (len0 < len1) ? len0 : len1;

        if (pb[len] == cur[len]) {
            while (++len != len_limit)
                if (pb[len] != cur[len])
                    break;
            if (len > len_best)
                len_best = len;
        }

        if (pb[len] < cur[len]) {
            cur_match = nodes[2 * cur_match + 1];
            len1 = len;
        } else {
            cur_match = nodes[2 * cur_match];
            len0 = len;
        }
    }
    return len_best;
}

/* { dg-final { scan-assembler-times {\mminu} 1 } } */
/* { dg-final { scan-assembler-not {\msext\.w\M} } } */
