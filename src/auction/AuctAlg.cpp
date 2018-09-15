#include "AuctAlg.h"

using namespace mtl;

// Compute the associated total payoff
int AuctAlg::totalpay (AuctPay& Pay, AuctShape& Sh, AuctAssoc& S) {
  int col, fullrow, row;
  int pindex;
  int total = 0;
  std::list<int> colind, colmult;
  std::list<int>::iterator i, j;

  for (row = 0; row<Sh.nrows(); row++) {
    colind.clear(); colmult.clear();
    // This loop over the Shape row pointers is over row multiplicities
    for (fullrow = Sh.get_rowptr(row); fullrow < Sh.get_rowptr(row+1);
	 fullrow++) {
      col = Sh.get_colmap(S.col(fullrow))+1;   // AuctPay indexes from 1.
      if (col != -1) colind.push_back(col);
    }
    // Compactify index list into single, unique entries, with an
    // associated list which stores the multiplicity of each entry
    colind.sort();
    for (i = colind.begin(); i != colind.end();) {
      colmult.push_back (std::count (i, colind.end(), *i));
      j = i++;
      std::remove(i, colind.end(), *j);
    }
    // This loop over Payoff row pointers is over a compressed row
    for (pindex = Pay.get_rowptr(row)-1;       // AuctPay indexes from 1
	 !colind.empty() && pindex < Pay.get_rowptr(row+1)-1;
	 pindex++) {
      if (colind.front() == Pay.get_col(pindex)) {
	total += colmult.front() * Pay.get_value(pindex);
	colmult.pop_front(); colind.pop_front();
      }
    }
  }
  return total;
}

