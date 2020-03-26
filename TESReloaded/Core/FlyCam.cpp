#include "FlyCam.h"

#if defined(NEWVEGAS)
#define kUpdateFlyCam 0x0094A8C0
#define kFBValue [ebp - 0x80]
#define kRLValue [ebp - 0x84]
static const UInt32 kUpdateForwardFlyCamHook = 0x0094ABDF;
static const UInt32 kUpdateForwardFlyCamReturn = 0x0094ABEB;
static const UInt32 kUpdateBackwardFlyCamHook = 0x0094ABFB;
static const UInt32 kUpdateBackwardFlyCamReturn = 0x0094AC07;
static const UInt32 kUpdateRightFlyCamHook = 0x0094AC17;
static const UInt32 kUpdateRightFlyCamReturn = 0x0094AC29;
static const UInt32 kUpdateLeftFlyCamHook = 0x0094AC39;
static const UInt32 kUpdateLeftFlyCamReturn = 0x0094AC4B;
#elif defined(OBLIVION)
#define kUpdateFlyCam 0x006643A0
#define kFBValue [esp + 0x0C]
#define kRLValue [esp + 0x08]
static const UInt32 kUpdateForwardFlyCamHook = 0x0066446C;
static const UInt32 kUpdateForwardFlyCamReturn = 0x0066447A;
static const UInt32 kUpdateBackwardFlyCamHook = 0x00664489;
static const UInt32 kUpdateBackwardFlyCamReturn = 0x00664497;
static const UInt32 kUpdateRightFlyCamHook = 0x006644A6;
static const UInt32 kUpdateRightFlyCamReturn = 0x006644B4;
static const UInt32 kUpdateLeftFlyCamHook = 0x006644C3;
static const UInt32 kUpdateLeftFlyCamReturn = 0x006644D1;
#endif
static float Scroll;

class FlyCam {

public:
	void TrackUpdateFlyCam();

};

void (__thiscall FlyCam::* UpdateFlyCam)();
void (__thiscall FlyCam::* TrackUpdateFlyCam)();
void FlyCam::TrackUpdateFlyCam() {

	if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.FlyCam.KeyAdd)) TheSettingManager->SettingsMain.FlyCam.ScrollMultiplier += TheSettingManager->SettingsMain.FlyCam.StepValue;
	if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.FlyCam.KeySubtract)) TheSettingManager->SettingsMain.FlyCam.ScrollMultiplier -= TheSettingManager->SettingsMain.FlyCam.StepValue;
	if (TheSettingManager->SettingsMain.FlyCam.ScrollMultiplier <= 0.0f) TheSettingManager->SettingsMain.FlyCam.ScrollMultiplier = 1.0f;
	Scroll = TheSettingManager->SettingsMain.FlyCam.ScrollMultiplier * 10.0f;
	(this->*UpdateFlyCam)();

}

static __declspec(naked) void UpdateForwardFlyCamHook()
{

	__asm
	{
		fld		kFBValue
		fadd	Scroll
		fstp	kFBValue
		jmp		kUpdateForwardFlyCamReturn
	}

}

static __declspec(naked) void UpdateBackwardFlyCamHook()
{

	__asm
	{
		fld		kFBValue
		fsub	Scroll
		fstp	kFBValue
		jmp		kUpdateBackwardFlyCamReturn
	}

}

static __declspec(naked) void UpdateRightFlyCamHook()
{

	__asm
	{
		fld		kRLValue
		fadd	Scroll
		fstp	kRLValue
		jmp		kUpdateRightFlyCamReturn
	}

}

static __declspec(naked) void UpdateLeftFlyCamHook()
{

	__asm
	{
		fld		kRLValue
		fsub	Scroll
		fstp	kRLValue
		jmp		kUpdateLeftFlyCamReturn
	}

}

void CreateFlyCamHook()
{
	*((int *)&UpdateFlyCam) = kUpdateFlyCam;
	TrackUpdateFlyCam		= &FlyCam::TrackUpdateFlyCam;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)UpdateFlyCam,		*((PVOID *)&TrackUpdateFlyCam));
	DetourTransactionCommit();

	WriteRelJump(kUpdateForwardFlyCamHook,	(UInt32)UpdateForwardFlyCamHook);
	WriteRelJump(kUpdateBackwardFlyCamHook, (UInt32)UpdateBackwardFlyCamHook);
	WriteRelJump(kUpdateRightFlyCamHook,	(UInt32)UpdateRightFlyCamHook);
	WriteRelJump(kUpdateLeftFlyCamHook,		(UInt32)UpdateLeftFlyCamHook);

}