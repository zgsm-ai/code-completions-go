#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <memory>
#include <cmath>
#include <queue>
#include <stack>

using namespace std;

#define MAX_VERTICES 100
#define MAX_EDGES 500

class Edge;

class Vertex {
private:
    int id;
    string label;
    double x, y; // Coordinates for visualization
    bool visited;
    int distance; // For shortest path algorithms
    int previous; // For path reconstruction

public:
    Vertex(int id = 0, const string& label = "", double x = 0.0, double y = 0.0)
        : id(id), label(label), x(x), y(y) {
        visited = false;
        distance = INT_MAX;
        previous = -1;
    }
    
    int getId() const { return id; }
    string getLabel() const { return label; }
    double getX() const { return x; }
    double getY() const { return y; }
    bool isVisited() const { return visited; }
    int getDistance() const { return distance; }
    int getPrevious() const { return previous; }
    
    void setId(int id) { this->id = id; }
    void setLabel(const string& label) { this->label = label; }
    void setX(double x) { this->x = x; }
    void setY(double y) { this->y = y; }
    void setVisited(bool visited) { this->visited = visited; }
    void setDistance(int distance) { this->distance = distance; }
    void setPrevious(int previous) { this->previous = previous; }
    
    void reset() {
        visited = false;
        distance = INT_MAX;
        previous = -1;
    }
    
    void display() const {
        cout << "Vertex ID: " << id << endl;
        cout << "Label: " << label << endl;
        cout << "Position: (" << x << ", " << y << ")" << endl;
        cout << "Visited: " << (visited ? "Yes" : "No") << endl;
        cout << "Distance: " << distance << endl;
        if (previous != -1) {
            cout << "Previous Vertex: " << previous << endl;
        }
    }
};

class Edge {
private:
    int id;
    int sourceId;
    int destinationId;
    double weight;
    bool directed;
    bool visited;
    
public:
    Edge(int id = 0, int sourceId = 0, int destinationId = 0, 
         double weight = 1.0, bool directed = false)
        : id(id), sourceId(sourceId), destinationId(destinationId), 
          weight(weight), directed(directed) {
        visited = false;
    }
    
    int getId() const { return id; }
    int getSourceId() const { return sourceId; }
    int getDestinationId() const { return destinationId; }
    double getWeight() const { return weight; }
    bool isDirected() const { return directed; }
    bool isVisited() const { return visited; }
    
    void setId(int id) { this->id = id; }
    void setSourceId(int sourceId) { this->sourceId = sourceId; }
    void setDestinationId(int destinationId) { this->destinationId = destinationId; }
    void setWeight(double weight) { this->weight = weight; }
    void setDirected(bool directed) { this->directed = directed; }
    void setVisited(bool visited) { this->visited = visited; }
    
    void reset() {
        visited = false;
    }
    
    void display() const {
        cout << "Edge ID: " << id << endl;
        cout << "Source: " << sourceId << " -> Destination: " << destinationId << endl;
        cout << "Weight: " << weight << endl;
        cout << "Directed: " << (directed ? "Yes" : "No") << endl;
        cout << "Visited: " << (visited ? "Yes" : "No") << endl;
    }
};

class Graph {
private:
    vector<Vertex> vertices;
    vector<Edge> edges;
    map<int, vector<int>> adjacencyList;
    bool directed;
    
public:
    Graph(bool directed = false) : directed(directed) {}
    
    bool addVertex(const Vertex& vertex) {
        if (vertices.size() >= MAX_VERTICES) {
            return false;
        }
        
        vertices.push_back(vertex);
        // Initialize adjacency list for this vertex
        adjacencyList[vertex.getId()] = vector<int>();
        
        return true;
    }
    
    bool addEdge(const Edge& edge) {
        if (edges.size() >= MAX_EDGES) {
            return false;
        }
        
        edges.push_back(edge);
        
        // Update adjacency list
        adjacencyList[edge.getSourceId()].push_back(edge.getDestinationId());
        
        // For undirected graphs, add reverse connection
        if (!directed) {
            adjacencyList[edge.getDestinationId()].push_back(edge.getSourceId());
        }
        
        return true;
    }
    
    bool removeVertex(int vertexId) {
        for (auto it = vertices.begin(); it != vertices.end(); ++it) {
            if (it->getId() == vertexId) {
                vertices.erase(it);
                
                // Remove edges connected to this vertex
                edges.erase(remove_if(edges.begin(), edges.end(),
                    [vertexId](const Edge& edge) {
                        return edge.getSourceId() == vertexId || edge.getDestinationId() == vertexId;
                    }), edges.end());
                
                // Rebuild adjacency list
                rebuildAdjacencyList();
                
                return true;
            }
        }
        
        return false;
    }
    
    bool removeEdge(int edgeId) {
        for (auto it = edges.begin(); it != edges.end(); ++it) {
            if (it->getId() == edgeId) {
                edges.erase(it);
                
                // Rebuild adjacency list
                rebuildAdjacencyList();
                
                return true;
            }
        }
        
        return false;
    }
    
