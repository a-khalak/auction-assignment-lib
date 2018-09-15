#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include "AuctAlg.h"

void adriver_timeprint (clock_t &current, clock_t &total,
			  clock_t & prev, const string& op="");


int main()
{

  clock_t prev, total, current;
  int i, j;

  // Read in user input
  char in_file[255], out_file[255];
  int nrow, ncol;
  cerr << "Enter input filename:  "; cin >> in_file;
  cerr << "Enter output filename: "; cin >> out_file;
  cerr << "Number of Rows:        "; cin >> nrow;
  cerr << "Number of Columns:     "; cin >> ncol;

  adriver_timeprint (current, total, prev);

  // create a vector of a vector of doubles (row-oriented matrix)
  std::vector<std:: vector<double> > values;
  values.resize(nrow);
  for (i = 0; i < nrow; i++) values[i].resize(ncol);

  // Loads the payoff matrix from an ascii file, presumably from Matlab
  std::ifstream fin(in_file, ios::in);
  for (i = 0; i < nrow; i++) {
    for (int j = 0; j < ncol; j++) {
      fin >> values[i][j];
    }
  }
  fin.close();

  adriver_timeprint (current, total, prev,  "loading");

  int nrmult, ncmult;
  cout << "Number of row multiples:" ; cin >> nrmult;
  cout << "Number of col multiples:" ; cin >> ncmult;

  for (int zz = 0; zz < 2; zz++) { // NJP - test destructor bug

  AuctParm  Parameters;
  Parameters.set_MAXINT(1000000);
  Parameters.set_res(1);
  Parameters.set_maxcycles(2000);
  Parameters.set_MESSG(true);
  Parameters.set_EScale(true);
  Parameters.set_DEBUG(false);

  AuctShape Shape (values);

  for (i = 0; i < Shape.nrows(); i++) Shape.set_rowmult(i,nrmult);
  for (i = 0; i < Shape.ncols(); i++) Shape.set_colmult(i,ncmult);
  AuctPay   Payoff(Parameters, Shape, values);
  AuctAssoc Associations(Shape);
  cout << "Full rows: " << Shape.nfullrows()
       << "    Full cols: " << Shape.nfullcols() << endl;
  cout << "Number of Non-zeros: " << Shape.nnz() << endl;
  adriver_timeprint (current, total, prev,  "constructing Auction inputs");

  // run the auction
  //
  AuctMetric metric;
  AuctAlg x;
  metric = x.auction(Payoff, Shape, Associations, Parameters);
  adriver_timeprint (current, total, prev,  "auction");

  // Output metrics to screen
  cout << endl << "Number of cycles: " << metric.ncycles() << ". Results are";
  if (!metric.isoptimal()) cout << " not";
  cout << " optimal.  Problem is";
  if (!metric.isfeasible()) cout << " not";
  cout << " feasible. " << endl;

  cout << "Number associated "
       << metric.nauction_assoc() << " by auction, "
       << metric.ntotal_assoc() << " total, with minassoc ="
       << metric.nmaxassoc() << endl;

  cout << "Type of auction: ";
  if (!metric.escaling()) cout << "no ";
  cout << "E-Scaling.   ";
  if (metric.rowmults() || metric.colmults()) cout << "multiple";
  else cout << "single";
  cout << " rows/columns   ";
  if (!metric.reoriented()) cout << "not";
  cout << " reoriented " << endl;

  cout << endl;
  cout << "Total associated payoff: "
       << x.totalpay(Payoff, Shape, Associations)
       << endl << endl;


  // output the results
  //
  std:: ofstream fout(out_file, ios::out);

  switch (2) {

  case 1:
    // option 1, output the raw association matrix
    for (i = 0; i < Shape.nfullrows(); i++) {
      for (j = 0; j < Shape.nfullcols(); j++) {
	fout << Associations.val(i,j) << " ";
      }
      fout << endl;
    }
    break;

  case 2:
    // option 2, output an association matrix the same size as the input
    // payoff with each entry being the total _number_ associated
    for (i = 0; i < Shape.nrows(); i++) {
      for (j = 0; j < Shape.ncols(); j++) {
	fout << Associations.numAssoc(Shape, i,j) << " ";
      }
      fout << endl;
    }
    break;


  case 3:
    // Option 3, output the column group number of each distinct row.
    // this will have Shape.nfullrows() lines.  If the row is
    // unassigned, a -1 appears
    std:: vector <int> assocrow;
    Associations.getrowassign(Shape, assocrow);
    int *Prof, *P;
	Prof = new int[Shape.nfullrows()];
	P    = new int[Shape.nfullcols()];
    Associations.get_prices(P);
    Associations.get_profits(Prof);
    for (i = 0; i < Shape.nfullrows(); i++)
      fout << assocrow[i] << "   ";
    fout << endl;
    fout << endl;
    for (i = 0; i < Shape.nfullcols(); i++)
      fout << P[i] << "   ";
    fout << endl;
  }

  fout.close();

  adriver_timeprint (current, total, prev,  "save");
  cout << "Total CPU: " << 1000.0*total/CLOCKS_PER_SEC << "ms" << endl;
  } // NJP for zz... test destructor bug

  return 0;
}

void adriver_timeprint (clock_t &current, clock_t &total,
			  clock_t & prev, const string& op) {

  total = clock(); current = total - prev; prev = total;

  if (op.length() > 0) {
  cout << "Done with " << op << ". Operation took ";
  cout << 1000.0*current/CLOCKS_PER_SEC << "ms" << endl;
  }
}
