//ItemMap.cpp
#include "ItemMap.h"
#include "Player.h"
#include <fstream>
#include <iostream>
#include <string>

HANDLE ghMutex;
vector<Item> itemVector;
hash_map<int, string> watchMap;


void ItemMap::Initialize()
{
	//printf("inside Initialize();\n");
	//ifstream file("C:\\Users\\Artemis\\Documents\\GitHub\\MacroFun\\ItemSniffer\\Debug\\watchList.txt");
	ifstream file("watchList.txt");
	
	string temp_str;
	string ItemID;
	string ItemName;
	int iItemID;
    while (!file.eof())
    {
		std::getline(file, temp_str);
		if (temp_str.find("/") == temp_str.npos)
		{
			SIZE_T split = temp_str.find("=");
			ItemID = temp_str.substr(0,split);
			ItemName = temp_str.substr(split+1);
			iItemID = atoi(ItemID.c_str());
			watchMap[iItemID] = ItemName;
		}
    }
	ghMutex = CreateMutex(NULL, FALSE, NULL);
}

void ItemMap::Destroy()
{
	CloseHandle(ghMutex);
}
void ItemMap::RemoveAllItems()
{
		DWORD dwWaitResult; 
	
	dwWaitResult = WaitForSingleObject(ghMutex, INFINITE);
	switch (dwWaitResult) 
	{
	case WAIT_OBJECT_0:
		__try {
			itemVector.clear();
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


void ItemMap::AddItem(Item itemToAdd, uint16 playerXLoc, uint16 playerYLoc)
{
	int distance = getDistance(playerXLoc, playerYLoc, itemToAdd.items.positionX, itemToAdd.items.positionY);
	//printf("In add item, distance from player to item: %i\n", distance);
	if (distance <= 100)
	{
		DWORD dwWaitResult; 
	
		dwWaitResult = WaitForSingleObject(ghMutex, INFINITE);
		switch (dwWaitResult) 
		{
		case WAIT_OBJECT_0:
			__try {
				if (itemToAdd.length == 1)
				{
					if (IsWatchedItem(itemToAdd.items.itemID1))
					{
						//printf("watched item added: %u\n", itemToAdd.items.itemID1 );
						itemVector.push_back(itemToAdd);
					}
				}
				else if(itemToAdd.length == 3)
				{
					if (IsWatchedItem(itemToAdd.items.itemID1) || IsWatchedItem(itemToAdd.items.itemID2) || IsWatchedItem(itemToAdd.items.itemID3))
					{
						//printf("watched item added: %u, %u, %u\n", itemToAdd.items.itemID1, itemToAdd.items.itemID2, itemToAdd.items.itemID3 );
						itemVector.push_back(itemToAdd);
					}
				}
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
	
}
void ItemMap::PrintItems()
{
	//printf("insidePrintItems();\n");
	//DWORD dwWaitResult; 
	//
	//	dwWaitResult = WaitForSingleObject(ghMutex, INFINITE);
	//	switch (dwWaitResult) 
	//	{
	//	case WAIT_OBJECT_0:
	//		__try {
	//			for(int vecLoc = 0; vecLoc < itemVector.size(); vecLoc++)
	//			{
	//				if (itemVector[vecLoc].length == 1)
	//				{
	//					printf("Single Item X/Y: %u/%u | Item1: %u\n", 
	//						itemVector[vecLoc].items.positionX, itemVector[vecLoc].items.positionY, itemVector[vecLoc].items.itemID1);
	//				}
	//				else if (itemVector[vecLoc].length == 3)
	//				{
	//					printf("Multiple Items X/Y: %u/%u | Item1: %u | Item2: %u | Item3: %u\n", 
	//						itemVector[vecLoc].items.positionX, itemVector[vecLoc].items.positionY,
	//						itemVector[vecLoc].items.itemID1, itemVector[vecLoc].items.itemID2,itemVector[vecLoc].items.itemID3);
	//				}
	//	
	//			}

	//		}
	//		__finally {
	//			if(!ReleaseMutex(ghMutex))
	//			{
	//				printf("Failed to release mutex: %d\n", GetLastError());
	//			}
	//		}
	//		break; 
	//	case WAIT_ABANDONED:
	//		break;
	//	}
	
}


BOOL ItemMap::IsWatchedItem(uint16 ItemID)
{
	return watchMap.count(ItemID);
}

string ItemMap::GetItemName(uint16 ItemID)
{
		string itemName = "";
		int found = watchMap.count(ItemID);
		if (found)
		{
			itemName = watchMap.at(ItemID);
		}
		else
		{
			itemName = "Unknown Item";
		}
		return itemName;
}

int ItemMap::getDistance(int px, int py, int ix, int iy)
{
	int deltax = px - ix;
	int deltay = py - iy;
	return (deltax * deltax) + (deltay * deltay);
}

vector<Item> ItemMap::getItemVector()
{
	DWORD dwWaitResult; 
	
	dwWaitResult = WaitForSingleObject(ghMutex, INFINITE);
	switch (dwWaitResult) 
	{
	case WAIT_OBJECT_0:
		__try {
			return itemVector;
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

