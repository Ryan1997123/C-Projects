/* Graph Lab
 * Written by Prof. Zhang, Completed by Kathy Tran
 * Known bugs: none so far
 * Date: April 2021
 * IMPORTANT: Please compile with g++ --std=c++11
 */

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

/* Read a graph from a file, format as below:
9 ## first line givens the total number of nodes, followed by 9 lines
0 1 3   #point 0 is connected with node 1 and node 3
1 0 2
2 1    #point 2 is only connected with 1
....
8  ## this would mean that point 8 is NOT connected with any other point
Post-condition: set NodeNum to the total numbre of nodes, and return adjacent
lists vector */
vector<list<int>> ReadGraph(string fileName, int &NodeNum);

/* Calculate and print nodes degree, directed or undirected, and adjacency
 * matrix */
void GraphInfo(const vector<list<int>> &graph);

/* Print a path */
void PrintPath(list<int> &path);

/* Print the set of visited of nodes  */
void PrintVisited(const unordered_set<int> &visited);

//////// Maze Solving ///////////
/* return a path from start point to finish point in a maze
 * represented by the "adjacent lists" */
list<int> SolveMaze(const vector<list<int>> &maze, int start, int finish);

/* Recursive maze walker and its wrapper */
list<int> SolveMaze_Wrapper(const vector<list<int>> &maze, int start,
                            int finish);
list<int> SolveMaze_Recursive(const vector<list<int>> &maze, int current_point,
                              int finish, unordered_set<int> &visited);

/* Depth First Search Traversal - Iterative and Recursive Methods*/
void DFS(const vector<list<int>> &graph);
void DFSFromSource(const vector<list<int>> &maze, int start, int &time,
                   map<int, string> &color, map<int, int> &pred,
                   map<int, int> &d, map<int, int> &f);
void DFSFromSourceRec(const vector<list<int>> &graph, int current_node,
                      unordered_set<int> &visited);
void DFSFromSourceColorRec(const vector<list<int>> &graph, int current_node,
                           int &time, map<int, string> &color,
                           map<int, int> &pred, map<int, int> &d,
                           map<int, int> &f);

/* Discover if a path exists between two nodes */
bool FindPathByDFS(const vector<list<int>> &graph, int u, int v,
                   list<int> &path);

/* Determine whether an undirected graph is connected using DFS */
bool IsConnected_ByDFS(const vector<list<int>> &graph);

/* Breadth First Traversal */
void BFS(const vector<list<int>> &graph, int src);

int main(int argc, char **argv) {
  int nodeNum;
  string fileName;

  if (argc == 2) {
    fileName = argv[1];
  } else {
    fileName = "final.txt";
  }

  vector<list<int>> graph = ReadGraph(fileName, nodeNum);

  int answer;
  // lab has a lot of components so I broke it down to test each one
  do {
    cout << "[1] GraphInfo" << endl;
    cout << "[2] DFS" << endl;
    cout << "[3] Graph Connectivity" << endl;
    cout << "[4] Find Path Between Two Nodes" << endl;
    cout << "[5] BFS" << endl;
    cout << "Enter the number corresponding to what you want to see: ";
    cin >> answer;
  } while (answer != 1 && answer != 2 && answer != 3 && answer != 4 &&
           answer != 5);

  if (answer == 1) {
    GraphInfo(graph);
  } else if (answer == 2) {
    DFS(graph);
  } else if (answer == 3) {
    // test if an undirected graph is connected or not
    if (IsConnected_ByDFS(graph)) {
      cout << "Graph is connected" << endl;
    } else {
      cout << "Graph is not connected" << endl;
    }
  } else if (answer == 4) {
    // find a path between two nodes, u and v
    list<int> path;
    int u, v;

    // enter number for nodes u and v
    // testMaze has 9 nodes, graph has 8 nodes
    if (fileName == "testMaze.txt") {
      cout << "Enter a number from 0 to 8 for u: ";
      cin >> u;
      cout << "Enter a number from 0 to 8 for v: ";
      cin >> v;
    } else if (fileName == "final.txt") {
      cout << "Enter a number from 0 to 7 for u: ";
      cin >> u;
      cout << "Enter a number from 0 to 7 for v: ";
      cin >> v;
    }

    if (FindPathByDFS(graph, u, v, path)) {
      cout << "There is a path between " << u << " and " << v << ": ";
      // the path list is currently backwards with node v at the beginning
      // instead of node u iterate backwards to get the correct path from node u
      // to node v
      for (list<int>::reverse_iterator it = path.rbegin(); it != path.rend();
           it++) {
        cout << *it << ", ";
      }
      cout << endl;
    } else {
      cout << "There is no path between " << u << " and " << v << "." << endl;
    }
  } else if (answer == 5) {
    BFS(graph, 0);
  }

  return 0;
}

