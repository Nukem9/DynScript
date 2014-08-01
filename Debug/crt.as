//
//
//
// Simple example defining some of the common runtime functions,
// showing usage with IDA's IDC types.
//
//
//

#include "ida.as"

void memcpy(long to, long from, long size)
{
  Dbg::MemWrite(to, from, size);
}

void memset(long to, byte value, long size)
{
  Dbg::MemFill(to, value, size);
}