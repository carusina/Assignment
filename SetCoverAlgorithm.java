import java.io.*;
import java.lang.Character.Subset;
import java.util.*;


public class SetCoverAlgorithm {
    public static void main(String[] args) {
        Graph input_Graph = new Graph();
        
        try (BufferedReader br = new BufferedReader(new FileReader("input_Set.txt"))) {
            String line;

            while ((line = br.readLine()) != null) {
                line = line.replace("(", "").replace(")", "").replaceAll("\\s+", "");
                String[] parts = line.split(",");
                String vertex1 = parts[0];
                String vertex2 = parts[1];
                
                Edge edge = new Edge(vertex1, vertex2);
                input_Graph.addEdge(edge);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        input_Graph.updateVertices();

        input_Graph.updateSetList();
        long start = System.nanoTime();
        List<List<String>> Cover = input_Graph.SetCover();
        long finish = System.nanoTime();

        for(List<String> set : Cover) {
            System.out.println(set);
        }
        System.out.println("running time about SetCover (ns): "+(finish-start));

        System.out.println();
        System.out.println();

        input_Graph.updateSetList();
        start = System.nanoTime();
        Cover = input_Graph.SetCover_Optimal();
        finish = System.nanoTime();

        for(List<String> set : Cover) {
            System.out.println(set);
        }
        System.out.println("running time about SetCoverOptimal (ns): "+(finish-start));
    }
}

class Edge {
    String vertex1;
    String vertex2;

    public Edge(String vertex1, String vertex2) {
        this.vertex1 = vertex1;
        this.vertex2 = vertex2;
    }

    @Override
    public String toString() {
        return "(" + vertex1 + ", " + vertex2+ ")";
    }
}

class Graph {
    List<Edge> edges;
    Set<String> vertices;
    List<String> vertexList;
    List<List<String>> setList;

    public Graph() {
        edges = new ArrayList<>();
        vertices = new HashSet<>();
    }

    public void addEdge(Edge edge) {
        edges.add(edge);
    }

    public void updateVertices() {
        for (Edge edge : edges) {
            vertices.add(edge.vertex1);
            vertices.add(edge.vertex2);
        }
        vertexList = new ArrayList<>(vertices);
    }

    public void updateSetList() {
        setList = new ArrayList<>();

        for(String vertex : vertexList) {
            List<String> Subset = new ArrayList<>();

            for(Edge edge : edges) {
                if(edge.vertex1.equals(vertex)) {
                    Subset.add(edge.vertex2);
                }
                else if(edge.vertex2.equals(vertex)) {
                    Subset.add(edge.vertex1);
                }
            }
            Subset.add(vertex);
            setList.add(Subset);
        }
    }

    public List<List<String>> SetCover() {
        List<List<String>> Cover = new ArrayList<>();
        Set<String> Cover_vertices = new HashSet<>(vertices);

        while(Cover_vertices.size() > 0) {
            int max = 0;
            List<String> Max_set = null;

            for(List<String> set : setList) {
                int cnt = 0;
                
                for(String vertex : Cover_vertices) {
                    if(set.contains(vertex)) cnt++;
                }
                if(max < cnt) {
                    Max_set = set;
                    max = cnt;
                }
            }
            if(Max_set != null) {
                for(String vertex : Max_set) {
                    Cover_vertices.remove(vertex);
                }
                Cover.add(Max_set);
                setList.remove(Max_set);
            }
        }

        return Cover;
    }

    public List<List<String>> SetCover_Optimal() {
        Set<String> Cover_vertices = null;
        int setList_size = setList.size();

        for(int i = 1; i <= setList_size; i++) {
            List<List<List<String>>> combinations = getCombinations(setList, i);

            for(List<List<String>> combination : combinations) {
                Cover_vertices = new HashSet<>();

                for(List<String> subset : combination) {
                    Cover_vertices.addAll(subset);
                }

                if(Cover_vertices.containsAll(vertices)) {
                    return combination;
                }
            }
        }

        return null;
    }

    public List<List<List<String>>> getCombinations(List<List<String>> setList, int k) {
        List<List<List<String>>> result = new ArrayList<>();
        CombinationHelper(setList, new ArrayList<>(), 0, k, result);

        return result;
    }

    private void CombinationHelper(List<List<String>> setList, List<List<String>> temp, int start, int k, List<List<List<String>>> result) {
        if(temp.size() == k) {
            result.add(new ArrayList<>(temp));
            return;
        }

        for(int i = start; i < setList.size(); i++) {
            temp.add(setList.get(i));
            CombinationHelper(setList, temp, i+1, k, result);
            temp.remove(temp.size()-1);
        }
    }

    public void printGraph() {
        for(Edge edge : edges) {
            System.out.println(edge);
        }
    }

    public void printVerteices() {
        System.out.println("Vertices: " + vertices);
    }

    public void printSet() {
        for(List<String> set : setList) {
            System.out.println(set);
        }
    }
}