/**
This module implements a singly-linked list container.
It can be used as a stack.

This module is a submodule of $(MREF std, container).

Source: $(PHOBOSSRC std/container/slist.d)

Copyright: 2010- Andrei Alexandrescu. All rights reserved by the respective holders.

License: Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at $(HTTP
boost.org/LICENSE_1_0.txt)).

Authors: $(HTTP erdani.com, Andrei Alexandrescu)

$(SCRIPT inhibitQuickIndex = 1;)
*/
module std.container.slist;

///
@safe unittest
{
    import std.algorithm.comparison : equal;
    import std.container : SList;

    auto s = SList!int(1, 2, 3);
    assert(equal(s[], [1, 2, 3]));

    s.removeFront();
    assert(equal(s[], [2, 3]));

    s.insertFront([5, 6]);
    assert(equal(s[], [5, 6, 2, 3]));

    // If you want to apply range operations, simply slice it.
    import std.algorithm.searching : countUntil;
    import std.range : popFrontN, walkLength;

    auto sl = SList!int(1, 2, 3, 4, 5);
    assert(countUntil(sl[], 2) == 1);

    auto r = sl[];
    popFrontN(r, 2);
    assert(walkLength(r) == 3);
}

public import std.container.util;

/**
   Implements a simple and fast singly-linked list.
   It can be used as a stack.

   `SList` uses reference semantics.
 */
