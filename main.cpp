#include "main.h"
#include "Player.h"
#include "ItemMap.h"
#include <stdio.h>
#include <windows.h>
#include <sys\timeb.h>



//typedef int (__cdecl* tUL)();
//tUL g_updateLightHook = (tUL)0x431A00;
//
//int __cdecl UpdateLightHook()
//{
//   int ret = g_updateLightHook();
//   float* test = (float*)0x2EE9670;
//   *test = 1;
//   return ret;
//}

Player *plr;
ItemMap& itemMapSingleton = ItemMap::instance();
//These two functions have been posted across forums everywhere, not sure who i could give credit to on these
bool Compare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for(;*szMask;++szMask,++pData,++bMask)
		if(*szMask=='x' && *pData!=*bMask)   return 0;
	return (*szMask) == NULL;
}
DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask)
{
	for(DWORD i=0; i<dwLen; i++)
		if (Compare((BYTE*)(dwAddress+i),bMask,szMask))  return (DWORD)(dwAddress+i);
	return 0;
}



BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	//FILE *file;
    //fopen_s(&file, "C:\\temp\\xenimusBotDLLMAINOutput.txt", "a+");
    if(fdwReason == DLL_PROCESS_ATTACH)
	{
		//fprintf(file, "DLL attach function called.\n");
		DisableThreadLibraryCalls(hinstDLL);

		createConsole();
		ftime(&startt);
		//ItemMap::Initialize();

		itemMapSingleton.Initialize();
		itemMapSingleton.PrintItems();

		

										/*{0x81, 0x3D, 0x38, 0x4D, 0x4F, 0x02, 0xDA, 0xDC, 
										0x95, 0xE4, 0x0F, 0x85, 0xAC, 0x00, 0x00, 0x00, 
										0xA1, 0x54, 0xB9, 0x0E, 0x01, 0xC1, 0xE0, 0x06, 
										0x0F, 0xB7, 0x88, 0x70, 0xA5, 0x53, 0x00, 0x56};*/
		uint8 SendPacketNeedle[] = {0x81, 0x3D, 0x00, 0x00, 0x00, 0x02, 0x00, 0x96, 
									0x00, 0x00, 0x0F, 0x85, 0xAC, 0x00, 0x00, 0x00,
									0xA1, 0x00, 0x00, 0x00, 0x01, 0xC1, 0xE0, 0x06, 
									0x0F, 0xB7, 0x88, 0x00, 0x00, 0x00, 0x00, 0x56};
									
		//char SendPacketMask[] = "xx???x?x??xxxxxxx???xxxxxxx???xx";
		char SendPacketMask[] = "xx???x????xxxxxxx???xxxxxxx???xx";




		uint8 EncryptNeedle[] = {0x8B, 0x44, 0x24, 0x04, 0x56, 0x8B, 0xF1, 0x89, 
								 0x46, 0x10, 0x80, 0x30, 0xAA, 0x8B, 0x46, 0x10,
								 0x80, 0x38, 0xAA, 0x0F, 0x84, 0xA5, 0x00, 0x00, 
								 0x00, 0x57, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x25};
		char EncryptMask[] = "xxxxxxxxxxxxxxxxxxxxxxxxxx????xx";

		uint8 DecryptNeedle[] = {0x56, 0x8B, 0x74, 0x24, 0x08, 0x89, 0x71, 0x10, 
								 0x0F, 0xB6, 0x16, 0x0F, 0xB6, 0x46, 0x01, 0x03, 
								 0xC2, 0x8B, 0x51, 0x28, 0x25, 0xFF, 0x00, 0x00, 
								 0x00, 0x89, 0x41, 0x04, 0x0F, 0xB6, 0x04, 0x10};
		char DecryptMask[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

		//0xAE 0x00 0x00 0x00 0x31 0x00 0x1B 0x01 0x00 0x00 0x00 0x00
		//uint8 transNeedle[] = {0xAE, 0x00, 0x00, 0x00, 0x31, 0x00, 0x1B, 0x01, 0x00, 0x00, 0x00, 0x00};

		//char transMask[] = "x???x?xx????";

		//dwModuleStartAddr is set as the encrypt address for 1.191
		//sendpacket/encrypt/decrypt should never be before this address ever again
		//Between 1.145 & 1.191, addresses shifted about 0xB320 
		//DWORD  dwModuleStartAddr = (DWORD)GetModuleHandle("Xenimus.exe") + 0x26600;
		//encrypt:0x427DE0 offset is 0x26DE0
		//send packet: 0x42DE50 offset is 0x2CE50
		//DWORD  dwModuleStartAddr = (DWORD)GetModuleHandle("Xenimus.exe") + 0x26DE0;
		DWORD  dwModuleStartAddr = (DWORD)GetModuleHandle("Xenimus.exe") + 0x26DE0;
		DWORD addrSendPacket	= FindPattern(dwModuleStartAddr, 0xA000, SendPacketNeedle, SendPacketMask); //0042EA2E

	
		oSendPacket = (tSendPacket)addrSendPacket;
		printf("sendPacket Address: %x\n", addrSendPacket);
		DWORD addrEncrypt		= FindPattern(dwModuleStartAddr, 0xA000, EncryptNeedle, EncryptMask);
		oEncrypt = (tEncrypt)addrEncrypt;

		DWORD addrDecrypt		= FindPattern(dwModuleStartAddr, 0xA000, DecryptNeedle, DecryptMask);
		oDecrypt = (tDecrypt)addrDecrypt;


		DWORD *vtable;
        DWORD addrD3D9   =  FindPattern((DWORD)GetModuleHandle("d3d9.dll"), 0x128000, (PBYTE)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86", "xx????xx????xx") + 2;
      
        memcpy(&vtable, (void *)addrD3D9, 4);
        oEndScene = (tEndScene)vtable[42];
        oReset = (tReset)vtable[16];


		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		printf("creating encrypt detour %x -> %x\n", addrEncrypt, encryptHook);
		DetourAttach(&(PVOID&)oEncrypt, encryptHook);
		printf("creating decrypt detour %x -> %x\n", addrDecrypt, decryptHook);
		DetourAttach(&(PVOID&)oDecrypt, decryptHook);
		//printf("creating UpdateLight detour %x -> %x\n", g_updateLightHook, UpdateLightHook);
		//DetourAttach(&(PVOID&)g_updateLightHook, UpdateLightHook);

		//end scene and reset
		printf("creating EndScene detour %x -> %x\n", vtable[42], EndSceneHook);
		DetourAttach(&(PVOID&)oEndScene, EndSceneHook);
		printf("creating Reset detour %x -> %x\n", vtable[16], ResetHook);
		DetourAttach(&(PVOID&)oReset, ResetHook);
		//end scene and reset
		DetourTransactionCommit();

		bRunning = true;
		printf("running\n");
		CreateThread(0, 0, InPacketsProcesser, 0, 0, 0);
		printf("packetProcessorCreated\n");

		printf("creating new player\n");
		plr = new Player;
		printf("initializing new player\n");
		plr->Initialize(addrSendPacket);
	}
	if(fdwReason == DLL_PROCESS_DETACH)
	{
		bRunning = false;

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)oEncrypt, encryptHook);
		DetourDetach(&(PVOID&)oDecrypt, decryptHook);
		//DetourDetach(&(PVOID&)g_updateLightHook, UpdateLightHook);
		//end scene and reset
		DetourDetach(&(PVOID&)oEndScene, EndSceneHook);
        DetourDetach(&(PVOID&)oReset, ResetHook);
	    //end scene and reset
		DetourTransactionCommit();

		delete plr;

		itemMapSingleton.Destroy();
    }
    return TRUE;
}

