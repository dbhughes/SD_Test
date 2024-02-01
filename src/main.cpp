
#include "FS.h"
#include "SD.h"
#include "time.h"

#include "MyFi.h"
#include "MySd.h"
#include "MyNtp.h"
#include "MyWeather.h"


MySd mysd;
MyWeather myw;
MyNtp myntp;



char txtBuf[250];

// Assign names to Pushbutton pins
const int PushButton0 = 36;
const char* logFileName="/WLog.txt";

void setup()
{
    Serial.begin(115200);
    while(!Serial) { delay (10); }
    Serial.print("\n\n");
    // Set the pin mode for the pushbutton
    pinMode(PushButton0, INPUT_PULLUP);

    // Instantiate object from MyFi Class to connect to WiFi
    MyFi Wap;
  // Use object method to connect to WAP
    Wap.ScanWapsAndConnect();

    // Configure RTC settings
    configTime(myntp.gmtOffset_sec, myntp.daylightOffset_sec, myntp.ntpServer);
    
    // Set the ESP32 RTC from specified NTP server
    myntp.SetRtcFromNtp();

    // Get the weather data
    myw.getWeatherData();

    // Print ESP Local IP Address
    Serial.println(WiFi.localIP());


    mysd.mountSd("V");

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
/*
    mysd.listDir(SD, "/", 0);
    mysd.createDir(SD, "/mydir");
    mysd.removeDir(SD, "/mydir");
    mysd.writeFile(SD, "/hello.txt", "Hello ");
    mysd.appendFile(SD, "/hello.txt", "World!\n");
    mysd.readFile(SD, "/hello.txt");
    mysd.deleteFile(SD, "/foo.txt");
    mysd.renameFile(SD, "/hello.txt", "/foo.txt");
    mysd.readFile(SD, "/foo.txt");
*/

    if (mysd.fileExists(SD, logFileName))
    {
        mysd.deleteFile(SD, logFileName);
    }
    

    if (!mysd.fileExists(SD, logFileName))
    {
        mysd.writeFile(SD, logFileName, "");
    }
    
    mysd.appendFile(SD, logFileName,  " World!\n");

    mysd.readFile(SD, logFileName);




    Serial.printf("Free space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void loop()
{
    char weatherBuf[250];
    unsigned long delayMillis;
    unsigned long currentMillis;
    unsigned long MinutesInterval = 5 * 60 * 1000;  // 5 minutes

    // Setup millis timeout for 5 minutes
    delayMillis = millis() + (MinutesInterval);

    // Loop forever
    while(1)
    {
        // Check for pushbutton
        if (digitalRead(PushButton0) == LOW )
        { 
            delay(500);
        }

        // Check the weather every 5 minutes
        if (millis() > delayMillis)
        {
        // Reset delay amount
        delayMillis = millis() + MinutesInterval;
        // Get the weather data
        myw.getWeatherData();
        // make the weather string
        sprintf(weatherBuf, "%18S %15s  %5s Deg  %5s Mph  %5s Deg", myntp.txtTime, myw.cWeather, myw.cHot, myw.cWindSpd, myw.cWindDir);
        // Append the weather to the file
        mysd.appendFile(SD, logFileName, weatherBuf);
        }
  }



}
