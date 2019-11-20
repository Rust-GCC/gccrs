#ifndef RUST_PARSE_H
#define RUST_PARSE_H

#include "rust-lex.h"
//#include "rust-tree.h"
#include "rust-scope.h"

//#include "rust-ast-containers.h"
// TODO: change, maybe?
#include "rust-ast-full.h"

namespace Rust {
    // Parser implementation for gccrs.
    class Parser {
      private:
        void skip_after_semicolon();
        void skip_after_end();
        void skip_after_end_block();
        void skip_after_end_attribute();

        bool skip_token(TokenId t);
        const_TokenPtr expect_token(TokenId t);
        void unexpected_token(const_TokenPtr t);

        // expression parsing
        int left_binding_power(const_TokenPtr tok);
        Tree null_denotation(const_TokenPtr tok);
        Tree left_denotation(const_TokenPtr tok, Tree left);

        Tree parse_expression(int right_binding_power);

        Tree coerce_binary_arithmetic(const_TokenPtr tok, Tree* left, Tree* right);
        bool check_logical_operands(const_TokenPtr tok, Tree left, Tree right);

        Tree get_printf_addr();
        Tree get_puts_addr();

        Tree get_scanf_addr();

        Tree build_label_decl(const char* name, location_t loc);
        Tree build_if_statement(Tree bool_expr, Tree then_part, Tree else_part);
        Tree build_while_statement(Tree bool_expr, Tree while_body);
        Tree build_for_statement(
          SymbolPtr ind_var, Tree lower_bound, Tree upper_bound, Tree for_body_stmt_list);

        const char* print_type(Tree type);

        TreeStmtList& get_current_stmt_list();

        void enter_scope();

        struct TreeSymbolMapping {
            Tree bind_expr;
            Tree block;
        };

        TreeSymbolMapping leave_scope();

        SymbolPtr query_type(const std::string& name, location_t loc);
        SymbolPtr query_variable(const std::string& name, location_t loc);
        SymbolPtr query_integer_variable(const std::string& name, location_t loc);

        void parse_statement_seq(bool (Parser::*done)());

        // AST-related stuff - maybe move or something?
        AST::Crate parse_crate();
        ::std::vector<AST::Attribute> parse_inner_attributes();
        AST::Attribute parse_inner_attribute();
        ::std::vector<AST::Attribute> parse_outer_attributes();
        AST::Attribute parse_outer_attribute();
        AST::Attribute parse_attribute_body();
        AST::AttrInput* parse_attr_input();

        // Path-related
        AST::SimplePath parse_simple_path();
        AST::SimplePathSegment parse_simple_path_segment();
        AST::TypePath parse_type_path();
        AST::TypePathSegment* parse_type_path_segment();
        AST::PathIdentSegment parse_path_ident_segment();
        AST::GenericArgs parse_path_generic_args();
        AST::GenericArgsBinding parse_generic_args_binding();
        AST::TypePathFunction parse_type_path_function();
        AST::PathInExpression parse_path_in_expression();
        AST::PathExprSegment parse_path_expr_segment();
        AST::QualifiedPathInExpression parse_qualified_path_in_expression();
        AST::QualifiedPathType parse_qualified_path_type();
        AST::QualifiedPathInType parse_qualified_path_in_type();

        // Token tree or macro related
        AST::DelimTokenTree parse_delim_token_tree();
        AST::TokenTree* parse_token_tree();
        AST::MacroRulesDefinition* parse_macro_rules_def(::std::vector<AST::Attribute> outer_attrs);
        AST::MacroInvocationSemi* parse_macro_invocation_semi(
          ::std::vector<AST::Attribute> outer_attrs);
        AST::MacroInvocation* parse_macro_invocation(::std::vector<AST::Attribute> outer_attrs);
        AST::MacroRule parse_macro_rule();
        AST::MacroMatcher parse_macro_matcher();
        AST::MacroMatch* parse_macro_match();
        AST::MacroMatchFragment* parse_macro_match_fragment();
        AST::MacroMatchRepetition* parse_macro_match_repetition();

        // Top-level item-related
        ::std::vector< ::std::unique_ptr<AST::Item> > parse_items();
        AST::Item* parse_item(bool called_from_statement);
        AST::VisItem* parse_vis_item(::std::vector<AST::Attribute> outer_attrs);
        AST::MacroItem* parse_macro_item(::std::vector<AST::Attribute> outer_attrs);
        AST::Visibility parse_visibility();