// Implement the auction algorithm
AuctMetric AuctAlg::auctionF (AuctPay& Pay, AuctShape& Sh, AuctAssoc& S,
		    AuctParm& Prms)
{
  int N = Sh.nrows(), M = Sh.ncols(), NNZ = Sh.nnz();
  int Nf=Sh.nfullrows(), Mf=Sh.nfullcols();
  int maxcycles = Prms.get_maxcycles();

  Sh.ifcompute();     // Insure that the Shape multiplicities are up-to-date
  // If there are multiplicities then "full" rows/cols don't match
  // payoff rows/cols.  Set MULTI appropriately.
  bool MULTI = (Nf == N && Mf == M)? false : true;
  bool EScale = Prms.EScale(), DEBUG = Prms.DEBUG(), MESSG = Prms.MESSG();
  if (MESSG) cerr << "Using multiple rows/cols? : " << MULTI << endl;

  AuctMetric metric;

  // Check consistency of inputs.
  if (!Pay.is_consistent(Sh) || !S.is_consistent(Sh)) {
    return metric;  // return with zero cycles.
  }
  // Check for a null Payoff
  if (Nf == 0 || Mf == 0) {
    return metric;  // return with zero cycles
  }

  // In the degenerate case of a 1x1 compressed payoff, EScaling is not
  // viable or useful, regardless of what the user wants.
  if (N == 1 && M == 1) {
     EScale = false;
  }

  // Set-up the Auction Metrics return class
  metric.set_rowmults((Nf != N));  metric.set_colmults((Mf != M));
  metric.set_escaling(EScale);   metric.set_paydensity((NNZ/M*N));
  metric.set_sparsified(Pay.dense_to_sparse());

  // Rescale and quantize payoff matrix to integer.  The scaling is
  // done in a temporary matrix of doubles (ScA), which is cast to
  // integer in a copy command.
  //
  InMat input (N, M, NNZ, Pay.get_valpt(), Pay.get_rowpt(),
	       Pay.get_indpt());

  bool reorient_flag = (Nf > Mf) ? true : false;
  metric.set_reoriented(reorient_flag);
  if (reorient_flag) std::swap(N,M);
  IMat A(N,M);

  if (reorient_flag) {
    if (MESSG) cerr << "Reorienting payoff matrix" << endl;
    Sh.transpose();
    S.transpose();
    AuctAlg::ctransp(input, A);
    mtl::scale(A, (std:: min(N, M)+1));
  } else {
    A.fast_copy(input);
    mtl::scale(A, (std:: min(N, M)+1));
  }

  if (DEBUG) cerr << "A has "<< A.nrows() << " rows and "
		  << A.ncols() << " cols." << endl;
  if (DEBUG) cerr << "input has "<< input.nrows() << " rows and "
		  << input.ncols() << " cols." << endl;

  // Loop over auction rounds until associated
  int assoc_thresh = std:: min(Nf, Mf);
  metric.set_nmaxassoc(assoc_thresh);
  int cycles=0;
  int epsstart;
  int epsfac   = Prms.get_epsfac();
  epsstart = (Prms.epsrelative())
    ? (int)(std:: min(N,M) * Prms.get_epsstart() / Prms.get_res())
    : (int)Prms.get_epsstart();
  if (epsstart < 2 || epsfac < 2) epsstart = 1;

  if (EScale) {
    int eps;
    for (eps=epsstart; eps > 1 && cycles < maxcycles; eps=eps/epsfac) {
      Prms.set_epsilon(eps);
      S.blank();
      if (MESSG) cerr << "Starting epsilon cycle: " << eps << endl;
      while (S.nassoc() < assoc_thresh && cycles < maxcycles) {
	while (S.nassoc() < assoc_thresh && cycles < maxcycles) {
	  cycles++;
	  if (DEBUG) cout << "Cycle: " << cycles << endl;
	  if (MULTI) {
	    AuctAlg::auct_cycle_SM(A, Sh, S, Prms);
	  } else {
	    AuctAlg::auct_cycle_SP1(A, Sh, S, Prms);
	  }
	}
	S.hidden_bid(Sh, Prms);
      }
    }

    // An opportunity to ditch if the number of cycles has hit the maximum
    if (cycles >= maxcycles) {
      metric.set_ncycles(cycles); metric.set_nauction_assoc(S.nassoc());
      metric.set_timed_out(true);
      AuctAlg::greedy_fill (A, Sh, S, Prms);
      metric.set_ntotal_assoc(S.nassoc());
      if (reorient_flag) {
	Sh.transpose();
	S.transpose();
      }
      return metric;
    }
  }

  // The last cycle at epsilon = 1.
  if (EScale) {
    Prms.set_epsilon(1);
  } else {
    Prms.set_epsilon(epsstart);
  }
  if (MESSG) cerr << "Starting epsilon cycle: " << Prms.get_epsilon() << endl;
  S.blank();
  while (S.nassoc() < assoc_thresh && cycles < maxcycles) {
    while (S.nassoc() < assoc_thresh && cycles < maxcycles) {
      cycles++;
      if (DEBUG) cout << "Cycle: " << cycles << endl;
	if (MULTI) {
	  AuctAlg::auct_cycle_SM(A, Sh, S, Prms);
	} else {
	  AuctAlg::auct_cycle_SP1(A, Sh, S, Prms);
	}
    }
    if (EScale) S.hidden_bid(Sh, Prms);
  }

  if (DEBUG) AuctAlg::checkecs(A, Sh, S, Prms);

  metric.set_ncycles(cycles);
  metric.set_nauction_assoc(S.nassoc());

  // Again, greedy fill if we timed out
  if (cycles >= maxcycles) {
    metric.set_timed_out(true);
    AuctAlg::greedy_fill (A, Sh, S, Prms);
  }

  if (reorient_flag) {
    Sh.transpose();
    S.transpose();
  }

  metric.set_ntotal_assoc(S.nassoc());
  return metric;
}



// This function transposes a compressed, row-oriented matrix, x,
// and puts the output into y.
void AuctAlg::ctransp (InMat& x, IMat& y) {

  typedef mtl::matrix<int, mtl::rectangle<>, mtl::compressed<int, mtl::external,
	  mtl::index_from_zero>, mtl::row_major>::type IMatExt;

  int N = x.nrows(), M = x.ncols(), NNZ = x.nnz();

  InMat::iterator k;
  InMat::OneD::iterator *iter, *iter_end, tk;
  iter = new InMat::OneD::iterator[N];
  iter_end = new InMat::OneD::iterator[N];

  int *ind, *npt, *nval, *nind;
  int curpt = 0;
  int i,j;

  ind = new int [N];
  npt = new int [M+1];
  nval = new int [NNZ];
  nind = new int [NNZ];

  for (j=0; j<N; j++) ind[j] = -1;

  for (k=x.begin(); k<x.end(); k++) {
    tk = (*k).begin();
    if (tk < (*k).end()) {
      iter[tk.row()]=tk;
      iter_end[tk.row()] = (*k).end();
      ind[tk.row()] =tk.column();
    }
  }

  for (i=0; i<M; i++) {
    npt[i] = curpt;
    for (j=0; j<N; j++) {
      if (ind[j] == i) {
	nval[curpt] = *iter[j];
	nind[curpt] = iter[j].row();
	curpt++;
	iter[j]++;
	ind[j] = (iter[j] < iter_end[j]) ? iter[j].column() : -1;
      }
    }
  }
  npt[M] = curpt;

  IMatExt temp (M, N, NNZ, nval, npt, nind);
  mtl::copy (temp, y);
}


