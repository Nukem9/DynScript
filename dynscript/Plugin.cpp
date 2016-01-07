#include "stdafx.h"

int pluginHandle;
HWND hwndDlg;
int hMenu;

#define ModuleCallback(function, ...)	Script::Db::ForeachModule([&](const Script::ModuleDef *Mod) \
										{															\
											asExecuteDynamic(Mod->function, __VA_ARGS__);			\
											return true;											\
										});

void InitDebugCallback(CBTYPE Type, PLUG_CB_INITDEBUG *Info)
{
	std::string file(Info->szFileName);

	// void OnInitDebug(string &in File)
	ModuleCallback(asOnInitDebug, (OBJECT)&file);
}

void StopDebugCallback(CBTYPE Type, PLUG_CB_STOPDEBUG *Info)
{
	// void OnStopDebug()
	ModuleCallback(asOnStopDebug);
}

void CreateProcessCallback(CBTYPE Type, PLUG_CB_CREATEPROCESS *Info)
{
	// void OnCreateProcess(CREATE_PROCESS_DEBUG_INFO &in Info, IMAGEHLP_MODULE64 &in Module, PROCESS_INFORMATION &in ProcessInfo)
	ModuleCallback(asOnCreateProcess, (OBJECT)Info->CreateProcessInfo, (OBJECT)Info->modInfo, (OBJECT)Info->fdProcessInfo);
}

void ExitProcessCallback(CBTYPE Type, PLUG_CB_EXITPROCESS *Info)
{
	// void OnExitProcess(EXIT_PROCESS_DEBUG_INFO &in Info)
	ModuleCallback(asOnExitProcess, (OBJECT)Info->ExitProcess);
}

void CreateThreadCallback(CBTYPE Type, PLUG_CB_CREATETHREAD *Info)
{
	// void OnCreateThread(CREATE_THREAD_DEBUG_INFO &in Info, uint ThreadId)
	ModuleCallback(asOnCreateThread, (OBJECT)Info->CreateThread, Info->dwThreadId);
}

void ExitThreadCallback(CBTYPE Type, PLUG_CB_EXITTHREAD *Info)
{
	// void OnExitThread(EXIT_THREAD_DEBUG_INFO &in Info, uint ThreadId)
	ModuleCallback(asOnExitThread, (OBJECT)Info->ExitThread, Info->dwThreadId);
}

void SystemBreakpointCallback(CBTYPE Type, PLUG_CB_SYSTEMBREAKPOINT *Info)
{
	// void OnSystemBreakpoint()
	ModuleCallback(asOnSystemBreakpoint);
}

void LoadDllCallback(CBTYPE Type, PLUG_CB_LOADDLL *Info)
{
	std::string modname(Info->modname);

	// void OnLoadDll(LOAD_DLL_DEBUG_INFO &in Info, IMAGEHLP_MODULE64 &in Module, string &in ModName)
	ModuleCallback(asOnLoadDll, (OBJECT)Info->LoadDll, (OBJECT)Info->modInfo, (OBJECT)&modname);
}

void UnloadDllCallback(CBTYPE Type, PLUG_CB_UNLOADDLL *Info)
{
	// void OnUnloadDll(UNLOAD_DLL_DEBUG_INFO &in Info)
	ModuleCallback(asOnUnloadDll, (OBJECT)Info->UnloadDll);
}

