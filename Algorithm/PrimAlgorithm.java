import java.io.*;
import java.util.*;


public class PrimAlgorithm {
    public static void main(String[] args) {
        Graph input_Graph = new Graph();
        
        try (BufferedReader br = new BufferedReader(new FileReader("input_Prim.txt"))) {
            String line;

            while ((line = br.readLine()) != null) {
                line = line.replace("(", "").replace(")", "");
                String[] parts = line.split(",");
                String vertex1 = parts[0];
                String vertex2 = parts[1];
                int weight = Integer.parseInt(parts[2]);
                
                Edge edge = new Edge(vertex1, vertex2, weight);
                input_Graph.addEdge(edge);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        input_Graph.updateVertices();

        long start = System.nanoTime();
        Graph output_Graph = input_Graph.PrimMST();
        long finish = System.nanoTime();

        output_Graph.printGraph();
        System.out.println("running time (ns): "+(finish-start));
    }
}

class Edge {
    String vertex1;
    String vertex2;
    int weight;

    public Edge(String vertex1, String vertex2, int weight) {
        this.vertex1 = vertex1;
        this.vertex2 = vertex2;
        this.weight = weight;
    }

    @Override
    public String toString() {
        return "(" + vertex1 + ", " + vertex2 + ", " + weight + ")";
    }
}

class Graph {
    List<Edge> edges;
    Set<String> vertices;
    List<String> vertexList;

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

    public void printGraph() {
        for(Edge edge : edges) {
            System.out.println(edge);
        }
    }

    public void printVerteices() {
        System.out.println("Vertices: " + vertices);
    }

    public Graph PrimMST() {
        Graph tree = new Graph();
        List<String> tree_verteices = new ArrayList<>();
        HashMap<String, Integer> weights = new HashMap<>();

        Random random = new Random();
        String p = vertexList.get(random.nextInt(vertexList.size()));

        for(String vertex : vertexList) weights.put(vertex, Integer.MAX_VALUE);
        weights.put(p, 0);
        tree_verteices.add(p);

        for(Edge edge : edges) {
            if(edge.vertex1.equals(p)) weights.put(edge.vertex2, edge.weight);
            else if(edge.vertex2.equals(p)) weights.put(edge.vertex1, edge.weight);
        }
        
        while(tree_verteices.size() < vertexList.size()) {
            String min_vertex = null;
            int min_weight = Integer.MAX_VALUE;

            for(String vertex : weights.keySet()) {
                int weight = weights.get(vertex);
                
                if(weight != 0 && weight < min_weight) {
                    min_weight = weight;
                    min_vertex = vertex;
                }
            }
            
            for(Edge edge : edges) {
                if(edge.weight == min_weight) {
                    if(edge.vertex1.equals(min_vertex) || edge.vertex2.equals(min_vertex)) {
                        for(String vertex : tree_verteices) {
                            if(edge.vertex1.equals(vertex) || edge.vertex2.equals(vertex)) {
                                tree.addEdge(edge);
                                break;
                            }
                        }
                    }
                }

                if(edge.vertex1.equals(min_vertex)) {
                    if(edge.weight < weights.get(edge.vertex2)) weights.put(edge.vertex2, edge.weight);
                }
                else if(edge.vertex2.equals(min_vertex)) {
                    if(edge.weight < weights.get(edge.vertex1)) weights.put(edge.vertex1, edge.weight);
                }
            }
            
            weights.put(min_vertex, 0);
            tree_verteices.add(min_vertex);
        }

        return tree;
    }
}