// This subroutine is a checker for epsilon-Complementarity slackness
// of the Association Matrix, and the Prices. e-CS is a necessary
// condition for optimality, and a full e-CS association with small
// enough e-CS is guaranteed to be optimal.
void AuctAlg::checkecs (IMat& A,AuctShape& Sh,AuctAssoc& S,AuctParm& Prms) {

  int diff;
  int tmp, aij, p;
  int bestval, bestcol, bestpr, bestaij, bestcolS;
  int maxval, maxcol, maxpr, maxaij, maxcolS;
  IMat::OneD::iterator j;
  int minassign, maxunass;
  int rowA, colA, rowS, colS;

  bestcol = bestpr = bestaij = bestcolS = 0;
  maxcol = maxpr = maxaij = maxcolS = 0;

  S.group_price_equalize(Sh);
  int *P;
  P = new int [Sh.nfullcols()];
  S.get_prices(P);


  for (rowA = 0; rowA < Sh.nrows(); rowA++) {
    maxval = 0;
    bestval = 0;
    for (colS = 0; colS < Sh.nfullcols(); colS++) {
      colA = Sh.get_colmap(colS);
      aij  = A(rowA,colA);
      p    = P[colS];
      tmp  = aij - p;
      rowS = S.row(colS);
      if (Sh.get_rowmap(rowS) == rowA && bestval < tmp) {
	  bestval = tmp;
	  bestcol = colA;
	  bestpr  = p;
	  bestcolS = colS;
	  bestaij  = aij;
      } else if (maxval < tmp) {
	  maxval = tmp;
	  maxcol = colA;
	  maxcolS = colS;
	  maxpr  = p;
	  maxaij = aij;
      }
    }
    diff =  maxval - bestval;
    if (diff > Prms.get_epsilon()) {
      cout << "CS violation at (row,col): (" <<
	rowA << "," << bestcol << ")" << " mxcol: " << maxcol <<
	"  val: " << bestval << "  maxval:" << maxval <<
	"  P:" << bestpr << " maxpr:" << maxpr <<
	endl;
      cout << "                           colid:" << bestcolS <<
	"  best aij:" << bestaij << "  maxcolid:" << maxcolS <<
	"  max aij:" << maxaij << endl;
    }
  }

  minassign = S.get_minassoc();
  maxunass = S.get_maxunass();
  cout << "lambda: " << maxunass << "  Min Assign: " << minassign << endl;

}

// This subroutine runs if the number of auction cycles hits the
// maximum allowed.  It greedily makes some associations to improve the
// partial answer offered by the auction algorithm.
void AuctAlg::greedy_fill (IMat& A, AuctShape& Sh, AuctAssoc& S,
			   AuctParm& Prms) {

  int maxbid, maxbidcol, assoccol;
  IMat::OneD::iterator j;
  int minassoc = min(Sh.nfullrows(), Sh.nfullcols());

  for (int i = 0; i < Sh.nfullrows() && S.nassoc() < minassoc; i++) {
    if  (S.col(i) == -1) {
      maxbidcol = -1;
      maxbid = -Prms.get_MAXINT();
      for (j = A[Sh.get_rowmap(i)].begin(); j<A[Sh.get_rowmap(i)].end(); j++) {
		assoccol = -1;
	    if (maxbid < *j) {
	  for (int k=Sh.get_colptr(j.column());
	       k<Sh.get_colptr(j.column()+1); k++) {
	    if (S.row(k) == -1) {
	      assoccol = k;
	      k = Sh.get_colptr(j.column()+1);
	    }
	  }
	  if (assoccol != -1) {
	    maxbid = *j;
	    maxbidcol = assoccol;
	  }
	}
      }
      if (maxbidcol > -1) S.add (i, maxbidcol);
    }
  }
}

