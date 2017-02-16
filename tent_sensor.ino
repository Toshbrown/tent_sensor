#include <Arduino.h>
#include <SPI.h>

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

void setup(void)
{
  
  Serial.begin(115200);
  
  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    Serial.println(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );


  /* Disable command echo from Bluefruit */
  ble.echo(false);

  /* reduce the power level to increase battery life */
  ble.sendCommandCheckOK("AT+BLEPOWERLEVEL=-4");
  
  /* Switch to DATA mode to have a better throughput */
  ble.setMode(BLUEFRUIT_MODE_DATA);

  ble.verbose(false);  // debug info is a little annoying after this point!

  ble.reset();
  
}


int sendDelayMs = 100;
int sensorValue = 0;
String tmpString = "";
char sendbuffer[BUFSIZE+1];

void loop(void)
{

  sensorValue = 1024 - analogRead(A0);
  tmpString = String(sensorValue) + '\0';
  // We need to convert the line to bytes, no more than 20 at this time
  tmpString.getBytes(sendbuffer, BUFSIZE);

  if(sensorValue < 100) {
     String error = String("RANDOM ERROR") + '\0';
     error.getBytes(sendbuffer, BUFSIZE);
  }
  
  if (ble.isConnected())
  {
      // write the data
      ble.write(sendbuffer);
      delay(sendDelayMs);
      
  } else {
    Serial.println(sendbuffer);
    delay(sendDelayMs*10);
  }

}


