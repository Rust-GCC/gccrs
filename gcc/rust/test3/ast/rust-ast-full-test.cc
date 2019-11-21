#include "rust-ast-full.h"
// Dummy implementations of as_string() for now - will complete later for debugging purposes

namespace Rust {
    namespace AST {
        ::std::string Crate::as_string() const {
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
                    str += tree->as_string() + ", ";
                }
            }
            str += end_delim;

            return str;
        }

        ::std::string Token::as_string() const {
            /* FIXME: only works when not identifier or literal or whatever, i.e. when doesn't store
             * string value */
            //return get_token_description(token_id);

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
                    fprintf(stderr, "segment in path is error - this should've been filtered out. first segment was '%s' \n", segments.at(0).as_string().c_str());
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

        ::std::string ModuleBodied::as_string() const {
            ::std::string vis_item = VisItem::as_string();

            return ::std::string("not implemented");
        }

        ::std::string ModuleNoBody::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string StaticItem::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string ExternCrate::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string TupleStruct::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string ConstantItem::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string InherentImpl::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string StructStruct::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string UseDeclaration::as_string() const {
            ::std::string str = VisItem::as_string();

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
            return ::std::string("not implemented");
        }

        ::std::string Trait::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string Union::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string Function::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string BlockExpr::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string TraitImpl::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string TypeAlias::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string MacroInvocationSemi::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string ExternBlock::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string MacroRulesDefinition::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string MacroInvocation::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string PathInExpression::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string ExprStmtWithBlock::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string ClosureExprInnerTyped::as_string() const {
            return ::std::string("not implemented");
        }

        ::std::string QualifiedPathInExpression::as_string() const {
            return ::std::string("not implemented");
        }
    }
}