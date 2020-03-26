#pragma once

class EquipmentManager // Never disposed
{
public:
	EquipmentManager();

	enum PositionRotationType
	{
		None,
		Shield,
		Weapon,
		TwoHandWeapon,
		Staff,
		Bow,
		Light,
	};

	void			SetRotationPosition(NiAVObject* Object, PositionRotationType PositionRotation, UInt8 SitSleepState);
	void			SetTorchLight(Actor* Act, NiAVObject* Torch, UInt8 Enable);
	void			SetAnimations(HighProcess* Process, UInt8 LeftWeapon);
	void			SetAnimGroup(HighProcess* Process, UInt8 AnimGroup, UInt8 AnimType, const char* Filename);
	void			ResetAnimGroup(HighProcess* Process, UInt8 AnimGroup, UInt8 AnimType);

	TESIdleForm*	OnBackAnim;
	float			LeftTime;
};

class HighProcessEx : public HighProcess
{
public:
	enum State
	{
		StateNone,
		In,
		Out,
	};
	
	enum ObjectType
	{
		ObjectTypeNone,
		Weapon,
		Shield,
		Light,
	};

	InventoryChanges::EntryData*	EquippedLeftWeaponData;
	NiNode*							LeftWeaponObject;		// This should be storage extending the SkinInfo class, but the SkinInfo is saved in the savegame so i avoid to touch it for compatibility
	NiNode*							LeftWeaponObjectFirst;	// Same as above
	UInt8							OnProcessNew;
	UInt8							OnBackActionState;
	UInt8							OnBackState;
	UInt8							EquippingType;
	UInt8							LeftEquippingState;
	UInt8							LeftAnimState;
	UInt8							OnBeltActionState;
	UInt8							OnBeltState;
};

void CreateEquipmentHook();