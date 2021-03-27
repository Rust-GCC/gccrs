/* An implementation of polymophic_value for C++11.
   https://github.com/jbcoe/polymorphic_value */

#include <memory>
#include <type_traits>

#include "rust-make-unique.h"

#ifndef RUST_POLY
#define RUST_POLY

namespace Rust {

template <bool U>
using enable_if_t = typename std::enable_if<U, std::nullptr_t>::type;

/* Cloner acts as a type-erasure device.
   The concrete type, U, is erased.
   However, clone() still creates an instance of U. */
template <typename T> class Cloner {
public:
  virtual ~Cloner() = default;
  virtual std::unique_ptr<Cloner<T>> clone() = 0;
  virtual T *get() = 0;
};

template <typename T> class ConcreteCloner : public Cloner<T> {
private:
  static_assert(std::is_copy_constructible<T>::value,
                "T is not copy constructible");

  std::unique_ptr<T> _t;

public:
  ConcreteCloner(std::unique_ptr<T> &&t) : _t(std::move(t)) {}

  virtual std::unique_ptr<Cloner<T>> clone() override {
    auto t2 = Rust::make_unique<T>(*_t);
    return Rust::make_unique<ConcreteCloner<T>>(std::move(t2));
  }

  virtual T *get() override { return _t.get(); }
};

template <typename T, typename U,
          typename Rust::enable_if_t<std::is_base_of<T, U>::value> = nullptr>
class DelegateCloner : public Cloner<T> {
private:
  std::unique_ptr<Cloner<U>> _worker;

public:
  DelegateCloner(std::unique_ptr<Cloner<U>> worker)
      : _worker(std::move(worker)) {}

  virtual std::unique_ptr<Cloner<T>> clone() override {
    return Rust::make_unique<DelegateCloner<T, U>>(_worker->clone());
  }

  virtual T *get() override { return _worker->get(); }
};

/* Value wrapper that supports polymophic dispatch.
   It happens to be implemented on top of std::unique_ptr, but
   conceptually it's a value, not a pointer. In particular,
   nullptr is not allowed. As a wrapper, poly is immutable--its
   binding cannot be changed once created.
 */
template <typename T> class poly {
public:
  std::unique_ptr<Cloner<T>> _cloner;

  // Construct from raw pointer
  // Will take ownership of the value.
  poly (std::unique_ptr<T> &&t)
  {
    rust_assert (t != nullptr);
    _cloner = Rust::make_unique<ConcreteCloner<T>> (std::move (t));
  }

  // Copy constructor
  poly(const poly &rhs) { _cloner = rhs._cloner->clone(); }

  // Move constructor
  poly(poly &&rhs) noexcept : _cloner(std::move(rhs._cloner)) {}

  // Covariant "copy" constructor
  template <typename U,
            typename Rust::enable_if_t<std::is_base_of<T, U>::value> = nullptr>
  poly(const poly<U> &rhs)
      : _cloner(Rust::make_unique<DelegateCloner<T, U>>(rhs._cloner->clone())) {
  }

  // Covariant "move" constructor
  template <typename U,
            typename Rust::enable_if_t<std::is_base_of<T, U>::value> = nullptr>
  poly(poly<U> &&rhs) noexcept
      : _cloner(
            Rust::make_unique<DelegateCloner<T, U>>(std::move(rhs._cloner))) {}

  T *operator->() const { return _cloner->get(); }

  T *get() const { return _cloner->get(); }
};

template <typename T, typename... Args> poly<T> make_poly(Args &&... args) {
  return poly<T>(std::move(Rust::make_unique<T>(std::forward<Args>(args)...)));
}

}; // namespace Rust

#endif
