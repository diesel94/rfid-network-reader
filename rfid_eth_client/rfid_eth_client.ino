#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte server[] = {10, 0, 0, 101};
EthernetClient client;

void snedToServer(String msg)
{
  if(client.connect(server, 5555))
  {
    client.print(msg);
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
      Serial.print(c);
    }
    Serial.println("");
    client.stop();
    Serial.println("MSG SENT");
  }
}

void setup() {
  Ethernet.begin(mac);
  Serial.begin(9600);
  while(!Serial){}

  Serial.print("IP: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  snedToServer("hello");
  delay(2000);
}
