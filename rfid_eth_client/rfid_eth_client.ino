#include <SPI.h>
#include <Ethernet.h>
#include <MFRC522.h>

#define MFRC522_RST_PIN 9
#define MFRC522_SS_PIN 8

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte server[] = {10, 0, 0, 102};
EthernetClient client;
MFRC522 mfrc522(MFRC522_SS_PIN, MFRC522_RST_PIN);

String readCardUID()
{
  String uidString = "";
  for(int i=0; i<mfrc522.uid.size; i++)
  {
    uidString += String(mfrc522.uid.uidByte[i], HEX);
  }
  mfrc522.PICC_HaltA();
  return uidString;
}
  
bool sendToServer(String msg)
{
  String response = "";
  if(client.connect(server, 5555))
  {
    client.print(msg);
    Serial.println("MSG SENT");
    client.flush();
    while(client.available() < 2) {}
    char c;
    while(true)
    {
      c = client.read();
      if(c == -1)
      {
        break;
      }
      response += c;
      Serial.print(c);
    }
    Serial.println("");
    client.stop();
  }
  return response == "OK";
}

void acceptCard()
{
  Serial.println("ACCEPT CARD EVENT");
}

void rejectCard()
{
  Serial.println("REJECT CARD EVENT");
}

void setup() {
  Serial.begin(9600);
  while(!Serial){}
  Serial.println("SERIAL OPEN");
  
  Ethernet.begin(mac);
  mfrc522.PCD_Init();
  
  Serial.print("IP: ");
  Serial.println(Ethernet.localIP());
}

void loop()
{
  if(!mfrc522.PICC_IsNewCardPresent())
  {
    return;   
  }

  if(!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  
  String uid = readCardUID();
  Serial.print("CARD READ ");
  Serial.println(uid);
  if(sendToServer(uid))
  {
    acceptCard();
  }
  else
  {
    rejectCard();
  }
}
