#ifndef RUST_AST_ITEM_H
#define RUST_AST_ITEM_H

#include "rust-ast.h"

namespace Rust {
    namespace AST {
        // TODO: remove
        typedef int Type;
        // typedef ::std::vector<int> Generics;
        // typedef ::std::string WhereClause;
        typedef ::std::vector<int> TypeParamBounds;
        typedef ::std::string Lifetime;
        typedef ::std::string LifetimeBounds;
        typedef ::std::string LifetimeParams;
        typedef Type FunctionReturnType;
        typedef Identifier AbiName;
        class BlockExpr;
        class Expr;
        typedef Type TypePath;

        // Base generic parameter in AST. Abstract - can be represented by a Lifetime or Type param
        class GenericParam {};

        class LifetimeParam : public GenericParam {
            bool has_outer_attribute;
            Attribute outer_attr;

            Lifetime lifetime;

            bool has_lifetime_bounds;
            LifetimeBounds lifetime_bounds;
        };

        class TypeParam : public GenericParam {
            bool has_outer_attribute;
            Attribute outer_attr;

            Identifier type_representation;

            bool has_type_param_bounds;
            TypeParamBounds type_param_bounds;

            bool has_type;
            Type type;
        };

        struct Generics {
            // inline: change all occurences of "Generics" to this single param
            ::std::vector<GenericParam> generic_params;
        };

        // "where" clause item base. Abstract - use LifetimeWhereClauseItem, TypeBoundWhereClauseItem
        class WhereClauseItem {};

        class LifetimeWhereClauseItem : public WhereClauseItem {
            Lifetime lifetime;

            LifetimeBounds lifetime_bounds;
        };

        class TypeBoundWhereClauseItem : public WhereClauseItem {
            bool has_for_lifetimes;
            LifetimeParams for_lifetimes;

            Type bound_type;

            bool has_type_param_bounds;
            TypeParamBounds type_param_bounds;
        };

        struct WhereClause {
            ::std::vector<WhereClauseItem> where_clause_items;
        };

        struct SelfParam {
            bool has_ref;
            bool has_lifetime; // only possible if also ref
            Lifetime lifetime;

            bool is_mut;

            bool has_type; // only possible if not ref
            Type type;
        };

        struct Method {
            FunctionQualifiers qualifiers;
            Identifier method_name;

            bool has_generics;
            Generics generic_params;

            SelfParam self_param;

            bool has_params;
            FunctionParams function_params;

            bool has_return_type;
            FunctionReturnType return_type;

            bool has_where_clause;
            WhereClause where_clause;

            BlockExpr* expr;

            ~Method() {
                delete expr;
            }
        };

        // Rust "item" node (declaration of top-level/module-level allowed stuff)
        class Item : public Node {
            ::std::vector<Attribute> outer_attrs; // maybe only outer attributes
        };

        // Visibility of item - private, public, module-only, etc. ...
        struct Visibility {
            bool is_pub;

            // if vis is public, one of these
            enum PublicVisType { NONE, CRATE, SELF, SUPER, IN_PATH } public_vis_type;

            // Only assigned if public_vis_type is IN_PATH
            SimplePath in_path;
        };

        // Item that supports visibility - abstract base class
        class VisItem : public Item {
            Visibility visibility;
        };

        // Rust module item - abstract base class
        class Module : public VisItem {};

        // Module with a body, defined in file
        class ModuleBodied : public Module {
            bool has_inner_attrs;
            ::std::vector<Attribute> inner_attrs;
            bool has_items;
            ::std::vector<Item> items;

          public:
            ::std::string as_string() const;
        };

        // Module without a body, loaded from external file
        class ModuleNoBody : public Module {
          public:
            ::std::string as_string() const;
        };

        struct CrateRef {
            // either an identifier or "self"
        };

        struct AsClause {
            // either an identifier or "_"
        };

        // Rust extern crate decl
        class ExternCrate : public VisItem {
            CrateRef referenced_crate;
            bool has_as_clause;
            AsClause as_clause;

            /* e.g.
                "extern crate foo as _"
                "extern crate foo"
                "extern crate std as cool_std"  */
          public:
            ::std::string as_string() const;
        };

        // Rust use decl (module)
        class UseDeclaration : public VisItem {
            // SimplePath path;
            bool has_asterisk; // glob
            // List of paths inside braces (with a common prefix) - vector?
            // identifier in as OR _ in as

          public:
            ::std::string as_string() const;
        };

        // Qualifiers for function, i.e. const, unsafe, etc.
        struct FunctionQualifiers {
            bool has_const;
            bool has_unsafe;
            bool has_extern;
            ::std::string extern_abi; // e.g. extern "C" fn() -> i32 {}
            // TODO: maybe ensure that extern_abi only exists if extern exists?
        };

        struct FunctionParam {
            Pattern* param_name;
            Type type;
        };

