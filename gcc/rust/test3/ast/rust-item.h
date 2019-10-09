#ifndef RUST_AST_ITEM_H
#define RUST_AST_ITEM_H

#include "rust-ast.h"

namespace Rust {
    namespace AST {
        // forward decls
        struct Lifetime;
        struct LifetimeBounds;
        struct TypeParamBounds;
        class BlockExpr;
        class Expr;
        class Type;

        // TODO: remove
        //typedef int Type;
        // typedef ::std::vector<int> Generics;
        // typedef ::std::string WhereClause;
        //typedef ::std::vector<int> TypeParamBounds;
        //typedef ::std::string Lifetime;
        //typedef ::std::string LifetimeBounds;
        //typedef ::std::string LifetimeParams;
        //typedef Type FunctionReturnType;
        //typedef Identifier AbiName;
        //typedef Type TypePath;

        // TODO: inline
        struct FunctionReturnType {
            Type return_type;
        };

        // TODO: inline
        struct AbiName {
            ::std::string abi_name;
            // Technically is meant to be STRING_LITERAL or RAW_STRING_LITERAL
        };

        // Base generic parameter in AST. Abstract - can be represented by a Lifetime or Type param
        class GenericParam {};

        // A lifetime generic parameter (as opposed to a type generic parameter)
        class LifetimeParam : public GenericParam {
            bool has_outer_attribute;
            Attribute outer_attr;

            Lifetime lifetime;

            bool has_lifetime_bounds;
            LifetimeBounds lifetime_bounds;
        };

        // TODO: remove and inline
        typedef ::std::vector<LifetimeParam> LifetimeParams;
        typedef LifetimeParams ForLifetimes;

        // A type generic parameter (as opposed to a lifetime generic parameter)
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

        // A lifetime where clause item
        class LifetimeWhereClauseItem : public WhereClauseItem {
            Lifetime lifetime;

            LifetimeBounds lifetime_bounds;
        };

        // A type bound where clause item
        class TypeBoundWhereClauseItem : public WhereClauseItem {
            bool has_for_lifetimes;
            //LifetimeParams for_lifetimes;
            ::std::vector<LifetimeParam> for_lifetimes; // inlined

            Type bound_type;

            bool has_type_param_bounds;
            TypeParamBounds type_param_bounds;
        };

        // A where clause
        struct WhereClause {
            ::std::vector<WhereClauseItem> where_clause_items;
        };

        // A self parameter in a method 
        struct SelfParam {
            bool has_ref;
            bool has_lifetime; // only possible if also ref
            Lifetime lifetime;

            bool is_mut;

            bool has_type; // only possible if not ref
            Type type;
        };

        // A method (function belonging to a type)
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

            public:
            ~Method() {
                delete expr;
            }
        };

        // Rust "item" AST node (declaration of top-level/module-level allowed stuff)
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

        // Rust extern crate declaration AST node
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

        // Rust use declaration (i.e. for modules) AST node
        class UseDeclaration : public VisItem {
            // SimplePath path;
            bool has_asterisk; // glob
            // List of paths inside braces (with a common prefix) - vector?
            // identifier in as OR _ in as

          public:
            ::std::string as_string() const;
        };

        // Qualifiers for function, i.e. const, unsafe, extern etc.
        struct FunctionQualifiers {
            bool has_const;
            bool has_unsafe;
            bool has_extern;
            ::std::string extern_abi; // e.g. extern "C" fn() -> i32 {}
            // TODO: maybe ensure that extern_abi only exists if extern exists?
        };

        // A function parameter
        struct FunctionParam {
            Pattern* param_name;
            Type type;
        };

        // Parameters used in a function - TODO inline?
        struct FunctionParams {
            ::std::vector<FunctionParam> params;
        };

        // Rust function declaration AST node
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

        // Rust type alias (i.e. typedef) AST node
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

        // Rust base struct declaration AST node - maybe abstract?
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

