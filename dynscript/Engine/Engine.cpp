#include "../stdafx.h"

namespace Script
{
	asIScriptEngine *EngInst;

	bool EngineInit()
	{
		// This can only be called once
		if (EngInst)
			return false;

		//
		// http://www.angelcode.com/angelscript/sdk/docs/manual/doc_hello_world.html
		// Create the script engine
		//
		EngInst = asCreateScriptEngine(ANGELSCRIPT_VERSION);

		if (!EngInst)
			return false;

		EngInst->SetEngineProperty(asEP_INCLUDE_JIT_INSTRUCTIONS, 0);
		EngInst->SetEngineProperty(asEP_OPTIMIZE_BYTECODE, 1);
		EngInst->SetEngineProperty(asEP_COMPILER_WARNINGS, 1);

		// Set the message callback to receive information on errors in human readable format
		VERIFY(EngInst->SetMessageCallback(asFUNCTION(EngineMessage), 0, asCALL_CDECL));

		//
		// Register everything
		//
		// Semi-built in
		//
		RegisterStdString(EngInst);

		// Custom
		Global::Init(EngInst);
		GUI::Init(EngInst);
		Dbg::Init(EngInst);

		// Reset the namespace
		EngInst->SetDefaultNamespace("");

		return true;
	}

	bool EngineLoad(ModuleDef *Module, const char *Path)
	{
		if (!EngInst)
			return false;

		CScriptBuilder builder;

#ifdef _WIN64
		builder.DefineWord("__X64__");
#else
		builder.DefineWord("__X86__");
#endif

		if (builder.StartNewModule(EngInst, "script") < 0)
		{
			// If the code fails here it is usually because there
			// is no more memory to allocate the module
			_plugin_printf("Unrecoverable error while starting a new module.\n");
			return false;
		}

		if (builder.AddSectionFromFile(Path) < 0)
			return false;

		if (builder.BuildModule() < 0)
			return false;

		// The module is built at this point
		// Get any script function handles
#define GetFunction(type, name, params) Module->as##name = Module->asModule->GetFunctionByDecl(#type " " #name " (" params ")");

		Module->asModule = builder.GetModule();

		GetFunction(void, OnPluginSetup,		"int DebuggerVersion, int PluginVersion");
		GetFunction(void, OnInitDebug,			"string &in File");
		GetFunction(void, OnStopDebug,			"");
		GetFunction(void, OnCreateProcess,		"CREATE_PROCESS_DEBUG_INFO &in Info, IMAGEHLP_MODULE64 &in Module, PROCESS_INFORMATION &in ProcessInfo");
		GetFunction(void, OnExitProcess,		"EXIT_PROCESS_DEBUG_INFO &in Info");
		GetFunction(void, OnCreateThread,		"CREATE_THREAD_DEBUG_INFO &in Info, uint ThreadId");
		GetFunction(void, OnExitThread,			"EXIT_THREAD_DEBUG_INFO &in Info, uint ThreadId");
		GetFunction(void, OnSystemBreakpoint,	"");
		GetFunction(void, OnLoadDll,			"LOAD_DLL_DEBUG_INFO &in Info, IMAGEHLP_MODULE64 &in Module, string &in ModName");
		GetFunction(void, OnUnloadDll,			"UNLOAD_DLL_DEBUG_INFO &in Info");
		GetFunction(void, OnOutputDebugString,	"OUTPUT_DEBUG_STRING_INFO &in Info, string &in Message");
		GetFunction(void, OnException,			"EXCEPTION_DEBUG_INFO &in Info");
		GetFunction(void, OnBreakpoint,			"BRIDGEBP &in Info");
		GetFunction(void, OnPauseDebug,			"");
		GetFunction(void, OnResumeDebug,		"");
		GetFunction(void, OnStepped,			"");
		GetFunction(void, OnAttach,				"uint ProcessId");
		GetFunction(void, OnDetach,				"PROCESS_INFORMATION &in Info");
		GetFunction(void, OnDebugEvent,			"DEBUG_EVENT &in Info");
		GetFunction(void, OnMenuEvent,			"int Entry");

#undef GetFunction

		return true;
	}

	void EngineMessage(const asSMessageInfo *msg, void *param)
	{
		const char *type = nullptr;

		switch (msg->type)
		{
		case asMSGTYPE_ERROR:		type = "ERR ";	break;
		case asMSGTYPE_WARNING:		type = "WARN";	break;
		case asMSGTYPE_INFORMATION: type = "INFO";	break;
		default:					type = "UNK ";	break;
		}

		_plugin_printf("AngelScript: %s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
	}

	asIScriptContext *CreateContext(asIScriptFunction *Function)
	{
		if (!EngInst)
			return nullptr;

		asIScriptContext *ctx = EngInst->CreateContext();
		ctx->Prepare(Function);

		return ctx;
	}
}