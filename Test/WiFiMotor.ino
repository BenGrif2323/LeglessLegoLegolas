#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char *ssid = "LeglessLegoLegolas";
const char *password = NULL;
WiFiServer server(80);

#define LED_RGB 48
#define PIN_STEP1 40
#define PIN_STEP2 47
#define PIN_STEP3 39
#define PIN_STEP4 37

#define PIN_DIR1 45
#define PIN_DIR2 21
#define PIN_DIR3 14
#define PIN_DIR4 35

#define PIN_DISABLE_MOTORS 46

const String PAGE_ROOT_HEADER = (
  "HTTP/1.1 200 OK\n"
  "Content-Type: text/html\n\n"
  "<!doctype html>\n"
  "<html><body>\n"
  "<h1>Set speed:</h1>\n"
);

const String PAGE_ROOT_FORMAT = (
  "<input type=range min=-100 max=100 value=%d class=slider id=velocity>\n"
);

const String PAGE_ROOT_FOOTER = (
  "<script>\n"
  "document.getElementById('velocity').oninput = function() {\n"
  "    var req = new XMLHttpRequest();\n"
  "    req.open('GET', '/' + this.value);\n"
  "    req.send();\n"
  "}\n"
  "</script>\n"
  "</body></html>"
);


const String PAGE_OK = (
  "HTTP/1.1 200 OK\n"
  "Content-Type: text/plain\n\n"
  "OK"
);


const String PAGE_404 = (
  "HTTP/1.1 404 Not Found\n"
  "Content-Type: text/html\n\n"
  "<h1>404 Not Found</h1>\n"
);


hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

volatile char cycle = 0;
volatile int vrx = 0;
volatile int dx = 0;

void ARDUINO_ISR_ATTR onTimer(){
  portENTER_CRITICAL_ISR(&timerMux);
  cycle = !cycle;

  if (cycle) {
    if (dx >= 100) {
      dx -= 100;
    }
    dx += abs(vrx);
  }
  portEXIT_CRITICAL_ISR(&timerMux);

  if (!cycle) {
    digitalWrite(PIN_STEP1, LOW);
  } else if (dx >= 100) {
    digitalWrite(PIN_STEP1, HIGH);
  }
}


void setup() {
  Serial.begin(115200);
  neopixelWrite(LED_RGB, 0, 0, 0);
  
  pinMode(PIN_DISABLE_MOTORS, OUTPUT);
  pinMode(PIN_STEP1, OUTPUT);
  pinMode(PIN_STEP2, OUTPUT);
  pinMode(PIN_DIR1, OUTPUT);
  pinMode(PIN_DIR2, OUTPUT);

  if (!WiFi.softAP(ssid, password)) {
    neopixelWrite(LED_RGB, 48, 0, 0);
    while(1);
  }

  IPAddress myIP = WiFi.softAPIP();
  server.begin();

  // Use 1st timer of 4 (counted from zero).
  // Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more
  // info).
  timer = timerBegin(0, 80, true);

  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 100, true);
  timerAlarmEnable(timer);
}


size_t readAddress(WiFiClient* client, char* buf, size_t len, unsigned long timeout) {
  const char *pat = "GET ";
  unsigned long start = millis();
  char c;
  
  for (int i = 0; i < 4; i++) {
    while ((c = client->read()) == -1) {
      if (millis() - start > timeout) return 0;
    }
    if (c != pat[i]) {
      return 0;
    }
  }
  
  for (int i = 0; i < len - 1; i++) {
    while ((c = client->read()) == -1) {
      if (millis() - start > timeout) return 0;
    }
    if (c == ' ') {
      buf[i] = 0;
      return i;
    }
    buf[i] = c;
  }

  buf[len - 1] = 0;
  return len - 1;
}

#define BUF_LEN 256
char buf[BUF_LEN];

void loop() {
  neopixelWrite(LED_RGB, 0, 48, 0);
  WiFiClient client = server.available();
  if (client && client.connected()) {
    neopixelWrite(LED_RGB, 0, 0, 48);
    int read = readAddress(&client, buf, BUF_LEN, 1000);
    if (read) {
      int new_vrx;
      if (strcmp(buf, "/") == 0) {
        client.print(PAGE_ROOT_HEADER);
        client.printf(PAGE_ROOT_FORMAT.c_str(), vrx);
        client.print(PAGE_ROOT_FOOTER);
      } else if (sscanf(buf, "/%d", &new_vrx) == 1) {
          client.print(PAGE_OK);
          vrx = constrain(new_vrx, -100, 100);
          digitalWrite(PIN_DIR1, vrx < 0);
          Serial.printf("vx: %d, dx: %d\n", vrx, dx);
      } else {
        client.print(PAGE_404);        
      }
    }
    
    client.stop();
  }
}