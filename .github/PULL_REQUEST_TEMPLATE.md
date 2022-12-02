Thank you for making Rust GCC better!

If your PR fixes an issue, you can add "Fixes #issue_number" into this
PR description and the git commit message. This way the issue will be
automatically closed when your PR is merged. If your change addresses
an issue but does not fully fix it please mark it as "Addresses #issue_number"
in the git commit message.

Here is a checklist to help you with your PR.

- \[ ] GCC development requires copyright assignment or the Developer's Certificate of Origin sign-off, see https://gcc.gnu.org/contribute.html or https://gcc.gnu.org/dco.html
- \[ ] Read contributing guidlines
- \[ ] `make check-rust` passes locally
- \[ ] Run `clang-format`
- \[ ] Added any relevant test cases to `gcc/testsuite/rust/`

Note that you can skip the above if you are just opening a WIP PR in
order to get feedback.
---

*Please write a comment explaining your change. This is the message
that will be part of the merge commit.
