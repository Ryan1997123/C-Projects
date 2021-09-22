// Ryan Monaghan
// Lab 4
// CISC-4080-R01
// Professor. Xiaolan Zhang

// all pairs of numbers that add up to K

// all triples (set of three) that add up to K

// all possible subsets of numbers that add up to K: recursive version is
// given, you are asked to solve it iteratively.

// Test the three functions using given test cases.

// Measure the running time of algorithms under various input sizes.

#include <algorithm>
#include <bitset>
#include <iostream>
#include <list>
#include <time.h>
#include <vector>

using namespace std;

const double BILLION = 1E9;

/* Print ALL pairs of number from the given vector that adds up to K
   @param a: a vector int values
   @param K: the sum we want to make
  */
void PairsAddupToK(vector<int> a, int K) {
  // TO DO
  // Store the size of my vector into n
  // to make it easier to read and for the for loops
  int n = a.size();
  // for loop to fix one element
  for (int i = 0; i < n - 1; i++) {
    for (int j = i + 1; j < n; j++) {
      // if value i and j in vector are equal
      // to the value K, then output them
      if (a[i] + a[j] == K) {
        // then we need to output the pairs and report duplicates found
        cout << a[i] << " + " << a[j] << " = " << K << endl;
      }
    }
  }
}

// Modify PairsAddupToK so that it returns all pairs that add up to K.
// return a vector of vectors, e.g., the first vector is the first pair that add
// up to K, the second
// vector is the second pair that add up to K, and so on…. *
// return a vector of pairs from a[left..right] that add up to a[k]
vector<vector<int>> PairsAddupToKRecursive(vector<int> a, int left, int right,
                                           int K) {
  vector<vector<int>> result;

  for (int i = left; i < right; i++) {
    for (int j = i + 1; j < right; j++) {
      // if value i and j in vector are equal
      // to the value K, then output them
      if (a[i] + a[j] == K) {
        // then we need to output the pairs and report duplicates found
        // cout << a[i] << " + " << a[j] << " = " << K;
        // TO DO by you

        // pair[0] = a[i];
        // pair[1] = a[j];
        vector<int> pair = {a[i], a[j]}; // for version 11 of c++
        result.push_back(pair);
      }
    }
  }
  return result;
}

/*Return a vector of pairs of numbers from choosing from  a[left...right] that
  add up to K FIRST ATTEMPT (not as efficient)*/

/* Divide-and-Conquer
 *  Print ALL pairs of number from the given vector a that adds up to K
   @param left, right: specify the starting and ending indices of a
   @param K: the sum we want to make
  */
/*
void RecursivePairsAddupToK(vector<int> a, int left, int right, int K) {
// TODO by you change *professor did in class*
cout << "\n";
// cout << "RecursivePairsAddupToK Function Running\n";
int vectorASize = a.size();
// base case
if (left == right) {
  return;
}

for (int i = 0; i < vectorASize - 2; i++) {
  // to iterate the right side
  for (int j = i + 1; j < vectorASize - 1; j++) {
    if (a[i] == a[j]) {
      cout << a[i] << " " << a[j] << " Duplicates were found";
    }
    for (int i = 0; i < vectorASize - 3; i++) {
      for (j = i + 1; vectorASize - 2; j++) {
        for (K = j + 1; vectorASize - 1; j++) {
          cout << a[i] << a[j] << a[K] << endl;
        }
      }
    }
  }
}
}
*/

/* Print ALL triples of numbers from the given vector a that adds up to K

@param a: the vector of int
@param K: the sum we want to make
*/
void TriplesAddupToK(vector<int> a, int K) {
  // TO DO

  // create some variable n to hold the size of a
  int n = a.size();
  // loop from the start of i  to n -3
  for (int i = 0; i <= n - 3; i++) {
    // loop through when j is the i + 1 and less than n - 2
    for (int j = i + 1; j <= n - 2; j++) {
      // loop through and increment k when k is j+1 and less than n-1
      for (int k = j + 1; k <= n - 1; k++) {
        //
        if (a[i] + a[j] + a[k] == K) {
          cout << a[i] << "," << a[j] << "," << a[k] << endl;
        }
      }
    }
  }
}

void PrintVector(vector<int> L) {
  // loop through vector L from index 0 to the end of vecto L and iterate on.
  // Then, outptu the content values in L or L[i]
  int size;
  for (size_t i = 0; i < L.size(); i++) {
    cout << L[i] << " ";
  }
  cout << " " << endl;
}

/* Print ALL triples of numbers from the given vector a that adds up to K
@param a: the vector of int
@param K: the sum we want to make
*/

