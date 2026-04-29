#include "gcc-plugin.h"
#include <stdlib.h>
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "intl.h"
#include "cp/cp-tree.h"
#include "cp/name-lookup.h"
#include "diagnostic.h"
#include "stringpool.h"

int plugin_is_GPL_compatible;

void plugin_walk_ns (tree);

const char *badnames[] = {
  /* See libstdc++-v3/testsuite/17_intro/{,bad}names.cc.  */
  "_A", "_B", "_C", "_G", "_L", "_N", "_P", "_S", "_T", "_U", "_X",
  "__deref", "__used", "__unused", "__inline", "_Complex",
  "__istype", "__maskrune", "__tolower", "__toupper", "__wchar_t",
  "__wint_t", "_res", "_res_ext", "_C2", "__lockable", "__null_sentinel",
  "__packed", "__weak", "__strong", "_In_", "_Inout_", "_Out_",
  "_Reserved_", "__inout", "__in_opt", "__out_opt"
};

vec<tree> nonugly_names;
hash_set<tree> whitelist;
bool impl_ns;

void
plugin_check_decl (tree decl)
{
  if (decl == NULL_TREE)
    return;

  if (VAR_OR_FUNCTION_DECL_P (decl)
      && DECL_EXTERNAL (decl)
      && DECL_CONTEXT (decl)
      && TREE_CODE (DECL_CONTEXT (decl)) == FUNCTION_DECL)
    return;

  tree name = DECL_NAME (decl);
  if (name == NULL_TREE
      || IDENTIFIER_ANON_P (name)
      || name == this_identifier
      || IDENTIFIER_ANY_OP_P (name))
    return;

  const char *cname = IDENTIFIER_POINTER (name);
  if (memcmp (cname, "auto:", 5) == 0)
    return;

  if (cname[0] != '_'
      || (cname[1] != '_'
	  && !ISUPPER (cname[1])
	  && cname[1]))
    {
      nonugly_names.safe_push (decl);
      return;
    }

  bool badname = false;
  for (int i = 0; i < ARRAY_SIZE (badnames); ++i)
    if (strcmp (badnames[i], cname) == 0)
      {
	badname = true;
	break;
      }
  if (!badname && cname[1] == 'E' && ISDIGIT (cname[2]))
    {
      if (cname[2] != '0'
	  && (!cname[3] || ((cname[2] == '1' || cname[2] == '2')
			    && ISDIGIT (cname[3])
			    && !cname[4])))
	badname = true;
    }
  else if (memcmp (cname + 1, "_tg_", 4) == 0)
    badname = true;
  if (badname)
    inform (DECL_SOURCE_LOCATION (decl), "badname %qs", cname);
}

tree
plugin_check_tree (tree *tp, int */*walk_subtrees*/, void */*data*/)
{
  if (TREE_CODE (*tp) == BIND_EXPR)
    for (tree var = BIND_EXPR_VARS (*tp); var; var = DECL_CHAIN (var))
      plugin_check_decl (var);
  if (TREE_CODE (*tp) == DECL_EXPR)
    plugin_check_decl (DECL_EXPR_DECL (*tp));
  if (TREE_CODE (*tp) == REQUIRES_EXPR)
    for (tree parm = REQUIRES_EXPR_PARMS (*tp); parm; parm = DECL_CHAIN (parm))
      plugin_check_decl (parm);
  return NULL_TREE;
}

void
plugin_check_fn (tree decl)
{
  for (tree arg = DECL_ARGUMENTS (decl); arg; arg = DECL_CHAIN (arg))
    plugin_check_decl (arg);

  if (DECL_DEFAULTED_FN (decl))
    return;
  cp_walk_tree_without_duplicates (&DECL_SAVED_TREE (decl), plugin_check_tree,
				   NULL);
}

bool
plugin_header_check (tree decl)
{
  expanded_location eloc = expand_location (DECL_SOURCE_LOCATION (decl));
  if (eloc.file == NULL)
    return false;
  if (strstr (eloc.file, "/libstdc++-v3/include/") == NULL
      && strstr (eloc.file, "/libstdc++-v3/libsupc++/") == NULL)
    return false;
  return true;
}

