@safe unittest
{
    import std.internal.entropy;

    int[4] bytes;
    if (getEntropy(cast(void[]) bytes).isOK)
    {
        // Success; data in `bytes` may be used.
    }

    assert((cast(void[]) bytes).length == bytes.sizeof);
}

@safe unittest
{
    import std.internal.entropy;

    ubyte[16] bytes;
    if (getEntropy(bytes).isOK)
    {
        // Success; data in `bytes` may be used.
    }
}

@system unittest
{
    import std.internal.entropy;

    ubyte[16] bytes;
    if (getEntropy(cast(void*) bytes.ptr, bytes.length).isOK)
    {
        // Success; data in `bytes` may be used.
    }
}

@system unittest
{
    import std.internal.entropy;

    int number = void;
    if (getEntropy(&number, number.sizeof).isOK)
    {
        // Success; value of `number` may be used.
    }
}

@system unittest
{
    import std.internal.entropy;

    ubyte[4] bytes;

    // `EntropySource.none` always fails.
    assert(!getEntropy(bytes.ptr, bytes.length, EntropySource.none).isOK);
}

@safe unittest
{
    import std.internal.entropy;

    ubyte[4] data;
    EntropyResult result = getEntropy(data[]);

    if (result.isOK)
    {
        // Success; data in `bytes` may be used.
    }
    else
    {
        // Failure

        if (result.isUnavailable)
        {
            // System’s entropy source was unavailable.
        }

        // Call `toString` to obtain a user-readable error message.
        assert(result.toString() !is null);
        assert(result.toString().length > 0);
    }
}

