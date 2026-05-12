/* PR tree-optimization/125291 */

char buf[1111];
char *archive_le16dec_filename = buf;
unsigned int archive_le16dec_end, archive_le16dec_fn_end,
    archive_le16dec_filename_size, archive_le16dec_offset;
char archive_le16dec_p[] = { 21, 0x7f };

[[gnu::noipa]]
void
archive_le16dec ()
{
  archive_le16dec_filename_size = (short)archive_le16dec_filename_size;
  unsigned char flagbits = 0, flagbyte;
  archive_le16dec_end = archive_le16dec_filename_size;
  archive_le16dec_fn_end = archive_le16dec_filename_size * 2;
  archive_le16dec_filename_size = flagbits = 0;
  while (archive_le16dec_offset < archive_le16dec_end
         && archive_le16dec_filename_size < archive_le16dec_fn_end)
    {
      if (!flagbits)
        {
          flagbyte = archive_le16dec_p[archive_le16dec_offset++];
          flagbits = 8;
        }
      flagbits -= 2;
      if (!(flagbyte >> flagbits & 3))
        archive_le16dec_filename_size++;
    }
}

int
main ()
{
  archive_le16dec_filename_size = 2;
  archive_le16dec ();
  if (archive_le16dec_filename_size != 1)
    __builtin_trap ();
}
