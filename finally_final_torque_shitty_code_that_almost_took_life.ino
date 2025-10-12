#include <ESP8266WiFi.h>

WiFiClient client;
WiFiServer server(80);

#define in1 D5
#define in2 D6
#define in3 D7
#define in4 D8
#define ena D1
#define enb D2
#define ledl D3// d3 pe right led on right dabane pe left jalega 
#define ledr D4// d4 pe left led on

int speed=400;  // Default motor speed

void setup() {
//led always on
  Serial.begin(9600);
  delay(100);
  
  // Create Access Point
  WiFi.softAP("ESPCARbotbot","1234567890");
  
  Serial.println();
  Serial.println("NodeMCU Started!");
  Serial.print("Access Point IP: ");
  Serial.println(WiFi.softAPIP());
  
  // Start server
  server.begin();
  Serial.println("Server started");
  
  // Set motor pins as output
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(ledl , OUTPUT);
  pinMode(ledr , OUTPUT);
  
  // Initialize motors to stop
  s();
}

// ===== Motor control functions =====
void fd() { Serial.println("Forward"); digitalWrite(in1,HIGH); digitalWrite(in2,LOW); digitalWrite(in3,HIGH); digitalWrite(in4,LOW); analogWrite(ena,speed); analogWrite(enb,speed);
  digitalWrite(ledl,HIGH);
digitalWrite(ledr,HIGH);
 }

void ld() { Serial.println("Left"); digitalWrite(in1,LOW); digitalWrite(in2,LOW); digitalWrite(in3,HIGH); digitalWrite(in4,LOW); analogWrite(ena,speed/2); analogWrite(enb,speed); 

digitalWrite(ledl,HIGH);

digitalWrite(ledr,LOW);

}

void bd() { Serial.println("Backward"); digitalWrite(in1,LOW); digitalWrite(in2,HIGH); digitalWrite(in3,LOW); digitalWrite(in4,HIGH); analogWrite(ena,speed); analogWrite(enb,speed);
  digitalWrite(ledl,HIGH);
digitalWrite(ledr,HIGH);
 }

void rd() { Serial.println("Right"); digitalWrite(in1,HIGH); digitalWrite(in2,LOW); digitalWrite(in3,LOW); digitalWrite(in4,LOW); analogWrite(ena,speed); analogWrite(enb,speed/2);
digitalWrite(ledr,HIGH);

digitalWrite(ledl,LOW);

 }

void s()  { Serial.println("Stop"); digitalWrite(in1,LOW); digitalWrite(in2,LOW); digitalWrite(in3,LOW); digitalWrite(in4,LOW); analogWrite(ena,0); analogWrite(enb,0); 
digitalWrite(ledr,LOW);

digitalWrite(ledl,LOW);
}

void cw() { Serial.println("Clockwise"); digitalWrite(in1,HIGH); digitalWrite(in2,LOW); digitalWrite(in3,LOW); digitalWrite(in4,HIGH); analogWrite(ena,speed); analogWrite(enb,speed);
  digitalWrite(ledl,HIGH);
digitalWrite(ledr,HIGH);
 }

void acw(){ Serial.println("Anti-Clockwise"); digitalWrite(in1,LOW); digitalWrite(in2,HIGH); digitalWrite(in3,HIGH); digitalWrite(in4,LOW); analogWrite(ena,speed); analogWrite(enb,speed);
  digitalWrite(ledl,HIGH);
digitalWrite(ledr,HIGH);
 }

void bld(){ Serial.println("Backward Left"); digitalWrite(in1,LOW); digitalWrite(in2,HIGH); digitalWrite(in3,LOW); digitalWrite(in4,HIGH); analogWrite(ena,speed/5); analogWrite(enb,speed); }

void brd(){ Serial.println("Backward Right"); digitalWrite(in1,LOW); digitalWrite(in2,HIGH); digitalWrite(in3,LOW); digitalWrite(in4,HIGH); analogWrite(ena,speed); analogWrite(enb,speed/5); }

void loop() {

  client = server.available();
  
  if (client) {
    

    Serial.println("Client connected");
    
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();
    
    // Process commands
    if (request.indexOf("/?dir=1") != -1) fd();
    else if (request.indexOf("/?dir=4") != -1) ld();
    else if (request.indexOf("/?dir=3") != -1) bd();
    else if (request.indexOf("/?dir=2") != -1) rd();
    else if (request.indexOf("/?dir=5") != -1) s();
    else if (request.indexOf("/?dir=7") != -1) acw();
    else if (request.indexOf("/?dir=6") != -1) cw();
    else if (request.indexOf("/?dir=8") != -1) bld();
    else if (request.indexOf("/?dir=9") != -1) brd();
    else if (request.indexOf("/?dir=a1") != -1) speed=50;
    else if (request.indexOf("/?dir=a2") != -1) speed=100;
    else if (request.indexOf("/?dir=a3") != -1) speed=150;
    else if (request.indexOf("/?dir=a4") != -1) speed=200;
    else if (request.indexOf("/?dir=a5") != -1) speed=250;

    
    // ===== Serve the Webpage =====
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML><html>");
    client.println("<head><title>RC Car Control</title>");
    client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    client.println("<style>");
    client.println("body{background-color:white;font-family:Arial;text-align:center;}");
    client.println("h1{color:#333;}");
    client.println("button{width:100px;height:50px;margin:5px;font-size:16px;border-radius:10px;border:none;cursor:pointer;box-shadow:0px 2px 5px rgba(0,0,0,0.3);}");
    client.println("button:hover{background:#ddd;}");
    client.println(".updown{display:block;margin:10px auto;}");
    client.println("</style></head><body>");
    client.println("<h1>RC Car Web Control</h1>");
    
    client.println("<div>");
    client.println("<button class='updown' onclick=\"location.href='/?dir=1'\">Forward</button><br>");
    client.println("<button onclick=\"location.href='/?dir=4'\">Left</button>");
    client.println("<button onclick=\"location.href='/?dir=5'\">Stop</button>");
    client.println("<button onclick=\"location.href='/?dir=2'\">Right</button><br>");
    client.println("<button class='updown' onclick=\"location.href='/?dir=3'\">Backward</button><br><br>");
    client.println("<button onclick=\"location.href='/?dir=6'\">ACW</button>");
    client.println("<button onclick=\"location.href='/?dir=7'\">CW</button><br><br>");
    client.println("<button onclick=\"location.href='/?dir=a1'\">Speed -</button>");
    client.println("<button onclick=\"location.href='/?dir=a2'\">Speed Reset</button>");
    client.println("<button onclick=\"location.href='/?dir=a3'\">Speed +</button>");
    client.println("</div>");
    
    client.println("<p>Current Speed: " + String(speed) + "</p>");
    client.println("</body></html>");
    
    delay(10);
    client.stop();
    Serial.println("Client disconnected");
  }
}