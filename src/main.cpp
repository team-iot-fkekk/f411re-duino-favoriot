/*
 WiFiEsp example: ConnectWPA
 
 This example connects to an encrypted WiFi network using an ESP8266 module.
 Then it prints the  MAC address of the WiFi shield, the IP address obtained
 and other network details.

 For more details see: http://yaab-arduino.blogspot.com/p/wifiesp-example-connect.html
*/
#include "Arduino.h"
#include "WiFiEsp.h"
#include "SoftwareSerial.h"

SoftwareSerial SerialESP(2, 8); // RX, TX

char ssid[] = "abidev";      // your network SSID (name)
char pass[] = "abcd1234";    // your network password
int status = WL_IDLE_STATUS; // the Wifi radio's status
int counter = 0;
//IPAddress ip(192, 168, 1, 242);
WiFiEspServer server(80);
WiFiEspClient client;

const String myDevice = "f411re@team-iot-fkekk";
String apikey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6InRlYW0taW90LWZrZWtrIiwicmVhZF93cml0ZSI6dHJ1ZSwiaWF0IjoxNTk2MDExOTA3fQ.A8_CEiEaIS8B7-uZiMc2QB-O1OhC-gPmD1-tzkccjRk"; // **replace with your api key from the FAVORIOT platform account setting
char serverAdd[] = "apiv2.favoriot.com";

void dataStream(int i);

void setup()
{
    // initialize serial for debugging
    Serial.begin(115200);
    // initialize serial for ESP module
    SerialESP.begin(115200);
    // initialize ESP module
    WiFi.init(&SerialESP);

    // check for the presence of the shield
    if (WiFi.status() == WL_NO_SHIELD)
    {
        Serial.println("WiFi shield not present");
        // don't continue
        while (true)
            ;
    }

    // attempt to connect to WiFi network
    while (status != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network
        status = WiFi.begin(ssid, pass);
    }

    Serial.println("You're connected to the network");
}

void loop()
{
    counter = counter + 1;
    dataStream(counter);
    delay(1000);
}

void dataStream(int i)
{
    // Json Data to send to Platform
    String json = "{\"device_developer_id\":\""+myDevice+"\",\"data\":{\"Counter\":\"" + String(i) + "\"}}";

    // String json = "{\"device_developer_id\":\"+myDevice+\",
    // \"data\":{   \"Sensor_1\":\""+sensor1+"\",
    //              \"Sensor_2\":\""+sensor2+"\",
    //              \"Sensor_3\":\""+sensor3+"\" }
    // }";

    // display counter value
    Serial.println("\n        COUNTER : " + String(i) + "");

    if (client.connect(serverAdd, 80))
    {
        // Make a HTTP request:
        Serial.println("        STATUS : Sending data..."); //Display sending status
        client.println("POST /v2/streams HTTP/1.1");
        client.println("Host: apiv2.favoriot.com");
        client.print(String("apikey: "));
        client.println(apikey);
        client.println("Content-Type: application/json");
        client.println("cache-control: no-cache");
        client.print("Content-Length: ");
        int thisLength = json.length();
        client.println(thisLength);
        client.println("Connection: close");

        client.println();
        client.println(json);
        Serial.println("        STATUS : Data sent!"); //display sent status
    }
}