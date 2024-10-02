import java.io.FileInputStream;
import java.io.FileOutputStream;

class QuickSort {
    public static void main(String[] args) {
        int[] inputs = new int[100];
        int index = 0;

        try{
            FileInputStream fileInputStream = new FileInputStream("input_sort.txt");
            FileOutputStream fileOutputStream = new FileOutputStream("output_quick_sort.xlsx");

            String input = "";
            int i = 0;
            while ((i = fileInputStream.read()) != -1) {
                if(i != '\n') { input += (char) i; }
                else {
                    input = input.trim();
                    inputs[index++] = Integer.parseInt(input);
                    input = "";
                }
            }
            fileInputStream.close();

            QSort sort = new QSort();

            long start = System.nanoTime();
            sort.QuickSort(inputs, 0, index-1);
            long finish = System.nanoTime();

            for(i = 0; i < 100; i++) {
                input = inputs[i]+"\n";

                fileOutputStream.write(input.getBytes());
                input = input.trim();
            }
            
            fileOutputStream.close();
            System.out.println("running time (ns): "+(finish-start));

        }catch (Exception e) {
            System.out.println(e);
        }
    }
}

class QSort {
    public void QuickSort(int[] inputs, int s, int e) {
        if(s < e) {
            int p = pivot(inputs, s, e);
            int temp = inputs[s]; inputs[s] = inputs[p]; inputs[p] = temp;

            p = s;  int p_e = e;
            for(int i = s+1; i <= p_e; i++, p++) {
                if(inputs[s] < inputs[i]) {
                    for(; p_e > p; p_e--) {
                        if(inputs[p_e] <= inputs[s]) {
                            temp = inputs[p_e]; inputs[p_e] = inputs[i]; inputs[i] = temp;
                            break;
                        }
                    }
                    if(p_e <= p) break;
                }
            }
            temp = inputs[s]; inputs[s] = inputs[p]; inputs[p] = temp;

            QuickSort(inputs, s, p-1);
            QuickSort(inputs, p+1, e);
        }
    }

    public int pivot(int[] inputs, int s, int e) {
        int div = (e-s+1)/3;  if(div < 1) return s;
        if(e-s+1 == div*3+1) div -=1;

        int first = mid(inputs, s, s+div);
        int second = mid(inputs, s+div+1, s+2*div+1);
        int third = mid(inputs, s+2*div+2, e);

        // System.out.println(first+": "+inputs[first]+"\t"+second+": "+inputs[second]+"\t"+third+": "+inputs[third]);
        // System.out.println();
        if(inputs[first] >= inputs[second]) {
            if(inputs[first] <= inputs[third]) return first;
            else if(inputs[second] >= inputs[third]) return second;
            else return third;
        }
        else {
            if(inputs[first] >= inputs[third]) return first;
            else if(inputs[second] <= inputs[third]) return second;
            else return third;
        }
    }

    public int mid(int[] inputs, int s, int e) {
        int mid = (s+e)/2;

        // System.out.println(s+": "+inputs[s]+"\t"+mid+": "+inputs[mid]+"\t"+e+": "+inputs[e]);
        if(inputs[s] >= inputs[mid]) {
            if(inputs[s] <= inputs[e]) return s;
            else if(inputs[mid] >= inputs[e]) return mid;
            else return e;
        }
        else {
            if(inputs[s] >= inputs[e]) return s;
            else if(inputs[mid] <= inputs[e]) return mid;
            else return e;
        }
    }
}