        struct FunctionParams {
            ::std::vector<FunctionParam> params;
        };

        // Rust function decl
        class Function : public VisItem {
            FunctionQualifiers qualifiers;

            Identifier function_name;

            bool has_generics;
            Generics generic_params;

            bool has_function_params;
            FunctionParams function_params;

            bool has_function_return_type;
            Type return_type;

            bool has_where_clause;
            WhereClause where_clause;

            BlockExpr* function_body;

          public:
            ~Function() {
                delete function_body;
            }
            ::std::string as_string() const;
        };

        // Rust type alias
        class TypeAlias : public VisItem {
            Identifier new_type_name;

            bool has_generics;
            Generics generic_params;

            bool has_where_clause;
            WhereClause where_clause;

            Type exiting_type;

          public:
            ::std::string as_string() const;
        };

        // Rust base struct decl - maybe abstract?
        class Struct : public VisItem {
            Identifier struct_name;

            bool has_generics;
            Generics generic_params;

            bool has_where_clause;
            WhereClause where_clause;
        };

        // A single field in a struct
        struct StructField {
            bool has_outer_attributes;
            ::std::vector<Attribute> outer_attribs;

            bool has_visibility;
            Visibility visibility;

            Identifier field_name;
            Type field_type;
        };

        // Rust struct with true struct type
        class StructStruct : public Struct {
            ::std::vector<StructField> fields;

          public:
            ::std::string as_string() const;
        };

        struct TupleField {
            bool has_outer_attributes;
            ::std::vector<Attribute> outer_attribs;

            bool has_visibility;
            Visibility visibility;

            Type field_type;
        };

        // Rust tuple declared using struct keyword
        class TupleStruct : public Struct {
            ::std::vector<TupleField> fields;

          public:
            ::std::string as_string() const;
        };

        class EnumItem {
            bool has_attrs;
            ::std::vector<Attribute> outer_attrs;

            Identifier variant_name;

          public:
            virtual ~EnumItem() {}
        };

        class EnumItemTuple : public EnumItem {
            bool has_tuple_fields;
            ::std::vector<TupleField> tuple_fields;
        };

        class EnumItemStruct : public EnumItem {
            bool has_struct_fields;
            ::std::vector<TupleField> struct_fields;
        };

        class EnumItemDiscriminant : public EnumItem {
            Expr* expression;

          public:
            ~EnumItemDiscriminant() {
                delete expression;
            }
        };

        // Rust "enum" - tagged union
        class Enum : public VisItem {
            Identifier enum_name;

            bool has_generics;
            Generics generics_params;

            bool has_where_clause;
            WhereClause where_clause;

            ::std::vector<EnumItem> items;

          public:
            ::std::string as_string() const;
        };

        // Untagged union used for C compat
        class Union : public VisItem {
            Identifier union_name;

            bool has_generics;
            Generics generics_params;

            bool has_where_clause;
            WhereClause where_clause;

            ::std::vector<StructField> variants;

          public:
            ::std::string as_string() const;
        };

        class ConstantItem : public VisItem {
            // either has an identifier or "_" - maybe handle in identifier?
            bool identifier_is_underscore;
            Identifier identifier;

            Type type;

            Expr* const_expr;

          public:
            ~ConstantItem() {
                delete const_expr;
            }

            ::std::string as_string() const;
        };

        class StaticItem : public VisItem {
            bool has_mut;

            Identifier name;

            Type type;

            Expr* expr;

          public:
            ~StaticItem() {
                delete expr;
            }

            ::std::string as_string() const;
        };

        class TraitItem {
            bool has_outer_attrs;
            ::std::vector<Attribute> outer_attrs;

          public:
            virtual ~TraitItem();
        };

        struct TraitFunctionDecl {
            // TODO: delete and replace with Function decl item?
            FunctionQualifiers qualifiers;
            Identifier function_name;

            bool has_generics;
            Generics generic_params;

            bool has_params;
            FunctionParams function_params;

            bool has_return_type;
            Type return_type;

            bool has_where_clause;
            WhereClause where_clause;
        };

        class TraitItemFunc : public TraitItem {
            TraitFunctionDecl decl;
            BlockExpr* block_expr;

          public:
            ~TraitItemFunc() {
                delete block_expr;
            }
        };

        struct TraitMethodDecl {
            // TODO: delete and replace with Function decl item?
            FunctionQualifiers qualifiers;
            Identifier function_name;

            bool has_generics;
            Generics generic_params;

            SelfParam self_param;

            bool has_params;
            FunctionParams function_params;

            bool has_return_type;
            Type return_type;

            bool has_where_clause;
            WhereClause where_clause;
        };

        class TraitItemMethod : public TraitItem {
            TraitMethodDecl decl;
            BlockExpr* block_expr;

          public:
            ~TraitItemMethod() {
                delete block_expr;
            }
        };

