//                                             Ryan T. Monaghan
//                                            Lab 1 due Feb. 14th at Midnight
//                                           Computer Algorithms (CISC 4080)
//                                           with Prof. Xialoan Zhang Fordham
//                                           University

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>


using namespace std;

// Pre declaration of the void functions & the vector generator of type int
vector<int> GenerateVector(int length, int min, int max);
void SelectionSort(vector<int> &L);
void BubbleSort(vector<int> &L);
void PrintVector(vector<int> L);
void binary_search(vector<int> &L, int value);
void linear_search(vector<int> &L, int value);
void rSelectionSort(vector<int> &L, int left, int right);
void rBubbleSort(vector<int> &L, int left, int right);

// main part of the program with driver code for testing the different sorting
// functions and for generating a vector
int main() {

  // prompt the user to enter a command of type string
  string command;
  // integer values needed to implement each function.
  int len, number, n, left, right, size;
  // initialize a vector of V. Or a vector variable called V
  vector<int> V;

  // Text introduction to the program
  cout << "Welcome! this program implements and tests three sorting algorithms "
          "on vectors of integers "
       << endl;
  // Warning for the user to use the 'generate' command first before continuing
  cout << "DO NOT forget to use command 'generate' before using L sorting "
          "algorithm (like bubble or selection, etc)"
       << endl;
  //@pre: A do while loop that implements user commands while the command is not
  //equal to "quit"
  //@post: the user has presumably generated a vector, and used one of the
  //sorting functions like bubble sort or selection sort.
  do {
    cout << "Enter command (generate, selection, bubble, lin_search, "
            "bin_search, rselection, rbubble quit): ";
    cin >> command;

    cout << "You entered: " << command << endl;

    // if statement to check if user wants to generate a vecotr of some length
    if (command == "generate") {
      // prompt the user for the length of the vector
      cout << "What is the length: ";
      // store that user input in a variable called len of type int
      cin >> len;

      // Vector V is generated randomly with maximum length of 99 by the user
      V = GenerateVector(len, 1, 99);
      // Code to output the size of the vector, and to
      cout << "Random Vector Generated: " << V.size() << endl;

      // print all elements one by one. scan and increment through the indexes
      // of the vector from 0 to the size of the vector
      for (int i = 0; i < V.size(); i++)
        // out the values at each index i
        cout << V[i] << endl;

      //==== Selection Sort  ====
    } else if (command == "selection") {
      // Todo call Print Vector
      PrintVector(V);
      cout << "Calling selection\n";
      // Calling selection sort on vector V
      SelectionSort(V);

      cout << "After selection\n";
      // Todo call PrintVector
      PrintVector(V);

      //==== Bubble Sort ====
    } else if (command == "bubble") {
      // Todo call PrintVector
      PrintVector(V);
      cout << "Calling bubble L\n";
      // Calling bubble sort on vector V
      BubbleSort(V);

      cout << "After bubble L\n";
      // Todo call PrintVector
      PrintVector(V);

      //========Recursive Bubble Sort =======
    } else if (command == "rbubble") {
      cout << "Calling Recursive Bubble Sort" << endl;
      // print the vector out that the user generated
      PrintVector(V);
      // Calling recursive bubble sort on vector V
      rBubbleSort(V, 0, V.size() - 1);
      cout << "After recursive bubble sort" << endl;
      // Calling print vector function to see the updated vector
      PrintVector(V);

      //========Recursive Selection Sort =======
    } else if (command == "rselection") {
      cout << "Calling Recursive Selection Sort" << endl;
      // print the vector out that the user generated
      PrintVector(V);
      // created an integer value called size and store the size of the vector V
      // into that value, so, no we can call size instead of V.size() when
      // implementing below.
      size = V.size();
      // implement the recursive selection sort, and input values. V is the
      // vector, 0 is left, and size is the right side of the vector
      rSelectionSort(V, 0, size);
      cout << "After recursive selection sort: ";
      // print the vector out that the user generated
      PrintVector(V);

      //========Linear  Search =======
    } else if (command == "lin_search") {
      // prompt the user to search for a number they want to find
      cout << "Enter the number to search for: ";
      // take user input and store it in a value called number
      cin >> number;
      // Calling linear search sort on vector V and compare the values in the
      // vector with the user value to find the user value if existent.
      linear_search(V, n);

      //=======Binary Search =====
    } else if (command == "bin_search") {
      // prompt the user to search for the number they are looking for
      cout << "Enter the number to search for: ";
      // take user input and store it in a value called number
      cin >> number;
      // Calling binary search sort on vector V and compare the values in the
      // vector with the user value to find the user value if existent.
      binary_search(V, number);
    }
    // is the command the user input equal to "quit"? if so then we want to
    // exit. Otherwise, do the code while the command is not equal to "quit"
  } while (command != "quit");
}

