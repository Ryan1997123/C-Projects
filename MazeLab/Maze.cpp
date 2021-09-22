/* Lab 5: Graph Lab Final Phase
 * Written by Prof. Zhang, Completed by Ina Patrice Gonzales and Ryan Monaghan
 * Known bugs: none so far
 * Date: April 2021 (with extension - thank you for accomodating my situation
 * :))
 */

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

// I used g++ -std=c++11 filename.cpp to run on mac terminal and then ./a.out

// FUNCTION DECLARATIONS
// READING GRAPH
/* Read a graph from a file, format as below:
9 ## first line givens the total number of nodes, followed by 9 lines
0 1 3   #point 0 is connected with node 1 and node 3
1 0 2
2 1    #point 2 is only connected with 1
....
8  ## this would mean that point 8 is NOT connected with any other point
Post-condition: set NodeNum to the total numbre of nodes, and return
   adjacent lists vector */
vector<list<int>> ReadGraph(string fileName, int &NodeNum);

// PRINTING FUNCTIONS
/* Calculate and print nodes degree, directed or undirected, and adjacency
 * matrix */
void GraphInfo(const vector<list<int>> &graph);
/* Print a path */
void PrintPath(list<int> &path);
/* Print the set of visited of nodes  */
void PrintVisited(const unordered_set<int> &visited);
// Print a map of given template
template <typename I, typename J> void PrintMap(map<I, J> const &k) {
  for (auto const &pair : k) {
    cout << "{" << pair.first << ": " << pair.second << "}\n";
  }
}

// SOLVING MAZE
//////// Maze Solving ///////////
/* return a path from start point to finish point in a maze
 * represented by the "adjacent lists" */
list<int> SolveMaze(const vector<list<int>> &maze, int start, int finish);
/* Recursive maze walker and its wrapper */
list<int> SolveMaze_Wrapper(const vector<list<int>> &maze, int start,
                            int finish);
list<int> SolveMaze_Recursive(const vector<list<int>> &maze, int current_point,
                              int finish, unordered_set<int> &visited);

// DFS FUNCTIONS
void DFSFromSource(const vector<list<int>> &maze, int start, int time,
                   map<int, string> color, map<int, int> d, map<int, int> f,
                   map<int, int> pred);
void DFS(const vector<list<int>> &graph);
void DFSRec(const vector<list<int>> &graph);
bool FindPathByDFS(const vector<list<int>> &graph, int u, int v,
                   list<int> &path);
bool IsConnected_ByDFS(const vector<list<int>> &graph);

// BFS FUNCTION
void BFS(const vector<list<int>> &graph, int src);

// EXTRA CREDIT
bool TopologicalSort(const vector<list<int>> &graph, list<int> &ordering);
list<int> ShortestPath(const vector<list<int>> &graph, int start, int finish);

// MAIN FUNCTION
int main(int argc, char **argv) {
  int nodeNum;
  string fileName;

  if (argc == 2) {
    fileName = argv[1];
  } else
    fileName = "testMaze.txt";

  vector<list<int>> graph = ReadGraph(fileName, nodeNum);

  GraphInfo(graph);

  // Testing out the functions from Phase 1 of Lab 5
  //    cout << "\nDFS Call\n";
  //    DFS (graph);
  //    cout << "\nDFS Recursive Call\n";
  //    DFSRec(graph);

  // Testing out the functions from Final Phase of Lab 5
  // cout << "\nIsConnected_ByDFS\n";
  // IsConnected_ByDFS(graph);

  // cout << "\nFindPathByDFS\n";
  // list<int>path;
  // FindPathByDFS(graph,0,2,path);

  // Testing BFS() Function
  cout << "\nTesting BFS\n";
  int source = 0;
  BFS(graph, source);

  // EXTRA CREDIT
  cout << "\nTesting ShortestPath()\n";
  list<int> shortest = ShortestPath(graph, source, nodeNum);
  PrintPath(shortest);
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
  for (it = visited.begin(); it != visited.end(); it++)
    cout << *it << ",";
  cout << endl;
}

////////////////// Maze Solving: find a path from start to finish in a graph
//////////
//     Iterative solution
/* return a path from start point to finish point in a maze
 * represented by the "adjacent lists" */
