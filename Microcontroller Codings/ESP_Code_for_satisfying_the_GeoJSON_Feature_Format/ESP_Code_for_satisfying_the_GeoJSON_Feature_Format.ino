#include <WiFiMulti.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

// Create an instance of WiFiMulti to manage multiple WiFi connections
WiFiMulti wifi;

// Server URL where the data will be sent
const char* server = "http://192.168.0.100:8000/data";

// Define a structure to hold user details
struct user_details {
  float lat;       // Latitude
  float lon;       // Longitude
  String comm_id;  // Common ID
  String desp;     // Description
};

// Initialize the user details with some sample data
user_details ud = {23.256999, 87.853396, "testUser", "Testing Feature Format for User 1"};

void setup()
{
  // Start serial communication for debugging
  Serial.begin(115200);

  // Add WiFi credentials to the WiFiMulti instance
  wifi.addAP("CYPHER", "2792143289");

  // Wait until the ESP32 is connected to WiFi
  while (wifi.run() != WL_CONNECTED) {
    delay(1000); // Delay 1 second before checking again
  }
  // Print a message when connected to WiFi
  Serial.println("Connected to Wifi");
}

void loop()
{
  // Create a StaticJsonDocument to build the GeoJSON object
  StaticJsonDocument<512> doc;

  // Set up the GeoJSON structure
  doc["type"] = "Feature";
  doc["geometry"]["type"] = "Point";
  doc["geometry"]["coordinates"][0] = ud.lon; // Longitude
  doc["geometry"]["coordinates"][1] = ud.lat; // Latitude
  doc["properties"]["common_id"] = ud.comm_id; // Common ID
  doc["properties"]["description"] = ud.desp;   // Description

  // Serialize the JSON document to a String
  String GeoJSON;
  serializeJson(doc, GeoJSON);

  // Check if the WiFi is still connected
  if (wifi.run() == WL_CONNECTED)
  {
    // Create an HTTPClient object for sending HTTP requests
    HTTPClient http;
    http.begin(server); // Specify the server URL
    http.addHeader("Content-Type", "application/json"); // Set the content type to JSON

    // Send a POST request with the GeoJSON data
    int httpResponseCode = http.POST(GeoJSON);

    // Check the HTTP response code
    if (httpResponseCode > 0) {
      // Print the HTTP response code and response message
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      // Print an error message if the POST request fails
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    // End the HTTP connection
    http.end();
  }
  
  // Wait for 5 seconds before the next loop iteration
  delay(5000);
}