struct SList(T)
if (!is(T == shared))
{
    import std.exception : enforce;
    import std.range : Take;
    import std.range.primitives : isInputRange, isForwardRange, ElementType;
    import std.traits : isImplicitlyConvertible;

    private struct Node
    {
        Node * _next;
        T _payload;
    }
    private struct NodeWithoutPayload
    {
        Node* _next;
    }
    static assert(NodeWithoutPayload._next.offsetof == Node._next.offsetof);

    private Node * _root;

    private void initialize() @trusted nothrow pure
    {
        if (_root) return;
        _root = cast (Node*) new NodeWithoutPayload();
    }

    private ref inout(Node*) _first() @property @safe nothrow pure inout
    {
        assert(_root, "root pointer must not be null");
        return _root._next;
    }

    private static Node * findLastNode(Node * n)
    {
        assert(n, "Node n pointer must not be null");
        auto ahead = n._next;
        while (ahead)
        {
            n = ahead;
            ahead = n._next;
        }
        return n;
    }

    private static Node * findLastNode(Node * n, size_t limit)
    {
        assert(n, "Node n pointer must not be null");
        assert(limit, "limit must be greater than 0");
        auto ahead = n._next;
        while (ahead)
        {
            if (!--limit) break;
            n = ahead;
            ahead = n._next;
        }
        return n;
    }

    private static Node * findNode(Node * n, Node * findMe)
    {
        assert(n, "Node n pointer must not be null");
        auto ahead = n._next;
        while (ahead != findMe)
        {
            n = ahead;
            enforce(n);
            ahead = n._next;
        }
        return n;
    }

    private static Node* findNodeByValue(Node* n, T value)
    {
        if (!n) return null;
        auto ahead = n._next;
        while (ahead && ahead._payload != value)
        {
            n = ahead;
            ahead = n._next;
        }
        return n;
    }

    private static auto createNodeChain(Stuff)(Stuff stuff)
    if (isImplicitlyConvertible!(Stuff, T))
    {
        import std.range : only;
        return createNodeChain(only(stuff));
    }

    private static auto createNodeChain(Stuff)(Stuff stuff)
    if (isInputRange!Stuff && isImplicitlyConvertible!(ElementType!Stuff, T))
    {
        static struct Chain
        {
            Node* first;
            Node* last;
            size_t length;
        }

        Chain ch;

        foreach (item; stuff)
        {
            auto newNode = new Node(null, item);
            (ch.first ? ch.last._next : ch.first) = newNode;
            ch.last = newNode;
            ++ch.length;
        }

        return ch;
    }

    private static size_t insertAfterNode(Stuff)(Node* n, Stuff stuff)
    {
        auto ch = createNodeChain(stuff);

        if (!ch.length) return 0;

        ch.last._next = n._next;
        n._next = ch.first;

        return ch.length;
    }

/**
Constructor taking a number of nodes
     */
    this(U)(U[] values...)
    if (isImplicitlyConvertible!(U, T))
    {
        insertFront(values);
    }

/**
Constructor taking an $(REF_ALTTEXT input range, isInputRange, std,range,primitives)
     */
    this(Stuff)(Stuff stuff)
    if (isInputRange!Stuff
            && isImplicitlyConvertible!(ElementType!Stuff, T)
            && !is(Stuff == T[]))
    {
        insertFront(stuff);
    }

/**
Comparison for equality.

Complexity: $(BIGOH min(n, n1)) where `n1` is the number of
elements in `rhs`.
     */
    bool opEquals(const SList rhs) const
    {
        return opEquals(rhs);
    }

    /// ditto
    bool opEquals(ref const SList rhs) const
    {
        if (_root is rhs._root) return true;
        if (_root is null) return rhs._root is null || rhs._first is null;
        if (rhs._root is null) return _root is null || _first is null;

        const(Node) * n1 = _first, n2 = rhs._first;

        for (;; n1 = n1._next, n2 = n2._next)
        {
            if (!n1) return !n2;
            if (!n2 || n1._payload != n2._payload) return false;
        }
    }

/**
Defines the container's primary range, which embodies a forward range.
     */
    struct Range
    {
        private Node * _head;
        private this(Node * p) { _head = p; }

        /// Input range primitives.
        @property bool empty() const { return !_head; }

        /// ditto
        @property ref T front()
        {
            assert(!empty, "SList.Range.front: Range is empty");
            return _head._payload;
        }

        /// ditto
        void popFront()
        {
            assert(!empty, "SList.Range.popFront: Range is empty");
            _head = _head._next;
        }

        /// Forward range primitive.
        @property Range save() { return this; }

        T moveFront()
        {
            import std.algorithm.mutation : move;

            assert(!empty, "SList.Range.moveFront: Range is empty");
            return move(_head._payload);
        }

        bool sameHead(Range rhs)
        {
            return _head && _head == rhs._head;
        }
    }

    @safe unittest
    {
        static assert(isForwardRange!Range);
    }

/**
Property returning `true` if and only if the container has no
elements.

Complexity: $(BIGOH 1)
     */
    @property bool empty() const
    {
        return _root is null || _first is null;
    }

/**
Duplicates the container. The elements themselves are not transitively
duplicated.

Complexity: $(BIGOH n).
     */
    @property SList dup()
    {
        return SList(this[]);
    }

/**
Returns a range that iterates over all elements of the container, in
forward order.

Complexity: $(BIGOH 1)
     */
    Range opSlice()
    {
        if (empty)
            return Range(null);
        else
            return Range(_first);
    }

/**
Forward to `opSlice().front`.

Complexity: $(BIGOH 1)
     */
    @property ref T front()
    {
        assert(!empty, "SList.front: List is empty");
        return _first._payload;
    }

    @safe unittest
    {
        auto s = SList!int(1, 2, 3);
        s.front = 42;
        assert(s == SList!int(42, 2, 3));
    }

/**
Returns a new `SList` that's the concatenation of `this` and its
argument. `opBinaryRight` is only defined if `Stuff` does not
define `opBinary`.
     */
    SList opBinary(string op, Stuff)(Stuff rhs)
    if (op == "~" && is(typeof(SList(rhs))))
    {
        import std.range : chain, only;

        static if (isInputRange!Stuff)
            alias r = rhs;
        else
            auto r = only(rhs);

        return SList(this[].chain(r));
    }

    /// ditto
    SList opBinaryRight(string op, Stuff)(Stuff lhs)
    if (op == "~" && !is(typeof(lhs.opBinary!"~"(this))) && is(typeof(SList(lhs))))
    {
        import std.range : chain, only;

        static if (isInputRange!Stuff)
            alias r = lhs;
        else
            auto r = only(lhs);

        return SList(r.chain(this[]));
    }

/**
Removes all contents from the `SList`.

Postcondition: `empty`

Complexity: $(BIGOH 1)
     */
    void clear()
    {
        if (_root)
            _first = null;
    }

/**
Reverses SList in-place. Performs no memory allocation.

Complexity: $(BIGOH n)
     */
    void reverse()
    {
        if (!empty)
        {
            Node* prev;
            while (_first)
            {
                auto next = _first._next;
                _first._next = prev;
                prev = _first;
                _first = next;
            }
            _first = prev;
        }
    }

/**
Inserts `stuff` to the front of the container. `stuff` can be a
value convertible to `T` or a range of objects convertible to $(D
T). The stable version behaves the same, but guarantees that ranges
iterating over the container are never invalidated.

Returns: The number of elements inserted

Complexity: $(BIGOH m), where `m` is the length of `stuff`
     */
    size_t insertFront(Stuff)(Stuff stuff)
    if (isInputRange!Stuff || isImplicitlyConvertible!(Stuff, T))
    {
        initialize();
        return insertAfterNode(_root, stuff);
    }

    /// ditto
    alias insert = insertFront;

    /// ditto
    alias stableInsert = insert;

    /// ditto
    alias stableInsertFront = insertFront;

/**
Picks one value in an unspecified position in the container, removes
it from the container, and returns it. The stable version behaves the same,
but guarantees that ranges iterating over the container are never invalidated.

Precondition: `!empty`

Returns: The element removed.

Complexity: $(BIGOH 1).
     */
    T removeAny()
    {
        import std.algorithm.mutation : move;

        assert(!empty, "SList.removeAny: List is empty");
        auto result = move(_first._payload);
        _first = _first._next;
        return result;
    }
    /// ditto
    alias stableRemoveAny = removeAny;

/**
Removes the value at the front of the container. The stable version
behaves the same, but guarantees that ranges iterating over the
container are never invalidated.

Precondition: `!empty`

Complexity: $(BIGOH 1).
     */
    void removeFront()
    {
        assert(!empty, "SList.removeFront: List is empty");
        _first = _first._next;
    }

    /// ditto
    alias stableRemoveFront = removeFront;

/**
Removes `howMany` values at the front or back of the
container. Unlike the unparameterized versions above, these functions
do not throw if they could not remove `howMany` elements. Instead,
if $(D howMany > n), all elements are removed. The returned value is
the effective number of elements removed. The stable version behaves
the same, but guarantees that ranges iterating over the container are
never invalidated.

Returns: The number of elements removed

Complexity: $(BIGOH howMany * log(n)).
     */
    size_t removeFront(size_t howMany)
    {
        size_t result;
        while (_first && result < howMany)
        {
            _first = _first._next;
            ++result;
        }
        return result;
    }

    /// ditto
    alias stableRemoveFront = removeFront;

/**
Inserts `stuff` after range `r`, which must be a range
previously extracted from this container. Given that all ranges for a
list end at the end of the list, this function essentially appends to
the list and uses `r` as a potentially fast way to reach the last
node in the list. Ideally `r` is positioned near or at the last
element of the list.

`stuff` can be a value convertible to `T` or a range of objects
convertible to `T`. The stable version behaves the same, but
guarantees that ranges iterating over the container are never
invalidated.

Returns: The number of values inserted.

Complexity: $(BIGOH k + m), where `k` is the number of elements in
`r` and `m` is the length of `stuff`.

Example:
--------------------
auto sl = SList!string(["a", "b", "d"]);
sl.insertAfter(sl[], "e"); // insert at the end (slowest)
assert(std.algorithm.equal(sl[], ["a", "b", "d", "e"]));
sl.insertAfter(std.range.take(sl[], 2), "c"); // insert after "b"
assert(std.algorithm.equal(sl[], ["a", "b", "c", "d", "e"]));
--------------------
     */

    size_t insertAfter(Stuff)(Range r, Stuff stuff)
    if (isInputRange!Stuff || isImplicitlyConvertible!(Stuff, T))
    {
        initialize();
        if (!_first)
        {
            enforce(!r._head);
            return insertFront(stuff);
        }
        enforce(r._head);
        auto n = findLastNode(r._head);
        return insertAfterNode(n, stuff);
    }

/**
Similar to `insertAfter` above, but accepts a range bounded in
count. This is important for ensuring fast insertions in the middle of
the list.  For fast insertions after a specified position `r`, use
$(D insertAfter(take(r, 1), stuff)). The complexity of that operation
only depends on the number of elements in `stuff`.

Precondition: $(D r.original.empty || r.maxLength > 0)

Returns: The number of values inserted.

Complexity: $(BIGOH k + m), where `k` is the number of elements in
`r` and `m` is the length of `stuff`.
     */
    size_t insertAfter(Stuff)(Take!Range r, Stuff stuff)
    if (isInputRange!Stuff || isImplicitlyConvertible!(Stuff, T))
    {
        auto orig = r.source;
        if (!orig._head)
        {
            // Inserting after a null range counts as insertion to the
            // front
            return insertFront(stuff);
        }
        enforce(!r.empty);
        // Find the last valid element in the range
        foreach (i; 1 .. r.maxLength)
        {
            if (!orig._head._next) break;
            orig.popFront();
        }
        // insert here
        return insertAfterNode(orig._head, stuff);
    }

/// ditto
    alias stableInsertAfter = insertAfter;

/**
Removes a range from the list in linear time.

Returns: An empty range.

Complexity: $(BIGOH n)
     */
    Range linearRemove(Range r)
    {
        if (!_first)
        {
            enforce(!r._head);
            return this[];
        }
        auto n = findNode(_root, r._head);
        n._next = null;
        return Range(null);
    }

/**
Removes a `Take!Range` from the list in linear time.

Returns: A range comprehending the elements after the removed range.

Complexity: $(BIGOH n)
     */
    Range linearRemove(Take!Range r)
    {
        auto orig = r.source;
        // We have something to remove here
        if (orig._head == _first)
        {
            // remove straight from the head of the list
            for (; !r.empty; r.popFront())
            {
                removeFront();
            }
            return this[];
        }
        if (!r.maxLength)
        {
            // Nothing to remove, return the range itself
            return orig;
        }
        // Remove from somewhere in the middle of the list
        enforce(_first);
        auto n1 = findNode(_root, orig._head);
        auto n2 = findLastNode(orig._head, r.maxLength);
        n1._next = n2._next;
        return Range(n1._next);
    }

/// ditto
    alias stableLinearRemove = linearRemove;

/**
Removes the first occurence of an element from the list in linear time.

Returns: True if the element existed and was successfully removed, false otherwise.

Params:
    value = value of the node to be removed

Complexity: $(BIGOH n)
     */
    bool linearRemoveElement(T value)
    {
        auto n1 = findNodeByValue(_root, value);

        if (n1 && n1._next)
        {
            n1._next = n1._next._next;
            return true;
        }

        return false;
    }
}

