extern "C" {
    fn printf(s: *const i8, ...);
}

pub fn main() {
    unsafe {
        let value = 10;
        let result = match value {
            10 => 15,
            _ => 20,
        };

        let format = "Result: %d\n\0" as *const str as *const i8;
        printf(format, result);
    }
}
