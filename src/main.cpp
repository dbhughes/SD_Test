
#include "FS.h"
#include "SD.h"
#include "time.h"

#include "MySd.h"
#include "MyWeather.h"
#include "MyNtp.h"


MySd mysd;


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
    if (digitalRead(PushButton0) == LOW )
    { 
        mysd.appendFile(SD, "/Logger.txt",  " DeDah\n");
        mysd.readFile(SD, "/Logger.txt");
        delay(500);
    }

}
