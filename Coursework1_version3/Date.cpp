#include "Main.h"

const char Date::MonthNames[12][4] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

Date::Date()
{
	time(&Now);
	struct tm Tm;
	localtime_s(&Tm, &Now);
	Day = Tm.tm_mday; // 1...31
	strcpy_s(Month, MonthNames[Tm.tm_mon]); // 0...11
	iMonth = Tm.tm_mon + 1;
	Year = Tm.tm_year + 1900; // current year - 1900
}

Date::Date(int d, int m, int y)
{
	SetYear(y);
	SetMonth(m);
	SetDay(d);
}

Date::Date(const Date &Original)
{ // copy constructor
	Day = Original.Day; 
	Year = Original.Year;
	iMonth = Original.iMonth;
	strcpy_s(Month, Original.Month);
	if (Original.pText)
	{
		pText = new char[12];
		strcpy_s(pText, 12, Original.pText);
	}
	else
	{
		pText = nullptr;
	}
}


Date::~Date()
{
	if (pText)
		delete pText;
}

bool Date::IsLeap(int y)
{ // from https://www.programiz.com/c-programming/examples/leap-year
	if (y % 4 == 0)
	{
		if (y % 100 == 0)
		{	// year is divisible by 400, hence the year is a leap year
			if (y % 400 == 0)
				return true;
			else
				return false;
		}
		else
			return true;
	}
	else
		return false;
}

void Date::SetMonth(int m)
{
	if (m < 1 || m > 12)
		throw exception("Wrong month");
	strcpy_s(Month, MonthNames[m - 1]);
	iMonth = m;
}

int Date::GetMonth(char *pBuf, int nBuf) const 
{
	if (!pBuf || nBuf < 4)
		return iMonth;
	strcpy_s(pBuf, nBuf, Month); 
	return iMonth;
}

void Date::SetYear(int y)  
{
	if (y < 0)
		throw exception("Wrong year");
	Year = y;
}

void Date::SetDay(int d) 
{
	if (d < 1 || d > 31)
		throw exception("Wrong day");
	if ((iMonth == 4 || iMonth == 6 || iMonth == 9 || iMonth == 11) && d == 31)
		throw exception("Wrong day");
	if (iMonth == 2)
	{
		if (IsLeap(Year))
		{
			if (d > 29)
				throw exception("Wrong day");
		}
		else
		{
			if (d > 28)
				throw exception("Wrong day");
		}
	}
	Day = d;
}

void Date::SetDate(int d, int m, int y)
{
	SetYear(y);
	SetMonth(m);
	SetDay(d);
}

bool Date::operator==(const Date &other) const
{
	if (Day == other.Day && iMonth == other.iMonth && Year == other.Year)
		return true;
	else
		return false;
}

bool Date::operator!=(const Date& other) const
{
	if (Day != other.Day || iMonth != other.iMonth || Year != other.Year)
		return true;
	else
		return false;
}

bool Date::operator<(const Date &other) const
{
	if (Year != other.Year)
		return Year < other.Year;
	if (iMonth != other.iMonth)
		return iMonth < other.iMonth;
	return Day < other.Day;
}

Date &Date::operator=(const Date &Right)
{
	if (this == &Right) // & - address operator
		return *this; // necessary for expressions like d1 = *pd where pd points to d1
	Day = Right.Day; 
	Year = Right.Year;
	iMonth = Right.iMonth;
	strcpy_s(Month, Right.Month);
	if (Right.pText)
	{
		pText = new char[12];
		strcpy_s(pText, 12, Right.pText);
	}
	else
	{
		pText = nullptr;
	}
	return *this;
}

Date::operator char*() const
{
	if (!pText)
	{
		(const_cast<Date*>(this))->pText = new char[12];
	}
	sprintf_s(pText, 12, "%02d %s %d", Day, Month, Year);
	return pText;
}

char *Date::ToString()
{
	if (!pText)
	{
		pText = new char[12];
	}
	sprintf_s(pText, 12, "%02d %s %d", Day, Month, Year);
	return pText;
}

Date Date::CreateRandomDate(Date begin, Date end)
{   // returns a date between begin and end
	// https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
	tm tm_begin, tm_end;
	memset(&tm_begin, 0, sizeof tm_begin);
	memset(&tm_end, 0, sizeof tm_end);
	tm_begin.tm_mday = begin.GetDay();
	tm_end.tm_mday = end.GetDay();
	tm_begin.tm_mon = begin.GetMonth() - 1;
	tm_end.tm_mon = end.GetMonth() - 1;
	tm_begin.tm_year = begin.GetYear() - 1900;
	tm_end.tm_year = end.GetYear() - 1900;
	time_t lower = mktime(&tm_begin);
	time_t upper = mktime(&tm_end);
	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator
	uniform_int_distribution<> distr((unsigned int)lower, (unsigned int)upper);
	time_t time_t_rand = distr(eng);
	tm tm_rand;
	localtime_s(&tm_rand, &time_t_rand);
	return Date(tm_rand.tm_mday, tm_rand.tm_mon + 1, tm_rand.tm_year + 1900);
}

