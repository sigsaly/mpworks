#include <ESP8266WiFi.h>
 
const char* ssid = "";
const char* password = "";

WiFiServer server(80);

int LED_pin = 2;
int motor1_pin = 0;
int motor2_pin = 14;
int led_turn_on = 0;
int led_turn_off = 1;
int motor_turn_on = 1;
int motor_turn_off = 0;

void setup() {
  Serial.begin(115200);
  delay(10);  

  pinMode(LED_pin, OUTPUT);
  pinMode(motor1_pin, OUTPUT);
  pinMode(motor2_pin, OUTPUT);
  digitalWrite(LED_pin, led_turn_off);    
  digitalWrite(motor1_pin, motor_turn_off);
  digitalWrite(motor2_pin, motor_turn_off);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  
  // Match the request
  //int value = led_turn_off;
  if (request.indexOf("/MT1=OFF") != -1)  {
    digitalWrite(motor1_pin, motor_turn_off);
    digitalWrite(LED_pin, led_turn_off);   
    //value = turn_on;
  }
  if (request.indexOf("/MT1=ON") != -1)  {
    digitalWrite(motor1_pin, motor_turn_on);
    digitalWrite(LED_pin, led_turn_on);       
    //value = turn_off;
  }
  if (request.indexOf("/MT2=OFF") != -1)  {
    digitalWrite(motor2_pin, motor_turn_off);
    digitalWrite(LED_pin, led_turn_off);       
    //value = turn_on;
  }
  if (request.indexOf("/MT2=ON") != -1)  {
    digitalWrite(motor2_pin, motor_turn_on);
    digitalWrite(LED_pin, led_turn_on);       
    //value = turn_off;
  }  
  
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("How about watering?");

  //if(value == turn_on) {
  //  client.print("On");
  //} else {
  //  client.print("Off");
  //}
  client.println("<br><br>");
  client.println("<a href=\"/MT1=OFF\"\"><button>Motor1 Off </button></a>");
  client.println("<a href=\"/MT1=ON\"\"><button>Motor1 On </button></a><br />"); 
  client.println("<br><br>");
  client.println("<a href=\"/MT2=OFF\"\"><button>Motor2 Off </button></a>");
  client.println("<a href=\"/MT2=ON\"\"><button>Motor2 On </button></a><br />");   
  client.println("</html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
} 