int __fastcall encryptHook(void *pthis, void* unk, uint8* packet, int len)
{
	//FILE *sendPacketFile;
	//fopen_s(&sendPacketFile, "C:\\temp\\sendPacketFile.txt", "a+");
	//
	//iOutPacketCount++;
	//if (logOutgoingPackets)
	//{
	//	fprintf(sendPacketFile, "Working on packet# %i\n", iOutPacketCount);
	//	for(int i = 0; i < len; i++)
	//	{
	//		fprintf(sendPacketFile, "%02x ", packet[i]);
	//	}
	//	fprintf(sendPacketFile,"\n");
	//}
	//if(packet[0] == 0x25 && packet[6] == 0x04) //Quest Log Request
	//{
	//	if(packet[10] == 0x00 && packet[11] == 0x00) //Almost always means its sent by the program
	//		bShowLog = false;
	//}
	//fclose(sendPacketFile);
	return oEncrypt(pthis, packet, len);
}

int __fastcall decryptHook(void *pthis, void* unk, uint8* packet, int len)
{
	int decryptres = oDecrypt(pthis, packet, len);

	if (packet[0] == 0x00)
		return decryptres;

	if(packet[0] == 0x1E)
	{
		if(!bShowLog)
		{
			packet[0] = 0xFF;
			bShowLog = true;
		}

		//plr->ParseQuestLog(*(QuestLog*)&packet[2]);

		return decryptres;
	}

	PDATA data;
	data.packet = new uint8[len];
	memcpy(data.packet, packet, len);
	data.len = len;
	inPackets.push(data);


	return decryptres;
}




