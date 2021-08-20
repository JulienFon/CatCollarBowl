#include <GSM.h>
#include <MKRGSM.h>
#include <ArduinoLowPower.h>
#include "secrets.h"

#define BUZZER 2

GPRS gprs;
GSM gsmAccess(true);
GSM_SMS sms;
GSMLocation location;
//char remoteNumber[20];


String GSMlatitude = "0.000000";
String GSMlongitude = "0.000000";

void measureLocation() {
  unsigned long timeout = millis();
  while (millis() - timeout < 45000) {
    if (location.available() && location.accuracy() < 300 && location.accuracy() != 0) {
      GSMlatitude = String(location.latitude(), 6);
      GSMlongitude = String(location.longitude(), 6);
      break;
    }
  }
}

void connectNetwork()
{
  boolean notConnected = true;
  while (notConnected)
  {
    if (gsmAccess.begin(SECRET_PINNUMBER) == GSM_READY && (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY))
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }
}

void ring() {
  const int songspeed = 1.5; //Change to 2 for a slower version of the song, the bigger the number the slower the song
  //*****************************************
  int NOTE_E4 = 330 ;
  int NOTE_G4 = 392 ;
  int NOTE_A4 = 440 ;
  int NOTE_B4 = 494 ;
  int NOTE_C5 = 523 ;
  int NOTE_D5 = 587 ;

  int notes[] = {       //Note of the song, 0 is a rest/pulse
    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
    NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
    NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
    NOTE_A4, NOTE_G4, NOTE_A4, 0,

  };
  int duration[] = {         //duration of each note (in ms) Quarter Note is set to 250 ms
    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 375, 125,

  };
  for (int i = 0; i < 19; i++) {       //19 is the total number of music notes in the song
    int wait = duration[i] * songspeed;
    tone(BUZZER, notes[i], wait);        //tone(pin,frequency,duration)
    delay(wait);
  }
}
void setup() {
  ring();
  pinMode(BUZZER, OUTPUT);
  Serial.begin(9600);
  Serial.println("SMS Messages Receiver");
  connectNetwork();
  location.begin();
  Serial.println("GSM initialized.");
  Serial.println("Waiting for messages");
}

void loop() {
  char c;
  String command = "";
  if (sms.available())
  {
    //    Serial.println("Message received from:");
    //    sms.remoteNumber(remoteNumber, 20);
    //    Serial.println(remoteNumber);
    while (c = sms.read())
      command.concat(c);
    if (command.equals("ring"))
      ring();
    if (command.equals("loc"))
    {
      measureLocation();
      sms.beginSMS("+"+SECRET_TEL_NB);
      sms.print("https://www.google.com/maps/place/" + GSMlatitude + "," + GSMlongitude);
      sms.endSMS();
    }
    //sms.flush();

  }
  else
    Serial.println("no message");
  //Turn off the GSM module to gain the lowest power consumption from the board while are sleeping
  gsmAccess.shutdown();
  LowPower.sleep(60000); //enable the low power for 60 seconds and after retry the board
  connectNetwork(); //turn on the module and reconect to data network
}