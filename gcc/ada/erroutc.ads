------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--                              E R R O U T C                               --
--                                                                          --
--                                 S p e c                                  --
--                                                                          --
--          Copyright (C) 1992-2025, Free Software Foundation, Inc.         --
--                                                                          --
-- GNAT is free software;  you can  redistribute it  and/or modify it under --
-- terms of the  GNU General Public License as published  by the Free Soft- --
-- ware  Foundation;  either version 3,  or (at your option) any later ver- --
-- sion.  GNAT is distributed in the hope that it will be useful, but WITH- --
-- OUT ANY WARRANTY;  without even the  implied warranty of MERCHANTABILITY --
-- or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License --
-- for  more details.  You should have  received  a copy of the GNU General --
-- Public License  distributed with GNAT; see file COPYING3.  If not, go to --
-- http://www.gnu.org/licenses for a complete copy of the license.          --
--                                                                          --
-- GNAT was originally developed  by the GNAT team at  New York University. --
-- Extensive contributions were provided by Ada Core Technologies Inc.      --
--                                                                          --
------------------------------------------------------------------------------

--  This package contains global variables and routines common to error
--  reporting packages, including Errout and Prj.Err.

with Table;
with Errsw; use Errsw;
with Errid; use Errid;
with Osint; use Osint;
with Types; use Types;