HRESULT WINAPI EndSceneHook(LPDIRECT3DDEVICE9 pDevice)
{
   HRESULT res = oEndScene(pDevice);
   
   if (!fontCreated)
   {
		D3DXCreateFont( pDevice, 15, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font );
		fontCreated = true;
   }
	vector<Item> itvec = itemMapSingleton.getItemVector();
   if (itvec.size() > 0 && plr != nullptr)
   {

		for (std::vector<Item>::const_iterator itItem = itvec.begin(); itItem != itvec.end(); itItem++)
		{
			
			vec = DrawItemOnGame(pDevice, plr->getXLoc(), plr->getYLoc(), itItem->items.positionX, itItem->items.positionY);
				
			if (itItem->length == 1)
			{
				DrawMessage(m_font, vec.x, vec.y, 255,255,0,255, itemMapSingleton.GetItemName(itItem->items.itemID1).c_str());
			}
			if (itItem->length == 2) //This shouldnt happen with the current set up
			{
				DrawMessage(m_font, vec.x, vec.y, 255,255,0,255, itemMapSingleton.GetItemName(itItem->items.itemID1).c_str());
				DrawMessage(m_font, vec.x, vec.y, 255,255,0,255, itemMapSingleton.GetItemName(itItem->items.itemID2).c_str());
			}
			if (itItem->length == 3)
			{
				DrawMessage(m_font, vec.x, vec.y, 255,255,0,255, itemMapSingleton.GetItemName(itItem->items.itemID1).c_str());
				DrawMessage(m_font, vec.x, vec.y, 255,255,0,255, itemMapSingleton.GetItemName(itItem->items.itemID2).c_str());
				DrawMessage(m_font, vec.x, vec.y, 255,255,0,255, itemMapSingleton.GetItemName(itItem->items.itemID3).c_str());
			}   
	   }
      
   }
	ftime(&endt);
	diff = (int) (1000.0 * (endt.time - startt.time) + (endt.millitm - startt.millitm));
	FPSCount++;
	if (diff >= 1000)
	{	
		itoa(FPSCount, buffer, 10);
		FPSCount = 0;
		ftime(&startt);
	}
	DrawMessage(m_font, 75, 50, 255,255,0,255, buffer);
	//printf("%s%s", buffer, "\n");
	//always return the original pointer
    return res;
}

HRESULT WINAPI ResetHook(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    HRESULT res = oReset(pDevice, pPresentationParameters);
	HRESULT cooperativeStat = pDevice->TestCooperativeLevel();
	
	switch (cooperativeStat)
	{
	case D3DERR_DEVICELOST:
		Sleep(1000);
		m_font->OnLostDevice();
		break;
	case D3DERR_DEVICENOTRESET:
		m_font->OnResetDevice();
		break;
	case D3DERR_DRIVERINTERNALERROR:
		break;
	case D3D_OK:  //Do youre drawings here
		break;
	}
    return res;
}


bool DrawMessage(LPD3DXFONT font, unsigned int x, unsigned int y, int alpha, unsigned char r, unsigned char g, unsigned char b, LPCSTR Message)
{   // Create a color for the text

   D3DCOLOR fontColor = D3DCOLOR_ARGB(alpha, r, g, b);
   RECT rct;
   rct.left=x-75;
   rct.right=x+75;
   rct.top=y-20;
   rct.bottom=rct.top+50;
   font->DrawTextA(NULL, Message, -1, &rct, 0, fontColor);

   return true;
}

