#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include <vector>
#include <functional>

////////////////////////////
// AngelScript /////////////
////////////////////////////
#include "angelscript/include/angelscript.h"
#include "angelscript/add-on/scriptstdstring/scriptstdstring.h"
#include "angelscript/add-on/scriptbuilder/scriptbuilder.h"

////////////////////////////
// X64DBG //////////////////
////////////////////////////
#define PLUGIN_NAME		"DynScript"
#define PLUGIN_VERSION	001

#ifdef _WIN64
#pragma comment(lib, "pluginsdk/x64dbg.lib")
#pragma comment(lib, "pluginsdk/x64bridge.lib")
#pragma comment(lib, "pluginsdk/TitanEngine/TitanEngine_x64.lib")
#pragma comment(lib, "pluginsdk/dbghelp/dbghelp_x64.lib")
#else
#pragma comment(lib, "pluginsdk/x32dbg.lib")
#pragma comment(lib, "pluginsdk/x32bridge.lib")
#pragma comment(lib, "pluginsdk/TitanEngine/TitanEngine_x86.lib")
#pragma comment(lib, "pluginsdk/dbghelp/dbghelp_x86.lib")
#endif

#define _plugin_printf(Format, ...) _plugin_logprintf("[" PLUGIN_NAME "] " Format, __VA_ARGS__)

#include "pluginsdk/_plugins.h"
#include "pluginsdk/bridgemain.h"
#include "pluginsdk/TitanEngine/TitanEngine.h"
#include "Plugin.h"

////////////////////////////
// ScriptEngine ////////////
////////////////////////////
#include "Engine/stdafx.h"