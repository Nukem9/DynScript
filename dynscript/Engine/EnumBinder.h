#pragma once

#define AS_BEGIN_ENUM(decl)	{ \
							static const char *__zname = #decl; \
							VERIFY(Engine->RegisterEnum(#decl));

#define AS_ADD_ENUM(member)	VERIFY(Engine->RegisterEnumValue(__zname, #member, member));

#define AS_END_ENUM()		}