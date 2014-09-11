#pragma once

#define STR_GET(x)	{ return std::string(Obj->x); }
#define STR_SET(x)	{ strcpy_s(Obj->x, Val.c_str()); }

#define AS_BEGIN_STRUCT(decl)		{																					\
									static const char *__zname = #decl;													\
									using objDecl = decl;																\
									VERIFY(Engine->RegisterObjectType(#decl, sizeof(decl), asOBJ_VALUE | asOBJ_POD));

#define AS_END_STRUCT()				}

#define AS_STRUCT_ADD(type, member)							VERIFY(Engine->RegisterObjectProperty(	\
																__zname,							\
																#type " " #member,					\
																asOFFSET(objDecl, member)));

//
// READ from a struct array:
// val = struct.array[index];
//
#define AS_STRUCT_ADD_ARRAY(type, member, func)				VERIFY(Engine->RegisterObjectMethod(				\
																__zname,										\
																#type " get_" #member "(uint)",					\
																asLAMBDA([](asUINT Index, objDecl *Obj) func),	\
																asCALL_CDECL_OBJLAST));

//
// READ from and WRITE to a struct member:
// val = struct.member;
// struct.member = val;
//
#define AS_STRUCT_ACCESS(type, member, readf, writef)		AS_STRUCT_ACCESS_MOD(type, member, "", readf, writef)

#define AS_STRUCT_ACCESS_MOD(type, member, mod, readf, writef)											\
															VERIFY(Engine->RegisterObjectMethod(			\
																__zname,									\
																#type mod " get_" #member "()",				\
																asLAMBDA([](objDecl *Obj) readf),			\
																asCALL_CDECL_OBJLAST));						\
															VERIFY(Engine->RegisterObjectMethod(			\
																__zname,									\
																"void set_" #member "(" #type " &in)",		\
																asLAMBDA([](type &Val, objDecl *Obj) writef), \
																asCALL_CDECL_OBJLAST));