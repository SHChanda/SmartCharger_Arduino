#include <ESP8266WiFi.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(D7, D5, D3, D2, D1, D0);

const char* ssid = "ssid";    //  Your Wi-Fi Name

const char* password = "password";   // Wi-Fi Password

int LED = 2; // led connected to GPIO2 (D4)

bool value = LOW;

WiFiServer server(80);

void setup()
{
  Serial.begin(115200); //Default Baudrate

  lcd.begin(16, 2);

  lcd.clear();

  pinMode(LED, OUTPUT);

  digitalWrite(LED, !value);

  lcd.setCursor(0, 0);
  lcd.print("Connecting...");
  lcd.blink();

  delay(3000);
  
  Serial.print("Connecting to the Newtork");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  server.begin();  // Starts the Server

  Serial.println("Server started");
  Serial.print("IP Address of network: "); // will IP address on Serial Monitor

  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: http://"); // Will print IP address in URL format

  Serial.print(WiFi.localIP());
  Serial.println("/");

  lcd.noBlink();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IP Address - ");

  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  delay(10000);
}

void loop()
{
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  Serial.println("Waiting for new client");

  while (!client.available())
  {
    delay(1);
  }

  //  if (value)
  //  {
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  if (request.indexOf("/LED=ON") != -1)
  {
    digitalWrite(LED, LOW); // Turn LED ON
    value = HIGH;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Battery is");
    lcd.setCursor(0,1);
    lcd.print("Charging...");
    lcd.blink();
  }

  else if (request.indexOf("/LED=OFF") != -1)
  {
    digitalWrite(LED, HIGH); // Turn LED OFF
    value = LOW;

    lcd.noBlink();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Battery is Fully");
    lcd.setCursor(0,1);
    lcd.print("Charged!");
    
  }

  else if (request.indexOf("/LED=DC") != -1)
  {
    digitalWrite(LED, HIGH); // Turn LED OFF
    value = LOW;

    lcd.noBlink();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Device is");
    lcd.setCursor(0,1);
    lcd.print("Disconnected!");
    
  }

  //*------------------HTML Page Code---------------------*//

  client.println("HTTP/1.1 200 OK"); //
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print(" CONTROL LED: ");

  if (value == HIGH)
  {
    client.print("ON");
  }
  else
  {
    client.print("OFF");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>ON</button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>OFF</button></a><br />");
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
  //  }
}
