#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <HardwareSerial.h>
#include <ArduinoJson.h>

// Create a HardwareSerial object for the A9G GPS module
HardwareSerial a9g_gps(2);

// Server URL to which GPS coordinates will be sent
const char* soham_server = "http://192.168.0.100:8000/coordinates";

// Create an instance of WiFiMulti to manage multiple WiFi connections
WiFiMulti wifimulti;

void setup() 
{
  // Start serial communication for debugging
  Serial.begin(115200);
  
  // Initialize the A9G GPS module communication with specified baud rate and pins
  a9g_gps.begin(115200, SERIAL_8N1, 16, 17); // 115200 baud rate, 8 data bits, no parity, 1 stop bit
  // Add WiFi credentials to WiFiMulti instance
  wifimulti.addAP("CYPHER", "2792143289");

  // Clear serial buffer to ensure clean start
  Serial.flush();
  
  // Connect to WiFi network
  connect();
  
  // Delay for stability
  delay(1000);

  // Send AT commands to the A9G GPS module
  sendToA9("AT+RST=1\r\n", 3000); // Reset the GPS module
  sendToA9("AT+GPS=1\r\n", 2000); // Enable GPS
  
  // Indicate setup completion
  Serial.println("Setup Completed, Ready to send data");
  Serial.flush();
}

void loop() 
{
  // Send command to GPS module to get location
  sendToA9("AT+LOCATION=2\r\n", 5000); // Request GPS location
  
  // Check if the GPS module has data available
  if (a9g_gps.available()) 
  {
    // Read and parse the data from GPS module
    parseData(a9g_gps.readString());
  } 
  else 
  {
    // Indicate waiting for GPS data
    Serial.println("Waiting");
    delay(500); // Wait before checking again
  }
}

// Function to send AT commands to the A9G GPS module
void sendToA9(const char* at_cmd, unsigned long time) 
{
  a9g_gps.println(at_cmd); // Send AT command
  delay(time); // Wait for the command to be processed
}

// Function to parse GPS data and send it to the server
void parseData(const String &buff) 
{
  // Check if GPS fix is available
  if (buff.indexOf("GPS NOT FIX") != -1) 
  {
    // Notify if GPS location is not available
    Serial.println("Location not found, waiting for satellite connectivity");
  } 
  else 
  {
    // Extract latitude and longitude from the data
    String lat_str = buff.substring(17, 26);
    String longi_str = buff.substring(27, 36);
    float lat = lat_str.toFloat();
    float longi = longi_str.toFloat();

    // Print the extracted latitude and longitude
    Serial.printf("Latitude: %f   Longitude: %f\n", lat, longi);
    Serial.flush();
    
    // Send the extracted coordinates to the server
    sendToServer(lat, longi);
  }
}

// Function to send the GPS coordinates to the server
void sendToServer(float &latitude, float &longitude) 
{
  // Check if WiFi is connected
  if (wifimulti.run() == WL_CONNECTED) 
  {
    // Create an HTTPClient object for sending HTTP requests
    HTTPClient http;
    http.begin(soham_server); // Specify the server URL

    // Create a StaticJsonDocument to build the JSON object
    StaticJsonDocument<200> doc;
    doc["common_id"] = "DPD Sir"; // Set a common ID for the data
    doc["type"] = "Point"; // Set the type of GeoJSON object
    doc["coordinates"][0] = latitude; // Longitude
    doc["coordinates"][1] = longitude; // Latitude

    // Serialize the JSON document to a String
    String geojson;
    serializeJson(doc, geojson);

    // Add headers and send the POST request
    http.addHeader("Content-Type", "application/json");
    Serial.print("Sending POST DATA: ");
    Serial.println(geojson);
    int responseCode = http.POST(geojson); // Send the POST request

    // Print the HTTP response code
    Serial.print("HTTP Response from FastAPI: ");
    Serial.println(responseCode);
    if (responseCode > 0) 
    {
      // Print the server response if successful
      String response = http.getString();
      Serial.printf("Response code is: %s\n", response.c_str());
    } 
    else 
    {
      // Print an error message if the POST request fails
      Serial.printf("Error with Code: %s\n", responseCode);
    }
    
    // End the HTTP connection
    http.end();
  } 
  else 
  {
    // If WiFi is not connected, attempt to reconnect
    Serial.println("LOST wifi connection, reconnecting.....");
    Serial.flush();
    connect();
  }
  Serial.println("\n");
}

// Function to connect to the WiFi network
void connect() 
{
  // Try connecting to WiFi until successful
  while (wifimulti.run() != WL_CONNECTED) 
  {
    Serial.println("Connecting to Wifi...");
    delay(50); // Wait before checking again
  }
  // Notify when connected to WiFi
  Serial.println("Connected to Wifi");
}
