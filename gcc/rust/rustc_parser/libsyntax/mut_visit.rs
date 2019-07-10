//! A MutVisitor represents an AST modification; it accepts an AST piece and
//! and mutates it in place. So, for instance, macro expansion is a MutVisitor
//! that walks over an AST and modifies it.
//!
//! Note: using a MutVisitor (other than the MacroExpander MutVisitor) on
//! an AST before macro expansion is probably a bad idea. For instance,
//! a MutVisitor renaming item names in a module will miss all of those
//! that are created by the expansion of a macro.

use crate::ast::*;
use crate::source_map::{Spanned, respan};
use crate::parse::token::{self, Token};
use crate::ptr::P;
use crate::ThinVec;
use crate::tokenstream::*;
use crate::util::map_in_place::MapInPlace;

use smallvec::{smallvec, Array, SmallVec};
use syntax_pos::Span;

use rustc_data_structures::sync::Lrc;
use std::ops::DerefMut;
use std::{panic, process, ptr};

pub trait ExpectOne<A: Array> {
    fn expect_one(self, err: &'static str) -> A::Item;
}

impl<A: Array> ExpectOne<A> for SmallVec<A> {
    fn expect_one(self, err: &'static str) -> A::Item {
        assert!(self.len() == 1, err);
        self.into_iter().next().unwrap()
    }
}

pub trait MutVisitor: Sized {
    // Methods in this trait have one of three forms:
    //
    //   fn visit_t(&mut self, t: &mut T);                      // common
    //   fn flat_map_t(&mut self, t: T) -> SmallVec<[T; 1]>;    // rare
    //   fn filter_map_t(&mut self, t: T) -> Option<T>;         // rarest
    //
    // Any additions to this trait should happen in form of a call to a public
    // `noop_*` function that only calls out to the visitor again, not other
    // `noop_*` functions. This is a necessary API workaround to the problem of
    // not being able to call out to the super default method in an overridden
    // default method.
    //
    // When writing these methods, it is better to use destructuring like this:
    //
    //   fn visit_abc(&mut self, ABC { a, b, c: _ }: &mut ABC) {
    //       visit_a(a);
    //       visit_b(b);
    //   }
    //
    // than to use field access like this:
    //
    //   fn visit_abc(&mut self, abc: &mut ABC) {
    //       visit_a(&mut abc.a);
    //       visit_b(&mut abc.b);
    //       // ignore abc.c
    //   }
    //
    // As well as being more concise, the former is explicit about which fields
    // are skipped. Furthermore, if a new field is added, the destructuring
    // version will cause a compile error, which is good. In comparison, the
    // field access version will continue working and it would be easy to
    // forget to add handling for it.

    fn visit_crate(&mut self, c: &mut Crate) {
        noop_visit_crate(c, self)
    }

    fn visit_meta_list_item(&mut self, list_item: &mut NestedMetaItem) {
        noop_visit_meta_list_item(list_item, self);
    }

    fn visit_meta_item(&mut self, meta_item: &mut MetaItem) {
        noop_visit_meta_item(meta_item, self);
    }

    fn visit_use_tree(&mut self, use_tree: &mut UseTree) {
        noop_visit_use_tree(use_tree, self);
    }

    fn flat_map_foreign_item(&mut self, ni: ForeignItem) -> SmallVec<[ForeignItem; 1]> {
        noop_flat_map_foreign_item(ni, self)
    }

    fn flat_map_item(&mut self, i: P<Item>) -> SmallVec<[P<Item>; 1]> {
        noop_flat_map_item(i, self)
    }

    fn visit_fn_header(&mut self, header: &mut FnHeader) {
        noop_visit_fn_header(header, self);
    }

    fn visit_struct_field(&mut self, sf: &mut StructField) {
        noop_visit_struct_field(sf, self);
    }

    fn visit_item_kind(&mut self, i: &mut ItemKind) {
        noop_visit_item_kind(i, self);
    }

    fn flat_map_trait_item(&mut self, i: TraitItem) -> SmallVec<[TraitItem; 1]> {
        noop_flat_map_trait_item(i, self)
    }

    fn flat_map_impl_item(&mut self, i: ImplItem) -> SmallVec<[ImplItem; 1]> {
        noop_flat_map_impl_item(i, self)
    }

    fn visit_fn_decl(&mut self, d: &mut P<FnDecl>) {
        noop_visit_fn_decl(d, self);
    }

    fn visit_asyncness(&mut self, a: &mut IsAsync) {
        noop_visit_asyncness(a, self);
    }

    fn visit_block(&mut self, b: &mut P<Block>) {
        noop_visit_block(b, self);
    }

    fn flat_map_stmt(&mut self, s: Stmt) -> SmallVec<[Stmt; 1]> {
        noop_flat_map_stmt(s, self)
    }

    fn visit_arm(&mut self, a: &mut Arm) {
        noop_visit_arm(a, self);
    }

    fn visit_pat(&mut self, p: &mut P<Pat>) {
        noop_visit_pat(p, self);
    }

    fn visit_anon_const(&mut self, c: &mut AnonConst) {
        noop_visit_anon_const(c, self);
    }

    fn visit_expr(&mut self, e: &mut P<Expr>) {
        noop_visit_expr(e, self);
    }

    fn filter_map_expr(&mut self, e: P<Expr>) -> Option<P<Expr>> {
        noop_filter_map_expr(e, self)
    }

    fn visit_generic_arg(&mut self, arg: &mut GenericArg) {
        noop_visit_generic_arg(arg, self);
    }

    fn visit_ty(&mut self, t: &mut P<Ty>) {
        noop_visit_ty(t, self);
    }

    fn visit_lifetime(&mut self, l: &mut Lifetime) {
        noop_visit_lifetime(l, self);
    }

    fn visit_ty_constraint(&mut self, t: &mut AssocTyConstraint) {
        noop_visit_ty_constraint(t, self);
    }

    fn visit_mod(&mut self, m: &mut Mod) {
        noop_visit_mod(m, self);
    }

    fn visit_foreign_mod(&mut self, nm: &mut ForeignMod) {
        noop_visit_foreign_mod(nm, self);
    }

    fn visit_variant(&mut self, v: &mut Variant) {
        noop_visit_variant(v, self);
    }

    fn visit_ident(&mut self, i: &mut Ident) {
        noop_visit_ident(i, self);
    }

    fn visit_path(&mut self, p: &mut Path) {
        noop_visit_path(p, self);
    }

    fn visit_qself(&mut self, qs: &mut Option<QSelf>) {
        noop_visit_qself(qs, self);
    }

    fn visit_generic_args(&mut self, p: &mut GenericArgs) {
        noop_visit_generic_args(p, self);
    }