/////////// Graph Helper function ////////
// use an iterator to print all points stored in path
void PrintPath(list<int> &path) {
  list<int>::iterator it; // an iterator to iterate through path...
  cout << "Path is ";

  for (it = path.begin(); it != path.end(); it++) {
    cout << *it << ","; // dereference it to get int element
  }
  cout << endl;
}

// Write a function to print visited, an unordered_set of int
void PrintVisited(unordered_set<int> &visited) {
  unordered_set<int>::iterator it;

  cout << "Visited points:";
  for (it = visited.begin(); it != visited.end(); it++) {
    cout << *it << ",";
  }
  cout << endl;
}

////////////////// Maze Solving: find a path from start to finish in a graph
//////////
//     Iterative solution
/* return a path from start point to finish point in a maze
 * represented by the "adjacent lists" */
list<int> SolveMaze(const vector<list<int>> &maze, int start, int finish) {
  // key data structures used:
  unordered_set<int> visited; // The set of points that have been visited
                              // already

  int current_point; // where we are right now?
  list<int>
      path; // keep track the current path from start point to current point
  // 2, 3, 1 => path 2, 3, 1

  // Initialization: what do we need to do?
  current_point = start;
  path.push_back(start);
  visited.insert(start);

  // Enter a loop
  //  as long as we haven't reached finish point yet, or path is not empty
  //    we check if current_point has any neighboring point that has not been
  //    visited yet, if so, we go to the neighboring point; else, we have
  //    reached a deadend, need to backtrack to previous point
  while (current_point != finish && path.size() != 0) {
    list<int> neighbors = maze[current_point];
    bool found = false; // Have we found a unvisited neighboring node yet?
    int cur_neighbor;
    for (list<int>::iterator it = neighbors.begin(); it != neighbors.end();
         it++) {
      cur_neighbor =
          *it; // dereference an iterator is like dereferencing a point
      if (visited.find(cur_neighbor) == visited.end()) {
        // cur_neighbor is not in visited set
        found = true;
        break;
      }
    }

    if (found) { // we find a neighbor that is not visited yet
      cout << "----------------------------------------------" << endl;
      cout << "Going to point: " << cur_neighbor << endl;
      path.push_back(cur_neighbor);
      visited.insert(cur_neighbor);
      current_point =
          path.back(); // current point is the point at the back of path,
      // i.e., what is push_back just now
      PrintPath(path);
      PrintVisited(visited);
    } else {
      // no way to go from current_point
      // backtrack by going back to previous point
      cout << "----------------------------------------------" << endl;
      cout << "Backtracking..." << endl;
      path.pop_back();             // This remove last element from path
      current_point = path.back(); // current_point is now the previous point
      cout << "Current point is " << current_point << endl;
      PrintPath(path);
      PrintVisited(visited);
    }
  }
  return path;
}

// Recursive solution
// Top level function
list<int> SolveMaze_Wrapper(const vector<list<int>> &maze, int start,
                            int finish) {
  unordered_set<int> visited; // this will be passed to all recursive calls
  return SolveMaze_Recursive(maze, start, finish, visited);
}

