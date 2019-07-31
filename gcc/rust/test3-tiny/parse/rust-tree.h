#ifndef RUST_TREE_H
#define RUST_TREE_H

#include "tree.h"

namespace Rust {
    struct Tree {
      public:
        Tree() : t(NULL_TREE), loc(UNKNOWN_LOCATION) {}
        Tree(tree t_) : t(t_), loc(EXPR_LOCATION(t)) {}
        Tree(tree t_, location_t loc_) : t(t_), loc(loc_) {}
        Tree(Tree t_, location_t loc_) : t(t_.get_tree()), loc(loc_) {}

        // Get tree's location_t.
        location_t get_locus() const {
            return loc;
        }

        // Sets tree's location_t.
        void set_locus(location_t loc_) {
            loc = loc_;
        }

        // Gets tree's tree in GCC form.
        tree get_tree() const {
            return t;
        }

        // Gets tree's GCC tree code.
        tree_code get_tree_code() const {
            return TREE_CODE(t);
        }

        // Sets tree's GCC tree.
        void set_tree(tree t_) {
            t = t_;
        }

        // Returns if tree is an error node?
        bool is_error() const {
            return error_operand_p(t);
        }

        // Returns whether tree node is null.
        bool is_null() {
            return t == NULL_TREE;
        }

        // Creates an error Tree.
        static Tree error() {
            return Tree(error_mark_node);
        }

        // Gets tree's GCC type.
        Tree get_type() const {
            return TREE_TYPE(t);
        }

        // Comparison by identity as tree is a pointer.
        inline bool operator==(Tree t1, Tree t2) {
            return t1.get_tree() == t2.get_tree();
        }
        inline bool operator!=(Tree t1, Tree t2) {
            return !(t1 == t2);
        }

        inline Tree build_tree(tree_code tc, location_t loc, Tree type, Tree t1) {
            return build1_loc(loc, tc, type.get_tree(), t1.get_tree());
        }

        inline Tree build_tree(tree_code tc, location_t loc, Tree type, Tree t1, Tree t2) {
            return build2_loc(loc, tc, type.get_tree(), t1.get_tree(), t2.get_tree());
        }

        inline Tree build_tree(tree_code tc, location_t loc, Tree type, Tree t1, Tree t2, Tree t3) {
            return build3_loc(loc, tc, type.get_tree(), t1.get_tree(), t2.get_tree(), t3.get_tree());
        }

        inline Tree build_tree(
          tree_code tc, location_t loc, Tree type, Tree t1, Tree t2, Tree t3, Tree t4) {
            return build4_loc(
              loc, tc, type.get_tree(), t1.get_tree(), t2.get_tree(), t3.get_tree(), t4.get_tree());
        }

        inline Tree build_tree(
          tree_code tc, location_t loc, Tree type, Tree t1, Tree t2, Tree t3, Tree t4, Tree t5) {
            return build5_loc(loc, tc, type.get_tree(), t1.get_tree(), t2.get_tree(), t3.get_tree(),
              t4.get_tree(), t5.get_tree());
        }

      private:
        // The tree object's gcc tree representation.
        tree t;
        // The tree's location.
        location_t loc;
    };
}

#endif // RUST_TREE_H