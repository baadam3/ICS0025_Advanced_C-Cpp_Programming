#include "Main.h"

int main()
{
	PrepareNames();
	Test();
	return 0;
}

static const char* NamesFile = "Birds.txt"; // http://www.jimpalt.org/birdwatcher/name.html 941 names of different birds, ASCII code, OD OA after each row
default_random_engine Generator;
static uniform_int_distribution<int> NameDistribution;
static vector<string> Names;
uniform_int_distribution<int> GroupDistribution('A', 'Z');
uniform_int_distribution<int> SubgroupDistribution(0, 99);

void PrepareNames()
{
	ifstream file;
	file.open(NamesFile, fstream::in);
	if (!file.good())
		throw exception("Unable to open the names file");
	char buf[100];
	while (file.good())
	{
		file.getline(buf, 100);
		if (buf[0])
			Names.push_back(string(buf));
	}
	file.close();
	uniform_int_distribution<int> distr(0, Names.size() - 1);
	NameDistribution = distr;
}

string CreateRandomName()
{
	int i = NameDistribution(Generator);
	return Names[i];
}

void Test()
{
	
	Data* pData = new Data(20);
	pData->PrintAll();
	cout << endl;
	
/* The first 20 items (groups, subgroups and names must be as below, the dates, of course, will differ)
A: 2: Great Crested Grebe 18 Nov 2018
C: 26: Moorland Francolin 06 Aug 2018
E: 86: Desert Finch 27 Dec 2018
H: 34: Ruby-throated Hummingbird 01 Oct 2018
I: 9: Sooty Tern 23 Jul 2018
   26: Cuckoo 02 Feb 2018
J: 2: Summer Tanager 13 Nov 2018
   43: Iraq Babbler 07 May 2018
   60: Erckel's Francolin 25 Apr 2018
M: 26: Ural Owl 24 Apr 2018
   44: Grey Francolin 23 Nov 2018
N: 5: Blue Rock Thrush 08 Apr 2018
P: 4: Red-breasted Merganser 04 Sep 2018
   35: Krüper's Nuthatch 17 Oct 2018
Q: 19: Mute Swan 15 May 2018
T: 93: Nubian Nightjar 11 Mar 2018
V: 20: White-backed Woodpecker 30 Jun 2018
X: 6  Ring-necked Duck 08 Sep 2018
   60: Small Button Quail (Andalusian Hemipode) 08 Aug 2018
Y: 45: Sooty Falcon 10 Dec 2018
*/

	cout << (pData->InsertItem('E', 86, "Kaljutuvi") ? "Inserted" : "Not inserted") << endl;
/* In those tests I used bird names from Estonian. You are free to replace them with any strings. But be careful, if
    you, for example, replace "Kassikakk" with "Eagle owl", do it everywhere in this function. */
	cout << (pData->InsertItem('E', 86, "Kassikakk", Date(29, 10, 2021)) ? "Inserted" : "Not inserted") << endl;
	cout << (pData->InsertItem('E', 86, "Koskel") ? "Inserted" : "Not inserted") << endl;
	cout << (pData->InsertItem('E', 87, "Ristpart", Date()) ? "Inserted" : "Not inserted") << endl;
	cout << (pData->InsertItem('B', 10, "Varblane") ? "Inserted" : "Not inserted") << endl;
	cout << (pData->InsertItem('#', 73, "Varblane") ? "Inserted" : "Not inserted") << endl;
	cout << (pData->InsertItem('B', 120, "Varblane") ? "Inserted" : "Not inserted") << endl;
	cout << (pData->InsertItem('B', 73, "") ? "Inserted" : "Not inserted") << endl;
	cout << endl;


	pData->PrintItem('Q', 19, "Mute Swan");
	pData->PrintItem('E', 86, "Kassikakk");
	try
	{
		pData->PrintItem('E', 10, "XXX");
	}
	catch (const invalid_argument& e)
	{
		cout << e.what();
	}
	try
	{
		pData->PrintItem('Z', 10, "XXX");
	}
	catch (const invalid_argument& e)
	{
		cout << e.what();
	}
	cout << endl;

	cout << "Items in group E subgroup 86: " << pData->CountSubgroupItems('E', 86) << endl;
	cout << "Items in group E subgroup 87: " << pData->CountSubgroupItems('E', 87) << endl;
	cout << "Items in group B subgroup 10: " << pData->CountSubgroupItems('B', 10) << endl;
	cout << "Items in group X subgroup 10: " << pData->CountSubgroupItems('X', 10) << endl;
	cout << "Items in group Z subgroup 10: " << pData->CountSubgroupItems('Z', 10) << endl;
	cout << endl;

	pData->PrintSubgroup('E', 86);
	pData->PrintSubgroup('E', 87);
	pData->PrintSubgroup('B', 10);
	try
	{
		pData->PrintSubgroup('X', 10);
	}
	catch (const invalid_argument& e)
	{
		cout << e.what();
	}
	try
	{
		pData->PrintSubgroup('Z', 10);
	}
	catch (const invalid_argument& e)
	{
		cout << e.what();
	}
	cout << endl;

	cout << "Items in group E: " << pData->CountGroupItems('E') << endl;
	cout << "Items in group B: " << pData->CountGroupItems('B') << endl;
	cout << "Items in group X: " << pData->CountGroupItems('X') << endl;
	cout << endl;

	pData->PrintGroup('E');
	pData->PrintGroup('B');
	try
	{
		pData->PrintGroup('Z');
	}
	catch (const invalid_argument& e)
	{
		cout << e.what();
	}
	cout << endl;
	
	cout << (pData->InsertSubgroup('W', 10, { {"Meigas", Date()}, {"Rasvatihane", nullopt} }) ? "Inserted" : "Not inserted") << endl;
	cout << (pData->InsertSubgroup('E', 88, { {"Turteltuvi", Date(17, 9, 2021)} }) ? "Inserted" : "Not inserted") << endl;
	cout << (pData->InsertSubgroup('T', 93, { {"Sinitihane", nullopt} }) ? "Inserted" : "Not inserted") << endl;
	cout << (pData->InsertSubgroup('#', 93, { {"Sinitihane", nullopt} }) ? "Inserted" : "Not inserted") << endl;
	cout << (pData->InsertSubgroup('T', 110, { {"Sinitihane", nullopt} }) ? "Inserted" : "Not inserted") << endl;
	cout << (pData->InsertSubgroup('T', 94, { { "Piiritaja", Date() }, { "", Date() }, { "Punarind", Date() } } ) ? "Inserted" : "Not inserted") << endl;
	pData->PrintGroup('E');
	pData->PrintGroup('W');
	cout << endl;

	tuple<string, optional<Date>> t1("Rohevint", Date(16, 9, 2021)), t2("Sootihane", Date(15, 9, 2021));
	tuple<string, optional<Date>> t3("Luik", nullopt);
	tuple<string, optional<Date>> t4("Nepp", Date()), t5("Lagle", Date()), t6("Metsis", Date());
	cout << (pData->InsertGroup('S', { 10, 20, 30 },
		{
			{ t1, t2 },
			{ t3 },
			{ t4, t5, t6 }
		}) ? "Inserted" : "Not inserted") << endl;
	cout << (pData->InsertGroup('W', { 10, 20, 30 },
		{
			{ t1, t2 },
			{ t3 },
			{ t4, t5, t6 }
		}) ? "Inserted" : "Not inserted") << endl;
	cout << (pData->InsertGroup('#', { 10, 20, 30 },
		{
			{ t1, t2 },
			{ t3 },
			{ t4, t5, t6 }
		}) ? "Inserted" : "Not inserted") << endl;
	cout << (pData->InsertGroup('W', { 10, 120, 30 },
		{
			{ t1, t2 },
			{ t3 },
			{ t4, t5, t6 }
		}) ? "Inserted" : "Not inserted") << endl;
	cout << (pData->InsertGroup('W', { 10, 20, 30 },
		{
			{ t1, t2 },
			{ { "", Date() } },
			{ t4, t5, t6 }
		}) ? "Inserted" : "Not inserted") << endl;
	cout << (pData->InsertGroup('W', { 10, 30 },
		{
			{ t1, t2 },
			{ t3 },
			{ t4, t5, t6 }
		}) ? "Inserted" : "Not inserted") << endl;
	cout << (pData->InsertGroup('W', { 10, 20, 30 },
		{
			{ t1, t2 },
			{ t4, t5, t6 }
		}) ? "Inserted" : "Not inserted") << endl;
	pData->PrintGroup('S');
	cout << endl;

	cout << (pData->RemoveItem('Q', 19, "Mute Swan") ? "Removed" : "Not removed") << endl;
	cout << (pData->RemoveItem('J', 43, "Iraq Babbler") ? "Removed" : "Not removed") << endl;
	cout << (pData->RemoveItem('E', 86, "Kassikakk") ? "Removed" : "Not removed") << endl;
	cout << (pData->RemoveItem('E', 10, "XXX") ? "Removed" : "Not removed") << endl;
	pData->PrintGroup('J');
	pData->PrintGroup('E');
	try
	{
		pData->PrintGroup('Q');
	}
	catch (const invalid_argument& e)
	{
		cout << e.what();
	}
	cout << endl;

	cout << (pData->RemoveSubgroup('Y', 45) ? "Removed" : "Not removed") << endl;
	cout << (pData->RemoveSubgroup('E', 86) ? "Removed" : "Not removed") << endl;
	cout << (pData->RemoveSubgroup('E', 10) ? "Removed" : "Not removed") << endl;
	cout << (pData->RemoveSubgroup('U', 1) ? "Removed" : "Not removed") << endl;
	cout << (pData->RemoveSubgroup('#', 1) ? "Removed" : "Not removed") << endl;
	pData->PrintGroup('E');
	try
	{
		pData->PrintGroup('Y');
	}
	catch (const invalid_argument& e)
	{
		cout << e.what();
	}
	cout << endl;

	cout << (pData->RemoveGroup('N') ? "Removed" : "Not removed") << endl;
	cout << (pData->RemoveGroup('P') ? "Removed" : "Not removed") << endl;
	cout << (pData->RemoveGroup('U') ? "Removed" : "Not removed") << endl;
	pData->PrintAll();
	cout << "The data structure contains " << pData->CountItems() << " items" << endl;
	delete pData;
cout << endl;
}
