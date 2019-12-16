#include "rust-ast-full.h"
// Dummy implementations of as_string() for now - will complete later for debugging purposes

namespace Rust {
    namespace AST {
        // Gets a string in a certain delim type.
        ::std::string get_string_in_delims(::std::string str_input, DelimType delim_type) {
            switch (delim_type) {
                case PARENS:
                    return "(" + str_input + ")";
                case SQUARE:
                    return "[" + str_input + "]";
                case CURLY:
                    return "{" + str_input + "}";
                default:
                    return "ERROR-MARK-STRING (delims)";
            }
            gcc_unreachable();
        }

        // Converts a frag spec enum item to a string form.
        ::std::string frag_spec_to_str(MacroFragSpec frag_spec) {
            switch (frag_spec) {
                case BLOCK:
                    return "block";
                case EXPR:
                    return "expr";
                case IDENT:
                    return "ident";
                case ITEM:
                    return "item";
                case LIFETIME:
                    return "lifetime";
                case LITERAL:
                    return "literal";
                case META:
                    return "meta";
                case PAT:
                    return "pat";
                case PATH:
                    return "path";
                case STMT:
                    return "stmt";
                case TT:
                    return "tt";
                case TY:
                    return "ty";
                case VIS:
                    return "vis";
                case INVALID:
                    return "INVALID_FRAG_SPEC";
                default:
                    return "ERROR_MARK_STRING - unknown frag spec";
            }
        }

        ::std::string Crate::as_string() const {
            fprintf(stderr, "beginning crate recursive as-string\n");

            ::std::string str("Crate: ");
            // add utf8bom and shebang
            if (has_utf8bom) {
                str += "\n has utf8bom";
            }
            if (has_shebang) {
                str += "\n has shebang";
            }

            // inner attributes
            str += "\n inner attributes: ";
            if (inner_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "inner attribute" syntax - just the body
                for (const auto& attr : inner_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            // items
            str += "\n items: ";
            if (items.empty()) {
                str += "none";
            } else {
                for (const auto& item : items) {
                    // DEBUG: null pointer check
                    if (item == NULL) {
                        fprintf(stderr,
                          "something really terrible has gone wrong - null pointer item in crate.");
                        return "NULL_POINTER_MARK";
                    }

                    str += "\n  " + item->as_string();
                }
            }

            return str + "\n";
        }

        ::std::string Attribute::as_string() const {
            ::std::string path_str = path.as_string();
            if (attr_input == NULL) {
                return path_str;
            } else {
                return path_str + attr_input->as_string();
            }
        }

        ::std::string DelimTokenTree::as_string() const {
            ::std::string start_delim;
            ::std::string end_delim;
            switch (delim_type) {
                case PARENS:
                    start_delim = "(";
                    end_delim = ")";
                    break;
                case SQUARE:
                    start_delim = "[";
                    end_delim = "]";
                    break;
                case CURLY:
                    start_delim = "{";
                    end_delim = "}";
                    break;
                default:
                    // error
                    return "";
            }
            ::std::string str = start_delim;
            if (token_trees.empty()) {
                str += "none";
            } else {
                for (const auto& tree : token_trees) {
                    // DEBUG: null pointer check
                    if (tree == NULL) {
                        fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                        "token tree in delim token tree.");
                        return "NULL_POINTER_MARK";
                    }

                    str += tree->as_string() + ", ";
                }
            }
            str += end_delim;

            return str;
        }

        ::std::string Token::as_string() const {
            /* FIXME: only works when not identifier or literal or whatever, i.e. when doesn't store
             * string value */
            // return get_token_description(token_id);

            // maybe fixed - stores everything as string though, so storage-inefficient
            return str;
        }

        ::std::string SimplePathSegment::as_string() const {
            return segment_name;
        }

        ::std::string SimplePath::as_string() const {
            ::std::string path;
            if (has_opening_scope_resolution) {
                path = "::";
            }

            // crappy hack because doing proper for loop would be more code
            bool first_time = true;
            for (const auto& segment : segments) {
                if (first_time) {
                    path += segment.as_string();
                    first_time = false;
                } else {
                    path += "::" + segment.as_string();
                }

                // DEBUG: remove later. Checks for path error.
                if (segment.is_error()) {
                    fprintf(stderr,
                      "segment in path is error - this should've been filtered out. first segment "
                      "was '%s' \n",
                      segments.at(0).as_string().c_str());
                }
            }

            return path;
        }

        ::std::string Visibility::as_string() const {
            switch (public_vis_type) {
                case NONE:
                    return ::std::string("pub");
                case CRATE:
                    return ::std::string("ub(crate)");
                case SELF:
                    return ::std::string("pub(self)");
                case SUPER:
                    return ::std::string("pub(super)");
                case IN_PATH:
                    return ::std::string("pub(in ") + in_path.as_string() + ::std::string(")");
                default:
                    gcc_unreachable();
            }
        }

        // Creates a string that reflects the visibility stored.
        ::std::string VisItem::as_string() const {
            // FIXME: can't do formatting on string to make identation occur.
            ::std::string str = Item::as_string();

            if (has_visibility()) {
                str = visibility.as_string() + " ";
            }

            return str;
        }

        // Creates a string that reflects the outer attributes stored.
        ::std::string Item::as_string() const {
            ::std::string str;

            if (!outer_attrs.empty()) {
                for (const auto& attr : outer_attrs) {
                    str += attr.as_string() + "\n";
                }
            }

            return str;
        }

        ::std::string Module::as_string() const {
            ::std::string vis_item = VisItem::as_string();

            return vis_item + "mod " + module_name;
        }

        ::std::string ModuleBodied::as_string() const {
            // get module string for "[vis] mod [name]"
            ::std::string str = Module::as_string();

            // inner attributes
            str += "\n inner attributes: ";
            if (inner_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "inner attribute" syntax - just the body
                for (const auto& attr : inner_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            // items
            str += "\n items: ";
            if (items.empty()) {
                str += "none";
            } else {
                for (const auto& item : items) {
                    // DEBUG: null pointer check
                    if (item == NULL) {
                        fprintf(stderr,
                          "something really terrible has gone wrong - null pointer item in crate.");
                        return "NULL_POINTER_MARK";
                    }

                    str += "\n  " + item->as_string();
                }
            }

            return str + "\n";
        }

        ::std::string ModuleNoBody::as_string() const {
            ::std::string str = Module::as_string();

            str += "\n no body (reference to external file)";

            return str + "\n";
        }

        ::std::string StaticItem::as_string() const {
            ::std::string str = VisItem::as_string();

            str += "static";

            if (has_mut) {
                str += " mut";
            }

            str += name;

            // DEBUG: null pointer check
            if (type == NULL) {
                fprintf(stderr,
                  "something really terrible has gone wrong - null pointer type in static item.");
                return "NULL_POINTER_MARK";
            }
            str += "\n  Type: " + type->as_string();

            // DEBUG: null pointer check
            if (expr == NULL) {
                fprintf(stderr,
                  "something really terrible has gone wrong - null pointer expr in static item.");
                return "NULL_POINTER_MARK";
            }
            str += "\n  Expression: " + expr->as_string();

            return str + "\n";
        }

        ::std::string ExternCrate::as_string() const {
            ::std::string str = VisItem::as_string();

            str += "extern crate " + referenced_crate;

            if (has_as_clause()) {
                str += " as " + as_clause_name;
            }

            return str;
        }

        ::std::string TupleStruct::as_string() const {
            ::std::string str = VisItem::as_string();

            str += "struct " + struct_name;

            // generic params
            str += "\n Generic params: ";
            if (generic_params.empty()) {
                str += "none";
            } else {
                for (const auto& param : generic_params) {
                    // DEBUG: null pointer check
                    if (param == NULL) {
                        fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                        "generic param in enum.");
                        return "NULL_POINTER_MARK";
                    }

                    str += "\n  " + param->as_string();
                }
            }

            // tuple fields
            str += "\n Tuple fields: ";
            if (fields.empty()) {
                str += "none";
            } else {
                for (const auto& field : fields) {
                    str += "\n  " + field.as_string();
                }
            }

            str += "\n Where clause: ";
            if (has_where_clause()) {
                str += where_clause.as_string();
            } else {
                str += "none";
            }

            return str;
        }

        ::std::string ConstantItem::as_string() const {
            ::std::string str = VisItem::as_string();

            str += "const " + identifier;

            // DEBUG: null pointer check
            if (type == NULL) {
                fprintf(stderr,
                  "something really terrible has gone wrong - null pointer type in const item.");
                return "NULL_POINTER_MARK";
            }
            str += "\n  Type: " + type->as_string();

            // DEBUG: null pointer check
            if (const_expr == NULL) {
                fprintf(stderr,
                  "something really terrible has gone wrong - null pointer expr in const item.");
                return "NULL_POINTER_MARK";
            }
            str += "\n  Expression: " + const_expr->as_string();

            return str + "\n";
        }

        ::std::string InherentImpl::as_string() const {
            ::std::string str = VisItem::as_string();

            str += "impl ";

            // generic params
            str += "\n Generic params: ";
            if (generic_params.empty()) {
                str += "none";
            } else {
                for (const auto& param : generic_params) {
                    // DEBUG: null pointer check
                    if (param == NULL) {
                        fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                        "generic param in inherent impl.");
                        return "NULL_POINTER_MARK";
                    }

                    str += "\n  " + param->as_string();
                }
            }

            str += "\n Type: " + trait_type->as_string();

            str += "\n Where clause: ";
            if (has_where_clause()) {
                str += where_clause.as_string();
            } else {
                str += "none";
            }

            // inner attributes
            str += "\n inner attributes: ";
            if (inner_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "inner attribute" syntax - just the body
                for (const auto& attr : inner_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            // inherent impl items
            str += "\n Inherent impl items: ";
            if (!has_impl_items()) {
                str += "none";
            } else {
                for (const auto& item : impl_items) {
                    str += "\n  " + item->as_string();
                }
            }

            return str;
        }

        ::std::string Method::as_string() const {
            ::std::string str("Method: \n ");

            str += vis.as_string() + " " + qualifiers.as_string();

            str += " fn " + method_name;

            // generic params
            str += "\n Generic params: ";
            if (generic_params.empty()) {
                str += "none";
            } else {
                for (const auto& param : generic_params) {
                    // DEBUG: null pointer check
                    if (param == NULL) {
                        fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                        "generic param in method.");
                        return "NULL_POINTER_MARK";
                    }

                    str += "\n  " + param->as_string();
                }
            }

            str += "\n Self param: " + self_param.as_string();

            str += "\n Function params: ";
            if (function_params.empty()) {
                str += "none";
            } else {
                for (const auto& param : function_params) {
                    str += "\n  " + param.as_string();
                }
            }

            str += "\n Return type: ";
            if (has_return_type()) {
                str += return_type->as_string();
            } else {
                str += "none (void)";
            }

            str += "\n Where clause: ";
            if (has_where_clause()) {
                str += where_clause.as_string();
            } else {
                str += "none";
            }

            str += "\n Block expr (body): \n  ";
            str += expr->as_string();

            return str;
        }

        ::std::string StructStruct::as_string() const {
            ::std::string str = VisItem::as_string();

            str += "struct " + struct_name;

            // generic params
            str += "\n Generic params: ";
            if (generic_params.empty()) {
                str += "none";
            } else {
                for (const auto& param : generic_params) {
                    // DEBUG: null pointer check
                    if (param == NULL) {
                        fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                        "generic param in enum.");
                        return "NULL_POINTER_MARK";
                    }

                    str += "\n  " + param->as_string();
                }
            }

            str += "\n Where clause: ";
            if (has_where_clause()) {
                str += where_clause.as_string();
            } else {
                str += "none";
            }

            // struct fields
            str += "\n Struct fields: ";
            if (is_unit) {
                str += "none (unit)";
            } else if (fields.empty()) {
                str += "none (non-unit)";
            } else {
                for (const auto& field : fields) {
                    str += "\n  " + field.as_string();
                }
            }

            return str;
        }

        ::std::string UseDeclaration::as_string() const {
            ::std::string str = VisItem::as_string();

            // DEBUG: null pointer check
            if (use_tree == NULL) {
                fprintf(stderr, "something really terrible has gone wrong - null pointer use tree in "
                                "use declaration.");
                return "NULL_POINTER_MARK";
            }

            str += "use " + use_tree->as_string();

            return str;
        }

        ::std::string UseTreeGlob::as_string() const {
            switch (glob_type) {
                case NO_PATH:
                    return "*";
                case GLOBAL:
                    return "::*";
                case PATH_PREFIXED: {
                    ::std::string path_str = path.as_string();
                    return path_str + "::*";
                }
                default:
                    // some kind of error
                    return "ERROR-PATH";
            }
            gcc_unreachable();
        }

        ::std::string UseTreeList::as_string() const {
            ::std::string path_str;
            switch (path_type) {
                case NO_PATH:
                    path_str = "{";
                    break;
                case GLOBAL:
                    path_str = "::{";
                    break;
                case PATH_PREFIXED: {
                    path_str = path.as_string() + "::{";
                    break;
                }
                default:
                    // some kind of error
                    return "ERROR-PATH-LIST";
            }

            if (has_trees()) {
                for (const auto& tree : trees) {
                    // DEBUG: null pointer check
                    if (tree == NULL) {
                        fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                        "tree in use tree list.");
                        return "NULL_POINTER_MARK";
                    }

                    path_str += tree->as_string() + ", ";
                }
            } else {
                path_str += "none";
            }

            return path_str + "}";
        }

