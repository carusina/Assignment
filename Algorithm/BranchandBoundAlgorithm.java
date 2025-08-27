import java.util.*;

class Node {
    char vertex;
    int distance;
    List<Character> path;

    public Node(char vertex, int distance, List<Character> path) {
        this.vertex = vertex;
        this.distance = distance;
        this.path = path;
    }
}

public class BranchandBoundAlgorithm {
    static Map<Character, Map<Character, Integer> > graph = new HashMap<>();

    static {
        addEdge('A', 'B', 2);
        addEdge('A', 'C', 7);
        addEdge('A', 'D', 3);
        addEdge('A', 'E', 10);
        addEdge('B', 'C', 3);
        addEdge('B', 'D', 5);
        addEdge('B', 'E', 4);
        addEdge('C', 'D', 6);
        addEdge('C', 'E', 1);
        addEdge('D', 'E', 9);
    }

    static void addEdge(char src, char dest, int weight) {
        graph.putIfAbsent(src, new HashMap<>());
        graph.putIfAbsent(dest, new HashMap<>());
        graph.get(src).put(dest, weight);
        graph.get(dest).put(src, weight);
    }

    static int BranchandBound(char start) {
        PriorityQueue<Node> pq = new PriorityQueue<>(Comparator.comparingInt(n -> n.distance));
        List<Character> initialPath = new ArrayList<>();
        initialPath.add(start);
        pq.add(new Node(start, 0, initialPath));

        int best_distance = Integer.MAX_VALUE;
        List<Character> best_path = new ArrayList<>();

        while(!pq.isEmpty()) {
            Node current = pq.poll();

            if(current.path.size() == graph.size() && graph.get(current.vertex).containsKey(start)) {
                int total_distance = current.distance + graph.get(current.vertex).get(start);
                if(total_distance < best_distance) {
                    best_distance = total_distance;
                    best_path = new ArrayList<>(current.path);
                    best_path.add(start);
                }
                continue;
            }

            for(Map.Entry<Character, Integer> neighbor : graph.get(current.vertex).entrySet()) {
                if(!current.path.contains(neighbor.getKey())) {
                    List<Character> newPath = new ArrayList<>(current.path);
                    newPath.add(neighbor.getKey());
                    pq.add(new Node(neighbor.getKey(), current.distance + neighbor.getValue(), newPath));
                }
            }
        }

        System.out.print(best_path+", ");
        return best_distance;
    }

    public static void main(String[] args) {
        char start = 'A';
        int best_distance = BranchandBound(start);
        System.out.println("Distance: "+ best_distance);
    }
}
