import java.util.ArrayList;

public class Main {
    
    public static void main(String[] args) {
        IO.initialize();
        FileManager.openFile(IO.getString(".ldr file path: "));
        ArrayList<Lego> legos = FileManager.readLDR();
        int xNorm = Integer.MAX_VALUE;
        int yNorm = Integer.MAX_VALUE;
        int zNorm = Integer.MAX_VALUE;
        System.out.println("Legos:");  //Output to check legos were read in
        for (Lego lego : legos) {
            System.out.println(lego);  //Output to check legos were read in
            if (lego.getX() < xNorm) {
                xNorm = lego.getX();
            };
            if (lego.getY() < yNorm) {
                yNorm = lego.getY();
            };
            if (lego.getZ() < zNorm) {
                zNorm = lego.getZ();
            };
        }
        System.out.println("Normalized:");  //Output to check normalization
        for (Lego lego : legos) {
            lego.normalize(xNorm, yNorm, zNorm);
            System.out.println(lego);  //Output to check normalization
        }
        FileManager.closeFile();
        IO.terminate();
    }
}
// Test File Used: /Users/benjamingrifka/Downloads/Test 3x3.ldr