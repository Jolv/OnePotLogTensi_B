
/* 
* Jorolv Arnekleiv, OKT2014
This sketch builds on SFE_BMP180 altitude example sketch from Sparkfun.
V10 Mike Grusin, SparkFun Electronics 10/24/2013
Hardware connections:
- (GND) to GND
+ (VDD) to 3.3V  !!!!
Any Arduino pins labeled:  SDA  SCL
Uno, Redboard, Pro:        A4   A5
Mega2560, Due:             20   21
Leonardo:                   2    3
Leave the IO (VDDIO) pin unconnected. This pin is for 1.8V systems
_____________________________________________
Reused 27/5-15 
*/

#include <SFE_BMP180.h>
#include <Wire.h>

byte WaterLevSn = 0;

// Create an SFE_BMP180 object, here called "hPaVal":
SFE_BMP180 hPaVal;
double baseline; // baseline pressure
//double a,P;
double T,P,p0,a;
//double presDiff;
int presDiff;
double millisBefore,millisAfter;
//---------------------
byte lowByteUt, highByteUt;
int intPrs=0, intPrsMask=0x1F;
int intTmpr;
#define XBser Serial1
//#define XBser Serial //<<<
void setup()
{
	Serial.begin(115200); 
	Serial.begin(9600); //<<<
	Serial.println("REBOOT");
/* The following blocks if Leo not connected to PC	
    while (!Serial) { 
	    //	 ; // wait for serial port to connect. Needed for Leonardo only
    }
*/	
	delay(3000);
    Serial.println(" ");
    Serial.println("Version: OnePotLogTensi, Dec15");
    XBser.begin(9600);
    Serial.println("Setup finished");


	// Initialize the sensor (it is important to get calibration values stored on the device).

	if (hPaVal.begin())
	{ 
	Serial.println("BMP180 init success");
	}
	else
	{
		// Oops, something went wrong, this is usually a connection problem,
		// see the comments at the top of this sketch for the proper connections.

		Serial.println("BMP180 init fail (disconnected?)\n\n");
		while(1); // Pause forever.
	}

	// Get the baseline pressure:
	millisBefore=millis();
	baseline = getPressure();
	millisAfter=millis()-millisBefore;
	
	Serial.print(millisAfter);
	Serial.print("  baseline p: ");
	Serial.print(baseline);
	Serial.println(" mb");
}//  setup()

void loop()
{//
// insert the reading of water level sensor (VLS) here
// put it in Byte WaterLevSn
//
	// Get a new pressure reading:
    P = getPressure();
	
// write Pressure to XBser
	intPrs=int(P);
	lowByteUt=lowByte(intPrs);
	highByteUt=highByte(intPrs);
	XBser.write(lowByteUt);
	XBser.write(highByteUt);
	Serial.print(intPrs); Serial.print(" - ");
	Serial.print(lowByteUt); Serial.print(" - ");
	Serial.println(highByteUt);
// write temperature to XBser
	intTmpr=int(T);
	lowByteUt=lowByte(intTmpr);
	highByteUt=highByte(intTmpr);
	XBser.write(lowByteUt);
	XBser.write(highByteUt);
	Serial.print(lowByteUt); Serial.print(" - ");
	Serial.println(highByteUt);
// write the VLS to XBser here
//	XBser.write(WaterLevSn);

// presDiff=P-baseline;
// Serial.print("Pressure diff=");Serial.print(presDiff);
Serial.print("  P= ");
Serial.print(P); Serial.print(" T= "); Serial.println(T);
	delay(9000);
}// loop()


double getPressure()
{
	char status;
	//double T,P,p0,a;

	// You must first get a temperature measurement to perform a pressure reading.
	
	// Start a temperature measurement:
	// If request is successful, the number of ms to wait is returned.
	// If request is unsuccessful, 0 is returned.

	status = hPaVal.startTemperature();
	if (status != 0)
	{
		// Wait for the measurement to complete:

		delay(status);

		// Retrieve the completed temperature measurement:
		// Note that the measurement is stored in the variable T.
		// Use '&T' to provide the address of T to the function.
		// Function returns 1 if successful, 0 if failure.

		status = hPaVal.getTemperature(T);
		if (status != 0)
		{
			// Start a pressure measurement:
			// The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
			// If request is successful, the number of ms to wait is returned.
			// If request is unsuccessful, 0 is returned.
  
  //Serial.print("Temperature=");
  //Serial.print(T);
  //Serial.print("  ");
  
			status = hPaVal.startPressure(3);
			if (status != 0)
			{
				// Wait for the measurement to complete:
				delay(status);

				// Retrieve the completed pressure measurement:
				// Note that the measurement is stored in the variable P.
				// Use '&P' to provide the address of P.
				// Note also that the function requires the previous temperature measurement (T).
				// (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
				// Function returns 1 if successful, 0 if failure.

				status = hPaVal.getPressure(P,T);
				if (status != 0)
				{
					return(P);
				}
				else Serial.println("error retrieving pressure measurement\n");
			}
			else Serial.println("error starting pressure measurement\n");
		}
		else Serial.println("error retrieving temperature measurement\n");
	}
	else Serial.println("error starting temperature measurement\n");
}



