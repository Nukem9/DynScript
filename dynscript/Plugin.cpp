#include "stdafx.h"

int pluginHandle;
HWND hwndDlg;
int hMenu;

Script::ModuleDef def;

void InitDebugCallback(CBTYPE Type, PLUG_CB_INITDEBUG *Info)
{
	std::string file(Info->szFileName);

	// void OnInitDebug(string &in File)
	auto context = Script::CreateContext(def.asOnInitDebug);
	context->SetArgObject(0, &file);
	context->Execute();
}

void StopDebugCallback(CBTYPE Type, PLUG_CB_STOPDEBUG *Info)
{
	// void OnStopDebug()
	auto context = Script::CreateContext(def.asOnStopDebug);
	context->Execute();
}

void CreateProcessCallback(CBTYPE Type, PLUG_CB_CREATEPROCESS *Info)
{
}

void ExitProcessCallback(CBTYPE Type, PLUG_CB_EXITPROCESS *Info)
{
	// void OnExitProcess(uint ExitCode)
	auto context = Script::CreateContext(def.asOnExitProcess);
	context->SetArgDWord(0, Info->ExitProcess->dwExitCode);
	context->Execute();
}

void CreateThreadCallback(CBTYPE Type, PLUG_CB_CREATETHREAD *Info)
{
}

void ExitThreadCallback(CBTYPE Type, PLUG_CB_EXITTHREAD *Info)
{
	// void OnExitThread(uint ThreadId, uint ExitCode)
	auto context = Script::CreateContext(def.asOnExitThread);
	context->SetArgDWord(0, Info->dwThreadId);
	context->SetArgDWord(1, Info->ExitThread->dwExitCode);
	context->Execute();
}

void SystemBreakpointCallback(CBTYPE Type, PLUG_CB_SYSTEMBREAKPOINT *Info)
{
	// void OnSystemBreakpoint()
	auto context = Script::CreateContext(def.asOnSystemBreakpoint);
	context->Execute();
}

void LoadDllCallback(CBTYPE Type, PLUG_CB_LOADDLL *Info)
{
}

void UnloadDllCallback(CBTYPE Type, PLUG_CB_UNLOADDLL *Info)
{
	// void OnUnloadDll(ptr DllBase)
	auto context = Script::CreateContext(def.asOnUnloadDll);
	context->SetArgPointerVal(0, (asDWORD)Info->UnloadDll->lpBaseOfDll);
	context->Execute();
}

void DebugStringCallback(CBTYPE Type, PLUG_CB_OUTPUTDEBUGSTRING *Info)
{
	std::string message(Info->DebugString->lpDebugStringData);

	// void OnOutputDebugString(string &in Message)
	auto context = Script::CreateContext(def.asOnOutputDebugString);
	context->SetArgObject(0, &message);
	context->Execute();
}

void ExceptionCallback(CBTYPE Type, PLUG_CB_EXCEPTION *Info)
{
}

void BreakpointCallback(CBTYPE Type, PLUG_CB_BREAKPOINT *Info)
{
	std::string name(Info->breakpoint->name);
	std::string mod(Info->breakpoint->mod);

	// void OnBreakpoint(int Type, ptr Address, string &in Name, string &in Module)
	auto context = Script::CreateContext(def.asOnBreakpoint);
	context->SetArgDWord(0,	Info->breakpoint->type);
	context->SetArgPointerVal(1, Info->breakpoint->addr);
	context->SetArgObject(2, &name);
	context->SetArgObject(3, &mod);
	context->Execute();
}

void PauseCallback(CBTYPE Type, PLUG_CB_PAUSEDEBUG *Info)
{
	// void OnPauseDebug()
	auto context = Script::CreateContext(def.asOnPauseDebug);
	context->Execute();
}

void ResumeCallback(CBTYPE Type, PLUG_CB_RESUMEDEBUG *Info)
{
	// void OnResumeDebug()
	auto context = Script::CreateContext(def.asOnResumeDebug);
	context->Execute();
}

void SteppedCallback(CBTYPE Type, PLUG_CB_STEPPED *Info)
{
	// void OnStepped()
	auto context = Script::CreateContext(def.asOnStepped);
	context->Execute();
}

void AttachCallback(CBTYPE Type, PLUG_CB_ATTACH *Info)
{
	// void OnAttach(uint ProcessId)
	auto context = Script::CreateContext(def.asOnAttach);
	context->SetArgDWord(0, Info->dwProcessId);
	context->Execute();
}

