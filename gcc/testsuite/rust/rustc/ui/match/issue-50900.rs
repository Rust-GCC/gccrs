#[derive(PartialEq, Eq)]
pub struct Tag(pub Context, pub u16);

#[derive(PartialEq, Eq)]
pub enum Context {
    Tiff,
    Exif,
}

impl Tag {
    const ExifIFDPointer: Tag = Tag(Context::Tiff, 34665);
}

fn main() {
    match Tag::ExifIFDPointer {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        Tag::ExifIFDPointer => {}
    }
}

