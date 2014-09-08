#pragma once

typedef PVOID **OBJECT;

#pragma warning(push)
#pragma warning(disable : 4244)
template<typename T>
FORCEINLINE int asAddArgDynamic(asIScriptContext *Context, int Index, T Argument)
{
	//
	// These checks are resolved at compile time
	//
	static_assert(!std::is_pointer<T>::value, "asAddArgDynamic<T> doesn't support pointers!");

	if (std::is_same<T, bool>::value ||
		std::is_same<T, char>::value ||
		std::is_same<T, unsigned char>::value
		)
		return Context->SetArgByte(Index, Argument);

	if (std::is_same<T, short>::value ||
		std::is_same<T, unsigned short>::value
		)
		return Context->SetArgWord(Index, Argument);

	if (std::is_same<T, int>::value ||
		std::is_same<T, unsigned int>::value ||
		std::is_same<T, long>::value ||
		std::is_same<T, unsigned long>::value
		)
		return Context->SetArgDWord(Index, Argument);

	if (std::is_same<T, __int64>::value ||
		std::is_same<T, unsigned __int64>::value ||
		std::is_same<T, long long>::value ||
		std::is_same<T, unsigned long long>::value
		)
		return Context->SetArgQWord(Index, Argument);

	if (std::is_same<T, float>::value)
		return Context->SetArgFloat(Index, Argument);

	if (std::is_same<T, double>::value)
		return Context->SetArgDouble(Index, Argument);

	//
	// Verify that the parameter was handled
	//
	static_assert((std::is_same<T, bool>::value ||
		std::is_same<T, char>::value ||
		std::is_same<T, unsigned char>::value ||
		std::is_same<T, short>::value ||
		std::is_same<T, unsigned short>::value ||
		std::is_same<T, int>::value ||
		std::is_same<T, unsigned int>::value ||
		std::is_same<T, long>::value ||
		std::is_same<T, unsigned long>::value ||
		std::is_same<T, __int64>::value ||
		std::is_same<T, unsigned __int64>::value ||
		std::is_same<T, long long>::value ||
		std::is_same<T, unsigned long long>::value ||
		std::is_same<T, float>::value ||
		std::is_same<T, double>::value
		), "Unsupported dynamic type detected!");

	return -1;
}

FORCEINLINE int asAddArgDynamic(asIScriptContext *Context, int Index, OBJECT Argument)
{
	return Context->SetArgObject(Index, (PVOID)Argument);
}
#pragma warning(pop)

template<typename T>
void AddArguments(asIScriptContext *Context, int Index, const T& Value)
{
	VERIFY(asAddArgDynamic(Context, Index, Value));
}

template<typename T, typename... Args>
void AddArguments(asIScriptContext *Context, int Index, const T& Head, const Args&... Tail)
{
	VERIFY(asAddArgDynamic(Context, Index, Head));

	AddArguments(Context, Index + 1, Tail...);
}

template<typename... Args>
asIScriptContext *asExecuteDynamic(asIScriptFunction *Function, Args&&... Parameters)
{
	auto context = Script::CreateContext(Function);

	//
	// Verify that the number of parameters matches the one in the script code
	//
	assert(Function);
	assert(Function->GetParamCount() == sizeof...(Parameters));

	//
	// Pass parameters and execute the function
	//
	if (context)
	{
		AddArguments(context, 0, Parameters...);

		context->Execute();
	}

	return context;
}

static asIScriptContext *asExecuteDynamic(asIScriptFunction *Function)
{
	auto context = Script::CreateContext(Function);

	if (context)
		context->Execute();

	return context;
}