        // VisItem subclass-related
        AST::Module* parse_module(AST::Visibility vis, ::std::vector<AST::Attribute> outer_attrs);
        AST::ExternCrate* parse_extern_crate(
          AST::Visibility vis, ::std::vector<AST::Attribute> outer_attrs);
        AST::UseDeclaration* parse_use_decl(
          AST::Visibility vis, ::std::vector<AST::Attribute> outer_attrs);
        AST::UseTree* parse_use_tree();
        AST::Function* parse_function(AST::Visibility vis, ::std::vector<AST::Attribute> outer_attrs);
        AST::FunctionQualifiers parse_function_qualifiers();
        ::std::vector< ::std::unique_ptr<AST::GenericParam> > parse_generic_params_in_angles();
        ::std::vector< ::std::unique_ptr<AST::GenericParam> > parse_generic_params();
        ::std::vector< ::std::unique_ptr<AST::LifetimeParam> > parse_lifetime_params();
        ::std::vector<AST::LifetimeParam> parse_lifetime_params_objs();
        AST::LifetimeParam parse_lifetime_param();
        ::std::vector< ::std::unique_ptr<AST::TypeParam> > parse_type_params();
        AST::TypeParam* parse_type_param();
        ::std::vector<AST::FunctionParam> parse_function_params();
        AST::FunctionParam parse_function_param();
        AST::Type* parse_function_return_type();
        AST::WhereClause parse_where_clause();
        AST::WhereClauseItem* parse_where_clause_item();
        AST::LifetimeWhereClauseItem* parse_lifetime_where_clause_item();
        AST::TypeBoundWhereClauseItem* parse_type_bound_where_clause_item();
        ::std::vector<AST::LifetimeParam> parse_for_lifetimes();
        ::std::vector< ::std::unique_ptr<AST::TypeParamBound> > parse_type_param_bounds();
        AST::TypeParamBound* parse_type_param_bound();
        AST::TraitBound* parse_trait_bound();
        ::std::vector<AST::Lifetime> parse_lifetime_bounds();
        AST::Lifetime parse_lifetime();
        AST::TypeAlias* parse_type_alias(
          AST::Visibility vis, ::std::vector<AST::Attribute> outer_attrs);
        AST::Struct* parse_struct(AST::Visibility vis, ::std::vector<AST::Attribute> outer_attrs);
        ::std::vector<AST::StructField> parse_struct_fields();
        AST::StructField parse_struct_field();
        ::std::vector<AST::TupleField> parse_tuple_fields();
        AST::TupleField parse_tuple_field();
        AST::Enum* parse_enum(AST::Visibility vis, ::std::vector<AST::Attribute> outer_attrs);
        ::std::vector< ::std::unique_ptr<AST::EnumItem> > parse_enum_items();
        AST::EnumItem* parse_enum_item();
        AST::Union* parse_union(AST::Visibility vis, ::std::vector<AST::Attribute> outer_attrs);
        AST::ConstantItem* parse_const_item(
          AST::Visibility vis, ::std::vector<AST::Attribute> outer_attrs);
        AST::StaticItem* parse_static_item(
          AST::Visibility vis, ::std::vector<AST::Attribute> outer_attrs);
        AST::Trait* parse_trait(AST::Visibility vis, ::std::vector<AST::Attribute> outer_attrs);
        AST::TraitItem* parse_trait_item();
        AST::TraitItemType* parse_trait_type(::std::vector<AST::Attribute> outer_attrs);
        AST::TraitItemConst* parse_trait_const(::std::vector<AST::Attribute> outer_attrs);
        AST::SelfParam parse_self_param();
        AST::Impl* parse_impl(AST::Visibility vis, ::std::vector<AST::Attribute> outer_attrs);
        AST::InherentImplItem* parse_inherent_impl_item();
        AST::InherentImplItem* parse_inherent_impl_function_or_method(
          AST::Visibility vis, ::std::vector<AST::Attribute> outer_attrs);
        AST::TraitImplItem* parse_trait_impl_item();
        AST::TraitImplItem* parse_trait_impl_function_or_method(
          AST::Visibility vis, ::std::vector<AST::Attribute> outer_attrs);
        AST::ExternBlock* parse_extern_block(
          AST::Visibility vis, ::std::vector<AST::Attribute> outer_attrs);
        AST::ExternalItem* parse_external_item();
        AST::NamedFunctionParam parse_named_function_param();
        AST::Method parse_method();

