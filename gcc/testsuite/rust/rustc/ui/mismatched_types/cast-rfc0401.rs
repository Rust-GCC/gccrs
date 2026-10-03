fn illegal_cast<U:?Sized,V:?Sized>(u: *const U) -> *const V
{
    u as *const V // { dg-error ".E0606." "" { target *-*-* } }
}

fn illegal_cast_2<U:?Sized>(u: *const U) -> *const str
{
    u as *const str // { dg-error ".E0606." "" { target *-*-* } }
}

trait Foo { fn foo(&self) {} }
impl<T> Foo for T {}

trait Bar { fn foo(&self) {} }
impl<T> Bar for T {}

enum E {
    A, B
}

fn main()
{
    let f: f32 = 1.2;
    let v = core::ptr::null::<u8>();
    let fat_v : *const [u8] = unsafe { &*core::ptr::null::<[u8; 1]>()};
    let fat_sv : *const [i8] = unsafe { &*core::ptr::null::<[i8; 1]>()};
    let foo: &dyn Foo = &f;

    let _ = v as &u8; // { dg-error ".E0605." "" { target *-*-* } }
    let _ = v as E; // { dg-error ".E0605." "" { target *-*-* } }
    let _ = v as fn(); // { dg-error ".E0605." "" { target *-*-* } }
    let _ = v as (u32,); // { dg-error ".E0605." "" { target *-*-* } }
    let _ = Some(&v) as *const u8; // { dg-error ".E0605." "" { target *-*-* } }

    let _ = v as f32; // { dg-error ".E0606." "" { target *-*-* } }
    let _ = main as f64; // { dg-error ".E0606." "" { target *-*-* } }
    let _ = &v as usize; // { dg-error ".E0606." "" { target *-*-* } }
    let _ = f as *const u8; // { dg-error ".E0606." "" { target *-*-* } }
    let _ = 3_i32 as bool; // { dg-error ".E0054." "" { target *-*-* } }
    let _ = E::A as bool; // { dg-error ".E0054." "" { target *-*-* } }
    let _ = 0x61u32 as char; // { dg-error ".E0604." "" { target *-*-* } }

    let _ = false as f32; // { dg-error ".E0606." "" { target *-*-* } }
    let _ = E::A as f32; // { dg-error ".E0606." "" { target *-*-* } }
    let _ = 'a' as f32; // { dg-error ".E0606." "" { target *-*-* } }

    let _ = false as *const u8; // { dg-error ".E0606." "" { target *-*-* } }
    let _ = E::A as *const u8; // { dg-error ".E0606." "" { target *-*-* } }
    let _ = 'a' as *const u8; // { dg-error ".E0606." "" { target *-*-* } }

    let _ = 42usize as *const [u8]; // { dg-error ".E0606." "" { target *-*-* } }
    let _ = v as *const [u8]; // { dg-error ".E0607." "" { target *-*-* } }
    let _ = fat_v as *const dyn Foo; // { dg-error ".E0277." "" { target *-*-* } }
    let _ = foo as *const str; // { dg-error ".E0606." "" { target *-*-* } }
    let _ = foo as *mut str; // { dg-error ".E0606." "" { target *-*-* } }
    let _ = main as *mut str; // { dg-error ".E0606." "" { target *-*-* } }
    let _ = &f as *mut f32; // { dg-error ".E0606." "" { target *-*-* } }
    let _ = &f as *const f64; // { dg-error ".E0606." "" { target *-*-* } }
    let _ = fat_sv as usize; // { dg-error ".E0606." "" { target *-*-* } }

    let a : *const str = "hello";
    let _ = a as *const dyn Foo; // { dg-error ".E0277." "" { target *-*-* } }

    // check no error cascade
    let _ = main.f as *const u32; // { dg-error ".E0609." "" { target *-*-* } }

    let cf: *const dyn Foo = &0;
    let _ = cf as *const [u16]; // { dg-error ".E0606." "" { target *-*-* } }
    let _ = cf as *const dyn Bar; // { dg-error ".E0606." "" { target *-*-* } }

    vec![0.0].iter().map(|s| s as f32).collect::<Vec<f32>>(); // { dg-error ".E0606." "" { target *-*-* } }
}

