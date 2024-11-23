import java.io.*;
import java.util.*;

public class BubbleSotringAlgorithm {
    public static void main(String[] args) {
        List<Integer> inpnut_nums = readNumbersFromFile("input.txt");
        BubbleSort(inpnut_nums);
        writeNumbersToFile("bubble_output.txt", inpnut_nums);
    }
    
    public static List<Integer> readNumbersFromFile(String filename) {
        List<Integer> nums = new ArrayList<>();
        try(BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            String line;
            while((line = reader.readLine()) != null) {
                nums.add(Integer.parseInt(line));
            }
        } catch(IOException e) {
            e.printStackTrace();
        }
    
        return nums;
    }
    
    public static void writeNumbersToFile(String filename, List<Integer> nums) {
        try(BufferedWriter writer = new BufferedWriter(new FileWriter(filename))) {
            for(int num : nums) {
                writer.write(Integer.toString(num));
                writer.newLine();
            }
        } catch(IOException e) {
            e.printStackTrace();
        }
    }

    public static void BubbleSort(List<Integer> nums) {
        int n = nums.size();
    
        for(int pass = 1; pass <= n-1; pass++) {
            for(int i = 1; i <= n-pass; i++) {
                if(nums.get(i-1) > nums.get(i)) {
                    Collections.swap(nums, i-1, i);
                }
            }
        }
    }
}