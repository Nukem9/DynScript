#pragma once

#define AS_BEGIN_STRUCT(decl)		{ \
									static const char *__zname = #decl; \
									using __zdecl = decl; \
									VERIFY(Engine->RegisterObjectType(#decl, sizeof(decl), asOBJ_VALUE | asOBJ_POD));

#define AS_ADD_STRUCT(type, member)	VERIFY(Engine->RegisterObjectProperty(__zname, #type " " #member, asOFFSET(__zdecl, member)));

#define AS_END_STRUCT()				}