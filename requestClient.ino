#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
const char* ssid = "YACHAYTECH";
const char* password = "";
int trigPin = 4;            // HC-SR04 trigger pin
int echoPin = 5;            // HC-SR04 echo pin
float duration, distance;
 
void setup () {
 
 // Setup consists on 
 //1. Pin configuration
 //2. Serial configuration
 //3. Wifi connection
 
 pinMode(2, OUTPUT);
 pinMode(trigPin, OUTPUT); // define trigger pin as output
 pinMode(echoPin, INPUT);  // define echoPin pin as input

Serial.begin(115200);
Serial.println("Initializing");
 
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) { //Make a connection attemp each 1 second 
 delay(1000);
 Serial.print("Connecting..");
 }
} //finish setup
 
void loop() {
  //the following digital writes will send a pulse to the sensor and get the time to reach an object
  digitalWrite(echoPin, LOW);   // set the echo pin LOW
  digitalWrite(trigPin, LOW);   // set the trigger pin LOW
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);  // set the trigger pin HIGH for 10μs
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);  // measure the echo time (μs)
  Serial.println(duration);
  distance = (duration/2.0)*0.0343;   // convert echo time to distance (cm)
 
    Serial.print("Distance: ");
    Serial.print(distance, 1); Serial.println(" cm");
  
 if (distance<10){ 
  digitalWrite(2, HIGH);  //power on the internal led if we have near an object 
  delay(10);

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    Serial.println("Estamos conectados");
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin("http://172.23.220.50:5000/ardu/100");  //Specify request destination
    int httpCode = http.GET();             
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      //Serial.println(payload);             //Print the response payload
    }
   }
  }
  
 else{
  digitalWrite(2, LOW); //power off the internal led if no object is present
  delay(10);
  
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    Serial.println("Estamos conectados");
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin("http://172.23.220.50:5000/ardu/0");  //Specify request destination
    int httpCode = http.GET();             
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
    }
   }
  }
delay(5000);    //Send a request every 30 seconds
 
}

//TODO: Only resend the request if a global variable modifies its status
//(the status changes only when an event occurs: eg. button pressed, distance is greater than certain value, etc)

//Also put the base api url in a variable at the beginning and use it instead of the whole url in the rest of the code
