@safe unittest
{
    import std.mathspecial;

    assert(sgnGamma(10_000) == 1);
}

@safe unittest
{
    import std.mathspecial;

    assert(beta(1, 2) == 0.5);
}

@safe unittest
{
    import std.mathspecial;

    const euler = 0.57721_56649_01532_86060_65121L;

    assert(isClose(digamma(1), -euler));
    assert(digamma(+0.) == -real.infinity);
    assert(digamma(-0.) == +real.infinity);
    assert(digamma(+real.infinity) == +real.infinity);
    assert(isNaN(digamma(-1)));
    assert(isNaN(digamma(-real.infinity)));
}

@safe unittest
{
    import std.mathspecial;

    assert(betaIncomplete(1, 1, .5) == .5);
    assert(betaIncomplete(+0., +0., 0) == 0);
    assert(isNaN(betaIncomplete(+0., +0., .5)));
    assert(isNaN(betaIncomplete(real.infinity, real.infinity, .5)));
    assert(betaIncomplete(real.infinity, real.infinity, 1) == 1);
    assert(betaIncomplete(NaN(0x1), 1, NaN(0x2)) is NaN(0x2));
    assert(betaIncomplete(1, NaN(0x3), -NaN(0x3)) is -NaN(0x3));
}

@safe unittest
{
    import std.mathspecial;

    assert(betaIncompleteCompl(.1, .2, 0) == betaIncomplete(.2, .1, 1));
}

@safe unittest
{
    import std.mathspecial;

    assert(isClose(gammaIncomplete(1, 1), 1 - 1/E));
    assert(gammaIncomplete(1, 0) == 0);
    assert(gammaIncomplete(1, real.infinity) == 1);
    assert(gammaIncomplete(+0., 1) == 1);
    assert(gammaIncomplete(real.infinity, 1) == 0);
}

@safe unittest
{
    import std.mathspecial;

    assert(isClose(gammaIncompleteCompl(2, 1), 2/E));
    assert(gammaIncompleteCompl(1, 0) == 1);
    assert(gammaIncompleteCompl(1, real.infinity) == 0);
    assert(gammaIncompleteCompl(+0., 1) == 0);
    assert(gammaIncompleteCompl(real.infinity, 1) == 1);
    assert(isClose(gammaIncompleteCompl(1, 2), 1-gammaIncomplete(1, 2)));
}

@safe unittest
{
    import std.mathspecial;

    const a = 2, p = 0.5L;
    assert(isClose(gammaIncompleteComplInverse(a, gammaIncompleteCompl(a, p)), p));

    assert(isClose(gammaIncompleteComplInverse(1, 1/E), 1));
    assert(isNaN(gammaIncompleteComplInverse(+0.0L, 0.1)));
    assert(isNaN(gammaIncompleteComplInverse(real.infinity, 0.2)));
    assert(gammaIncompleteComplInverse(3, 0) is real.infinity);
    assert(gammaIncompleteComplInverse(4, 1) == 0);
}

