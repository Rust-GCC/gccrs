// Written in the D programming language.
/**
Implements logging facilities.

Copyright: Copyright Robert "burner" Schadek 2013 --
License: <a href="http://www.boost.org/LICENSE_1_0.txt">Boost License 1.0</a>.
Authors: $(HTTP www.svs.informatik.uni-oldenburg.de/60865.html, Robert burner Schadek)

$(H3 Basic Logging)

Message logging is a common approach to expose runtime information of a
program. Logging should be easy, but also flexible and powerful, therefore
`D` provides a standard interface for logging.

The easiest way to create a log message is to write:
-------------
import std.logger;

void main() {
    info("Hello World");
}
-------------
This will print a message to the `stderr` device. The message will contain
the filename, the line number, the name of the surrounding function, the time
and the message.

$(H4 Quickstart)

$(B $(RED Pitfall:)) The $(B defaults) of this package are meant to be $(B reasonable for those who do $(I not) use logging).
Users who intend to use logging are expected to set it up accordingly.

The logging machinery is basically a pipeline.
Convenience functions like `log()`, `info()` and `trace()` are on the very top end of it.
When one calls `trace()`, it sends a log message with `LogLevel.trace` down the pipes.

One of the first checks is for `globalLogLevel`, where any message of a lesser log level is filtered out;
the default here is `LogLevel.all` which allows all log messages to be passed down further.

Eventually, the logged message is passed into a logger which will perform an output operation.
Keep in mind, loggers themselves also have an assigned log level they filter for.

On default settings, a log message will eventually reach `sharedLog`;
this one is a [FileLogger] writing to `stderr` with a threshold of `LogLevel.info`.
This default was chosen because of concerns regarding spamming trace messages to unsuspecting downstream library users.

As consequence of this design decision, the `LogLevel` of `FileLogger` needs to be adjusted
in order to have `trace()` or `log()` actually print something.

-------------
import std.logger;

void main() @system
{
    // `globalLogLevel` defaults to `LogLevel.all`.
    // `sharedLog.logLevel` defaults to `LogLevel.info`.
    assert(globalLogLevel == LogLevel.all);
    assert((cast() sharedLog).logLevel == LogLevel.info);

    // Log a few messages. These will be printed to `stderr` output.
    info("Works out of the box.");
    warning("Works out of the box.");
    error("Works out of the box.");
    critical("Works out of the box.");

    // `log()` and `trace()` will not actually print anything as-is.
    log("Does *not* work out of the box.");
    trace("Does *not* work out of the box.");

    // Enable trace logging output.
    (cast() sharedLog).logLevel = LogLevel.trace;

    // Emit a log message with `LogLevel.trace`.
    trace("Tracing the World");

    // Set the log level to the most permissive option, i.e. `LogLevel.all`.
    (cast() sharedLog).logLevel = LogLevel.all;

    // Emit a log message with `LogLevel.all`.
    log("All aboard!");

    // Logging a fatal message also throws an `Error`, i.e. crashes the program.
    fatal("C u l8r, alligator!");
}
-------------

$(H5 Rationale)

$(UL
    $(LI $(LINK2 https://bugzilla-archive.dlang.org/bugs/22532/, Bugzilla Issue 22532 (archived)))
    $(LI $(LINK2 https://github.com/dlang/phobos/pull/8336, GitHub: dlang/phobos Pull request #8336))
    $(LI $(LINK2 https://github.com/dlang/phobos/pull/8406, GitHub: dlang/phobos Pull request #8406))
    $(LI $(LINK2 https://github.com/dlang/phobos/issues/10858, GitHub: dlang/phobos Issue #10858))
)

$(H3 Advanced Usage)

More complex log call can go along the lines like:
-------------
log("Logging to the sharedLog with its default LogLevel");
logf(LogLevel.info, 5 < 6, "%s to the sharedLog with its LogLevel.info", "Logging");
info("Logging to the sharedLog with its info LogLevel");
warning(5 < 6, "Logging to the sharedLog with its LogLevel.warning if 5 is less than 6");
error("Logging to the sharedLog with its error LogLevel");
errorf("Logging %s the sharedLog %s its error LogLevel", "to", "with");
critical("Logging to the"," sharedLog with its error LogLevel");
fatal("Logging to the sharedLog with its fatal LogLevel");

auto fileLogger = new FileLogger("NameOfTheLogFile");
fileLogger.log("Logging to the fileLogger with its default LogLevel");
fileLogger.info("Logging to the fileLogger with its default LogLevel");
fileLogger.warning(5 < 6, "Logging to the fileLogger with its LogLevel.warning if 5 is less than 6");
fileLogger.warningf(5 < 6, "Logging to the fileLogger with its LogLevel.warning if %s is %s than 6", 5, "less");
fileLogger.critical("Logging to the fileLogger with its info LogLevel");
fileLogger.log(LogLevel.trace, 5 < 6, "Logging to the fileLogger"," with its default LogLevel if 5 is less than 6");
fileLogger.fatal("Logging to the fileLogger with its warning LogLevel");
-------------
Additionally, this example shows how a new `FileLogger` is created.
Individual `Logger` and the global log functions share commonly named
functions to log data.

The names of the functions are as follows:
$(UL
    $(LI `log`)
    $(LI `trace`)
    $(LI `info`)
    $(LI `warning`)
    $(LI `error`)
    $(LI `critical`)
    $(LI `fatal`)
)
The default `Logger` will by default log to `stderr` and has a default
`LogLevel` of `LogLevel.info`. The default Logger can be accessed by
using the property called `sharedLog`. This property is a reference to the
current default `Logger`. This reference can be used to assign a new
default `Logger`.
-------------
sharedLog = new shared FileLogger("New_Default_Log_File.log");
-------------

Additional `Logger` can be created by creating a new instance of the
required `Logger`.

$(H3 Logging Fundamentals)
$(H4 LogLevel)
The `LogLevel` of a log call can be defined in two ways. The first is by
calling `log` and passing the `LogLevel` explicitly as the first argument.
The second way of setting the `LogLevel` of a
log call, is by calling either `trace`, `info`, `warning`,
`critical`, or `fatal`. The log call will then have the respective
`LogLevel`. If no `LogLevel` is defined the log call will use the
current `LogLevel` of the used `Logger`. If data is logged with
`LogLevel` `fatal` by default an `Error` will be thrown.
This behaviour can be modified by using the member `fatalHandler` to
assign a custom delegate to handle log call with `LogLevel` `fatal`.

$(H4 Conditional Logging)
Conditional logging can be achieved be passing a `bool` as first
argument to a log function. If conditional logging is used the condition must
be `true` in order to have the log message logged.

In order to combine an explicit `LogLevel` passing with conditional
logging, the `LogLevel` has to be passed as first argument followed by the
`bool`.

$(H4 Filtering Log Messages)
Messages are logged if the `LogLevel` of the log message is greater than or
equal to the `LogLevel` of the used `Logger` and additionally if the
`LogLevel` of the log message is greater than or equal to the global `LogLevel`.
If a condition is passed into the log call, this condition must be true.

The global `LogLevel` is accessible by using `globalLogLevel`.
To assign a `LogLevel` of a `Logger` use the `logLevel` property of
the logger.

$(H4 Printf Style Logging)
If `printf`-style logging is needed add a $(B f) to the logging call, such as
$(D myLogger.infof("Hello %s", "world");) or $(D fatalf("errno %d", 1337)).
The additional $(B f) appended to the function name enables `printf`-style
logging for all combinations of explicit `LogLevel` and conditional
logging functions and methods.

$(H4 Thread Local Redirection)
Calls to the free standing log functions are not directly forwarded to the
global `Logger` `sharedLog`. Actually, a thread local `Logger` of
type `StdForwardLogger` processes the log call and then, by default, forwards
the created `Logger.LogEntry` to the `sharedLog` `Logger`.
The thread local `Logger` is accessible by the `stdThreadLocalLog`
property. This property allows to assign user defined `Logger`. The default
`LogLevel` of the `stdThreadLocalLog` `Logger` is `LogLevel.all`
and it will therefore forward all messages to the `sharedLog` `Logger`.
The `LogLevel` of the `stdThreadLocalLog` can be used to filter log
calls before they reach the `sharedLog` `Logger`.

$(H3 User Defined Logger)
To customize the `Logger` behavior, create a new `class` that inherits from
the abstract `Logger` `class`, and implements the `writeLogMsg`
method.
-------------
class MyCustomLogger : Logger
{
    this(LogLevel lv) @safe
    {
        super(lv);
    }

    override void writeLogMsg(ref LogEntry payload)
    {
        // log message in my custom way
    }
}

auto logger = new MyCustomLogger(LogLevel.info);
logger.log("Awesome log message with LogLevel.info");
-------------

To gain more precise control over the logging process, additionally to
overriding the `writeLogMsg` method the methods `beginLogMsg`,
`logMsgPart` and `finishLogMsg` can be overridden.

$(H3 Provided Logger)
By default four `Logger` implementations are given. The `FileLogger`
logs data to files. It can also be used to log to `stdout` and `stderr`
as these devices are files as well. A `Logger` that logs to `stdout` can
therefore be created by $(D new FileLogger(stdout)).
The `MultiLogger` is basically an associative array of `string`s to
`Logger`. It propagates log calls to its stored `Logger`. The
`ArrayLogger` contains an array of `Logger` and also propagates log
calls to its stored `Logger`. The `NullLogger` does not do anything. It
will never log a message and will never throw on a log call with `LogLevel`
`error`.

Source: $(PHOBOSSRC std/logger/package.d)
*/
module std.logger;

public import std.logger.core;
public import std.logger.filelogger;
public import std.logger.multilogger;
public import std.logger.nulllogger;