        // Expression-related
        AST::Expr* parse_expr();
        AST::ExprWithoutBlock* parse_expr_without_block(::std::vector<AST::Attribute> outer_attrs = ::std::vector<AST::Attribute>());
        AST::BlockExpr* parse_block_expr(
          ::std::vector<AST::Attribute> outer_attrs = ::std::vector<AST::Attribute>());
        AST::IfExpr* parse_if_expr(::std::vector<AST::Attribute> outer_attrs = ::std::vector<AST::Attribute>());
        AST::IfLetExpr* parse_if_let_expr(::std::vector<AST::Attribute> outer_attrs = ::std::vector<AST::Attribute>());
        AST::LoopExpr* parse_loop_expr(::std::vector<AST::Attribute> outer_attrs = ::std::vector<AST::Attribute>());
        AST::WhileLoopExpr* parse_while_loop_expr(::std::vector<AST::Attribute> outer_attrs = ::std::vector<AST::Attribute>());
        AST::WhileLetLoopExpr* parse_while_let_loop_expr(::std::vector<AST::Attribute> outer_attrs = ::std::vector<AST::Attribute>());
        AST::ForLoopExpr* parse_for_loop_expr(::std::vector<AST::Attribute> outer_attrs = ::std::vector<AST::Attribute>());
        AST::MatchExpr* parse_match_expr(::std::vector<AST::Attribute> outer_attrs = ::std::vector<AST::Attribute>());
        AST::BaseLoopExpr* parse_labelled_loop_expr(::std::vector<AST::Attribute> outer_attrs = ::std::vector<AST::Attribute>());
        AST::AsyncBlockExpr* parse_async_block_expr(::std::vector<AST::Attribute> outer_attrs = ::std::vector<AST::Attribute>());
        AST::UnsafeBlockExpr* parse_unsafe_block_expr(::std::vector<AST::Attribute> outer_attrs = ::std::vector<AST::Attribute>());
        AST::GroupedExpr* parse_grouped_expr(::std::vector<AST::Attribute> outer_attrs = ::std::vector<AST::Attribute>());
        AST::ClosureExpr* parse_closure_expr(::std::vector<AST::Attribute> outer_attrs = ::std::vector<AST::Attribute>());
        AST::ClosureParam parse_closure_param();
        AST::LiteralExpr* parse_literal_expr(::std::vector<AST::Attribute> outer_attrs = ::std::vector<AST::Attribute>());

        // Type-related
        AST::Type* parse_type();
        AST::TypeNoBounds* parse_type_no_bounds();

        // Statement-related
        AST::Stmt* parse_stmt();
        AST::LetStmt* parse_let_stmt(::std::vector<AST::Attribute> outer_attrs);
        AST::ExprStmt* parse_expr_stmt(::std::vector<AST::Attribute> outer_attrs);
        AST::ExprStmtWithBlock* parse_expr_stmt_with_block(::std::vector<AST::Attribute> outer_attrs);
        AST::ExprStmtWithoutBlock* parse_expr_stmt_without_block(
          ::std::vector<AST::Attribute> outer_attrs);

        // Pattern-related
        AST::Pattern* parse_pattern();

        // void parse_crate();
        // AST::Module parse_module();
        // void parse_module_item(AST::Module module_for_items, AST::AttributeList
        // item_outer_attributes); AST::Visibility parse_visibility();

        bool done_end();
        bool done_end_or_else();
        bool done_end_of_file();

        typedef Tree (Parser::*BinaryHandler)(const_TokenPtr, Tree);
        BinaryHandler get_binary_handler(TokenId id);

      public:
        // Construct parser with specified lexer reference.
        Parser(Lexer& parLexer) : lexer(parLexer), puts_fn(), printf_fn(), scanf_fn() {}

        // Main entry point for parser.
        void parse_program();

        Tree parse_statement();

        Tree parse_variable_declaration();
        Tree parse_type_declaration();

        // Tree parse_type();
        Tree parse_record();
        Tree parse_field_declaration(std::vector<std::string>& field_names);

