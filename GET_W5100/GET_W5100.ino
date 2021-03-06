#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
 
IPAddress ip(192,168,137,200); //IP address for your arduino.

char server[] = "192.168.137.1"; //IP address of your computer.

int interrupt=0; //Variable to control the iterations of void loop().

String mod="testing"; //Variable to be written on the database.

String rcv=""; //Variable in which the server response is recorded.

EthernetClient client;

void setup()
{
  
  Serial.begin(9600);
  
  Ethernet.begin(mac, ip);
  
  delay(5000); //Wait for ethernet to connect.

}

void httpRequest()
{
  if (client.connect(server, 81)) 
  {
    Serial.println("Connection established 1");
    client.print(String("GET ") + "/tryjson.php/" + " HTTP/1.1\r\n" + "Host: " + server + "\r\n" + "Connection: close\r\n\r\n"); //GET request for server response.
    unsigned long timeout = millis();
    while (client.available() == 0) 
    {
      if (millis() - timeout > 25000) //If nothing is available on server for 25 seconds, close the connection.
      { 
        return;
      }
    }
    while(client.available())
    {
      String line = client.readStringUntil('\r'); //Read the server response line by line..
      rcv+=line; //And store it in rcv.
    }
    client.stop(); // Close the connection.
  }
  else
  {
    Serial.println("Connection failed 1");
  }
  Serial.println("Received string: ");
  Serial.println(rcv); //Display the server response.
}

void loop() 
{
  if(interrupt==0)
  {
      httpRequest(); //Call the function to read the response from the server.
      delay(1000);
      if (client.connect(server, 81)) 
      {
      Serial.println("Connection Established 2");
      client.print("GET /info.php?"); //GET request to write data to the database.
      client.print("request=");
      client.print(mod);
      client.println(" HTTP/1.1"); 
      client.println("Host: 192.168.137.1"); 
      client.println("Connection: close"); 
      client.println(); 
      client.println(); 
      client.stop();
      }
      else
      {
        Serial.println("Connection failed 2");
      }
  }
  interrupt++;
  delay(10000);
}
 
