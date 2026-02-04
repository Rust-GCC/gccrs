#![feature(no_core)]
#![no_core]
mod framing {
    mod public_message {
        use super::super::*;

        #[derive(Debug)]
        pub struct ConfirmedTranscriptHashInput;
    }

    mod public_message_in {
        use super::*;

        #[derive(Debug)]
        pub struct ConfirmedTranscriptHashInput;
    }

    pub use self::public_message::*;
    pub use self::public_message_in::*;
}

use crate::framing::ConfirmedTranscriptHashInput;
fn main() { }
