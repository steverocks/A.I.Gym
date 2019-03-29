#include <Wire.h>
#include <SPI.h>
#include "WizFi250.h"
#define TRIG 2
#define ECHO 3
#define LED 9

// #define APIKEY 나중에 API키 입력할 공간

char ssid[] = "HeeKyun";                      // 사용하고있는 와이파이 이름
char pass[] = "stndan9294";                   // 사용하고있는 와이파이 비밀번호
int status = WL_IDLE_STATUS;                  // 와이파이 상태
int user;

char server[] = "" ;                          // 서버 아이피 입력란
String rcvbuf;                                

boolean viewData;
boolean getIsConnected = false;

WiFiClient client;

void httpRequest();
void printWifiStatus();

void setup()
{
  Serial.begin(9600);
  Serial.println(F("\r\nSerial Initialize"));
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);  
  
  WiFi.init();

  if (WiFi.status() == WL_NO_SHIELD) {                        //와이파이쉴드 연결불량
    Serial.println("WiFi shield not present");
    while (true);
  }

  while ( status != WL_CONNECTED) {                           //와이파이쉴드 연결
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
 
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  
  printWifiStatus();
}

int count=0;

void loop(){
  digitalWrite(TRIG,LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG,HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG,LOW);

  long distance = pulseIn(ECHO,HIGH)/58.2;

  if(distance>30)
    user = 1;
  else if(distance>50)
    user = 0;
    
  Serial.print(distance);
  Serial.println("cm");
  delay(1000);
 
  while(client.available()){
    digitalWrite(LED,HIGH);
    char c = client.read();
    if(c!=NULL){
      if(rcvbuf.length()>130)
       rcvbuf="";
      rcvbuf += c;
      Serial.write(c);              //서버로부터 버퍼 받아서 시리얼 모니터에 출력( 디버깅 용도 )
    }
  }
  delay(1000);
  count+=1000;
  if(count%3000==0){
    httpRequest();
  }
  rcvbuf="";
}

void httpRequest(){
  Serial.println();

  client.stop();        //이전에 서버와 연결되어있던것 종료

  if(client.connect(server,3000)){
    Serial.println("Connecting....");
    client.print(F("GET /tracking/"));

    client.print(F(" HTTP/1.1\r\n"));
    client.print(F("Host: openapi.gbis.go.kr\r\n"));
    client.print(F("Connection: close\r\n"));    
    client.print(F("\r\n\r\n"));
    Serial.println("server connected"); 
  }
  else 
    Serial.println("Connection failed");
}

void printWifiStatus() {
  
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");                 //연결된 와이파이의 ip주소
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");      //연결된 와이파이 강도
  Serial.print(rssi);
  Serial.println(" dBm");
}

