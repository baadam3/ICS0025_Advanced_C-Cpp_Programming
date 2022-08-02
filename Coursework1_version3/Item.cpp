#pragma once
#include "Main.h"
#include "any"
#include "variant"
#include "iostream"

extern default_random_engine Generator;
extern uniform_int_distribution<int> GroupDistribution;
extern uniform_int_distribution<int> SubgroupDistribution;
const Date Item::Begin = Date(1, 1, 2018), Item::End = Date(31, 12, 2018);

Item::Item() 
{  // creates a pseudo-random item
	Group = (char)GroupDistribution(Generator);
	Subgroup = SubgroupDistribution(Generator);
	Name = ::CreateRandomName();
	Timestamp = Date::CreateRandomDate(Begin, End);
}
// To do
int Item::getSubgroup() const
{
	return Subgroup;
}
void Item::setName(string name)
{
	this->Name = name;
}
void Item::setTimestamp(Date timestamp)
{
	this->Timestamp = timestamp;
}
Item::Item(char group, int subgroup, string name, optional<Date> date)
{
	Group = group;
	Subgroup = subgroup;
	Name.assign(name);
	Timestamp = date;
}
string Item::getName() const
{
	return Name;
}
string Item::ToString() const
{
	if (Timestamp.has_value())
	{// If Date 
		Date ts = Timestamp.value();
		return Name + " " + ts.ToString();
	}
	else
	{
		return Name;
	}

}
char Item::getGroup() const
{
	return Group;
}

Item::~Item() {}