import java.util.ArrayList;

public class Main {
    public static void main(String[] args) {
        String greeting = "Hello again, World!";
        System.out.println(greeting);
        int length = greeting.length();

        ArrayList<Character> myList = new ArrayList<Character>(length);

        for (int i = 0; i < length; i++) {
            myList.add(i, greeting.charAt(i));
        } 

        for (char c : myList) {
            System.out.println(c); 
        }
    }
}
