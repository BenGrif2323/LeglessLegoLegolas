import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class FileReader {
    private static File file;
    private static Scanner fileReader;

    FileReader(String path) {
        try {
            file = new File(path);
            System.out.println("Successfully opened " + path);
            fileReader = new Scanner(file);
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
    }

    public ArrayList<Lego> readLDR() {
        ArrayList<Lego> legos = new ArrayList<Lego>();
        while (fileReader.hasNextLine()) {
            String in = fileReader.nextLine();
            String[] data = in.split(" ");
            if (data[0].equals("1")) {
                legos.add(new Lego(Integer.parseInt(data[2]), Integer.parseInt(data[3]), Integer.parseInt(data[4])));
            }
        }
        return legos;
    }

    public void closeFile() {
        fileReader.close();
    }
}