// Practice #1: cout statements to SolveMaze show the progress:
//   1) when you go to a new point... "going to point ...", diplay path, display
//   visted 2) when you backtracking  ... "backtracking to previous point...",
//   display path, display visited
/* Recursive function to return a path from current_point to finish point, using
 * only points that are not visited yet. */
list<int> SolveMaze_Recursive(const vector<list<int>> &maze, int current_point,
                              int finish, unordered_set<int> &visited) {
  list<int> subPath, path;
  cout << "Recursive " << current_point << endl;
  if (current_point == finish) {
    path.push_back(current_point);
    return path;
  }

  list<int> neighbors = maze[current_point];
  visited.insert(current_point);
  for (list<int>::iterator it = neighbors.begin(); it != neighbors.end();
       it++) {
    int cur_neighbor = *it;
    if (visited.find(cur_neighbor) == visited.end()) {
      cout << "Going to point " << cur_neighbor << endl;
      subPath = SolveMaze_Recursive(maze, cur_neighbor, finish, visited);
      // if the above function find a path, then add current point to the front
      //   of returned path, and return it.
      if (subPath.size() != 0) {
        path = subPath;
        path.insert(path.begin(), current_point);
        return path;
      }
    }
  }
  // implicit backtracking, if the path is empty, it returns to previous call
  cout << "Backtracking..." << endl;
  return path; // path will be empty at this point
}

// Todo #1: Implement DFSFromSource_Rec recursively based upon notes
/////////////// Recursive DFSFromSource_Rec ///////////////
/* PURPOSE: Perform a DFS traversal using "current_node" as our starting node,
visit all nodes that are reachable from start point
@param graph: vector of vectors holding nodes of the graph and its adjacent
nodes
@param current_node: source node to begin traversal
@param visited: keep track of all visited nodes
@precondition: graph has been initialized
@postcondition: all nodes are visited and are inserted in the visited set
*/
void DFSFromSourceRec(const vector<list<int>> &graph, int current_node,
                      unordered_set<int> &visited) {
  // insert current_node into visited
  visited.insert(current_node);
  // pred used to keep track of predecessor of node when tracing through the
  // function
  int pred;

  // for each neighbor, nbr, of current_node
  // if nbr is not visited yet
  // make recursive call: DFSFromSourceRec (graph, nbr, visited)
  list<int> neighbors = graph[current_node];
  int cur_neighbor;
  for (list<int>::iterator it = neighbors.begin(); it != neighbors.end();
       it++) {
    cur_neighbor = *it; // dereference an iterator is like dereferencing a point
    if (visited.find(cur_neighbor) == visited.end()) {
      cout << "----------------------------------------------" << endl;
      cout << "Going to point: " << cur_neighbor << endl;
      pred = current_node;
      visited.insert(cur_neighbor);
      cout << "Predecessor of point " << cur_neighbor << " is " << pred << endl;
      DFSFromSourceRec(graph, cur_neighbor, visited);
      cout << "Current point is: " << current_node << endl;
    }
  }
  cout << "----------------------------------------------" << endl;
  cout << "Backtracking..." << endl;
}

