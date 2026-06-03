#![no_core]
#![feature(no_core, lang_items, box_syntax)]

#[lang = "sized"]
pub trait Sized {}

#[lang = "phantom_data"]
pub struct PhantomData<T: ?Sized>;

pub struct NonNull<T: ?Sized> {
    _pointer: *const T,
}

pub struct Unique<T: ?Sized> {
    _pointer: NonNull<T>,
    _marker: PhantomData<T>,
}

pub struct Global;

#[lang = "exchange_malloc"]
fn _exchange_malloc() -> *mut u8 {
    0 as *mut u8
}

#[lang = "owned_box"]
pub struct Box<T, A = Global>(Unique<T>, A);

impl<T> Box<T> {
    pub fn new(x: T) -> Self {
        box x
    }
}

struct NonCopyStruct {
    id: i32,
}

impl NonCopyStruct {
    fn get_id(&self) -> i32 {
        self.id
    }
}

fn main() {
    let my_box: Box<NonCopyStruct> = Box::new(NonCopyStruct { id: 42 });
    let _moved_val = *my_box;


    let my_box2: Box<NonCopyStruct> = Box::new(NonCopyStruct { id: 100 });

    let _val_id = my_box2.id;

    let _val_method_id = my_box2.get_id();
}
