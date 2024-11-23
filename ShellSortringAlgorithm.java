import java.io.*;
import java.util.*;

public class ShellSotringAlgorithm {
    public static void main(String[] args) {
        List<Integer> inpnut_nums = readNumbersFromFile("input.txt");
        ShellSort(inpnut_nums);
        writeNumbersToFile("shell_output.txt", inpnut_nums);
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

    public static void ShellSort(List<Integer> nums) {
        int n = nums.size();

        List<Integer> gaps = new ArrayList<>();
        gaps.add(100); gaps.add(50); gaps.add(10); gaps.add(5); gaps.add(1);

        for(int gap : gaps) {
            for(int i = gap; i <= n-1; i++) {
                int CurrentElement = nums.get(i);
                int j = i;
                while(j >= gap && nums.get(j-gap) > CurrentElement) {
                    nums.set(j, nums.get(j-gap));
                    j = j-gap;
                }
                nums.set(j, CurrentElement);
            }
        }
    }
}
