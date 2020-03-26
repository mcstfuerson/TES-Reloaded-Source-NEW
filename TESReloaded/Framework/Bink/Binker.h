#pragma once

class Binker {

public:
	static void Render(IDirect3DDevice9* Device, char* CurrentPath, const char* MovieFilename, unsigned int Width, unsigned int Height);
	static void Close(IDirect3DDevice9* Device);

};