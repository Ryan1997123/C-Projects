/*Ryan Monaghan
Lab #2 Due 02/21/2021
Professor. Xiaolan Zhang
Computer Algorithms CIS4080
One extension used for 2/22 at midnight
*
*
*/

// Libraries used
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Functions called before main/driver part of program
vector<int> GenerateVector(int len, int min, int max);
void PrintVector(vector<int> L, int size);
vector<int> ReadVector(int length);
void countSort(vector<int> &v);
unordered_map<int, int> Summarize(const vector<int> &list);
bool ContainDuplicate(vector<int> list);
bool containsDuplicateTwo(vector<int> &nums);
bool IsPermutation(vector<int> list1, vector<int> list2);
void RadixSort(vector<int> &L);
void otherPrintVector(vector<int> &L);

// Main/Driver part of the program
int main() {
  // Vectors used for implementation
  vector<int> myVector;
  vector<int> V2;
  vector<int> B;
  vector<int> C;
  // Strings
  string command, askUser;
  // Integer values
  int len, min, max, userInput;
  // Boolean values
  bool result;

  cout << "Welcome. This program implementes and tests three sorting "
          "algorithms on vector of integers"
       << endl;

  do {
    cout << "*** Warning. Be sure to generate a vector with the generate "
            "command before sorting! ***"
         << endl;
    cout << "Enter a command. (generate, csort, Summarize, CheckDuplicate, "
            "Permutation, Radix, quit): ";
    cin >> command;
    cout << "You entered: " << command << endl;

    if (command == "generate") {
      cout << "Do you want to generate enter the vector manually?: (Y/N) ";
      cin >> askUser;

      // The user input Y or Yes that they want to manually input the size of a
      // vector
      if (askUser == "Y") {
        // Function that allows the user to enter a vector manually
        cout << "Enter the length of the vector: ";
        cin >> len;
        myVector = ReadVector(len);
        cout << "Your generated a vector of size " << myVector.size() << endl;
        // iterate from index 0 to the size of the user generated vector of
        // length 1 to 99 and output the values in the vector
        for (int i = 0; i < myVector.size(); i++) {
          cout << myVector[i] << endl;
        }
        // Second vector generated
        cout << "Enter the length of the second vector";
        cin >> len;
        V2 = ReadVector(len);
        cout << "You generated a vector of size" << V2.size() << endl;
        for (int i = 0; i < V2.size(); i++) {
          cout << V2[i] << endl;
        }

        // User does not want to manually input their own vector
        // Computer will generate its own vecotr of n length from 1 to 99.
        // length entered by the
      } else if (askUser == "N") {
        // prompt the user for the len of the vector
        cout << "I will generate a random vector for you. " << endl;
        cout << "What is the length of the vector: ";
        cin >> len;
        // A random vector is generated and stored in myVector (user length,
        // vector can go from from 1 to 99)
        myVector = GenerateVector(len, 1, 99);
        cout << "Random Vector Generated: " << myVector.size() << endl;
        // Iterate from index 0 to the entire vector of myVector and output the
        // elements
        for (int i = 0; i < myVector.size(); i++)
          cout << myVector[i] << endl;
      }

      // Second vector generated
      cout << "Enter the length of the second vector";
      cin >> len;
      V2 = GenerateVector(len, 1, 99);
      cout << "You generated a vector of size" << V2.size() << endl;
      for (int i = 0; i < V2.size(); i++) {
        cout << V2[i] << endl;
      }

      // Counting sort
    } else if (command == "csort") {
      countSort(myVector);
      // a for loop that iterates through the user created vector myVector using
      // an int val called x and outptus the values of the vector in sorted
      // order
      for (int x : myVector) {
        cout << x << " ";
      }

      cout << endl;

    } else if (command == "Permutation") {
      IsPermutation(myVector, V2);

    } else if (command == "Summarize") {
      Summarize(myVector);
      // Check duplicate command
    } else if (command == "CheckDuplicate") {
      cout << "Running the main contains duplicate function to check\n";
      ContainDuplicate(myVector);
      cout << "\n";

      cout << "Running other version of contain duplicate function to check "
              "again. \n";
      containsDuplicateTwo(myVector);
      // Radix sort command
    } else if (command == "Radix") {
      cout << "Before sorting: \n";
      otherPrintVector(myVector);
      RadixSort(myVector);
      cout << "After sorting: \n";
      otherPrintVector(myVector);
    }
    // if the user enters quit we will exit the do while loop
  } while (command != "quit");
  return 0;
}