        Tree parse_assignment_statement();
        Tree parse_if_statement();
        Tree parse_while_statement();
        Tree parse_for_statement();
        Tree parse_read_statement();
        Tree parse_write_statement();

        Tree parse_expression();
        Tree parse_expression_naming_variable();
        Tree parse_lhs_assignment_expression();
        Tree parse_boolean_expression();
        Tree parse_integer_expression();

        // Dumps all lexer output.
        void debug_dump_lex_output();
        void debug_dump_ast_output();

      private:
        // The lexer associated with the parser.
        Lexer& lexer;
        // The current scope.
        Scope scope;

        // The simulated "main" function inside which the entire program lies.
        tree main_fndecl;

        // Address to function declaration of printf.
        Tree printf_fn;
        // Address to function declaration of puts.
        Tree puts_fn;
        // Address to function declaration of scanf.
        Tree scanf_fn;

        // The statement stack.
        std::vector<TreeStmtList> stack_stmt_list;
        // The VAR_DECL stack.
        std::vector<TreeChain> stack_var_decl_chain;

        // The block stack.
        std::vector<BlockChain> stack_block_chain;

// x-macro list for binary operators - only defined here to be inside Parser class
#define BINARY_HANDLER_LIST                                   \
    BINARY_HANDLER(plus, PLUS)                                \
    BINARY_HANDLER(minus, MINUS)                              \
    BINARY_HANDLER(mult, ASTERISK)                            \
    BINARY_HANDLER(div, DIV)                                  \
    BINARY_HANDLER(mod, PERCENT)                              \
    BINARY_HANDLER(bitwise_and, AMP)                          \
    BINARY_HANDLER(bitwise_or, PIPE)                          \
    BINARY_HANDLER(bitwise_xor, CARET)                        \
    BINARY_HANDLER(left_shift, LEFT_SHIFT)                    \
    BINARY_HANDLER(right_shift, RIGHT_SHIFT)                  \
                                                              \
    BINARY_HANDLER(equal, EQUAL_EQUAL)                        \
    BINARY_HANDLER(not_equal, NOT_EQUAL)                      \
    BINARY_HANDLER(smaller_than, LEFT_ANGLE)                  \
    BINARY_HANDLER(smaller_equal, LESS_OR_EQUAL)              \
    BINARY_HANDLER(greater_than, RIGHT_ANGLE)                 \
    BINARY_HANDLER(greater_equal, GREATER_OR_EQUAL)           \
                                                              \
    BINARY_HANDLER(logical_and, LOGICAL_AND)                  \
    BINARY_HANDLER(logical_or, OR)                            \
                                                              \
    BINARY_HANDLER(as_cast, AS)                               \
    /* should this really be an operator? */                  \
                                                              \
    BINARY_HANDLER(array_ref, LEFT_SQUARE)                    \
                                                              \
    BINARY_HANDLER(field_ref, DOT)                            \
    /*BINARY_HANDLER(question_mark, QUESTION_MARK)*/          \
    /* not a binary operator, technically */                  \
    BINARY_HANDLER(assignment_expr, EQUAL)                    \
    /* should this really be an operator? or a binary one? */ \
    /* if it should, also add all operation-assign below:*/   \
    BINARY_HANDLER(plus_assig, PLUS_EQ)                       \
    BINARY_HANDLER(minus_assig, MINUS_EQ)                     \
    BINARY_HANDLER(mult_assig, ASTERISK_EQ)                   \
    BINARY_HANDLER(div_assig, DIV_EQ)                         \
    BINARY_HANDLER(mod_assig, PERCENT_EQ)                     \
    BINARY_HANDLER(bitwise_and_assig, AMP_EQ)                 \
    BINARY_HANDLER(bitwise_or_assig, PIPE_EQ)                 \
    BINARY_HANDLER(bitwise_xor_assig, CARET_EQ)               \
    BINARY_HANDLER(left_shift_assig, LEFT_SHIFT_EQ)           \
    BINARY_HANDLER(right_shift_assig, RIGHT_SHIFT_EQ)

// create declarations for binary op handling
#define BINARY_HANDLER(name, _) Tree binary_##name(const_TokenPtr tok, Tree left);
        BINARY_HANDLER_LIST
#undef BINARY_HANDLER
    };
}

#endif // RUST_PARSE_H