void DebugStringCallback(CBTYPE Type, PLUG_CB_OUTPUTDEBUGSTRING *Info)
{
	//
	// Convert from unicode to multi-byte if needed
	//
	char buffer[2048];
	memset(buffer, 0, sizeof(buffer));

	size_t maxlen = min(ARRAYSIZE(buffer) - 1, Info->DebugString->nDebugStringLength);

	if (Info->DebugString->fUnicode)
	{
		wchar_t wcharbuf[2048];

		if (DbgMemRead((duint)Info->DebugString->lpDebugStringData, (PUCHAR)wcharbuf, maxlen * sizeof(wchar_t)))
			wcstombs(buffer, wcharbuf, ARRAYSIZE(buffer));
	}
	else
	{
		DbgMemRead((duint)Info->DebugString->lpDebugStringData, (PUCHAR)buffer, maxlen);
	}

	//
	// Convert to script object
	//
	std::string message(buffer);

	// void OnOutputDebugString(OUTPUT_DEBUG_STRING_INFO &in Info, string &in Message)
	ModuleCallback(asOnOutputDebugString, (OBJECT)Info->DebugString, (OBJECT)&message);
}

void ExceptionCallback(CBTYPE Type, PLUG_CB_EXCEPTION *Info)
{
	// void OnException(EXCEPTION_DEBUG_INFO &in Info)
	ModuleCallback(asOnException, (OBJECT)Info->Exception);
}

void BreakpointCallback(CBTYPE Type, PLUG_CB_BREAKPOINT *Info)
{
	// void OnBreakpoint(BRIDGEBP &in Info)
	ModuleCallback(asOnBreakpoint, (OBJECT)Info->breakpoint);
}

void PauseCallback(CBTYPE Type, PLUG_CB_PAUSEDEBUG *Info)
{
	// void OnPauseDebug()
	ModuleCallback(asOnPauseDebug);
}

void ResumeCallback(CBTYPE Type, PLUG_CB_RESUMEDEBUG *Info)
{
	// void OnResumeDebug()
	ModuleCallback(asOnResumeDebug);
}

void SteppedCallback(CBTYPE Type, PLUG_CB_STEPPED *Info)
{
	// void OnStepped()
	ModuleCallback(asOnStepped);
}

void AttachCallback(CBTYPE Type, PLUG_CB_ATTACH *Info)
{
	// void OnAttach(uint ProcessId)
	ModuleCallback(asOnAttach, Info->dwProcessId);
}

void DetachCallback(CBTYPE Type, PLUG_CB_DETACH *Info)
{
	// void OnDetach(PROCESS_INFORMATION &in Info)
	ModuleCallback(asOnDetach, (OBJECT)Info->fdProcessInfo);
}

void DebugEventCallback(CBTYPE Type, PLUG_CB_DEBUGEVENT *Info)
{
	// void OnDebugEvent(DEBUG_EVENT &in Info)
	ModuleCallback(asOnDebugEvent, (OBJECT)Info->DebugEvent);
}

void MenuEntryCallback(CBTYPE Type, PLUG_CB_MENUENTRY *Info)
{
	// void OnMenuEvent(int Entry)
	ModuleCallback(asOnMenuEvent, Info->hEntry);
}

void MyCompleter(const char *text, char **entries, int *entryCount)
{
	for (int i = 0; i < *entryCount; i++)
	{
		char *temp = (char *)BridgeAlloc(sizeof(char) * 64);
		sprintf_s(temp, 64, "entry%i", i);

		entries[i] = temp;
	}
}

bool MyTest(const char *cmd)
{
	// Must be a valid command
	if (!cmd || strlen(cmd) <= 0)
		return false;

	// Check for a return value
	if (!strstr(cmd, "return"))
	{
		// If there's no return statement, expect a void
		ExecuteString(Script::EngInst, cmd);
	}
	else
	{
		// Assume returning integer
		int ret = 0;
		ExecuteString(Script::EngInst, cmd, &ret, asTYPEID_INT32);
		_plugin_printf("ret: %d\n", ret);
	}

	return true;
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

	Script::Db::LoadModule(dir);

	SCRIPTTYPEINFO info;
	strcpy_s(info.name, "AngelScript");
	info.id = 0;
	info.execute = MyTest;
	info.completeCommand = MyCompleter;
	GuiRegisterScriptLanguage(&info);

	// Add all of the callbacks
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
	ModuleCallback(asOnPluginSetup, DBG_VERSION, PLUGIN_VERSION);
}