    fn visit_angle_bracketed_parameter_data(&mut self, p: &mut AngleBracketedArgs) {
        noop_visit_angle_bracketed_parameter_data(p, self);
    }

    fn visit_parenthesized_parameter_data(&mut self, p: &mut ParenthesizedArgs) {
        noop_visit_parenthesized_parameter_data(p, self);
    }

    fn visit_local(&mut self, l: &mut P<Local>) {
        noop_visit_local(l, self);
    }

    fn visit_mac(&mut self, _mac: &mut Mac) {
        panic!("visit_mac disabled by default");
        // N.B., see note about macros above. If you really want a visitor that
        // works on macros, use this definition in your trait impl:
        //   mut_visit::noop_visit_mac(_mac, self);
    }

    fn visit_macro_def(&mut self, def: &mut MacroDef) {
        noop_visit_macro_def(def, self);
    }

    fn visit_label(&mut self, label: &mut Label) {
        noop_visit_label(label, self);
    }

    fn visit_attribute(&mut self, at: &mut Attribute) {
        noop_visit_attribute(at, self);
    }

    fn visit_arg(&mut self, a: &mut Arg) {
        noop_visit_arg(a, self);
    }

    fn visit_generics(&mut self, generics: &mut Generics) {
        noop_visit_generics(generics, self);
    }

    fn visit_trait_ref(&mut self, tr: &mut TraitRef) {
        noop_visit_trait_ref(tr, self);
    }

    fn visit_poly_trait_ref(&mut self, p: &mut PolyTraitRef) {
        noop_visit_poly_trait_ref(p, self);
    }

    fn visit_variant_data(&mut self, vdata: &mut VariantData) {
        noop_visit_variant_data(vdata, self);
    }

    fn visit_generic_param(&mut self, param: &mut GenericParam) {
        noop_visit_generic_param(param, self);
    }

    fn visit_generic_params(&mut self, params: &mut Vec<GenericParam>) {
        noop_visit_generic_params(params, self);
    }

    fn visit_tt(&mut self, tt: &mut TokenTree) {
        noop_visit_tt(tt, self);
    }

    fn visit_tts(&mut self, tts: &mut TokenStream) {
        noop_visit_tts(tts, self);
    }

    fn visit_token(&mut self, t: &mut Token) {
        noop_visit_token(t, self);
    }

    fn visit_interpolated(&mut self, nt: &mut token::Nonterminal) {
        noop_visit_interpolated(nt, self);
    }

    fn visit_param_bound(&mut self, tpb: &mut GenericBound) {
        noop_visit_param_bound(tpb, self);
    }

    fn visit_mt(&mut self, mt: &mut MutTy) {
        noop_visit_mt(mt, self);
    }

    fn visit_field(&mut self, field: &mut Field) {
        noop_visit_field(field, self);
    }

    fn visit_where_clause(&mut self, where_clause: &mut WhereClause) {
        noop_visit_where_clause(where_clause, self);
    }

    fn visit_where_predicate(&mut self, where_predicate: &mut WherePredicate) {
        noop_visit_where_predicate(where_predicate, self);
    }

    fn visit_vis(&mut self, vis: &mut Visibility) {
        noop_visit_vis(vis, self);
    }

    fn visit_id(&mut self, _id: &mut NodeId) {
        // Do nothing.
    }