        class TraitItemConst : public TraitItem {
            Identifier name;
            Type type;

            bool has_expression;
            Expr* expr;

          public:
            ~TraitItemConst() {
                delete expr;
            }
        };

        class TraitItemType : public TraitItem {
            Identifier name;

            bool has_type_param_bounds;
            TypeParamBounds param_bounds;
        };

        class TraitItemMacroInvoc : public TraitItem {
            MacroInvocationSemi macro_invoc;
        };

        class Trait : public VisItem {
            bool has_unsafe;

            Identifier name;

            bool has_generics;
            Generics generic_params;

            bool has_type_param_bounds;
            TypeParamBounds type_param_bounds;

            bool has_where_clause;
            WhereClause where_clause;

            bool has_trait_items;
            ::std::vector<TraitItem> trait_items;

          public:
            ::std::string as_string() const;
        };

        class Impl : public VisItem {
            bool has_generics;
            Generics generic_params;

            Type trait_type;

            bool has_where_clause;
            WhereClause where_clause;

            bool has_inner_attrs;
            ::std::vector<Attribute> inner_attrs;

          public:
            ::std::string as_string() const;
        };

        class InherentImplItem {
            bool has_outer_attrs;
            ::std::vector<Attribute> outer_attrs;
        };

        class InherentImplItemMacro : public InherentImplItem {
            MacroInvocationSemi macro;
        };

        class InherentImplItemConstant : public InherentImplItem {
            bool has_visibility;
            Visibility visibility;

            ConstantItem constant_item;
        };

        class InherentImplItemFunction : public InherentImplItem {
            bool has_visibility;
            Visibility visibility;

            Function function;
        };

        class InherentImplItemMethod : public InherentImplItem {
            bool has_visibility;
            Visibility visibility;

            Method method;
        };

        // Regular "impl foo" impl block declaration
        class InherentImpl : public Impl {
            bool has_impl_items;
            ::std::vector<InherentImplItem> impl_items;
        };

        class TraitImplItem {
            bool has_outer_attrs;
            ::std::vector<Attribute> outer_attrs;
        };

        class TraitImplItemMacro : public TraitImplItem {
            MacroInvocationSemi macro;
        };

        class TraitImplItemTypeAlias : public TraitImplItem {
            bool has_visibility;
            Visibility visibility;

            TypeAlias type_alias;
        };

        class TraitImplItemConstant : public TraitImplItem {
            bool has_visibility;
            Visibility visibility;

            ConstantItem constant_item;
        };

        class TraitImplItemFunction : public TraitImplItem {
            bool has_visibility;
            Visibility visibility;

            Function function;
        };

        class TraitImplItemMethod : public TraitImplItem {
            bool has_visibility;
            Visibility visibility;

            Method method;
        };

        // The "impl footrait for foo" impl block declaration
        class TraitImpl : public Impl {
            bool has_unsafe;

            bool has_exclam;

            TypePath trait_path;

            bool has_impl_items;
            ::std::vector<TraitImplItem> impl_items;

          public:
            ::std::string as_string() const;
        };

        class ExternalItem {
            bool has_outer_attrs;
            ::std::vector<Attribute> outer_attrs;

            bool has_visibility;
            Visibility visibility;

            Identifier item_name;
        };

        class ExternalStaticItem : public ExternalItem {
            bool has_mut;

            Type item_type;
        };

        class ExternalFunctionItem : public ExternalItem {
            bool has_generics;
            Generics generic_params;

            bool has_return_type;
            FunctionReturnType return_type;

            bool has_where_clause;
            WhereClause where_clause;
        };

        struct NamedFunctionParam {
            bool has_name; // otherwise is _
            Identifier name;

            Type param_type;
        };

        class ExternalFunctionItemNoParams : public ExternalFunctionItem {};

        class ExternalFunctionItemNoVariadics : public ExternalFunctionItem {
            ::std::vector<NamedFunctionParam> function_params;
        };

        class ExternalFunctionItemVariadics : public ExternalFunctionItem {
            ::std::vector<NamedFunctionParam> function_params;
            // just merge into one with no variadics and have a "has variadics" boolean?
            // variadics are just ... in syntax - nothing else is different
        };

        class ExternBlock : public VisItem {
            bool has_abi;
            AbiName abi;

            bool has_inner_attrs;
            ::std::vector<Attribute> inner_attrs;

            bool has_extern_items;
            ::std::vector<ExternalItem> extern_items;

          public:
            ::std::string as_string() const;
        };

        class MacroItem : public Item {
            /*public:
            ::std::string as_string() const;*/
        };

        class MacroInvocationSemi : public MacroItem {
          public:
            ::std::string as_string() const;
        };

        class MacroRulesDefinition : public MacroItem {
          public:
            ::std::string as_string() const;
        };
    }
}

#endif