package Erroutc is

   Exit_Code : Exit_Code_Type := E_Success;
   --  Exit_Code used at the end of the compilation

   type Error_Msg_Type is
     (Error,  -- Default value
      Non_Serious_Error,
      --  An error message that is not considered fatal an the analys of
      --  the source file can resume.
      Warning,
      Style, --  A special kind of warning only triggered by a style check
      Info,
      Low_Check,  --  A type of GNATProve Check messages
      Medium_Check, --  A type of GNATProve Check messages
      High_Check --  A type of GNATProve Check messages
     );

   Class_Flag : Boolean := False;
   --  This flag is set True when outputting a reference to a class-wide
   --  type, and is used by Add_Class to insert 'Class at the proper point

   Continuation : Boolean := False;
   --  Indicates if current message is a continuation. Initialized from the
   --  Msg_Cont parameter in Error_Msg_Internal and then set True if a \
   --  insertion character is encountered.

   Continuation_New_Line : Boolean := False;
   --  Indicates if current message was a continuation line marked with \\ to
   --  force a new line. Set True if \\ encountered.

   Flag_Source : Source_File_Index;
   --  Source file index for source file where error is being posted

   Has_Double_Exclam : Boolean := False;
   --  Set true to indicate that the current message contains the insertion
   --  sequence !! (force warnings even in non-main unit source files).

   Has_Error_Code : Boolean := False;
   --  Set true to indicate that the current message contains the insertion
   --  sequence [] (insert error code).

   Has_Insertion_Line : Boolean := False;
   --  Set True to indicate that the current message contains the insertion
   --  character # (insert line number reference).

   Is_Compile_Time_Msg : Boolean := False;
   --  Set true to indicate that the current message originates from a
   --  Compile_Time_Warning or Compile_Time_Error pragma.

   Is_Runtime_Raise_Msg : Boolean := False;
   --  Set to True to indicate that the current message is a constraint error
   --  that will be raised at runtime (contains [).

   Is_Unconditional_Msg : Boolean := False;
   --  Set True to indicate that the current message contains the insertion
   --  character ! and is thus to be treated as an unconditional message.

   Error_Msg_Kind : Error_Msg_Type := Error;

   Warning_Msg_Char : String (1 .. 2);
   --  Diagnostics switch:
   --    "  "      -- ?   or <   appeared on its own in message
   --    "? "      -- ??  or <<  appeared in message
   --    "x "      -- ?x? or <x< appeared in message
   --              -- (x = a .. z | A .. Z | * | $)
   --    ".x"      -- ?.x? appeared in message (x = a .. z | A .. Z)
   --    "_x"      -- ?_x? appeared in message (x = a .. z | A .. Z)
   --  In the case of the < sequences, this is set only if the message is
   --  actually a warning, i.e. if Error_Msg_Warn is True

   Kill_Message : Boolean := False;
   --  A flag used to kill weird messages (e.g. those containing uninterpreted
   --  implicit type references) if we have already seen at least one message
   --  already. The idea is that we hope the weird message is a junk cascaded
   --  message that should be suppressed.

   Last_Killed : Boolean := False;
   --  Set True if the most recently posted non-continuation message was
   --  killed. This is used to determine the processing of any continuation
   --  messages that follow.

   List_Pragmas_Index : Int := 0;
   --  Index into List_Pragmas table

   List_Pragmas_Mode : Boolean := False;
   --  Starts True, gets set False by pragma List (Off), True by List (On)

   Manual_Quote_Mode : Boolean := False;
   --  Set True in manual quotation mode

   Max_Msg_Length : constant := 1024 + 2 * Int (Column_Number'Last);
   --  Maximum length of error message. The addition of 2 * Column_Number'Last
   --  ensures that two insertion tokens of maximum length can be accommodated.
   --  The value of 1024 is an arbitrary value that should be more than long
   --  enough to accommodate any reasonable message (and for that matter, some
   --  pretty unreasonable messages).

   Msg_Buffer : String (1 .. Max_Msg_Length);
   --  Buffer used to prepare error messages

   Msglen : Integer := 0;
   --  Number of characters currently stored in the message buffer

   Suppress_Message : Boolean;
   --  A flag used to suppress certain obviously redundant messages (i.e.
   --  those referring to a node whose type is Any_Type). This suppression
   --  is effective only if All_Errors_Mode is off.

   Suppress_Instance_Location : Boolean := False;
   --  Normally, if a # location in a message references a location within
   --  a generic template, then a note is added giving the location of the
   --  instantiation. If this variable is set True, then this note is not
   --  output. This is used for internal processing for the case of an
   --  illegal instantiation. See Error_Msg routine for further details.

   ----------------------------
   -- Message ID Definitions --
   ----------------------------

   type Error_Msg_Id is new Int;
   --  A type used to represent specific error messages. Used by the clients
   --  of this package only in the context of the Get_Error_Id and
   --  Change_Error_Text subprograms.

   No_Error_Msg : constant Error_Msg_Id := 0;
   --  A constant which is different from any value returned by Get_Error_Id.
   --  Typically used by a client to indicate absence of a saved Id value.

   Cur_Msg : Error_Msg_Id := No_Error_Msg;
   --  Id of most recently posted error message

   function Get_Msg_Id return Error_Msg_Id;
   --  Returns the Id of the message most recently posted using one of the
   --  Error_Msg routines.

   function Get_Location (E : Error_Msg_Id) return Source_Ptr;
   --  Returns the flag location of the error message with the given id E

   -----------------------------------
   -- Error Message Data Structures --
   -----------------------------------

   --  The error messages are stored as a linked list of error message objects
   --  sorted into ascending order by the source location (Sloc). Each object
   --  records the text of the message and its source location.

   --  The following record type and table are used to represent error
   --  messages, with one entry in the table being allocated for each message.

   type Labeled_Span_Id is new Int;
   No_Labeled_Span : constant Labeled_Span_Id := 0;

   type Labeled_Span_Type is record
      Label : String_Ptr := null;
      --  Text associated with the span

      Span : Source_Span := (others => No_Location);
      --  Textual region in the source code

      Is_Primary : Boolean := True;
      --  Primary spans are used to indicate the primary location of the
      --  diagnostic. Typically there should just be one primary span per
      --  diagnostic.
      --  Non-primary spans are used to indicate secondary locations and
      --  typically are formatted in a different way or omitted in some
      --  contexts.

      Is_Region : Boolean := False;
      --  Regional spans are multiline spans that have a unique way of being
      --  displayed in the pretty output.

      Next : Labeled_Span_Id := No_Labeled_Span;

   end record;

   No_Labeled_Span_Object : Labeled_Span_Type := (others => <>);

   package Locations is new Table.Table (
     Table_Component_Type => Labeled_Span_Type,
     Table_Index_Type     => Labeled_Span_Id,
     Table_Low_Bound      => 1,
     Table_Initial        => 200,
     Table_Increment      => 200,
     Table_Name           => "Location");

   type Edit_Id is new Int;
   No_Edit : constant Edit_Id := 0;

   type Edit_Type is record
      Span : Source_Span;
      --  Region of the file to be removed

      Text : String_Ptr;
      --  Text to be inserted at the start location of the span

      Next : Edit_Id := No_Edit;
   end record;

   package Edits is new Table.Table (
     Table_Component_Type => Edit_Type,
     Table_Index_Type     => Edit_Id,
     Table_Low_Bound      => 1,
     Table_Initial        => 200,
     Table_Increment      => 200,
     Table_Name           => "Edit");

   type Fix_Id is new Int;
   No_Fix : constant Fix_Id := 0;

   type Fix_Type is record
      Description : String_Ptr := null;
      --  Message describing the fix that will be displayed to the user.

      Edits : Edit_Id := No_Edit;
      --   File changes for the fix.

      Next : Fix_Id := No_Fix;
   end record;

   package Fixes is new Table.Table (
     Table_Component_Type => Fix_Type,
     Table_Index_Type     => Fix_Id,
     Table_Low_Bound      => 1,
     Table_Initial        => 200,
     Table_Increment      => 200,
     Table_Name           => "Fix");

   type Warning_As_Error_Kind is
     (None, From_Pragma, From_Warn_As_Err, From_Run_Time_As_Err);
   --  The reason for a warning to be converted as an error:
   --  * None - Regular warning. Default value for non-warning messages.
   --  * From_Pragma - Warning converted to an error due to a pragma
   --    Warning_As_Error.
   --  * From_Warn_As_Err - Warning converted to an error because the
   --    Warning_Mode was set to Treat_As_Errors by -gnatwe.
   --  * From_Run_Time_As_Err - Warning converted to an error because the
   --    Warning_Mode was set to Treat_Run_Time_Warnings_As_Errors by -gnatwE.

   type Error_Msg_Object is record
      Text : String_Ptr;
      --  Text of error message, fully expanded with all insertions

      Next : Error_Msg_Id;
      --  Pointer to next message in error chain. A value of No_Error_Msg
      --  indicates the end of the chain.

      Prev : Error_Msg_Id;
      --  Pointer to previous message in error chain. Only set during the
      --  Finalize procedure. A value of No_Error_Msg indicates the first
      --  message in the chain.

      Sfile : Source_File_Index;
      --  Source table index of source file. In the case of an error that
      --  refers to a template, always references the original template
      --  not an instantiation copy.

      Sptr : Source_Span;
      --  Flag pointer. In the case of an error that refers to a template,
      --  always references the original template, not an instantiation copy.
      --  This value is the actual place in the source that the error message
      --  will be posted. Note that an error placed on an instantiation will
      --  have Sptr pointing to the instantiation point.

      Optr : Source_Span;
      --  Flag location used in the call to post the error. This is the same as
      --  Sptr, except when an error is posted on a particular instantiation of
      --  a generic. In such a case, Sptr will point to the original source
      --  location of the instantiation itself, but Optr will point to the
      --  template location (more accurately to the template copy in the
      --  instantiation copy corresponding to the instantiation referenced by
      --  Sptr).

      Insertion_Sloc : Source_Ptr;
      --  Location in message for insertion character # when used

      Line : Physical_Line_Number;
      --  Line number for error message

      Col : Column_Number;
      --  Column number for error message

      Compile_Time_Pragma : Boolean;
      --  True if the message originates from a Compile_Time_Warning or
      --  Compile_Time_Error pragma

      Warn_Err : Warning_As_Error_Kind;
      --  By default this is None. If the warning was converted by some reason
      --  to an error then it has a different value. Depending on the value
      --  the warning will be printed in a different way due to historical
      --  reasons.

      Warn_Chr : String (1 .. 2);
      --  See Warning_Msg_Char

      Uncond : Boolean;
      --  True if unconditional message (i.e. insertion character ! appeared)

      Msg_Cont : Boolean;
      --  This is used for logical messages that are composed of multiple
      --  individual messages. For messages that are not part of such a
      --  group, or that are the first message in such a group. Msg_Cont
      --  is set to False. For subsequent messages in a group, Msg_Cont
      --  is set to True. This is used to make sure that such a group of
      --  messages is either suppressed or retained as a group (e.g. in
      --  the circuit that deletes identical messages).

      Deleted : Boolean;
      --  If this flag is set, the message is not printed. This is used
      --  in the circuit for deleting duplicate/redundant error messages.

      Kind : Error_Msg_Type;
      --  The kind of the error message. This determines how the message
      --  should be handled and what kind of prefix should be added before the
      --  message text.

      Switch : Switch_Id := No_Switch_Id;
      --  Identifier for a given switch that enabled the diagnostic

      Id : Diagnostic_Id := No_Diagnostic_Id;
      --  Unique error code for the given message

      Locations : Labeled_Span_Id := No_Labeled_Span;
      --  Identifier to the first location identified by the error message.
      --  These locations are marked with an underlying span line and
      --  optionally given a short label.

      Fixes : Fix_Id := No_Fix;
      --  Identifier to the first fix object for the error message. The fix
      --  contains a suggestion to prevent the error from being triggered.
      --  This includes edits that can be made to the source code. An edit
      --  contians a region of the code that needs to be changed and the new
      --  text that should be inserted to that region.
   end record;

   package Errors is new Table.Table (
     Table_Component_Type => Error_Msg_Object,
     Table_Index_Type     => Error_Msg_Id,
     Table_Low_Bound      => 1,
     Table_Initial        => 200,
     Table_Increment      => 200,
     Table_Name           => "Error");

   First_Error_Msg : Error_Msg_Id;
   --  The list of error messages, i.e. the first entry on the list of error
   --  messages. This is not the same as the physically first entry in the
   --  error message table, since messages are not always inserted in sequence.

   Last_Error_Msg : Error_Msg_Id;
   --  The last entry on the list of error messages. Note: this is not the same
   --  as the physically last entry in the error message table, since messages
   --  are not always inserted in sequence.

   procedure Next_Error_Msg (E : in out Error_Msg_Id);
   --  Update E to point to the next error message in the list of error
   --  messages. Skip deleted and continuation messages.

   procedure Next_Continuation_Msg (E : in out Error_Msg_Id);
   --  Update E to point to the next continuation message

   function Kind_To_String (E : Error_Msg_Object) return String is
     (if E.Warn_Err in From_Pragma | From_Run_Time_As_Err then "error"
      else
        (case E.Kind is
           when Error | Non_Serious_Error => "error",
           when Warning                   => "warning",
           when Style                     => "style",
           when Info                      => "info",
           when Low_Check                 => "low",
           when Medium_Check              => "medium",
           when High_Check                => "high"));
   --  Returns the name of the error message kind. If it is a warning that has
   --  been turned to an error then it returns "error".

   function Get_Doc_Switch (E : Error_Msg_Object) return String;
   --  Returns the documentation switch for a given Error_Msg_Object.
   --
   --  This either the name of the switch encased in brackets. E.g [-gnatwx].
   --
   --  If the Warn_Char is "* " is then it will return [restriction warning].
   --
   --  Otherwise for messages without a switch it will return
   --  [enabled by default] .

   function Primary_Location (E : Error_Msg_Object) return Labeled_Span_Id;
   --  Returns the first Primary Labeled_Span associated with the error
   --  message. Otherwise it returns No_Labeled_Span.

   function Get_Human_Id (E : Error_Msg_Object) return String_Ptr;
   --  Returns a longer human readable name for the switch associated with the
   --  error message.

   function Get_Switch (E : Error_Msg_Object) return Switch_Type;
   --  Returns the Switch information for the given error message

   function Get_Switch_Id (E : Error_Msg_Object) return Switch_Id;
   --  Returns the Switch information identifier for the given error message

   function Get_Switch_Id
     (Kind : Error_Msg_Type; Warn_Chr : String) return Switch_Id;
   --  Returns the Switch information identifier based on the error kind and
   --  the warning character.

   --------------------------
   -- Warning Mode Control --
   --------------------------

   --  Pragma Warnings allows warnings to be turned off for a specified region
   --  of code, and the following tables are the data structures used to keep
   --  track of these regions.

   --  The first table is used for the basic command line control, and for the
   --  forms of Warning with a single ON or OFF parameter.

   --  It contains pairs of source locations, the first being the start
   --  location for a warnings off region, and the second being the end
   --  location. When a pragma Warnings (Off) is encountered, a new entry is
   --  established extending from the location of the pragma to the end of the
   --  current source file. A subsequent pragma Warnings (On) adjusts the end
   --  point of this entry appropriately.

   --  If all warnings are suppressed by command switch, then there is a dummy
   --  entry (put there by Errout.Initialize) at the start of the table which
   --  covers all possible Source_Ptr values. Note that the source pointer
   --  values in this table always reference the original template, not an
   --  instantiation copy, in the generic case.

   --  Reason is the reason from the pragma Warnings (Off,..) or the null
   --  string if no reason parameter is given.

   type Warnings_Entry is record
      Start  : Source_Ptr;
      Stop   : Source_Ptr;
      Reason : String_Id;
   end record;

   package Warnings is new Table.Table (
     Table_Component_Type => Warnings_Entry,
     Table_Index_Type     => Natural,
     Table_Low_Bound      => 1,
     Table_Initial        => 100,
     Table_Increment      => 200,
     Table_Name           => "Warnings");

   --  The second table is used for the specific forms of the pragma, where
   --  the first argument is ON or OFF, and the second parameter is a string
   --  which is the pattern to match for suppressing a warning.

   type Specific_Warning_Entry is record
      Start : Source_Ptr;
      Stop  : Source_Ptr;
      --  Starting and ending source pointers for the range. These are always
      --  from the same source file.

      Reason : String_Id;
      --  Reason string from pragma Warnings, or null string if none

      Msg : String_Ptr;
      --  Message from pragma Warnings (Off, string)

      Open : Boolean;
      --  Set to True if OFF has been encountered with no matching ON

      Used : Boolean;
      --  Set to True if entry has been used to suppress a warning

      Config : Boolean;
      --  True if pragma is configuration pragma (in which case no matching Off
      --  pragma is required, and it is not required that a specific warning be
      --  suppressed).
   end record;

   package Specific_Warnings is new Table.Table (
     Table_Component_Type => Specific_Warning_Entry,
     Table_Index_Type     => Natural,
     Table_Low_Bound      => 1,
     Table_Initial        => 100,
     Table_Increment      => 200,
     Table_Name           => "Specific_Warnings");

   --  Note on handling configuration case versus specific case. A complication
   --  arises from this example:

   --     pragma Warnings (Off, "not referenced*");
   --     procedure Mumble (X : Integer) is
   --     pragma Warnings (On, "not referenced*");
   --     begin
   --        null;
   --     end Mumble;

   --  The trouble is that the first pragma is technically a configuration
   --  pragma, and yet it is clearly being used in the context of thinking of
   --  it as a specific case. To deal with this, what we do is that the On
   --  entry can match a configuration pragma from the same file, and if we
   --  find such an On entry, we cancel the indication of it being the
   --  configuration case. This seems to handle all cases we run into ok.

   -------------------
   -- Color Control --
   -------------------

   Use_SGR_Control : Boolean := False;
   --  Set to True for enabling colored output. This should only be done when
   --  outputting messages to a terminal that supports it.

   --  Colors in messages output to a terminal are controlled using SGR
   --  (Select Graphic Rendition).

   Color_Separator  : constant String := ";";
   Color_None       : constant String := "00";
   Color_Bold       : constant String := "01";
   Color_Underscore : constant String := "04";
   Color_Blink      : constant String := "05";
   Color_Reverse    : constant String := "07";
   Color_Fg_Black   : constant String := "30";
   Color_Fg_Red     : constant String := "31";
   Color_Fg_Green   : constant String := "32";
   Color_Fg_Yellow  : constant String := "33";
   Color_Fg_Blue    : constant String := "34";
   Color_Fg_Magenta : constant String := "35";
   Color_Fg_Cyan    : constant String := "36";
   Color_Fg_White   : constant String := "37";
   Color_Bg_Black   : constant String := "40";
   Color_Bg_Red     : constant String := "41";
   Color_Bg_Green   : constant String := "42";
   Color_Bg_Yellow  : constant String := "43";
   Color_Bg_Blue    : constant String := "44";
   Color_Bg_Magenta : constant String := "45";
   Color_Bg_Cyan    : constant String := "46";
   Color_Bg_White   : constant String := "47";

   SGR_Start        : constant String := ASCII.ESC & "[";
   SGR_End          : constant String := "m" & ASCII.ESC & "[K";

   function SGR_Seq (Str : String) return String is
     (if Use_SGR_Control then SGR_Start & Str & SGR_End else "");
   --  Return the SGR control string for the commands in Str. It returns the
   --  empty string if Use_SGR_Control is False, so that we can insert this
   --  string unconditionally.

   function SGR_Reset return String is (SGR_Seq (""));
   --  This ends the current section of colored output

   --  We're using the same colors as gcc/g++ for errors/warnings/notes/locus.
   --  More colors are defined in gcc/g++ for other features of diagnostic
   --  messages (e.g. inline types, fixit) and could be used in GNAT in the
   --  future. The following functions start a section of colored output.

   function SGR_Error return String is
     (SGR_Seq (Color_Bold & Color_Separator & Color_Fg_Red));
   function SGR_Warning return String is
     (SGR_Seq (Color_Bold & Color_Separator & Color_Fg_Magenta));
   function SGR_Note return String is
     (SGR_Seq (Color_Bold & Color_Separator & Color_Fg_Cyan));
   function SGR_Locus return String is
     (SGR_Seq (Color_Bold));

   function Get_SGR_Code (E_Msg : Error_Msg_Object) return String is
     (if E_Msg.Warn_Err /= None then SGR_Error
      else
        (case E_Msg.Kind is
           when Warning | Style => SGR_Warning,
           when Info => SGR_Note,
           when others => SGR_Error));

   -----------------
   -- Subprograms --
   -----------------

   procedure Add_Class;
   --  Add 'Class to buffer for class wide type case (Class_Flag set)

   function Buffer_Ends_With (C : Character) return Boolean;
   --  Tests if message buffer ends with given character

   function Buffer_Ends_With (S : String) return Boolean;
   --  Tests if message buffer ends with given string preceded by a space

   procedure Buffer_Remove (C : Character);
   --  Remove given character from end of buffer if it is present

   procedure Buffer_Remove (S : String);
   --  Removes given string from end of buffer if it is present at end of
   --  buffer, and preceded by a space.

   function Compilation_Errors return Boolean;
   --  Returns true if errors have been detected, or warnings that are treated
   --  as errors.

   procedure dmsg (Id : Error_Msg_Id);
   --  Debugging routine to dump an error message

   procedure Decrease_Error_Msg_Count (E : Error_Msg_Object);

   procedure Debug_Output (N : Node_Id);
   --  Called from Error_Msg_N and Error_Msg_NE to generate line of debug
   --  output giving node number (of node N) if the debug X switch is set.

   procedure Check_Duplicate_Message (M1, M2 : Error_Msg_Id);
   --  This function is passed the Id values of two error messages. If either
   --  M1 or M2 is a continuation message, or is already deleted, the call is
   --  ignored. Otherwise a check is made to see if M1 and M2 are duplicated or
   --  redundant. If so, the message to be deleted and all its continuations
   --  are marked with the Deleted flag set to True.

   function Get_Warning_Option (Id : Error_Msg_Id) return String;
   function Get_Warning_Option (E : Error_Msg_Object) return String;
   --  Returns the warning switch causing this warning message or an empty
   --  string is there is none..

   function Get_Warning_Tag (Id : Error_Msg_Id) return String;
   function Get_Warning_Tag (E : Error_Msg_Object) return String;
   --  Given an error message, return tag showing warning message class, or
   --  the null string if this option is not enabled or this is not a warning.

   procedure Increase_Error_Msg_Count (E : Error_Msg_Object);
   --  Increase the error count for the given kind of error message

   function Is_Redundant_Error_Message
     (Prev_Msg : Error_Msg_Id; Cur_Msg : Error_Msg_Id) return Boolean;
   --  Check if the Cur_Msg can be removed if it was issued at the same line as
   --  the Prev_Msg.

   function Matches (S : String; P : String) return Boolean;
   --  Returns true if the String S matches the pattern P, which can contain
   --  wildcard chars (*). The entire pattern must match the entire string.
   --  Case is ignored in the comparison (so X matches x).

   procedure Output_Error_Msgs (E : in out Error_Msg_Id);
   --  Output source line, error flag, and text of stored error message and all
   --  subsequent messages for the same line and unit. On return E is set to be
   --  one higher than the last message output.

   procedure Output_Line_Number (L : Logical_Line_Number);
   --  Output a line number as six digits (with leading zeroes suppressed),
   --  followed by a period and a blank (note that this is 8 characters which
   --  means that tabs in the source line will not get messed up). Line numbers
   --  that match or are less than the last Source_Reference pragma are listed
   --  as all blanks, avoiding output of junk line numbers.

   procedure Output_Msg_Location (E : Error_Msg_Id);
   --  Write the location of the error message in the following format:
   --
   --  <File_Name>:<Line>:<Col>:
   --
   --  If Full_Path_Name_For_Brief_Errors then full path of the file is used.

   procedure Output_Msg_Text (E : Error_Msg_Id);
   --  Outputs characters of text in the text of the error message E. Note that
   --  no end of line is output, the caller is responsible for adding the end
   --  of line. If Error_Msg_Line_Length is non-zero, this is the routine that
   --  splits the line generating multiple lines of output, and in this case
   --  the last line has no terminating end of line character.

   procedure Output_Text_Within (Txt : String; Line_Length : Nat);
   --  Output the text in Txt, splitting it into lines of at most the size of
   --  Line_Length.

   procedure Prescan_Message (Msg : String);
   --  Scans message text and sets the following variables:
   --
   --    Is_Unconditional_Msg is set True if the message contains the character
   --    ! and is otherwise set False.
   --
   --    Has_Double_Exclam is set True if the message contains the sequence !!
   --    and is otherwise set False.
   --
   --    Has_Error_Code is set True if the message contains the sequence []
   --    and is otherwise set False.
   --
   --    Has_Insertion_Line is set True if the message contains the character #
   --    and is otherwise set False.
   --
   --    Error_Msg_Kind is set to one of the following values:
   --      * Style             - if the message starts with "(style)"
   --      * Info              - if the message starts with "info: "
   --      * Warning           - if the message contains a "?" and they
   --                            are neither Info or Style messages.
   --      * Low_Check         - if the message starts with "low: "
   --      * Medium_Check      - if the message starts with "medium: "
   --      * High_Check        - if the message starts with "high: "
   --      * Non_Serious_Error - if the message contains "|"
   --      * Error             - otherwise
   --
   --  We need to know right away these aspects of a message, since we will
   --  test these values before doing the full error scan.
   --
   --  Note that the call has no effect for continuation messages (those whose
   --  first character is '\') except for the Has_Insertion_Line setting.

   --  Definitions for valid message kind prefixes within error messages.

   Info_Prefix   : constant String := "info: ";
   Low_Prefix    : constant String := "low: ";
   Medium_Prefix : constant String := "medium: ";
   High_Prefix   : constant String := "high: ";
   Style_Prefix  : constant String := "(style) ";

   Warn_As_Err_Tag : constant String := "[warning-as-error]";
   --  Tag used at the end of warning messages that were converted by
   --  pragma Warning_As_Error.

   procedure Purge_Messages (From : Source_Ptr; To : Source_Ptr);
   --  All error messages whose location is in the range From .. To (not
   --  including the end points) will be deleted from the error listing.

   function Same_Error (M1, M2 : Error_Msg_Id) return Boolean;
   --  See if two messages have the same text. Returns true if the text of the
   --  two messages is identical, or if one of them is the same as the other
   --  with an appended "instance at xxx" tag.

   procedure Set_Msg_Blank;
   --  Sets a single blank in the message if the preceding character is a
   --  non-blank character other than a left parenthesis or minus. Has no
   --  effect if manual quote mode is turned on.

   procedure Set_Msg_Blank_Conditional;
   --  Sets a single blank in the message if the preceding character is a
   --  non-blank character other than a left parenthesis or quote. Has no
   --  effect if manual quote mode is turned on.

   procedure Set_Msg_Char (C : Character);
   --  Add a single character to the current message. This routine does not
   --  check for special insertion characters (they are just treated as text
   --  characters if they occur).

   procedure Set_Msg_Insertion_Code;
   --  Handle error code insertion ([] insertion character)

   procedure Set_Msg_Insertion_Column;
   --  Handle column number insertion (@ insertion character)

   procedure Set_Msg_Insertion_File_Name;
   --  Handle file name insertion (left brace insertion character)

   procedure Set_Msg_Insertion_Line_Number (Loc, Flag : Source_Ptr);
   --  Handle line number insertion (# insertion character). Loc is the
   --  location to be referenced, and Flag is the location at which the
   --  flag is posted (used to determine whether to add "in file xxx")

   procedure Set_Msg_Insertion_Name_Literal;

   procedure Set_Msg_Insertion_Name;
   --  Handle name insertion (% insertion character)

   procedure Set_Msg_Insertion_Reserved_Name;
   --  Handle insertion of reserved word name (* insertion character)

   procedure Set_Msg_Insertion_Reserved_Word
     (Text : String;
      J    : in out Integer);
   --  Handle reserved word insertion (upper case letters). The Text argument
   --  is the current error message input text, and J is an index which on
   --  entry points to the first character of the reserved word, and on exit
   --  points past the last character of the reserved word. Note that RM and
   --  SPARK are treated specially and not considered to be keywords.

   procedure Set_Msg_Insertion_Run_Time_Name;
   --  If package System contains a definition for Run_Time_Name (see package
   --  Targparm for details), then this procedure will insert a message of
   --  the form (name) into the current error message, with name set in mixed
   --  case (upper case after any spaces). If no run time name is defined,
   --  then this routine has no effect).

   procedure Set_Msg_Insertion_Uint;
   --  Handle Uint insertion (^ insertion character)

   procedure Set_Msg_Int (Line : Int);
   --  Set the decimal representation of the argument in the error message
   --  buffer with no leading zeroes output.

   procedure Set_Msg_Name_Buffer;
   --  Output name from Namet.Global_Name_Buffer, with surrounding quotes
   --  unless manual quotation mode is in effect.

   procedure Set_Msg_Quote;
   --  Set quote if in normal quote mode, nothing if in manual quote mode

   procedure Set_Msg_Str (Text : String);
   --  Add a sequence of characters to the current message. This routine does
   --  not check for special insertion characters (they are just treated as
   --  text characters if they occur). It does perform the transformation of
   --  the special strings _xxx (xxx = Pre/Post/Type_Invariant) to xxx'Class.

   procedure Set_Next_Non_Deleted_Msg (E : in out Error_Msg_Id);
   --  Given a message id, move to next message id, but skip any deleted
   --  messages, so that this results in E on output being the first non-
   --  deleted message following the input value of E, or No_Error_Msg if
   --  the input value of E was either already No_Error_Msg, or was the
   --  last non-deleted message.

   procedure Set_Specific_Warning_Off
     (Node   : Node_Id;
      Msg    : String;
      Reason : String_Id;
      Config : Boolean;
      Used   : Boolean := False);
   --  This is called in response to the two argument form of pragma Warnings
   --  where the first argument is OFF, and the second argument is a string
   --  which identifies a specific warning to be suppressed. The first argument
   --  is the corresponding N_Pragma node, and the second argument is the
   --  string from the pragma. Sloc (Node) is the start of the range to
   --  suppress. Reason is the reason string from the pragma, or the null
   --  string if no reason is given. Config is True for the configuration
   --  pragma case (where there is no requirement for a matching OFF pragma).
   --  Used is set True to disable the check that the warning actually has the
   --  effect of suppressing a warning.

   procedure Set_Specific_Warning_On
     (Loc : Source_Ptr;
      Msg : String;
      Err : out Boolean);
   --  This is called in response to the two argument form of pragma Warnings
   --  where the first argument is ON, and the second argument is a string
   --  which identifies a specific warning to be suppressed. The first argument
   --  is the end of the suppression range, and the second argument is the
   --  string from the pragma. Err is set to True on return to report the error
   --  of no matching Warnings Off pragma preceding this one.

   procedure Set_Warnings_Mode_Off (Loc : Source_Ptr; Reason : String_Id);
   --  Called in response to a pragma Warnings (Off) to record the source
   --  location from which warnings are to be turned off. Reason is the
   --  Reason from the pragma, or the null string if none is given.

   procedure Set_Warnings_Mode_On (Loc : Source_Ptr);
   --  Called in response to a pragma Warnings (On) to record the source
   --  location from which warnings are to be turned back on.

   function Warnings_Suppressed (Loc : Source_Ptr) return String_Id;
   --  Determines if given location is covered by a warnings off suppression
   --  range in the warnings table (or is suppressed by compilation option,
   --  which generates a warning range for the whole source file). This routine
   --  only deals with the general ON/OFF case, not specific warnings. The
   --  returned result is No_String if warnings are not suppressed. If warnings
   --  are suppressed for the given location, then corresponding Reason
   --  parameter from the pragma is returned (or the null string if no Reason
   --  parameter was present).

   function Warning_Specifically_Suppressed
     (Loc : Source_Ptr;
      Msg : String_Ptr;
      Tag : String := "") return String_Id;
   --  Determines if given message to be posted at given location is suppressed
   --  by specific ON/OFF Warnings pragmas specifying this particular message.
   --  If the warning is not suppressed then No_String is returned, otherwise
   --  the corresponding warning string is returned (or the null string if no
   --  Warning argument was present in the pragma). Tag is the error message
   --  tag for the message in question or the null string if there is no tag.
   --
   --  Note: we have a null default for Tag to deal with calls from an old
   --  branch of gnat2why, which does not know about tags in the calls but
   --  which uses the latest version of erroutc.

   function Warning_Treated_As_Error (Msg : String) return Boolean;
   --  Returns True if the warning message Msg matches any of the strings
   --  given by Warning_As_Error pragmas, as stored in the Warnings_As_Errors
   --  table.

   function Warning_Treated_As_Error (E : Error_Msg_Object) return Boolean;
   --  Returns true if a Warning_As_Error pragma matches either the error text
   --  or the warning tag of the message.

   procedure Write_Error_Summary;
   --  Write error summary

end Erroutc;
