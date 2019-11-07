#ifndef RUST_AST_MACRO_H
#define RUST_AST_MACRO_H

#include "rust-ast.h"

namespace Rust {
    namespace AST {
        // Decls as definitions moved to rust-ast.h
        class MacroItem;
        class MacroInvocationSemi;

        enum MacroFragSpec {
            BLOCK,
            EXPR,
            IDENT,
            ITEM,
            LIFETIME,
            LITERAL,
            META,
            PAT,
            PATH,
            STMT,
            TT,
            TY,
            VIS
        };

        // A macro match that has an identifier and frag spec
        class MacroMatchFragSpec : public MacroMatch {
            Identifier ident;
            MacroFragSpec frag_spec;

          public:
            MacroMatchFragSpec(Identifier ident, MacroFragSpec frag_spec) :
              ident(ident), frag_spec(frag_spec) {}
        };

        // A repetition macro match
        class MacroMatchRep : public MacroMatch {
            //::std::vector<MacroMatch> matches;
            ::std::vector< ::std::unique_ptr<MacroMatch> > matches;
            enum MacroRepOp { NONE, ASTERISK, PLUS, QUESTION_MARK } op;

            // bool has_sep;
            typedef Token MacroRepSep;
            // any token except delimiters and repetition operators
            ::std::unique_ptr<MacroRepSep> sep;

          public:
            // Returns whether macro match repetition has separator token.
            inline bool has_sep() const {
                return sep != NULL;
            }

            MacroMatchRep(::std::vector< ::std::unique_ptr<MacroMatch> > matches, MacroRepOp op,
              MacroRepSep* sep) :
              matches(matches),
              op(op), sep(sep) {}

            // Copy constructor with clone
            MacroMatchRep(MacroMatchRep const& other) :
              matches(other.matches), op(other.op), sep(other.sep->clone_token()) {}

            // Destructor - define here if required

            // Overloaded assignment operator to clone
            MacroMatchRep& operator=(MacroMatchRep const& other) {
                matches = other.matches;
                op = other.op;
                sep = other.sep->clone_token();

                return *this;
            }

            // no move constructors as not supported in c++03
            /*MacroMatchRep(MacroMatchRep&& other) = default;
            MacroMatchRep& operator=(MacroMatchRep&& other) = default;*/
        };

        // TODO: inline
        class MacroMatcher : public MacroMatch {
            enum DelimType { PARENS, SQUARE, CURLY } delim_type;
            //::std::vector<MacroMatch> matches;
            ::std::vector< ::std::unique_ptr<MacroMatch> > matches;

          public:
            MacroMatcher(
              DelimType delim_type, ::std::vector< ::std::unique_ptr<MacroMatch> > matches) :
              delim_type(delim_type),
              matches(matches) {}
        };

        // TODO: inline?
        struct MacroTranscriber {
            DelimTokenTree token_tree;

          public:
            MacroTranscriber(DelimTokenTree token_tree) : token_tree(token_tree) {}
        };

        // A macro rule? Matcher and transcriber pair?
        struct MacroRule {
            MacroMatcher matcher;
            MacroTranscriber transcriber;

          public:
            MacroRule(MacroMatcher matcher, MacroTranscriber transcriber) :
              matcher(matcher), transcriber(transcriber) {}
        };

        // TODO: inline
        /*struct MacroRules {
            ::std::vector<MacroRule> rules;
        };*/

        // TODO: inline?
        /*struct MacroRulesDef {
            enum DelimType {
                PARENS,
                SQUARE,
                CURLY // only one without required semicolon at end
            } delim_type;
            //MacroRules rules;
            ::std::vector<MacroRule> rules; // inlined form
        };*/

        // A macro rules definition item AST node
        class MacroRulesDefinition : public MacroItem {
            Identifier rule_name;
            // MacroRulesDef rules_def; // TODO: inline
            enum DelimType {
                PARENS,
                SQUARE,
                CURLY // only one without required semicolon at end
            } delim_type;
            // MacroRules rules;
            ::std::vector<MacroRule> rules; // inlined form

          public:
            ::std::string as_string() const;

            MacroRulesDefinition(Identifier rule_name, DelimType delim_type,
              ::std::vector<MacroRule> rules, ::std::vector<Attribute> outer_attrs) :
              rule_name(rule_name),
              delim_type(delim_type), rules(rules), MacroItem(outer_attrs) {}
        };

        // AST node of a macro invocation, which is replaced by the macro result at compile time
        class MacroInvocation
          : public TypeNoBounds
          , public Pattern
          , public ExprWithoutBlock {
            SimplePath path;
            DelimTokenTree token_tree;

          public:
            ::std::string as_string() const;

            MacroInvocation(
              SimplePath path, DelimTokenTree token_tree, ::std::vector<Attribute> outer_attrs) :
              path(path),
              token_tree(token_tree), ExprWithoutBlock(outer_attrs) {}

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual MacroInvocation* clone_pattern_impl() const OVERRIDE {
                return new MacroInvocation(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual MacroInvocation* clone_expr_impl() const OVERRIDE {
                return new MacroInvocation(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual MacroInvocation* clone_expr_without_block_impl() const OVERRIDE {
                return new MacroInvocation(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual MacroInvocation* clone_type_impl() const OVERRIDE {
                return new MacroInvocation(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual MacroInvocation* clone_type_no_bounds_impl() const OVERRIDE {
                return new MacroInvocation(*this);
            }
        };
    }
}

#endif