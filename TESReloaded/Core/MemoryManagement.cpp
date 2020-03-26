#include "MemoryManagement.h"

#if defined(OBLIVION)
static const UInt32 kCreateTextureHook = 0x007610D3;
#elif defined(NEWVEGAS)
static const UInt32 kCreateTextureHook = 0x00E68DCD;
#endif

HRESULT __stdcall CreateTextureHook(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, LPDIRECT3DTEXTURE9* ppTexture) {

	return D3DXCreateTextureFromFileInMemoryEx(pDevice, pSrcData, SrcDataSize, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, ppTexture);

}

void CreateMemoryManagementHook() {

	WriteRelCall(kCreateTextureHook, (UInt32)CreateTextureHook);

}