list<int> SolveMaze(const vector<list<int>> &maze, int start, int finish) {
  // key data structures used:
  //
  unordered_set<int>
      visited; // The set of points that have been visited already
               // implemented using hashtable
               // https://www.cplusplus.com/reference/unordered_set/unordered_set/

  int current_point; // where we are right now?
  list<int> path;    // keep track the current path from start point to current
                  // point 2, 3, 1 => path 2, 3, 1

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
      cur_neighbor = *it; // dereference an iterator is like
                          // deferencing a point
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
      current_point = path.back(); // current point is the point at the back of
                                   // path, i.e., what is push_back just now

      PrintPath(path);
      PrintVisited(visited);
    } else {
      // no way to go from current_point
      // backtrack by going back to previous point
      cout << "----------------------------------------------" << endl;
      cout << "Backtracking...";
      path.pop_back(); // This remove last element from path

      current_point = path.back(); // current_point is now the previos point

      cout << "current point is " << current_point << endl;

      PrintPath(path);
      PrintVisited(visited);
    }

    // Think about this: what's the loop invariant?
    // what's in the path?
  }

  // post-condition: either path is empty; or current_point==finish, and
  // pathstores
  // the path from start to finish:
  //
  //
  //

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

/* Recursive function to return a path from current_point to finish point,
   using only points that are not visited yet. */
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
  return path; // path will be empty at this point
}

// Todo #1: Implement DFSFromSource_Rec recursively based upon notes
/////////////// Recursive DFSFromSource_Rec ///////////////
// @pre: you have a graph with elements that need to go through DFS
// @param:
/*    graph: the graph that we want to solve
      current_node: u
      visited: visisted nodes
   @post: Perform a DFS traversal using current_node for the beginning, visit
   all nodes taht are reachable from start point
*/
void DFSFromSourceRec(const vector<list<int>> &graph, int current_node,
                      unordered_set<int> &visited) {
  // insert current_node into visisted
  visited.insert(current_node);

  int cur_neighbor;
  list<int> neighbors = graph[current_node];

  // for each neighbor, nbr, of cur_node
  for (list<int>::iterator it = neighbors.begin(); it != neighbors.end();
       it++) {
    cur_neighbor = *it; // dereference an iterator is like
                        // deferencing a point
    if (visited.find(cur_neighbor) == visited.end()) {
      // cur_neighbor is not in visited set
      // add neighbor to visited set
      visited.insert(cur_neighbor);
      // call DFSFromSourceRec()
      DFSFromSourceRec(graph, cur_neighbor, visited);
    }
  }

  PrintVisited(visited);
}

/////// Todo #2: Extend DFS, DFSFromSource To Keep track Color, Pred, d, and f
// DFSFromSource()
/* @pre: Pass by refenrence back to the caller the following maps (mapping node
   u to its color, predcessor, discover time and finish time
   @param:
      maze: the maze in question
      start: node u
      visisted: the visited nodes
      color[u]: color of node u
      pred[u]: color of node u
      d[u]: discover time of node u
      f[u]: finish time of node u */
/* @post: Perform a DFS traversal using "start" as our starting node, visit all
 * nodes taht are reachable from start point */
