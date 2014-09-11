#pragma once

namespace Script
{
namespace Global
{
	void Init(asIScriptEngine *Engine);

	// AngelScript functions
	void asTypeof(asIScriptGeneric *Gen);
	void asSizeof(asIScriptGeneric *Gen);
	void asTypesize(asIScriptGeneric *Gen);
	void asAddressof(asIScriptGeneric *Gen);

	void asSprintf(asIScriptGeneric *Gen);
	void asPrintf(asIScriptGeneric *Gen);
	void asParseVarArgs(asIScriptGeneric *Gen, int ArgIndex, char *Buffer, size_t Size);
}
}