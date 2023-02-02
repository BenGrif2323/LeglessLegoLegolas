public class Lego {
    // in LDU
    // 1 LDU ~ 1/64 inch
    // 1 LDU ~ 0.4 mm
    int x;
    int y;
    int z;

    Lego(int i, int j, int k) {
        x = i;
        y = j;
        z = k;
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }

    public int getZ() {
        return z;
    }

    @Override
    public String toString() {
        return "(" + x + ", " + y + ", " + z + ")";
    }

    public void normalize(int i, int j, int k) {
        x -= i;
        y -= j;
        z -= k;
    }
}
