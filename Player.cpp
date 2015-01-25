#include "Player.h"
#include "Packet.h"

#include "ItemMap.h"
#include <math.h>


void Player::Initialize(DWORD addr)
{
	oSendPacket = (tSendPacket)addr;

}
void Player::LoginData(InitialLoginData ild)
{
	p_id = ild.serverId;
}


void Player::Update(UpdatePacketSelf upt)
{
	DWORD dwWaitResult; 
	
	dwWaitResult = WaitForSingleObject(ghMutex, INFINITE);
	switch (dwWaitResult) 
	{
	case WAIT_OBJECT_0:
		__try {
			p_update = upt;
		}
		__finally {
			if(!ReleaseMutex(ghMutex))
			{
				printf("Failed to release mutex: %d\n", GetLastError());
			}
		}
		break; 
	case WAIT_ABANDONED:
		break;
	}
}
UpdatePacketSelf Player::GetUpdateInfo()
{
	UpdatePacketSelf ret;
	DWORD dwWaitResult; 	
	dwWaitResult = WaitForSingleObject(ghMutex, INFINITE);
	switch (dwWaitResult) 
	{
	case WAIT_OBJECT_0:
		__try {
			ret = p_update;
		}
		__finally {
			if(!ReleaseMutex(ghMutex))
			{
				printf("Failed to release mutex: %d\n", GetLastError());
			}
		}
		break; 
	case WAIT_ABANDONED:
		break;
	}

	return ret;
}

//get ID
DWORD Player::GetID()
{
	return p_id;
}
BOOL Player::CheckRadius(int px, int py, int mx, int my, int r)
{
	
	if( ((px - mx) * (px - mx)) + ((py - my) * (py - my)) <= (r * r))
		return true;
	return false;
}

void Player::PrintLoc(int loc)
{
	UpdatePacketSelf ups = GetUpdateInfo();
	printf("%u X/Y: %u/%u HP/MP/exp %u/%u/%u\n",loc,ups.positionX, ups.positionY, ups.currentHPPct, ups.currentMPPct, ups.currentExpPct);
}

uint16 Player::getXLoc()
{
	UpdatePacketSelf ups = GetUpdateInfo();
	return ups.positionX;
}

uint16 Player::getYLoc()
{
	UpdatePacketSelf ups = GetUpdateInfo();
	return ups.positionY;
}

uint16 Player::getHP()
{
	UpdatePacketSelf ups = GetUpdateInfo();
	return ups.currentHPPct;
}

uint16 Player::getMP()
{
	UpdatePacketSelf ups = GetUpdateInfo();
	return ups.currentMPPct;
}

int Player::DistanceBetweenPoints(int pt1, int pt2)
{
	int difference = pt2-pt1;
	double sum = difference * difference;
	double sqroot = std::sqrt(sum);
	int result = static_cast<int>(sqroot);
	return result;
}

