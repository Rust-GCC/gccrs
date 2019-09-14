#ifndef RUST_TYPES_H
#define RUST_TYPES_H
// statically-typed AST nodes and whatever - may move

namespace Rust {
    // TODO: remove
    typedef int location_t;

    namespace AST {
        // Base AST node object
        struct Node {
          public:
            // Gets node's location_t.
            location_t get_locus() const {
                return loc;
            }

            // Sets node's location_t.
            void set_locus(location_t loc_) {
                loc = loc_;
            }

            virtual ~Node() {}

          private:
            // The node's location.
            location_t loc;
        };

        struct Expr : public Node {};

        // Literals? Or literal base?
        struct LiteralExpr : public Expr {};

        struct PathExpr : public Expr {};

        struct BlockExpr : public Expr {
            //vector<InnerAttribute> attributes;
            //Statement? statement;
            // can be Statement, Statement ExpressionWithoutBlock, ExpressionWithoutBlock
        };

        // Expression in parentheses (i.e. like literally just any 3 + (2 * 6))
        struct GroupedExpr : public Expr {
            //vector<InnerAttribute> attributes;
            //Expr actual_expr;
        };

        struct ArrayExpr : public Expr {
            //vector<InnerAttribute> attributes;
            //ArrayElements element;
            // can be comma-ed values inside array or semi-coloned
        };

        struct ArrayIndexExpr : public Expr {
            Expr array_expr;
            Expr index_expr;
        };

        struct TupleExpr : public Expr {
            //vector<InnerAttribute> attributes;
            //TupleElements element;
            // can be comma-ed values - at least one
        };

        struct TupleIndexExpr : public Expr {
            Expr tuple_expr;
            //DecimalIntLiteral index;
        };

        struct StructExpr : public Expr {};

        struct StructExprStruct : public StructExpr {
            //PathInExpression path;
            //vector<InnerAttribute> attributes;
            //(StructExprFields or StructBase)? something;

            //StructExprFields

            // too hard, come back later
        };

        // enum - too hard, come back later

        struct CallExpr : public Expr {
            Expr function;
            //CallParams? params;
        };

        struct CallParams {
            //vector<Expression> params;
        };

        struct MethodCallExpr : public Expr {
            Expr receiver;
            //PathExprSegment path;
            //CallParams? params;
        };

        struct FieldAccessExpr : public Expr {
            Expr receiver;
            //Identifier field;
        };   

        struct ClosureExpr : public Expr {
            bool hasMove;
            //vector<ClosureParam> params;
            union {
                Expr closure_inner;
                struct {
                    //TypeNoBounds type;
                    BlockExpr expr;
                } closure_inner_typed;
            } closure_inner;
        };

        struct ClosureParam {
            //Pattern pattern;
            //Type type;
        };

        // Base loop expression AST node
        struct BaseLoopExpr : public Expr {
            BlockExpr loop_block;
        };

        // 'Loop' expression (i.e. the infinite loop) AST node
        struct LoopExpr : public BaseLoopExpr {};

        // While loop expression AST node
        struct WhileLoopExpr : public BaseLoopExpr {
            Expr condition;
        };

        // While let loop expression AST node
        struct WhileLetLoopExpr : public BaseLoopExpr {
            //MatchArmPatterns patterns;
            Expr condition;
        };

        // For loop expression AST node (iterator)
        struct ForLoopExpr : public BaseLoopExpr {
            //Pattern pattern;
            Expr iterator_expr;
        };

        struct LoopLabel : public Node {
            //Lifetime label;
        };

        struct BreakExpr : public Expr {
            //Lifetime? label;
            Expr? break_expr;
        };

        struct ContinueExpr : public Expr {
            //Lifetime? label;
        };

        // Base range expression AST node object
        struct RangeExpr : public Expr {};

        // Range from (inclusive) and to (exclusive) expression AST node object
        struct RangeFromToExpr : public RangeExpr {
            Expr from;
            Expr to;
        };

        // Range from (inclusive) expression AST node object
        struct RangeFromExpr : public RangeExpr {
            Expr from;
        };

        // Range to (exclusive) expression AST node object
        struct RangeToExpr : public RangeExpr {
            Expr to;
        };

        // Full range expression AST node object
        struct RangeFullExpr : public RangeExpr {};

        // Range from (inclusive) and to (inclusive) expression AST node object
        struct RangeFromToInclExpr : public RangeExpr {
            Expr from;
            Expr to;
        };

        // Range to (inclusive) expression AST node object
        struct RangeToInclExpr : public RangeExpr {
            Expr to;
        };

        // forward decl for IfExpr
        struct IfLetExpr;

