#include "mbed.h"
#include "ultrasonic.h"
#include "Servo.h"
#include "X_NUCLEO_NFC01A1.h"
#include "NDefLib/NDefNfcTag.h"
#include "NDefLib/RecordType/RecordURI.h"
#include "NDefLib/RecordType/RecordText.h"

// *****************************************//
// Inductive sensor wiring: Brown -> Postive, Blue -> Ground, Black -> Digital output
// Ultrasonic wiring: Trigger Pulse Input, Echo Pulse Output, 5V Supply,0V Ground
// Servo Motor wiring: Brown -> Ground, Red -> Postive, Orange -> PWM signal is given in through this wire to drive the motor
// *****************************************//

Serial pc(USBTX, USBRX);
// inductive input
DigitalIn mySwitch(D6);
DigitalOut myled(LED1);
// servo object
Servo myservo(D3);

// variables
int flag = 0; 

bool inductiveTest()
{
    if (mySwitch == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int updateM24SR(X_NUCLEO_NFC01A1 *nfcNucleo, char *data)
{
    //retrieve the NdefLib interface
    NDefLib::NDefNfcTag &tag = nfcNucleo->getM24SR().getNDefTag();
    printf("System Init done: !\n\r");
    /*SESSION 1*/
    //open the i2c session with the nfc chip
    if (tag.openSession())
    {
        printf("Session opened\n\r");
        nfcNucleo->getLed1() = 1;

        //create the NDef message and record
        NDefLib::Message msg;
        NDefLib::RecordText rText(data);
        msg.addRecord(&rText);

        //write the tag
        if (tag.write(msg))
        {
            printf("Tag written\n\r");
            nfcNucleo->getLed2() = 1;
        }
        else
        {
            printf("Error writing \n\r");
        } //if-else

        //close the i2c session
        if (tag.closeSession())
        {
            printf("Session closed\n\r");
            nfcNucleo->getLed3() = 1;
        }
        else
        {
            printf("Error closing the session\n\r");
        } //if-else
    }
    else
        printf("Error opening the session\n\r");
}

void dist(int distance)
{
    if (distance > 90) //mm
    {
        pc.printf("Distance > 90 %d mm\r\n", distance);
        flag = 0;
    }
    else //if (distance < 90)
    {
        //        wait(1);
        pc.printf("Distance <= 90 %d mm\n\r", distance);
        if (inductiveTest())
        {
            myled = !myled;
            pc.printf("OBJECT DETECTED - Start Metal sensor\n\r", mySwitch);
            myservo.write(0);
            flag = 1;
        }
    }
}

ultrasonic mu(D8, D9, .1, 1, &dist); //Set the trigger pin to D8 and the echo pin to D9
                                     //have updates every .1 seconds and a timeout after 1
                                     //second, and call dist when the distance changes

int main(void)
{
    pc.baud(9600);
    myservo.write(180);
    
    //use default board pinout
    I2C i2cChannel(X_NUCLEO_NFC01A1::DEFAULT_SDA_PIN, X_NUCLEO_NFC01A1::DEFAULT_SDL_PIN);

    X_NUCLEO_NFC01A1 *nfcNucleo = X_NUCLEO_NFC01A1::Instance(i2cChannel, NULL,
    X_NUCLEO_NFC01A1::DEFAULT_GPO_PIN, X_NUCLEO_NFC01A1::DEFAULT_RF_DISABLE_PIN,
    X_NUCLEO_NFC01A1::DEFAULT_LED1_PIN, X_NUCLEO_NFC01A1::DEFAULT_LED2_PIN,
    X_NUCLEO_NFC01A1::DEFAULT_LED3_PIN);

    //retrieve the NdefLib interface
    NDefLib::NDefNfcTag &tag = nfcNucleo->getM24SR().getNDefTag();

    //start measuring the distance
    mu.startUpdates(); 
    
    //call checkDistance() as much as possible, as this is where the class checks if dist needs to be called.
    for (size_t i = 0; i < 50; i++)
    {
        mu.checkDistance();
        printf("Check Distance: !\n\r");
        if (flag == 1)
        {
            updateM24SR(nfcNucleo, "YES");
            printf("Inside the write tag condition: !\n\r");
            wait(5);
            NVIC_SystemReset();
        }
    }

//  to delete the tag and not give credit
    updateM24SR(nfcNucleo, "NO");
    printf("not metal: !\n\r");

    pc.printf("wait for 4  sec\n\r");
    wait(4);
    NVIC_SystemReset();
}
