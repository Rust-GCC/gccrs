fn main() {
trait Seq { }

impl<T> Seq<T> for Vec<T> { // { dg-error ".E0107." "" { target *-*-* } }
    /* ... */
}
impl Seq<bool> for u32 { // { dg-error ".E0107." "" { target *-*-* } }
   /* Treat the integer as a sequence of bits */
}

}