    fn visit_span(&mut self, _sp: &mut Span) {
        // Do nothing.
    }
}

/// Use a map-style function (`FnOnce(T) -> T`) to overwrite a `&mut T`. Useful
/// when using a `flat_map_*` or `filter_map_*` method within a `visit_`
/// method. Abort the program if the closure panics.
//
// No `noop_` prefix because there isn't a corresponding method in `MutVisitor`.
pub fn visit_clobber<T, F>(t: &mut T, f: F) where F: FnOnce(T) -> T {
    unsafe {
        // Safe because `t` is used in a read-only fashion by `read()` before
        // being overwritten by `write()`.
        let old_t = ptr::read(t);
        let new_t = panic::catch_unwind(panic::AssertUnwindSafe(|| f(old_t)))
            .unwrap_or_else(|_| process::abort());
        ptr::write(t, new_t);
    }
}

// No `noop_` prefix because there isn't a corresponding method in `MutVisitor`.
#[inline]
pub fn visit_vec<T, F>(elems: &mut Vec<T>, mut visit_elem: F) where F: FnMut(&mut T) {
    for elem in elems {
        visit_elem(elem);
    }
}

// No `noop_` prefix because there isn't a corresponding method in `MutVisitor`.
#[inline]
pub fn visit_opt<T, F>(opt: &mut Option<T>, mut visit_elem: F) where F: FnMut(&mut T) {
    if let Some(elem) = opt {
        visit_elem(elem);
    }
}

// No `noop_` prefix because there isn't a corresponding method in `MutVisitor`.
pub fn visit_attrs<T: MutVisitor>(attrs: &mut Vec<Attribute>, vis: &mut T) {
    visit_vec(attrs, |attr| vis.visit_attribute(attr));
}

// No `noop_` prefix because there isn't a corresponding method in `MutVisitor`.
pub fn visit_thin_attrs<T: MutVisitor>(attrs: &mut ThinVec<Attribute>, vis: &mut T) {
    for attr in attrs.iter_mut() {
        vis.visit_attribute(attr);
    }
}

// No `noop_` prefix because there isn't a corresponding method in `MutVisitor`.
pub fn visit_exprs<T: MutVisitor>(exprs: &mut Vec<P<Expr>>, vis: &mut T) {
    exprs.flat_map_in_place(|expr| vis.filter_map_expr(expr))
}

// No `noop_` prefix because there isn't a corresponding method in `MutVisitor`.
pub fn visit_bounds<T: MutVisitor>(bounds: &mut GenericBounds, vis: &mut T) {
    visit_vec(bounds, |bound| vis.visit_param_bound(bound));
}

// No `noop_` prefix because there isn't a corresponding method in `MutVisitor`.
pub fn visit_method_sig<T: MutVisitor>(MethodSig { header, decl }: &mut MethodSig, vis: &mut T) {
    vis.visit_fn_header(header);
    vis.visit_fn_decl(decl);
}

pub fn noop_visit_use_tree<T: MutVisitor>(use_tree: &mut UseTree, vis: &mut T) {
    let UseTree { prefix, kind, span } = use_tree;
    vis.visit_path(prefix);
    match kind {
        UseTreeKind::Simple(rename, id1, id2) => {
            visit_opt(rename, |rename| vis.visit_ident(rename));
            vis.visit_id(id1);
            vis.visit_id(id2);
        }
        UseTreeKind::Nested(items) => {
            for (tree, id) in items {
                vis.visit_use_tree(tree);
                vis.visit_id(id);
            }
        }
        UseTreeKind::Glob => {}
    }
    vis.visit_span(span);
}

pub fn noop_visit_arm<T: MutVisitor>(
    Arm { attrs, pats, guard, body, span }: &mut Arm,
    vis: &mut T,
) {
    visit_attrs(attrs, vis);
    visit_vec(pats, |pat| vis.visit_pat(pat));
    visit_opt(guard, |guard| vis.visit_expr(guard));
    vis.visit_expr(body);
    vis.visit_span(span);
}

pub fn noop_visit_ty_constraint<T: MutVisitor>(
    AssocTyConstraint { id, ident, kind, span }: &mut AssocTyConstraint,
    vis: &mut T
) {
    vis.visit_id(id);
    vis.visit_ident(ident);
    match kind {
        AssocTyConstraintKind::Equality { ref mut ty } => {
            vis.visit_ty(ty);
        }
        AssocTyConstraintKind::Bound { ref mut bounds } => {
            visit_bounds(bounds, vis);
        }
    }
    vis.visit_span(span);
}

pub fn noop_visit_ty<T: MutVisitor>(ty: &mut P<Ty>, vis: &mut T) {
    let Ty { id, node, span } = ty.deref_mut();
    vis.visit_id(id);
    match node {
        TyKind::Infer | TyKind::ImplicitSelf | TyKind::Err |
            TyKind::Never | TyKind::CVarArgs => {}
        TyKind::Slice(ty) => vis.visit_ty(ty),
        TyKind::Ptr(mt) => vis.visit_mt(mt),
        TyKind::Rptr(lt, mt) => {
            visit_opt(lt, |lt| noop_visit_lifetime(lt, vis));
            vis.visit_mt(mt);
        }
        TyKind::BareFn(bft) => {
            let BareFnTy { unsafety: _, abi: _, generic_params, decl } = bft.deref_mut();
            vis.visit_generic_params(generic_params);
            vis.visit_fn_decl(decl);
        }
        TyKind::Tup(tys) => visit_vec(tys, |ty| vis.visit_ty(ty)),
        TyKind::Paren(ty) => vis.visit_ty(ty),
        TyKind::Path(qself, path) => {
            vis.visit_qself(qself);
            vis.visit_path(path);
        }
        TyKind::Array(ty, length) => {
            vis.visit_ty(ty);
            vis.visit_anon_const(length);
        }
        TyKind::Typeof(expr) => vis.visit_anon_const(expr),
        TyKind::TraitObject(bounds, _syntax) =>
            visit_vec(bounds, |bound| vis.visit_param_bound(bound)),
        TyKind::ImplTrait(id, bounds) => {
            vis.visit_id(id);
            visit_vec(bounds, |bound| vis.visit_param_bound(bound));
        }
        TyKind::Mac(mac) => vis.visit_mac(mac),
    }
    vis.visit_span(span);
}

pub fn noop_visit_foreign_mod<T: MutVisitor>(foreign_mod: &mut ForeignMod, vis: &mut T) {
    let ForeignMod { abi: _, items} = foreign_mod;
    items.flat_map_in_place(|item| vis.flat_map_foreign_item(item));
}

pub fn noop_visit_variant<T: MutVisitor>(variant: &mut Variant, vis: &mut T) {
    let Spanned { node: Variant_ { ident, attrs, id, data, disr_expr }, span } = variant;
    vis.visit_ident(ident);
    visit_attrs(attrs, vis);
    vis.visit_id(id);
    vis.visit_variant_data(data);
    visit_opt(disr_expr, |disr_expr| vis.visit_anon_const(disr_expr));
    vis.visit_span(span);
}

pub fn noop_visit_ident<T: MutVisitor>(Ident { name: _, span }: &mut Ident, vis: &mut T) {
    vis.visit_span(span);
}

pub fn noop_visit_path<T: MutVisitor>(Path { segments, span }: &mut Path, vis: &mut T) {
    vis.visit_span(span);
    for PathSegment { ident, id, args } in segments {
        vis.visit_ident(ident);
        vis.visit_id(id);
        visit_opt(args, |args| vis.visit_generic_args(args));
    }
}

pub fn noop_visit_qself<T: MutVisitor>(qself: &mut Option<QSelf>, vis: &mut T) {
    visit_opt(qself, |QSelf { ty, path_span, position: _ }| {
        vis.visit_ty(ty);
        vis.visit_span(path_span);
    })
}

pub fn noop_visit_generic_args<T: MutVisitor>(generic_args: &mut GenericArgs, vis: &mut T) {
    match generic_args {
        GenericArgs::AngleBracketed(data) => vis.visit_angle_bracketed_parameter_data(data),
        GenericArgs::Parenthesized(data) => vis.visit_parenthesized_parameter_data(data),
    }
}

pub fn noop_visit_generic_arg<T: MutVisitor>(arg: &mut GenericArg, vis: &mut T) {
    match arg {
        GenericArg::Lifetime(lt) => vis.visit_lifetime(lt),
        GenericArg::Type(ty) => vis.visit_ty(ty),
        GenericArg::Const(ct) => vis.visit_anon_const(ct),
    }
}

pub fn noop_visit_angle_bracketed_parameter_data<T: MutVisitor>(data: &mut AngleBracketedArgs,
                                                                vis: &mut T) {
    let AngleBracketedArgs { args, constraints, span } = data;
    visit_vec(args, |arg| vis.visit_generic_arg(arg));
    visit_vec(constraints, |constraint| vis.visit_ty_constraint(constraint));
    vis.visit_span(span);
}

pub fn noop_visit_parenthesized_parameter_data<T: MutVisitor>(args: &mut ParenthesizedArgs,
                                                              vis: &mut T) {
    let ParenthesizedArgs { inputs, output, span } = args;
    visit_vec(inputs, |input| vis.visit_ty(input));
    visit_opt(output, |output| vis.visit_ty(output));
    vis.visit_span(span);
}

pub fn noop_visit_local<T: MutVisitor>(local: &mut P<Local>, vis: &mut T) {
    let Local { id, pat, ty, init, span, attrs } = local.deref_mut();
    vis.visit_id(id);
    vis.visit_pat(pat);
    visit_opt(ty, |ty| vis.visit_ty(ty));
    visit_opt(init, |init| vis.visit_expr(init));
    vis.visit_span(span);
    visit_thin_attrs(attrs, vis);
}

pub fn noop_visit_attribute<T: MutVisitor>(attr: &mut Attribute, vis: &mut T) {
    let Attribute { id: _, style: _, path, tokens, is_sugared_doc: _, span } = attr;
    vis.visit_path(path);
    vis.visit_tts(tokens);
    vis.visit_span(span);
}

pub fn noop_visit_mac<T: MutVisitor>(Spanned { node, span }: &mut Mac, vis: &mut T) {
    let Mac_ { path, delim: _, tts } = node;
    vis.visit_path(path);
    vis.visit_tts(tts);
    vis.visit_span(span);
}

pub fn noop_visit_macro_def<T: MutVisitor>(macro_def: &mut MacroDef, vis: &mut T) {
    let MacroDef { tokens, legacy: _ } = macro_def;
    vis.visit_tts(tokens);
}

pub fn noop_visit_meta_list_item<T: MutVisitor>(li: &mut NestedMetaItem, vis: &mut T) {
    match li {
        NestedMetaItem::MetaItem(mi) => vis.visit_meta_item(mi),
        NestedMetaItem::Literal(_lit) => {}
    }
}

pub fn noop_visit_meta_item<T: MutVisitor>(mi: &mut MetaItem, vis: &mut T) {
    let MetaItem { path: _, node, span } = mi;
    match node {
        MetaItemKind::Word => {}
        MetaItemKind::List(mis) => visit_vec(mis, |mi| vis.visit_meta_list_item(mi)),
        MetaItemKind::NameValue(_s) => {}
    }
    vis.visit_span(span);
}

pub fn noop_visit_arg<T: MutVisitor>(Arg { attrs, id, pat, ty }: &mut Arg, vis: &mut T) {
    vis.visit_id(id);
    visit_thin_attrs(attrs, vis);
    vis.visit_pat(pat);
    vis.visit_ty(ty);
}

pub fn noop_visit_tt<T: MutVisitor>(tt: &mut TokenTree, vis: &mut T) {
    match tt {
        TokenTree::Token(token) => {
            vis.visit_token(token);
        }
        TokenTree::Delimited(DelimSpan { open, close }, _delim, tts) => {
            vis.visit_span(open);
            vis.visit_span(close);
            vis.visit_tts(tts);
        }
    }
}

pub fn noop_visit_tts<T: MutVisitor>(TokenStream(tts): &mut TokenStream, vis: &mut T) {
    visit_opt(tts, |tts| {
        let tts = Lrc::make_mut(tts);
        visit_vec(tts, |(tree, _is_joint)| vis.visit_tt(tree));
    })
}

// Apply ident visitor if it's an ident, apply other visits to interpolated nodes.
// In practice the ident part is not actually used by specific visitors right now,
// but there's a test below checking that it works.
pub fn noop_visit_token<T: MutVisitor>(t: &mut Token, vis: &mut T) {
    let Token { kind, span } = t;
    match kind {
        token::Ident(name, _) | token::Lifetime(name) => {
            let mut ident = Ident::new(*name, *span);
            vis.visit_ident(&mut ident);
            *name = ident.name;
            *span = ident.span;
            return; // avoid visiting the span for the second time
        }
        token::Interpolated(nt) => {
            let mut nt = Lrc::make_mut(nt);
            vis.visit_interpolated(&mut nt);
        }
        _ => {}
    }
    vis.visit_span(span);
}

/// Apply visitor to elements of interpolated nodes.
//
// N.B., this can occur only when applying a visitor to partially expanded
// code, where parsed pieces have gotten implanted ito *other* macro
// invocations. This is relevant for macro hygiene, but possibly not elsewhere.
//
// One problem here occurs because the types for flat_map_item, flat_map_stmt,
// etc. allow the visitor to return *multiple* items; this is a problem for the
// nodes here, because they insist on having exactly one piece. One solution
// would be to mangle the MutVisitor trait to include one-to-many and
// one-to-one versions of these entry points, but that would probably confuse a
// lot of people and help very few. Instead, I'm just going to put in dynamic
// checks. I think the performance impact of this will be pretty much
// nonexistent. The danger is that someone will apply a MutVisitor to a
// partially expanded node, and will be confused by the fact that their
// "flat_map_item" or "flat_map_stmt" isn't getting called on NtItem or NtStmt
// nodes. Hopefully they'll wind up reading this comment, and doing something
// appropriate.
//
// BTW, design choice: I considered just changing the type of, e.g., NtItem to
// contain multiple items, but decided against it when I looked at
// parse_item_or_view_item and tried to figure out what I would do with
// multiple items there....
pub fn noop_visit_interpolated<T: MutVisitor>(nt: &mut token::Nonterminal, vis: &mut T) {
    match nt {
        token::NtItem(item) =>
            visit_clobber(item, |item| {
                // This is probably okay, because the only visitors likely to
                // peek inside interpolated nodes will be renamings/markings,
                // which map single items to single items.
                vis.flat_map_item(item).expect_one("expected visitor to produce exactly one item")
            }),
        token::NtBlock(block) => vis.visit_block(block),
        token::NtStmt(stmt) =>
            visit_clobber(stmt, |stmt| {
                // See reasoning above.
                vis.flat_map_stmt(stmt).expect_one("expected visitor to produce exactly one item")
            }),
        token::NtPat(pat) => vis.visit_pat(pat),
        token::NtExpr(expr) => vis.visit_expr(expr),
        token::NtTy(ty) => vis.visit_ty(ty),
        token::NtIdent(ident, _is_raw) => vis.visit_ident(ident),
        token::NtLifetime(ident) => vis.visit_ident(ident),
        token::NtLiteral(expr) => vis.visit_expr(expr),
        token::NtMeta(meta) => vis.visit_meta_item(meta),
        token::NtPath(path) => vis.visit_path(path),
        token::NtTT(tt) => vis.visit_tt(tt),
        token::NtImplItem(item) =>
            visit_clobber(item, |item| {
                // See reasoning above.
                vis.flat_map_impl_item(item)
                    .expect_one("expected visitor to produce exactly one item")
            }),
        token::NtTraitItem(item) =>
            visit_clobber(item, |item| {
                // See reasoning above.
                vis.flat_map_trait_item(item)
                    .expect_one("expected visitor to produce exactly one item")
            }),
        token::NtVis(visib) => vis.visit_vis(visib),
        token::NtForeignItem(item) =>
            visit_clobber(item, |item| {
                // See reasoning above.
                vis.flat_map_foreign_item(item)
                    .expect_one("expected visitor to produce exactly one item")
            }),
    }
}

pub fn noop_visit_asyncness<T: MutVisitor>(asyncness: &mut IsAsync, vis: &mut T) {
    match asyncness {
        IsAsync::Async { closure_id, return_impl_trait_id } => {
            vis.visit_id(closure_id);
            vis.visit_id(return_impl_trait_id);
        }
        IsAsync::NotAsync => {}
    }
}

pub fn noop_visit_fn_decl<T: MutVisitor>(decl: &mut P<FnDecl>, vis: &mut T) {
    let FnDecl { inputs, output, c_variadic: _ } = decl.deref_mut();
    visit_vec(inputs, |input| vis.visit_arg(input));
    match output {
        FunctionRetTy::Default(span) => vis.visit_span(span),
        FunctionRetTy::Ty(ty) => vis.visit_ty(ty),
    }
}

pub fn noop_visit_param_bound<T: MutVisitor>(pb: &mut GenericBound, vis: &mut T) {
    match pb {
        GenericBound::Trait(ty, _modifier) => vis.visit_poly_trait_ref(ty),
        GenericBound::Outlives(lifetime) => noop_visit_lifetime(lifetime, vis),
    }
}

pub fn noop_visit_generic_param<T: MutVisitor>(param: &mut GenericParam, vis: &mut T) {
    let GenericParam { id, ident, attrs, bounds, kind } = param;
    vis.visit_id(id);
    vis.visit_ident(ident);
    visit_thin_attrs(attrs, vis);
    visit_vec(bounds, |bound| noop_visit_param_bound(bound, vis));
    match kind {
        GenericParamKind::Lifetime => {}
        GenericParamKind::Type { default } => {
            visit_opt(default, |default| vis.visit_ty(default));
        }
        GenericParamKind::Const { ty } => {
            vis.visit_ty(ty);
        }
    }
}

pub fn noop_visit_generic_params<T: MutVisitor>(params: &mut Vec<GenericParam>, vis: &mut T){
    visit_vec(params, |param| vis.visit_generic_param(param));
}

pub fn noop_visit_label<T: MutVisitor>(Label { ident }: &mut Label, vis: &mut T) {
    vis.visit_ident(ident);
}

fn noop_visit_lifetime<T: MutVisitor>(Lifetime { id, ident }: &mut Lifetime, vis: &mut T) {
    vis.visit_id(id);
    vis.visit_ident(ident);
}

pub fn noop_visit_generics<T: MutVisitor>(generics: &mut Generics, vis: &mut T) {
    let Generics { params, where_clause, span } = generics;
    vis.visit_generic_params(params);
    vis.visit_where_clause(where_clause);
    vis.visit_span(span);
}

pub fn noop_visit_where_clause<T: MutVisitor>(wc: &mut WhereClause, vis: &mut T) {
    let WhereClause { predicates, span } = wc;
    visit_vec(predicates, |predicate| vis.visit_where_predicate(predicate));
    vis.visit_span(span);
}

pub fn noop_visit_where_predicate<T: MutVisitor>(pred: &mut WherePredicate, vis: &mut T) {
    match pred {
        WherePredicate::BoundPredicate(bp) => {
            let WhereBoundPredicate { span, bound_generic_params, bounded_ty, bounds } = bp;
            vis.visit_span(span);
            vis.visit_generic_params(bound_generic_params);
            vis.visit_ty(bounded_ty);
            visit_vec(bounds, |bound| vis.visit_param_bound(bound));
        }
        WherePredicate::RegionPredicate(rp) => {
            let WhereRegionPredicate { span, lifetime, bounds } = rp;
            vis.visit_span(span);
            noop_visit_lifetime(lifetime, vis);
            visit_vec(bounds, |bound| noop_visit_param_bound(bound, vis));
        }
        WherePredicate::EqPredicate(ep) => {
            let WhereEqPredicate { id, span, lhs_ty, rhs_ty } = ep;
            vis.visit_id(id);
            vis.visit_span(span);
            vis.visit_ty(lhs_ty);
            vis.visit_ty(rhs_ty);
        }
    }
}

pub fn noop_visit_variant_data<T: MutVisitor>(vdata: &mut VariantData, vis: &mut T) {
    match vdata {
        VariantData::Struct(fields, ..) => visit_vec(fields, |field| vis.visit_struct_field(field)),
        VariantData::Tuple(fields, id) => {
            visit_vec(fields, |field| vis.visit_struct_field(field));
            vis.visit_id(id);
        },
        VariantData::Unit(id) => vis.visit_id(id),
    }
}

pub fn noop_visit_trait_ref<T: MutVisitor>(TraitRef { path, ref_id }: &mut TraitRef, vis: &mut T) {
    vis.visit_path(path);
    vis.visit_id(ref_id);
}

pub fn noop_visit_poly_trait_ref<T: MutVisitor>(p: &mut PolyTraitRef, vis: &mut T) {
    let PolyTraitRef { bound_generic_params, trait_ref, span } = p;
    vis.visit_generic_params(bound_generic_params);
    vis.visit_trait_ref(trait_ref);
    vis.visit_span(span);
}

pub fn noop_visit_struct_field<T: MutVisitor>(f: &mut StructField, visitor: &mut T) {
    let StructField { span, ident, vis, id, ty, attrs } = f;
    visitor.visit_span(span);
    visit_opt(ident, |ident| visitor.visit_ident(ident));
    visitor.visit_vis(vis);
    visitor.visit_id(id);
    visitor.visit_ty(ty);
    visit_attrs(attrs, visitor);
}

pub fn noop_visit_field<T: MutVisitor>(f: &mut Field, vis: &mut T) {
    let Field { ident, expr, span, is_shorthand: _, attrs } = f;
    vis.visit_ident(ident);
    vis.visit_expr(expr);
    vis.visit_span(span);
    visit_thin_attrs(attrs, vis);
}

pub fn noop_visit_mt<T: MutVisitor>(MutTy { ty, mutbl: _ }: &mut MutTy, vis: &mut T) {
    vis.visit_ty(ty);
}

pub fn noop_visit_block<T: MutVisitor>(block: &mut P<Block>, vis: &mut T) {
    let Block { id, stmts, rules: _, span } = block.deref_mut();
    vis.visit_id(id);
    stmts.flat_map_in_place(|stmt| vis.flat_map_stmt(stmt));
    vis.visit_span(span);
}

pub fn noop_visit_item_kind<T: MutVisitor>(kind: &mut ItemKind, vis: &mut T) {
    match kind {
        ItemKind::ExternCrate(_orig_name) => {}
        ItemKind::Use(use_tree) => vis.visit_use_tree(use_tree),
        ItemKind::Static(ty, _mut, expr) => {
            vis.visit_ty(ty);
            vis.visit_expr(expr);
        }
        ItemKind::Const(ty, expr) => {
            vis.visit_ty(ty);
            vis.visit_expr(expr);
        }
        ItemKind::Fn(decl, header, generics, body) => {
            vis.visit_fn_decl(decl);
            vis.visit_fn_header(header);
            vis.visit_generics(generics);
            vis.visit_block(body);
        }
        ItemKind::Mod(m) => vis.visit_mod(m),
        ItemKind::ForeignMod(nm) => vis.visit_foreign_mod(nm),
        ItemKind::GlobalAsm(_ga) => {}
        ItemKind::Ty(ty, generics) => {
            vis.visit_ty(ty);
            vis.visit_generics(generics);
        }
        ItemKind::Existential(bounds, generics) => {
            visit_bounds(bounds, vis);
            vis.visit_generics(generics);
        }
        ItemKind::Enum(EnumDef { variants }, generics) => {
            visit_vec(variants, |variant| vis.visit_variant(variant));
            vis.visit_generics(generics);
        }
        ItemKind::Struct(variant_data, generics) |
        ItemKind::Union(variant_data, generics) => {
            vis.visit_variant_data(variant_data);
            vis.visit_generics(generics);
        }
        ItemKind::Impl(_unsafety, _polarity, _defaultness, generics, trait_ref, ty, items) => {
            vis.visit_generics(generics);
            visit_opt(trait_ref, |trait_ref| vis.visit_trait_ref(trait_ref));
            vis.visit_ty(ty);
            items.flat_map_in_place(|item| vis.flat_map_impl_item(item));
        }
        ItemKind::Trait(_is_auto, _unsafety, generics, bounds, items) => {
            vis.visit_generics(generics);
            visit_bounds(bounds, vis);
            items.flat_map_in_place(|item| vis.flat_map_trait_item(item));
        }
        ItemKind::TraitAlias(generics, bounds) => {
            vis.visit_generics(generics);
            visit_bounds(bounds, vis);
        }
        ItemKind::Mac(m) => vis.visit_mac(m),
        ItemKind::MacroDef(def) => vis.visit_macro_def(def),
    }
}

pub fn noop_flat_map_trait_item<T: MutVisitor>(mut item: TraitItem, vis: &mut T)
    -> SmallVec<[TraitItem; 1]>
{
    let TraitItem { id, ident, attrs, generics, node, span, tokens: _ } = &mut item;
    vis.visit_id(id);
    vis.visit_ident(ident);
    visit_attrs(attrs, vis);
    vis.visit_generics(generics);
    match node {
        TraitItemKind::Const(ty, default) => {
            vis.visit_ty(ty);
            visit_opt(default, |default| vis.visit_expr(default));
        }
        TraitItemKind::Method(sig, body) => {
            visit_method_sig(sig, vis);
            visit_opt(body, |body| vis.visit_block(body));
        }
        TraitItemKind::Type(bounds, default) => {
            visit_bounds(bounds, vis);
            visit_opt(default, |default| vis.visit_ty(default));
        }
        TraitItemKind::Macro(mac) => {
            vis.visit_mac(mac);
        }
    }
    vis.visit_span(span);

    smallvec![item]
}

pub fn noop_flat_map_impl_item<T: MutVisitor>(mut item: ImplItem, visitor: &mut T)
                                              -> SmallVec<[ImplItem; 1]>
{
    let ImplItem { id, ident, vis, defaultness: _, attrs, generics, node, span, tokens: _ } =
        &mut item;
    visitor.visit_id(id);
    visitor.visit_ident(ident);
    visitor.visit_vis(vis);
    visit_attrs(attrs, visitor);
    visitor.visit_generics(generics);
    match node  {
        ImplItemKind::Const(ty, expr) => {
            visitor.visit_ty(ty);
            visitor.visit_expr(expr);
        }
        ImplItemKind::Method(sig, body) => {
            visit_method_sig(sig, visitor);
            visitor.visit_block(body);
        }
        ImplItemKind::Type(ty) => visitor.visit_ty(ty),
        ImplItemKind::Existential(bounds) => visit_bounds(bounds, visitor),
        ImplItemKind::Macro(mac) => visitor.visit_mac(mac),
    }
    visitor.visit_span(span);

    smallvec![item]
}

pub fn noop_visit_fn_header<T: MutVisitor>(header: &mut FnHeader, vis: &mut T) {
    let FnHeader { unsafety: _, asyncness, constness: _, abi: _ } = header;
    vis.visit_asyncness(&mut asyncness.node);
}

pub fn noop_visit_mod<T: MutVisitor>(Mod { inner, items, inline: _ }: &mut Mod, vis: &mut T) {
    vis.visit_span(inner);
    items.flat_map_in_place(|item| vis.flat_map_item(item));
}

pub fn noop_visit_crate<T: MutVisitor>(krate: &mut Crate, vis: &mut T) {
    visit_clobber(krate, |Crate { module, attrs, span }| {
        let item = P(Item {
            ident: Ident::invalid(),
            attrs,
            id: DUMMY_NODE_ID,
            vis: respan(span.shrink_to_lo(), VisibilityKind::Public),
            span,
            node: ItemKind::Mod(module),
            tokens: None,
        });
        let items = vis.flat_map_item(item);

        let len = items.len();
        if len == 0 {
            let module = Mod { inner: span, items: vec![], inline: true };
            Crate { module, attrs: vec![], span }
        } else if len == 1 {
            let Item { attrs, span, node, .. } = items.into_iter().next().unwrap().into_inner();
            match node {
                ItemKind::Mod(module) => Crate { module, attrs, span },
                _ => panic!("visitor converted a module to not a module"),
            }
        } else {
            panic!("a crate cannot expand to more than one item");
        }
    });
}

// Mutate one item into possibly many items.
pub fn noop_flat_map_item<T: MutVisitor>(mut item: P<Item>, visitor: &mut T)
                                         -> SmallVec<[P<Item>; 1]> {
    let Item { ident, attrs, id, node, vis, span, tokens: _ } = item.deref_mut();
    visitor.visit_ident(ident);
    visit_attrs(attrs, visitor);
    visitor.visit_id(id);
    visitor.visit_item_kind(node);
    visitor.visit_vis(vis);
    visitor.visit_span(span);

    // FIXME: if `tokens` is modified with a call to `vis.visit_tts` it causes
    //        an ICE during resolve... odd!

    smallvec![item]
}

pub fn noop_flat_map_foreign_item<T: MutVisitor>(mut item: ForeignItem, visitor: &mut T)
    -> SmallVec<[ForeignItem; 1]>
{
    let ForeignItem { ident, attrs, node, id, span, vis } = &mut item;
    visitor.visit_ident(ident);
    visit_attrs(attrs, visitor);
    match node {
        ForeignItemKind::Fn(fdec, generics) => {
            visitor.visit_fn_decl(fdec);
            visitor.visit_generics(generics);
        }
        ForeignItemKind::Static(t, _m) => visitor.visit_ty(t),
        ForeignItemKind::Ty => {}
        ForeignItemKind::Macro(mac) => visitor.visit_mac(mac),
    }
    visitor.visit_id(id);
    visitor.visit_span(span);
    visitor.visit_vis(vis);

    smallvec![item]
}

pub fn noop_visit_pat<T: MutVisitor>(pat: &mut P<Pat>, vis: &mut T) {
    let Pat { id, node, span } = pat.deref_mut();
    vis.visit_id(id);
    match node {
        PatKind::Wild => {}
        PatKind::Ident(_binding_mode, ident, sub) => {
            vis.visit_ident(ident);
            visit_opt(sub, |sub| vis.visit_pat(sub));
        }
        PatKind::Lit(e) => vis.visit_expr(e),
        PatKind::TupleStruct(path, pats, _ddpos) => {
            vis.visit_path(path);
            visit_vec(pats, |pat| vis.visit_pat(pat));
        }
        PatKind::Path(qself, path) => {
            vis.visit_qself(qself);
            vis.visit_path(path);
        }
        PatKind::Struct(path, fields, _etc) => {
            vis.visit_path(path);
            for Spanned { node: FieldPat { ident, pat, is_shorthand: _, attrs }, span } in fields {
                vis.visit_ident(ident);
                vis.visit_pat(pat);
                visit_thin_attrs(attrs, vis);
                vis.visit_span(span);
            };
        }
        PatKind::Tuple(elts, _ddpos) => visit_vec(elts, |elt| vis.visit_pat(elt)),
        PatKind::Box(inner) => vis.visit_pat(inner),
        PatKind::Ref(inner, _mutbl) => vis.visit_pat(inner),
        PatKind::Range(e1, e2, Spanned { span: _, node: _ }) => {
            vis.visit_expr(e1);
            vis.visit_expr(e2);
            vis.visit_span(span);
        }
        PatKind::Slice(before, slice, after) => {
            visit_vec(before, |pat| vis.visit_pat(pat));
            visit_opt(slice, |slice| vis.visit_pat(slice));
            visit_vec(after, |pat| vis.visit_pat(pat));
        }
        PatKind::Paren(inner) => vis.visit_pat(inner),
        PatKind::Mac(mac) => vis.visit_mac(mac),
    }
    vis.visit_span(span);
}

pub fn noop_visit_anon_const<T: MutVisitor>(AnonConst { id, value }: &mut AnonConst, vis: &mut T) {
    vis.visit_id(id);
    vis.visit_expr(value);
}

pub fn noop_visit_expr<T: MutVisitor>(Expr { node, id, span, attrs }: &mut Expr, vis: &mut T) {
    match node {
        ExprKind::Box(expr) => vis.visit_expr(expr),
        ExprKind::Array(exprs) => visit_exprs(exprs, vis),
        ExprKind::Repeat(expr, count) => {
            vis.visit_expr(expr);
            vis.visit_anon_const(count);
        }
        ExprKind::Tup(exprs) => visit_exprs(exprs, vis),
        ExprKind::Call(f, args) => {
            vis.visit_expr(f);
            visit_exprs(args, vis);
        }
        ExprKind::MethodCall(PathSegment { ident, id, args }, exprs) => {
            vis.visit_ident(ident);
            vis.visit_id(id);
            visit_opt(args, |args| vis.visit_generic_args(args));
            visit_exprs(exprs, vis);
        }
        ExprKind::Binary(_binop, lhs, rhs) => {
            vis.visit_expr(lhs);
            vis.visit_expr(rhs);
        }
        ExprKind::Unary(_unop, ohs) => vis.visit_expr(ohs),
        ExprKind::Cast(expr, ty) => {
            vis.visit_expr(expr);
            vis.visit_ty(ty);
        }
        ExprKind::Type(expr, ty) => {
            vis.visit_expr(expr);
            vis.visit_ty(ty);
        }
        ExprKind::AddrOf(_m, ohs) => vis.visit_expr(ohs),
        ExprKind::Let(pats, scrutinee) => {
            visit_vec(pats, |pat| vis.visit_pat(pat));
            vis.visit_expr(scrutinee);
        }
        ExprKind::If(cond, tr, fl) => {
            vis.visit_expr(cond);
            vis.visit_block(tr);
            visit_opt(fl, |fl| vis.visit_expr(fl));
        }
        ExprKind::While(cond, body, label) => {
            vis.visit_expr(cond);
            vis.visit_block(body);
            visit_opt(label, |label| vis.visit_label(label));
        }
        ExprKind::ForLoop(pat, iter, body, label) => {
            vis.visit_pat(pat);
            vis.visit_expr(iter);
            vis.visit_block(body);
            visit_opt(label, |label| vis.visit_label(label));
        }
        ExprKind::Loop(body, label) => {
            vis.visit_block(body);
            visit_opt(label, |label| vis.visit_label(label));
        }
        ExprKind::Match(expr, arms) => {
            vis.visit_expr(expr);
            visit_vec(arms, |arm| vis.visit_arm(arm));
        }
        ExprKind::Closure(_capture_by, asyncness, _movability, decl, body, span) => {
            vis.visit_asyncness(asyncness);
            vis.visit_fn_decl(decl);
            vis.visit_expr(body);
            vis.visit_span(span);
        }
        ExprKind::Block(blk, label) => {
            vis.visit_block(blk);
            visit_opt(label, |label| vis.visit_label(label));
        }
        ExprKind::Async(_capture_by, node_id, body) => {
            vis.visit_id(node_id);
            vis.visit_block(body);
        }
        ExprKind::Await(_origin, expr) => vis.visit_expr(expr),
        ExprKind::Assign(el, er) => {
            vis.visit_expr(el);
            vis.visit_expr(er);
        }
        ExprKind::AssignOp(_op, el, er) => {
            vis.visit_expr(el);
            vis.visit_expr(er);
        }
        ExprKind::Field(el, ident) => {
            vis.visit_expr(el);
            vis.visit_ident(ident);
        }
        ExprKind::Index(el, er) => {
            vis.visit_expr(el);
            vis.visit_expr(er);
        }
        ExprKind::Range(e1, e2, _lim) => {
            visit_opt(e1, |e1| vis.visit_expr(e1));
            visit_opt(e2, |e2| vis.visit_expr(e2));
        }
        ExprKind::Path(qself, path) => {
            vis.visit_qself(qself);
            vis.visit_path(path);
        }
        ExprKind::Break(label, expr) => {
            visit_opt(label, |label| vis.visit_label(label));
            visit_opt(expr, |expr| vis.visit_expr(expr));
        }
        ExprKind::Continue(label) => {
            visit_opt(label, |label| vis.visit_label(label));
        }
        ExprKind::Ret(expr) => {
            visit_opt(expr, |expr| vis.visit_expr(expr));
        }
        ExprKind::InlineAsm(asm) => {
            let InlineAsm { asm: _, asm_str_style: _, outputs, inputs, clobbers: _, volatile: _,
                            alignstack: _, dialect: _, ctxt: _ } = asm.deref_mut();
            for out in outputs {
                let InlineAsmOutput { constraint: _, expr, is_rw: _, is_indirect: _ } = out;
                vis.visit_expr(expr);
            }
            visit_vec(inputs, |(_c, expr)| vis.visit_expr(expr));
        }
        ExprKind::Mac(mac) => vis.visit_mac(mac),
        ExprKind::Struct(path, fields, expr) => {
            vis.visit_path(path);
            visit_vec(fields, |field| vis.visit_field(field));
            visit_opt(expr, |expr| vis.visit_expr(expr));
        },
        ExprKind::Paren(expr) => {
            vis.visit_expr(expr);

            // Nodes that are equal modulo `Paren` sugar no-ops should have the same ids.
            *id = expr.id;
            vis.visit_span(span);
            visit_thin_attrs(attrs, vis);
            return;
        }
        ExprKind::Yield(expr) => {
            visit_opt(expr, |expr| vis.visit_expr(expr));
        }
        ExprKind::Try(expr) => vis.visit_expr(expr),
        ExprKind::TryBlock(body) => vis.visit_block(body),
        ExprKind::Lit(_) | ExprKind::Err => {}
    }
    vis.visit_id(id);
    vis.visit_span(span);
    visit_thin_attrs(attrs, vis);
}

pub fn noop_filter_map_expr<T: MutVisitor>(mut e: P<Expr>, vis: &mut T) -> Option<P<Expr>> {
    Some({ vis.visit_expr(&mut e); e })
}

pub fn noop_flat_map_stmt<T: MutVisitor>(Stmt { node, mut span, mut id }: Stmt, vis: &mut T)
    -> SmallVec<[Stmt; 1]>
{
    vis.visit_id(&mut id);
    vis.visit_span(&mut span);
    noop_flat_map_stmt_kind(node, vis).into_iter().map(|node| {
        Stmt { id, node, span }
    }).collect()
}

pub fn noop_flat_map_stmt_kind<T: MutVisitor>(node: StmtKind, vis: &mut T)
                                              -> SmallVec<[StmtKind; 1]> {
    match node {
        StmtKind::Local(mut local) =>
            smallvec![StmtKind::Local({ vis.visit_local(&mut local); local })],
        StmtKind::Item(item) => vis.flat_map_item(item).into_iter().map(StmtKind::Item).collect(),
        StmtKind::Expr(expr) => {
            vis.filter_map_expr(expr).into_iter().map(StmtKind::Expr).collect()
        }
        StmtKind::Semi(expr) => {
            vis.filter_map_expr(expr).into_iter().map(StmtKind::Semi).collect()
        }
        StmtKind::Mac(mut mac) => {
            let (mac_, _semi, attrs) = mac.deref_mut();
            vis.visit_mac(mac_);
            visit_thin_attrs(attrs, vis);
            smallvec![StmtKind::Mac(mac)]
        }
    }
}

pub fn noop_visit_vis<T: MutVisitor>(Spanned { node, span }: &mut Visibility, vis: &mut T) {
    match node {
        VisibilityKind::Public | VisibilityKind::Crate(_) | VisibilityKind::Inherited => {}
        VisibilityKind::Restricted { path, id } => {
            vis.visit_path(path);
            vis.visit_id(id);
        }
    }
    vis.visit_span(span);
}

#[cfg(test)]
mod tests {
    use crate::ast::{self, Ident};
    use crate::util::parser_testing::{string_to_crate, matches_codepattern};
    use crate::print::pprust;
    use crate::mut_visit;
    use crate::with_default_globals;
    use super::*;

