//! impl bool {}

#[lang = "bool"]
impl bool {
    /// Returns `Some(t)` if the `bool` is `true`, or `None` otherwise.
    ///
    /// # Examples
    ///
    /// ```
    /// #![feature(bool_to_option)]
    ///
    /// assert_eq!(false.then_some(0), None);
    /// assert_eq!(true.then_some(0), Some(0));
    /// ```
    #[unstable(feature = "bool_to_option", issue = "64260")]
    #[inline]
    pub fn then_some<T>(self, t: T) -> Option<T> {
        if self { Some(t) } else { None }
    }

    /// Returns `Some(f())` if the `bool` is `true`, or `None` otherwise.
    ///
    /// # Examples
    ///
    /// ```
    /// #![feature(bool_to_option)]
    ///
    /// assert_eq!(false.then(|| 0), None);
    /// assert_eq!(true.then(|| 0), Some(0));
    /// ```
    #[unstable(feature = "bool_to_option", issue = "64260")]
    #[inline]
    pub fn then<T, F: FnOnce() -> T>(self, f: F) -> Option<T> {
        if self { Some(f()) } else { None }
    }
}