@safe unittest
{
    import std.algorithm.comparison : equal;

    auto e = SList!int();
    auto b = e.linearRemoveElement(2);
    assert(b == false);
    assert(e.empty());
    auto a = SList!int(-1, 1, 2, 1, 3, 4);
    b = a.linearRemoveElement(1);
    assert(equal(a[], [-1, 2, 1, 3, 4]));
    assert(b == true);
    b = a.linearRemoveElement(-1);
    assert(b == true);
    assert(equal(a[], [2, 1, 3, 4]));
    b = a.linearRemoveElement(1);
    assert(b == true);
    assert(equal(a[], [2, 3, 4]));
    b = a.linearRemoveElement(2);
    assert(b == true);
    b = a.linearRemoveElement(20);
    assert(b == false);
    assert(equal(a[], [3, 4]));
    b = a.linearRemoveElement(4);
    assert(b == true);
    assert(equal(a[], [3]));
    b = a.linearRemoveElement(3);
    assert(b == true);
    assert(a.empty());
    a.linearRemoveElement(3);
}

@safe unittest
{
    import std.algorithm.comparison : equal;

    auto a = SList!int(5);
    auto b = a;
    auto r = a[];
    a.insertFront(1);
    b.insertFront(2);
    assert(equal(a[], [2, 1, 5]));
    assert(equal(b[], [2, 1, 5]));
    r.front = 9;
    assert(equal(a[], [2, 1, 9]));
    assert(equal(b[], [2, 1, 9]));
}

