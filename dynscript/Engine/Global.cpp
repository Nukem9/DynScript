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
		VERIFY(StringTypeId = Engine->GetTypeIdByDecl("string"));

		VERIFY(Engine->RegisterTypedef("byte", "uint8"));
		VERIFY(Engine->RegisterTypedef("word", "uint16"));
		VERIFY(Engine->RegisterTypedef("dword", "uint"));
		VERIFY(Engine->RegisterTypedef("qword", "uint64"));

#ifdef _WIN64
		VERIFY(Engine->RegisterTypedef("ptr", "uint64"));
		VERIFY(Engine->RegisterTypedef("handle", "uint64"));
#else
		VERIFY(Engine->RegisterTypedef("ptr", "uint"));
		VERIFY(Engine->RegisterTypedef("handle", "uint"));
#endif

		//
		// FUNCTIONS
		//
		VERIFY(Engine->RegisterGlobalFunction("string typeof(?&in)", asFUNCTION(asTypeof), asCALL_GENERIC));
		VERIFY(Engine->RegisterGlobalFunction("uint sizeof(?&in)", asFUNCTION(asSizeof), asCALL_GENERIC));
		VERIFY(Engine->RegisterGlobalFunction("uint typesize(string &in Type)", asFUNCTION(asTypesize), asCALL_GENERIC));
		VERIFY(Engine->RegisterGlobalFunction("ptr addressof(?&in)", asFUNCTION(asAddressof), asCALL_GENERIC));

		VERIFY(Engine->RegisterGlobalFunction("string sprintf(string &in format, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null)", asFUNCTION(asSprintf), asCALL_GENERIC));
		VERIFY(Engine->RegisterGlobalFunction("void printf(string &in format, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null)", asFUNCTION(asPrintf), asCALL_GENERIC));
	}

	void asTypeof(asIScriptGeneric *Gen)
	{
		// Variable type -> type decl string
		std::string typeName(Gen->GetEngine()->GetTypeDeclaration(Gen->GetArgTypeId(0)));

		Gen->SetReturnObject(&typeName);
	}

	void asSizeof(asIScriptGeneric *Gen)
	{
		// Variable type -> variable id
		int typeId = Gen->GetArgTypeId(0);

		// Set the return value to the type size
		Gen->SetReturnDWord(Gen->GetEngine()->GetSizeOfPrimitiveType(typeId));
	}

	void asTypesize(asIScriptGeneric *Gen)
	{
		// Argument is a string
		std::string *arg = static_cast<std::string *>(Gen->GetArgObject(0));

		// Type name -> type id
		int typeId = Gen->GetEngine()->GetTypeIdByDecl(arg->c_str());

		// Set the return value to the type size
		Gen->SetReturnDWord(Gen->GetEngine()->GetSizeOfPrimitiveType(typeId));
	}

	void asAddressof(asIScriptGeneric *Gen)
	{
		// Get a pointer to the argument and return it
		Gen->SetReturnPointerVal(Gen->GetAddressOfArg(0));
	}

	void asSprintf(asIScriptGeneric *Gen)
	{
		char buf[2048];
		asParseFormat(Gen, 0, buf, ARRAYSIZE(buf));

		// Set the return data
		std::string ret(buf);

		Gen->SetReturnObject(&ret);
	}

	void asPrintf(asIScriptGeneric *Gen)
	{
		char buf[2048];
		asParseFormat(Gen, 0, buf, ARRAYSIZE(buf));

		// Send the string to the log window
		_plugin_logprintf(buf);
	}

	void asParseFormat(asIScriptGeneric *Gen, int FormatArgIndex, char *Buffer, size_t Size)
	{
		// Format is the first parameter
		std::string *format = static_cast<std::string *>(Gen->GetArgObject(0));

		// Buffer to store arguments for vsnprintf_s
		char data[256];
		asParseVarArgs(Gen, 1, data, sizeof(data));

		// Send back to caller
		vsnprintf_s(Buffer, Size, _TRUNCATE, format->c_str(), (va_list)&data);
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

			// Custom type macro to shorten code
#define MAKE_TYPE(id, type)						\
			case id:							\
			assert(size == sizeof(type));		\
			va_arg(va, type) = *(type *)addr;	\
			break

			// Handle each specific anglescript arg with the va_arg api
			switch (type)
			{
			MAKE_TYPE(asTYPEID_BOOL,	bool);
			MAKE_TYPE(asTYPEID_UINT8,	unsigned __int8);
			MAKE_TYPE(asTYPEID_INT8,	__int8);
			MAKE_TYPE(asTYPEID_UINT16,	unsigned __int16);
			MAKE_TYPE(asTYPEID_INT16,	__int16);
			MAKE_TYPE(asTYPEID_UINT32,	unsigned __int32);
			MAKE_TYPE(asTYPEID_INT32,	__int32);
			MAKE_TYPE(asTYPEID_UINT64,	unsigned __int64);
			MAKE_TYPE(asTYPEID_INT64,	__int64);
			MAKE_TYPE(asTYPEID_FLOAT,	float);
			MAKE_TYPE(asTYPEID_DOUBLE,	double);

			// Non-value type
			case asTYPEID_VOID:
				assert(false);
				break;

			default:
			{
				// std::string
				if (type == StringTypeId)
					va_arg(va, const char *) = ((std::string *)addr)->c_str();
				else
					assert(false);
			}
			break;
			}
#undef MAKE_TYPE
		}
	}
}
}