void DFSFromSource(const vector<list<int>> &maze, int start, int time,
                   map<int, string> color, map<int, int> d, map<int, int> f,
                   map<int, int> pred) {
  // key data structures used:
  //
  // unordered_set<int> visited; //The set of points that have been visited
  // already implemented using hashtable
  // https://www.cplusplus.com/reference/unordered_set/unordered_set/

  int current_point; // where we are right now?
  list<int> path;    // keep track the current path from start point to current
                  // point 2, 3, 1 => path 2, 3, 1
  // Initialization: what do we need to do?
  color[start] = "Gray";
  pred[start] = 0;
  d[start] = ++time;
  current_point = start;
  path.push_back(start);

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
      cur_neighbor = *it; // dereference an iterator is like
                          // deferencing a point
      if (color[cur_neighbor] == "White") {
        // cur_neighbor is not in visited set
        found = true;
        break;
      }
    }

    if (found) { // we find a neighbor that is not visited yet
      cout << "----------------------------------------------" << endl;
      cout << "Going to point: " << cur_neighbor << endl;
      path.push_back(cur_neighbor);
      current_point = path.back(); // current point is the point at the back of
                                   // path, i.e., what is push_back just now
      color[cur_neighbor] = "Gray";
      d[cur_neighbor] = time++;
      pred[cur_neighbor] = current_point;

      PrintPath(path);
      cout << "Color:\n";
      PrintMap(color);
      cout << "Predecessor:\n";
      PrintMap(pred);
      cout << "Discover Time:\n";
      PrintMap(d);
      cout << "Finish Time:\n";
      PrintMap(f);
      cout << "Time: " << time << "\n";

    } else {
      // no way to go from current_point
      // backtrack by going back to previous point
      cout << "----------------------------------------------" << endl;
      cout << "Backtracking...";

      path.pop_back();             // This remove last element from path
      current_point = path.back(); // current_point is now the previos point

      PrintPath(path);

      color[current_point] = "Black";
      f[current_point] = time++;

      cout << "current point is " << current_point << endl;

      cout << "Color:\n";
      PrintMap(color);
      cout << "Predecessor:\n";
      PrintMap(pred);
      cout << "Discover Time:\n";
      PrintMap(d);
      cout << "Finish Time:\n";
      PrintMap(f);
      cout << "Time: " << time << "\n";
    }
  }
}
// DFSRec()
// used for testing the DFSFromSourceRec() function
// @pre: graph is a maze with elements
// @param:
/*    graph: is a vector list of ints
   @post: the graph has gone through DFSRec function and has returned the
   visisted points
*/
void DFSRec(const vector<list<int>> &graph) {
  unordered_set<int> visited;
  int nodeNum = graph.size();

  // DFSFromSourceRec() call
  for (int i = 0; i < nodeNum; i++) {
    if (visited.find(i) == visited.end()) {
      cout << "DFSFromSourceRec: " << i << endl;
      DFSFromSourceRec(graph, i, visited);
    }
  }
}

// used for testing the DFSFromSource() function
// @pre: graph is a maze with elements
// @param:
/*    graph: is a vector list of ints
   @post: the graph has gone through DFSFromSource() function and has returned
   the visisted points
*/
void DFS(const vector<list<int>> &graph) {
  // map information
  map<int, int> pred;
  map<int, string> color;
  map<int, int> d; // discovery time, when u turns gray
  map<int, int> f; // finish time, when u turns black

  int time = 0; // virtual clock to keep track of time

  // initializing all of the maps
  unordered_set<int> visited;
  int nodeNum = graph.size();

  for (int i = 0; i < nodeNum; i++) {
    color[i] = "White";
    pred[i] = 0;
    d[i] = 0;
    f[i] = 0;
  }

  // DFSFromSource() call
  for (int i = 0; i < nodeNum; i++) {
    if (visited.find(i) == visited.end()) {
      cout << "DFSFromSource: " << i << endl;
      DFSFromSource(graph, i, time, color, d, f, pred);
    }
  }
}

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

// check if the graph is directed or not
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

// Calculate and print nodes degree, directed or undirected, and adjacency
// matrix
void GraphInfo(const vector<list<int>> &graph) {
  int nodeNum = graph.size();

  for (int i = 0; i < nodeNum; i++) {
    cout << "Node" << i << " out-degree: " << graph[i].size() << endl;
  }

  // check if the graph is directed or not
  if (IsUndirected(graph))
    cout << "The graph is undirected\n";
  else
    cout << "The graph is directed\n";

  cout << "Adjacency matrix:\n";
  // print out the adjacency matrix of the graph
  cout << "---------------------------------\n";
  for (int i = 0; i < nodeNum; i++) {
    for (int j = 0; j < nodeNum; j++) {
      // is there an edge from i to j, is j in node i's adjacency list?
      // if (graph[i].find(j)!=graph[i].end())
      if (find(graph[i].begin(), graph[i].end(), j) == graph[i].end())
        cout << "\t" << 0;
      else
        cout << "\t" << 1;
    }
    cout << endl;
  }
  cout << "---------------------------------\n";
}