void DetachCallback(CBTYPE Type, PLUG_CB_DETACH *Info)
{
	// void OnDetach(uint ProcessId, uint ThreadId)
	auto context = Script::CreateContext(def.asOnDetach);
	context->SetArgDWord(0, Info->fdProcessInfo->dwProcessId);
	context->SetArgDWord(1, Info->fdProcessInfo->dwThreadId);
	context->Execute();
}

void DebugEventCallback(CBTYPE Type, PLUG_CB_DEBUGEVENT *Info)
{
}

void MenuEntryCallback(CBTYPE Type, PLUG_CB_MENUENTRY *Info)
{
	// void OnMenuEvent(int Entry)
	auto context = Script::CreateContext(def.asOnMenuEvent);
	context->SetArgDWord(0, Info->hEntry);
	context->Execute();
}

DLL_EXPORT bool pluginit(PLUG_INITSTRUCT *InitStruct)
{
	InitStruct->pluginVersion	= PLUGIN_VERSION;
	InitStruct->sdkVersion		= PLUG_SDKVERSION;
	pluginHandle				= InitStruct->pluginHandle;
	strcpy_s(InitStruct->pluginName, PLUGIN_NAME);

	// Initialize the scripting engine
	char dir[MAX_PATH];
	GetCurrentDirectoryA(sizeof(dir), dir);
	strcat_s(dir, "\\test.as");
	
	if (!Script::EngineInit())
	{
		_plugin_printf("Script engine initialization failed!\n");
		return false;
	}

	Script::EngineLoad(&def, dir);

	// Add any of the callbacks
	_plugin_registercallback(pluginHandle, CB_INITDEBUG,			(CBPLUGIN)InitDebugCallback);
	_plugin_registercallback(pluginHandle, CB_STOPDEBUG,			(CBPLUGIN)StopDebugCallback);
	_plugin_registercallback(pluginHandle, CB_CREATEPROCESS,		(CBPLUGIN)CreateProcessCallback);
	_plugin_registercallback(pluginHandle, CB_EXITPROCESS,			(CBPLUGIN)ExitProcessCallback);
	_plugin_registercallback(pluginHandle, CB_CREATETHREAD,			(CBPLUGIN)CreateThreadCallback);
	_plugin_registercallback(pluginHandle, CB_EXITTHREAD,			(CBPLUGIN)ExitThreadCallback);
	_plugin_registercallback(pluginHandle, CB_SYSTEMBREAKPOINT,		(CBPLUGIN)SystemBreakpointCallback);
	_plugin_registercallback(pluginHandle, CB_LOADDLL,				(CBPLUGIN)LoadDllCallback);
	_plugin_registercallback(pluginHandle, CB_UNLOADDLL,			(CBPLUGIN)UnloadDllCallback);
	_plugin_registercallback(pluginHandle, CB_OUTPUTDEBUGSTRING,	(CBPLUGIN)DebugStringCallback);
	_plugin_registercallback(pluginHandle, CB_EXCEPTION,			(CBPLUGIN)ExceptionCallback);
	_plugin_registercallback(pluginHandle, CB_BREAKPOINT,			(CBPLUGIN)BreakpointCallback);
	_plugin_registercallback(pluginHandle, CB_PAUSEDEBUG,			(CBPLUGIN)PauseCallback);
	_plugin_registercallback(pluginHandle, CB_RESUMEDEBUG,			(CBPLUGIN)ResumeCallback);
	_plugin_registercallback(pluginHandle, CB_STEPPED,				(CBPLUGIN)SteppedCallback);
	_plugin_registercallback(pluginHandle, CB_ATTACH,				(CBPLUGIN)AttachCallback);
	_plugin_registercallback(pluginHandle, CB_DETACH,				(CBPLUGIN)DetachCallback);
	_plugin_registercallback(pluginHandle, CB_DEBUGEVENT,			(CBPLUGIN)DebugEventCallback);
	_plugin_registercallback(pluginHandle, CB_MENUENTRY,			(CBPLUGIN)MenuEntryCallback);

	return true;
}

DLL_EXPORT bool plugstop()
{
	_plugin_menuclear(hMenu);

	return true;
}

DLL_EXPORT void plugsetup(PLUG_SETUPSTRUCT *SetupStruct)
{
	hwndDlg = SetupStruct->hwndDlg;
	hMenu	= SetupStruct->hMenu;

	// Initialize the menu
	_plugin_menuaddentry(hMenu, 0, "Load script");

	// void OnPluginSetup(int DebuggerVersion, int PluginVersion)
	auto context = Script::CreateContext(def.asOnPluginSetup);
	context->SetArgDWord(0, 20);
	context->SetArgDWord(1, PLUGIN_VERSION);
	context->Execute();
}