// Auction Cycle SM, with a sparse payoff matrix, A, and
//                   accomodating multiple rows and columns
//
// A is the payoff matrix, which is constructed using the Matrix
// Template library.  The matrix is stored sparsely
// (column-compressed) but may be accessed using row and column
// iterators, which automatically skip over the empty entries.
//
// Sh is the Auction "shape" structure.  This stores the size of A, as
// well as the multiplicities of each row and column of A.  It has
// methods to map between the "full" association matrix, S, (with each
// row/column copy of A having its own index) and the payoff matrix, A.
//
// S is the Auction association structure, which includes the
// association matrix, as well as the price vector.  It contains
// special methods to make the one-to-one mapping "look" like a
// matrix, but be efficiently stored.
//
// Prms is the Auction parameters structure
//
void AuctAlg::auct_cycle_SM (IMat& A, AuctShape& Sh, AuctAssoc& S,
				AuctParm& Prms) {

  int NEGLARGE = -Prms.get_MAXINT();
  int epsilon = Prms.get_epsilon();

  IMat::iterator i;
  IMat::OneD::iterator j;

  PreBidClassMult curshop, empty;
  std::list<PreBidClassMult> shoplist;
  std::list<PreBidClassMult>::iterator si;
  int rowA, rowS, colS;
  int refgroup, refval, tmpval, saveval, startCol, endCol, minprice;

  empty.v = NEGLARGE;
  empty.js = empty.grp = -1;

  bool freerow_flag, groupmatch_flag;
  std::list<int> usedcols, freerows;
  std::list<int>::iterator ui;

  bool DEBUG = Prms.DEBUG();

  // Compute bids, submitting as many bids as possible for each A row.

  // Loop over rows of A
  for (i = A.begin(); i < A.end(); i++) {
    rowA = ((*i).begin()).row();

    // Build list of occupied columns and free rows corrsp. to this A row.
    freerow_flag = false;
    usedcols.clear(); freerows.clear();
    for (rowS = Sh.get_rowptr(rowA); rowS < Sh.get_rowptr(rowA+1); rowS++) {
      if (S.col(rowS) != -1) {
	usedcols.push_back(S.col(rowS));
      } else {
	freerow_flag = true;
	freerows.push_back(rowS);
      }
    }

    if (DEBUG) cout << "On A row " << rowA << " currently has "
		    << freerows.size() << " free rows" << endl;

    // If there are any free rows, then find bids for them.  This is
    // done by assembling a "shopping list" of the top value columns to
    // be bid upon.
    if (freerow_flag) {
      shoplist.clear();
      shoplist.insert(shoplist.begin(), freerows.size()+1, empty);

      // Loop over columns of A
      for (j = (*i).begin(); j < (*i).end(); j++) {
	curshop.grp = j.column();

	// Loop over cooresponding columns of S.
	for (colS = Sh.get_colptr(curshop.grp);
	     colS < Sh.get_colptr(curshop.grp+1); colS++) {
	  curshop.js = colS;
	  curshop.v  = *j - S.Price(colS); // value = payoff - price

	  // Insert into the shopping list if appropriate.  To decide,
	  // run down the list, considering several properties:
	  //
	  // P1: list entry value >= curshop.v
	  // P2: curshop.js already taken (i.e. is in usedcols list)
	  // P3: list entry group == curshop.grp
	  //
	  // The decision tree is a follows:
	  //   Case 1: P1                   step to next list value
	  //   Case 2: !P1 & P2             end (forget this S column)
	  //   Case 3: !P1 & !P2            insert into list
	  si = shoplist.begin();
	  while (si != shoplist.end()) {
	    if (si->v > curshop.v) {             // Case 1: step
	      si++;
	    } else {
	      ui = std::find(usedcols.begin(), usedcols.end(), curshop.js);
	      if (ui != usedcols.end()) {
		si = shoplist.end();             // Case 2: end
	      } else {
		shoplist.insert(si,curshop);     // Case 3: insert
		shoplist.pop_back();
		si = shoplist.end();
	      }
	    }
	  } // end loop over shopping list
	} // end loop over S cols corresponding to this A col
      } // end loop over columns of A

      // Construct reference from last member of the shopping list
      refgroup = shoplist.front().grp;
      refval   = shoplist.back().v;
      shoplist.pop_back();

      // Make sure that we are working with a non-empty row
      if (refgroup != -1) {

	// In the instance that the whole shopping list, including the
	// reference, are from the same group, explicitly look for a
	// reference value from a distinct group.
	groupmatch_flag = true;
	si = shoplist.begin();
	while (++si != shoplist.end() && groupmatch_flag)
	  groupmatch_flag = si->grp == refgroup;

	if (groupmatch_flag) {                    // Reference is unsuitable
	  saveval = NEGLARGE;
	  for (j = (*i).begin(); j < (*i).end(); j++) {
	    if (j.column() != refgroup) {
	      startCol = Sh.get_colptr(j.column());
	      endCol   = Sh.get_colptr(j.column()+1);
	      minprice = S.min_price_in_colrange(startCol, endCol);
	      tmpval   = *j - minprice;
	      if (tmpval <= refval && tmpval > saveval) saveval = tmpval;
	    }
	  }
	  refval = saveval;
	} // end if block (for recomputing reference value)

	// Make the bids for each row in freerows.
	for (si = shoplist.begin(), ui = freerows.begin();
	     si != shoplist.end(); si++, ui++) {
	  S.add (*ui, si->js);
	  S.set_Price(si->js, S.Price(si->js) + si->v - refval + epsilon);
	  if (DEBUG) {
	    cout << "Submitted a bid for ("<<*ui<<","<<si->js<<") for "
		 << "value " << si->v << " reference " << refval << endl;
	  }
	} // end for loop over bids
      } // end if non-empty row
    } // end if block (for unassociated entries)
  } // end loop over rows
}

