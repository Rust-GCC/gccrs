/* An implementation of the inline visitor.
   https://members.accu.org/index.php/articles/2021 */

#include <utility>

#ifndef RUST_INLINE_VISITOR
#define RUST_INLINE_VISITOR

namespace Rust {

/* VisitorWrapper is a wrapper over a (possibly incomplete) Visitor.
   Add another visit() method by calling on(). These lambdas are stored
   in _args as a linked list, and passed to the actual visitor when
   end_visitor() is called. */
template <typename BaseVisitor, typename Args> struct VisitorWrapper
{
  Args _args;

  VisitorWrapper (Args &&args) : _args (args) {}

  // The actual visitor as an inner type.
  // Each visitor inherits from the last one, and implements one more visit().
  template <typename T, typename F> struct Visitor : BaseVisitor
  {
    F _f;
    Visitor (std::pair<F, Args> args)
      : BaseVisitor (args.second), _f (args.first)
    {}

    using BaseVisitor::visit;
    virtual void visit (T &t) final override { _f (t); }
  };

  template <typename T, typename F>
  VisitorWrapper<Visitor<T, F>, std::pair<F, Args>> on (F &&f)
  {
    return VisitorWrapper<Visitor<T, F>, std::pair<F, Args>> (
      std::make_pair (f, _args));
  }

  BaseVisitor end_visitor () { return BaseVisitor (_args); }
};

/* An empty visitor. A wrapper around the target Visitor we're matching against.
   Consumes the final nullptr of the linked list. */
template <typename TargetVisitor> struct EmptyVisitor : TargetVisitor
{
  EmptyVisitor (std::nullptr_t ptr) {}

  using TargetVisitor::visit;
};

// The entry point
template <typename TargetVisitor>
VisitorWrapper<EmptyVisitor<TargetVisitor>, std::nullptr_t>
begin_visitor ()
{
  return VisitorWrapper<EmptyVisitor<TargetVisitor>, std::nullptr_t> (nullptr);
}

} // namespace Rust

#endif
