/**
 * number
 * - the phone number who receive the message/call
 * - change with your number, +63 is used in Philippines
 * - if "+63..." not works make it into an 11 digit number
 */
#include <SoftwareSerial.h>
int txd_pin = 30;
int rxd_pin = 31;
int _timeout;
String _buffer;
String number = "+63xxxxxxxxxx";
SoftwareSerial sim(txd_pin, rxd_pin);

void setup() {
  //delay(7000); //delay for 7 seconds to make sure the modules get the signal
  Serial.begin(9600);
  _buffer.reserve(50);
  Serial.println("System Started...");
  sim.begin(9600);
  delay(1000);
  Serial.println("Type s to send an SMS, r to receive an SMS, and c to make a call");
}
void loop() {
  if (Serial.available() > 0)
    switch (Serial.read())
    {
      case 's':
        send_message();
        break;
      case 'r':
        receive_message();
        break;
      case 'c':
        callNumber();
        break;
    }
  if (sim.available() > 0)
    Serial.write(sim.read());
}
void send_message()
{
  //Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r");
  delay(200);
  String SMS = "Hello, how are you?";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  _buffer = _readSerial();
}
void receive_message()
{
  Serial.println ("SIM800L Read an SMS");
  sim.println("AT+CMGF=1");
  delay (200);
  sim.println("AT+CNMI=1,2,0,0,0"); // AT Command to receive a live SMS
  delay(200);
  Serial.write ("Unread Message done");
}
String _readSerial() {
  _timeout = 0;
  while  (!sim.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}
void callNumber() {
  sim.print (F("ATD"));
  sim.print (number);
  sim.print (F(";\r\n"));
  _buffer = _readSerial();
  Serial.println(_buffer);
}