void
plugin_walk_decl (tree decl)
{
  if (TREE_CODE (decl) == NAMESPACE_DECL)
    {
      if (impl_ns
	  || (DECL_NAME (decl)
	      && IDENTIFIER_POINTER (DECL_NAME (decl))[0] != '_'
	      && !IDENTIFIER_ANON_P (DECL_NAME (decl)))
	  || DECL_NAMESPACE_INLINE_P (decl))
	plugin_walk_ns (decl);
      else
	{
	  impl_ns = true;
	  plugin_walk_ns (decl);
	  impl_ns = false;
	}
      return;
    }

  if (!plugin_header_check (decl))
    return;

  if (!impl_ns
      && DECL_NAME (decl)
      && !IDENTIFIER_ANON_P (DECL_NAME (decl))
      && !IDENTIFIER_ANY_OP_P (DECL_NAME (decl))
      && IDENTIFIER_POINTER (DECL_NAME (decl))[0] != '_')
    whitelist.add (DECL_NAME (decl));
  else if (impl_ns && DECL_NAME (decl))
    plugin_check_decl (decl);

  if (TREE_CODE (decl) == TEMPLATE_DECL)
    {
      tree parms = DECL_INNERMOST_TEMPLATE_PARMS (decl);
      for (tree node : tree_vec_range (parms))
	plugin_check_decl (TREE_VALUE (node));
      if (tree constr
	  = TEMPLATE_PARMS_CONSTRAINTS (DECL_TEMPLATE_PARMS (decl)))
        cp_walk_tree_without_duplicates (&constr, plugin_check_tree,
					 NULL);
    }

  if (DECL_FUNCTION_TEMPLATE_P (decl))
    plugin_check_fn (DECL_TEMPLATE_RESULT (decl));
  else if (TREE_CODE (decl) == FUNCTION_DECL
	   && DECL_LANG_SPECIFIC (decl)
	   && DECL_TEMPLATE_INFO (decl))
    plugin_check_fn (decl);

  if (TREE_CODE (STRIP_TEMPLATE (decl)) == CONCEPT_DECL)
    cp_walk_tree_without_duplicates (&DECL_INITIAL (STRIP_TEMPLATE (decl)),
				     plugin_check_tree, NULL);

  if (DECL_CLASS_TEMPLATE_P (decl))
    decl = DECL_TEMPLATE_RESULT (decl);
  if (TREE_CODE (decl) == TYPE_DECL
      && DECL_IMPLICIT_TYPEDEF_P (decl))
    {
      bool save_impl_ns = impl_ns;
      if (impl_ns
	  && DECL_NAME (decl)
	  && id_equal (DECL_NAME (decl), "_Promise_erased")
	  && DECL_CONTEXT (decl)
	  && TREE_CODE (DECL_CONTEXT (decl)) == NAMESPACE_DECL
	  && DECL_NAME (DECL_CONTEXT (decl))
	  && id_equal (DECL_NAME (DECL_CONTEXT (decl)), "__gen"))
	/* Workaround: std::generator<R, V, A>::promise_type in
	   the standard is actually defined as std::__gen::_Promise_erased,
	   but we want to whitelist its members.  Temporarily clear
	   impl_ns in that class.  */
	impl_ns = false;
      if (RECORD_OR_UNION_TYPE_P (TREE_TYPE (decl)))
	{
	  for (tree fld = TYPE_FIELDS (TREE_TYPE (decl));
	       fld; fld = DECL_CHAIN (fld))
	    plugin_walk_decl (fld);
	  for (tree f = DECL_FRIENDLIST (decl); f; f = TREE_CHAIN (f))
	    for (tree l = FRIEND_DECLS (f); l; l = TREE_CHAIN (l))
	      plugin_walk_decl (TREE_VALUE (l));
	}
      else if (TREE_CODE (TREE_TYPE (decl)) == ENUMERAL_TYPE)
	for (tree en = TYPE_VALUES (TREE_TYPE (decl));
	     en; en = TREE_CHAIN (en))
	  plugin_walk_decl (TREE_VALUE (en));
      impl_ns = save_impl_ns;
    }
}

void
plugin_walk_binding (tree binding)
{
  tree value = NULL_TREE;

  if (STAT_HACK_P (binding))
    {
      if (!STAT_TYPE_HIDDEN_P (binding)
	  && STAT_TYPE (binding))
	return plugin_walk_decl (STAT_TYPE (binding));
      else if (!STAT_DECL_HIDDEN_P (binding))
	value = STAT_DECL (binding);
    }
  else
    value = binding;

  value = ovl_skip_hidden (value);
  if (value)
    {
      value = OVL_FIRST (value);
      return plugin_walk_decl (value);
    }
}

void
plugin_walk_ns (tree ns)
{
  using itert = hash_table<named_decl_hash>::iterator;
  itert end (DECL_NAMESPACE_BINDINGS (ns)->end ());
  for (itert iter (DECL_NAMESPACE_BINDINGS (ns)->begin ());
       iter != end; ++iter)
    {
      tree b = *iter;
      gcc_assert (TREE_CODE (b) != BINDING_VECTOR);
      plugin_walk_binding (b);
    }
}