// Recursive DFSFromSource using extended DFS that involves colors and time
/* PURPOSE: Perform a DFS traversal using "current_node" as our starting node,
visit all nodes that are reachable from start point
@param graph: vector of vectors holding nodes of the graph and its adjacent
nodes
@param current_node: source node to begin traversal
@param time: virtual clock
@param color[]: color of node u
@param pred[]: predecessor of node u
@param d[]: discover time of node u
@param f[]: finish time of node u
@precondition: maze has been initialized
@postcondition: all nodes are visited and are marked by color, discovery/finish
time and by its predecessor node
*/
void DFSFromSourceColorRec(const vector<list<int>> &graph, int current_node,
                           int &time, map<int, string> &color,
                           map<int, int> &pred, map<int, int> &d,
                           map<int, int> &f) {
  // initialize current node's color, discovery time
  color[current_node] = "gray";
  cout << "Node " << current_node << " is " << color[current_node] << endl;
  d[current_node] = ++time;
  cout << "Node " << current_node << " discovery time is " << d[current_node]
       << endl;
  string w = "white";

  list<int> neighbors = graph[current_node];
  int cur_neighbor;

  for (list<int>::iterator it = neighbors.begin(); it != neighbors.end();
       it++) {
    cur_neighbor = *it; // dereference an iterator is like deferencing a point
    if (color[cur_neighbor] == w) {
      cout << "----------------------------------------------" << endl;
      cout << "Going to node " << cur_neighbor << endl;
      pred[cur_neighbor] = current_node;
      cout << "Node " << cur_neighbor << " predecessor is "
           << pred[cur_neighbor] << endl;
      DFSFromSourceColorRec(graph, cur_neighbor, time, color, pred, d, f);
    }
  }

  cout << "----------------------------------------------" << endl;
  cout << "Backtracking..." << endl;
  color[current_node] = "black";
  cout << "Node " << current_node << " is " << color[current_node] << endl;
  f[current_node] = ++time;
  cout << "Node " << current_node << " finish time is " << f[current_node]
       << endl;
  cout << "Current node is " << pred[current_node] << endl;
}

/////// Todo #2: Extend DFS, DFSFromSource To Keep track Color, Pred, d, and f
/* PURPOSE: Perform a DFS traversal using "start" as our starting node, visit
all nodes that are reachable from start point
@param maze: vector of vectors holding nodes of the graph and its adjacent nodes
@param start: source node to begin traversal
@param time: virtual clock
@param color[]: color of node u
@param pred[]: predecessor of node u
@param d[]: discover time of node u
@param f[]: finish time of node u
@precondition: maze has been initialized
@postcondition: all nodes are visited and are marked by color, discovery/finish
time and by its predecessor node
*/
void DFSFromSource(const vector<list<int>> &maze, int start, int &time,
                   map<int, string> &color, map<int, int> &pred,
                   map<int, int> &d, map<int, int> &f) {
  // key data structures used:
  // unordered_set<int> visited; //The set of points that have been visited
  // already https://www.cplusplus.com/reference/unordered_set/unordered_set/
  int current_point; // where we are right now?
  list<int>
      path; // keep track the current path from start point to current point
  // 2, 3, 1 => path 2, 3, 1

  // Initialization: what do we need to do?
  current_point = start;
  path.push_back(start);
  color[start] = "gray";
  pred[start] = 0;
  d[start] = ++time;
  string w = "white";

  // Enter a loop
  //  as long as the path is not empty (i.e., we haven't backtracked from start
  //  point:
  //    we check if current_point has any neighboring point that has not been
  //    visited yet, if so, we go to the neighboring point; else, we have
  //    reached a deadend, need to backtrack to previous point
  while (path.size() != 0) {
    list<int> neighbors = maze[current_point];
    bool found = false; // Have we found a unvisited neighboring node yet?
    int cur_neighbor;
    for (list<int>::iterator it = neighbors.begin(); it != neighbors.end();
         it++) {
      cur_neighbor =
          *it; // dereference an iterator is like dereferencing a point
      if (color[cur_neighbor] == w) {
        found = true;
        break;
      }
    }

    if (found) { // we find a neighbor that is not visited yet
      cout << "----------------------------------------------" << endl;
      cout << "Going to point: " << cur_neighbor << endl;
      color[cur_neighbor] = "gray";
      cout << "Node " << cur_neighbor << " is " << color[cur_neighbor] << endl;
      d[cur_neighbor] = ++time;
      cout << "Node " << cur_neighbor << " discovery time is "
           << d[cur_neighbor] << endl;
      pred[cur_neighbor] = current_point;
      cout << "Node " << cur_neighbor << " predecessor is "
           << pred[cur_neighbor] << endl;
      path.push_back(cur_neighbor);
      current_point =
          path.back(); // current point is the point at the back of path,
      // i.e., what is push_back just now
      PrintPath(path);
    } else {
      // no way to go from current_point
      // backtrack by going back to previous point
      cout << "----------------------------------------------" << endl;
      cout << "Backtracking..." << endl;
      color[current_point] = "black";
      cout << "Node " << current_point << " is " << color[current_point]
           << endl;
      f[current_point] = ++time;
      cout << "Node " << current_point << " finish time is " << f[current_point]
           << endl;
      path.pop_back();             // This remove last element from path
      current_point = path.back(); // current_point is now the previous point
      cout << "Current point is " << current_point << endl;
      PrintPath(path);
    }
  }
}

