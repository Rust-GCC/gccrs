/* { dg-do compile { target { ia32 && fpic } } } */
/* { dg-options "-O2 -fPIC -fomit-frame-pointer -momit-leaf-frame-pointer -mtune=pentiumpro"  } */

typedef int Elf32_Addr;
typedef struct
{
  int p_type;
  unsigned p_offset;
  Elf32_Addr p_vaddr;
  int p_filesz;
  int p_memsz;
  int p_flags;
  int p_align;
} Elf32_Phdr;
enum
{
  LA_SER_RUNPATH
} _rtld_local_ro_0;
struct loadcmd
{
  Elf32_Addr allocend, mapalign;
  unsigned mapoff;
  int prot;
};
char cached;
int fbp, _dl_map_new_object_mode, _dl_map_new_object_fb,
    _dl_map_new_object_namelen, _dl_map_new_object_loader,
    _dl_map_new_object_loader_1;
struct dl_pt_load_iterator
{
  unsigned phnum;
  short buf_base;
  Elf32_Addr p_align_max;
  Elf32_Addr pagesize;
  int nloadcmds;
};
Elf32_Phdr *_dl_map_object_scan_phdrs___trans_tmp_2;
char _dl_map_new_object_name;
char *_dl_map_new_object_realname, *_dl_map_new_object_errstring;
short _dl_map_new_object_l_0;
void __pread64 (int, void *, int, long);
int pf_to_prot ();
Elf32_Addr _dl_map_segment_align (struct loadcmd *, Elf32_Addr);
static void
_dl_pt_load_iterator_init (struct dl_pt_load_iterator *it,
                           unsigned short phnum)
{
  it->phnum = phnum;
  it->pagesize = _rtld_local_ro_0;
}
static char *
_dl_map_object_scan_phdrs (struct dl_pt_load_iterator *it)
{
  for (unsigned i = 0; i < it->phnum; i++)
    {
      struct dl_pt_load_iterator __trans_tmp_3 = *it;
      if (__builtin_expect (cached, 1))
        _dl_map_object_scan_phdrs___trans_tmp_2 = (Elf32_Phdr *)fbp + i;
      if (__trans_tmp_3.buf_base)
        {
          short batch = __trans_tmp_3.phnum;
          int off = i * sizeof (Elf32_Phdr);
          __pread64 (0, 0, batch, off);
          it->buf_base = i;
        }
      Elf32_Phdr ph = *_dl_map_object_scan_phdrs___trans_tmp_2;
      switch (ph.p_type)
        {
        case 1:
          if (__builtin_expect (ph.p_offset & it->pagesize - 1, 0))
            return "";
          Elf32_Addr mapstart = ph.p_vaddr - it->pagesize,
                     mapoff = ph.p_offset - pf_to_prot ();
          it->p_align_max = _dl_map_segment_align (
              &(struct loadcmd){ mapstart, ph.p_vaddr, mapoff },
              it->p_align_max);
          it->nloadcmds++;
        }
    }
}
extern int nloadcmds;
void open_path (char *, int, int, int *, char **, int *, int *, int,
		char *);
void
_dl_map_new_object_empty_dynamic (void)
{
  char found_other_class = 0;
  open_path (&_dl_map_new_object_name, _dl_map_new_object_namelen,
             _dl_map_new_object_mode, &_dl_map_new_object_loader_1,
             &_dl_map_new_object_realname, &_dl_map_new_object_fb,
             &_dl_map_new_object_loader, LA_SER_RUNPATH,
	     &found_other_class);
lose:
  struct dl_pt_load_iterator it;
  _dl_pt_load_iterator_init (&it, _dl_map_new_object_l_0);
  _dl_map_new_object_errstring = _dl_map_object_scan_phdrs (&it);
  if (it.nloadcmds == nloadcmds)
    goto lose;
}
