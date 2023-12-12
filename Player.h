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
	vector<PureItem> bag;
	Map	map;
public:
	Player();
	~Player();
	void updateHealthPoint(int newHeathPoint);
	void updateSanityPoint(int newSanityPoint);
	void updateErosionPoint(int newErosionPoint);
	void updateMap(Map newMap);
	void updateBag(vector<PureItem> newBag);
	void setRoom(int room);
	Map getMap();
	int getHealthPoint();
	int getSanityPoint();
	int getErosionPoint();
	int getRoom();

};
Player::Player()
{
	healthPoint = 100;
	sanityPoint = 100;
	erosionPoint = 0;
}
Player::~Player()
{
	
}
void Player::updateHealthPoint(int newHeathPoint)
{
	healthPoint = newHeathPoint;
}
void Player::updateSanityPoint(int newSanityPoint)
{
	sanityPoint = newSanityPoint;
}
void Player::updateErosionPoint(int newErosionPoint)
{
	erosionPoint = newErosionPoint;
}
void Player::updateMap(Map newMap)
{
	map = newMap;
}
void Player::updateBag(vector<PureItem> newBag)
{
	bag = newBag;
}
void Player::setRoom(int room)
{
	this->room = room;
}
Map Player::getMap()
{
	return map;
}
int Player::getHealthPoint()
{
	return healthPoint;
}
int Player::getSanityPoint()
{
	return sanityPoint;
}
int Player::getErosionPoint()
{
	return erosionPoint;
}
#endif