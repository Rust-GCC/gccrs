#ifndef RUST_AST_MACRO_H
#define RUST_AST_MACRO_H

namespace Rust {
    namespace AST {
        // A macro item AST node - potentially abstract base class
        class MacroItem : public Item {
            /*public:
            ::std::string as_string() const;*/
        };

        // A macro invocation item (or statement) AST node
        class MacroInvocationSemi : public MacroItem, public Statement {
            SimplePath path;
            enum DelimType {
                PARENS,
                SQUARE,
                CURLY // all delim types except curly must have invocation end with a semicolon
            } delim_type;
            //::std::vector<TokenTree> token_trees;
            ::std::vector< ::gnu::unique_ptr<TokenTree> > token_trees;

          public:
            ::std::string as_string() const;
        };

        // Abstract base class for a macro match
        class MacroMatch {};

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
        };

        // A repetition macro match
        class MacroMatchRep : public MacroMatch {
            //::std::vector<MacroMatch> matches;
            ::std::vector< ::gnu::unique_ptr<MacroMatch> > matches;
            enum MacroRepOp { NONE, ASTERISK, PLUS, QUESTION_MARK } op;

            bool has_sep;
            typedef Token MacroRepSep;
            MacroRepSep sep; // any token except delimiters and repetition operators
        };

        // TODO: inline
        class MacroMatcher : public MacroMatch {
            enum DelimType { PARENS, SQUARE, CURLY } delim_type;
            //::std::vector<MacroMatch> matches;
            ::std::vector< ::gnu::unique_ptr<MacroMatch> > matches;
        };

        // TODO: inline?
        struct MacroTranscriber {
            DelimTokenTree token_tree;
        };

        // A macro rule? Matcher and transcriber pair?
        struct MacroRule {
            MacroMatcher matcher;
            MacroTranscriber transcriber;
        };

        // TODO: inline
        struct MacroRules {
            ::std::vector<MacroRule> rules;
        };

        // TODO: inline?
        struct MacroRulesDef {
            enum DelimType {
                PARENS,
                SQUARE,
                CURLY // only one without required semicolon at end
            } delim_type;
            MacroRules rules;
        };

        // A macro rules definition item AST node
        class MacroRulesDefinition : public MacroItem {
            Identifier rule_name;
            MacroRulesDef rules_def; // TODO: inline

          public:
            ::std::string as_string() const;
        };

        // AST node of a macro invocation, which is replaced by the macro result at compile time
        class MacroInvocation : public TypeNoBounds, public Pattern, public ExprWithoutBlock {
            SimplePath path;
            DelimTokenTree token_tree;

          public:
            ::std::string as_string() const;
        };
    }
}

#endif