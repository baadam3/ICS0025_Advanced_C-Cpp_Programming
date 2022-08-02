#pragma once
class Time
{
private:
	int Hours,
		Mins,
		Secs;
	time_t Now = 0; // in Visual Studio 64-bit integer, the number of seconds since January 1, 1970, 0:00 UTC
	char* pText = nullptr;
public:
	Time();
	Time(int h, int m, int s);
	Time(const Time&);
	virtual ~Time();
	void SetHours(int h);
	int GetHours() const { return Hours; }
	void SetMins(int);
	int GetMins() const { return Mins; }
	void SetSecs(int);
	int GetSecs() const { return Secs; }
	void SetTime(int h, int m, int s);
	char* ToString();
	bool operator==(const Time&) const;
	bool operator!=(const Time&) const;
	bool operator<(const Time&) const;
	operator char* () const;
	Time& operator=(const Time&);
	static Time CreateRandomTime();
};

