#include<iostream>
#include<vector>
#include"PureItem.h"
#include"map.h"
#ifndef PLAYER_H
#define PLAYER_H
using namespace std;
class Player
{
private:
	int healthPoint;
	int sanityPoint;
	int	erosionPoint;
	int room;
	Map	map;
public:
	Player();
	~Player();
	void updateHealthPoint(int newHeathPoint);
	void updateSanityPoint(int newSanityPoint);
	void updateErosionPoint(int newErosionPoint);
	void updateMap(Map newMap);
	void updateBag(vector<PureItem*> newBag);
	void setRoom(int room);
	Map getMap();
	int getHealthPoint();
	int getSanityPoint();
	int getErosionPoint();
	int getRoom();
	vector<PureItem*> bag;

};
#endif