// Functions called after main/driver part of program

/* Generate a vector of int of given len, and return the
vector
 @param len: len of vector to be created
 @param min: minimum value
 @param max: maximum value, these specify the range for the
random numbers to be put into the vector
 @return vector of random integer values generated
 @pre: len>=1, min < max
 @post: omitted the obvious
*/
vector<int> GenerateVector(int len, int min, int max) {
  int r;
  vector<int> L;
  // There are L max-min+1 possible ways for [min...max]
  // iterated and increment through the indexes of the user generated vector.
  // started from index 0 to the len the user specified.
  for (int i = 0; i < len; i++) {
    r = rand() % (max - min + 1);

    L.push_back(r); // append L value to the front of the vector, and then next
                    // position, and onward
  }
  // return vector L
  return L;
}

/*Print L vector of int*/
//@pre: This function will read in a vector of type int from index 0 to the end
// of the vector.
//@post: The vector will print out to the screen
/* Print a vector of int, e.g.,
 3 13 9 23 12 29
 */
void PrintVector(vector<int> L, int size) {
  // loop through vector L from index 0 to the end of vecto L and iterate on.
  // Then, outptu the content values in L or L[i]
  for (size_t i = 0; i < L.size(); i++) {
    cout << L[i] << " ";
  }
  cout << " " << endl;
}

// Function very similar to PrintVecotr (without size) used for Radix Sort
// specifically
void otherPrintVector(vector<int> &L) {
  // find the element in L vector
  for (auto elem :
       L) { // once the element is found let the user know and print it unsorted
    cout << elem << "\t";
  }
  // then printed out the sorted version
  cout << "\n";
}

/*
 Read the given number of integers from cin, put them in a
vector return the vector
 @param length: length of vector to be created
 @return vector of integer values entered by the user
 @pre: length>=1
 @post: omitted the obvious */

vector<int> ReadVector(int length) {
  // create a vector called L
  vector<int> L;
  // integer valued use to store user input for vector
  int userVals;
  // ask the user to enter a vector
  cout << "Enter a vector: ";
  // index from 0 to the end of the vector >- length (speicfied by the user
  // beforehand)
  for (int i = 0; i < length; i++) {
    cin >> userVals;       // user enters vector values
    L.push_back(userVals); // push the user entered vector values into the
                           // vector using push_back on the
  }
  // return the vector L
  return L;
}

/*
 Read the given number of integers from cin, put them in a
vector return the vector
 @param len: len of vector to be created
 @return vector of integer values entered by the user
 @pre: len>=1
 @post: omitted the obvious */

/* Use counting sort to sort a vector of positive int into
ascending order.
Note: You will need to find the largest value in the list
yourself.
@param list: the list to be sorted, list.length() gives the
size of the list
@pre: list has been initialized with a certain number of
elements
@post: elements in list has been arranged into ascending order
*/

void countSort(vector<int> &v) {
  // create an empty map to store the frequency of vector elements
  map<int, int> freq;

  // store distinct values in the input vector as key and
  // their respective counts as values
  for (int x : v) {
    // iterate through the map called frequency for x values (keys)
    freq[x]++;
  }

  // traverse the map and overwrite the input vector with sorted elements
  // (the map will iterate based on the sorted order of keys)
  // creates an integer called i and starts at 0
  int i = 0;
  // itereate through the map called frequency using p variable
  for (auto p : freq) {
    // a while loop to check p and the vector. we iterate through vector v
    // starting from i or 0, and assign the value to first stored  value of p
    while (p.second--) {
      v[i++] = p.first;
    }
  }
}

