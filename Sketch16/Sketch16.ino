#pragma region Includes & Imports
/*This is all the included libraries for our program*/
#include <Arduino.h>
#include <binary.h>
#include <Client.h>
#include <emoro_2560.h>
#include <emoro_acc.h>
#include <emoro_bluetooth.h>
#include <emoro_ds18s20.h>
#include <emoro_gyr.h>
#include <emoro_i2c.h>
#include <emoro_intdef.h>
#include <emoro_interrupts.h>
#include <emoro_lcd.h>
#include <emoro_mag.h>
#include <emoro_servo.h>
#include <emoro_switch.h>
#include <HardwareSerial.h>
#include <IPAddress.h>
#include <new.h>
#include <pitches.h>
#include <Platform.h>
#include <Print.h>
#include <Printable.h>
#include <Server.h>
#include <Stream.h>
#include <Udp.h>
#include <USBAPI.h>
#include <USBCore.h>
#include <USBDesc.h>
#include <WCharacter.h>
#include <wiring_private.h>
#include <WString.h>

#pragma endregion
#pragma region Variables
/*This is our variables for control of movement and for recieving the distance of the ultrasonic sensor*/

static int Lfwd = 2000;                                         
static int Rfwd = 1000;                                         
static int Lbck = 1000;                                
static int Rbck = 2000;                                        
static int Lstp = 1500;                                          
static int Rstp = 1500; 
int cm;             

#pragma endregion
#pragma region Functions
//This is a function we use at the bootup of the Emoro
void setup() {
	//Here we instanciate the object of the robot allowing us to use the layout specific ports and functions of the robot
	InitEmoro();                                               
	//Here we logically attach the motors to the ports
	EmoroServo.attach(SERVO_0);                                     
	EmoroServo.attach(SERVO_1);
	//Here we initialize the pins IO_0 & 1 as INPUT ports which is connected to the sensors
	pinMode(IO_0, INPUT);                                    
	pinMode(IO_1, INPUT);
	//Here we logically attach the ultrasonic sensor to the GPP_0 port
	Ultrasonic.attach(GPP_0);
}

//This is a function we use as our running code
//It is called continiously after the setup function has been called
/*Inside our loop we define the variable "cm" as the current input on the ultrasonic sensor then we check if the variable is less than 8 (as in 8 cm)*/
/*if that is the case then we turn the robot for half a second to the left and then we drive forward and a little to the right to navigate back to the road
and finally we skip the rest of the iteration of the while(true) loop*/
//If the previous check is false then it checks if both of the sensors read white; if that is the case then it drives forward
/*If both the sensors doesnt read white then it checks if the one sensor reads black -
if it does then it turns to the left if it doesnt then it checks the other sensor and turns to the right*/
void loop() {

	while (true)
	{
		cm = Ultrasonic.read(GPP_0);
		if (cm < 8)
		{
			for (int i = 0; i < 25; i++)
			{
				EmoroServo.write(SERVO_0, Lfwd);
				EmoroServo.write(SERVO_1, Rbck);
				delay(20);
			}
			for (int i=0;i<5||(digitalRead(IO_1)==1&&digitalRead(IO_0) == 1);i++)
			{
				EmoroServo.write(SERVO_0, 1750);
				EmoroServo.write(SERVO_1, 1000);
				delay(20);
			}
			continue;
		}
		else if (digitalRead(IO_0) == 1 && digitalRead(IO_1) == 1) {       
			EmoroServo.write(SERVO_0, Lfwd);                            
			EmoroServo.write(SERVO_1, Rfwd);   
			delay(20);
		}
		else if (digitalRead(IO_1) == 0) {                            
			EmoroServo.write(SERVO_0, Lfwd);                            
			EmoroServo.write(SERVO_1, Rbck);                            
			delay(20);                                                  
		}
		else if (digitalRead(IO_0) == 0) {                            
			EmoroServo.write(SERVO_0, Lbck);                            
			EmoroServo.write(SERVO_1, Rfwd);                            
			delay(20);                                                  
		}
	}
}
#pragma endregion