        struct IfExpr : public Expr {
            Expr condition;
            BlockExpr if_block;
            union {
                BlockExpr else_block;
                IfExpr* if_expr;
                IfLetExpr if_let_expr;
            } consequent_block;
        };

        struct IfLetExpr : public Expr {
            //MatchArmPatterns patterns;
            Expr value;
            BlockExpr if_block;
            union {
                BlockExpr else_block;
                IfExpr* if_expr;
                IfLetExpr if_let_expr;
            } consequent_block;
        };

        struct MatchExpr : public Expr {
            Expr branch_value;
            //vector<InnerAttribute> attributes;
            //MatchArms? match_arms;
        };

        struct MatchArms {
            //vector<MatchCase> cases;
        };

        struct MatchArm {
            //vector<OuterAttributes> attributes;
            //vector<Pattern> patterns;
            Expr? match_arm_guard;
        };

        struct MatchCase {
            MatchArm arm;
            union {
                BlockExpr match_block;
                Expr match_expr;
            } arm_expr;
        };

        struct ReturnExpr : public Expr {
            Expr return_expr;
        };

        // Pattern base AST node
        struct Pattern : public Node {};

        struct LiteralPattern : public Pattern {
            //Literal val?
            // Actually, this might be a good place to use a template. 
        };

        struct IdentifierPattern : public Pattern {
            //Identifier variable_ident;
            bool is_ref;
            bool is_mut;
            Pattern? to_bind;
        };

        struct WildcardPattern : public Pattern {};

        struct RangePatternBound {
            /*union {
                CharLiteral char_lit;
                ByteLiteral byte_lit;
                IntLiteral int_lit;
                FloatLiteral float_lit;
                PathInExpression path;
                QualifiedPathInExpression qual_path;
            } pattern;*/
        };

        struct RangePattern : public Pattern {
            RangePatternBound lower;
            RangePatternBound upper;
        };

        struct ReferencePattern : public Pattern {
            bool has_two_amps;
            bool is_mut;
            Pattern pattern;
        };

        struct StructPatternEtc {
            //vector<OuterAttribute> attribs;
        };

        struct StructPatternField {
            //vector<OuterAttribute> attribs;
            union {
                struct {
                    //TupleIndex index;
                    Pattern tuple_pattern;
                } tuple_pattern;
                struct {
                    //Identifier ident;
                    Pattern ident_pattern;
                } ident_pattern;
                struct {
                    bool has_ref;
                    bool has_mut;
                    //Identifier ident;
                } ident;
            } pattern;
        };

        struct StructPatternElements {};

        struct StructPatternElementsField : public StructPatternElements {
            //vector<StructPatternField> fields;
            StructPatternEtc? etc;
        };

        struct StructPatternElementsEtcOnly : public StructPatternElements {
            StructPatternEtc etc;
        };

        struct StructPattern : public Pattern {
            PathInExpression path;
            StructPatternElements elems;
        };

        struct TupleStructItems {};

        struct TupleStructItemsNoRange : public TupleStructItems {
            //vector<Pattern> patterns;
        };

        struct TupleStructItemsRange : public TupleStructItems {
            //vector<Pattern> lower_patterns;
            //vector<Pattern> upper_patterns;
        };

        struct TupleStructPattern : public Pattern {
            PathInExpression path;
            //TupleStructItems items;
        };

        struct TuplePattern : public Pattern {};

        struct GroupedPattern : public Pattern {};

        struct SlicePattern : public Pattern {};

        struct PathPattern : public Pattern {};

        struct MacroInvocation : public Pattern {};
        
#if 0
        // Base statement AST node object
        struct Statement : public Node {};

        // Item declaration statement AST node object
        //struct Item : public Statement {};

        // Let statement AST node object
        struct LetStatement : public Statement {
            // TODO
            //vector<OuterAttribute> attributes;
            //"pattern" - this is complicated
            //type annotation? type hint?
            //expression
        };

        // Expression statement AST node object
        struct ExpressionStatement : public Statement {
            //Expression expression;
            //bool expression_has_block; ?
            // Some way of distinguishing between expression with block and without block
        };

        // Macro invocation semi statement AST node object
        struct MacroInvocationSemi : public Statement {};

        // Module AST node object
        struct Module : public Node {
            //Identifier identifier;
        };

        // Module (with body) AST node object
        struct ModuleItem : public Module {
            //vector<InnerAttribute> attributes;
            //vector<Item> items;
        };

        // Module (with no body - i.e. the importing one) AST node object
        struct ModuleDecl : public Module {};

        struct UseDecl : public Node {};
#endif
    }
}

#endif