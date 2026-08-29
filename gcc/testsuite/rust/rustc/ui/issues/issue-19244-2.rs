struct MyStruct { field: usize }
const STRUCT: MyStruct = MyStruct { field: 42 };

fn main() {
    let a: [isize; STRUCT.nonexistent_field];
// { dg-error ".E0609." "" { target *-*-* } .-1 }
}