    void rebuildAdjacencyList() {
        adjacencyList.clear();
        
        // Rebuild from edges
        for (const auto& vertex : vertices) {
            adjacencyList[vertex.getId()] = vector<int>();
        }
        
        for (const auto& edge : edges) {
            adjacencyList[edge.getSourceId()].push_back(edge.getDestinationId());
            
            // For undirected graphs, add reverse connection
            if (!directed) {
                adjacencyList[edge.getDestinationId()].push_back(edge.getSourceId());
            }
        }
    }
    
    Vertex* findVertex(int vertexId) {
        for (auto& vertex : vertices) {
            if (vertex.getId() == vertexId) {
                return &vertex;
            }
        }
        
        return nullptr;
    }
    
    Edge* findEdge(int edgeId) {
        for (auto& edge : edges) {
            if (edge.getId() == edgeId) {
                return &edge;
            }
        }
        
        return nullptr;
    }
    
    vector<Vertex*> getVertices() {
        vector<Vertex*> result;
        for (auto& vertex : vertices) {
            result.push_back(&vertex);
        }
        return result;
    }
    
    vector<Edge*> getEdges() {
        vector<Edge*> result;
        for (auto& edge : edges) {
            result.push_back(&edge);
        }
        return result;
    }
    
    vector<int> getAdjacentVertices(int vertexId) {
        return adjacencyList[vertexId];
    }
    
    int getVertexCount() const {
        return vertices.size();
    }
    
    int getEdgeCount() const {
        return edges.size();
    }
    
    bool isEmpty() const {
        return vertices.empty();
    }
    
    void reset() {
        for (auto& vertex : vertices) {
            vertex.reset();
        }
        
        for (auto& edge : edges) {
            edge.reset();
        }
    }
    
    // Graph algorithms
    
    // Depth-First Search
    vector<int> dfs(int startVertexId) {
        reset();
        
        vector<int> visitedOrder;
        stack<int> stack;
        
        stack.push(startVertexId);
        
        while (!stack.empty()) {
            int currentId = stack.top();
            stack.pop();
            
            Vertex* current = findVertex(currentId);
            if (!current || current->isVisited()) {
                continue;
            }
            
            current->setVisited(true);
            visitedOrder.push_back(currentId);
            
            // Get adjacent vertices
            vector<int> adjacent = getAdjacentVertices(currentId);
            
            // Add adjacent vertices to stack (in reverse order to visit them in order)
            for (auto it = adjacent.rbegin(); it != adjacent.rend(); ++it) {
                Vertex* adjacentVertex = findVertex(*it);
                if (adjacentVertex && !adjacentVertex->isVisited()) {
                    stack.push(*it);
                }
            }
        }
        
        return visitedOrder;
    }
    
    // Breadth-First Search
    vector<int> bfs(int startVertexId) {
        reset();
        
        vector<int> visitedOrder;
        queue<int> queue;
        
        queue.push(startVertexId);
        
        while (!queue.empty()) {
            int currentId = queue.front();
            queue.pop();
            
            Vertex* current = findVertex(currentId);
            if (!current || current->isVisited()) {
                continue;
            }
            
            current->setVisited(true);
            visitedOrder.push_back(currentId);
            
            // Get adjacent vertices
            vector<int> adjacent = getAdjacentVertices(currentId);
            
            // Add adjacent vertices to queue
            for (int adjacentId : adjacent) {
                Vertex* adjacentVertex = findVertex(adjacentId);
                if (adjacentVertex && !adjacentVertex->isVisited()) {
                    queue.push(adjacentId);
                }
            }
        }
        
        return visitedOrder;
    }
    
