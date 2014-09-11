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

void OnCreateProcess(CREATE_PROCESS_DEBUG_INFO &in Info, IMAGEHLP_MODULE64 &in Module, PROCESS_INFORMATION &in ProcessInfo)
{
  printf("OnCreateProcess()\n");
}

void OnExitProcess(EXIT_PROCESS_DEBUG_INFO &in Info)
{
  printf("OnExitProcess(0x%X)\n", Info.dwExitCode);
}

void OnCreateThread(CREATE_THREAD_DEBUG_INFO &in Info, uint ThreadId)
{
  printf("OncreateThread(0x%X)\n", ThreadId);
}

void OnExitThread(EXIT_THREAD_DEBUG_INFO &in Info, uint ThreadId)
{
  printf("OnExitThread(0x%X, 0x%X)\n", Info.dwExitCode, ThreadId);
}

void OnSystemBreakpoint()
{
  printf("OnSystemBreakpoint()\n");
  
  REGDUMP dump;
  Dbg::GetRegDump(dump);
  
  printf("EAX = 0x%X\n", dump.cax);
  printf("EBX = 0x%X\n", dump.cbx);
  printf("ECX = 0x%X\n", dump.ccx);
  printf("EDX = 0x%X\n", dump.cdx);
  printf("EBP = 0x%X\n", dump.cbp);
  printf("ESP = 0x%X\n", dump.csp);
  printf("ESI = 0x%X\n", dump.csi);
  printf("EDI = 0x%X\n", dump.cdi);
  printf("EIP = 0x%X\n", dump.cip);
  printf("\n");
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

void OnLoadDll(LOAD_DLL_DEBUG_INFO &in Info, IMAGEHLP_MODULE64 &in Module, string &in ModName)
{
  printf("OnLoadDll(0x%p)\n", Info.lpBaseOfDll);
}

void OnUnloadDll(UNLOAD_DLL_DEBUG_INFO &in Info)
{
  printf("OnUnloadDll(0x%p)\n", Info.lpBaseOfDll);
}

void OnOutputDebugString(OUTPUT_DEBUG_STRING_INFO &in Info, string &in Message)
{
  printf("OnOutputDebugString(%s)\n", Message);
}

void OnException(EXCEPTION_DEBUG_INFO &in Info)
{
  printf("OnException(0x%p)\n", Info.ExceptionRecord.ExceptionAddress);
}

void OnBreakpoint(BRIDGEBP &in Info)
{
  printf("OnBreakpoint(%d, 0x%p, %s, %s)\n", Info.type, Info.addr, Info.name, Info.mod);
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

void OnDetach(PROCESS_INFORMATION &in Info)
{
  printf("OnDetach(0x%X, 0x%X)\n", Info.dwProcessId, Info.dwThreadId);
}

void OnDebugEvent(DEBUG_EVENT &in Info)
{
  // printf("OnDebugEvent()\n");
}

void OnMenuEvent(int Entry)
{
  printf("OnMenuEvent(%d)\n", Entry);
}