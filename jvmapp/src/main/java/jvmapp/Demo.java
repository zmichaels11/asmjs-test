package jvmapp;

public class Demo {
    public static native void testReentrant();

    public static void main(String[] args) {
        System.out.println("Printing all arguments...");
        
        for (String arg : args) {
            System.out.println(arg);
        }

        testReentrant();
    }
}