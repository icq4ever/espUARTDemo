#include <HardwareSerial.h>

#define RXD2 16
#define TXD2 17

const int potPin = 34;
int value = 0;

// union : float / 4 binary
typedef union{
  float floatNum;
  byte binNum[4];
} floatNumber;

floatNumber fNum;


// 식별자와 함께 유니온의 포인터를 넘겨준다.
// 패킷에 담아 시리얼로 전송
void sendingFloatNumber(char header, floatNumber *number){
  char packetBuffer[sizeof(fNum) + 3];

  packetBuffer[0] = '/';
  packetBuffer[1] = header;
  for(int i=0; i<4; i++){
    packetBuffer[i+2] = number->binNum[i];
  };
  packetBuffer[sizeof(packetBuffer) -1] = 0;

  Serial2.write(packetBuffer, sizeof(packetBuffer));
}

void print_hex_byte (byte b){
  Serial.print (b >> 4, HEX) ;   // assuming sending to serial stream...
  Serial.print (b & 15, HEX) ;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // USB UART0
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);  // UART2
}

void loop() {
  // put your main code here, to run repeatedly:
  fNum.floatNum = analogRead(potPin)/ 100.f;   // 일부러 소수점
  Serial.print(fNum.floatNum);
  Serial.print(" : ");
  for(int i=0; i<4; i++){
    print_hex_byte(fNum.binNum[i]);
  }
  Serial.println();
  sendingFloatNumber('f', &fNum);
  delay(50);
}