        ::std::string UseTreeRebind::as_string() const {
            ::std::string path_str = path.as_string();

            switch (bind_type) {
                case NONE:
                    // nothing to add, just path
                    break;
                case IDENTIFIER:
                    path_str += " as " + identifier;
                    break;
                case WILDCARD:
                    path_str += " as _";
                    break;
                default:
                    // error
                    return "ERROR-PATH-REBIND";
            }

            return path_str;
        }

        ::std::string Enum::as_string() const {
            ::std::string str = VisItem::as_string();
            str += enum_name;

            // generic params
            str += "\n Generic params: ";
            if (generic_params.empty()) {
                str += "none";
            } else {
                for (const auto& param : generic_params) {
                    // DEBUG: null pointer check
                    if (param == NULL) {
                        fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                        "generic param in enum.");
                        return "NULL_POINTER_MARK";
                    }

                    str += "\n  " + param->as_string();
                }
            }

            str += "\n Where clause: ";
            if (has_where_clause()) {
                str += where_clause.as_string();
            } else {
                str += "none";
            }

            // items
            str += "\n Items: ";
            if (items.empty()) {
                str += "none";
            } else {
                for (const auto& item : items) {
                    // DEBUG: null pointer check
                    if (item == NULL) {
                        fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                        "enum item in enum.");
                        return "NULL_POINTER_MARK";
                    }

                    str += "\n  " + item->as_string();
                }
            }

