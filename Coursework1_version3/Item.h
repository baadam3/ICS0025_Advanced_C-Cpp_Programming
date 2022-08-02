#pragma once
#include "optional"

class Item
{
private:
	char Group;     // Any from range 'A'...'Z'
	int Subgroup;   // Any from range 0...99
	string Name;    // Any, but not empty 
	optional<Date> Timestamp; // Any 
	
public:
	static const Date Begin;
	static const Date End;
	Item();
	Item(char, int, string, Date);
	Item(const Item &);
	Item(char group, int subgroup, string name, optional<Date> date);
	~Item();
// To do
	int getSubgroup() const;
	void setName(string name);
	void setTimestamp(Date timestamp);
	std::string getName() const;
	std::string ToString() const;
	char getGroup() const;

};

