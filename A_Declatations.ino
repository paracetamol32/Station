#include <SD.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include <EEPROM.h>


#include <EEPROMAnything.h>
#include <TimeLord.h>
#define BUFSIZ 100
#define MAX_STRING_LEN  20

//NTP
#include <EthernetUdp.h>
#include <NTPClient.h>
#include <time.h>
#include <TimeLib.h>
EthernetUDP ntpUDP;
NTPClient timeClient(ntpUDP, "78.46.189.152", 3600);

//Interface Definitions
int     RxPin      = 18;  //The number of signal from the Rx
int     VccPin     = 26;
int     AntPin     = 48;
int     Gnd        = 52;
int     Gnd2       = 42;
int     Gnd3      = 3;
int     LedSD       = 2;
int     ledPin     = 16;    //The number of the onboard LED pin
int     RedPin     = 5;     //The Red LED, turn on for a valid Temp sensor packet
int     GrePin     = 6;     //The Green LED, turn on for a valid Wind sensor packet
int     BluPin     = 7;     //The Blue LED, turn on for a valid Rain sensor packet
int     Ground     = 8;
const int chipSelect = 4;
// Variables for Manchester Receiver Logic:
word    sDelay     = 240;    //Small Delay about 1/4 of bit duration  try like 250 to 500
word    lDelay     = 490;    //Long Delay about 1/2 of bit duration  try like 500 to 1000, 1/4 + 1/2 = 3/4
byte    polarity   = 1;      //0 for lo->hi==1 or 1 for hi->lo==1 for Polarity, sets tempBit at start
byte    tempBit    = 1;      //Reflects the required transition polarity
byte    discards   = 0;      //how many leading "bits" need to be dumped, usually just a zero if anything eg discards=1
byte    discNos    = 0;      //Counter for the Discards
boolean firstZero  = false;  //has it processed the first zero yet?  This a "sync" bit.
boolean noErrors   = true;   //flags if signal does not follow Manchester conventions
//variables for Header detection
byte    headerBits = 15;     //The number of ones expected to make a valid header
byte    headerHits = 0;      //Counts the number of "1"s to determine a header
//Variables for Byte storage
byte    dataByte   = 0;      //Accumulates the bit information
byte    nosBits    = 0;      //Counts to 8 bits within a dataByte
byte    maxBytes   = 9;      //Set the bytes collected after each header. NB if set too high, any end noise will cause an error
byte    nosBytes   = 0;      //Counter stays within 0 -> maxBytes
//Bank array for packet (at least one will be needed)
byte    manchester[12];      //Stores manchester pattern decoded on the fly
//Oregon bit pattern, causes nibble rotation to the right, ABCDabcd becomes DCBAdcba
byte    oregon[]   = {
  16, 32, 64, 128, 1, 2, 4, 8
};
byte    csIndex       = 0;   //counter for nibbles needed for checksum
//Weather Variables
byte    quadrant      = 0;   //used to look up 16 positions around the compass rose
double  avWindspeed   = 0.0;
double  avWindspeed2   = 0.0;
double OldavWindspeed   = 0.0;
double Oldwspeed   = 0.0;
double  wspeed = 0.0;
double  gustWindspeed = 0.0; //now used for general anemometer readings rather than avWindspeed
double   rainTotal     = 0.0;
double   rainRate      = 0.0;
double  temperature   = 0.0;
int     humidity      = 0;
int     channel      = 0;
double  intTemp       = 0;
double  outHum       = 0;
double  outTemp       = 0;
double  intHum       = 0;
double  Temp5       = 0.0;
double  Hum5     = 0.0;
double  Temp6       = 0.0;
double  Hum6     = 0.0;
double  intPres       = 0;
byte    intSolar      = 0;   //eg Solar power
byte    intLightning  = 0;   //eg Lightning Strikes
byte    intUV         = 0;   //eg UV Light Levels