/* PURPOSE: traverse a graph using Depth First Search
@param graph: vector of vectors holding nodes of the graph and its adjacent
nodes
@precondition: graph has been initialized
@postcondition: every node in the graph has been visited and is colored "black"
*/
void DFS(const vector<list<int>> &graph) {
  map<int, string> color;
  map<int, int> pred;
  map<int, int> d; // discover time
  map<int, int> f; // finish time
  string w = "white";

  int nodeNum = graph.size();

  for (int i = 0; i < nodeNum; i++) {
    color[i] = "white";
    pred[i] = 0;
    d[i] = 0;
    f[i] = 0;
  }

  // virtual clock to keep track of discovery time (when nodes are gray) and
  // finish time (when nodes are black)
  int time = 0;

  int answer;

  do {
    cout << "[1] DFSFromSource" << endl;
    cout << "[2] DFSFromSourceRec" << endl;
    cout << "[3] DFSFromSourceColorRec" << endl;
    cout << "Enter the number corresponding to the function you want to test: ";
    cin >> answer;
  } while (answer != 1 && answer != 2 && answer != 3);

  if (answer == 1) {
    for (int i = 0; i < nodeNum; i++) {
      // for any nodes that are white
      if (color[i] == w) {
        cout << "DFSFromSource: " << i << endl;
        DFSFromSource(graph, i, time, color, pred, d, f);
      }
    }
  } else if (answer == 2) {
    unordered_set<int> visited;
    for (int i = 0; i < nodeNum; i++) {
      if (visited.find(i) == visited.end()) {
        cout << "DFSFromSourceRec: " << i << endl;
        DFSFromSourceRec(graph, i, visited);
      }
    }
  } else if (answer == 3) {
    for (int i = 0; i < nodeNum; i++) {
      // for any nodes that are white
      if (color[i] == w) {
        cout << "DFSFromSourceColorRec: " << i << endl;
        DFSFromSourceColorRec(graph, i, time, color, pred, d, f);
      }
    }
  }
}

/* PURPOSE: read in information about a graph from a file
@param fileName: name of the text file being read in
@param NodeNum: number of nodes in the graph
@precondition: file exists and can be opened
@postcondition: graph is initialized with contents of the file
@return vector of vectors holding nodes of the graph and its adjacent nodes
*/
vector<list<int>> ReadGraph(string fileName, int &NodeNum) {
  string line;
  ifstream graphFile(fileName);
  int u, v;

  if (!graphFile.is_open()) {
    vector<list<int>> emptylists;
    cout << "Failed to open file " << fileName << endl;
    return emptylists;
  }

  graphFile >> NodeNum;
  // cout <<"Total # of nodes:" << NodeNum;
  vector<list<int>> lists(NodeNum);
  getline(graphFile, line);
  // graphFile >> u;
  while (getline(graphFile, line)) {
    stringstream s(line);
    s >> u;
    list<int> adj;

    while (s >> v) {
      adj.push_back(v);
    }

    lists[u] = adj;
  }

  graphFile.close();
  return lists;
}

