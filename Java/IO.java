import java.util.Scanner;

public class IO {
    private static Scanner in;

    IO() {
        in = new Scanner(System.in);
    }

    public void terminate() {
        in.close();
    }

    public String getString(String prompt) {
        System.out.print(prompt);
        String s = in.nextLine();
        return s;
    }

    public int getInt(String prompt) {
        System.out.print(prompt);
        int i = in.nextInt();
        return i;
    }

    public float getFloat(String prompt) {
        System.out.print(prompt);
        float f = in.nextFloat();
        return f;
    }
}
