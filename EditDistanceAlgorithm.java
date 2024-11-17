public class EditDistanceAlgorithm {
    public static void main(String[] args) {
        String str1 = "strong";
        String str2 = "stone";
        int[][] E = new int[str1.length()+1][str2.length()+1];

        int result = EditDistance(E, str1, str2);
        printC(E, str1, str2);
    }

    public static int EditDistance(int[][] E, String str1, String str2) {
        int m = str1.length();
        int n = str2.length();

        for(int i = 0; i <= m; i++) E[i][0] = i;
        for(int j = 0; j <= n; j++) E[0][j] = j;

        for(int i = 1; i <= m; i++) {
            for(int j = 1; j <= n; j++) {
                int a = (str1.charAt(i-1) == str2.charAt(j-1)) ? 0 : 1;
                E[i][j] = Math.min(Math.min(E[i][j-1]+1, E[i-1][j]+1), E[i-1][j-1]+a);
            }
        } 

        return E[m][n];
    }

    public static void printC(int[][] E, String str1, String str2) {
        str1 = "ε"+str1;
        str2 = "ε"+str2;
        int m = str1.length();
        int n = str2.length();
        

        System.out.printf("   ");
        for (int i = 0; i < n; i++) {
            System.out.printf("%-3s", str2.charAt(i));
        }
        System.out.println();
    
        for (int i = 0; i < m; i++) {
            System.out.printf("%-3s", str1.charAt(i));
            for (int j = 0; j < n; j++) {
                System.out.printf("%-3d", E[i][j]);
            }
            System.out.println();
        }
    }
}
