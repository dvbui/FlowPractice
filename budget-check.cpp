// Auditor - Budget validator program
//
//
// Hypothesis: The output file is valid to be accepted.
//
// Type I errors:
//   Rejects if the answer file is not valid.
//   Rejects whitespace on empty lines.
//   (, ..?)
//
// Type II errors:
//   Probably accepts if values overflow by multiples of 2^32..
//   Allows negative budget entries, but this is according to the
//   problem specification.
//   (, ..?)
//
// ((, ..?)=up to extreme input cases, overseen bugs and interpretation)
//
//
// Mattias de Zalenski

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

typedef vector<int> V;
typedef vector<V> M;

int P = 0; // case
string reject_reason = "null";


// Give a detailed reason to reject
bool reject(string reason) {
  cerr << "Case " << P << ": ";
  cerr << "Rejected " << reason << '.' << endl;
  reject_reason = reason;
  return false;
}

// Give a detailed reason to accept
bool accept(string hesitation) {
  cerr << "Accepted " << hesitation << '.' << endl;
  reject_reason = hesitation;
  return true;
}


// Check that a stream is ok, but has no more data except whitespace
bool empty_stream(istream &i) {
  string s;
  return
    (i || reject("stream error")) &&
    (!(i >> s) || reject(s + " non-empty stream"));
}


// Validate one value constraint
bool audit(int v, char op, int x) {
  if (op == '<') return v < x;
  if (op == '=') return v == x;
  if (op == '>') return v > x;
  return reject("unknown input");
}


// Validate one test case
bool check(istream &in, istream &out, istream &ans) {
  // read matrix size
  int m, n;
  in >> m >> n;
  V row(n, 0);
  M matrix(m, row);

  // read matrix, detect IMPOSSIBLE solutions from the _answer_ file
  string o, a;
  for (int i = 0; i < m; ++i) {
    getline(out, o);
    getline(ans, a);
    istringstream os(o);
    istringstream as(a);
    string aimp;
    as >> aimp;
    if (i == 0 && aimp == "IMPOSSIBLE") {
      string oimp;
      os >> oimp;
      // read all input
      int k, x;
      char c;
      for (int i = 0; i < m + n + 1; ++i)
	in >> k;
      for (int i = 0; i < k; ++i)
	in >> x >> x >> c >> x;
      return empty_stream(os) && empty_stream(as) && oimp == "IMPOSSIBLE"
	|| reject("IMPOSSIBLE solution not detected");
    }

    // read a row
    istringstream str(o);
    for (int j = 0; j < n; ++j)
      str >> matrix[i][j];

    // nothing less!
    if (!str) return reject("bad matrix row");
    // and nothing more!
    if (!empty_stream(str)) return reject("extra output");
  }

  // read and check the row sums
  for (int i = 0; i < m; ++i) {
    int sum;
    in >> sum;
    for (int j = 0; j < n; ++j)
      sum -= matrix[i][j];
    if (sum != 0) return reject("incorrect row sum");
  }

  // read and check the column sums
  for (int j = 0; j < n; ++j) {
    int sum;
    in >> sum;
    for (int i = 0; i < m; ++i)
      sum -= matrix[i][j];
    if (sum != 0) return reject("incorrect column sum");
  }

  // read and check the constraints
  int k;
  in >> k;
  while (k-- > 0) {
    int r, c;
    char op;
    int x;
    in >> r >> c >> op >> x;

    int r1 = r == 0 ? 1 : r, r2 = r == 0 ? m : r;
    int c1 = c == 0 ? 1 : c, c2 = c == 0 ? n : c;
    for (int i = r1; i <= r2; ++i)
      for (int j = c1; j <= c2; ++j)
	if (!audit(matrix[i - 1][j - 1], op, x))
	  return reject("constraint not satisfied");
  }

  // check for negative integers in the budget?
  for (int i = 0; i < m; ++i)
    for (int j = 0; j < n; ++j)
      if (matrix[i][j] < 0) return accept("negative"); // and approve!

  return true; // budget accepted.
}


// Validate an empty line between cases
bool empty_line(istream &out, istream &ans) {
  string o, a;
  getline(out, o);
  getline(ans, a);
  istringstream os(o);
  istringstream as(a);
  return empty_stream(os) && empty_stream(as)
    || reject("non-empty empty line");
}


// Output the xml result tag
void xml(ostream &res, string outcome, string security) {
  res << "<?xml version=\"1.0\"?>" << endl;
  res << "<result " <<
    "outcome=\"" << outcome << "\" " <<
    "security=\"" << security << "\"> " <<
    reject_reason << " </result>" << endl;
}


// Auditor
int main(int argc, char *argv[]) {
  if (argc <= 4) {
    cerr << "Usage: auditor infile outfile ansfile resfile" << endl;
    return 1;
  }
  ifstream in(argv[1]);
  ifstream out(argv[2]);
  ifstream ans(argv[3]);
  ofstream res(argv[4]);

  // for-solve
  int n;
  in >> n;

  bool valid = true;
  for (int i = 0; valid && i < n; ++i) {
    P = i + 1;
    if (i > 0) valid &= empty_line(out, ans); // empty line between cases
    if (valid) valid &= check(in, out, ans); // correct answer
  }
  // no extra data
  if (valid) {
    valid &= empty_stream(in) || reject("bad or extra input data");
    valid &= empty_stream(out) || reject("bad or extra output data");
    valid &= empty_stream(ans) || reject("bad or extra answer data");
  }

  string outcome = valid ? "YES" : "NO - Wrong Answer";
  string security = argv[4];

  xml(res, outcome, security);

  return 0;
}
