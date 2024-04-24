#include <WiFi.h>
#define LED_PIN 5 // Change this to the pin connected to your LED

const char* ssid = "YourESP32-";      // Name of your AP
const char* password = "password123";  // Password for your AP
const int port = 80;                    // Port for HTTP server

WiFiServer server(port);

void ledON() {
  Serial.println("Turning LED ON");
  digitalWrite(LED_PIN, HIGH); // Turn on the LED
}

void ledOFF() {
  Serial.println("Turning LED OFF");
  digitalWrite(LED_PIN, LOW); // Turn off the LED
}

void setup() {
  Serial.begin(115200);
  
  // Initialize Wi-Fi AP
  WiFi.softAP(ssid, password);

  // Start HTTP server
  server.begin();

  Serial.println("HTTP server started");
  Serial.print("Connect to AP: ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client connected");

    // Read the HTTP request
    while (client.connected()) {
      if (client.available()) {
        // Read the first line of the request
        String request = client.readStringUntil('\r');
        Serial.println(request);

        // Send the HTTP response
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println();
        client.println("<html><head><title>ESP32 HTTP Server</title></head><body>");
        client.println("<h1>Hello, this server is working!</h1>");
        client.println("<button onclick=\"sendRequest('on')\">ON</button>");
        client.println("<button onclick=\"sendRequest('off')\">OFF</button>");
        client.println("</body></html>");

        // Handle the request
        if (request.indexOf("/?action=on") != -1) {
          ledON();
          Serial.println("LED turned ON");
        } else if (request.indexOf("/?action=off") != -1) {
          ledOFF();
          Serial.println("LED turned OFF");
        }
        break;
      }
    }
    delay(1000);
    client.stop();
    Serial.println("Client disconnected");
  }
}