// ATTEMPT 2 for count sort (Caused a segmenetation fault :(!!!))

/*
//Let k be the largest element in L
int k = L.size()-1;
//Let count[0…k] be a new array
//An array called count with 99 elements 0-99
int count[99];



for(int i=0;i<k;i++){
     count[i] = 0;
      // count[v] stores # of occurrences of value v (used i instead, i am not
sure if correct)

}
//for index 0 at j until the entire size of the vector (n-1) we want to iterate
through the vector for(int j=0;j<k;j++){ count[L[j]]++; //we see one more value
of L[j], so increment the counter value
}

 // make count[v] store the # of values <=v
 for(int i=1;i<k;i++){
     count[i] = count[i-1]+1; //used i instead, not sure if correct
 }
// Create a separate vector R of same size as L
vector<int> R;

// place list elements to R[0…n-1], in sorted order (making use of count array)
//for j=n-1 downto 0
for(int j=0;j=R.size()-1;j++){
    //Using the count array to sort j list elements of vector R into ascending
order (0 to n-1) count[R[j]++]; int p = count[L[j]]; //L[J] is the p-th smallest
number in L R[p-1] = L[j]; //L[j] should go to index p-1, as my index is
starting from 0 count[L[j]] = count[L[j]]-1;
    //The above three lines can be combined into one:
    // R[-- count[L[j]] ] = L[j]
}

  ///Copy R to L, one element at a time
 // Or You can also use vector assignment: L = R
 for(int j=0;j<R.size()-1;j++){
     L[j] = R[j];
 }

}
*/

/* return a summary of the given list, i.e.,
For each value that occurred in the list, how many times it
occurs
@param list: data to be summarized
 @pre: list has been initialized with a certain number of
elements
@post: return a summary of the list */
unordered_map<int, int> Summarize(const vector<int> &list) {

  // create an unorder map called data
  unordered_map<int, int> data;
  // loop through a const auto variable called value for the vector called
  // "list"
  for (const auto value : list) {
    // increment using the unordered map called "data" through the values stored
    // in the vector called "list"
    ++data[value];
  }
  // kvp is A variable. Its meant as a short hand for "key value pair", which is
  // what you get when you iterate over a map. auto is used so that i dont have
  // to write out std::pair<int,int>. auto will deduce the type that should
  // stand in its place.
  for (const auto kvp : data) {
    // create a const variable called number and assign it to the first key
    // value pair variable
    const auto number = kvp.first;
    // creates a const count variable and assign it to the seond key value pair
    // variable
    const auto count = kvp.second;
    // output the number (first key value pair) and the count (second key value
    // pair) to check & tell the user how many times it was output
    cout << number << " appeared " << count << " times\n";
  }
  // return the unordered map called data that we created and incremented for
  // the vector called list
  return data;
}

/* Check if a given vector contains duplicate values or not.
 @param list: a list of int
 @post: return true if list contains some duplicate int values
 Return false if all int values in list are distinct
(unique)
 */
bool ContainDuplicate(vector<int> list) {
  // base case (the list is empty)
  if (list.empty()) {
    // tell the user
    cout << "The vector is empty. Generate a vector first " << endl;
    // return false. nothing was found because the list is empty!
    return false;
  }
  // loop through one list until the end, an int value called i is starting at
  // index 0 and itereates to the length of the list (list.size())
  for (int i = 0; i < list.size(); i++)
    // loop through a second time using an int value called j instead of i this
    // time, and iterate through the list until list.size() or end of list
    for (int j = i + 1; j < list.size(); j++)
      // if there are duplicates in indexes i and j (this of j below the list
      // and i above the list indexign through, like in class)
      if (list[i] == list[j]) {
        // then we need to tell the user that they are duplicates + return true
        cout << "There are duplicates" << endl;
        return true;
      }
  // Otherwise there are no duplicates found + we return false to the screen
  cout << "No duplicates found" << endl;
  return false;
}

