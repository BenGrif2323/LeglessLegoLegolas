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


const String PAGE_ROOT = (
  "HTTP/1.1 200 OK\n"
  "Content-Type: text/html\n\n"
  "<!doctype html>\n"
  "<html><head><title>Armless Artist Artemis</title></head>\n"
  "<body>\n"

  "<label for=vz style='float:left'>VZ:</label><input type=range min=-100 max=100 step=20 value=0 id=vz>\n"
  "<input type=button id=down value='Calibrate down'>\n"
  "<input type=button id=up value='Calibrate up'>\n"
  "<input type=button id=stop value='STOP'>\n"
  "<br>\n"
  "<input type=button id=go-down value='DOWN'>\n"
  "<input type=button id=go-up value='UP'><br>\n"
  "<canvas id=art width=500 height=500></canvas>\n"
  "<script>\n"
  "function pointDist(o, a, p) {\n"
  "    var ax = a[0] - o[0];\n"
  "    var ay = a[1] - o[1];\n"
  "    var px = p[0] - o[0];\n"
  "    var py = p[1] - o[1];\n"
  "    var a2 = ax * ax + ay * ay\n"
  "    if (a2 == 0) {\n"
  "        return Math.sqrt(px * px + py * py);\n"
  "    }\n"

  "    return Math.abs(ax * py - px * ay)/Math.sqrt(a2)\n"
  "}\n"

  "function maxDist(p1, p2, points) {\n"
  "    var maxDist = 0;\n"
  "    var maxArg = 0;\n"

  "    for (var i = 0; i < points.length; i++) {\n"
  "        var dist = pointDist(p1, p2, points[i]);\n"
  "        if (dist > maxDist) {\n"
  "            maxDist = dist;\n"
  "            maxArg = i;\n"
  "        }\n"
  "    }\n"

  "    return [maxArg, maxDist];\n"
  "}\n"

  "function simplify(points, threshold) {\n"
  "    if (points.length <= 2) {\n"
  "        return points;\n"
  "    }\n"

  "    var start = points[0];\n"
  "    var end = points[points.length - 1];\n"
  "    var [amid, dist] = maxDist(start, end, points.slice(1, -1));\n"

  "    if (dist < threshold) {\n"
  "        return [start, end];\n"
  "    }\n"

  "    return simplify(points.slice(0, amid+2), threshold).concat(\n"
  "        simplify(points.slice(amid+1), threshold).slice(1)\n"
  "    );\n"
  "}\n"


  "var art = document.querySelector('#art');\n"
  "var ctx = art.getContext('2d');\n"
  "ctx.fillStyle = '#DDD';\n"
  "ctx.fillRect(0, 0, art.width, art.height);\n"
  "var points = [];\n"
  "var drawing = false;\n"

  "function physicalPoints(points) {\n"
  "    var rect = art.getBoundingClientRect();\n"
  "    return points.map(p => [\n"
  "        Math.floor(5000 * (p[0]/rect.width - 0.5)),\n"
  "        Math.floor(-5000 * (p[1]/rect.height - 0.5))\n"
  "    ]);\n"
  "}\n"

  "function addPoint(e, draw) {\n"
  "    var rect = art.getBoundingClientRect();\n"
  "    var x = e.clientX - rect.x;\n"
  "    var y = e.clientY - rect.y;\n"
  "    if (draw && points.length) {\n"
  "        var last = points[points.length - 1];\n"
  "        ctx.beginPath();\n"
  "        ctx.moveTo(last[0], last[1]);\n"
  "        ctx.lineTo(x, y);\n"
  "        ctx.lineWidth = 2;\n"
  "        ctx.lineCap = 'round';\n"
  "        ctx.strokeStyle = '#555'\n"
  "        ctx.stroke();\n"
  "    }\n"
  "    points.push([x, y]);\n"
  "}\n"

  "art.addEventListener('mousemove', function(e) {\n"
  "    if (!drawing) return;\n"
  "    addPoint(e, true);\n"
  "});\n"

  "art.addEventListener('mousedown', function(e) {\n"
  "    addPoint(e, false);\n"
  "    drawing = true;\n"
  "});\n"

  "function endPath(e) {\n"
  "    if (!drawing) return;\n"
  "    drawing = false;\n"
  "    addPoint(e, true);\n"
  "    ctx.beginPath();\n"
  "    var newPoints = simplify(points, 3);\n"
  "    console.log(points.length, newPoints.length);\n"
  "    points = [];\n"

  "    ctx.moveTo(newPoints[0][0], newPoints[0][1]);\n"
  "    for (var i = 1; i < newPoints.length; i++) {\n"
  "        ctx.lineTo(newPoints[i][0], newPoints[i][1]);\n"
  "    }\n"

  "    ctx.strokeStyle = '#000'\n"
  "    ctx.stroke();\n"

  "    var req = new XMLHttpRequest();\n"
  "    req.open('GET', '/path/' + physicalPoints(newPoints).join('/'));\n"
  "    req.send();\n"
  "}\n"

  "art.addEventListener('mouseup', endPath);\n"
  "art.addEventListener('mouseout', endPath);\n"


  "var vz = document.querySelector('#vz');\n"
  "vz.addEventListener('input', function() {\n"
  "    var req = new XMLHttpRequest();\n"
  "    req.open('GET', '/vz/' + this.value);\n"
  "    req.send();\n"
  "});\n"

  "document.querySelector('#down').addEventListener('click', function() {\n"
  "    vz.value = 0;\n"
  "    var req = new XMLHttpRequest();\n"
  "    req.open('GET', '/calib/down');\n"
  "    req.send();\n"
  "});\n"

  "document.querySelector('#up').addEventListener('click', function() {\n"
  "    vz.value = 0;\n"
  "    var req = new XMLHttpRequest();\n"
  "    req.open('GET', '/calib/up');\n"
  "    req.send();\n"
  "});\n"

  "document.querySelector('#stop').addEventListener('click', function() {\n"
  "    vz.value = 0;\n"
  "    var req = new XMLHttpRequest();\n"
  "    req.open('GET', '/stop');\n"
  "    req.send();\n"
  "});\n"

  "document.querySelector('#go-down').addEventListener('click', function() {\n"
  "    var req = new XMLHttpRequest();\n"
  "    req.open('GET', '/down');\n"
  "    req.send();\n"
  "});\n"

  "document.querySelector('#go-up').addEventListener('click', function() {\n"
  "    var req = new XMLHttpRequest();\n"
  "    req.open('GET', '/up');\n"
  "    req.send();\n"
  "});\n"


  "</script>\n"
  "</body></html>\n"
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

#define MAXPATH 512
int path[MAXPATH][3];
int startPoint = 0;
int endPoint = 0;

void pushPoint(int x, int y, int z) {
  path[endPoint][0] = x;
  path[endPoint][1] = y;
  path[endPoint][2] = z;
  endPoint++;
  endPoint %= MAXPATH;  
}


// Probably most of this doesn't have to be volatile, IDK.
volatile char cycle = 0;
volatile int x = 0;
volatile int y = 0;
volatile int z = 0;
volatile int vx = 0;
volatile int dx = 0;
volatile int vy = 0;
volatile int dy = 0;
volatile int vz = 0;
volatile int dz = 0;

volatile int tx = 0;
volatile int ty = 0;
volatile int tz = 0;
volatile bool ztgt = false;


void setZTarget(int target) {
  ztgt = true;
  tz = target;
  vz = 25 * constrain(tz - z, -1, 1);
  digitalWrite(PIN_DIR1, vz < 0);
}

void setZVel(int vel) {
  ztgt = false;
  vz = vel;
  digitalWrite(PIN_DIR1, vz < 0);
}

int down = 0;
int up = 0;

#define MAX_VX 25
#define MAX_VY 25

void ARDUINO_ISR_ATTR onTimer(){
  if (ztgt) {
    if (z == tz) {
      vz = 0;
    }
  } else {
    tz = z;
  }

  if (x == tx && y == ty && z == tz) {
    if (startPoint != endPoint) {
      tx = path[startPoint][0];
      ty = path[startPoint][1];
      setZTarget(path[startPoint][2]);
      startPoint++;
      startPoint %= MAXPATH;
      
      int distX = tx - x;
      int distY = ty - y;
      
      double s = sqrt(sq(distX) + sq(distY));
      vx = (int) ((distX * MAX_VX) / s);
      vy = (int) ((distY * MAX_VY) / s);

      digitalWrite(PIN_DIR3, vx < 0);
      digitalWrite(PIN_DIR2, vy < 0);
    } else if (vz == 0) {
      digitalWrite(PIN_DISABLE_MOTORS, HIGH);
    }
  }
  
  cycle = !cycle;

  if (!cycle) {
    digitalWrite(PIN_STEP1, LOW);
    digitalWrite(PIN_STEP2, LOW);
    digitalWrite(PIN_STEP3, LOW);
  } else {
    if (vz) {
      dz += abs(vz);
      if (dz >= 100) {
        dz -= 100;
        digitalWrite(PIN_STEP1, HIGH);
        z += constrain(vz, -1, 1);
      }
    } else {
      if (x == tx) vy = constrain(ty - y, -1, 1) * MAX_VY;
      if (y == ty) vx = constrain(tx - x, -1, 1) * MAX_VX;
      dx += abs(vx);
      dy += abs(vy);

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


void clearQueue() {
  endPoint = startPoint;
  tx = x;
  ty = y;
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

#define BUF_LEN 65536
char buf[BUF_LEN];

void loop() {
  neopixelWrite(LED_RGB, 0, 48, 0);
  WiFiClient client = server.available();
  if (client && client.connected()) {
    neopixelWrite(LED_RGB, 0, 0, 48);
    int read = readAddress(&client, buf, BUF_LEN, 1000);
    if (read) {
      int a,b,c;
      if (strcmp(buf, "/") == 0) {
        client.print(PAGE_ROOT);
        Serial.printf("start: %d, end: %d, vx: %d, vy: %d, vz: %d, x: %d, y: %d, z: %d, tx: %d, ty: %d, tz: %d\n", startPoint, endPoint, vx, vy, vz, x, y, z, tx, ty, tz);
      } else if (strncmp(buf, "/path", 5) == 0) {
        client.print(PAGE_OK);
        char *cur = buf + 5;
        Serial.println(cur);
        setZTarget(z);
        for (int i = 0; sscanf(cur, "/%d,%d%n", &a, &b, &c) == 2; i++) {          
          cur += c;
          if (i == 0) {
            pushPoint(a, b, up);          
          }
          pushPoint(a, b, down);
          digitalWrite(PIN_DISABLE_MOTORS, LOW);
        }
        
        int prev = (endPoint - 1) % MAXPATH;
        pushPoint(path[prev][0], path[prev][1], up);
      } else if (sscanf(buf, "/vz/%d", &a) == 1) {
        client.print(PAGE_OK);
        clearQueue();
        setZVel(constrain(a, -100, 100));
        digitalWrite(PIN_DISABLE_MOTORS, LOW);
      } else if (strcmp(buf, "/calib/up") == 0) {
        client.print(PAGE_OK);
        setZVel(0);
        up = z;
      } else if (strcmp(buf, "/calib/down") == 0) {
        client.print(PAGE_OK);
        setZVel(0);
        down = z;
      } else if (strcmp(buf, "/stop") == 0) {
        client.print(PAGE_OK);
        clearQueue();
        setZVel(0);
      } else if (strcmp(buf, "/up") == 0) {
        client.print(PAGE_OK);
        setZTarget(up);
        digitalWrite(PIN_DISABLE_MOTORS, LOW);
      } else if (strcmp(buf, "/down") == 0) {
        client.print(PAGE_OK);
        setZTarget(down);
        digitalWrite(PIN_DISABLE_MOTORS, LOW);
      } else {
        client.print(PAGE_404);        
      }
    }
    
    client.stop();
  }
}
