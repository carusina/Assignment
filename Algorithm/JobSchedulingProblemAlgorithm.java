import java.util.*;

public class JobSchedulingProblemAlgorithm {
    public static void main(String[] args) {
        int m = 4;
        List<Integer> operation_time = Arrays.asList(5, 2, 4, 3, 4, 7, 9, 2, 4, 1);

        List<List<String>> machines = new ArrayList<>();
        int end_time = Approx_JobScheduling(m, operation_time, machines);
        
        printMachines(m, end_time, machines);
    }

    public static int Approx_JobScheduling(int m, List<Integer> operation_time, List<List<String>> machines) {
        for(int i = 0; i < m; i++) {
            machines.add(new ArrayList<>());
        }
        
        List<Integer> L = new ArrayList<>();
        for(int i = 0; i < m; i++) {
            L.add(0);
        }
        
        int min;
        for(int i = 0; i < operation_time.size(); i++) {
            min = 0;
            for(int j = 1; j < m; j++) {
                if(L.get(j) < L.get(min)) {
                    min = j;
                }
            }
            machines.get(min).add("t" + (i+1) + " " + operation_time.get(i));
            L.set(min, L.get(min)+operation_time.get(i));
        }

        int end_time = 0;
        for(int i = 0; i < m; i++) {
            if(end_time < L.get(i)) {
                end_time = L.get(i);
            }
        }
        return end_time;
    }

    public static void printMachines(int m, int end_time, List<List<String>> machines) {
        System.out.print("m\\t  ");
        for(int i = 0; i < end_time; i++) {
            System.out.printf("%-5d", i);
        }
        System.out.println();

        for(int i = 0; i < m; i++) {
            System.out.printf("%-5d", i+1);
            List<String> jobs = machines.get(i);

            for(String job : jobs) {
                String[] parts =job.split(" ");
                int operation_time = Integer.parseInt(parts[1]);
                for(int j = 0; j < operation_time; j++) {
                    System.out.printf("%-5s", parts[0]);
                }
            }

            System.out.println();
        }
    }
}
