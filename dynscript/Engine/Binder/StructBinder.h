#pragma once

#define AS_BEGIN_STRUCT(decl)		{ \
									static const char *__zname = #decl; \
									using __zdecl = decl; \
									VERIFY(Engine->RegisterObjectType(#decl, sizeof(decl), asOBJ_VALUE | asOBJ_POD));
#define AS_END_STRUCT()				}

#define AS_ADD_STRUCT(type, member)					VERIFY(Engine->RegisterObjectProperty(																	\
																					__zname,																\
																					#type " " #member,														\
																					asOFFSET(__zdecl, member)));

#define AS_ADD_STRUCT_ARRAY(type, member, func)		VERIFY(Engine->RegisterObjectMethod(																	\
																					__zname,																\
																					#type " get_" #member "(uint)",											\
																					asFUNCTION(to_function_pointer([](asUINT Index, __zdecl *Obj) func)),	\
																					asCALL_CDECL_OBJLAST));

#define AS_ADD_STRUCT_ACCESS(type, member, func)	VERIFY(Engine->RegisterObjectMethod(																	\
																					__zname,																\
																					#type " get_" #member "()",												\
																					asFUNCTION(to_function_pointer([](asUINT Index, __zdecl *Obj) func)),	\
																					asCALL_CDECL_OBJLAST));