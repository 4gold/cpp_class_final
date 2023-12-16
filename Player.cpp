#include "Player.h"


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
int Player::getRoom()
{
	return room;
}