        // Rust struct declaration with true struct type AST node
        class StructStruct : public Struct {
            ::std::vector<StructField> fields;

          public:
            ::std::string as_string() const;
        };

        // A single field in a tuple
        struct TupleField {
            bool has_outer_attributes;
            ::std::vector<Attribute> outer_attribs;

            bool has_visibility;
            Visibility visibility;

            Type field_type;
        };

        // Rust tuple declared using struct keyword AST node
        class TupleStruct : public Struct {
            ::std::vector<TupleField> fields;

          public:
            ::std::string as_string() const;
        };

        // An item used in an "enum" tagged union
        class EnumItem {
            bool has_attrs;
            ::std::vector<Attribute> outer_attrs;

            Identifier variant_name;

          public:
            virtual ~EnumItem() {}
        };

        // A tuple item used in an "enum" tagged union
        class EnumItemTuple : public EnumItem {
            bool has_tuple_fields;
            ::std::vector<TupleField> tuple_fields;
        };

        // A struct item used in an "enum" tagged union
        class EnumItemStruct : public EnumItem {
            bool has_struct_fields;
            ::std::vector<TupleField> struct_fields;
        };

        // A discriminant item sued in an "enum" tagged union
        class EnumItemDiscriminant : public EnumItem {
            Expr* expression;

          public:
            ~EnumItemDiscriminant() {
                delete expression;
            }
        };

        // AST node for Rust "enum" - tagged union 
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

        // Rust untagged union used for C compat AST node
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

        // "Constant item" AST node - used for constant, compile-time expressions within module scope
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

        // Static item AST node - items within module scope with fixed storage duration?
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

        // Item used in trait declarations - abstract base class
        class TraitItem {
            bool has_outer_attrs;
            ::std::vector<Attribute> outer_attrs;

          public:
            virtual ~TraitItem();
        };

        // Function declaration in traits
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

        // Actual trait item function declaration within traits
        class TraitItemFunc : public TraitItem {
            TraitFunctionDecl decl;
            BlockExpr* block_expr;

          public:
            ~TraitItemFunc() {
                delete block_expr;
            }
        };

        // Method declaration within traits
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

        // Actual trait item method declaration within traits
        class TraitItemMethod : public TraitItem {
            TraitMethodDecl decl;
            BlockExpr* block_expr;

          public:
            ~TraitItemMethod() {
                delete block_expr;
            }
        };

        // Constant item within traits
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

        // Type items within traits
        class TraitItemType : public TraitItem {
            Identifier name;

            bool has_type_param_bounds;
            TypeParamBounds param_bounds;
        };

        // Macro invocation items within traits
        class TraitItemMacroInvoc : public TraitItem {
            MacroInvocationSemi macro_invoc;
        };

        // Rust trait item declaration AST node
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

        // Implementation item declaration AST node - abstract base class
        class Impl : public VisItem {
            bool has_generics;
            Generics generic_params;

            Type trait_type;

            bool has_where_clause;
            WhereClause where_clause;

            bool has_inner_attrs;
            ::std::vector<Attribute> inner_attrs;
        };

        // Abstract base class for items used within an inherent impl block (the impl name {} one)
        class InherentImplItem {
            bool has_outer_attrs;
            ::std::vector<Attribute> outer_attrs;
        };

        // Macro item used within an inherent impl block
        class InherentImplItemMacro : public InherentImplItem {
            MacroInvocationSemi macro;
        };

        // Constant item used within an inherent impl block
        class InherentImplItemConstant : public InherentImplItem {
            bool has_visibility;
            Visibility visibility;

            ConstantItem constant_item;
        };

        // Function item used within an inherent impl block
        class InherentImplItemFunction : public InherentImplItem {
            bool has_visibility;
            Visibility visibility;

            Function function;
        };

        // Method item used within an inherent impl block
        class InherentImplItemMethod : public InherentImplItem {
            bool has_visibility;
            Visibility visibility;