int
plugin_ctx_check (tree decl)
{
  int ret = 1;
  tree ctx = DECL_CONTEXT (decl);
  while (ctx)
    {
      if (ctx == std_node)
	return ret;
      if (TREE_CODE (ctx) == NAMESPACE_DECL
	  && DECL_NAME (ctx)
	  && id_equal (DECL_NAME (ctx), "__gnu_cxx")
	  && CP_DECL_CONTEXT (ctx) == global_namespace)
	return 2;
      if (TREE_CODE (ctx) == NAMESPACE_DECL
	  && (!DECL_NAME (ctx)
	      || IDENTIFIER_POINTER (DECL_NAME (ctx))[0] == '_'
	      || IDENTIFIER_ANON_P (DECL_NAME (ctx)))
	  && !DECL_NAMESPACE_INLINE_P (ctx))
	ret = 2;
      if (TYPE_P (ctx))
	ctx = TYPE_CONTEXT (ctx);
      else
	ctx = DECL_CONTEXT (ctx);
    }
  return 0;
}

void
plugin_finish_parse_function (void *event_data, void *)
{
  tree decl = (tree) event_data;
  if (!plugin_header_check (decl))
    return;

  /* Only diagnose stuff nested in ::std or ::__gnu_cxx
     namespaces.  */
  if (!plugin_ctx_check (decl))
    return;

  /* Templates are handled from plugin_walk_ns.  */
  if (DECL_LANG_SPECIFIC (decl) && DECL_TEMPLATE_INFO (decl))
    return;
  plugin_check_fn (decl);
}

void
plugin_walk_specialization (bool, spec_entry *entry, void *)
{
  tree info;
  if (TYPE_P (entry->spec))
    info = TYPE_TEMPLATE_INFO (entry->spec);
  else
    info = DECL_TEMPLATE_INFO (entry->spec);
  if (PRIMARY_TEMPLATE_P (TI_TEMPLATE (info)) && TI_PARTIAL_INFO (info))
    info = TI_PARTIAL_INFO (info);
  tree decl = TI_TEMPLATE (info);
  if (int c = plugin_ctx_check (decl))
    {
      impl_ns = c == 2;
      plugin_walk_decl (decl);
    }
}

void
plugin_finish_unit (void *, void *)
{
  plugin_walk_ns (std_node);
  tree gnucxx_ns = lookup_qualified_name (global_namespace, "__gnu_cxx");
  if (gnucxx_ns && TREE_CODE (gnucxx_ns) == NAMESPACE_DECL)
    plugin_walk_ns (gnucxx_ns);
  walk_specializations (false, plugin_walk_specialization, NULL);
  walk_specializations (true, plugin_walk_specialization, NULL);
  unsigned int i;
  tree decl;
  FOR_EACH_VEC_ELT (nonugly_names, i, decl)
    if (!whitelist.contains (DECL_NAME (decl)))
      inform (DECL_SOURCE_LOCATION (decl), "non-uglified name %qs",
	      IDENTIFIER_POINTER (DECL_NAME (decl)));
    else
      inform (DECL_SOURCE_LOCATION (decl), "non-uglified whitelisted name %qs",
	      IDENTIFIER_POINTER (DECL_NAME (decl)));
}

void
plugin_ggc_walker (void *)
{
  unsigned int i;
  tree decl;
  FOR_EACH_VEC_ELT (nonugly_names, i, decl)
    gt_ggc_m_9tree_node (decl);
  for (hash_set<tree>::iterator it = whitelist.begin ();
       it != whitelist.end (); ++it)
    gt_ggc_m_9tree_node (*it);
}

static const struct ggc_root_tab xtratab[] = {
  { (void *) &xtratab, 1, 1, plugin_ggc_walker, NULL },
  LAST_GGC_ROOT_TAB
};

int
plugin_init (struct plugin_name_args *plugin_info,
	     struct plugin_gcc_version *version)
{
  const char *plugin_name = plugin_info->base_name;

  register_callback (plugin_name, PLUGIN_FINISH_UNIT,
		     plugin_finish_unit, NULL);
  register_callback (plugin_name, PLUGIN_FINISH_PARSE_FUNCTION,
		     plugin_finish_parse_function, NULL);
  register_callback (plugin_name, PLUGIN_REGISTER_GGC_ROOTS,
		     NULL, (void *) xtratab);

  return 0;
}
