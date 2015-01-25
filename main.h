#ifndef __MAIN_H__
#define __MAIN_H__


#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <queue>
#include <hash_map>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#include <sys\timeb.h>

using namespace std;

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "detours\detours.h"
#pragma comment(lib, "detours\\detours.lib")

typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned int	uint32;
typedef signed char		int8;
typedef signed short	int16;
typedef signed int		int32;

//These are the function prototypes
typedef int (__thiscall* tDecrypt)(void* pthis, uint8* packet, int length);
typedef int (__thiscall* tEncrypt)(void* pthis, uint8* packet, int len);
typedef int (__cdecl *tSendPacket)(uint8 opcode, uint8* packet, int len);
typedef HRESULT (WINAPI* tEndScene)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(WINAPI* tReset)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

tDecrypt oDecrypt = NULL;
tEncrypt oEncrypt = NULL;
tSendPacket oSendPacket = NULL;
tEndScene oEndScene = NULL;
tReset oReset = NULL;

int __fastcall encryptHook(void* pthis, void* unk, uint8* packet, int len);
int __fastcall decryptHook(void* pthis, void* unk, uint8* packet, int len);

HRESULT WINAPI ResetHook(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
HRESULT WINAPI EndSceneHook(LPDIRECT3DDEVICE9 pDevice);
D3DXVECTOR3 DrawItemOnGame(IDirect3DDevice9 *dev, uint16 playerXLoc, uint16 playerYLoc, uint16 itemXLoc, uint16 itemYLoc);
bool DrawMessage(LPD3DXFONT font, unsigned int x, unsigned int y, int alpha, unsigned char r, unsigned char g, unsigned char b, LPCSTR Message);
void createConsole();


struct PDATA{
	uint8* packet;
	int len;
};

queue<PDATA> inPackets;
DWORD WINAPI InPacketsProcesser(LPVOID lpBuffer);
LPD3DXFONT	m_font = NULL;
BOOL bRunning = false;
BOOL bShowLog = true;
BOOL logOutgoingPackets = true;

int  iOutPacketCount = 0;
int diff = 0;
int FPSCount = 0;
char buffer [33];
struct timeb startt, endt;
bool fontCreated = false;
D3DXVECTOR3 vec;




void handleUpdatePacket(uint8* data, int len, int count);


#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

BOOL DLL_EXPORT APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__