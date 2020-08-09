#include <time.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#include "Font_Data.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8

// Arbitrary output pins
MD_Parola P = MD_Parola(HARDWARE_TYPE, 0, 0, 0, MAX_DEVICES);

#define SPEED_TIME  75
#define PAUSE_TIME  0
#define MAX_MESG  20

#ifdef delay
#undef delay
#define delay(x) usleep(1000*x)
#endif

// Global variables
int dow;
int h,m,s;
int  day;
uint8_t month;
char szTime[9];    // mm:ss\0
char szsecond[4];    // ss
char szMesg[MAX_MESG+1] = "";


char *mon2str(uint8_t mon, char *psz, uint8_t len)
// Get a label from PROGMEM into a char array
{
  static const char str[][4] PROGMEM =
  {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };

  *psz = '\0';
  mon--;
  if (mon < 12)
  {
    strncpy(psz, str[mon], len);
    psz[len] = '\0';
  }

  return(psz);
}

char *dow2str(uint8_t code, char *psz, uint8_t len)
{
  static const char str[][10] PROGMEM =
  {
    "Sunday", "Monday", "Tuesday", "Wednesday",
    "Thursday", "Friday", "Saturday"
  };

  *psz = '\0';
  code--;
  if (code < 7)
  {
    strncpy(psz, str[code], len);
    psz[len] = '\0';
  }

  return(psz);
}

void getsec(char *psz)
// Code for reading clock date
{
  sprintf(psz, "%02d", s);
}

void getTime(char *psz, bool f = true)
// Code for reading clock time
{
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
      h = p_tm->tm_hour;
      m = p_tm->tm_min;
      s = p_tm->tm_sec;
  sprintf(psz, "%02d%c%02d", h, (f ? ':' : ' '), m);

  //printf("getTime : %s\n", psz);
}

void getDate(char *psz)
// Code for reading clock date
{
  char  szBuf[10];
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
      dow = p_tm->tm_wday+1;
      day = p_tm->tm_mday;
      month = p_tm->tm_mon + 1;
  sprintf(psz, "%d %s %04d", day, mon2str(month, szBuf, sizeof(szBuf)-1), (p_tm->tm_year + 1900));
  //printf("getDate : %s\n", psz);
}

void setup(void)
{
  P.begin(3);

  P.setZone(0, 4, 7);
  P.setZone(1, 0, 0);
  P.setZone(2, 1, 3);

  P.setIntensity(0,0);
  P.setIntensity(1,0);
  P.setIntensity(2,0);

  P.setFont(1, numeric7Seg);
  P.setFont(2, numeric7Se);
  P.displayZoneText(0, szMesg, PA_CENTER, SPEED_TIME, 0, PA_PRINT, PA_SCROLL_LEFT);
  P.displayZoneText(1, szsecond, PA_RIGHT, SPEED_TIME, 0, PA_PRINT, PA_NO_EFFECT);
  P.displayZoneText(2, szTime, PA_RIGHT, SPEED_TIME, PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);

  getDate(szMesg);
  getTime(szTime);
}

void loop(void)
{
  static uint32_t lastTime = 0; // millis() memory
  static uint8_t  display = 0;  // current display mode
  static bool flasher = false;  // seconds passing flasher

  P.displayAnimate();

  if (P.getZoneStatus(0))
  {
    switch (display)
    {
      case 0: // day of week
        P.setTextEffect(0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
        display++;
        dow2str(dow, szMesg, MAX_MESG);
        break;

      case 1: // Calendar
        P.setTextEffect(0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
        display++;
        getDate(szMesg);
        break;

      default:  // Relative Humidity    
        P.setTextEffect(0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
        display = 0;
        //getHumidit(szMesg);
        break;
    }

    P.displayReset(0);
  }

  // Finally, adjust the time string if we have to
  if (millis() - lastTime >= 1000)
  {
    lastTime = millis();
    getsec(szsecond);
    getTime(szTime, flasher);
    flasher = !flasher;

    P.displayReset(1);
    P.displayReset(2);
  }
}

