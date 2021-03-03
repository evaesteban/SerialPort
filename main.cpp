#include <iostream>
#include "SerialPort.hpp"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <windows.h>
#define FILETIME_TO_UNIX 116444736000000000i64

using namespace std;

char* portName = "\\\\.\\COM7";

#define MAX_DATA_LENGTH 255

char incomingData[MAX_DATA_LENGTH];

//Control signals for turning on and turning off the led
//Check arduino code
char ledON[] = "ON\n";
char ledOFF[] = "OFF\n";

//Arduino SerialPort object
SerialPort *arduino;

//Blinking Delay
const unsigned int BLINKING_DELAY = 1000;

//If you want to send data then define "SEND" else comment it out
//#define SEND

/**********************************
*   Function Prototypes 
**********************************/

static double get_timestamp (void);
static void signalHandler(int signum);
static void receiveData(void);
static void writeData(void);
static void autoConnect(void);


static void signalHandler(int signum){
    arduino->closeSerial();
    exit(signum);
}

static void receiveData(void)
{
    int readResult = arduino->readSerialPort(incomingData, MAX_DATA_LENGTH);

    if(incomingData[0] == 'W'){
        double time = get_timestamp();
        printf("%f\n", time);
        exit(0);
    }
}

static void writeData(unsigned int delayTime)
{
    arduino->writeSerialPort(ledON, MAX_DATA_LENGTH);
    Sleep(delayTime);
    arduino->writeSerialPort(ledOFF, MAX_DATA_LENGTH);
    Sleep(delayTime);
}

static void autoConnect(void)
{
    //better than recusion
    //avoid stack overflows
    while(1) {
        // ui - searching
        std::cout << "Searching in progress";
        // wait connection
        while (!arduino->isConnected()) {
            Sleep(100);
            std::cout << ".";
            arduino = new SerialPort(portName);
        }

        //Checking if arduino is connected or not
        if (arduino->isConnected()) {
            std::cout  << std::endl << "Connection established at port " << portName << std::endl;
        }

        #ifdef SEND
            while(arduino->isConnected()) writeData(BLINKING_DELAY);
        #else // SEND
            while(arduino->isConnected()) receiveData();
        #endif // SEND
    }
}

static double get_timestamp(void)
{
    FILETIME ft;
    GetSystemTimePreciseAsFileTime (&ft);
    int64_t t = ((int64_t)ft.dwHighDateTime << 32L) | (int64_t)ft.dwLowDateTime;
    return (t - FILETIME_TO_UNIX) / (10.0 * 1000.0 * 1000.0);
}

int main()
{
    signal(SIGINT, signalHandler);

    arduino = new SerialPort(portName);

    autoConnect();

}
