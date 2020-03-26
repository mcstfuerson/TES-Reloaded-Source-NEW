#include "D3D9Hook.h"
#include "D3D9Device.h"

#if defined(NEWVEGAS)
static const UInt32 kCreateDeviceHook = 0x00E731FF;
static const UInt32 kCreateDeviceReturn = 0x00E73204;
#elif defined(OBLIVION)
static const UInt32 kCreateDeviceHook = 0x0076A542;
static const UInt32 kCreateDeviceReturn = 0x0076A547;
#elif defined(SKYRIM)
static const UInt32 kCreateDeviceHook = 0x00CD8B6F;
static const UInt32 kCreateDeviceReturn = 0x00CD8B74;
#endif

HRESULT STDMETHODCALLTYPE CreateDevice(IDirect3D9Ex* D3DInterface, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9Ex** ppReturnedDeviceInterface) {
	
	HRESULT R = D3DInterface->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, (IDirect3DDevice9**)ppReturnedDeviceInterface);
	*ppReturnedDeviceInterface = new TESRDirect3DDevice9(D3DInterface, *ppReturnedDeviceInterface);
	return R;
	
}

static __declspec(naked) void CreateDeviceHook()
{

	__asm
	{
		mov		eax, [ecx + 0x40]
		call	CreateDevice
		jmp		kCreateDeviceReturn
	}

}

void CreateD3D9Hook() {

	WriteRelJump(kCreateDeviceHook,		(UInt32)CreateDeviceHook);

}
