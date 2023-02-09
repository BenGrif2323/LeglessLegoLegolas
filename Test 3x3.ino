#define PIN_VRX A1
#define PIN_VRY A0
#define PIN_SWITCH A2

// Pinout according to https://github.com/leoheck/developower-v2.0/blob/master/board.pdf
#define PIN_DISABLE_MOTORS 8

#define PIN_DIR_X 5
#define PIN_STEP_X 2
#define PIN_DIR_Y 6
#define PIN_STEP_Y 3
#define PIN_DIR_Z 7
#define PIN_STEP_Z 4

// Not entirely sure about these ones
#define PIN_ZPLUSMINUS 12
#define PIN_YPLUSMINUS 10
#define PIN_XPLUSMINUS 9

#define NSAMPLES 8
typedef struct {
  int samples[NSAMPLES];
  int curr;
} Sampler;
int averaged(Sampler *s, int value) {
  s->samples[s->curr++] = value;
  s->curr %= NSAMPLES;

  int sum = 0;
  for (int i = 0; i < NSAMPLES; i++) {
    sum += s->samples[i];
  }

  return sum / NSAMPLES;
}

void initSampler(Sampler *s, int value) {
  for (int i = 0; i < NSAMPLES; i++) s->samples[i] = value;
}

Sampler xSampler;
Sampler ySampler;

int dx = 0;
int dy = 0;

void setup() {
  //put your setup code here, to run once:
  pinMode(PIN_DISABLE_MOTORS, OUTPUT);
  pinMode(PIN_VRX, INPUT);
  pinMode(PIN_VRY, INPUT);
  pinMode(PIN_STEP_X, OUTPUT);
  pinMode(PIN_STEP_Y, OUTPUT);
  pinMode(PIN_DIR_X, OUTPUT);
  pinMode(PIN_DIR_Y, OUTPUT);
  pinMode(PIN_SWITCH, INPUT_PULLUP);
  //analogReadResolution(10);
  //Serial.begin(9600);

  initSampler(&xSampler, 512);
  initSampler(&ySampler, 512);
}

int map3(int val, int min1, int cen1, int max1, int min2, int cen2, int max2) {
  if (val < cen1) {
    return map(val, min1, cen1, min2, cen2);
  } else {
    return map(val, cen1, max1, cen2, max2);
  }
}

// Determined experimentally by measuring analogRead(PIN_VR{X,Y})
#define CENX 497
#define CENY 526
// Determined by making it big enough to reduce unwanted stepping
#define SLOP 7

void loop() {
//code to initialize head location
//code to move head to lego loading location
//code to pick up lego
//code to move head to (0, 0, 0)
//code to place lego
//code to move head to lego loading location
//code to pick up lego
//code to move head to (0, 0, 40)
//code to place lego
//code to move head to lego loading location
//code to pick up lego
//code to move head to (0, 0, 80)
//code to place lego
//code to move head to lego loading location
//code to pick up lego
//code to move head to (40, 0, 0)
//code to place lego
//code to move head to lego loading location
//code to pick up lego
//code to move head to (40, 0, 40)
//code to place lego
//code to move head to lego loading location
//code to pick up lego
//code to move head to (40, 0, 80)
//code to place lego
//code to move head to lego loading location
//code to pick up lego
//code to move head to (80, 0, 0)
//code to place lego
//code to move head to lego loading location
//code to pick up lego
//code to move head to (80, 0, 40)
//code to place lego
//code to move head to lego loading location
//code to pick up lego
//code to move head to (80, 0, 80)
//code to place lego
}