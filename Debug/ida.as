
// FAR FROM COMPLETE
//
//
// Declares AngelScript-to-IDA types
// and #defines
//
//
//
typedef bool success;

ptr here = 0;// this is a #define

#if __X86__
typedef uint long;

const long BADADDR = 0xFFFFFFFF;
const long BADSEL = 0xFFFFFFFF;
#endif

#if __X64__
typedef uint64 long;

const long BADADDR = 0xFFFFFFFFFFFFFFFF;
const long BADSEL = 0xFFFFFFFFFFFFFFFF;
#endif

//
// Functions
//
// ----------------------------------------------------------------------------
//        I N T E R A C T I O N   W I T H   T H E   U S E R
// ----------------------------------------------------------------------------

// Ask the user to enter a string
//      defval - the default string value. This value
//               will appear in the dialog box.
//      prompt - the prompt to display in the dialog box
// Returns: the entered string.

string  AskStr          (string defval,string prompt)
{
  return "";
}


// Ask the user to choose a file
//      forsave- 0: "Open" dialog box, 1: "Save" dialog box
//      mask   - the input file mask as "*.*" or the default file name.
//      prompt - the prompt to display in the dialog box
// Returns: the selected file.

string  AskFile         (bool forsave,string mask,string prompt)
{
  return "";
}

// Ask the user to enter an address
//      defval - the default address value. This value
//               will appear in the dialog box.
//      prompt - the prompt to display in the dialog box
// Returns: the entered address or BADADDR.

long    AskAddr         (long defval,string prompt)
{
  return BADADDR;
}

// Ask the user to enter a number
//      defval - the default value. This value
//               will appear in the dialog box.
//      prompt - the prompt to display in the dialog box
// Returns: the entered number or -1.

long    AskLong         (long defval,string prompt)
{
  return -1;
}

// Ask the user to enter a segment value
//      defval - the default value. This value
//               will appear in the dialog box.
//      prompt - the prompt to display in the dialog box
// Returns: the entered segment selector or BADSEL.

long    AskSeg          (long defval,string prompt)
{
  return BADSEL;
}


// Ask the user to enter an identifier
//      defval - the default identifier. This value
//               will appear in the dialog box.
//      prompt - the prompt to display in the dialog box
// Returns: the entered identifier.

string  AskIdent        (string defval,string prompt)
{
  return "";
}


// Ask the user a question and let him answer Yes/No/Cancel
//      defval - the default answer. This answer will be selected if the user
//               presses Enter. -1:cancel,0-no,1-ok
//      prompt - the prompt to display in the dialog box
// Returns: -1:cancel,0-no,1-ok

long    AskYN           (long defval,string prompt)
{
  return -1;
}


// Display a message in the message window
//      format - printf() style format string
//      ...    - additional parameters if any
// This function can be used to debug IDC scripts
// Thread-safe function.

void    Message         (string format /*,... */)
{
  printf(format);
}


// Print variables in the message window
// This function print text representation of all its arguments to the output window.
// This function can be used to debug IDC scripts

void    print           (string format /* ... */)
{
  printf(format);
}


// Display a message in a message box
//      format - printf() style format string
//      ...    - additional parameters if any
// This function can be used to debug IDC scripts
// The user will be able to hide messages if they appear twice in a row on the screen

void    Warning         (string format /* ,... */)
{
}


// Display a fatal message in a message box and quit IDA
//      format - printf() style format string
//      ...    - additional parameters if any

void    Fatal           (string format /* ,... */)
{
}

// ----------------------------------------------------------------------------
// C H A N G E   P R O G R A M   R E P R E S E N T A T I O N
// ----------------------------------------------------------------------------

// Change value of a program byte
// If debugger was active then the debugged process memory will be patched too
//      ea    - linear address
//      value - new value of the byte
// Returns: 1 if successful, 0 if not

success PatchByte       (long ea,long value)
{
  return Dbg::MemWrite(ea, addressof(value), typesize("byte"));
}


// Change value of a program word (2 bytes)
//      ea    - linear address
//      value - new value of the word
// Returns: 1 if successful, 0 if not

success PatchWord       (long ea,long value)
{
  return Dbg::MemWrite(ea, addressof(value), typesize("uint16"));
}


// Change value of a double word
//      ea    - linear address
//      value - new value of the double word
// Returns: 1 if successful, 0 if not

success PatchDword      (long ea,long value)
{
  return Dbg::MemWrite(ea, addressof(value), typesize("uint32"));
}

//...............
byte Byte(ptr Address)
{
  byte val;
  
  if (!Dbg::MemRead(Address, addressof(val), typesize("byte")))
    return 0;
  
  return val;
}

success MakeName(long ea, string name) { return MakeNameEx(ea,name,1); }

success MakeNameEx(long ea, string name, long flags)
{
  return false;
}