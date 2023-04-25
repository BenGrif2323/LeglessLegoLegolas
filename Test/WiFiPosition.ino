#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char *ssid = "ArmlessArtistArtemis";
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
  "<html><head><title>Armless Artist Artemis</title></head>\n"
  "<body>\n"
  // "<h1>Draw</h1>\n"
);

const String PAGE_ROOT_FORMAT = (
  "<label for=vz style='float:left'>VZ:</label><input type=range min=-100 max=100 step=20 value=%d id=vz><br>\n"
);

const String PAGE_ROOT_FOOTER = (
  "<canvas id=art width=500 height=500></canvas>\n"
  "<script>\n"
  "var art = document.getElementById('art');\n"
  "var ctx = art.getContext('2d');\n"
  "ctx.fillStyle = '#DDD';\n"
  "ctx.fillRect(0, 0, art.width, art.height);\n\n"
  "art.addEventListener('click', e => {\n"
  "  var rect = art.getBoundingClientRect();\n"
  "  var x = e.clientX - rect.x;\n"
  "  var y = e.clientY - rect.y;\n"
  "  ctx.beginPath();\n"
  "  ctx.arc(x, y, 5, 0, 2 * Math.PI);\n"
  "  ctx.closePath();\n"
  "  ctx.fillStyle = '#000'\n"
  "  ctx.fill();\n\n"
  "  var px = 5000 * (x / rect.width - 0.5);\n"
  "  var py = -5000 * (y / rect.height - 0.5);\n\n"
  "  var req = new XMLHttpRequest();\n"
  "  req.open('GET', '/xy/' + Math.floor(px) + '/' + Math.floor(py));\n"
  "  req.send();\n"
  "})\n\n"

  "document.querySelectorAll('#vz').forEach(el => el.oninput = function() {\n"
  "    var req = new XMLHttpRequest();\n"
  "    req.open('GET', '/' + this.id + '/' + this.value);\n"
  "    req.send();\n"
  "})\n"
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
// portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#define MAXPATH 256
int path[MAXPATH][2];
int startPoint = 0;
int endPoint = 0;


// Probably most of this doesn't have to be volatile, IDK.
volatile char cycle = 0;
volatile int x = 0;
volatile int y = 0;
volatile int vrx = 0;
volatile int dx = 0;
volatile int vry = 0;
volatile int dy = 0;
volatile int vrz = 0;
volatile int dz = 0;

volatile int tx = 0;
volatile int ty = 0;

#define MAX_VX 25
#define MAX_VY 25

void ARDUINO_ISR_ATTR onTimer(){
  if (x == tx && y == ty) {
    if (startPoint != endPoint) {
      tx = path[startPoint][0];
      ty = path[startPoint][1];
      startPoint++;
      startPoint %= MAXPATH;
      
      int distX = tx - x;
      int distY = ty - y;
      
      double s = sqrt(sq(distX) + sq(distY));
      vrx = (int) ((distX * MAX_VX) / s);
      vry = (int) ((distY * MAX_VY) / s);

      digitalWrite(PIN_DIR3, vrx < 0);
      digitalWrite(PIN_DIR2, vry < 0);
    } else if (vrz == 0) {
      digitalWrite(PIN_DISABLE_MOTORS, HIGH);      
    }
  }
  
  cycle = !cycle;

  if (!cycle) {
    digitalWrite(PIN_STEP1, LOW);
    digitalWrite(PIN_STEP2, LOW);
    digitalWrite(PIN_STEP3, LOW);
  } else {
    if (x == tx) vry = constrain(ty - y, -1, 1) * MAX_VY;
    if (y == ty) vrx = constrain(tx - x, -1, 1) * MAX_VX;
    dx += abs(vrx);
    dy += abs(vry);
    dz += abs(vrz);

    if (dx >= 100) {
      dx -= 100;
      digitalWrite(PIN_STEP3, HIGH);
      x += constrain(tx - x, -1, 1);
    }

    if (dy >= 100) {  
      dy -= 100;
      digitalWrite(PIN_STEP2, HIGH);
      y += constrain(ty - y, -1, 1);
    }

    if (dz >= 100) {
      dz -= 100;
      digitalWrite(PIN_STEP1, HIGH);
    }
  }
}


void setup() {
  Serial.begin(115200);
  neopixelWrite(LED_RGB, 0, 0, 0);
  
  pinMode(PIN_DISABLE_MOTORS, OUTPUT);
  pinMode(PIN_STEP1, OUTPUT);
  pinMode(PIN_STEP2, OUTPUT);
  pinMode(PIN_STEP3, OUTPUT);
  pinMode(PIN_DIR1, OUTPUT);
  pinMode(PIN_DIR2, OUTPUT);
  pinMode(PIN_DIR3, OUTPUT);

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
      int a,b;
      if (strcmp(buf, "/") == 0) {
        client.print(PAGE_ROOT_HEADER);
        client.printf(PAGE_ROOT_FORMAT.c_str(), vrz);
        client.print(PAGE_ROOT_FOOTER);
        
        Serial.printf("start: %d, end: %d, vrx: %d, vry: %d, x: %d, y: %d, tx: %d, ty: %d\n", startPoint, endPoint, vrx, vry, x, y, tx, ty);
      } else if (sscanf(buf, "/xy/%d/%d", &a, &b) == 2) {
          client.print(PAGE_OK);
          path[endPoint][0] = a;
          path[endPoint][1] = b;
          endPoint++;
          endPoint %= MAXPATH;
          digitalWrite(PIN_DISABLE_MOTORS, LOW);
      } else if (sscanf(buf, "/vz/%d", &a) == 1) {
          client.print(PAGE_OK);
          vrz = constrain(a, -100, 100);
          digitalWrite(PIN_DIR1, vrz < 0);
          digitalWrite(PIN_DISABLE_MOTORS, LOW);
      } else {
        client.print(PAGE_404);        
      }
    }
    
    client.stop();
  }
}