/* PURPOSE: check if the graph is directed or not
@param graph: vector of vectors holding nodes of the graph and its adjacent
nodes
@precondition: graph has been initialized
@postcondition: if each node and its adjacent nodes have an edge with each
other, the graph is undirected
@return a bool to note whether the graph is undirected or not
*/
bool IsUndirected(const vector<list<int>> &graph) {
  int nodeNum = graph.size();
  // for each edge from node i to node j, see if there is an edge from j to i
  for (int i = 0; i < nodeNum; i++) {
    for (auto it = graph[i].begin(); it != graph[i].end(); ++it) {
      int j = *it;
      // see if there is an edge from j to i
      // if (graph[j].find (i)==graph[j].end()){
      if (find(graph[j].begin(), graph[j].end(), i) == graph[j].end()) {
        cout << "There is edge :" << i << " to " << j << endl;
        cout << "But not the other way\n";
        return false;
      }
    }
  }
  return true;
}

/* PURPOSE: Calculate and print nodes degree, directed or undirected, and
adjacency matrix
@param graph: vector of vectors holding nodes of the graph and its adjacent
nodes
@precondition: graph has been initialized
@postcondition: the graph's nodes degrees, directed or undirected, and adjacency
matrix are printed
*/
void GraphInfo(const vector<list<int>> &graph) {
  int nodeNum = graph.size();

  for (int i = 0; i < nodeNum; i++) {
    cout << "Node" << i << " out-degree: " << graph[i].size() << endl;
  }

  // check if the graph is directed or not
  if (IsUndirected(graph)) {
    cout << "The graph is undirected\n";
  } else {
    cout << "The graph is directed\n";
  }

  cout << "Adjacency matrix:\n";
  // print out the adjacency matrix of the graph
  cout << "---------------------------------\n";
  for (int i = 0; i < nodeNum; i++) {
    for (int j = 0; j < nodeNum; j++) {
      // is there an edge from i to j, is j in node i's adjacency list?
      // if (graph[i].find(j)!=graph[i].end())
      if (find(graph[i].begin(), graph[i].end(), j) == graph[i].end()) {
        cout << "\t" << 0;
      } else {
        cout << "\t" << 1;
      }
    }
    cout << endl;
  }

  cout << "---------------------------------\n";
}

//////////////////////////// DFS Application /////////////////////

/* Todo 3 :  Implement the following function, using extended DFSFromSource */
/* PURPOSE: Return true if there is a path from node u to node v, and set the
path in the reference parameter; return false if there is no path
@param u: first node
@param v: second node
@param path: list of nodes in between node u and node v that create a path
between the two nodes
@precondition: graph has been initialized, and u and v are two nodes that exist
in the graph; path should be empty
@postcondition: if a path exists, the nodes that create it are pushed into the
path vector
@return a bool to note whether a path exists between the two nodes
*/
bool FindPathByDFS(const vector<list<int>> &graph, int u, int v,
                   list<int> &path) {
  map<int, string> color;
  map<int, int> pred;
  map<int, int> d;
  map<int, int> f;
  string w = "white";
  int time = 0;
  int nodeNum = graph.size();

  // initialize node color, predecessor, discovery time and finish time
  for (int i = 0; i < nodeNum; i++) {
    color[i] = "white";
    pred[i] = 0;
    d[i] = 0;
    f[i] = 0;
  }

  // perform DFSFromSource from node u
  DFSFromSource(graph, u, time, color, pred, d, f);
  // path does not exist if node v is white, which means its unvisited
  if (color[v] == w) {
    return false;
  }

  int nodes = v;
  // until node u is found in the path, piece together path using node v's
  // predecessors
  while (find(path.begin(), path.end(), u) == path.end()) {
    path.push_back(nodes);
    nodes = pred[nodes];
  }

  return true;
}

