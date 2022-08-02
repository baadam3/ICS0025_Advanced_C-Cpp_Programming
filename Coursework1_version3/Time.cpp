#include "Main.h"

Time::Time()
{
	time(&Now);
	struct tm Tm;
	localtime_s(&Tm, &Now);
	Hours = Tm.tm_hour; // 0...23
	Mins = Tm.tm_min; // 0 ... 59
	Secs = Tm.tm_sec; // 0 ... 60
}

Time::Time(int h, int m, int s)
{
	SetHours(h);
	SetMins(m);
	SetSecs(s);
}

Time::Time(const Time& Original)
{ // copy constructor
	Hours = Original.Hours;
	Mins = Original.Mins;
	Secs = Original.Secs;
	if (Original.pText)
	{
		pText = new char[9];
		strcpy_s(pText, 9, Original.pText);
	}
	else
	{
		pText = nullptr;
	}
}

Time::~Time()
{
	if (pText)
		delete pText;
}

void Time::SetHours(int h)
{
	if (h < 0 || h > 23)
		throw exception("Wrong hours");
	Hours = h;
}

void Time::SetMins(int m)
{
	if (m < 0 || m > 59)
		throw exception("Wrong minutes");
	Mins = m;
}

void Time::SetSecs(int s)
{
	if (s < 0 || s > 60)
		throw exception("Wrong seconds");
	Secs = s;
}

void Time::SetTime(int h, int m, int s)
{
	SetHours(h);
	SetMins(m);
	SetSecs(s);
}

bool Time::operator==(const Time& other) const
{
	if (Hours == other.Hours && Mins == other.Mins && Secs == other.Secs)
		return true;
	else
		return false;
}

bool Time::operator!=(const Time& other) const
{
	if (Hours != other.Hours || Mins != other.Mins || Secs != other.Secs)
		return true;
	else
		return false;
}

bool Time::operator<(const Time& other) const
{
	if (Hours != other.Hours)
		return Hours < other.Hours;
	if (Mins != other.Mins)
		return Mins < other.Mins;
	return Secs < other.Secs;
}


Time& Time::operator=(const Time& Right)
{
	if (this == &Right) 
		return *this; 
	Hours = Right.Hours;
	Mins = Right.Mins;
	Secs = Right.Secs;
	if (Right.pText)
	{
		pText = new char[9];
		strcpy_s(pText, 9, Right.pText);
	}
	else
	{
		pText = nullptr;
	}
	return *this;
}

Time::operator char *() const
{
	if (!pText)
	{
		(const_cast<Time*>(this))->pText = new char[9];
	}
	sprintf_s(pText, 9, "%02d:%02d:%02d", Hours, Mins, Secs);
	return pText;
}

char *Time::ToString()
{
	if (!pText)
	{
		pText = new char[9];
	}
	sprintf_s(pText, 9, "%02d:%02d:%02d", Hours, Mins, Secs);
	return pText;
}

Time Time::CreateRandomTime()
{
	random_device rd; // obtain a random number from hardware
	mt19937 time_generator(rd()); // seed the generator
	uniform_int_distribution<int> hours_distribution(0, 23);
	uniform_int_distribution<int> mins_distribution(0, 59);
	uniform_int_distribution<int> secs_distribution(0, 60);
	return Time(hours_distribution(time_generator), mins_distribution(time_generator), secs_distribution(time_generator));
}