//////////////////////////// DFS Application /////////////////////
/* Todo 3 :  Implement the following function, using extended DFSFromSource */
/* Return true if there is a path from node u to node v, and set the path in the
   reference parameter; return false if there is no path  */
bool FindPathByDFS(const vector<list<int>> &graph, int u, int v,
                   list<int> &path) {
  // Idea:
  // Perform DFSFromSource from node u
  // if v is visited, then there is a path from u to v
  // piece together the path using pred[] map
  map<int, string> color;
  map<int, int> pred;
  map<int, int> d; // the time its discovered
  map<int, int> f; // the time its finished
  string w = "White";
  int time = 0;
  int numNode = graph.size();

  // to initialize the node, predecessor, discovery and finish times
  for (int i = 0; i < numNode; i++) {
    color[i] = "White";
    pred[i] = 0;
    d[i] = 0;
    f[i] = 0;
  }

  DFSFromSource(graph, u, time, color, pred, d, f);

  // case if path is not existent if v node is white (unvisted)
  if (color[v] == w) {
    cout << "\nFindPathByDFS: False\n";
    return false;
  }

  int nodes = v;

  // iterate through until a node u is found, use v's pred
  while (find(path.begin(), path.end(), u) == path.end()) {
    path.push_back(nodes);
    nodes = pred[nodes];
  }
  cout << "\nFindPathByDFS: True\n";
  return true;
}
// In-class demo
/* Checking if an undirected graph is connected or not */
// In-class demo
/* Checking if an undirected graph is connected or not */
bool IsConnected_ByDFS(const vector<list<int>> &graph) {

  assert(IsUndirected(graph));
  map<int, string> color;
  map<int, int> pred;
  map<int, int> d; // the time its discovered
  map<int, int> f; // the time its finished
  string w = "White";
  int time = 0;
  int start = 0;
  int numNodes = graph.size();

  // to initialize the node, predecessor, discovery and finish times
  for (int i = 0; i < numNodes; i++) {

    color[i] = "White";
    pred[i] = 0;
    d[i] = 0;
    f[i] = 0;
  }

  DFSFromSource(graph, start, time, color, d, f, pred);

  // perform DFSFromSource from node 0
  // if all nodes are visisted, then the graph is connected
  // else it's not connected

  // go through each node and check if a node is still white
  for (int i = 0; i < numNodes; i++) {
    if (color[i] == w) {
      cout << "\nIsConnected_ByDFS: False\n";
      return false;
    }
  }
  cout << "\nIsConnected_ByDFS: True\n";
  return true;
}

///////////////////////// BFS //////////////
// Todo 4 : implement BFS function defined as below, test it //
//@pre: graph is a maze with elements
// @param:
/*    graph: is a vector list of ints
@post: graph has gone through BFS*/
void BFS(const vector<list<int>> &graph, int src) {
  // map information
  map<int, int> pred;
  map<int, string> color;
  map<int, int> d; // discovery time, when u turns gray
  map<int, int> f; // finish time, when u turns black

  int time = 0; // virtual clock to keep track of time
  // initializing all of the maps
  list<int> visited;
  int nodeNum = graph.size();

  for (int i = 0; i < nodeNum; i++) {
    color[i] = "White";
    pred[i] = 0;
    d[i] = 0;
    f[i] = 0;
  }

  // intializing
  list<int> q; // queue used for BFS
  int cur_node = src;
  // marking the source node as discovered -- making it gray
  color[cur_node] = "Gray";
  // inserting source node into queue
  q.push_back(cur_node);
  // initializing the other maps
  pred[src] = 0;
  d[cur_node] = 0;

  for (list<int>::iterator i = q.begin(); i != q.end(); i++) {
    cout << *i << endl;
  }

  cout << endl;

  // repeat until q is empty
  while (!q.empty()) {
    cur_node = q.front(); // head of q
    list<int> neighbors = graph[cur_node];
    q.pop_front();
    // for each nbr in adjacency list of cur_node
    list<int>::iterator it;
    for (it = neighbors.begin(); it != neighbors.end();
         it++) { // following edges until a white node is found
      cur_node = *it;
      if (color[cur_node] == "White") {
        pred[cur_node] = cur_node;
        color[cur_node] = "Gray"; // make that node gray
        q.push_back(cur_node);    // insert that node to q
        d[cur_node] = ++time;

        cout << "Cur_node: " << cur_node << "\n";
        cout << "Color:\n";
        PrintMap(color);
        cout << "Predecessor:\n";
        PrintMap(pred);
        cout << "Discover Time:\n";
        PrintMap(d);
        cout << "Finish Time:\n";
        PrintMap(f);
        cout << "Time: " << time << "\n";

        cout << "---------------------------------\n";
      }
    }
    color[cur_node] = "Black";
    f[cur_node] = ++time;

    for (list<int>::iterator i = q.begin(); i != q.end(); i++) {
      cout << *i << endl;
    }

    cout << "Cur_node: " << cur_node << "\n";
    cout << "Color:\n";
    PrintMap(color);
    cout << "Predecessor:\n";
    PrintMap(pred);
    cout << "Discover Time:\n";
    PrintMap(d);
    cout << "Finish Time:\n";
    PrintMap(f);
    cout << "Time: " << time << "\n";

    cout << endl;
    cout << "---------------------------------\n";
  }
}