            Method method;
        };

        // Regular "impl foo" impl block declaration AST node
        class InherentImpl : public Impl {
            bool has_impl_items;
            ::std::vector<InherentImplItem> impl_items;

          public:
            ::std::string as_string() const;
        };

        // Abstract base class for items used in a trait impl
        class TraitImplItem {
            bool has_outer_attrs;
            ::std::vector<Attribute> outer_attrs;
        };

        // Macro invocation item in a trait impl
        class TraitImplItemMacro : public TraitImplItem {
            MacroInvocationSemi macro;
        };

        // Type alias item in a trait impl
        class TraitImplItemTypeAlias : public TraitImplItem {
            bool has_visibility;
            Visibility visibility;

            TypeAlias type_alias;
        };

        // Constant item in a trait impl
        class TraitImplItemConstant : public TraitImplItem {
            bool has_visibility;
            Visibility visibility;

            ConstantItem constant_item;
        };

        /// Function item in a trait impl
        class TraitImplItemFunction : public TraitImplItem {
            bool has_visibility;
            Visibility visibility;

            Function function;
        };

        // Method item in a trait impl
        class TraitImplItemMethod : public TraitImplItem {
            bool has_visibility;
            Visibility visibility;

            Method method;
        };

        // The "impl footrait for foo" impl block declaration AST node
        class TraitImpl : public Impl {
            bool has_unsafe;

            bool has_exclam;

            TypePath trait_path;

            bool has_impl_items;
            ::std::vector<TraitImplItem> impl_items;

          public:
            ::std::string as_string() const;
        };

        // Abstract base class for an item used inside an extern block 
        class ExternalItem {
            bool has_outer_attrs;
            ::std::vector<Attribute> outer_attrs;

            bool has_visibility;
            Visibility visibility;

            Identifier item_name;
        };

        // A static item used in an extern block
        class ExternalStaticItem : public ExternalItem {
            bool has_mut;

            Type item_type;
        };

        // A function item used in an extern block - abstract base class
        class ExternalFunctionItem : public ExternalItem {
            bool has_generics;
            Generics generic_params;

            bool has_return_type;
            FunctionReturnType return_type;

            bool has_where_clause;
            WhereClause where_clause;
        };

        // A named function parameter used in external functions
        struct NamedFunctionParam {
            bool has_name; // otherwise is _
            Identifier name;

            Type param_type;
        };

        // An external function inside an extern block with no parameters
        class ExternalFunctionItemNoParams : public ExternalFunctionItem {};

        // An external function inside an extern block without variadic parameters
        class ExternalFunctionItemNoVariadics : public ExternalFunctionItem {
            ::std::vector<NamedFunctionParam> function_params;
        };

        // An external function inside an extern block with variadic parameters
        class ExternalFunctionItemVariadics : public ExternalFunctionItem {
            ::std::vector<NamedFunctionParam> function_params;
            // just merge into one with no variadics and have a "has variadics" boolean?
            // variadics are just ... in syntax - nothing else is different
        };

        // An extern block AST node
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

        // Replaced with forward decls - defined in "rust-macro.h"
        class MacroItem;
        class MacroInvocationSemi;
        class MacroRulesDefinition;

        /*// A macro item AST node - potentially abstract base class
        class MacroItem : public Item {
        };

        // A macro invocation item (or statement) AST node
        class MacroInvocationSemi : public MacroItem, public Statement {
            SimplePath path;
            enum DelimType {
                PARENS,
                SQUARE,
                CURLY   // all delim types except curly must have invocation end with a semicolon
            } delim_type;
            ::std::vector<TokenTree> token_trees;

          public:
            ::std::string as_string() const;
        };

        // A macro rules definition item AST node
        class MacroRulesDefinition : public MacroItem {
          public:
            ::std::string as_string() const;
        };*/
    }
}

#endif