/* Generate L vector of int of given length, and return the
vector
 @param length: length of vector to be created
 @param min: minimum value
 @param max: maximum value, these specify the range for the
random numbers to be put into the vector
 @return vector of random integer values generated
 @pre: length>=1, min < max
 @post: omitted the obvious
*/

vector<int> GenerateVector(int length, int min, int max) {
  int r;
  vector<int> L;
  // There are L max-min+1 possible ways for [min...max]
  // iterated and increment through the indexes of the user generated vector.
  // started from index 0 to the length the user specified.
  for (int i = 0; i < length; i++) {
    r = rand() % (max - min + 1);

    L.push_back(r); // append L value to the front of the vector, and then next
                    // position, and onward
  }
  // return vector L
  return L;
}

/*Print L vector of int*/
//@pre: This function will read in a vector of type int from index 0 to the end
//of the vector.
//@post: The vector will print out to the screen
void PrintVector(vector<int> L) {
  for (size_t i = 0; i < L.size(); i++) {
    cout << L[i] << " ";
  }
  cout << " " << endl;
}

/* selection L L vector of int into ascending order
 @param list: to list to be sorted, list.length() gives the
size of the list
 @pre: list has been initialized with L certain number of
elements
 @post: elements in list has been arranged into ascending order
*/
void SelectionSort(vector<int> &L) {
  // We select smallest element in L; swap it to the front of the list.
  // We repeat until the whole list is sorted.
  int n = L.size();
  // iteratre throguh the vector L from 0 to n-1 (start to finish indexes) and
  // increment through the vector indexes
  for (int s = 0; s < n - 1; s++) {
    // select the smallest element in L
    int minIndex = s;
    // iterate/incrmenet through the index of vector L from the next element
    // from minIndex up to the entire vector L size
    for (int i = s + 1; i < n; i++)
      // if the start of vector L is less than the minIndex of the vector L,
      // then we want to assign the minIndex to i, or the first index at the
      // start of the vector
      if (L[i] < L[minIndex])
        minIndex = i;

    // if the minIndex is not equal to the s (the starting index), then we will
    // swap the minIndex of vector L with the starting index of vector L
    if (minIndex != s)
      swap(L[minIndex], L[s]);
  }
}

// Similar to B,C, implement bubble L, OR insertion L,
// both iteratively and recursively.
// @param list: to list to be sorted, list.length() gives the size of the list
//@pre: list has been initialized with L certain number of elements
//@post: elements in list has been arranged into ascending order
// The elements will be swapped if needed once the function scans the indexes of
// the vector L from i=0 to L.size()-1
void BubbleSort(vector<int> &L) {
  bool swapping = true;
  while (swapping) {
    swapping = false;
    for (size_t i = 0; i < L.size() - 1; i++) {
      if (L[i] > L[i + 1]) {
        L[i] += L[i + 1];
        L[i + 1] = L[i] - L[i + 1];
        L[i] -= L[i + 1];
        swapping = true;
      }
    }
  }
}

// Recurive Selection Sort
// Recursive selection sort. n is size of L[] and index
// is index of starting element
// @param list: L is the vector of ints, left for the start of the array, right
// for the end of the array.
//@pre: list has been initialized with L certain number of elements, minIndex is
//assigned to the start of the vector L index.
// If the left index is equal to the right index then the function will return.
// We will check the index of the Vector from left+1 to right (the end) and
// increment through the indexes. If the index of k for vecotr L is smaller than
// the minIndex, then the function will assign k as the minIndex.
// If the minIndex is not left (the start of vector L), then I want to swap the
// index at left with minIndex, and then call rSelectionSort.
//@post: elements in list has been arranged into ascending order
// The elements will be swapped if needed once the function scans the indexes of
// the vector L from i=0 to L.size()-1

