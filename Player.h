#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <hash_map>
using namespace std;


typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned int	uint32;
typedef signed char		int8;
typedef signed short	int16;
typedef signed int		int32;

typedef int (__cdecl *tSendPacket)(uint8 opcode, uint8* packet, int len);


struct UpdatePacketSelf
{
   uint16 positionX; //0
   uint16 positionY; //2
   uint16 spellflags; //4
   uint16 colorbits; //6
   uint16 numInRangeDynamicObjects; //8
   uint8 numInRangeUnits; //10
   uint8 unklol; //11
   uint8 flags; //12 if & 1, expect self say text at end, if & 2, spellflags contain cooldown data, expect no buffs or skills this update if & 4 you're in pvp mode
   uint8 rotation; //13
   uint8 animation; //14
   uint8 spellEffect; //15
   uint8 numInRangeSpellEffects; //16
   uint8 currentTime; //17
   uint8 unklol3; //18
   uint8 currentHPPct; //19
   uint8 currentMPPct; //20 for hp mp and exp, 0 = 0%, 255 = 100%
   uint8 currentExpPct; //21
};


struct InitialLoginData
{
   int32 positionX; //0
   int32 positionY; //4
   uint32 mapId; //8
   uint16 serverId; //12
   uint16 unk; //14 high bits of serverid, seems to be 32bit, unused
   uint16 unk2; //16 seems to be initial login (show tutorial if 1)
   uint16 unk3; //18
   uint16 serverId2; //20 if this != previous id, do not login, don't know why this is here, maybe an old hack by that hollow guy
   uint16 unk4; //22 high bits of serverid, unused
   float time_current; //24 time as of login
   float time_roc; //28 rate of change (every 50 milliseconds passing, rate value changes)
   uint8 unk7; //32
};

struct _POINT{
	uint16 x;
	uint16 y;
};


class Player{
private:
	uint16 p_id;
	UpdatePacketSelf p_update;
	tSendPacket oSendPacket;
	uint32 QuestFlags;
	HANDLE ghMutex, ghMutexLog;
	int DistanceBetweenPoints(int pt1, int pt2);
	
	
public:
	Player(){ srand(time(0)); QuestFlags = 0; ghMutex = CreateMutex(NULL, FALSE, NULL); ghMutexLog = CreateMutex(NULL, FALSE, NULL); }
	~Player(){ CloseHandle(ghMutex); CloseHandle(ghMutexLog); }

	void Initialize(DWORD addr);
	void LoginData(InitialLoginData ild);
	void Update(UpdatePacketSelf upt);

	UpdatePacketSelf GetUpdateInfo();

	DWORD GetID();

	//Misc Functions
	BOOL CheckRadius(int px, int py, int mx, int my, int r);

	void PrintLoc(int key);
	uint16 getXLoc();
	uint16 getYLoc();
	uint16 getHP();
	uint16 getMP();
};


#endif