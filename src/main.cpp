/*
 * pin 1 - not used          |  Micro SD card     |
 * pin 2 - CS (SS)           |                   /
 * pin 3 - DI (MOSI)         |                  |__
 * pin 4 - VDD (3.3V)        |                    |
 * pin 5 - SCK (SCLK)        | 8 7 6 5 4 3 2 1   /
 * pin 6 - VSS (GND)         | ▄ ▄ ▄ ▄ ▄ ▄ ▄ ▄  /
 * pin 7 - DO (MISO)         | ▀ ▀ █ ▀ █ ▀ ▀ ▀ |
 * pin 8 - not used          |_________________|
 *                             ║ ║ ║ ║ ║ ║ ║ ║
 *                     ╔═══════╝ ║ ║ ║ ║ ║ ║ ╚═════════╗
 *                     ║         ║ ║ ║ ║ ║ ╚══════╗    ║
 *                     ║   ╔═════╝ ║ ║ ║ ╚═════╗  ║    ║
 * Connections for     ║   ║   ╔═══╩═║═║═══╗   ║  ║    ║
 * full-sized          ║   ║   ║   ╔═╝ ║   ║   ║  ║    ║
 * SD card             ║   ║   ║   ║   ║   ║   ║  ║    ║
 * Pin name         |  -  DO  VSS SCK VDD VSS DI CS    -  |
 * SD pin number    |  8   7   6   5   4   3   2   1   9 /
 *                  |                                  █/
 *                  |__▍___▊___█___█___█___█___█___█___/
 *
 * Note:  The SPI pins can be manually configured by using `SPI.begin(sck, miso, mosi, cs).`
 *        Alternatively, you can change the CS pin and use the other default settings by using `SD.begin(cs)`.
 *
 * +--------------+---------+-------+----------+----------+----------+
 * | SPI Pin Name | ESP8266 | ESP32 | ESP32-S2 | ESP32-C3 | ESP32-S3 |
 * +==============+=========+=======+==========+==========+==========+
 * | CS (SS)      | GPIO15  | GPIO5 | GPIO5    | GPIO13   | GPIO13   |
 * +--------------+---------+-------+----------+----------+----------+
 * | DI (MOSI)    | GPIO13  | GPIO23| GPIO24   | GPIO14   | GPIO14   |
 * +--------------+---------+-------+----------+----------+----------+
 * | DO (MISO)    | GPIO12  | GPIO19| GPIO25   | GPIO15   | GPIO15   |
 * +--------------+---------+-------+----------+----------+----------+
 * | SCK (SCLK)   | GPIO14  | GPIO18| GPIO19   | GPIO16   | GPIO16   |
 * +--------------+---------+-------+----------+----------+----------+
 *
 * For more info see file README.md in this library or on URL:
 * https://github.com/espressif/arduino-esp32/tree/master/libraries/SD
 */

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "time.h"

#include "MySd.h"

MySd mysd("V");


char txtBuf[250];

// Assign names to Pushbutton pins
const int PushButton0 = 36;

/*
#define REASSIGN_PINS
int sck = 14;
int miso = 12;
int mosi = 13;
int cs = 15;
*/


void setup()
{
    Serial.begin(115200);
    while(!Serial) { delay (10); }

    // Set the pin mode for the pushbutton
    pinMode(PushButton0, INPUT_PULLUP);

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
/*
    mysd.listDir(SD, "/", 0);
    mysd.createDir(SD, "/mydir");
    mysd.listDir(SD, "/", 0);
    mysd.removeDir(SD, "/mydir");
    mysd.listDir(SD, "/", 2);
    mysd.writeFile(SD, "/hello.txt", "Hello ");
    mysd.appendFile(SD, "/hello.txt", "World!\n");
    mysd.readFile(SD, "/hello.txt");
    mysd.deleteFile(SD, "/foo.txt");
    mysd.renameFile(SD, "/hello.txt", "/foo.txt");
    mysd.readFile(SD, "/foo.txt");
    mysd.testFileIO(SD, "/test.txt");

    mysd.writeFile(SD, "/Derek.txt", "Hello Derek");
    mysd.writeFile(SD, "/BILLY.txt", "Hello BB");
*/

    if (!mysd.fileExists(SD, "/Logger.txt"))
    {
        mysd.writeFile(SD, "/Logger.txt", "Hello Loggy\n");
        mysd.appendFile(SD, "/Logger.txt",  " What\n");
        mysd.appendFile(SD, "/Logger.txt",  " a\n");
        mysd.appendFile(SD, "/Logger.txt",  " Great\n");
        mysd.appendFile(SD, "/Logger.txt",  " World!\n");
    }
    
    mysd.readFile(SD, "/Logger.txt");




    Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
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
