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
    private boolean outOfBounds;
    private int placingLayer;
    private String output;

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
        output += "//code to set up .ino file" + System.lineSeparator();
        output += "//code to initialize head location" + System.lineSeparator();
        headPosX = 0;
        headPosY = maxY;
        headPosZ = 0;
    }

    private void end() {
        output += "//code to close .ino file";
    }

    private void pickUpLego() {
        output += "//code to move head to lego loading location" + System.lineSeparator();
        output += "//code to pick up lego" + System.lineSeparator();
        headPosX = 0;
        headPosY = placingLayer + 1;
        headPosZ = 0;
    }

    private void placeLego(Lego lego) {
        output += "//code to move head to (" + lego.getX() + ", " + lego.getY() + ", " + lego.getZ() + ")"
                + System.lineSeparator();
        output += "//code to place lego" + System.lineSeparator();
        headPosX = lego.getX();
        headPosY = lego.getY();
        headPosZ = lego.getZ();
    }

    public void normalize(ArrayList<Lego> legos) {
        int xNorm = Integer.MAX_VALUE;
        int yNorm = Integer.MAX_VALUE;
        int zNorm = Integer.MAX_VALUE;
        for (Lego lego : legos) {
            if (lego.getX() < xNorm) {
                xNorm = lego.getX();
            }
            ;
            if (lego.getY() < yNorm) {
                yNorm = lego.getY();
            }
            ;
            if (lego.getZ() < zNorm) {
                zNorm = lego.getZ();
            }
            ;
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
                            placingLayer = lego.getY();
                            pickUpLego();
                            placeLego(lego);
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
