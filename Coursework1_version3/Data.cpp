#pragma once
#include "Data.h"
#include "optional"
#include "tuple"
#include "RandomUtils.h"

Data::Data(){}

Data::Data(int numberOfElements) // good
{
	std::default_random_engine generator;
	RandomUtils random(generator);

	for (int i = 0; i < numberOfElements; i++) {
		char group = random.getRandomGroup();
		int subgroup = random.getRandomSubgroup();
		std::string name = random.getRandomName();
		optional<Date> d = random.getRandomTimestamp();

		InsertItem(group, subgroup, name, d);
	}
}

map<int, list<Item*> >* Data::InsertGroup(char c, initializer_list<int> subgroups, initializer_list<initializer_list<tuple<string, optional<Date>>>> items)
{
	try
	{
		if (!isalpha(c))
		{
			return {};
		}
		map<int, list<Item*> >* pmGroupExists = GetGroup(c);
		if (pmGroupExists) // group exists
		{
			return nullptr;
		}

		map<int, list<Item*> >* pmGroup = new map<int, list<Item*> >(); //new empty group

		for (int cc : subgroups) //going through the subgroups
		{
			list<Item*>* newSubgroup = new list<Item*>(); //new empty subgroup

			for (auto itemList : items) // checking item lists in subgroups
			{
				for (auto item : itemList) // checking items in item lists
				{
					Item* pNewItem = new Item(c, cc, std::get<0>(item), optional<Date>(std::get<1>(item).value_or(Date::CreateRandomDate(Date(10, 10, 2018), Date(10, 10, 2022)))));
					if (pNewItem->getSubgroup() == cc)
					{
						newSubgroup->push_back(pNewItem); // adds item to item list
					}
				}
				pmGroup->insert({ cc, *newSubgroup }); // insert item list to map
			}
		}
		DataStructure.insert( { c, pmGroup } ); // insert map to data structure
		return pmGroup;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return nullptr;
	}
}

list<Item*>* Data::InsertSubgroup(char c, int i, initializer_list<tuple<string, optional<Date>>> items)
{
	try {

		if (!isalpha(c))
		{
			return {};
		}

		auto existingSubgroup = GetSubgroup(c, i);
		if (existingSubgroup) {
			return nullptr;
		}
		string name;
		optional<Date> date;
		auto group = DataStructure.find(c);

		for (auto item : items) {
			auto [str, opt] = item;
			if (!opt.has_value()) { // if opt is empty, generate a date
				date = Date::CreateRandomDate(Date(1, 1, 2018), Date(1, 1, 2022));
			}
			else
			{
				date = opt;
			}
			// have to check if date or not, cuz function at the end will crash or only find the date variable
			//Tudom hogy date lesz benne
			name = str;
		}

		if (group != DataStructure.end()) { // group exists
			//group->second->emplace(i, new std::list<tuple<string, optional<Date>>>(items));
			InsertGroup(c, { i }, { items });

			if (group->first == c )
			{
				if (group->second->find(i) != group->second->end())
				{
					return &group->second->find(i)->second;
				}

				list<Item*>* pNewItems = new list<Item*>;
				for (auto item : items) {
					auto val = new Item(c, i, name, date);
					pNewItems->push_back(val);
				}
				auto pSubgroup = group->second;
				if (pSubgroup->find(i) != pSubgroup->end())
				{
					pSubgroup->find(i)->second = *pNewItems; // itt szar
				}
				else
				{

					InsertGroup(c, { i }, { items });
					pSubgroup->insert({ i, *pNewItems });
					group = DataStructure.find(c); // update to existing group

					group->second = pSubgroup; //inserts group to data structure

					return &group->second->find(i)->second; 
				}
				//map<char, map<int, vector<Item> > >
				return &group->second->find(i)->second;
			}
			else
			{

			}
			
		}
		else {
			InsertGroup(c, { i }, { items });
			group = DataStructure.find(c);
			return &group->second->find(i)->second;
		}

		return nullptr;
	}
	catch (const std::exception& e) {
		std::cout << e.what();
		return nullptr;
	}
}

list<Item*>* Data::GetSubgroup(char c, int i)
{
	auto pMainGroup = Data::GetGroup(c);
	if (!pMainGroup)//group not exists
	{
		return nullptr;
	}

	auto pListOfItems = pMainGroup->find(i);
	if (pListOfItems != pMainGroup->end()) //subgroup exists
	{
		return &pListOfItems->second;
	}
	return nullptr;
}

map<int, list<Item*> >* Data::GetGroup(char c)
{
	try
	{
		auto pMainGroup = Data::DataStructure.find(c);
		auto pEndOfStructure = Data::DataStructure.end();
		if (pMainGroup == pEndOfStructure)
		{
			return nullptr;
		}
		return pMainGroup->second;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return nullptr;
	}
}

Item* Data::InsertItem(char c, int i, string s, optional<Date> d)
{
	try {
		if (!isalpha(c) or s == "")
		{
			return {};
		}
		auto it = DataStructure.find(c);

		Item* newItem = new Item(c, i, s, d);

		Item* existing = GetItem(c, i, s);
		if (existing)
		{ // item already exists
			return nullptr;
		}

		if (it != DataStructure.end())
		{ // group exists
			auto group = it->second;
			auto subgroupIt = group->find(i);

			if (subgroupIt != group->end())
			{ // subgroup exists
				subgroupIt->second.push_back(newItem);
			}
			else
			{
				InsertSubgroup(c, i,  { make_tuple(newItem->getName(), d) } );
			}
		}
		else
		{
			InsertGroup(c, { i }, { { make_tuple(newItem->getName(), d) } });
		}

		return newItem;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return nullptr;
	}
}

