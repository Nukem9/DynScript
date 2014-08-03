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
[Prototype; const int PluginHandle]
[Prototype; const handle ProcessHandle]

[Prototype; ptr ValFromString(string &in Value)]
[Prototype; bool MemWrite(ptr Address, ptr Buffer, uint Size, uint &out BytesWritten = 0)]
[Prototype; bool MemRead(ptr Address, ptr Buffer, uint Size, uint &out BytesRead = 0)]
[Prototype; bool MemFill(ptr Address, byte Value, uint Size)]
}

namespace GUI
{
[Prototype; const int MenuHandle]

[Prototype; int AddSubmenu(int Handle, string &in Title)]
[Prototype; bool AddMenuEntry(int Handle, int Entry, string &in Title)]
[Prototype; bool AddMenuSeparator(int Handle)]
[Prototype; bool ClearMenu(int Handle)]
}

///////////////////////////////
// SCRIPT DEFINED PROTOTYPES //
// ************************* //
///////////////////////////////

// strlen
[Param value;   Source string]
[Returns;       Length of string]
uint strlen(string &in value)
{
  return value.length();
}

// memcpy
[Param to;      Destination address]
[Param from;    Source address]
[Param size;    Number of bytes to copy]
[Returns;       Nothing]
void memcpy(ptr to, ptr from, ptr size)
{
  // This occurs in the REMOTE process
  Dbg::MemWrite(to, from, size);
}

// memset
[Param to;      Destination address]
[Param value;   Byte value to fill the memory]
[Param size;    Number of bytes to fill]
[Returns;       Nothing]
void memset(ptr to, byte value, uint size)
{
  // This occurs in the REMOTE process
  Dbg::MemFill(to, value, size);
}

// IsBadReadPtr
[Param addr;  Address to check]
[Returns;     Success: false, Failure: true]
bool IsBadReadPtr(ptr addr)
{
  ptr val;
  
  return !Dbg::MemRead(addr, addressof(val), typesize("ptr"));
}

// IsBadWritePtr
[Param addr;  Address to check]
[Returns;     Success: false, Failure: true]
bool IsBadWritePtr(ptr addr)
{
  ptr val;
  
  if (!Dbg::MemRead(addr, addressof(val), typesize("ptr")))
    return true;
    
  return !Dbg::MemWrite(addr, addressof(val), typesize("ptr"));
}