// Extra credits
// Return true if the graph has a topological order (i.e., graph is a DAG)
//   and set ordering to be the topological order found
// Return false otherwise
bool TopologicalSort(const vector<list<int>> &graph, list<int> &ordering) {
  // Perform (complete) DFS on graph
  //   if detect a cycle, return false
  // Insert nodes into ordering, by descending order of the nodes' finish time,
  // f[.] return true Note: If you want to review heap, you can use make_heap
  // for the last step above
  //      (Warning: very C++ STL heavy!!!: require using pair, make_heap

  // ATTEMPT AT IMPLEMENTING THIS FUNCTION
  //     queue <int> QUEUE;
  //     int cur_node;
  //     int new_node;

  //     for (int i = 0; i < graph.size();i++){
  //         QUEUE.push_back(graph[i]);
  //     }

  //     while (!QUEUE.empty()) {
  //         cur_node = QUEUE.front();
  //         QUEUE.pop();
  //         ordering.push_back(cur_node);
  //         for (int j = 0; j < graph[cur_node].size(); j++){
  //             new_node = graph[cur_node].at(j);
  //             QUEUE.push_back(new_node);
  //         }
  //     }
  return true;
}

// Extra credit: Implement the following function to find a shortest hop path
list<int> ShortestPath(const vector<list<int>> &graph, int src, int finish) {
  list<int> path;
  // 1. Perform a BFS using start as source node
  // map information
  map<int, int> pred;
  map<int, string> color;
  map<int, int> d; // discovery time, when u turns gray
  map<int, int> f; // finish time, when u turns black

  int time = 0; // virtual clock to keep track of time
  // initializing all of the maps
  list<int> visited;
  int nodeNum = graph.size();

  for (int i = 0; i < nodeNum; i++) {
    color[i] = "White";
    pred[i] = 0;
    d[i] = 0;
    f[i] = 0;
  }

  // intializing
  list<int> q; // queue used for BFS
  int cur_node = src;
  // marking the source node as discovered -- making it gray
  color[cur_node] = "Gray";
  // inserting source node into queue
  q.push_back(cur_node);
  // initializing the other maps
  pred[src] = 0;
  d[cur_node] = 0;

  cout << endl;

  // repeat until q is empty
  while (!q.empty()) {
    cur_node = q.front(); // head of q
    list<int> neighbors = graph[cur_node];
    q.pop_front();
    // for each nbr in adjacency list of cur_node
    list<int>::iterator it;
    for (it = neighbors.begin(); it != neighbors.end();
         it++) { // following edges until a white node is found
      cur_node = *it;
      if (color[cur_node] == "White") {
        pred[cur_node] = cur_node;
        color[cur_node] = "Gray"; // make that node gray
        q.push_back(cur_node);    // insert that node to q
        d[cur_node] = ++time;
        path.push_back(cur_node);
      }
    }
    color[cur_node] = "Black";
    f[cur_node] = ++time;

    // 2. if finish is visisted, use pred[] to put together the path from start
    // to finish
    //   otherwise, return empty list (there is no path)
    if (color[finish] == "Black") {
      path.pop_back();
    }
    PrintPath(path);
  }

  return path;
}