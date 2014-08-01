#include "../stdafx.h"

namespace Script
{
namespace Global
{
	int StringTypeId;

	void Init(asIScriptEngine *Engine)
	{
		// Register generic functions and types
		Engine->SetDefaultNamespace("");

		//
		// TYPES
		//
		VERIFY(Engine->RegisterTypedef("byte", "uint8"));
		VERIFY(Engine->RegisterTypedef("handle", "uint"));

#ifdef _WIN64_
		VERIFY(Engine->RegisterTypedef("ptr", "uint64"));
#else
		VERIFY(Engine->RegisterTypedef("ptr", "uint"));
#endif

		StringTypeId = Engine->GetTypeIdByDecl("string");

		//
		// FUNCTIONS
		//
		VERIFY(Engine->RegisterGlobalFunction("string typeof(?&in)", asFUNCTION(asTypeof), asCALL_GENERIC));
		VERIFY(Engine->RegisterGlobalFunction("uint typesize(string &in Type)", asFUNCTION(asTypesize), asCALL_GENERIC));
		VERIFY(Engine->RegisterGlobalFunction("ptr addressof(?&in)", asFUNCTION(asAddressof), asCALL_GENERIC));

		VERIFY(Engine->RegisterGlobalFunction("string sprintf(string &in format, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null)", asFUNCTION(asSprintf), asCALL_GENERIC));
		VERIFY(Engine->RegisterGlobalFunction("void printf(string &in format, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null)", asFUNCTION(asPrintf), asCALL_GENERIC));
	}

	void asTypeof(asIScriptGeneric *Gen)
	{
		std::string typeName(Gen->GetEngine()->GetTypeDeclaration(Gen->GetArgTypeId(0)));

		Gen->SetReturnObject(&typeName);
	}

	void asTypesize(asIScriptGeneric *Gen)
	{
		// Argument is a string
		std::string *arg = (std::string *)Gen->GetArgObject(0);

		// Type name -> type id
		int typeId = Gen->GetEngine()->GetTypeIdByDecl(arg->c_str());

		// Set the return value to the type size
		Gen->SetReturnDWord(Gen->GetEngine()->GetSizeOfPrimitiveType(typeId));
	}

	void asAddressof(asIScriptGeneric *Gen)
	{
		// Get a pointer to the argument and return it
		Gen->SetReturnDWord((asDWORD)Gen->GetAddressOfArg(0));
	}

	void asSprintf(asIScriptGeneric *Gen)
	{
		// Format is the first parameter
		std::string *format = static_cast<std::string *>(Gen->GetArgObject(0));

		// Buffer to store arguments for vsnprintf_s
		char data[256];
		asParseVarArgs(Gen, 1, data, sizeof(data));

		char buf[1024];
		vsnprintf_s(buf, _TRUNCATE, format->c_str(), (va_list)&data);

		// Set the return data
		std::string ret(buf);

		Gen->SetReturnObject(&ret);
	}

	void asPrintf(asIScriptGeneric *Gen)
	{
		// Format is the first parameter
		std::string *format = static_cast<std::string *>(Gen->GetArgObject(0));

		// Buffer to store arguments for vsnprintf_s
		char data[256];
		asParseVarArgs(Gen, 1, data, sizeof(data));

		char buf[1024];
		vsnprintf_s(buf, _TRUNCATE, format->c_str(), (va_list)&data);

		// Send the string to the log window
		_plugin_logprintf(buf);
	}

	void asParseVarArgs(asIScriptGeneric *Gen, int ArgIndex, char *Buffer, size_t Size)
	{
		// Zero any data
		memset(Buffer, 0, Size);

		// Convert each parameter to the va_list form
		va_list va = (va_list)Buffer;

		for (int i = ArgIndex; i < Gen->GetArgCount(); i++)
		{
			PVOID addr	= *(PVOID *)Gen->GetAddressOfArg(i);
			int type	= Gen->GetArgTypeId(i);
			int size	= Gen->GetEngine()->GetSizeOfPrimitiveType(type);

			switch (type)
			{
			case asTYPEID_VOID:
				break;

			case asTYPEID_BOOL:
			case asTYPEID_UINT8:
			case asTYPEID_INT8:
			case asTYPEID_UINT16:
			case asTYPEID_INT16:
			case asTYPEID_UINT32:
			case asTYPEID_INT32:
			case asTYPEID_UINT64:
			case asTYPEID_INT64:
			case asTYPEID_FLOAT:
			case asTYPEID_DOUBLE:
				memcpy(va, addr, size);
				va += size;
				break;

			default:
				if (type == StringTypeId)
					va_arg(va, const char *) = ((std::string *)addr)->c_str();
				else
					va_arg(va, PVOID) = *(PVOID *)addr;
				break;
			}
		}
	}
}
}