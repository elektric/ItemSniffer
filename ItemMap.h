//ItemMap.h
#ifndef __ITEMMAP_H__
#define __ITEMMAP_H__

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <hash_map>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
using namespace std;

typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned int	uint32;

struct UpdateSingleItemData
{
	uint16 positionX;
	uint16 positionY;
	uint16 itemID;
};
struct UpdateMultipleItemData
{
	uint16 positionX;
	uint16 positionY;
	uint16 itemID1;
	uint16 itemID2;
	uint16 itemID3;
};

struct UpdateItemData
{
    uint16 positionX;
	uint16 positionY;
	uint16 itemID1;
	uint16 itemID2;
	uint16 itemID3;
};

struct SingleItem
{
   UpdateSingleItemData item;
   BOOL inrange;
};

struct MultipleItems
{
   UpdateMultipleItemData items;
   BOOL inrange;
};

struct Item
{
	UpdateItemData items;
	bool pickedUp;
	int length;
};

class ItemMap{
private:
	 HANDLE ghMutex; 
	 vector<Item> itemVector;
	 hash_map<int, string> watchMap;
	 int getDistance(int px, int py, int ix, int iy);


public:
	 void Initialize();
	 void Destroy();
	 void RemoveAllItems();
	 void AddItem(Item itemToAdd, uint16 playerXLoc, uint16 playerYLoc);
	 void PrintItems();
	 BOOL IsWatchedItem(uint16 ItemID);
	 vector<Item> getItemVector();
	 void PrintInventoryItems();
	 string GetItemName(uint16 ItemID);
	 static ItemMap& instance()
	 {
		 static ItemMap INSTANCE;
		 return INSTANCE;
	 }

};


#endif