            return str;
        }

        ::std::string Trait::as_string() const {
            ::std::string str = VisItem::as_string();

            if (has_unsafe) {
                str += "unsafe ";
            }

            str += "trait " + name;

            // generic params
            str += "\n Generic params: ";
            if (generic_params.empty()) {
                str += "none";
            } else {
                for (const auto& param : generic_params) {
                    // DEBUG: null pointer check
                    if (param == NULL) {
                        fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                        "generic param in trait.");
                        return "NULL_POINTER_MARK";
                    }

                    str += "\n  " + param->as_string();
                }
            }

            str += "\n Type param bounds: ";
            if (!has_type_param_bounds()) {
                str += "none";
            } else {
                for (const auto& bound : type_param_bounds) {
                    // DEBUG: null pointer check
                    if (bound == NULL) {
                        fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                        "type param bound in trait.");
                        return "NULL_POINTER_MARK";
                    }

                    str += "\n  " + bound->as_string();
                }
            }

            str += "\n Where clause: ";
            if (!has_where_clause()) {
                str += "none";
            } else {
                str += where_clause.as_string();
            }

            str += "\n Trait items: ";
            if (!has_trait_items()) {
                str += "none";
            } else {
                for (const auto& item : trait_items) {
                    // DEBUG: null pointer check
                    if (item == NULL) {
                        fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                        "trait item in trait.");
                        return "NULL_POINTER_MARK";
                    }

                    str += "\n  " + item->as_string();
                }
            }

            return str;
        }

        ::std::string Union::as_string() const {
            ::std::string str = VisItem::as_string();

            str += "union " + union_name;

            // generic params
            str += "\n Generic params: ";
            if (generic_params.empty()) {
                str += "none";
            } else {
                for (const auto& param : generic_params) {
                    // DEBUG: null pointer check
                    if (param == NULL) {
                        fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                        "generic param in union.");
                        return "NULL_POINTER_MARK";
                    }

                    str += "\n  " + param->as_string();
                }
            }

            str += "\n Where clause: ";
            if (has_where_clause()) {
                str += where_clause.as_string();
            } else {
                str += "none";
            }

            // struct fields
            str += "\n Struct fields (variants): ";
            if (variants.empty()) {
                str += "none";
            } else {
                for (const auto& field : variants) {
                    str += "\n  " + field.as_string();
                }
            }

            return str;
        }

        ::std::string Function::as_string() const {
            ::std::string str = VisItem::as_string() + "Function: ";
            ::std::string qualifiers_str = "Qualifiers: " + qualifiers.as_string();

            ::std::string generic_params_str("Generic params: ");
            if (has_generics()) {
                for (const auto& generic_param : generic_params) {
                    // DEBUG: null pointer check
                    if (generic_param == NULL) {
                        fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                        "generic param in function item.");
                        return "NULL_POINTER_MARK";
                    }

                    generic_params_str += generic_param->as_string() + ", ";
                }
            } else {
                generic_params_str += "none";
            }

            ::std::string function_params_str("Function params: ");
            if (has_function_params()) {
                for (const auto& param : function_params) {
                    function_params_str += param.as_string() + ", ";
                }
            } else {
                function_params_str += "none";
            }

            ::std::string return_type_str("Return type: ");
            if (has_function_return_type()) {
                // DEBUG: null pointer check
                if (return_type == NULL) {
                    fprintf(stderr, "something really terrible has gone wrong - null pointer return "
                                    "type in function.");
                    return "NULL_POINTER_MARK";
                }

                return_type_str += return_type->as_string();
            } else {
                return_type_str += "none (void)";
            }

            ::std::string where_clause_str("Where clause: ");
            if (has_where_clause()) {
                where_clause_str += where_clause.as_string();
            } else {
                where_clause_str += "none";
            }

            // DEBUG: null pointer check
            if (function_body == NULL) {
                fprintf(stderr, "something really terrible has gone wrong - null pointer function "
                                "body in function.");
                return "NULL_POINTER_MARK";
            }
            ::std::string body_str = "Body: " + function_body->as_string();

            str += "\n   " + qualifiers_str + "\n   " + generic_params_str + "\n   "
                   + function_params_str + "\n   " + return_type_str + "\n   " + where_clause_str
                   + "\n   " + body_str;

            return str;
        }

        ::std::string WhereClause::as_string() const {
            // just print where clause items, don't mention "where" or "where clause"
            ::std::string str;

            if (where_clause_items.empty()) {
                str = "none";
            } else {
                for (const auto& item : where_clause_items) {
                    str += "\n  " + item->as_string();
                }
            }

            return str;
        }

        ::std::string BlockExpr::as_string() const {
            ::std::string str = "BlockExpr: ";

            // get outer attributes
            str += "\n " + Expr::as_string();

            // inner attributes
            str += "\n inner attributes: ";
            if (inner_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "inner attribute" syntax - just the body
                for (const auto& attr : inner_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            // statements
            str += "\n statements: ";
            if (statements.empty()) {
                str += "none";
            } else {
                for (const auto& stmt : statements) {
                    // DEBUG: null pointer check
                    if (stmt == NULL) {
                        fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                        "stmt in block expr.");
                        return "NULL_POINTER_MARK";
                    }

                    str += "\n  " + stmt->as_string();
                }
            }

            // final expression
            str += "\n final expression: ";
            if (expr == NULL) {
                str += "none";
            } else {
                str += "\n  " + expr->as_string();
            }

            return str;
        }

        ::std::string TraitImpl::as_string() const {
            ::std::string str = VisItem::as_string();

            if (has_unsafe) {
                str += "unsafe ";
            }

            str += "impl ";

            // generic params
            str += "\n Generic params: ";
            if (!has_generics()) {
                str += "none";
            } else {
                for (const auto& param : generic_params) {
                    str += "\n  " + param->as_string();
                }
            }

            str += "\n Has exclam: ";
            if (has_exclam) {
                str += "true";
            } else {
                str += "false";
            }

            str += "\n TypePath (to trait): " + trait_path.as_string();

            str += "\n Type (struct to impl on): " + trait_type->as_string();

            str += "\n Where clause: ";
            if (!has_where_clause()) {
                str += "none";
            } else {
                str += where_clause.as_string();
            }

            // inner attributes
            str += "\n inner attributes: ";
            if (inner_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "inner attribute" syntax - just the body
                for (const auto& attr : inner_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            str += "\n trait impl items: ";
            if (!has_impl_items()) {
                str += "none";
            } else {
                for (const auto& item : impl_items) {
                    str += "\n  " + item->as_string();
                }
            }

            return str;
        }

        ::std::string TypeAlias::as_string() const {
            ::std::string str = VisItem::as_string();

            str += " " + new_type_name;

            // generic params
            str += "\n Generic params: ";
            if (!has_generics()) {
                str += "none";
            } else {
                for (const auto& param : generic_params) {
                    str += param->as_string() + ", ";
                }
            }

            str += "\n Where clause: ";
            if (!has_where_clause()) {
                str += "none";
            } else {
                str += where_clause.as_string();
            }

            str += "\n Type: " + existing_type->as_string();

            return str;
        }

        ::std::string MacroInvocationSemi::as_string() const {
            // get outer attrs
            ::std::string str = MacroItem::as_string();

            str += "\n" + path.as_string() + "!";

            ::std::string tok_trees;
            if (token_trees.empty()) {
                tok_trees = "none";
            } else {
                for (const auto& tree : token_trees) {
                    // DEBUG: null pointer check
                    if (tree == NULL) {
                        fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                        "token tree in macro invocation semi.");
                        return "NULL_POINTER_MARK";
                    }

                    tok_trees += tree->as_string() + ", ";
                }
            }

            return str + get_string_in_delims(::std::move(tok_trees), delim_type);
        }

        ::std::string ExternBlock::as_string() const {
            ::std::string str = VisItem::as_string();

            str += "extern ";
            if (has_abi()) {
                str += "\"" + abi + "\" ";
            }

            // inner attributes
            str += "\n inner attributes: ";
            if (inner_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "inner attribute" syntax - just the body
                for (const auto& attr : inner_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            str += "\n external items: ";
            if (!has_extern_items()) {
                str += "none";
            } else {
                for (const auto& item : extern_items) {
                    str += "\n  " + item->as_string();
                }
            }

            return str;
        }

        ::std::string MacroRule::as_string() const {
            ::std::string str("Macro rule: ");

            str += "\n Matcher: \n  ";
            str += matcher.as_string();

            str += "\n Transcriber: \n  ";
            str += transcriber.as_string();

            return str;
        }

        ::std::string MacroRulesDefinition::as_string() const {
            ::std::string str("macro_rules!");

            str += rule_name;

            str += "\n Macro rules: ";
            if (rules.empty()) {
                str += "none";
            } else {
                for (const auto& rule : rules) {
                    str += "\n  " + rule.as_string();
                }
            }

            str += "\n Delim type: ";
            switch (delim_type) {
                case PARENS:
                    str += "parentheses";
                    break;
                case SQUARE:
                    str += "square";
                    break;
                case CURLY:
                    str += "curly";
                    break;
                default:
                    return "ERROR_MARK_STRING - delim type in macro invocation";
            }

            return str;
        }

        ::std::string MacroInvocation::as_string() const {
            return path.as_string() + "!" + token_tree.as_string();
        }

        ::std::string PathInExpression::as_string() const {
            ::std::string str;

            if (has_opening_scope_resolution) {
                str = "::";
            }

            return str + PathPattern::as_string();
        }

        ::std::string ExprStmtWithBlock::as_string() const {
            ::std::string str("ExprStmtWithBlock: \n ");

            if (expr == NULL) {
                str += "none (this should not happen and is an error)";
            } else {
                str += expr->as_string();
            }

            return str;
        }

        ::std::string ClosureParam::as_string() const {
            ::std::string str(pattern->as_string());

            if (has_type_given()) {
                str += " : " + type->as_string();
            }

            return str;
        }

        ::std::string ClosureExpr::as_string() const {
            ::std::string str("ClosureExpr:\n Has move: ");
            if (has_move) {
                str += "true";
            } else {
                str += "false";
            }

            str += "\n Params: ";
            if (params.empty()) {
                str += "none";
            } else {
                for (const auto& param : params) {
                    str += "\n  " + param.as_string();
                }
            }

            return str;
        }

        ::std::string ClosureExprInnerTyped::as_string() const {
            ::std::string str = ClosureExpr::as_string();

            str += "\n Return type: " + return_type->as_string();

            str += "\n Body: " + expr->as_string();

            return str;
        }

        ::std::string PathPattern::as_string() const {
            ::std::string str;

            for (const auto& segment : segments) {
                str += segment.as_string() + "::";
            }

            // basically a hack - remove last two characters of string (remove final ::)
            str.erase(str.length() - 2);

            return str;
        }

        ::std::string QualifiedPathType::as_string() const {
            ::std::string str("<");
            str += type_to_invoke_on->as_string();

            if (has_as_clause()) {
                str += " as " + trait_path.as_string();
            }

            return str + ">";
        }

        ::std::string QualifiedPathInExpression::as_string() const {
            return path_type.as_string() + "::" + PathPattern::as_string();
        }

        ::std::string BorrowExpr::as_string() const {
            ::std::string str("&");

            if (double_borrow) {
                str += "&";
            }

            if (is_mut) {
                str += "mut ";
            }

            str += main_or_left_expr->as_string();

            return str;
        }

        ::std::string ReturnExpr::as_string() const {
            ::std::string str("return ");

            if (has_return_expr()) {
                str += return_expr->as_string();
            }

            return str;
        }

        ::std::string GroupedExpr::as_string() const {
            ::std::string str("Grouped expr:");

            // inner attributes
            str += "\n inner attributes: ";
            if (inner_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "inner attribute" syntax - just the body
                for (const auto& attr : inner_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            str += "\n Expr in parens: " + expr_in_parens->as_string();

            return str;
        }

        ::std::string RangeToExpr::as_string() const {
            return ".." + to->as_string();
        }

        ::std::string ContinueExpr::as_string() const {
            ::std::string str("continue ");

            if (has_label()) {
                str += label.as_string();
            }

            return str;
        }

        ::std::string NegationExpr::as_string() const {
            ::std::string str;

            switch (negation_type) {
                case NEGATE:
                    str = "-";
                    break;
                case NOT:
                    str = "!";
                    break;
                default:
                    return "ERROR_MARK_STRING - negation expr";
            }

            str += main_or_left_expr->as_string();

            return str;
        }

        ::std::string RangeFromExpr::as_string() const {
            return from->as_string() + "..";
        }

        ::std::string RangeFullExpr::as_string() const {
            return "..";
        }

        ::std::string ArrayIndexExpr::as_string() const {
            return array_expr->as_string() + "[" + index_expr->as_string() + "]";
        }

        ::std::string AssignmentExpr::as_string() const {
            ::std::string str("AssignmentExpr: ");

            if (main_or_left_expr == NULL || right_expr == NULL) {
                str += "error (either or both expressions are null)";
            } else {
                // left expr
                str += "\n left: " + main_or_left_expr->as_string();

                // right expr
                str += "\n right: " + right_expr->as_string();
            }

            return str;
        }

        ::std::string AsyncBlockExpr::as_string() const {
            ::std::string str = "AsyncBlockExpr: ";

            // get outer attributes
            str += "\n " + Expr::as_string();

            str += "\n Has move: ";
            str += has_move ? "true" : "false";

            return str + "\n" + block_expr->as_string();
        }

        ::std::string ComparisonExpr::as_string() const {
            ::std::string str(main_or_left_expr->as_string());

            switch (expr_type) {
                case EQUAL:
                    str += " == ";
                    break;
                case NOT_EQUAL:
                    str += " != ";
                    break;
                case GREATER_THAN:
                    str += " > ";
                    break;
                case LESS_THAN:
                    str += " < ";
                    break;
                case GREATER_OR_EQUAL:
                    str += " >= ";
                    break;
                case LESS_OR_EQUAL:
                    str += " <= ";
                    break;
                default:
                    return "ERROR_MARK_STRING - comparison expr";
            }

            str += right_expr->as_string();

            return str;
        }

        ::std::string MethodCallExpr::as_string() const {
            ::std::string str("MethodCallExpr: \n Object (receiver) expr: ");

            str += receiver->as_string();

            str += "\n Method path segment: \n";

            str += method_name.as_string();

            str += "\n Call params:";
            if (params.empty()) {
                str += "none";
            } else {
                for (const auto& param : params) {
                    if (param == NULL) {
                        return "ERROR_MARK_STRING - method call expr param is null";
                    }

                    str += "\n  " + param->as_string();
                }
            }

            return str;
        }

        ::std::string TupleIndexExpr::as_string() const {
            return tuple_expr->as_string() + "." + ::std::to_string(tuple_index);
        }

        ::std::string DereferenceExpr::as_string() const {
            return "*" + main_or_left_expr->as_string();
        }

        ::std::string FieldAccessExpr::as_string() const {
            return receiver->as_string() + "." + field;
        }

        ::std::string LazyBooleanExpr::as_string() const {
            ::std::string str(main_or_left_expr->as_string());

            switch (expr_type) {
                case LOGICAL_OR:
                    str += " || ";
                    break;
                case LOGICAL_AND:
                    str += " && ";
                    break;
                default:
                    return "ERROR_MARK_STRING - lazy boolean expr out of bounds";
            }

            str += right_expr->as_string();

            return str;
        }

        ::std::string RangeFromToExpr::as_string() const {
            return from->as_string() + ".." + to->as_string();
        }

        ::std::string RangeToInclExpr::as_string() const {
            return "..=" + to->as_string();
        }

        ::std::string UnsafeBlockExpr::as_string() const {
            ::std::string str = "UnsafeBlockExpr: ";

            // get outer attributes
            str += "\n " + Expr::as_string();

            return str + "\n" + expr->as_string();
        }

        ::std::string ClosureExprInner::as_string() const {
            ::std::string str = ClosureExpr::as_string();

            str += "\n Expression: " + closure_inner->as_string();

            return str;
        }

        ::std::string IfExpr::as_string() const {
            ::std::string str("IfExpr: ");

            str += "\n Condition expr: " + condition->as_string();

            str += "\n If block expr: " + if_block->as_string();

            return str;
        }

        ::std::string IfExprConseqElse::as_string() const {
            ::std::string str = IfExpr::as_string();

            str += "\n Else block expr: " + else_block->as_string();

            return str;
        }

        ::std::string IfExprConseqIf::as_string() const {
            ::std::string str = IfExpr::as_string();

            str += "\n Else if expr: \n  " + if_expr->as_string();

            return str;
        }

        ::std::string IfExprConseqIfLet::as_string() const {
            ::std::string str = IfExpr::as_string();

            str += "\n Else if let expr: \n  " + if_let_expr->as_string();

            return str;
        }

        ::std::string IfLetExpr::as_string() const {
            ::std::string str("IfLetExpr: ");

            str += "\n Condition match arm patterns: ";
            if (match_arm_patterns.empty()) {
                str += "none";
            } else {
                for (const auto& pattern : match_arm_patterns) {
                    str += "\n  " + pattern->as_string();
                }
            }

            str += "\n Scrutinee expr: " + value->as_string();

            str += "\n If let block expr: " + if_block->as_string();

            return str;
        }

        ::std::string IfLetExprConseqElse::as_string() const {
            ::std::string str = IfLetExpr::as_string();

            str += "\n Else block expr: " + else_block->as_string();

            return str;
        }

        ::std::string IfLetExprConseqIf::as_string() const {
            ::std::string str = IfLetExpr::as_string();

            str += "\n Else if expr: \n  " + if_expr->as_string();

            return str;
        }

        ::std::string IfLetExprConseqIfLet::as_string() const {
            ::std::string str = IfLetExpr::as_string();

            str += "\n Else if let expr: \n  " + if_let_expr->as_string();

            return str;
        }

        ::std::string RangeFromToInclExpr::as_string() const {
            return from->as_string() + "..=" + to->as_string();
        }

        ::std::string ErrorPropogationExpr::as_string() const {
            return main_or_left_expr->as_string() + "?";
        }

        ::std::string CompoundAssignmentExpr::as_string() const {
            ::std::string operator_str;
            operator_str.reserve(1);

            // get operator string
            switch (expr_type) {
                case ADD:
                    operator_str = "+";
                    break;
                case SUBTRACT:
                    operator_str = "-";
                    break;
                case MULTIPLY:
                    operator_str = "*";
                    break;
                case DIVIDE:
                    operator_str = "/";
                    break;
                case MODULUS:
                    operator_str = "%";
                    break;
                case BITWISE_AND:
                    operator_str = "&";
                    break;
                case BITWISE_OR:
                    operator_str = "|";
                    break;
                case BITWISE_XOR:
                    operator_str = "^";
                    break;
                case LEFT_SHIFT:
                    operator_str = "<<";
                    break;
                case RIGHT_SHIFT:
                    operator_str = ">>";
                    break;
                default:
                    operator_str = "invalid operator. wtf";
                    break;
            }

            operator_str += "=";

            ::std::string str("CompoundAssignmentExpr: ");
            if (main_or_left_expr == NULL || right_expr == NULL) {
                str += "error. this is probably a parsing failure.";
            } else {
                str += "\n left: " + main_or_left_expr->as_string();
                str += "\n right: " + right_expr->as_string();
                str += "\n operator: " + operator_str;
            }

            return str;
        }

        ::std::string ArithmeticOrLogicalExpr::as_string() const {
            ::std::string operator_str;
            operator_str.reserve(1);

            // get operator string
            switch (expr_type) {
                case ADD:
                    operator_str = "+";
                    break;
                case SUBTRACT:
                    operator_str = "-";
                    break;
                case MULTIPLY:
                    operator_str = "*";
                    break;
                case DIVIDE:
                    operator_str = "/";
                    break;
                case MODULUS:
                    operator_str = "%";
                    break;
                case BITWISE_AND:
                    operator_str = "&";
                    break;
                case BITWISE_OR:
                    operator_str = "|";
                    break;
                case BITWISE_XOR:
                    operator_str = "^";
                    break;
                case LEFT_SHIFT:
                    operator_str = "<<";
                    break;
                case RIGHT_SHIFT:
                    operator_str = ">>";
                    break;
                default:
                    operator_str = "invalid operator. wtf";
                    break;
            }

            ::std::string str("ArithmeticOrLogicalExpr: ");
            if (main_or_left_expr == NULL || right_expr == NULL) {
                str += "error. this is probably a parsing failure.";
            } else {
                str += "\n left: " + main_or_left_expr->as_string();
                str += "\n right: " + right_expr->as_string();
                str += "\n operator: " + operator_str;
            }

            return str;
        }

        ::std::string CallExpr::as_string() const {
            ::std::string str("CallExpr: \n Function expr: ");

            str += function->as_string();

            str += "\n Call params:";
            if (!has_params()) {
                str += "none";
            } else {
                for (const auto& param : params) {
                    if (param == NULL) {
                        return "ERROR_MARK_STRING - call expr param is null";
                    }

                    str += "\n  " + param->as_string();
                }
            }

            return str;
        }

        ::std::string WhileLoopExpr::as_string() const {
            ::std::string str("WhileLoopExpr: ");

            str += "\n Label: ";
            if (!has_loop_label()) {
                str += "none";
            } else {
                str += loop_label.as_string();
            }

            str += "\n Conditional expr: " + condition->as_string();

            str += "\n Loop block: " + loop_block->as_string();

            return str;
        }

        ::std::string WhileLetLoopExpr::as_string() const {
            ::std::string str("WhileLetLoopExpr: ");

            str += "\n Label: ";
            if (!has_loop_label()) {
                str += "none";
            } else {
                str += loop_label.as_string();
            }

            str += "\n Match arm patterns: ";
            if (match_arm_patterns.empty()) {
                str += "none";
            } else {
                for (const auto& pattern : match_arm_patterns) {
                    str += "\n  " + pattern->as_string();
                }
            }

            str += "\n Scrutinee expr: " + condition->as_string();

            str += "\n Loop block: " + loop_block->as_string();

            return str;
        }

        ::std::string LoopExpr::as_string() const {
            ::std::string str("LoopExpr: (infinite loop)");

            str += "\n Label: ";
            if (!has_loop_label()) {
                str += "none";
            } else {
                str += loop_label.as_string();
            }

            str += "\n Loop block: " + loop_block->as_string();

            return str;
        }

        ::std::string ArrayExpr::as_string() const {
            ::std::string str("ArrayExpr:");

            // inner attributes
            str += "\n inner attributes: ";
            if (inner_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "inner attribute" syntax - just the body
                for (const auto& attr : inner_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            str += "\n Array elems: ";
            if (!has_array_elems()) {
                str += "none";
            } else {
                str += internal_elements->as_string();
            }

            return str;
        }

        ::std::string AwaitExpr::as_string() const {
            return awaited_expr->as_string() + ".await";
        }

        ::std::string BreakExpr::as_string() const {
            ::std::string str("break ");

            if (has_label()) {
                str += label.as_string() + " ";
            }

            if (has_break_expr()) {
                str += break_expr->as_string();
            }

            return str;
        }

        ::std::string LoopLabel::as_string() const {
            return label.as_string() + ": (label) ";
        }

        ::std::string MatchArm::as_string() const {
            // outer attributes
            ::std::string str = "Outer attributes: ";
            if (outer_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "outer attribute" syntax - just the body
                for (const auto& attr : outer_attrs) {
                    str += "\n " + attr.as_string();
                }
            }

            str += "\nPatterns: ";
            if (match_arm_patterns.empty()) {
                str += "none";
            } else {
                for (const auto& pattern : match_arm_patterns) {
                    str += "\n " + pattern->as_string();
                }
            }

            str += "\nGuard expr: ";
            if (!has_match_arm_guard()) {
                str += "none";
            } else {
                str += guard_expr->as_string();
            }

            return str;
        }

        ::std::string MatchCase::as_string() const {
            ::std::string str("MatchCase: (match arm) ");

            str += "\n Match arm matcher: \n" + arm.as_string();

            return str;
        }

        ::std::string MatchCaseBlockExpr::as_string() const {
            ::std::string str = MatchCase::as_string();

            str += "\n Block expr: " + block_expr->as_string();

            return str;
        }

        ::std::string MatchCaseExpr::as_string() const {
            ::std::string str = MatchCase::as_string();

            str += "\n Expr: " + expr->as_string();

            return str;
        }

        ::std::string MatchExpr::as_string() const {
            ::std::string str("MatchExpr:");

            str += "\n Scrutinee expr: " + branch_value->as_string();

            // inner attributes
            str += "\n inner attributes: ";
            if (inner_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "inner attribute" syntax - just the body
                for (const auto& attr : inner_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            // match arms
            str += "\n Match arms: ";
            if (match_arms.empty()) {
                str += "none";
            } else {
                for (const auto& arm : match_arms) {
                    str += "\n  " + arm->as_string();
                }
            }

            return str;
        }

        ::std::string TupleExpr::as_string() const {
            ::std::string str("TupleExpr:");

            // inner attributes
            str += "\n inner attributes: ";
            if (inner_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "inner attribute" syntax - just the body
                for (const auto& attr : inner_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            str += "\n Tuple elements: ";
            if (tuple_elems.empty()) {
                str += "none";
            } else {
                for (const auto& elem : tuple_elems) {
                    str += "\n  " + elem->as_string();
                }
            }

            return str;
        }

        ::std::string ExprStmtWithoutBlock::as_string() const {
            ::std::string str("ExprStmtWithoutBlock: \n ");

            if (expr == NULL) {
                str += "none (this shouldn't happen and is probably an error)";
            } else {
                str += expr->as_string();
            }

            return str;
        }

        ::std::string FunctionParam::as_string() const {
            return param_name->as_string() + " : " + type->as_string();
        }

        ::std::string FunctionQualifiers::as_string() const {
            ::std::string str;

            switch (const_status) {
                case NONE:
                    // do nothing
                    break;
                case CONST:
                    str += "const ";
                    break;
                case ASYNC:
                    str += "async ";
                    break;
                default:
                    return "ERROR_MARK_STRING: async-const status failure";
            }

            if (has_unsafe) {
                str += "unsafe ";
            }

            if (has_extern) {
                str += "extern";
                if (extern_abi != "") {
                    str += " \"" + extern_abi + "\"";
                }
            }

            return str;
        }

        ::std::string TraitBound::as_string() const {
            ::std::string str("TraitBound:");

            str += "\n Has opening question mark: ";
            if (opening_question_mark) {
                str += "true";
            } else {
                str += "false";
            }

            str += "\n For lifetimes: ";
            if (!has_for_lifetimes()) {
                str += "none";
            } else {
                for (const auto& lifetime : for_lifetimes) {
                    str += "\n  " + lifetime.as_string();
                }
            }

            str += "\n Type path: " + type_path.as_string();

            return str;
        }

        ::std::string MacroMatcher::as_string() const {
            ::std::string str("Macro matcher: ");

            str += "\n Delim type: ";

            switch (delim_type) {
                case PARENS:
                    str += "parentheses";
                    break;
                case SQUARE:
                    str += "square";
                    break;
                case CURLY:
                    str += "curly";
                    break;
                default:
                    return "ERROR_MARK_STRING - macro matcher delim";
            }

            str += "\n Matches: ";

            if (matches.empty()) {
                str += "none";
            } else {
                for (const auto& match : matches) {
                    str += "\n  " + match->as_string();
                }
            }

            return str;
        }

        ::std::string LifetimeParam::as_string() const {
            ::std::string str("LifetimeParam: ");

            str += "\n Outer attribute: ";
            if (!has_outer_attribute()) {
                str += "none";
            } else {
                str += outer_attr.as_string();
            }

            str += "\n Lifetime: " + lifetime.as_string();

            str += "\n Lifetime bounds: ";
            if (!has_lifetime_bounds()) {
                str += "none";
            } else {
                for (const auto& bound : lifetime_bounds) {
                    str += "\n  " + bound.as_string();
                }
            }

            return str;
        }

        ::std::string MacroMatchFragment::as_string() const {
            return "$" + ident + ": " + frag_spec_to_str(frag_spec);
        }

        ::std::string QualifiedPathInType::as_string() const {
            ::std::string str = path_type.as_string();

            for (const auto& segment : segments) {
                str += "::" + segment->as_string();
            }

            return str;
        }

        ::std::string MacroMatchRepetition::as_string() const {
            ::std::string str("Macro match repetition: ");

            str += "\n Matches: ";
            if (matches.empty()) {
                str += "none";
            } else {
                for (const auto& match : matches) {
                    str += "\n  " + match->as_string();
                }
            }

            str += "\n Sep: ";
            if (!has_sep()) {
                str += "none";
            } else {
                str += sep->as_string();
            }

            str += "\n Op: ";
            switch (op) {
                case ASTERISK:
                    str += "*";
                    break;
                case PLUS:
                    str += "+";
                    break;
                case QUESTION_MARK:
                    str += "?";
                    break;
                case NONE:
                    str += "no op? shouldn't be allowed";
                    break;
                default:
                    return "ERROR_MARK_STRING - unknown op in macro match repetition";
            }

            return str;
        }

        ::std::string Lifetime::as_string() const {
            if (is_error()) {
                return "error lifetime";
            }

            switch (lifetime_type) {
                case NAMED:
                    return "'" + lifetime_name;
                case STATIC:
                    return "'static";
                case WILDCARD:
                    return "'_";
                default:
                    return "ERROR-MARK-STRING: lifetime type failure";
            }
        }

        ::std::string TypePath::as_string() const {
            ::std::string str;

            if (has_opening_scope_resolution) {
                str = "::";
            }

            for (const auto& segment : segments) {
                str += segment->as_string() + "::";
            }

            // kinda hack - remove last 2 '::' characters
            str.erase(str.length() - 2);

            return str;
        }

        ::std::string TypeParam::as_string() const {
            ::std::string str("TypeParam: ");

            str += "\n Outer attribute: ";
            if (!has_outer_attribute()) {
                str += "none";
            } else {
                str += outer_attr.as_string();
            }

            str += "\n Identifier: " + type_representation;

            str += "\n Type param bounds: ";
            if (!has_type_param_bounds()) {
                str += "none";
            } else {
                for (const auto& bound : type_param_bounds) {
                    str += "\n  " + bound->as_string();
                }
            }

            str += "\n Type: ";
            if (!has_type()) {
                str += "none";
            } else {
                str += type->as_string();
            }

            return str;
        }

        SimplePath PathPattern::convert_to_simple_path(bool with_opening_scope_resolution) const {
            if (!has_segments()) {
                return SimplePath::create_empty();
            }

            // create vector of reserved size (to minimise reallocations)
            ::std::vector<SimplePathSegment> simple_segments;
            simple_segments.reserve(segments.size());

            for (const auto& segment : segments) {
                // return empty path if doesn't meet simple path segment requirements
                if (segment.is_error() || segment.has_generic_args()
                    || segment.as_string() == "Self") {
                    return SimplePath::create_empty();
                }

                // create segment and add to vector
                ::std::string segment_str = segment.as_string();
                simple_segments.push_back(SimplePathSegment(::std::move(segment_str)));
            }

            return SimplePath(::std::move(simple_segments), with_opening_scope_resolution);
        }

        SimplePath TypePath::as_simple_path() const {
            if (segments.empty()) {
                return SimplePath::create_empty();
            }

            // create vector of reserved size (to minimise reallocations)
            ::std::vector<SimplePathSegment> simple_segments;
            simple_segments.reserve(segments.size());

            for (const auto& segment : segments) {
                // return empty path if doesn't meet simple path segment requirements
                if (segment == NULL || segment->is_error() || !segment->is_ident_only()
                    || segment->as_string() == "Self") {
                    return SimplePath::create_empty();
                }

                // create segment and add to vector
                ::std::string segment_str = segment->as_string();
                simple_segments.push_back(SimplePathSegment(::std::move(segment_str)));
            }

            return SimplePath(::std::move(simple_segments), has_opening_scope_resolution);
        }

        ::std::string PathExprSegment::as_string() const {
            ::std::string ident_str = segment_name.as_string();
            if (has_generic_args()) {
                ident_str += "::<" + generic_args.as_string() + ">";
            }

            return ident_str;
        }

        ::std::string GenericArgs::as_string() const {
            ::std::string args;

            // lifetime args
            if (!lifetime_args.empty()) {
                for (const auto& lifetime_arg : lifetime_args) {
                    args += lifetime_arg.as_string() + ", ";
                }
            }

            // type args
            if (!type_args.empty()) {
                for (const auto& type_arg : type_args) {
                    args += type_arg->as_string() + ", ";
                }
            }

            // binding args
            if (!binding_args.empty()) {
                for (const auto& binding_arg : binding_args) {
                    args += binding_arg.as_string() + ", ";
                }
            }

            return args;
        }

        ::std::string GenericArgsBinding::as_string() const {
            return identifier + " = " + type->as_string();
        }

        ::std::string ForLoopExpr::as_string() const {
            ::std::string str("ForLoopExpr: ");

            str += "\n Label: ";
            if (!has_loop_label()) {
                str += "none";
            } else {
                str += loop_label.as_string();
            }

            str += "\n Pattern: " + pattern->as_string();

            str += "\n Iterator expr: " + iterator_expr->as_string();

            str += "\n Loop block: " + loop_block->as_string();

            return str;
        }

        ::std::string RangePattern::as_string() const {
            if (has_ellipsis_syntax) {
                return lower->as_string() + "..." + upper->as_string();
            } else {
                return lower->as_string() + "..=" + upper->as_string();
            }
        }

        ::std::string RangePatternBoundLiteral::as_string() const {
            ::std::string str;

            if (has_minus) {
                str += "-";
            }

            str += literal.as_string();

            return str;
        }

        ::std::string SlicePattern::as_string() const {
            ::std::string str("SlicePattern: ");

            for (const auto& pattern : items) {
                str += "\n " + pattern->as_string();
            }

            return str;
        }

        ::std::string TuplePatternItemsMultiple::as_string() const {
            ::std::string str;

            for (const auto& pattern : patterns) {
                str += "\n " + pattern->as_string();
            }

            return str;
        }

        ::std::string TuplePatternItemsRanged::as_string() const {
            ::std::string str;

            str += "\n Lower patterns: ";
            if (lower_patterns.empty()) {
                str += "none";
            } else {
                for (const auto& lower : lower_patterns) {
                    str += "\n  " + lower->as_string();
                }
            }

            str += "\n Upper patterns: ";
            if (upper_patterns.empty()) {
                str += "none";
            } else {
                for (const auto& upper : upper_patterns) {
                    str += "\n  " + upper->as_string();
                }
            }

            return str;
        }

        ::std::string TuplePattern::as_string() const {
            return "TuplePattern: " + items->as_string();
        }

        ::std::string StructPatternField::as_string() const {
            // outer attributes
            ::std::string str("Outer attributes: ");
            if (outer_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "outer attribute" syntax - just the body
                for (const auto& attr : outer_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            return str;
        }

        ::std::string StructPatternFieldIdent::as_string() const {
            ::std::string str = StructPatternField::as_string();

            str += "\n";

            if (has_ref) {
                str += "ref ";
            }

            if (has_mut) {
                str += "mut ";
            }

            str += ident;

            return str;
        }

        ::std::string StructPatternFieldTuplePat::as_string() const {
            ::std::string str = StructPatternField::as_string();

            str += "\n";

            str += ::std::to_string(index) + " : " + tuple_pattern->as_string();

            return str;
        }

        ::std::string StructPatternFieldIdentPat::as_string() const {
            ::std::string str = StructPatternField::as_string();

            str += "\n";

            str += ident + " : " + ident_pattern->as_string();

            return str;
        }

        ::std::string StructPatternElements::as_string() const {
            ::std::string str("\n  Fields: ");

            if (!has_struct_pattern_fields()) {
                str += "none";
            } else {
                for (const auto& field : fields) {
                    str += "\n   " + field->as_string();
                }
            }

            str += "\n  Etc: ";
            if (has_struct_pattern_etc) {
                str += "true";
            } else {
                str += "false";
            }

            return str;
        }

        ::std::string StructPattern::as_string() const {
            ::std::string str("StructPattern: \n Path: ");

            str += path.as_string();

            str += "\n Struct pattern elems: ";
            if (!has_struct_pattern_elems()) {
                str += "none";
            } else {
                str += elems.as_string();
            }

            return str;
        }

        ::std::string LiteralPattern::as_string() const {
            ::std::string str;

            if (has_minus) {
                str += "-";
            }

            return str + lit.as_string();
        }

        ::std::string ReferencePattern::as_string() const {
            ::std::string str("&");

            if (has_two_amps) {
                str += "&";
            }

            if (is_mut) {
                str += "mut ";
            }

            str += pattern->as_string();

            return str;
        }

        ::std::string IdentifierPattern::as_string() const {
            ::std::string str;

            if (is_ref) {
                str += "ref ";
            }

            if (is_mut) {
                str += "mut ";
            }

            str += variable_ident;

            if (has_pattern_to_bind()) {
                str += " @ " + to_bind->as_string();
            }

            return str;
        }

        ::std::string TupleStructItemsNoRange::as_string() const {
            ::std::string str;

            for (const auto& pattern : patterns) {
                str += "\n  " + pattern->as_string();
            }

            return str;
        }

        ::std::string TupleStructItemsRange::as_string() const {
            ::std::string str("\n  Lower patterns: ");

            if (lower_patterns.empty()) {
                str += "none";
            } else {
                for (const auto& lower : lower_patterns) {
                    str += "\n   " + lower->as_string();
                }
            }

            str += "\n  Upper patterns: ";
            if (upper_patterns.empty()) {
                str += "none";
            } else {
                for (const auto& upper : upper_patterns) {
                    str += "\n   " + upper->as_string();
                }
            }

            return str;
        }

        ::std::string TupleStructPattern::as_string() const {
            ::std::string str("TupleStructPattern: \n Path: ");

            str += path.as_string();

            str += "\n Tuple struct items: " + items->as_string();

            return str;
        }

        ::std::string LetStmt::as_string() const {
            // outer attributes
            ::std::string str = "Outer attributes: ";
            if (outer_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "outer attribute" syntax - just the body
                for (const auto& attr : outer_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            str += "\nlet " + variables_pattern->as_string();

            if (has_type()) {
                str += " : " + type->as_string();
            }

            if (has_init_expr()) {
                str += " = " + init_expr->as_string();
            }

            return str;
        }

        // Used to get outer attributes for expressions.
        ::std::string Expr::as_string() const {
            // outer attributes
            ::std::string str = "outer attributes: ";
            if (outer_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "outer attribute" syntax - just the body
                for (const auto& attr : outer_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            return str;
        }

        // hopefully definition here will prevent circular dependency issue
        TraitBound* TypePath::to_trait_bound(bool in_parens) const {
            // create clone FIXME is this required? or is copy constructor automatically called?
            TypePath copy(*this);
            return new TraitBound(::std::move(copy), in_parens);
        }

        ::std::string InferredType::as_string() const {
            return "_ (inferred)";
        }

        ::std::string TypeCastExpr::as_string() const {
            return main_or_left_expr->as_string() + " as " + type_to_convert_to->as_string();
        }

        ::std::string ImplTraitType::as_string() const {
            ::std::string str("ImplTraitType: \n TypeParamBounds: ");

            if (type_param_bounds.empty()) {
                str += "none";
            } else {
                for (const auto& bound : type_param_bounds) {
                    str += "\n  " + bound->as_string();
                }
            }

            return str;
        }

        ::std::string ReferenceType::as_string() const {
            ::std::string str("&");

            if (has_lifetime()) {
                str += lifetime.as_string() + " ";
            }

            if (has_mut) {
                str += "mut ";
            }

            str += type->as_string();

            return str;
        }

        ::std::string RawPointerType::as_string() const {
            ::std::string str("*");

            switch (pointer_type) {
                case MUT:
                    str += "mut ";
                    break;
                case CONST:
                    str += "const ";
                    break;
                default:
                    return "ERROR_MARK_STRING - unknown pointer type in raw pointer type";
            }

            str += type->as_string();

            return str;
        }

        ::std::string TraitObjectType::as_string() const {
            ::std::string str("TraitObjectType: \n Has dyn dispatch: ");

            if (has_dyn) {
                str += "true";
            } else {
                str += "false";
            }

            str += "\n TypeParamBounds: ";
            if (type_param_bounds.empty()) {
                str += "none";
            } else {
                for (const auto& bound : type_param_bounds) {
                    str += "\n  " + bound->as_string();
                }
            }

            return str;
        }

        ::std::string BareFunctionType::as_string() const {
            ::std::string str("BareFunctionType: \n For lifetimes: ");

            if (!has_for_lifetimes()) {
                str += "none";
            } else {
                for (const auto& for_lifetime : for_lifetimes) {
                    str += "\n  " + for_lifetime.as_string();
                }
            }

            str += "\n Qualifiers: " + function_qualifiers.as_string();

            str += "\n Params: ";
            if (params.empty()) {
                str += "none";
            } else {
                for (const auto& param : params) {
                    str += "\n  " + param.as_string();
                }
            }

            str += "\n Is variadic: ";
            if (is_variadic) {
                str += "true";
            } else {
                str += "false";
            }

            str += "\n Return type: ";
            if (!has_return_type()) {
                str += "none (void)";
            } else {
                str += return_type->as_string();
            }

            return str;
        }

        ::std::string ImplTraitTypeOneBound::as_string() const {
            ::std::string str("ImplTraitTypeOneBound: \n TraitBound: ");

            return str + trait_bound.as_string();
        }

        ::std::string TypePathSegmentGeneric::as_string() const {
            return TypePathSegment::as_string() + "<" + generic_args.as_string() + ">";
        }

        ::std::string TraitObjectTypeOneBound::as_string() const {
            ::std::string str("TraitObjectTypeOneBound: \n Has dyn dispatch: ");

            if (has_dyn) {
                str += "true";
            } else {
                str += "false";
            }

            str += "\n TraitBound: " + trait_bound.as_string();

            return str;
        }

        ::std::string TypePathFunction::as_string() const {
            ::std::string str("(");

            if (has_inputs()) {
                for (const auto& param : inputs) {
                    str += param->as_string() + ", ";
                }
            }

            str += ")";

            if (has_return_type()) {
                str += " -> " + return_type->as_string();
            }

            return str;
        }

        ::std::string TypePathSegmentFunction::as_string() const {
            return TypePathSegment::as_string() + function_path.as_string();
        }

        ::std::string ArrayType::as_string() const {
            return "[" + elem_type->as_string() + "; " + size->as_string() + "]";
        }

        ::std::string SliceType::as_string() const {
            return "[" + elem_type->as_string() + "]";
        }

        ::std::string TupleType::as_string() const {
            ::std::string str("(");

            if (!is_unit_type()) {
                for (const auto& elem : elems) {
                    str += elem->as_string() + ", ";
                }
            }

            str += ")";

            return str;
        }

        ::std::string StructExpr::as_string() const {
            ::std::string str = ExprWithoutBlock::as_string();

            str += "\nStructExpr";

            str += "\n PathInExpr: " + struct_name.as_string();

            return str;
        }

        ::std::string StructExprTuple::as_string() const {
            ::std::string str = StructExpr::as_string();

            // inner attributes
            str += "\n inner attributes: ";
            if (inner_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "inner attribute" syntax - just the body
                for (const auto& attr : inner_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            str += "\n Tuple fields: ";
            if (exprs.empty()) {
                str += "none";
            } else {
                for (const auto& field : exprs) {
                    // debug - null pointer check
                    if (field == NULL) {
                        return "ERROR_MARK_STRING - nullptr struct expr tuple field";
                    }

                    str += "\n  " + field->as_string();
                }
            }

            return str;
        }

        ::std::string StructExprStruct::as_string() const {
            ::std::string str("StructExprStruct (or subclass): ");

            str += "\n Path: " + get_struct_name().as_string();

            // inner attributes
            str += "\n inner attributes: ";
            if (inner_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "inner attribute" syntax - just the body
                for (const auto& attr : inner_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            return str;
        }

        ::std::string StructBase::as_string() const {
            if (base_struct != NULL) {
                return base_struct->as_string();
            } else {
                return "ERROR_MARK_STRING - invalid struct base had as string applied";
            }
        }

        ::std::string StructExprFieldWithVal::as_string() const {
            // used to get value string
            return value->as_string();
        }

        ::std::string StructExprFieldIdentifierValue::as_string() const {
            return field_name + " : " + StructExprFieldWithVal::as_string();
        }

        ::std::string StructExprFieldIndexValue::as_string() const {
            return ::std::to_string(index) + " : " + StructExprFieldWithVal::as_string();
        }

        ::std::string StructExprStructFields::as_string() const {
            ::std::string str = StructExprStruct::as_string();

            str += "\n Fields: ";
            if (fields.empty()) {
                str += "none";
            } else {
                for (const auto& field : fields) {
                    str += "\n  " + field->as_string();
                }
            }

            str += "\n Struct base: ";
            if (!has_struct_base()) {
                str += "none";
            } else {
                str += struct_base.as_string();
            }

            return str;
        }

        ::std::string EnumItem::as_string() const {
            // outer attributes
            ::std::string str = "outer attributes: ";
            if (outer_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "outer attribute" syntax - just the body
                for (const auto& attr : outer_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            str += "\n" + variant_name;

            return str;
        }

        ::std::string EnumItemTuple::as_string() const {
            ::std::string str = EnumItem::as_string();

            // add tuple opening parens
            str += "(";

            // tuple fields
            if (has_tuple_fields()) {
                for (const auto& field : tuple_fields) {
                    str += field.as_string() + ", ";
                }
            }

            // add tuple closing parens
            str += ")";

            return str;
        }

        ::std::string TupleField::as_string() const {
            // outer attributes
            ::std::string str = "outer attributes: ";
            if (outer_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "outer attribute" syntax - just the body
                for (const auto& attr : outer_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            if (has_visibility()) {
                str += "\n" + visibility.as_string();
            }

            str += " " + field_type->as_string();

            return str;
        }

        ::std::string EnumItemStruct::as_string() const {
            ::std::string str = EnumItem::as_string();

            // add struct opening parens
            str += "{";

            // tuple fields
            if (has_struct_fields()) {
                for (const auto& field : struct_fields) {
                    str += field.as_string() + ", ";
                }
            }

            // add struct closing parens
            str += "}";

            return str;
        }

        ::std::string StructField::as_string() const {
            // outer attributes
            ::std::string str = "outer attributes: ";
            if (outer_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "outer attribute" syntax - just the body
                for (const auto& attr : outer_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            if (has_visibility()) {
                str += "\n" + visibility.as_string();
            }

            str += " " + field_name + " : " + field_type->as_string();

            return str;
        }

        ::std::string EnumItemDiscriminant::as_string() const {
            ::std::string str = EnumItem::as_string();

            // add equal and expression
            str += " = " + expression->as_string();

            return str;
        }

        ::std::string ExternalItem::as_string() const {
            // outer attributes
            ::std::string str = "outer attributes: ";
            if (outer_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "outer attribute" syntax - just the body
                for (const auto& attr : outer_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            // start visibility on new line and with a space
            str += "\n" + visibility.as_string() + " ";

            return str;
        }

        ::std::string ExternalStaticItem::as_string() const {
            ::std::string str = ExternalItem::as_string();

            str += "static ";

            if (has_mut) {
                str += "mut ";
            }

            // add name
            str += get_item_name();

            // add type on new line
            str += "\n Type: " + item_type->as_string();

            return str;
        }

        ::std::string ExternalFunctionItem::as_string() const {
            ::std::string str = ExternalItem::as_string();

            str += "fn ";

            // add name
            str += get_item_name();

            // generic params
            str += "\n Generic params: ";
            if (generic_params.empty()) {
                str += "none";
            } else {
                for (const auto& param : generic_params) {
                    // DEBUG: null pointer check
                    if (param == NULL) {
                        fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                        "generic param in external function item.");
                        return "NULL_POINTER_MARK";
                    }

                    str += "\n  " + param->as_string();
                }
            }

            // function params
            str += "\n Function params: ";
            if (function_params.empty()) {
                str += "none";
            } else {
                for (const auto& param : function_params) {
                    str += "\n  " + param.as_string();
                }
                if (has_variadics) {
                    str += "\n  .. (variadic)";
                }
            }

            // add type on new line
            str += "\n (return) Type: " + return_type->as_string();

            // where clause
            str += "\n Where clause: ";
            if (has_where_clause()) {
                str += where_clause.as_string();
            } else {
                str += "none";
            }

            return str;
        }

        ::std::string NamedFunctionParam::as_string() const {
            ::std::string str = name;

            str += "\n Type: " + param_type->as_string();

            return str;
        }

        /*::std::string TraitItem::as_string() const {
            // outer attributes
            ::std::string str = "outer attributes: ";
            if (outer_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "outer attribute" syntax - just the body
                for (const auto& attr : outer_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            return str;
        }*/

        ::std::string TraitItemFunc::as_string() const {
            ::std::string str = "outer attributes: ";
            if (outer_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "outer attribute" syntax - just the body
                for (const auto& attr : outer_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            str += "\n" + decl.as_string();

            str += "\n Definition (block expr): ";
            if (has_definition()) {
                str += block_expr->as_string();
            } else {
                str += "none";
            }

            return str;
        }

        ::std::string TraitFunctionDecl::as_string() const {
            ::std::string str = qualifiers.as_string() + "fn " + function_name;

            // generic params
            str += "\n Generic params: ";
            if (generic_params.empty()) {
                str += "none";
            } else {
                for (const auto& param : generic_params) {
                    // DEBUG: null pointer check
                    if (param == NULL) {
                        fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                        "generic param in trait function decl.");
                        return "NULL_POINTER_MARK";
                    }

                    str += "\n  " + param->as_string();
                }
            }

            str += "\n Function params: ";
            if (has_params()) {
                for (const auto& param : function_params) {
                    str += "\n  " + param.as_string();
                }
            } else {
                str += "none";
            }

            str += "\n Return type: ";
            if (has_return_type()) {
                str += return_type->as_string();
            } else {
                str += "none (void)";
            }

            str += "\n Where clause: ";
            if (has_where_clause()) {
                str += where_clause.as_string();
            } else {
                str += "none";
            }

            return str;
        }

        ::std::string TraitItemMethod::as_string() const {
            ::std::string str = "outer attributes: ";
            if (outer_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "outer attribute" syntax - just the body
                for (const auto& attr : outer_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            str += "\n" + decl.as_string();

            str += "\n Definition (block expr): ";
            if (has_definition()) {
                str += block_expr->as_string();
            } else {
                str += "none";
            }

            return str;
        }

        ::std::string TraitMethodDecl::as_string() const {
            ::std::string str = qualifiers.as_string() + "fn " + function_name;

            // generic params
            str += "\n Generic params: ";
            if (generic_params.empty()) {
                str += "none";
            } else {
                for (const auto& param : generic_params) {
                    // DEBUG: null pointer check
                    if (param == NULL) {
                        fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                        "generic param in trait function decl.");
                        return "NULL_POINTER_MARK";
                    }

                    str += "\n  " + param->as_string();
                }
            }

            str += "\n Self param: " + self_param.as_string();

            str += "\n Function params: ";
            if (has_params()) {
                for (const auto& param : function_params) {
                    str += "\n  " + param.as_string();
                }
            } else {
                str += "none";
            }

            str += "\n Return type: ";
            if (has_return_type()) {
                str += return_type->as_string();
            } else {
                str += "none (void)";
            }

            str += "\n Where clause: ";
            if (has_where_clause()) {
                str += where_clause.as_string();
            } else {
                str += "none";
            }

            return str;
        }

        ::std::string TraitItemConst::as_string() const {
            ::std::string str = "outer attributes: ";
            if (outer_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "outer attribute" syntax - just the body
                for (const auto& attr : outer_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            str += "\nconst " + name + " : " + type->as_string();

            if (has_expression()) {
                str += " = " + expr->as_string();
            }

            return str;
        }

        ::std::string TraitItemType::as_string() const {
            ::std::string str = "outer attributes: ";
            if (outer_attrs.empty()) {
                str += "none";
            } else {
                // note that this does not print them with "outer attribute" syntax - just the body
                for (const auto& attr : outer_attrs) {
                    str += "\n  " + attr.as_string();
                }
            }

            str += "\ntype " + name;

            str += "\n Type param bounds: ";
            if (!has_type_param_bounds()) {
                str += "none";
            } else {
                for (const auto& bound : type_param_bounds) {
                    // DEBUG: null pointer check
                    if (bound == NULL) {
                        fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                        "type param bound in trait item type.");
                        return "NULL_POINTER_MARK";
                    }

                    str += "\n  " + bound->as_string();
                }
            }

            return str;
        }

        ::std::string SelfParam::as_string() const {
            if (is_error()) {
                return "error";
            } else {
                if (has_type()) {
                    // type (i.e. not ref, no lifetime)
                    ::std::string str;

                    if (is_mut) {
                        str += "mut ";
                    }

                    str += "self : ";

                    str += type->as_string();

                    return str;
                } else if (has_lifetime()) {
                    // ref and lifetime
                    ::std::string str = "&" + lifetime.as_string() + " ";

                    if (is_mut) {
                        str += "mut ";
                    }

                    str += "self";

                    return str;
                } else if (has_ref) {
                    // ref with no lifetime
                    ::std::string str = "&";

                    if (is_mut) {
                        str += " mut ";
                    }

                    str += "self";

                    return str;
                } else {
                    // no ref, no type
                    ::std::string str;

                    if (is_mut) {
                        str += "mut ";
                    }

                    str += "self";

                    return str;
                }
            }
        }

        ::std::string ArrayElemsCopied::as_string() const {
            return elem_to_copy->as_string() + "; " + num_copies->as_string();
        }

        ::std::string LifetimeWhereClauseItem::as_string() const {
            ::std::string str("Lifetime: ");

            str += lifetime.as_string();

            str += "\nLifetime bounds: ";

            for (const auto& bound : lifetime_bounds) {
                str += "\n " + bound.as_string();
            }

            return str;
        }

        ::std::string TypeBoundWhereClauseItem::as_string() const {
            ::std::string str("For lifetimes: ");

            if (!has_for_lifetimes()) {
                str += "none";
            } else {
                for (const auto& for_lifetime : for_lifetimes) {
                    str += "\n " + for_lifetime.as_string();
                }
            }

            str += "\nType: " + bound_type->as_string();

            str += "\nType param bounds bounds: ";

            for (const auto& bound : type_param_bounds) {
                // debug null pointer check
                if (bound == NULL) {
                    return "NULL_POINTER_MARK - type param bounds";
                }

                str += "\n " + bound->as_string();
            }

            return str;
        }

        ::std::string ArrayElemsValues::as_string() const {
            ::std::string str;

            for (const auto& expr : values) {
                // DEBUG: null pointer check
                if (expr == NULL) {
                    fprintf(stderr, "something really terrible has gone wrong - null pointer "
                                    "expr in array elems values.");
                    return "NULL_POINTER_MARK";
                }

                str += "\n  " + expr->as_string();
            }

            return str;
        }

        ::std::string MaybeNamedParam::as_string() const {
            ::std::string str;

            switch (param_kind) {
                case UNNAMED:
                    break;
                case IDENTIFIER:
                    str = name + " : ";
                    break;
                case WILDCARD:
                    str = "_ : ";
                    break;
                default:
                    return "ERROR_MARK_STRING - maybe named param unrecognised param kind";
            }

            str += param_type->as_string();

            return str;
        }
    }
}