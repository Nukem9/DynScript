//
//
//
// Simple file defining some of the common runtime functions
// and internal application prototypes.
//
//
//

/////////////////////////
// INTERNAL PROTOTYPES //
// ******************* //
/////////////////////////

namespace Dbg
{
[Prototype: const int PluginHandle]

[Prototype: ptr  ValFromString(string &in Value)]
[Prototype: bool MemWrite(ptr Address, ptr Buffer, uint Size, uint &out BytesWritten = 0)]
[Prototype: bool MemRead(ptr Address, ptr Buffer, uint Size, uint &out BytesRead = 0)]
[Prototype: bool MemFill(ptr Address, byte Value, uint Size)]
}

namespace GUI
{
[Prototype: const int MenuHandle]

[Prototype: int  AddSubmenu(int Handle, string &in Title)]
[Prototype: bool AddMenuEntry(int Handle, int Entry, string &in Title)]
[Prototype: bool AddMenuSeparator(int Handle)]
[Prototype: bool ClearMenu(int Handle)]
}

[Enum: SEGMENTREG]
[
    SEG_DEFAULT,
    SEG_ES,
    SEG_DS,
    SEG_FS,
    SEG_GS,
    SEG_CS,
    SEG_SS
]

[Enum: BPXTYPE]
[
    bp_none=0,
    bp_normal=1,
    bp_hardware=2,
    bp_memory=4
]

///////////////////////////////
// SCRIPT DEFINED PROTOTYPES //
// ************************* //
///////////////////////////////

// strlen
[Parameter:   Value, Source string]
[Returns:     Length of string]
uint strlen(string &in value)
{
  return value.length();
}

// memcpy
[Parameter:   to,   Destination address]
[Parameter:   from, Source address]
[Parameter:   size, Number of bytes to copy]
[Returns:     Nothing]
void memcpy(ptr to, ptr from, ptr size)
{
  // This occurs in the REMOTE process
  Dbg::MemWrite(to, from, size);
}

// memset
[Parameter:   to,     Destination address]
[Parameter:   value,  Byte value to fill the memory]
[Parameter:   size,   Number of bytes to fill]
[Returns:     Nothing]
void memset(ptr to, byte value, uint size)
{
  // This occurs in the REMOTE process
  Dbg::MemFill(to, value, size);
}

// IsBadReadPtr
[Parameter:   addr, Address to check]
[Returns:     Success: false, Failure: true]
bool IsBadReadPtr(ptr addr)
{
  ptr val;
  
  return !Dbg::MemRead(addr, addressof(val), typesize("ptr"));
}

// IsBadWritePtr
[Parameter:   addr, Address to check]
[Returns:     Success: false, Failure: true]
bool IsBadWritePtr(ptr addr)
{
  ptr val;
  
  if (IsBadReadPtr(addr))
    return true;
    
  return !Dbg::MemWrite(addr, addressof(val), typesize("ptr"));
}

// AskYesNo
[Parameter:   message, Message shown to the user]
[Returns:     Yes: true, No: false]
bool AskYesNo(string message)
{
  if (Dbg::CmdExec("msgyn \"" + message + "\""))
    return Dbg::ValFromString("$result") == 1;
    
  return false;
}