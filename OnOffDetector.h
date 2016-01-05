#pragma once
class OnOffDetector
{
public:
	bool sts;
	int ct; // conter for debouncing
	int ctLim;

	OnOffDetector(int pCtLim, bool normalSts);
	bool turnedOff(bool inpt);
	bool turnedOn(bool inpt);
};

