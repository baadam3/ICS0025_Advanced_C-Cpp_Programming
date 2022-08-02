#pragma once

#include "map"
#include "list"
#include "optional"
#include "Main.h"

class Data
{
public:

	Data(int n); //DONE
	//Constructs the objectand fills the container with n random items.

	Data();
	//Constructs the object with empty container;

	~Data();
	//Destructs the objectand releases all the memory occupied by the containerand the items in it.

	void PrintAll(); //DONE
	//Prints all the items stored in the container in command prompt window in easily readable format
	//(see Appendix).Items from the same groupand subgroup must be ordered by their names.

	int CountItems(); //DONE
	//Returns the total number of items in the container.

	map<int, list<Item*> >* GetGroup(char c); //DONE!!!!
	//Returns the pointer to map containing all the items from group c. If the group does not exist, returns nullptr.

	void PrintGroup(char c); //DONE
	/*Prints all the items from group c in command prompt window in easily readable format(see Appendix). Items from the same subgroup must be ordered by their names.If the group was not
	found, throws invalid_argument_exception.*/

	int CountGroupItems(char c); //DONE
	//Returns the current number of items in group c. If the group does not exist, returns 0.

	list<Item*>* GetSubgroup(char c, int i); //DONE!!!!
	//Returns the pointer to list containing all the items from subgroup I from group c. If the subgroup
	//does not exist, returns nullptr.

	void PrintSubgroup(char c, int i); //DONE
	/*Prints all the items from subgroup i from group c in command prompt window in easily readable
	format(see Appendix).Items must be ordered by their names.If the subgroup was not found,
	throws invalid_argument_exception.*/

	int CountSubgroupItems(char c, int i); //DONE
	//Returns the current number of items in subgroup i from group c.If the subgroup does not exist,
	//returns 0.

	Item* GetItem(char c, int i, string s); //DONE
	//Returns the pointer to the first of items specified by group c, subgroup iand name s.If the item was
	//not found returns nullptr.

	void PrintItem(char c, int i, string s); //DONE
	//Prints the first of items specified by group c, subgroup iand name s.If the item was not found
	//throws invalid_argument_exception.

	Item* InsertItem(char c, int i, string s, optional<Date> d = nullopt); //DONE
	/*Creates and inserts the specified item.Returns the pointer to new item.If the input parameters are
	not correct, returns nullptr.If necessary, creates the missing group and subgroup.If Timestamp is
	not specified, use Date::CreateRandomDate(Date, Date) to create. Parameters cannot be empty string.*/

	list<Item*>* InsertSubgroup(char c, int i, initializer_list<tuple<string, optional<Date>>> items); //DONE
	/*Creates and inserts the specified subgroup.The initializer_list items contains tuples specifying the
	Names(cannot be empty strings) and Timestamps(if not specified use Data::CreateRandomDate(Date, Date) to create) of new items. Returns the pointer to new
	subgroup. If the specified subgroup already exists or the input parameters are not correct, returns nullptr. If necessary, creates the missing group.*/

	map<int, list<Item*> >* InsertGroup(char c, initializer_list<int> subgroups, initializer_list<initializer_list<tuple<string, optional<Date>>>> items); //DONE
	/*Creates and inserts the specified group.The subgroups initializer_list presents the keys to be
	included into the new group.The items initializer_list contains initializer_lists presenting tuples
	specifying the Names and Timetamps of new items.The first initializer_list from items corresponds
	to the first integer in subgroups. Returns the pointer to new group. If the specified group already
	exists or the input parameters are not correct, returns nullptr.*/

	bool RemoveItem(char c, int i, string s); //DONE
	/*Removes the specified item.If after removing the subgroup has no members, remove it too.If after
	that the group is empty, remove it also.All the not used memory must be released.Return value :
	false if the item was not found, otherwise true.*/

	bool RemoveSubgroup(char c, int i); //DONE
	/*Removes the specified subgroup.If after removing the corresponding group has no members,
	remove it too.All the not used memory must be released.Return value : false if the subgroup was
	not found, otherwise true.*/

	bool RemoveGroup(char c); //DONE
	//Removes the specified group.All the not used memory must be released.Return value : false if the
	//group was not found, otherwise true.


private:

	map<char, map<int, list<Item*> >*> DataStructure;
};