@safe unittest
{
    auto s = SList!int(1, 2, 3);
    auto n = s.findLastNode(s._root);
    assert(n && n._payload == 3);
}

@safe unittest
{
    import std.range.primitives;
    auto s = SList!int(1, 2, 5, 10);
    assert(walkLength(s[]) == 4);
}

@safe unittest
{
    import std.range : take;
    auto src = take([0, 1, 2, 3], 3);
    auto s = SList!int(src);
    assert(s == SList!int(0, 1, 2));
}

@safe unittest
{
    auto a = SList!int();
    auto b = SList!int();
    auto c = a ~ b[];
    assert(c.empty);
}

@safe unittest
{
    auto a = SList!int(1, 2, 3);
    auto b = SList!int(4, 5, 6);
    auto c = a ~ b[];
    assert(c == SList!int(1, 2, 3, 4, 5, 6));
}

@safe unittest
{
    auto a = SList!int(1, 2, 3);
    auto b = [4, 5, 6];
    auto c = a ~ b;
    assert(c == SList!int(1, 2, 3, 4, 5, 6));
}

@safe unittest
{
    auto a = SList!int(1, 2, 3);
    auto c = a ~ 4;
    assert(c == SList!int(1, 2, 3, 4));
}

@safe unittest
{
    auto a = SList!int(2, 3, 4);
    auto b = 1 ~ a;
    assert(b == SList!int(1, 2, 3, 4));
}

@safe unittest
{
    auto a = [1, 2, 3];
    auto b = SList!int(4, 5, 6);
    auto c = a ~ b;
    assert(c == SList!int(1, 2, 3, 4, 5, 6));
}