D3DXVECTOR3 DrawItemOnGame(IDirect3DDevice9 *dev, uint16 playerXLoc, uint16 playerYLoc, uint16 itemXLoc, uint16 itemYLoc)
{
	D3DVIEWPORT9 viewport;
	dev->GetViewport(&viewport);
	float x = (itemXLoc * 20) + 10;
	float y = (itemYLoc * 20) + 10;
	float z = 0.0;

	D3DXVECTOR3 pos((x-playerXLoc) * 4.0f, (playerYLoc-y) * 4.0f, z);
	
	
	D3DXVECTOR3 screen;

    D3DXMATRIX world, projection, view;
	dev->GetTransform(D3DTS_WORLD, &world);
	dev->GetTransform(D3DTS_PROJECTION, &projection);
    dev->GetTransform(D3DTS_VIEW, &view);

    D3DXVec3Project(&screen, &pos, &viewport, &projection, &view, &world);
	return screen;
}


DWORD WINAPI InPacketsProcesser(LPVOID lpBuffer)
{
	//FILE *packetfile;
	//fopen_s(&packetfile, "C:\\temp\\IncomingPackets.txt", "a+");
	int count = 0;
	//int updatePacketCounter = 0;
	while(bRunning)
	{
		if(!inPackets.empty()) //If there are packets in the queue
		{
			//count++;
			//fprintf(packetfile, "packet #%d:\n",count);
			
			uint8* packet = inPackets.front().packet;
			int len = inPackets.front().len;
			//fprintf(packetfile, "normal packet contents: %u   ", packet);
			//fprintf(packetfile, "dereferenced packet contents: %u   ", &packet);
			//fprintf(packetfile, "packet[0]: %02x   ", packet[0]);
			//fprintf(packetfile, "packet[2]: %02x   ", packet[2]);
			//fprintf(packetfile, "&packet[2]: %u   ", &packet[2]);

			//if(packet[0] == 0x17)
			//{ 
			//	fprintf(packetfile, "Merchant packet:\n");
			//	for(int i = 0; i < len; i++)
			//	{
			//		fprintf(packetfile, "%02x ", packet[i]);
			//	}

			//	fprintf(packetfile, "\n");
			//}

			/*if(packet[0] == 0x12)
			{ 
				fprintf(packetfile, "Inventory packet:\n");
				for(int i = 0; i < len; i++)
				{
					fprintf(packetfile, "%02x ", packet[i]);
				}
				fprintf(packetfile, "\n");
			}*/
			//Do whatever with packet here
			if(packet[0] == 0x1F)
			{ 
				plr->LoginData(*(InitialLoginData*)&packet[2]);
				DWORD tmp = plr->GetID();
				//fprintf(packetfile, "Plr login data set: %u\n", tmp);
			}
			else if(packet[0] == 0x03)
			{
				//fprintf(packetfile, "Update packet.\n");
				//updatePacketCounter = (updatePacketCounter + 1) % 2;
				handleUpdatePacket(packet, inPackets.front().len, count);
			}
			else if(packet[0] == 0x42) //Quest Packet
			{
					/*fprintf(packetfile, "Quest packet.\n");
					for(int i = 0; i < len; i++)
					{
						fprintf(packetfile, "%02x ", packet[i]);
					}
					fprintf(packetfile, "\n");*/
				//plr->ParseQuestText(packet);
			}
			else if(packet[0] == 0x12)
			{
				//printf("got inventory packet\n");
				/*ItemMap::ClearInventoryItems();
				int offset = 92;
				int maxItems = 50;
				int itemCount = 0;
				InventoryItem II = *(InventoryItem*)&packet[offset];
				while(itemCount < maxItems && II.itemID != 0)
				{
					itemCount++;
					ItemMap::AddInventoryItem(II);
					offset+= 6;
					II = *(InventoryItem*)&packet[offset];
				}*/
			}
			else
			{
				//fprintf(packetfile, "Unknown packet, uint: %u.\n",packet[0]);
			}

			free(packet);
			inPackets.pop();
		}
		Sleep(10);
	}
	//fclose(packetfile);
	return 0;
}

