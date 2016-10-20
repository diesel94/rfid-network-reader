#include <SPI.h>
#include <Ethernet.h>
#include <MFRC522.h>

#define MFRC522_RST_PIN 9
#define MFRC522_SS_PIN 8
#define RED_LED_PIN 2
#define GREEN_LED_PIN 3
#define BUZZER_PIN 4

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte server[] = {10, 0, 0, 101};
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
  }
  return response == "OK";
}

void onOK()
{
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(600);
  turnOffSignals();
}

void onNO()
{
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
  delay(200);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  turnOffSignals();
}

void turnOffSignals()
{
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}

void setup() {
  Serial.begin(9600);
  while(!Serial){}
  Serial.println("SERIAL OPEN");

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  turnOffSignals();
  
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
  Serial.println(uid);
  if(sendToServer(uid))
  {
    onOK();
  }
  else
  {
    onNO();
  }
  client.stop();
}
