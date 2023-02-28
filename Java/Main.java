import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

public class Main {

    public static void main(String[] args) {
        IO io = new IO();
        String filePath = io.getString(System.lineSeparator() + ".ldr file path: ");
        FileReader ldrFile = new FileReader(filePath);
        ArrayList<Lego> legos = ldrFile.readLDR();
        ldrFile.closeFile();
        System.out.println(System.lineSeparator() + "Legos:"); // Output to check legos were read in
        for (Lego lego : legos) {
            System.out.println(lego); // Output to check legos were read in
        }
        Printer printer = new Printer();
        printer.normalize(legos);
        System.out.println(System.lineSeparator() + "Normalized:"); // Output to check normalization
        for (Lego lego : legos) {
            System.out.println(lego); // Output to check normalization
        }
        printer.createPrint(legos);
        FileWriter output;
        try {
            output = new FileWriter(filePath.replace(".ldr", ".ino"));
            output.write(printer.getOutput());
            output.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.out.println(System.lineSeparator() + "Output text for adruino code:");
        System.out.println(printer.getOutput());
        io.terminate();
    }
}
// Test File Used: Test 3x3.ldr