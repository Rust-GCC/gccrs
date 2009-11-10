/* go-convert-interface.c -- convert interfaces for Go.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <assert.h>
#include <stdlib.h>

#include "go-alloc.h"
#include "go-panic.h"
#include "go-refcount.h"
#include "interface.h"

/* Convert one interface type into another interface type.
   LHS_DESCRIPTOR is the type descriptor of the resulting interface.
   RHS is the interface we are converting, a pointer to struct
   __go_interface.  We need to build a new set of interface method
   pointers.  If any interface method is not implemented by the
   object, the conversion fails.  If SUCCESS is not NULL, we set it to
   whether or not the conversion succeeds.  If SUCCESS is NULL, and
   the conversion fails, we panic.  */

struct __go_interface *
__go_convert_interface (const struct __go_type_descriptor* lhs_descriptor,
			const void *rhs_arg, _Bool *success)
{
  const struct __go_interface *rhs = (const struct __go_interface *) rhs_arg;
  const struct __go_interface_type *lhs_interface;
  int lhs_method_count;
  const struct __go_interface_method* lhs_methods;
  const void **methods;
  struct __go_interface *ret;

  if (rhs == NULL)
    {
      if (success != NULL)
	*success = 1;
      return NULL;
    }

  assert (lhs_descriptor->__code == GO_INTERFACE);
  lhs_interface = (const struct __go_interface_type *) lhs_descriptor;
  lhs_method_count = lhs_interface->__methods.__count;
  lhs_methods = ((const struct __go_interface_method *)
		 lhs_interface->__methods.__values);

  if (lhs_method_count == 0)
    methods = NULL;
  else
    {
      const struct __go_uncommon_type *rhs_uncommon;
      int rhs_method_count;
      const struct __go_method *p_rhs_method;
      int i;

      methods = (const void **) __go_alloc (lhs_method_count
					    * sizeof (void *));

      rhs_uncommon = rhs->__type_descriptor->__uncommon;
      if (rhs_uncommon == NULL)
	{
	  rhs_method_count = 0;
	  p_rhs_method = NULL;
	}
      else
	{
	  rhs_method_count = rhs_uncommon->__methods.__count;
	  p_rhs_method = ((const struct __go_method *)
			  rhs_uncommon->__methods.__values);
	}

      for (i = 0; i < lhs_method_count; ++i)
	{
	  const struct __go_interface_method *p_lhs_method;

	  p_lhs_method = &lhs_methods[i];

	  while (rhs_method_count > 0
		 && (!__go_ptr_strings_equal (p_lhs_method->__name,
					      p_rhs_method->__name)
		     || !__go_ptr_strings_equal (p_lhs_method->__pkg_path,
						 p_rhs_method->__pkg_path)))
	    {
	      ++p_rhs_method;
	      --rhs_method_count;
	    }

	  if (rhs_method_count == 0)
	    {
	      if (success != NULL)
		{
		  *success = 0;
		  return NULL;
		}
	      __go_panic ("interface conversion failed: no '%.*s' method",
			  (int) (*p_lhs_method->__name)->__length,
			  (*p_lhs_method->__name)->__data);
	    }

	  if (p_lhs_method->__hash != p_rhs_method->__hash)
	    {
	      if (success != NULL)
		{
		  *success = 0;
		  return NULL;
		}
	      __go_panic (("interface conversion failed: "
			   "'%.*s' method has wrong type"),
			  (int) (*p_lhs_method->__name)->__length,
			  (*p_lhs_method->__name)->__data);
	    }

	  methods[i] = p_rhs_method->__function;
	}
    }

  ret = (struct __go_interface *) __go_alloc (sizeof (struct __go_interface));
  ret->__type_descriptor = rhs->__type_descriptor;
  ret->__methods = methods;
  ret->__object = rhs->__object;

  __go_increment_refcount (ret->__object, ret->__type_descriptor);

  if (success != NULL)
    *success = 1;

  return ret;
}
