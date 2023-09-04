#ifndef RUST_ATTRIBUTES_VALUE_H
#define RUST_ATTRIBUTES_VALUE_H

namespace Rust {
namespace Values {
// TODO: Change this to a namespace + inline constexpr in the future
class Attributes
{
public:
  static constexpr auto &INLINE = "inline";
  static constexpr auto &COLD = "cold";
  static constexpr auto &CFG = "cfg";
  static constexpr auto &CFG_ATTR = "cfg_attr";
  static constexpr auto &DEPRECATED = "deprecated";
  static constexpr auto &ALLOW = "allow";
  static constexpr auto &ALLOW_INTERNAL_UNSTABLE = "allow_internal_unstable";
  static constexpr auto &DOC = "doc";
  static constexpr auto &MUST_USE = "must_use";
  static constexpr auto &LANG = "lang";
  static constexpr auto &LINK_SECTION = "link_section";
  static constexpr auto &NO_MANGLE = "no_mangle";
  static constexpr auto &REPR = "repr";
  static constexpr auto &RUSTC_BUILTIN_MACRO = "rustc_builtin_macro";
  static constexpr auto &PATH = "path";
  static constexpr auto &MACRO_USE = "macro_use";
  static constexpr auto &MACRO_EXPORT = "macro_export";
  static constexpr auto &PROC_MACRO = "proc_macro";
  static constexpr auto &PROC_MACRO_DERIVE = "proc_macro_derive";
  static constexpr auto &PROC_MACRO_ATTRIBUTE = "proc_macro_attribute";
  static constexpr auto &TARGET_FEATURE = "target_feature";
  // From now on, these are reserved by the compiler and gated through
  // #![feature(rustc_attrs)]
  static constexpr auto &RUSTC_INHERIT_OVERFLOW_CHECKS
    = "rustc_inherit_overflow_checks";
  static constexpr auto &STABLE = "stable";
};
} // namespace Values
} // namespace Rust

#endif /* !RUST_ATTRIBUTES_VALUE_H */