// Auction Cycle, with only single rows/columns, without profits calculations.
// Gauss-Seidel version (i.e. bids executed as made).
void AuctAlg::auct_cycle_SP1 (IMat& A, AuctShape& Sh, AuctAssoc& S,
			   AuctParm& Prms) {


  int NEGLARGE = -Prms.get_MAXINT(), epsilon = Prms.get_epsilon();
  bool DEBUG   = Prms.DEBUG();

  IMat::iterator i;
  IMat::OneD::iterator j;

  PreBidClass curshop, empty;
  std::list<PreBidClass> shoplist;
  std::list<PreBidClass>::iterator si, snxt;

  int rowA=0;
  empty.v = NEGLARGE; empty.js = -1;

  // Loop over nonempty rows of A
  for (i = A.begin(); i < A.end(); i++) {
    rowA = ((*i).begin()).row();
    if (DEBUG) cout << "On row " << rowA << " currently associated to "
		    << S.col(rowA) << endl;
    if (S.col(rowA) == -1) {
      shoplist.clear();
      shoplist.push_front(empty);
      shoplist.push_front(empty);

      // Loop over nonempty columns of A, insertion sort for top two vals.
      for (j = (*i).begin(); j < (*i).end(); j++) {
	curshop.js = j.column();
	curshop.v  = *j - S.Price(curshop.js);
	si = shoplist.begin();
	while (si != shoplist.end()) {
	  if  (curshop.v > si->v) {
	    shoplist.insert(si, curshop);
	    shoplist.pop_back();
	    si = shoplist.end();
	  } else si++;
	}
      } // end loop over columns of A

      // Only assign the row if it is non-empty
      si = shoplist.begin();
      if (si->js != -1) {
	snxt = si; ++snxt;
	S.add (rowA, si->js);
	S.set_Price(si->js, S.Price(si->js) + si->v - snxt->v + epsilon);
	if (DEBUG) cout << "Added (" << rowA << "," << si->js
			<< ") for price " << S.Price(rowA) << endl;
      } // end if non-empty row
    } // end if block (for unassociated entries)
  } // end loop over rows of A
}

void AuctAlg::auct_cycle_SP2 (IMat& A, AuctShape& Sh, AuctAssoc& S,
			   AuctParm& Prms) {

  // This version: NO has multiple rows/columns, YES has profits. (plain 2).
  //               NO modified  YES sparse payoff
  // Gauss-Seidel Version

  int NEGLARGE = -Prms.get_MAXINT();
  int epsilon = Prms.get_epsilon();
  int rowA=0;

  IMat::iterator i;
  IMat::OneD::iterator j;

  PreBidClass curshop, empty;
  std::list<PreBidClass> shoplist;
  std::list<PreBidClass>::iterator si, snxt;

  empty.v = NEGLARGE; empty.js = -1;

  // Loop over nonempty rows of A
  for (i = A.begin(); i < A.end(); i++) {
    rowA = ((*i).begin()).row();
    if (S.col(rowA) == -1) {
      shoplist.clear();
      shoplist.push_front(empty);
      shoplist.push_front(empty);

      // Loop over nonempty columns of A, insertion sort for top two values
      for (j = (*i).begin(); j < (*i).end(); j++) {
	curshop.js = j.column();
	curshop.v  = *j - S.Price(curshop.js);
	si = shoplist.begin();
	while (si != shoplist.end()) {
	  if  (curshop.v > si->v) {
	    shoplist.insert(si, curshop);
	    shoplist.pop_back();
	    si = shoplist.end();
	  } else si++;
	}
      } // end loop over columns of A

      // Only assign the row if it is non-empty
      si = shoplist.begin();
      if (si->js != -1) {
	snxt = si; ++snxt;
	S.add (rowA, si->js);
	S.set_Price (si->js, S.Price(si->js) + si->v - snxt->v + epsilon);
	S.set_Prof(rowA, snxt->v - epsilon);
      } // end if non-empty row
    } // end if block (for unassociated entries)
  } // end loop over rows of A
}