// Second version of contains duplicate
bool containsDuplicateTwo(vector<int> &nums) {
  // base case. if the size of the vector is less than two, than just return
  // because we cannot compare two elements if only one is in the vector.
  if (nums.size() < 2) {
    // return to the screen
    return 0;
  }
  // call sort on the beginning and end of the vector
  sort(nums.begin(), nums.end());
  auto first = nums.begin();
  auto last = nums.end() - 1;
  // loop through the beginning and end of vector and if the first element is
  // not equal to the last and the first element is not equal to the element
  // directly after the first element then increment the start of the nums
  // vector
  while (first != last && (*first != *(first + 1))) {
    ++first; // moving first each time
  }
  // Check if the beginning and end of vector are equal
  if (first == last) {
    cout << "Duplicates have not been found";
    cout << endl;
    return 0;
  } else {
    // otherwise duplicates have  been found
    cout << "Duplicates have been found";
    cout << endl;
    return 1;
  }
}

/* Given two vectors of integers, check if the two vectors are
permutations of each other, i.e., they contains same values in
same or different order.
 V1={3,4,10,4,2,11} and V2={11,2,10,4,3,4} stores same set
of data points
 @param list1, list2: two vectors of integers
 @pre: list1, list2 have been initialized
 @post: return true if list1 and list2 stores same values (in
same or different order); return false, if not. */
bool IsPermutation(vector<int> list1, vector<int> list2) {

  // assign the size of list1 and list2 to variables size1 and size2s
  int size1 = list1.size();
  int size2 = list2.size();
  // if the size of list1 is not equal to the size of list2 then we have a
  // problem. So, return false
  if (size1 != size2)
    cout << "Error they are not the same size";
  return false;
  // sort the list1 beginning and end
  sort(list1.begin(), list1.end());
  // sort the list2 beginning and end
  sort(list2.begin(), list2.end());
  // loop through size1 (list 1) and iterate from index 0 to the end of size1 or
  // list1.size()
  for (int i = 0; i < size1; i++) { // if the elements in list1 are not the same
                                    // as list2, return false
    if (list1[i] != list2[i])
      cout << "Not a permutation";
    return false;
  }
  // otherwise return true
  cout << "Is a permutation";
  return true;
}

// Radix Sort (Extra credit 20pts)
// Function passes a vector to the radix_sort function. It uses a variable
// called radix to identify the digit in a particular place (unit, ten, hundred
// etc.) in the number. Based on this digit, it sorts elements into buckets.
// Once this sorting is done, the elements are taken out based on the order of
// bucket
//(The elements in the zero-th bucket is printed first, followed by the elements
// in first bucket, second bucket and so on). This is repeated until all the
// digits in the largest element are visited.
void RadixSort(vector<int> &L) {
  // sets radix equal to one
  int radix = 1;
  // Largest element in unsorted array
  int max = *(max_element(L.begin(), L.end()));
  // While the max of the vector is / by radix
  while (max / radix) {
    // Buckets used for sorting. Each bucket representing a digit.
    vector<vector<int>> buckets(10, vector<int>());
    for (const auto &num : L) {
      // take an integer called digit and assign it to number over radix with a
      // remainder of 10. Assign the digit variable of the buckets and call
      // push_back function on the num that was auto generated for the L vector
      int digit = num / radix % 10;
      buckets[digit].push_back(num);
    }
    // size of k set to 0
    size_t k = 0;
    // Take the elements out of buckets into the array
    for (size_t i = 0; i < 10;
         i++) { // index & iterate through the bucket to the end of the bucket
      for (size_t j = 0; j < buckets[i].size(); j++) {
        // Assign k elements of vector L to bucket elements i and j
        L[k] = buckets[i][j];
        // increment through the k elements of vector L which have been assigne
        // to buckets[i][j]
        k++;
      }
    }
    // This code will change the place of digit used for sorting
    radix *= 10;
  }
}
