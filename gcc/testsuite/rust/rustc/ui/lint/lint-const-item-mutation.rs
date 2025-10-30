// check-pass

struct MyStruct {
    field: bool,
    inner_array: [char; 1],
    raw_ptr: *mut u8
}
impl MyStruct {
    fn use_mut(&mut self) {}
}

struct Mutable {
    msg: &'static str,
}
impl Drop for Mutable {
    fn drop(&mut self) {
        println!("{}", self.msg);
    }
}

struct Mutable2 { // this one has drop glue but not a Drop impl
    msg: &'static str,
    other: String,
}

const ARRAY: [u8; 1] = [25];
const MY_STRUCT: MyStruct = MyStruct { field: true, inner_array: ['a'], raw_ptr: 2 as *mut u8 };
const RAW_PTR: *mut u8 = 1 as *mut u8;
const MUTABLE: Mutable = Mutable { msg: "" };
const MUTABLE2: Mutable2 = Mutable2 { msg: "", other: String::new() };
const VEC: Vec<i32> = Vec::new();
const PTR: *mut () = 1 as *mut _;

fn main() {
    ARRAY[0] = 5; // { dg-warning "" "" { target *-*-* } }
    MY_STRUCT.field = false; // { dg-warning "" "" { target *-*-* } }
    MY_STRUCT.inner_array[0] = 'b'; // { dg-warning "" "" { target *-*-* } }
    MY_STRUCT.use_mut(); // { dg-warning "" "" { target *-*-* } }
    &mut MY_STRUCT; // { dg-warning "" "" { target *-*-* } }
    (&mut MY_STRUCT).use_mut(); // { dg-warning "" "" { target *-*-* } }

    // Test that we don't warn when writing through
    // a raw pointer
    // This is U.B., but this test is check-pass,
    // so this never actually executes
    unsafe {
        *RAW_PTR = 0;
        *MY_STRUCT.raw_ptr = 0;
    }

    MUTABLE.msg = "wow"; // no warning, because Drop observes the mutation
    MUTABLE2.msg = "wow"; // { dg-warning "" "" { target *-*-* } }
    VEC.push(0); // { dg-warning "" "" { target *-*-* } }

    // Test that we don't warn when converting a raw pointer
    // into a mutable reference
    unsafe { &mut *PTR };
}

