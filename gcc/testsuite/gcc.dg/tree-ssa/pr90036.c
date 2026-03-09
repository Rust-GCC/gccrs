/* { dg-do compile } */
/* { dg-options "-O2 -Wall" } */

extern int sprintf (char *__restrict __s,
                    const char *__restrict __format, ...)
  __attribute__ ((__nothrow__));

typedef int bfd_boolean;



struct stab_type_stack
{
  long index;
  bfd_boolean definition;
};



struct stab_write_handle
{
  struct stab_type_stack *type_stack;
};
extern char *stab_pop_type (struct stab_write_handle *);
bfd_boolean stab_start_struct_type (void *, const char *, unsigned int,
                                    bfd_boolean, unsigned int);


bfd_boolean
stab_start_class_type (void *p, const char *tag, unsigned int id,
                       bfd_boolean structp, unsigned int size,
                       bfd_boolean vptr, bfd_boolean ownvptr)
{
  struct stab_write_handle *info = (struct stab_write_handle *) p;
  bfd_boolean definition;
  char *vstring;

  if (!vptr || ownvptr)
    {
      definition = 0;
      vstring = ((void *) 0);
    }
  else
    {
      definition = info->type_stack->definition;
      vstring = stab_pop_type (info);
    }



  if (vptr)
    {
      if (ownvptr)
        {

          sprintf (p, "~%%%ld", info->type_stack->index);
        }
      else
        {
          sprintf (p, "~%%%s", vstring);
        }

    }

  if (definition)
    info->type_stack->definition = 1;

  return 1;
}
