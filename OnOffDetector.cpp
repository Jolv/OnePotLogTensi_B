#include "OnOffDetector.h"


OnOffDetector::OnOffDetector(int pCtLim, bool normalSts)
{
	ctLim = pCtLim;
	// assume that sensor is in normal state when execution starts
	// normal state may be ON or OFF, set debounce parameters accordingly
	if (normalSts)
	{   // Normal state is ON
		ct = ctLim;
		sts = true;
	}
	else
	{   // Normal state is OFF
		ct = 0;
		sts = false;
	}
}// OnOffDetector( CONSTRUCTOR

bool OnOffDetector::turnedOff(bool inpt)
{
	if (inpt) return false;
	if (!sts)
	{
		ct = 0; // ensure that a sequence only true inputs
		// are received before sts is changed in fn turnedOn()
		return false;
	}
	// a debounce situation exsists
	if (ct<=0)
	{
		sts = false;
		return true;
	}
	else
	{
		ct--;
		return false;
	}
}//turnedOff(

bool OnOffDetector::turnedOn(bool inpt)
{
	if (!inpt) return false;
	if (sts)
	{
		ct = ctLim; // ensure that a sequence only false inputs
		// are received before sts is changed in fn turnedOff()
		return false;
	}
	// a debounce situation exsists
	if (ct >= ctLim)
	{
		sts = true;
		return true;
	}
	else
	{
		ct++;
		return false;
	}
}//turnedOn(