void createConsole()
{
	AllocConsole();
	int hCrt, i;
	FILE *hf;
	hCrt = _open_osfhandle((long) GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	hf = _fdopen( hCrt, "w" );
	*stdout = *hf;
	i = setvbuf( stdout, NULL, _IONBF, 0 );	
}

void handleUpdatePacket(uint8* data, int len, int count)
{
	/*FILE *updatePacketfile;
	fopen_s(&updatePacketfile, "C:\\temp\\updatePacketfile.txt", "a+");
	fprintf(updatePacketfile, "Working on packet# %i\n", count);*/
	//fprintf(updatePacketfile, "data[2]: %u, &data[2]: %u\n", data[2], &data[2]);
	UpdatePacketSelf update = *(UpdatePacketSelf*)&data[2];
	/*fprintf(updatePacketfile, "posX: %u, posY: %u, spellFlags: %u, colorbits: %u, numObjects: %u, numUnits: %u, unklol: %u, flags: %u, rot: %u, ani: %u, spelleff: %u, numSpelleff: %u, currentTime: %u, unklol3: %u, hp: %u, mp: %u, exp: %u\n",
		update.positionX, update.positionY, update.spellflags, update.colorbits, update.numInRangeDynamicObjects, update.numInRangeUnits, update.unklol,
		update.flags, update.rotation, update.animation, update.spellEffect, update.numInRangeSpellEffects, update.currentTime,
		update.unklol3, update.currentHPPct, update.currentMPPct, update.currentExpPct);*/

	//fprintf(updatePacketfile, "Updating Player.....\n");
	plr->Update(update); // sets player updatepacketself if mutex is released
	//fprintf(updatePacketfile, "Player update finished..\n");
	int offset = 24;

	//fprintf(updatePacketfile, "numInRangeDynamicObjects: %u\n", update.numInRangeDynamicObjects);
	
	if(update.numInRangeDynamicObjects > 0)
	{
		
		

		/*FILE *numInRangeObjects;
		fopen_s(&numInRangeObjects, "C:\\temp\\numInRangeObjects.txt", "a+");
		fprintf(numInRangeObjects, "Working on packet# %i\n", count);*/
		itemMapSingleton.RemoveAllItems();
		uint16 aplayerXLoc = (plr->getXLoc() - 10) / 20;
		uint16 aplayerYLoc = (plr->getYLoc() - 10) / 20;
		for(int i = 0; i < update.numInRangeDynamicObjects; i++)
		{
			Item* itemdatastruct = new Item();
			if(data[offset] == 0x01)
			{
				UpdateItemData itemdata = *(UpdateItemData*)&data[offset+2];
				itemdatastruct->items = itemdata;
				itemdatastruct->pickedUp = false;
				itemdatastruct->length = 1;
				itemMapSingleton.AddItem(*itemdatastruct, aplayerXLoc, aplayerYLoc);
				//if (m_font != NULL && myDevice != NULL)
				//{
				//	ftime(&endt);
				//	diff = (int) (1000.0 * (endt.time - startt.time) + (endt.millitm - startt.millitm));
				//	printf("diff %d\n", diff);
				//	if (diff >= 150)
				//	{
				//		ItemMap::drawItems(m_font, myDevice, *itemdatastruct, aplayerXLoc, aplayerYLoc);
				//		ftime(&startt);
				//		
				//	}
				//}
				/*printf("Found a single item.\n");
				UpdateSingleItemData item = *(UpdateSingleItemData*)&data[offset+2];
				printf("Item X/Y/ID: %u/%u/%u\n", item.positionX, item.positionY, item.itemID);*/
				/*for(int i = 0; i < 8; i++)
				{
					fprintf(numInRangeObjects, "%02x ", data[offset+i]);
				}
				fprintf(numInRangeObjects, "\n");*/
				offset += 8;
			}
			else if(data[offset] == 0x02)
			{	
				UpdateItemData itemdata = *(UpdateItemData*)&data[offset+2];
				itemdatastruct->items = itemdata;
				itemdatastruct->pickedUp = false;
				itemdatastruct->length = 3;
				itemMapSingleton.AddItem(*itemdatastruct, aplayerXLoc, aplayerYLoc);
				//printf("Found a pile of 3 items.\n");
				//UpdateMultipleItemData items = *(UpdateMultipleItemData*)&data[offset+2];
				//printf("Items X/Y: ID1/ID2/ID3: %u/%u: %u/%u/%u\n", items.positionX, items.positionY, items.itemID1, items.itemID2, items.itemID3);
				
				/*for(int i = 0; i < 12; i++)
				{
					fprintf(numInRangeObjects, "%02x ", data[offset+i]);
				}
				fprintf(numInRangeObjects, "\n");*/
				offset += 12;
			}
			else if(data[offset] == 0x03) 
			{
				/*for(int i = 0; i < 6; i++)
				{
					fprintf(numInRangeObjects, "%02x ", data[offset+i]);
				}*/
				//fprintf(numInRangeObjects, "\n");
				offset += 6;
			}
			/*if (itemdatastruct->length == 3)
			{
				printf("Multiple Items X/Y: %u/%u | Item1: %u | Item2: %u | Item3: %u\n", itemdatastruct->items.positionX, itemdatastruct->items.positionY, itemdatastruct->items.itemID1, itemdatastruct->items.itemID2, itemdatastruct->items.itemID3);
			}
			else if (itemdatastruct->length == 1)
			{
				printf("Single Item X/Y: %u/%u | Item1: %u\n", itemdatastruct->items.positionX, itemdatastruct->items.positionY, itemdatastruct->items.itemID1);
			}*/
		}
		//ItemMap::PrintItems();
		//fprintf(numInRangeObjects, "\n");
		//fclose(numInRangeObjects);
	}
	if (update.numInRangeDynamicObjects == 0)
	{
		itemMapSingleton.RemoveAllItems();
	}
	//fprintf(updatePacketfile, "finished numInRangeDynamicObjects\n");

	//fprintf(updatePacketfile, "numInRangeSpellEffects: %u\n", update.numInRangeSpellEffects);
	if(update.numInRangeSpellEffects > 0)
	{
		for(int x = 0; x < update.numInRangeSpellEffects; x++)
		{
			if(data[offset] >= 240) offset += 12;
			else offset += 8;
		}
	}
	//fprintf(updatePacketfile, "finished numInRangeSpellEffects\n");

	//fprintf(updatePacketfile, "set all out of range\n");
	//UnitMap::SetAllOutOfRange();
	//fprintf(updatePacketfile, "finished set all out of range\n");

	//fprintf(updatePacketfile, "numInRangeUnits: %u\n", update.numInRangeUnits);
	if(update.numInRangeUnits > 0)
	{
		for(int i = 0; i < update.numInRangeUnits; i++)
		{
			uint16 id = (data[offset+1] << 8) + data[offset];
			//fprintf(updatePacketfile, "non true id: %u\n", id);
			uint16 trueid = id & 0x7FFF;
			//fprintf(updatePacketfile, "true id: %u\n", trueid);
			//UnitMap::SetUnitInRange(trueid);
			offset += 2;

			if(id & 0x8000)
				continue;
			else
			{
				uint8 updateflag = data[offset];
				offset += 1;

				if(updateflag & 0x01)
				{
					//UnitMap::UpdateUnitsMovement(trueid, *(UpdatePacketUnitMovement*)&data[offset]); 
					offset += 5;
				}
				if(updateflag & 0x02)
				{
					//UnitMap::UpdateUnitsAuras(trueid, *(UpdatePacketUnitAuras*)&data[offset]);
					offset += 3;
				}
				if(updateflag & 0x04)
				{
					//UnitMap::UpdateUnitsModel(trueid, *(UpdatePacketUnitModels*)&data[offset]);
					offset += 8;
				}
				if(updateflag & 0x08)
				{
					//UnitMap::UpdateUnitsAnim(trueid, *(UpdatePacketUnitAnim*)&data[offset]);
					offset += 1;
				}
				if(updateflag & 0x10)
				{
					//UnitMap::UpdateUnitsSpellEffect(trueid, *(UpdatePacketUnitSpellEffect*)&data[offset]);
					offset += 1;
				}
				if(updateflag & 0x20)
				{
					//UnitMap::UpdateUnitsChat(trueid, *(UpdatePacketUnitChat*)&data[offset]);
					offset += 1 + data[offset];
				}
			}
		}
	}
	//fprintf(updatePacketfile, "finished numInRangeSpellEffects\n");

	//fprintf(updatePacketfile, "Cleaning units\n");
	//UnitMap::CleanUnits();
	//fprintf(updatePacketfile, "finished Cleaning Units\n\n\n\n");
	//fclose(updatePacketfile);
	//UnitMap::OutputUnitMap();
}



