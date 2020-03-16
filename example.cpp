#include "include.hpp"
#include "DHT22_Driver.cpp"
#include "i2cControl.cpp"
#include "lcdDriver.cpp"
#define lcdAdd 0x27

int main(int argc, char *argv[])
{
    I2cControl *i2c = new I2cControl(1);
    LcdDriver lcd(lcdAdd, i2c);
    time_t now;
    struct tm ts;
    char tempForTime[9], tempLastRow[20], tempForDate[16], tempHumid[5];
    char *outHumid = &tempHumid[0];
    char *outTime = &tempForTime[0];
    char *outLastRow = &tempLastRow[0];
    char *outDate = &tempForDate[0];
    float lastTemp = 70.0, dt = 0.0, change[6]={0.0,0.0,0.0,0.0,0.0,0.0};
    int index = 0;

    //initial LCD prints for static text
    lcd.setDisplayOnCursorOff();
    lcd.lcdSendCommand(LCD_BEG_LINE_1);
    lcd.lcdString("USS Maya NCC-2021");
    lcd.setCursorPositionRowCol(2,0);
    lcd.lcdString("Stardate");
    lcd.setCursorPositionRowCol(3,0);
    lcd.lcdString("Time");
    long curr = 0, nextTime = 0, nextDate = 0, nextTemp = 0, nextdT = 5000;

    while(true){
        time( &now );
        ts = *localtime(&now);
        
        //Current date/static prints
        if(nextDate < curr){
          strftime(tempForDate, 16, "20%y%m.%d",&ts);
          lcd.setCursorPositionRowCol(1,0);
          lcd.lcdString("USS Maya NCC-2021");
          lcd.setCursorPositionRowCol(2,0);
          lcd.lcdString("Stardate");
          lcd.setCursorPositionRowCol(2,9);
          lcd.lcdString(outDate);
          lcd.setCursorPositionRowCol(3,0);
          lcd.lcdString("Time");

          nextDate = curr+30000;
        }
        
        //current time
        if(nextTime < curr){
          strftime(tempForTime, 9, "%H:%M:%S",&ts);
          lcd.setCursorPositionRowCol(3,5);
          lcd.lcdString(outTime);
          nextTime = curr+100;
        }
        
        //calculate dt
        if(nextdT < curr){
          change[index] = (fahrenheit - lastTemp);
          lastTemp = fahrenheit;
          index++;
          if(index == 4){
            index = 0;
          }
          dt = (change[0]+change[1]+change[2]+change[3]+change[4]+change[5]);
          
          nextdT = curr + 10000;
        }
        
        //read and print temp
        if(nextTemp < curr){
          //read and prep data
          saveData();
          if(dt>0.001){
            sprintf(tempLastRow, "%2.1f'C %2.1f'F +%1.3f" ,celsius,fahrenheit,dt);
            sprintf(tempHumid, "%1.1f%%",humidity);
          }
          if((dt<0.001) & (dt>-0.001)){
            sprintf(tempLastRow, "%2.1f'C %2.1f'F stable" ,celsius,fahrenheit);
            sprintf(tempHumid, "%1.1f%%",humidity);
          }
          if(dt<-0.001){
            sprintf(tempLastRow, "%2.1f'C %2.1f'F %1.3f" ,celsius,fahrenheit,dt);
            sprintf(tempHumid, "%1.1f%%",humidity);
          }
          
          //print data
          lcd.setCursorPositionRowCol(3,15);
          lcd.lcdString(outHumid);
          lcd.setCursorPositionRowCol(4,0);
          lcd.lcdString(outLastRow);
          lcd.setCursorPositionRowCol(1,0);
          lcd.lcdString("USS Maya NCC-2021");
          nextTemp = curr + 2500;
        }
        
        //increment time and set cursor end
        lcd.setCursorPositionRowCol(4,19);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        curr = curr + 50;   

    }
    return(0);
}
