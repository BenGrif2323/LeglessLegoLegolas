import java.util.Scanner;

public class IO {
    private static Scanner in;
    
    public static void initialize() {
        in = new Scanner(System.in);
    }

    public static void terminate() {
        in.close();
    }

    public static String getString(String prompt) {
        System.out.print(prompt);
        String s = in.nextLine();
        return s;
    }

    public static int getInt(String prompt) {
        System.out.print(prompt);
        int i = in.nextInt();
        return i;
    }

    public static float getFloat(String prompt) {
        System.out.print(prompt);
        float f = in.nextFloat();
        return f;
    }
}
