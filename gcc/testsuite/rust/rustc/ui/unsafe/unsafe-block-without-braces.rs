fn main() {
    unsafe //{
        std::mem::transmute::<f32, u32>(1.0);
    //}
}
// { dg-error "" "" { target *-*-* } .-3 }