// General case
// calculate triples that include a[left] that add up to K
// uses PairsAddUpToK function to find pairs from a[left+1...right] that add
// up to K-a[left] temp holds the value K-a[left]
void TriplesAddupToKRecursive(vector<int> a, int left, int right, int K) {
  // TO DO
  // 0. solve base case

  // 1. calculate triples that include a[left] that add up to K
  // using PairsAddupToK function to find  pairs from a[left+1 .. right]
  // that add up to ....????

  // base case
  if (left == right) {
    // return if left is right
    return;
  }

  if (left + 2 == right) {
    if (a[left] + a[left + 1] + a[right] == K) {
      cout << a[left] << ", " << a[left + 1] << ", " << a[right] << endl;
    }
    return;
  }

  int temp = K - a[left];
  vector<vector<int>> pair =
      PairsAddupToKRecursive(a, left + 1, right, K - a[left]);

  // if a pair exists, we must print out a[left] with every pair that equals to
  // K-a[left]
  // this creates a triple that equals to K
  if (pair.size() != 0) {
    for (int i = 0; i < pair.size(); i++) {
      cout << a[left] << ",";
      PrintVector(pair[i]);
    }
  }

  // 2. Calculate and print out all triples (Excluding a[left]]) add up to K
  // How to do this?
  TriplesAddupToKRecursive(a, left + 1, right, K);
}

// A function that finds all possible subsets of numbers that add up to K
// (Another way to do it maybe)
// Seems to work and uses PairsAddUpToK and TriplesAddUpToK effectively
// Attempt two at TriplesAddUpToKRecursive
void TriplesAddUpToKRecursiveVer2(vector<int> a, int K) {
  // variables

  int i, j, p, sum;
  for (i = 0; i < a.size(); i++) {
    // if the first element in a is K
    if (a[i] == K) {
      // output it
      cout << a[i];
    }
  }
  PairsAddupToK(a, K);
  TriplesAddupToK(a, K);
  for (i = 0; i < a.size(); i++) {
    for (j = i + 3; j < a.size(); j++) {
      // set my sum variable to 0
      sum = 0;
      // for a variable p if its assigned to ith element in a, and is less
      // than or equal to jth element of a then incremnet
      for (p = i; p <= j; p++) {
        // add and assign sum to the pth element of a
        sum = sum + a[p];
      }
      // if the sum is at K
      if (sum == K) {
        // loop through from p to j (or i to j) in a and increment through
        for (p = i; p <= j; p++) {
          // output the pth element of a
          cout << a[p] << ",";
        }
      }
    }
  }
}

/* Compare the running time of three functions for input size n
 @param n: the length of vector/list
 */
void Test_MeasureRunningTime(vector<int> a) {
  struct timespec start, finish;
  double r1, r2, r3, r4;

  clock_gettime(CLOCK_REALTIME, &start);
  PairsAddupToK(a, 100);
  clock_gettime(CLOCK_REALTIME, &finish);
  r1 = (finish.tv_sec - start.tv_sec) +
       (finish.tv_nsec - start.tv_nsec) / BILLION;

  // Todo: add the testing for the DAC PairsAddupToK

  clock_gettime(CLOCK_REALTIME, &start);
  TriplesAddupToK(a, 100);
  clock_gettime(CLOCK_REALTIME, &finish);
  r2 = (finish.tv_sec - start.tv_sec) +
       (finish.tv_nsec - start.tv_nsec) / BILLION;

  cout << "n=" << a.size() << "\t";
  cout << "Pairs"
       << "\t" << r1 << "s\t";
  cout << "Triples"
       << "\t" << r2 << "s\t"
       << "\n";
}

int main() {

  vector<int> a5 = {50, 90, 20, 30, 10};
  vector<int> a10 = {50, 33, 90, 2, 20, 72, 30, 10, 92, 8};
  vector<int> a20 = {50, 33, 11, 79,  90, 2,  20, 72, 30, 10,
                     92, 8,  99, 101, 25, 71, 48, 72, 35, 9};
  vector<int> a30 = {50, 33, 11, 79,  90, 2,  20, 72, 30, 10,
                     92, 8,  99, 101, 25, 71, 48, 72, 35, 9,
                     37, 41, 55, 73,  67, 66, 22, 11, 6,  4};

  /* Measuring and comparing running time   */
  Test_MeasureRunningTime(a5);
  Test_MeasureRunningTime(a10);
  Test_MeasureRunningTime(a20);
  Test_MeasureRunningTime(a30);
}