    // Dijkstra's Shortest Path Algorithm
    map<int, int> dijkstra(int startVertexId) {
        reset();
        
        // Set distance to start vertex as 0
        Vertex* start = findVertex(startVertexId);
        if (!start) {
            return map<int, int>();
        }
        
        start->setDistance(0);
        
        // Priority queue of vertices to visit (ordered by distance)
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, startVertexId});
        
        while (!pq.empty()) {
            int currentId = pq.top().second;
            pq.pop();
            
            Vertex* current = findVertex(currentId);
            if (!current || current->isVisited()) {
                continue;
            }
            
            current->setVisited(true);
            
            // Get adjacent vertices
            vector<int> adjacent = getAdjacentVertices(currentId);
            
            for (int adjacentId : adjacent) {
                Vertex* adjacentVertex = findVertex(adjacentId);
                if (!adjacentVertex) {
                    continue;
                }
                
                // Find the edge between current and adjacent
                double edgeWeight = 1.0; // Default weight
                for (const auto& edge : edges) {
                    if (edge.getSourceId() == currentId && edge.getDestinationId() == adjacentId) {
                        edgeWeight = edge.getWeight();
                        break;
                    }
                }
                
                int newDistance = current->getDistance() + edgeWeight;
                
                if (newDistance < adjacentVertex->getDistance()) {
                    adjacentVertex->setDistance(newDistance);
                    adjacentVertex->setPrevious(currentId);
                    pq.push({newDistance, adjacentId});
                }
            }
        }
        
        // Build result map
        map<int, int> distances;
        for (const auto& vertex : vertices) {
            if (vertex.getDistance() < INT_MAX) {
                distances[vertex.getId()] = vertex.getDistance();
            }
        }
        
        return distances;
    }
    
    // Get shortest path between two vertices
    vector<int> getShortestPath(int startVertexId, int endVertexId) {
        map<int, int> distances = dijkstra(startVertexId);
        
        if (distances.find(endVertexId) == distances.end()) {
            return vector<int>(); // No path found
        }
        
        // Reconstruct path
        vector<int> path;
        int currentId = endVertexId;
        
        while (currentId != -1) {
            path.insert(path.begin(), currentId);
            
            Vertex* current = findVertex(currentId);
            if (!current) {
                break;
            }
            
            currentId = current->getPrevious();
        }
        
        return path;
    }
    
    // Check if graph is connected
    bool isConnected() {
        if (vertices.empty()) {
            return true;
        }
        
        reset();
        
        // Start DFS from the first vertex
        vector<int> visited = dfs(vertices[0].getId());
        
        return visited.size() == vertices.size();
    }
    
    // Check if graph has a cycle
    bool hasCycle() {
        reset();
        
        for (const auto& vertex : vertices) {
            if (!vertex.isVisited()) {
                if (hasCycleUtil(vertex.getId(), -1)) {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    // Topological Sort (for directed acyclic graphs)
    vector<int> topologicalSort() {
        reset();
        
        vector<int> result;
        stack<int> stack;
        
        // Perform DFS and add vertices to stack when done
        for (const auto& vertex : vertices) {
            if (!vertex.isVisited()) {
                topologicalSortUtil(vertex.getId(), stack);
            }
        }
        
        // Pop from stack to get topological order
        while (!stack.empty()) {
            result.push_back(stack.top());
            stack.pop();
        }
        
        return result;
    }
    
    // Minimum Spanning Tree using Prim's algorithm (for undirected graphs)
    vector<Edge> primMST() {
        if (vertices.empty() || directed) {
            return vector<Edge>();
        }
        
        reset();
        
        vector<Edge> mst;
        
        // Start with the first vertex
        Vertex* start = &vertices[0];
        start->setVisited(true);
        
        while (mst.size() < vertices.size() - 1) {
            Edge* minEdge = nullptr;
            double minWeight = INT_MAX;
            
            // Find the minimum edge connecting visited to unvisited vertices
            for (const auto& vertex : vertices) {
                if (vertex.isVisited()) {
                    for (const auto& edge : edges) {
                        if (edge.getSourceId() == vertex.getId() || 
                            (!directed && edge.getDestinationId() == vertex.getId())) {
                            
                            int unvisitedId = (edge.getSourceId() == vertex.getId()) ? 
                                              edge.getDestinationId() : edge.getSourceId();
                            
                            Vertex* unvisited = findVertex(unvisitedId);
                            if (unvisited && !unvisited->isVisited() && 
                                edge.getWeight() < minWeight) {
                                minWeight = edge.getWeight();
                                minEdge = const_cast<Edge*>(&edge);
                            }
                        }
                    }
                }
            }
            
            if (!minEdge) {
                break; // Graph is not connected
            }
            
            mst.push_back(*minEdge);
            
            // Mark the unvisited vertex as visited
            int unvisitedId = (minEdge->getSourceId() == vertices[0].getId()) ? 
                              minEdge->getDestinationId() : minEdge->getSourceId();
            
            Vertex* unvisited = findVertex(unvisitedId);
            if (unvisited) {
                unvisited->setVisited(true);
            }
        }
        
        return mst;
    }
    
    // Display methods
    void display() const {
        cout << "Graph Information:" << endl;
        cout << "Directed: " << (directed ? "Yes" : "No") << endl;
        cout << "Vertices: " << vertices.size() << endl;
        cout << "Edges: " << edges.size() << endl;
        cout << endl;
        
        cout << "Vertices:" << endl;
        for (const auto& vertex : vertices) {
            cout << "  ID: " << vertex.getId() << ", Label: " << vertex.getLabel();
            cout << ", Position: (" << vertex.getX() << ", " << vertex.getY() << ")" << endl;
        }
        
        cout << endl << "Edges:" << endl;
        for (const auto& edge : edges) {
            cout << "  ID: " << edge.getId() << ", ";
            cout << edge.getSourceId() << " -> " << edge.getDestinationId();
            cout << ", Weight: " << edge.getWeight();
            if (edge.isDirected()) {
                cout << " (Directed)";
            }
            cout << endl;
        }
    }
    
    void displayAdjacencyList() const {
        cout << "Adjacency List:" << endl;
        for (const auto& pair : adjacencyList) {
            cout << "  Vertex " << pair.first << ": ";
            for (size_t i = 0; i < pair.second.size(); i++) {
                cout << pair.second[i];
                if (i < pair.second.size() - 1) {
                    cout << " -> ";
                }
            }
            cout << endl;
        }
    }
    
    void displayPath(const vector<int>& path) const {
        cout << "Path: ";
        for (size_t i = 0; i < path.size(); i++) {
            const Vertex* vertex = nullptr;
            for (const auto& v : vertices) {
                if (v.getId() == path[i]) {
                    vertex = &v;
                    break;
                }
            }
            
            if (vertex) {
                cout << vertex->getLabel() << " (" << vertex->getId() << ")";
            } else {
                cout << path[i];
            }
            
            if (i < path.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
    }
    
    // File operations
    bool saveToFile(const string& filename) const {
        ofstream outFile(filename);
        if (!outFile) {
            return false;
        }
        
        outFile << "GRAPH" << endl;
        outFile << "DIRECTED " << (directed ? "1" : "0") << endl;
        
        outFile << "VERTICES" << endl;
        for (const auto& vertex : vertices) {
            outFile << vertex.getId() << "|" 
                    << vertex.getLabel() << "|" 
                    << vertex.getX() << "|" 
                    << vertex.getY() << endl;
        }
        
        outFile << "EDGES" << endl;
        for (const auto& edge : edges) {
            outFile << edge.getId() << "|" 
                    << edge.getSourceId() << "|" 
                    << edge.getDestinationId() << "|" 
                    << edge.getWeight() << "|" 
                    << (edge.isDirected() ? "1" : "0") << endl;
        }
        
        outFile.close();
        return true;
    }
    
    bool loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            return false;
        }
        
        string line;
        string section = "";
        
        while (getline(inFile, line)) {
            if (line.substr(0, 5) == "GRAPH") {
                section = "GRAPH";
                continue;
            } else if (line.substr(0, 8) == "DIRECTED") {
                stringstream ss(line);
                string token;
                getline(ss, token, ' ');
                getline(ss, token, ' ');
                directed = (token == "1");
                continue;
            } else if (line == "VERTICES") {
                section = "VERTICES";
                continue;
            } else if (line == "EDGES") {
                section = "EDGES";
                continue;
            }
            
            if (line.empty()) continue;
            
            stringstream ss(line);
            string token;
            vector<string> tokens;
            
            while (getline(ss, token, '|')) {
                tokens.push_back(token);
            }
            
            if (section == "VERTICES" && tokens.size() >= 4) {
                int id = stoi(tokens[0]);
                string label = tokens[1];
                double x = stod(tokens[2]);
                double y = stod(tokens[3]);
                
                Vertex vertex(id, label, x, y);
                addVertex(vertex);
            } else if (section == "EDGES" && tokens.size() >= 5) {
                int id = stoi(tokens[0]);
                int sourceId = stoi(tokens[1]);
                int destinationId = stoi(tokens[2]);
                double weight = stod(tokens[3]);
                bool isDirected = (tokens[4] == "1");
                
                Edge edge(id, sourceId, destinationId, weight, isDirected);
                addEdge(edge);
            }
        }
        
        inFile.close();
        return true;
    }
    
private:
    bool hasCycleUtil(int vertexId, int parentId) {
        Vertex* vertex = findVertex(vertexId);
        if (!vertex) {
            return false;
        }
        
        vertex->setVisited(true);
        
        vector<int> adjacent = getAdjacentVertices(vertexId);
        for (int adjacentId : adjacent) {
            Vertex* adjacentVertex = findVertex(adjacentId);
            if (!adjacentVertex) {
                continue;
            }
            
            if (!adjacentVertex->isVisited()) {
                if (hasCycleUtil(adjacentId, vertexId)) {
                    return true;
                }
            } else if (adjacentId != parentId) {
                return true; // Back edge found
            }
        }
        
        return false;
    }
    
    void topologicalSortUtil(int vertexId, stack<int>& stack) {
        Vertex* vertex = findVertex(vertexId);
        if (!vertex) {
            return;
        }
        
        vertex->setVisited(true);
        
        vector<int> adjacent = getAdjacentVertices(vertexId);
        for (int adjacentId : adjacent) {
            Vertex* adjacentVertex = findVertex(adjacentId);
            if (adjacentVertex && !adjacentVertex->isVisited()) {
                topologicalSortUtil(adjacentId, stack);
            }
        }
        
        stack.push(vertexId);
    }
};

class GraphVisualization {
private:
    Graph* graph;
    int width, height;
    
public:
    GraphVisualization(Graph* graph, int width = 800, int height = 600)
        : graph(graph), width(width), height(height) {}
    
    void display() const {
        cout << "Graph Visualization:" << endl;
        cout << "Canvas Size: " << width << "x" << height << endl;
        cout << endl;
        
        // Simple text-based visualization
        // Create a 2D grid
        const int gridWidth = width / 50; // Each cell is 50 pixels wide
        const int gridHeight = height / 50; // Each cell is 50 pixels high
        
        char grid[gridHeight][gridWidth];
        
        // Initialize grid
        for (int y = 0; y < gridHeight; y++) {
            for (int x = 0; x < gridWidth; x++) {
                grid[y][x] = ' ';
            }
        }
        
        // Place vertices on grid
        vector<Vertex*> vertices = graph->getVertices();
        for (Vertex* vertex : vertices) {
            int x = (int)(vertex->getX() / 50);
            int y = (int)(vertex->getY() / 50);
            
            if (x >= 0 && x < gridWidth && y >= 0 && y < gridHeight) {
                grid[y][x] = 'V'; // V for vertex
            }
        }
        
        // Draw edges
        vector<Edge*> edges = graph->getEdges();
        for (Edge* edge : edges) {
            Vertex* source = graph->findVertex(edge->getSourceId());
            Vertex* destination = graph->findVertex(edge->getDestinationId());
            
            if (source && destination) {
                int x1 = (int)(source->getX() / 50);
                int y1 = (int)(source->getY() / 50);
                int x2 = (int)(destination->getX() / 50);
                int y2 = (int)(destination->getY() / 50);
                
                // Simple line drawing (Bresenham's algorithm)
                drawLine(grid, gridWidth, gridHeight, x1, y1, x2, y2);
            }
        }
        
        // Print grid
        for (int y = 0; y < gridHeight; y++) {
            for (int x = 0; x < gridWidth; x++) {
                cout << grid[y][x];
            }
            cout << endl;
        }
        
        // Print legend
        cout << endl << "Legend:" << endl;
        cout << "V = Vertex" << endl;
        cout << "-, |, \\, / = Edge" << endl;
        
        // Print vertex list with coordinates
        cout << endl << "Vertex List:" << endl;
        for (Vertex* vertex : vertices) {
            cout << "  ID: " << vertex->getId() << ", Label: " << vertex->getLabel();
            cout << ", Position: (" << vertex->getX() << ", " << vertex->getY() << ")" << endl;
        }
    }
    
    void displayWithPath(const vector<int>& path) const {
        cout << "Graph Visualization with Path:" << endl;
        cout << "Canvas Size: " << width << "x" << height << endl;
        cout << endl;
        
        // Create a 2D grid
        const int gridWidth = width / 50;
        const int gridHeight = height / 50;
        
        char grid[gridHeight][gridWidth];
        
        // Initialize grid
        for (int y = 0; y < gridHeight; y++) {
            for (int x = 0; x < gridWidth; x++) {
                grid[y][x] = ' ';
            }
        }
        
        // Place all vertices on grid
        vector<Vertex*> vertices = graph->getVertices();
        for (Vertex* vertex : vertices) {
            int x = (int)(vertex->getX() / 50);
            int y = (int)(vertex->getY() / 50);
            
            if (x >= 0 && x < gridWidth && y >= 0 && y < gridHeight) {
                // Check if this vertex is in the path
                bool inPath = false;
                for (int pathVertexId : path) {
                    if (vertex->getId() == pathVertexId) {
                        inPath = true;
                        break;
                    }
                }
                
                grid[y][x] = inPath ? '*' : 'V'; // * for vertices in path, V for others
            }
        }
        
        // Draw all edges
        vector<Edge*> edges = graph->getEdges();
        for (Edge* edge : edges) {
            Vertex* source = graph->findVertex(edge->getSourceId());
            Vertex* destination = graph->findVertex(edge->getDestinationId());
            
            if (source && destination) {
                int x1 = (int)(source->getX() / 50);
                int y1 = (int)(source->getY() / 50);
                int x2 = (int)(destination->getX() / 50);
                int y2 = (int)(destination->getY() / 50);
                
                // Check if this edge is in the path
                bool inPath = false;
                for (size_t i = 0; i < path.size() - 1; i++) {
                    if ((path[i] == edge->getSourceId() && path[i+1] == edge->getDestinationId()) ||
                        (!edge->isDirected() && path[i] == edge->getDestinationId() && path[i+1] == edge->getSourceId())) {
                        inPath = true;
                        break;
                    }
                }
                
                // Draw the edge with different character based on whether it's in the path
                drawLineWithChar(grid, gridWidth, gridHeight, x1, y1, x2, y2, inPath ? '#' : '-');
            }
        }
        
        // Print grid
        for (int y = 0; y < gridHeight; y++) {
            for (int x = 0; x < gridWidth; x++) {
                cout << grid[y][x];
            }
            cout << endl;
        }
        
        // Print legend
        cout << endl << "Legend:" << endl;
        cout << "* = Vertex in path" << endl;
        cout << "V = Vertex not in path" << endl;
        cout << "# = Edge in path" << endl;
        cout << "-, |, \\, / = Edge not in path" << endl;
    }
    
private:
    void drawLine(char grid[][800/50], int gridWidth, int gridHeight, 
                int x1, int y1, int x2, int y2) const {
        // Simple line drawing using Bresenham's algorithm
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        
        int err = dx - dy;
        
        int x = x1;
        int y = y1;
        
        while (true) {
            // Make sure we're within bounds
            if (x >= 0 && x < gridWidth && y >= 0 && y < gridHeight) {
                // Don't overwrite vertices
                if (grid[y][x] == ' ') {
                    grid[y][x] = '-';
                }
            }
            
            if (x == x2 && y == y2) {
                break;
            }
            
            int e2 = 2 * err;
            
            if (e2 > -dy) {
                err -= dy;
                x += sx;
            }
            
            if (e2 < dx) {
                err += dx;
                y += sy;
            }
        }
    }
    
    void drawLineWithChar(char grid[][800/50], int gridWidth, int gridHeight, 
                       int x1, int y1, int x2, int y2, char c) const {
        // Simple line drawing using Bresenham's algorithm
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        
        int err = dx - dy;
        
        int x = x1;
        int y = y1;
        
        while (true) {
            // Make sure we're within bounds
            if (x >= 0 && x < gridWidth && y >= 0 && y < gridHeight) {
                // Don't overwrite vertices
                if (grid[y][x] == ' ') {
                    grid[y][x] = c;
                }
            }
            
            if (x == x2 && y == y2) {
                break;
            }
            
            int e2 = 2 * err;
            
            if (e2 > -dy) {
                err -= dy;
                x += sx;
            }
            
            if (e2 < dx) {
                err += dx;
                y += sy;
            }
        }
    }
};

// Function prototypes
void displayMenu();
void handleMenuChoice(Graph& graph, GraphVisualization& visualization, int choice);
void addVertexMenu(Graph& graph);
void addEdgeMenu(Graph& graph);
void removeVertexMenu(Graph& graph);
void removeEdgeMenu(Graph& graph);
void dfsMenu(Graph& graph);
void bfsMenu(Graph& graph);
void dijkstraMenu(Graph& graph);
void shortestPathMenu(Graph& graph);
void connectivityMenu(Graph& graph);
void cycleMenu(Graph& graph);
void topologicalSortMenu(Graph& graph);
void primMSTMenu(Graph& graph);
void displayMenu(Graph& graph);
void displayAdjacencyListMenu(Graph& graph);
void visualizationMenu(Graph& graph, GraphVisualization& visualization);
void saveToFileMenu(Graph& graph);
void loadFromFileMenu(Graph& graph);
void generateSampleDataMenu(Graph& graph);

int main() {
    Graph graph;
    GraphVisualization visualization(&graph);
    
    cout << "Graph Management System" << endl;
    cout << "Initializing system..." << endl;
    
    // Try to load data from file, or generate sample data if file doesn't exist
    if (!graph.loadFromFile("graph_data.txt")) {
        cout << "No existing data found. Generating sample data..." << endl;
        generateSampleDataMenu(graph);
        graph.saveToFile("graph_data.txt");
        cout << "Sample data generated and saved." << endl;
    } else {
        cout << "Existing data loaded successfully." << endl;
    }
    
    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        
        // Clear input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        handleMenuChoice(graph, visualization, choice);
        
        cout << endl;
        system("pause");
        system("cls");
    } while (choice != 0);
    
    // Save data before exiting
    graph.saveToFile("graph_data.txt");
    cout << "Data saved. Exiting program." << endl;
    
    return 0;
}

void displayMenu() {
    cout << "Graph Management System" << endl;
    cout << "1. Add Vertex" << endl;
    cout << "2. Add Edge" << endl;
    cout << "3. Remove Vertex" << endl;
    cout << "4. Remove Edge" << endl;
    cout << "5. Depth-First Search" << endl;
    cout << "6. Breadth-First Search" << endl;
    cout << "7. Dijkstra's Algorithm" << endl;
    cout << "8. Find Shortest Path" << endl;
    cout << "9. Check Connectivity" << endl;
    cout << "10. Check for Cycles" << endl;
    cout << "11. Topological Sort" << endl;
    cout << "12. Prim's Minimum Spanning Tree" << endl;
    cout << "13. Display Graph" << endl;
    cout << "14. Display Adjacency List" << endl;
    cout << "15. Visualize Graph" << endl;
    cout << "16. Visualize with Shortest Path" << endl;
    cout << "17. Save Data to File" << endl;
    cout << "18. Load Data from File" << endl;
    cout << "19. Generate Sample Data" << endl;
    cout << "0. Exit" << endl;
}

void handleMenuChoice(Graph& graph, GraphVisualization& visualization, int choice) {
    switch (choice) {
        case 1:
            addVertexMenu(graph);
            break;
        case 2:
            addEdgeMenu(graph);
            break;
        case 3:
            removeVertexMenu(graph);
            break;
        case 4:
            removeEdgeMenu(graph);
            break;
        case 5:
            dfsMenu(graph);
            break;
        case 6:
            bfsMenu(graph);
            break;
        case 7:
            dijkstraMenu(graph);
            break;
        case 8:
            shortestPathMenu(graph);
            break;
        case 9:
            connectivityMenu(graph);
            break;
        case 10:
            cycleMenu(graph);
            break;
        case 11:
            topologicalSortMenu(graph);
            break;
        case 12:
            primMSTMenu(graph);
            break;
        case 13:
            displayMenu(graph);
            break;
        case 14:
            displayAdjacencyListMenu(graph);
            break;
        case 15:
            visualizationMenu(graph, visualization);
            break;
        case 16: {
            int startId, endId;
            cout << "Enter Start Vertex ID: ";
            cin >> startId;
            cout << "Enter End Vertex ID: ";
            cin >> endId;
            
            vector<int> path = graph.getShortestPath(startId, endId);
            if (!path.empty()) {
                visualization.displayWithPath(path);
                graph.displayPath(path);
            } else {
                cout << "No path found between the specified vertices." << endl;
            }
            break;
        }
        case 17:
            saveToFileMenu(graph);
            break;
        case 18:
            loadFromFileMenu(graph);
            break;
        case 19:
            generateSampleDataMenu(graph);
            break;
        case 0:
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }
}

void addVertexMenu(Graph& graph) {
    int id;
    string label;
    double x, y;
    
    cout << "Enter Vertex ID: ";
    cin >> id;
    cin.ignore();
    
    cout << "Enter Vertex Label: ";
    getline(cin, label);
    
    cout << "Enter X Coordinate: ";
    cin >> x;
    
    cout << "Enter Y Coordinate: ";
    cin >> y;
    
    Vertex vertex(id, label, x, y);
    
    if (graph.addVertex(vertex)) {
        cout << "Vertex added successfully." << endl;
    } else {
        cout << "Failed to add vertex. The vertex ID may already exist or graph is full." << endl;
    }
}

void addEdgeMenu(Graph& graph) {
    int id, sourceId, destinationId;
    double weight;
    char directed;
    
    cout << "Enter Edge ID: ";
    cin >> id;
    
    cout << "Enter Source Vertex ID: ";
    cin >> sourceId;
    
    cout << "Enter Destination Vertex ID: ";
    cin >> destinationId;
    
    cout << "Enter Weight: ";
    cin >> weight;
    
    cout << "Is this a directed edge? (y/n): ";
    cin >> directed;
    
    Edge edge(id, sourceId, destinationId, weight, (directed == 'y' || directed == 'Y'));
    
    if (graph.addEdge(edge)) {
        cout << "Edge added successfully." << endl;
    } else {
        cout << "Failed to add edge. The edge ID may already exist, vertices may not exist, or graph is full." << endl;
    }
}

void removeVertexMenu(Graph& graph) {
    int vertexId;
    
    cout << "Enter Vertex ID to remove: ";
    cin >> vertexId;
    
    if (graph.removeVertex(vertexId)) {
        cout << "Vertex removed successfully." << endl;
    } else {
        cout << "Failed to remove vertex. The vertex may not exist." << endl;
    }
}

void removeEdgeMenu(Graph& graph) {
    int edgeId;
    
    cout << "Enter Edge ID to remove: ";
    cin >> edgeId;
    
    if (graph.removeEdge(edgeId)) {
        cout << "Edge removed successfully." << endl;
    } else {
        cout << "Failed to remove edge. The edge may not exist." << endl;
    }
}

void dfsMenu(Graph& graph) {
    int startVertexId;
    
    cout << "Enter Start Vertex ID: ";
    cin >> startVertexId;
    
    vector<int> visitedOrder = graph.dfs(startVertexId);
    
    if (!visitedOrder.empty()) {
        cout << "DFS traversal order: ";
        for (size_t i = 0; i < visitedOrder.size(); i++) {
            cout << visitedOrder[i];
            if (i < visitedOrder.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
    } else {
        cout << "Start vertex not found or graph is empty." << endl;
    }
}

void bfsMenu(Graph& graph) {
    int startVertexId;
    
    cout << "Enter Start Vertex ID: ";
    cin >> startVertexId;
    
    vector<int> visitedOrder = graph.bfs(startVertexId);
    
    if (!visitedOrder.empty()) {
        cout << "BFS traversal order: ";
        for (size_t i = 0; i < visitedOrder.size(); i++) {
            cout << visitedOrder[i];
            if (i < visitedOrder.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
    } else {
        cout << "Start vertex not found or graph is empty." << endl;
    }
}

void dijkstraMenu(Graph& graph) {
    int startVertexId;
    
    cout << "Enter Start Vertex ID: ";
    cin >> startVertexId;
    
    map<int, int> distances = graph.dijkstra(startVertexId);
    
    if (!distances.empty()) {
        cout << "Shortest distances from vertex " << startVertexId << ":" << endl;
        for (const auto& pair : distances) {
            cout << "  Vertex " << pair.first << ": " << pair.second << endl;
        }
    } else {
        cout << "Start vertex not found or graph is empty." << endl;
    }
}

void shortestPathMenu(Graph& graph) {
    int startVertexId, endVertexId;
    
    cout << "Enter Start Vertex ID: ";
    cin >> startVertexId;
    
    cout << "Enter End Vertex ID: ";
    cin >> endVertexId;
    
    vector<int> path = graph.getShortestPath(startVertexId, endVertexId);
    
    if (!path.empty()) {
        cout << "Shortest path from vertex " << startVertexId 
             << " to vertex " << endVertexId << ":" << endl;
        graph.displayPath(path);
        
        // Calculate path length
        map<int, int> distances = graph.dijkstra(startVertexId);
        if (distances.find(endVertexId) != distances.end()) {
            cout << "Path length: " << distances[endVertexId] << endl;
        }
    } else {
        cout << "No path found between the specified vertices." << endl;
    }
}

void connectivityMenu(Graph& graph) {
    if (graph.isConnected()) {
        cout << "The graph is connected." << endl;
    } else {
        cout << "The graph is not connected." << endl;
    }
}

void cycleMenu(Graph& graph) {
    if (graph.hasCycle()) {
        cout << "The graph contains at least one cycle." << endl;
    } else {
        cout << "The graph does not contain any cycles." << endl;
    }
}

void topologicalSortMenu(Graph& graph) {
    vector<int> sortedOrder = graph.topologicalSort();
    
    if (!sortedOrder.empty()) {
        cout << "Topological sort order: ";
        for (size_t i = 0; i < sortedOrder.size(); i++) {
            cout << sortedOrder[i];
            if (i < sortedOrder.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
    } else {
        cout << "Cannot perform topological sort. The graph may contain a cycle." << endl;
    }
}

void primMSTMenu(Graph& graph) {
    vector<Edge> mst = graph.primMST();
    
    if (!mst.empty()) {
        cout << "Minimum Spanning Tree (Prim's algorithm):" << endl;
        double totalWeight = 0.0;
        
        for (const auto& edge : mst) {
            cout << "  " << edge.getSourceId() << " -> " << edge.getDestinationId() 
                 << " (Weight: " << edge.getWeight() << ")" << endl;
            totalWeight += edge.getWeight();
        }
        
        cout << "Total weight of MST: " << totalWeight << endl;
    } else {
        cout << "Cannot create MST. The graph may be empty, directed, or not connected." << endl;
    }
}

void displayMenu(Graph& graph) {
    graph.display();
}

void displayAdjacencyListMenu(Graph& graph) {
    graph.displayAdjacencyList();
}

void visualizationMenu(Graph& graph, GraphVisualization& visualization) {
    visualization.display();
}

void saveToFileMenu(Graph& graph) {
    string filename;
    
    cout << "Enter filename to save: ";
    getline(cin, filename);
    
    if (filename.empty()) {
        filename = "graph_data.txt";
    }
    
    if (graph.saveToFile(filename)) {
        cout << "Data saved successfully to " << filename << endl;
    } else {
        cout << "Failed to save data." << endl;
    }
}

void loadFromFileMenu(Graph& graph) {
    string filename;
    
    cout << "Enter filename to load: ";
    getline(cin, filename);
    
    if (filename.empty()) {
        filename = "graph_data.txt";
    }
    
    if (graph.loadFromFile(filename)) {
        cout << "Data loaded successfully from " << filename << endl;
    } else {
        cout << "Failed to load data." << endl;
    }
}

void generateSampleDataMenu(Graph& graph) {
    // Create a simple graph
    // Clear existing data
    graph = Graph();
    
    // Add vertices
    Vertex v1(1, "A", 100, 100);
    Vertex v2(2, "B", 300, 100);
    Vertex v3(3, "C", 500, 100);
    Vertex v4(4, "D", 200, 300);
    Vertex v5(5, "E", 400, 300);
    Vertex v6(6, "F", 300, 500);
    
    graph.addVertex(v1);
    graph.addVertex(v2);
    graph.addVertex(v3);
    graph.addVertex(v4);
    graph.addVertex(v5);
    graph.addVertex(v6);
    
    // Add edges (undirected)
    Edge e1(1, 1, 2, 2.0, false);
    Edge e2(2, 2, 3, 3.0, false);
    Edge e3(3, 1, 4, 1.0, false);
    Edge e4(4, 2, 5, 4.0, false);
    Edge e5(5, 3, 5, 2.0, false);
    Edge e6(6, 4, 5, 1.0, false);
    Edge e7(7, 4, 6, 3.0, false);
    Edge e8(8, 5, 6, 1.0, false);
    
    graph.addEdge(e1);
    graph.addEdge(e2);
    graph.addEdge(e3);
    graph.addEdge(e4);
    graph.addEdge(e5);
    graph.addEdge(e6);
    graph.addEdge(e7);
    graph.addEdge(e8);
    
    cout << "Sample graph data generated:" << endl;
    cout << "Vertices: 6" << endl;
    cout << "Edges: 8" << endl;
    cout << "Graph type: Undirected" << endl;
    
    // Display the graph
    graph.display();
}
<｜fim▁hole｜>