@safe unittest
{
    auto s = SList!int(1, 2, 3, 4);
    s.insertFront([ 42, 43 ]);
    assert(s == SList!int(42, 43, 1, 2, 3, 4));
}

@safe unittest
{
    auto s = SList!int(1, 2, 3);
    assert(s.removeAny() == 1);
    assert(s == SList!int(2, 3));
    assert(s.stableRemoveAny() == 2);
    assert(s == SList!int(3));
}

@safe unittest
{
    import std.algorithm.comparison : equal;

    auto s = SList!int(1, 2, 3);
    s.removeFront();
    assert(equal(s[], [2, 3]));
    s.stableRemoveFront();
    assert(equal(s[], [3]));
}

@safe unittest
{
    auto s = SList!int(1, 2, 3, 4, 5, 6, 7);
    assert(s.removeFront(3) == 3);
    assert(s == SList!int(4, 5, 6, 7));
}

@safe unittest
{
    auto a = SList!int(1, 2, 3);
    auto b = SList!int(1, 2, 3);
    assert(a.insertAfter(a[], b[]) == 3);
}

@safe unittest
{
    import std.range : take;
    auto s = SList!int(1, 2, 3, 4);
    auto r = take(s[], 2);
    assert(s.insertAfter(r, 5) == 1);
    assert(s == SList!int(1, 2, 5, 3, 4));
}

@safe unittest
{
    import std.algorithm.comparison : equal;
    import std.range : take;

    // insertAfter documentation example
    auto sl = SList!string(["a", "b", "d"]);
    sl.insertAfter(sl[], "e"); // insert at the end (slowest)
    assert(equal(sl[], ["a", "b", "d", "e"]));
    sl.insertAfter(take(sl[], 2), "c"); // insert after "b"
    assert(equal(sl[], ["a", "b", "c", "d", "e"]));
}

@safe unittest
{
    import std.range.primitives;
    auto s = SList!int(1, 2, 3, 4, 5);
    auto r = s[];
    popFrontN(r, 3);
    auto r1 = s.linearRemove(r);
    assert(s == SList!int(1, 2, 3));
    assert(r1.empty);
}

@safe unittest
{
    auto s = SList!int(1, 2, 3, 4, 5);
    auto r = s[];
    auto r1 = s.linearRemove(r);
    assert(s == SList!int());
    assert(r1.empty);
}

@safe unittest
{
    import std.algorithm.comparison : equal;
    import std.range;

    auto s = SList!int(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    auto r = s[];
    popFrontN(r, 3);
    auto r1 = take(r, 4);
    assert(equal(r1, [4, 5, 6, 7]));
    auto r2 = s.linearRemove(r1);
    assert(s == SList!int(1, 2, 3, 8, 9, 10));
    assert(equal(r2, [8, 9, 10]));
}

@safe unittest
{
    import std.range.primitives;
    auto lst = SList!int(1, 5, 42, 9);
    assert(!lst.empty);
    assert(lst.front == 1);
    assert(walkLength(lst[]) == 4);

    auto lst2 = lst ~ [ 1, 2, 3 ];
    assert(walkLength(lst2[]) == 7);

    auto lst3 = lst ~ [ 7 ];
    assert(walkLength(lst3[]) == 5);
}

@safe unittest
{
    auto s = make!(SList!int)(1, 2, 3);
}

// https://issues.dlang.org/show_bug.cgi?id=5193
@safe unittest
{
    static struct Data
    {
        const int val;
    }
    SList!Data list;
}

@safe unittest
{
    auto s = SList!int([1, 2, 3]);
    s.front = 5; //test frontAssign
    assert(s.front == 5);
    auto r = s[];
    r.front = 1; //test frontAssign
    assert(r.front == 1);
}

// https://issues.dlang.org/show_bug.cgi?id=14920
@safe unittest
{
    SList!int s;
    s.insertAfter(s[], 1);
    assert(s.front == 1);
}

// https://issues.dlang.org/show_bug.cgi?id=15659
@safe unittest
{
    SList!int s;
    s.clear();
}

@safe unittest
{
    SList!int s;
    s.reverse();
}

@safe unittest
{
    import std.algorithm.comparison : equal;

    auto s = SList!int([1, 2, 3]);
    assert(s[].equal([1, 2, 3]));

    s.reverse();
    assert(s[].equal([3, 2, 1]));
}

@safe unittest
{
    auto s = SList!int([4, 6, 8, 12, 16]);
    auto d = s.dup;
    assert(d !is s);
    assert(d == s);
}