Data::~Data()
{

	for (auto& outerMapIt : DataStructure) {

		for (auto& innerMapIt : *outerMapIt.second) {
			for (auto& it3 : innerMapIt.second) {
				// deallocate every element of the list
				delete it3;
			}
			// deallocate the list
			std::cout << "Destroying Subgroup " << innerMapIt.first << std::endl;
			innerMapIt.second.clear();
		}
		// deallocate the inner map
		std::cout << "destroying Group \"" << outerMapIt.first << '\"' << std::endl;
		outerMapIt.second->clear();
	}  
	DataStructure.clear();
}

void Data::PrintAll()
{
	auto print = [](Item* item) { std::cout << "  - " << item->ToString() << std::endl; };

	for (auto it = DataStructure.cbegin(); it != DataStructure.cend(); ++it) {
		auto group = it->second;
		std::cout << it->first << ':' << std::endl; // print group key

		for (auto inner_it = group->cbegin(); inner_it != group->cend(); ++inner_it) {
			std::cout << " " << inner_it->first << ':' << std::endl; // print subgroup key

			// print items under subgroup
			std::for_each(inner_it->second.cbegin(), inner_it->second.cend(), print);
		}

		std::cout << std::endl << std::endl;
	}
}

Item* Data::GetItem(char c, int i, string s)
{

	auto subgroup = GetSubgroup(c, i);
	if (!subgroup) { return {}; }

	Item* testItem = new Item(c, i, s, optional<Date>(Date()));
	auto itemIter = std::find_if(subgroup->begin(), subgroup->end(),
		[&testItem](Item* x) { return (x->getName() == testItem->getName() && x->getSubgroup() == testItem->getSubgroup() && x->getGroup() && testItem->getGroup()); });

	if (itemIter != subgroup->end()) { // item exists
		return *itemIter;
	}

	return {};
}

void Data::PrintItem(char c, int i, string s)
{
	try
	{
		auto item = GetItem(c, i, s);
		if (item)
		{
			Item* item = GetItem(c, i, s);
			std::cout << item->ToString() << std::endl;
		}
		else
		{
			std::cout << "\nItem not found!\n" << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

int Data::CountSubgroupItems(char c, int i)
{
	auto subgroup = GetSubgroup(c, i);
	if (!subgroup) {
		return 0;
	}

	return subgroup->size();
}

void Data::PrintSubgroup(char c, int i)
{
	auto subgroup = GetSubgroup(c, i);
	if (!subgroup)
	{
		throw std::invalid_argument("\nSubgroup not found!\n");
	}
	
	for (auto it = subgroup->begin(); it != subgroup->end(); it++)
	{
		std::cout << " - " << (*it)->ToString() << std::endl;
	}

}

int Data::CountGroupItems(char c)
{
	auto group = GetGroup(c);
	if (!group)
	{
		return 0;
	}

	int totalSize = 0;
	for (auto inner_it = group->cbegin(); inner_it != group->cend(); ++inner_it)
	{
		totalSize += inner_it->second.size();
	}

	return totalSize;
}

void Data::PrintGroup(char c)
{
	auto group = GetGroup(c);
	if (!group) {
		throw std::invalid_argument("Group not found!\n");
	}

	auto print = [](Item* item) { std::cout << "  - " << (*item).ToString() << std::endl; };

	std::cout << c << ':' << std::endl; // print group

	for (auto inner_it = group->cbegin(); inner_it != group->cend(); inner_it++) {
		std::cout << " " << inner_it->first << ':' << std::endl; // print subgroup

		// print items under subgroup
		std::for_each(inner_it->second.begin(), inner_it->second.end(), print);
	}

	std::cout << std::endl << std::endl; // print group 
}

bool Data::RemoveItem(char c, int i, string s)
{
	auto group = GetGroup(c);
	auto subgroup = GetSubgroup(c, i);
	auto item = GetItem(c, i, s);

	if (group && subgroup && item) {

		Item* testItem = new Item(c, i, s, optional<Date>(Date()));
		auto iterator = std::remove_if(subgroup->begin(), subgroup->end(),
			[&testItem](Item* x) { return ((*x).getName() == (*testItem).getName() && (*x).getSubgroup() == (*testItem).getSubgroup() && (*x).getGroup() && (*testItem).getGroup()); }
		);

		subgroup->erase(iterator, subgroup->end());

		// if after removing item subgroup is empty,
		// remove subgroup too
		if (subgroup->size() == 0) {
			return RemoveSubgroup(c, i); //do this
		}

		return true;
	}

	return false;
}

bool Data::RemoveSubgroup(char c, int i)
{
	auto group = GetGroup(c);
	auto subgroup = GetSubgroup(c, i);

	if (group && subgroup) {

		subgroup->clear();
		group->erase(i);

		// if after removing subgroup, group is empty,
		// remove group too
		if (group->size() == 0) {
			return RemoveGroup(c);
		}

		return true;
	}
	return false;
}

bool Data::RemoveGroup(char c)
{
	auto group = GetGroup(c);
	if (group) group->clear();
	return DataStructure.erase(c);
}

int Data::CountItems()
{
	int totalSize = 0;
	for (auto it = DataStructure.cbegin(); it != DataStructure.cend(); ++it)
	{
		auto subgroup = it->second;
		for (auto inner_it = subgroup->begin(); inner_it != subgroup->end(); inner_it++)
		{
			totalSize += inner_it->second.size();
		}
	}

	return totalSize;
}