int OutTempNow = -999;
int OutHumNow = -999;
int UVNow = -999;
int GustNow = -999;
int AverageNow = -999;
int DirectionNow = -999;
int BarrometerNow = -999;
String Forecast = "";
int InTempNow = -999;
int InHumNow = -999;
String Comfort = "";
double RainRateNow = -999;
double TotalRainFrom0000 = 0;
double TotalRainHour = 0;
int PowerNow = 0;
double RainDaysMonth = -999;
double RainTotalMonth = -999;
double RainTotal = -999;
int OutTempBat = 100;
int UVBat = 100;
int WindBat = 100;
int RainBat = 100;
int InTempBat = 100;
double MaxTemp24 = -999;
double MinTemp24 = 999;
double MaxGust24 = -999;
int Direction24 = -999;
int MaxPower24 = -999;
int MaxUV24 = -999;
long PreviousTime;
long UVTime = 0;
long WindTime = 0;
long OutTime = 0;
long RainTime = 0;
long InTime = 0;
long CH1Time = 0;
long CH2Time = 0;
long CH3Time = 0;
long PowerTime = 0;
double OldRainTotal = 0;
boolean RainNewFlag = true;
boolean WriteFlag = true;
boolean WinNewFlag = false;
int monat, yeaat;
char charbuffermonat[4], charbufferyeaat[4];
char LogMonth[3], LogDay[3], LogHour[3], LogMinute[3], LogSecond[3], PrintDay[3], PrintMonth[3];
char ExtremeMonth[3], ExtremeDay[3], ExtremeTime[3];
extern volatile unsigned long timer0_overflow_count;

const char windDir[16][4] = {"N  ", "NNE", "NE ", "ENE",  "E  ", "ESE", "SE ", "SSE",  "S  ", "SSW", "SW ", "WSW",  "W  ", "WNW", "NW ", "NNW"};
byte    scan      = 0; // &7!=0 means that all three sensors has been detected, so it reports all three with meaningful figures first up (not the latest addition though)
byte  seconds     = 0; // Counter to trigger the 60 seconds worth of data.
byte  mseconds     = 0; // Counter to trigger the 5 min worth of data.
byte  batStat     = 0; // bit1= temp, bit2=wind bit3=rain, bit4=exp if true then that sensor has not been logged for 20 minutes, its battery probably getting flat
byte  logTemp     = 0; // Counter for number of minutes a sensor reading is missed
byte  logWind     = 0; // Counter for number of minutes a sensor reading is missed
byte  logRain     = 0; // Counter for number of minutes a sensor reading is missed
byte  logUV       = 0; // Counter for number of minutes a sensor reading is missed
byte  logExp      = 0; // Counter for number of minutes a sensor reading is missed
int   aday        = 0; // Counts the number of minutes in a day and clears battery status every 24hrs
//Web
String Myversion = "1.2<br>&#169;Chris Wicks 2011/12<br>Compiled on Arduino 1.0";

//NTP





//Eth
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 32, 41);
IPAddress gateway(192, 168, 32, 2);
// the subnet:
IPAddress subnet(255, 255, 255, 0);
IPAddress dnServer(9, 9, 9, 9);
char serverName[] = "www.host.com"; //host web site for the php files to enable emails
EthernetServer server(80); //select the port you wish to use
EthernetClient client;

//Set up variable list for the max and min yearly data
struct config_t
{
  int MaxTempY;
  long MaxTempYD;
  int MinTempY;
  long MinTempYD;
  int MaxGustY;
  int DirectionY;
  long WindYD;
  float TotalPowerY;
  int TotalRainY;
} YearData;
int Trends [14] [24] [2] = { -999}; //Item, Hour, 24/48
// DateTime now;
char compiledate[] = __DATE__;//Log compile time for reference
char compiletime[] = __TIME__;
//long PreviousTime;

//RTC_DS1307 RTC;//Set up RTC
int cardSize;//Set up SD card
Sd2Card card;
SdVolume volume;
//SdFile root;
SdFile logfile;
SdFile html;
SdFile file;

File root;


