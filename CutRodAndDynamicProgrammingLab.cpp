// Ryan Monaghan
// In class practice
// Submitted on Blackboard for today's class

/* Practice cut rod problem & dynamic programming
 * By: X. Zhang
 * 4/27/2021
 * In-class Exercise: finish and submit by the end of today
 * 1). Draw recursion tree for CutRod(n=3), with the help of the code...
 *     Mark return value and cutting for each function calls
 * 2). In-class: modify CutRodMemoization to return the cutting
 * 3). Extend CutRodBottomUp to output the cutting that achieves the max profit
 *   in the end, before returning the max profit achieved for n
 */
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int CutRodHelper(vector<int> p, int n, int r[], int c[]);

// Recursive solution: declare the CutRod function
/* return the max. revenue
 * @param: p is the price lookup table
 * @param: n is the length of rod given
 * @cutting: is set to the cutting that achieved the max.
 *     revenue upon the function's return
 */
int CutRod(vector<int> p, int n, vector<int> &cutting) {
  char input;
  cout << "CutRod(n=" << n << ") called..." << endl;
  cout << "Press any key to continue:";
  cin >> input;

  if (n == 0) // base case
  {
    cutting.clear(); // just to make sure
                     // cutting is empty.
    // help with tracing
    cout << "CutRod(n=0) returning 0, cutting is empty" << endl;
    cout << "Press any key and then Ret to continue:";
    cin >> input;
    return 0;
  } else if (n == 1) {
    cutting.clear();
    cutting.push_back(1);
    cout << "CutRod(n=" << 1 << ") return " << p[1] << ", cutting is ";
    for (auto c : cutting)
      cout << c << ",";
    cout << endl;
    cout << "Press any key and then Ret to continue:";
    cin >> input;
    return p[1];
  }

  // general case
  int curMax; // current max. revenue
  curMax = 0;

  // explore all possible ways to make first
  // cut (i=1...n), and recursive evaluate smaller prob
  // Cut_Rod (n-i)
  int maxRodLength = p.size() - 1;
  maxRodLength = min(n, maxRodLength);

  int bestFirstCut;

  for (int i = 1; i <= maxRodLength;
       i++) { // go through all possible first cut i=1...

    vector<int> cutting1;
    int newRev = p[i] + CutRod(p, n - i, cutting1);

    // when  the above recursive call returns, cutting1 is set to
    // the cutting that achieves max. revenue for rod of length n-i

    if (newRev > curMax) { // update if this option is better then curMax...
      curMax = newRev;
      bestFirstCut = i;

      // Update cutting to be what that achieves curMax
      cutting = cutting1; // the cutting that max for n-i
      cutting.insert(cutting.begin(), bestFirstCut); // add the first cut, i,
                                                     // to get the cutting for n
    }
  }

  cout << "CutRod(n=" << n << ") return " << curMax << ", cutting is ";
  for (auto c : cutting)
    cout << c << ",";
  cout << endl;
  cout << "Press any key and then Ret to continue:";
  cin >> input;

  return curMax;
}

// 2. Recursive with Meomoization:
// Extend thse two functions to output the cutting that achieves the max. profit
// declare the MemoizedCutRod function
/* return the max. revenue
 */

int MemoizedCutRod(vector<int> p, int n) {
  int r[n + 1]; // r[i] stores max profit for a rod of length i
  int c[n + 1]; // c[i] stores the first rod cut and sell==> max. profit

  for (int i = 0; i <= n; i++)
    r[i] = -1;

  // Output the cutting
  // cut n into 2, 3, ...
  int profit = CutRodHelper(p, n, r, c);

  // Output the two tables content here
  for (int i = 0; i <= n; i++)
    cout << i << "\t";
  cout << endl;
  for (int i = 0; i <= n; i++)
    cout << r[i] << "\t";
  cout << endl;
  for (int i = 0; i <= n; i++)
    cout << c[i] << "\t";
  cout << endl;

  // To output the cutting that achieves profit for n
  int remain = n;
  while (remain > 0) {
    cout << c[remain] << ",";
    remain = remain - c[remain];
  }
  return profit;
} // Modify to return the cutting

// The recursive helper function called by MemoizedCutRod
int CutRodHelper(vector<int> p, int n, int r[], int c[]) {
  if (r[n] >= 0)
    return r[n];

  // base case
  if (n == 0) {
    r[0] = 0;
    c[0] = 0;
    return 0;
  }
  if (n == 1) {
    r[1] = p[1];
    c[1] = 1;
    return r[1];
  }
  int curMax; // current max. revenue
  int bestC1;

  int curProfit;
  curMax = 0;

  // explore all possible ways to make first
  // cut , and recursive evaluate smaller prob
  int maxRodLength = p.size() - 1;
  maxRodLength = min(n, maxRodLength);

  for (int c1 = 1; c1 <= maxRodLength; c1++) {

    // the max. profit achievable if we first cut out c1 and sell it
    curProfit =
        p[c1] +
        CutRodHelper(p, n - c1, r,
                     c); // best we can do with remaining rod of length n-c1

    if (curProfit > curMax) {
      bestC1 = c1; // the best first cut for a rod of length n
      // How to remeber all cuts?
      curMax = curProfit;
    }
  }

  c[n] = bestC1;
  r[n] = curMax;

  return curMax;
}

/* 3 Bottom up implementation of Dynamic Programming
 */

// TO DO: Extend CutRodBottomUp to output the cutting that achieves the max
// profit
//  in the end, before returning the max profit achieved for n
int CutRodBottomUp(vector<int> p, int n) {
  int curProfit;
  int r[n + 1];
  int c[n + 1];

  r[0] = 0;
  r[1] = p[1];
  c[0] = 0;
  c[1] = 1;

  for (int i = 2; i <= n; i++) {
    // calculate max profit for rod length i
    int curMax; // current max. revenue
    int bestC1;
    curMax = 0;

    // explore all possible ways to make first
    // cut , and recursive evaluate smaller prob
    int maxRodLength = p.size() - 1;
    maxRodLength = min(i, maxRodLength);

    for (int c1 = 1; c1 <= maxRodLength; c1++) {
      curProfit = p[c1] + r[i - c1];
      if (curProfit > curMax) {
        curMax = curProfit;
      }
    }

    // save the result in the table
    r[i] = curMax;
    c[i] = bestC1;
  }

  // finish evaluating r1, r2, ..., rn
  // if (r[n] >= 0)
  // return r[n];

  // How to output the cutting that achieves r[n]
  cout << "Cutting to achieve max profit for length" << n << ": ";
  int remainder = n;
  while (remainder > 0) {
    cout << c[remainder] << ", ";
    remainder = remainder - c[remainder];
  }
  cout << endl;
  return r[n];
}

int main() {
  vector<int> p{0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
  // p[i] is the price for rod of  length i
  // the max. rod length is p.size()-1

  int n, r;
  char ans;

  vector<int> cutting;

  do {
    cout << "Enter the rod length:";
    cin >> n;

    // call your function to find max. revenue
    // display the result
    /*
    r = CutRod (p, n, cutting);
    cout <<"Maxi. revenue:"<<r<<endl;
    cout <<" achieved with cutting:";
    for (int rlen:cutting)
            cout <<rlen<<" ";
    cout <<endl;
    */

    r = MemoizedCutRod(p, n);
    cout << "Memoized Maxi. revenue:" << r << endl;

    r = CutRodBottomUp(p, n);
    cout << "Bottom up solution:" << r << endl;

    // continue?
    cout << "Continue(y/n)?";
    cin >> ans;
  } while (ans == 'y');
}
