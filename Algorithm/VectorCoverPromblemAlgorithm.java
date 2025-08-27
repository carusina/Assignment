import java.util.*;

public class VectorCoverPromblemAlgorithm {
    public static void main(String[] args) {
        String[] edges = {
            "A-B", "A-E", "B-E", "B-F", "B-G", "B-C", "C-D", "C-G", "D-G", "D-H",
            "E-F", "E-J", "E-I", "F=G", "F-J", "G-J", "G-K", "G-L", "G-H", "H-L",
            "I-J", "I-M", "J-M", "J-N", "J-O", "J-K", "K-O", "K-L", "L-O", "L-P",
            "M-N", "N-O", "O-P"
        };

        Map<String, Set<String>> graph = new HashMap<>();
        for(String edge : edges) {
            String[] vertices = edge.split("-");
            if(vertices.length == 2) {
                graph.putIfAbsent(vertices[0], new HashSet<>());
                graph.putIfAbsent(vertices[1], new HashSet<>());
                graph.get(vertices[0]).add(vertices[1]);
                graph.get(vertices[1]).add(vertices[0]);
            }
        }

       Set<String> matching = Approx_Matching_VC(graph);
       for(String edge : matching) {
        System.out.println(edge);
       }
    }

    public static Set<String> Approx_Matching_VC(Map<String, Set<String>> graph) {
        Set<String> matching = new HashSet<>();
        Set<String> matched = new HashSet<>();

        for(String u : graph.keySet()) {
            for(String v : graph.get(u)) {
                if(!matched.contains(u) && !matched.contains(v)) {
                    matching.add(u + "-" + v);
                    matched.add(u);
                    matched.add(v);
                    break;
                }
            }
        }

        return matching;
    }
}