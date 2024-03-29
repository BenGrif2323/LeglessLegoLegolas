import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public class Printer {
    private int headPosX;
    private int headPosY;
    private int headPosZ;
    private int maxY;
    private int maxX;
    private int maxZ;
    private final int stepSizeX = 1;
    private final int stepSizeY = 1;
    private final int stepSizeZ = 1;
    private boolean outOfBounds;
    private int placingY;
    private String output;
    private final String inoBegin = "#define PIN_VRX A1" + System.lineSeparator() +
            "#define PIN_VRY A0" + System.lineSeparator() +
            "#define PIN_SWITCH A2" + System.lineSeparator() + System.lineSeparator() +
            "// Pinout according to https://github.com/leoheck/developower-v2.0/blob/master/board.pdf"
            + System.lineSeparator() +
            "#define PIN_DISABLE_MOTORS 8" + System.lineSeparator() + System.lineSeparator() +
            "#define PIN_DIR_X 5" + System.lineSeparator() +
            "#define PIN_STEP_X 2" + System.lineSeparator() +
            "#define PIN_DIR_Y 6" + System.lineSeparator() +
            "#define PIN_STEP_Y 3" + System.lineSeparator() +
            "#define PIN_DIR_Z 7" + System.lineSeparator() +
            "#define PIN_STEP_Z 4" + System.lineSeparator() + System.lineSeparator() +
            "// Not entirely sure about these ones" + System.lineSeparator() +
            "#define PIN_ZPLUSMINUS 12" + System.lineSeparator() +
            "#define PIN_YPLUSMINUS 10" + System.lineSeparator() +
            "#define PIN_XPLUSMINUS 9" + System.lineSeparator() + System.lineSeparator() +
            "#define NSAMPLES 8" + System.lineSeparator() +
            "typedef struct {" + System.lineSeparator() +
            "  int samples[NSAMPLES];" + System.lineSeparator() +
            "  int curr;" + System.lineSeparator() +
            "} Sampler;" + System.lineSeparator() + System.lineSeparator() +
            "int averaged(Sampler *s, int value) {" + System.lineSeparator() +
            "  s->samples[s->curr++] = value;" + System.lineSeparator() +
            "  s->curr %= NSAMPLES;" + System.lineSeparator() + System.lineSeparator() +
            "  int sum = 0;" + System.lineSeparator() +
            "  for (int i = 0; i < NSAMPLES; i++) {" + System.lineSeparator() +
            "    sum += s->samples[i];" + System.lineSeparator() +
            "  }" + System.lineSeparator() + System.lineSeparator() +
            "  return sum / NSAMPLES;" + System.lineSeparator() +
            "}" + System.lineSeparator() + System.lineSeparator() +
            "void initSampler(Sampler *s, int value) {" + System.lineSeparator() +
            "  for (int i = 0; i < NSAMPLES; i++) s->samples[i] = value;" + System.lineSeparator() +
            "}" + System.lineSeparator() + System.lineSeparator() +
            "Sampler xSampler;" + System.lineSeparator() +
            "Sampler ySampler;" + System.lineSeparator() + System.lineSeparator() +
            "int dx = 0;" + System.lineSeparator() +
            "int dy = 0;" + System.lineSeparator() +
            "//int sw = 0;" + System.lineSeparator() +
            "int stepDelay = 40;" + System.lineSeparator() + System.lineSeparator() +
            "int map3(int val, int min1, int cen1, int max1, int min2, int cen2, int max2) {" + System.lineSeparator() +
            "  if (val < cen1) {" + System.lineSeparator() +
            "    return map(val, min1, cen1, min2, cen2);" + System.lineSeparator() +
            "  } else {" + System.lineSeparator() +
            "    return map(val, cen1, max1, cen2, max2);" + System.lineSeparator() +
            "  }" + System.lineSeparator() +
            "}" + System.lineSeparator() + System.lineSeparator() +
            "void move(int x, int absX, int y, int absY, int z, int absZ, int max) {" + System.lineSeparator() +
            "  if (x >= 0) {" + System.lineSeparator() +
            "    //X Steps in Positive Direction" + System.lineSeparator() +
            "  }" + System.lineSeparator() +
            "  else {" + System.lineSeparator() +
            "    //X Steps in Negative Direction" + System.lineSeparator() +
            "  }" + System.lineSeparator() +
            "  if (y >= 0) {" + System.lineSeparator() +
            "    //Y Steps in Positive Direction" + System.lineSeparator() +
            "  }" + System.lineSeparator() +
            "  else {" + System.lineSeparator() +
            "    //Y Steps in Negative Direction" + System.lineSeparator() +
            "  }" + System.lineSeparator() +
            "  if (z >= 0) {" + System.lineSeparator() +
            "    //Z Steps in Positive Direction" + System.lineSeparator() +
            "  }" + System.lineSeparator() +
            "  else {" + System.lineSeparator() +
            "    //Z Steps in Negative Direction" + System.lineSeparator() +
            "  }" + System.lineSeparator() +
            "  for (int i = 0; i < max; i++) {" + System.lineSeparator() +
            "    if (i < absX) {" + System.lineSeparator() +
            "      digitalWrite(PIN_STEP_X, HIGH);" + System.lineSeparator() +
            "    }" + System.lineSeparator() +
            "    if (i < absY) {" + System.lineSeparator() +
            "      digitalWrite(PIN_STEP_Y, HIGH);" + System.lineSeparator() +
            "    }" + System.lineSeparator() +
            "    /*if (i < absZ) {" + System.lineSeparator() +
            "      digitalWrite(PIN_STEP_Z, HIGH);" + System.lineSeparator() +
            "    }*/" + System.lineSeparator() +
            "    delayMicroseconds(stepDelay);" + System.lineSeparator() +
            "    digitalWrite(PIN_STEP_X, LOW);" + System.lineSeparator() +
            "    digitalWrite(PIN_STEP_Y, LOW);" + System.lineSeparator() +
            "    //digitalWrite(PIN_STEP_Z, LOW);" + System.lineSeparator() +
            "    delayMicroseconds(stepDelay);" + System.lineSeparator() +
            "  }" + System.lineSeparator() +
            "}" + System.lineSeparator() + System.lineSeparator() +
            "// Determined experimentally by measuring analogRead(PIN_VR{X,Y})" + System.lineSeparator() +
            "#define CENX 497" + System.lineSeparator() +
            "#define CENY 526" + System.lineSeparator() +
            "// Determined by making it big enough to reduce unwanted stepping" + System.lineSeparator() +
            "#define SLOP 7" + System.lineSeparator() + System.lineSeparator() +
            "void setup() {" + System.lineSeparator() + System.lineSeparator() +
            "  //put your setup code here, to run once:" + System.lineSeparator() +
            "  pinMode(PIN_DISABLE_MOTORS, OUTPUT);" + System.lineSeparator() +
            "  pinMode(PIN_VRX, INPUT);" + System.lineSeparator() +
            "  pinMode(PIN_VRY, INPUT);" + System.lineSeparator() +
            "  pinMode(PIN_STEP_X, OUTPUT);" + System.lineSeparator() +
            "  pinMode(PIN_STEP_Y, OUTPUT);" + System.lineSeparator() +
            "  pinMode(PIN_DIR_X, OUTPUT);" + System.lineSeparator() +
            "  pinMode(PIN_DIR_Y, OUTPUT);" + System.lineSeparator() +
            "  pinMode(PIN_SWITCH, INPUT_PULLUP);" + System.lineSeparator() +
            "  //analogReadResolution(10);" + System.lineSeparator() +
            "  //Serial.begin(9600);" + System.lineSeparator() + System.lineSeparator() +
            "  initSampler(&xSampler, 512);" + System.lineSeparator() +
            "  initSampler(&ySampler, 512);" + System.lineSeparator() + System.lineSeparator();
    private final String inoEnd = "}" + System.lineSeparator() + System.lineSeparator() +
            "void loop() {" + System.lineSeparator() +
            "  //sw = digitalRead(PIN_SWITCH);" + System.lineSeparator() +
            "  //stepDelay = sw? 120 : 40;" + System.lineSeparator() + "}";
    private final String init = "  /*****************************************"
            + System.lineSeparator() + "  code to initialize head location" + System.lineSeparator()
            + "  *****************************************/" + System.lineSeparator() + System.lineSeparator();

    private String moveTo(int x, int y, int z) {
        int tempX = (x - headPosX) / stepSizeX;
        int tempY = (y - headPosY) / stepSizeY;
        int tempZ = (z - headPosZ) / stepSizeZ;
        int tempMax = Math.max(Math.max(Math.abs(tempX), Math.abs(tempY)), Math.abs(tempZ));
        String out = "  move(" + Integer.toString(tempX) + ", " + Integer.toString(Math.abs(tempX)) + ", "
                + Integer.toString(tempY) + ", " + Integer.toString(Math.abs(tempY)) + ", " + Integer.toString(tempZ)
                + ", " + Integer.toString(Math.abs(tempZ)) + ", " + Integer.toString(tempMax) + ");"
                + System.lineSeparator();
        headPosX = x;
        headPosY = y;
        headPosZ = z;
        return out;
    }

    private String legoPickup() {
        String out = "  /*********************" + System.lineSeparator() + "  code to pick up lego"
                + System.lineSeparator() + "  *********************/" + System.lineSeparator() + System.lineSeparator();
        return moveTo(0, 0, 0) + System.lineSeparator() + out;
    }

    private String legoReload() {
        return moveTo(0, placingY, 0) + System.lineSeparator() + legoPickup() + moveTo(0, placingY, 0)
                + System.lineSeparator();
    }

    private String legoPlace(Lego lego) {
        String out = "  /*********************" + System.lineSeparator() + "  code to place lego"
                + System.lineSeparator() + "  *********************/" + System.lineSeparator() + System.lineSeparator();
        return moveTo(lego.getX(), placingY, lego.getZ()) + System.lineSeparator() + out;
    }

    Printer() {
        maxX = Integer.MAX_VALUE;
        maxY = Integer.MAX_VALUE;
        maxZ = Integer.MAX_VALUE;
        outOfBounds = false;
        output = "";
        initialize();
    }

    public String getOutput() {
        return output;
    }

    private void initialize() {
        output = inoBegin;
        output += init;
        headPosX = 0;
        headPosY = 0;
        headPosZ = 0;
    }

    private void end() {
        output += inoEnd;
    }

    public void normalize(ArrayList<Lego> legos) {
        int xNorm = Integer.MAX_VALUE;
        int yNorm = Integer.MAX_VALUE;
        int zNorm = Integer.MAX_VALUE;
        for (Lego lego : legos) {
            if (lego.getX() < xNorm) {
                xNorm = lego.getX();
            }
            if (lego.getY() < yNorm) {
                yNorm = lego.getY();
            }
            if (lego.getZ() < zNorm) {
                zNorm = lego.getZ();
            }
        }
        for (Lego lego : legos) {
            lego.normalize(xNorm, yNorm, zNorm);
        }
    }

    private ArrayList<ArrayList<Lego>> splitY(ArrayList<Lego> legos) {
        ArrayList<ArrayList<Lego>> out = new ArrayList<ArrayList<Lego>>();
        Collections.sort(legos, Comparator.comparing(Lego::getY));
        while (!legos.isEmpty()) {
            int tempY = legos.get(0).getY();
            if (tempY < maxY) {
                ArrayList<Lego> tempList = new ArrayList<Lego>();
                for (Lego lego : legos) {
                    if (lego.getY() == tempY) {
                        tempList.add(lego);
                    }
                }
                legos.removeAll(tempList);
                out.add(tempList);
            } else {
                outOfBounds = true;
                break;
            }
        }
        return out;
    }

    private ArrayList<ArrayList<Lego>> splitX(ArrayList<Lego> legos) {
        ArrayList<ArrayList<Lego>> out = new ArrayList<ArrayList<Lego>>();
        Collections.sort(legos, Comparator.comparing(Lego::getX));
        while (!legos.isEmpty()) {
            int tempX = legos.get(0).getX();
            if (tempX < maxX) {
                ArrayList<Lego> tempList = new ArrayList<Lego>();
                for (Lego lego : legos) {
                    if (lego.getX() == tempX) {
                        tempList.add(lego);
                    }
                }
                legos.removeAll(tempList);
                out.add(tempList);
            } else {
                outOfBounds = true;
                break;
            }
        }
        return out;
    }

    public void createPrint(ArrayList<Lego> legos) {
        if (!legos.isEmpty()) {
            normalize(legos);
            ArrayList<ArrayList<Lego>> splitListY = splitY(legos);
            for (ArrayList<Lego> listY : splitListY) {
                ArrayList<ArrayList<Lego>> splitListX = splitX(listY);
                for (ArrayList<Lego> listX : splitListX) {
                    Collections.sort(listX, Comparator.comparing(Lego::getZ));
                    for (Lego lego : listX) {
                        if (lego.getZ() < maxZ) {
                            placingY = lego.getY() + 1;
                            output += legoReload();
                            output += legoPlace(lego);
                        } else {
                            outOfBounds = true;
                            break;
                        }
                    }
                }
            }
            if (outOfBounds) {
                output = "//legos are out of bounds" + System.lineSeparator();
            } else {
                end();
            }
        } else {
            output = "//no legos found" + System.lineSeparator();
        }
    }
}