// In-class demo
/* PURPOSE: Checking if an undirected graph is connected or not
@param graph: vector of vectors holding nodes of the graph and its adjacent
nodes
@precondition: graph has been initialized by nodes and is undirected
@postcondition: nodes are visited using DFS and if any are unvisited nodes left,
graph is not connected
@return a bool to note whether the graph is connected or not
*/
bool IsConnected_ByDFS(const vector<list<int>> &graph) {
  // assert that the graph is undirected
  assert(IsUndirected(graph));

  map<int, string> color;
  map<int, int> pred;
  map<int, int> d; // discover time
  map<int, int> f; // finish time
  string w = "white";
  int time = 0;
  int nodeNum = graph.size();

  // initialize node color, predecessor, discovery time and finish time
  for (int i = 0; i < nodeNum; i++) {
    color[i] = "white";
    pred[i] = 0;
    d[i] = 0;
    f[i] = 0;
  }

  // perform DFSFromSource from node 0
  DFSFromSource(graph, 0, time, color, pred, d, f);

  // go through each node, if there is still a node that is white, it means it
  // is unvisited and the graph is not connected
  for (int i = 0; i < nodeNum; i++) {
    if (color[i] == w) {
      return false;
    }
  }

  return true;
}

///////////////////////// BFS //////////////
// Todo 4 : implement BFS function defined as below, test it //
/* PURPOSE: traverse a graph using Breadth First Search
@param graph: vector of vectors holding nodes of the graph and its adjacent
nodes
@param src: source node to begin traversal
@precondition: graph has been initialized and the source node is an existing
node in the graph
@postcondition: every node in the graph has been visited and is colored "black"
*/
void BFS(const vector<list<int>> &graph, int src) {
  map<int, string> color;
  map<int, int> pred;
  map<int, int> d; // discover time
  map<int, int> f; // finish time
  int time = 0;
  string w = "white";
  int nodeNum = graph.size();

  // keep track of order of nodes discovered
  list<int> discovered;

  // initialize node color, predecessor, discovery time and finish time
  for (int i = 0; i < nodeNum; i++) {
    color[i] = "white";
    pred[i] = 0;
    d[i] = 0;
    f[i] = 0;
  }

  // use a "queue" to keep track of which node to visit
  list<int> queue;

  // source node is discovered first, so make it gray and push it into the queue
  int cur_node = src;
  queue.push_back(cur_node);
  color[cur_node] = "gray";
  d[cur_node] = 0;

  // print the current queue
  cout << "Queue: ";
  for (list<int>::iterator it = queue.begin(); it != queue.end(); it++) {
    cout << *it << " ";
  }
  cout << endl;

  while (!queue.empty()) {
    // get the neighbors of the current node
    cur_node = queue.front();
    list<int> neighbors = graph[cur_node];
    // push current node into the discovered node list
    discovered.push_back(cur_node);
    // remove node from the front of the queue
    queue.pop_front();

    int cur_nbr;
    for (list<int>::iterator it = neighbors.begin(); it != neighbors.end();
         it++) {
      cur_nbr = *it;
      // discover any white nodes, color them gray and insert them into the
      // queue
      if (color[cur_nbr] == w) {
        color[cur_nbr] = "gray";
        cout << "Node " << cur_nbr << " color is: " << color[cur_nbr] << endl;
        d[cur_nbr] = ++time;
        cout << "Discovery time for node " << cur_nbr << " is: " << d[cur_nbr]
             << endl;
        pred[cur_nbr] = cur_node;
        cout << "Node " << cur_nbr << " predecessor is: " << pred[cur_nbr]
             << endl;
        queue.push_back(cur_nbr);
        cout << "---------------------------------\n";
      }
    }

    // once all the neighbors of the current node have been colored gray
    // color the current node black and print the finish time and the updated
    // queue
    color[cur_node] = "black";
    cout << "Node " << cur_node << " color is: " << color[cur_node] << endl;
    f[cur_node] = ++time;
    cout << "Node " << cur_node << " finish time is: " << f[cur_node] << endl;

    // print the updated queue
    cout << "Queue: ";
    for (list<int>::iterator it = queue.begin(); it != queue.end(); it++) {
      cout << *it << ", ";
    }
    cout << endl;
    cout << "---------------------------------\n";
  }

  // print out the order of discovered nodes from the source node using BFS
  cout << "BFS from node 0 is: ";
  for (list<int>::iterator it = discovered.begin(); it != discovered.end();
       it++) {
    cout << *it << ", ";
  }
  cout << endl;
}