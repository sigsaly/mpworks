#include <ESP8266WiFi.h>

#define ND_D0 16
#define ND_D1 5
#define ND_D2 4
#define ND_D3 0
#define ND_D4 2
#define ND_D5 14
#define ND_D6 12
#define ND_D7 13
#define ND_D8 15
#define ND_RX 3
#define ND_TX 1 
#define ND_S2 10
#define ND_S3 9

const char* ssid = "";
const char* password = "";

WiFiServer server(80);


int water_sense_pin = ND_D2;
int LED_pin = ND_D4;
int motor1_pin = ND_D3;
int motor2_pin = ND_D5;
int led_turn_on = 0;
int led_turn_off = 1;
int motor_turn_on = 1;
int motor_turn_off = 0;
int water_sense_value = 0;
  
void setup() {
  Serial.begin(115200);
  delay(10);  

  pinMode(LED_pin, OUTPUT);
  pinMode(motor1_pin, OUTPUT);
  pinMode(motor2_pin, OUTPUT);
  pinMode(water_sense_pin, INPUT);
  
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

  water_sense_value = digitalRead(water_sense_pin);
  //Serial.print("water_sense_value:");
  //Serial.println(water_sense_value);  

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
  
  if (request.indexOf("/MT1=OFF") != -1)  {
    digitalWrite(motor1_pin, motor_turn_off);
    digitalWrite(LED_pin, led_turn_off);   
  }
  if (request.indexOf("/MT1=ON") != -1)  {
    if (water_sense_value)
      digitalWrite(motor1_pin, motor_turn_on);
    digitalWrite(LED_pin, led_turn_on);       
  }
  if (request.indexOf("/MT2=OFF") != -1)  {
    digitalWrite(motor2_pin, motor_turn_off);
    digitalWrite(LED_pin, led_turn_off);       
  }
  if (request.indexOf("/MT2=ON") != -1)  {
    if (water_sense_value)
      digitalWrite(motor2_pin, motor_turn_on);
    digitalWrite(LED_pin, led_turn_on);       
  }  
  
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #4caf50; border: none; color: white; padding: 15px 32px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println("</style></head>");
  
  if (water_sense_value)
    client.print("<body><h1>How about watering?</h1>");
  else
    client.print("<body><h1>No water. Fill the water first</h1>");
    
  client.println("<br><br>");
  client.println("<a href=\"/MT1=OFF\"\"><button>Motor1 Off </button></a>");
  client.println("<a href=\"/MT1=ON\"\"><button>Motor1 On </button></a><br />"); 
  client.println("<br><br>");
  client.println("<a href=\"/MT2=OFF\"\"><button>Motor2 Off </button></a>");
  client.println("<a href=\"/MT2=ON\"\"><button>Motor2 On </button></a><br />");   
  client.println("</body></html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
} 
