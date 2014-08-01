#include "ida.as"
#include "crt.as"

void OnPluginSetup(int DebuggerVersion, int PluginVersion)
{
  GUI::AddMenuSeparator(GUI::MenuHandle);
  GUI::AddMenuEntry(GUI::MenuHandle, 1, "Test");

  printf("OnPluginSetup(%d, %d)\n", DebuggerVersion, PluginVersion);
}

void OnInitDebug(string &in File)
{
  printf("OnInitdebug(%s)\n", File);
}

void OnStopDebug()
{
  printf("OnStopDebug()\n");
}

void OnCreateProcess(int param)
{
  printf("OnCreateProcess()\n");
}

void OnExitProcess(uint ExitCode)
{
  printf("OnExitProcess(0x%X)\n", ExitCode);
}

void OnCreateThread(int param)
{
  printf("OncreateThread()\n");
}

void OnExitThread(uint ThreadId, uint ExitCode)
{
  printf("OnExitThread(0x%X, 0x%X)\n", ThreadId, ExitCode);
}

void OnSystemBreakpoint()
{
  printf("OnSystemBreakpoint()\n");
  printf("EAX = 0x%X\n", Dbg::ValFromString("eax"));
  printf("EBX = 0x%X\n", Dbg::ValFromString("ebx"));
  printf("ECX = 0x%X\n", Dbg::ValFromString("ecx"));
  printf("EDX = 0x%X\n", Dbg::ValFromString("edx"));
  printf("EBP = 0x%X\n", Dbg::ValFromString("ebp"));
  printf("ESP = 0x%X\n", Dbg::ValFromString("esp"));
  printf("ESI = 0x%X\n", Dbg::ValFromString("esi"));
  printf("EDI = 0x%X\n", Dbg::ValFromString("edi"));
  printf("EIP = 0x%X\n", Dbg::ValFromString("eip"));
}

void OnLoadDll(int param)
{
  printf("OnLoadDll()\n");
}

void OnUnloadDll(ptr DllBase)
{
  printf("OnUnloadDll(0x%llX)\n", DllBase);
}

void OnOutputDebugString(string &in Message)
{
  printf("OnOutputDebugString(%s)\n", Message);
}

void OnException(int param)
{
  printf("OnException()\n");
}

void OnBreakpoint(int Type, ptr Address, string &in Name, string &in Module)
{
  // TODO: Add BPTYPE enum
  printf("OnBreakpoint(%d, 0x%llX, %s, %s)\n", Type, Address, Name, Module);
}

void OnPauseDebug()
{
  printf("OnPauseDebug()\n");
}

void OnResumeDebug()
{
  printf("OnResumeDebug()\n");
}

void OnStepped()
{
  printf("OnStepped()\n");
}

void OnAttach(uint ProcessId)
{
  printf("OnAttach(0x%X)\n", ProcessId);
}

void OnDetach(uint ProcessId, uint ThreadId)
{
  printf("OnDetach(0x%X, 0x%X)\n", ProcessId, ThreadId);
}

void OnDebugEvent(uint ProcessId, uint ThreadId, uint Code, ptr)
{
  printf("OnDebugEvent()\n");
}

void OnMenuEvent(int Entry)
{
  printf("OnMenuEvent(%d)\n", Entry);
}