    // this version doesn't care about getting comments or docstrings in.
    fn fake_print_crate(s: &mut pprust::State<'_>,
                        krate: &ast::Crate) {
        s.print_mod(&krate.module, &krate.attrs)
    }

    // change every identifier to "zz"
    struct ToZzIdentMutVisitor;

    impl MutVisitor for ToZzIdentMutVisitor {
        fn visit_ident(&mut self, ident: &mut ast::Ident) {
            *ident = Ident::from_str("zz");
        }
        fn visit_mac(&mut self, mac: &mut ast::Mac) {
            mut_visit::noop_visit_mac(mac, self)
        }
    }

    // maybe add to expand.rs...
    macro_rules! assert_pred {
        ($pred:expr, $predname:expr, $a:expr , $b:expr) => (
            {
                let pred_val = $pred;
                let a_val = $a;
                let b_val = $b;
                if !(pred_val(&a_val, &b_val)) {
                    panic!("expected args satisfying {}, got {} and {}",
                          $predname, a_val, b_val);
                }
            }
        )
    }

    // make sure idents get transformed everywhere
    #[test] fn ident_transformation () {
        with_default_globals(|| {
            let mut zz_visitor = ToZzIdentMutVisitor;
            let mut krate = string_to_crate(
                "#[a] mod b {fn c (d : e, f : g) {h!(i,j,k);l;m}}".to_string());
            zz_visitor.visit_crate(&mut krate);
            assert_pred!(
                matches_codepattern,
                "matches_codepattern",
                pprust::to_string(|s| fake_print_crate(s, &krate)),
                "#[zz]mod zz{fn zz(zz:zz,zz:zz){zz!(zz,zz,zz);zz;zz}}".to_string());
        })
    }

    // even inside macro defs....
    #[test] fn ident_transformation_in_defs () {
        with_default_globals(|| {
            let mut zz_visitor = ToZzIdentMutVisitor;
            let mut krate = string_to_crate(
                "macro_rules! a {(b $c:expr $(d $e:token)f+ => \
                (g $(d $d $e)+))} ".to_string());
            zz_visitor.visit_crate(&mut krate);
            assert_pred!(
                matches_codepattern,
                "matches_codepattern",
                pprust::to_string(|s| fake_print_crate(s, &krate)),
                "macro_rules! zz((zz$zz:zz$(zz $zz:zz)zz+=>(zz$(zz$zz$zz)+)));".to_string());
        })
    }
}
