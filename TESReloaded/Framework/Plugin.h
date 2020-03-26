#pragma once

#if defined(NEWVEGAS)
class PluginInterface {
public:
	UInt32	Version;
	UInt32	GameVersion;
	UInt32	EditorVersion;
	UInt32	IsEditor;

	bool	(*RegisterCommand)(CommandInfo* info);
	void	(*SetOpcodeBase)(UInt32 opcode);
	void*	(*QueryInterface)(UInt32 id);
};

class PluginInfo {
public:
	enum {
		kInfoVersion = 1
	};

	UInt32		InfoVersion;
	const char* Name;
	UInt32		Version;
};
#elif defined(OBLIVION)
class PluginInterface {
public:
	UInt32	Version;
	UInt32	GameVersion;
	UInt32	EditorVersion;
	UInt32	IsEditor;

	bool	(*RegisterCommand)(CommandInfo* info);
	void	(*SetOpcodeBase)(UInt32 opcode);
	void*	(*QueryInterface)(UInt32 id);
};

class PluginInfo {
public:
	enum {
		kInfoVersion = 2
	};

	UInt32		InfoVersion;
	const char* Name;
	UInt32		Version;
};
#elif defined(SKYRIM)
class PluginInterface {
public:
	enum InterfaceType {
		kInterface_Invalid = 0,
		kInterface_Scaleform,
		kInterface_Papyrus,
		kInterface_Serialization,
		kInterface_Task,
		kInterface_Messaging,
		kInterface_Max,
	};

	UInt32	Version;
	UInt32	GameVersion;
	UInt32	EditorVersion;
	UInt32	IsEditor;

	void*	(*QueryInterface)(UInt32 id);
};

class PluginInfo {
public:
	enum {
		kInfoVersion = 1
	};

	UInt32		InfoVersion;
	const char* Name;
	UInt32		Version;
};

class PapyrusInterface {
public:
	typedef bool (*RegisterFunctions)(VMClassRegistry* registry);

	enum {
		kInterfaceVersion = 1
	};

	UInt32	interfaceVersion;
	bool	(*Register)(RegisterFunctions callback);
};
#endif