void rSelectionSort(vector<int> &L, int left, int right) {
  int minIndex = min_element(L.begin(), L.end()) - L.begin();

  // Base case
  if (left == right) {
    return;
  }

  minIndex = left;
  for (int k = left + 1; k < right; k++) {
    if (L.at(k) < L.at(minIndex))
      minIndex = k;
  }
  if (minIndex != left)
    swap(L.at(left), L.at(minIndex));
  rSelectionSort(L, left + 1, right);
}

// Recursive Bubble Sort
// A function to implement bubble sort
// @param list: to list to be sorted, list.length() gives the size of the list
//@pre: list has been initialized with L certain number of elements, a left
//index for the start of the vector, and a right index for the end of the
//vector.
//@post: elements in list has been arranged into ascending order
// The elements will be swapped if needed once the function scans the indexes of
// the vector L from i=0 to L.size()-1
void rBubbleSort(vector<int> &L, int left, int right) {
  // Base case
  if (left == right) {
    return;
  }
  // One pass of bubble sort. After
  // this pass, the largest element
  // is moved (or bubbled) to end.
  for (int i = left; i < right; i++) {
    // if the previous index is greater than the next index; swap the previous
    // index with the next index.
    if (L[i] > L[i + 1]) {
      swap(L[i], L[i + 1]);
    }
  }
  // Largest element is fixed,
  // recur for remaining vector
  rBubbleSort(L, left, right - 1);
}

/* Implement linear search and binary search functions
 * define the function’s header and write its comment.
 * implement the two functions
 * Test them from main
 */

// Binary search functions
//@pre: a vector of type int is declared with an int value to search for a
//number. left, right, and mid of the vector are initalized and searched.
//@post:the number the user is looking for will be printed out to the screen, as
//well as the index for which it was found. Otherwise, if the value is not
// found, then the function will print out not found to the screen.
void binary_search(vector<int> &L, int value) {
  // left assigned to the starting index of the vector
  int left = 0;
  // right assigned to the last index of the vector
  int right = L.size() - 1;
  // mid is used as the midle index of the vector
  int mid;

  // while the left index of the vector is less than/equal to right index
  while (left <= right) {
    // assign the middle of the vector to the left and add the right and divide
    // the vector into half
    mid = left + (right - left) / 2;
    // if the value in the middle index of the vector is equal to the value the
    // user is searching for output that the value was found and at what index.
    if (L[mid] == value) {
      // value the user is searching for is found! yay
      cout << "Found at index " << mid << endl;
      return;
      // if the elements at the middle of the vector are less than the value
      // input, assign the left index of the vector to the next index from the
      // start of the middle
    } else if (L[mid] < value) {
      left = mid + 1;
    } else {
      // else if  the middle of the vecotr is less than the value input, assign
      // the right side of the vector to the end of middle.
      right = mid - 1;
    }
  }
  // Otherwise, output that the value is not found
  cout << "Not found. " << endl;
}

// Linear search functions
//@pre: a vector of type int is declared with an int value to search for a
//number, and "value" is declared of type int and is used for the user to search
//for a value inside of the vector.
//@post:the number the user is looking for will be printed out to the screen, as
//well as the index for which it was found. Otherwise, if the value is not
// found, then the function will print out not found to the screen.
void linear_search(vector<int> &L, int value) {
  // created an integer value called n and assign it to the size of the vector
  // L. So, now "n" holds the value of the vector size.
  int n = L.size();
  // scan the indexes of the vector starting from 0 to n (the size of the
  // vector) and increment.
  for (int i = 0; i < n; i++) {
    // if the vector of index elements i is searched and the value the user
    // wants is found, then we continue
    if (L[i] == value) {
      // great! we found the number at an index, lets print out the index we
      // found the number and return
      cout << "Found at index " << i << endl;
      return;
    }
  }
  // sorry, the function could not find the number at any index the user was
  // looking for
  cout << "Not found." << endl;
}
