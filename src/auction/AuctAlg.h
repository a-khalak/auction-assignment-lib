
#ifndef auction_H
#define auction_H

#include <list>
#include <algorithm>
#include "AuctParm.h"
#include "AuctShape.h"
#include "AuctPay.h"
#include "AuctAssoc.h"
#include "AuctMetric.h"

#include "mtl/mtl.h"
#include "mtl/matrix.h"

class AuctAlg {

  // ------------------------------------------------------------------
  //
  //	Structures, Enumerators and Type Definitions.
  //
  // ------------------------------------------------------------------

 private:

  /**
   * MTL Type definition for Input Payoff Matrix Data type
   */
  typedef mtl::matrix<int, mtl::rectangle<>, mtl::compressed<int, 
    mtl::external, mtl::index_from_one>, mtl::row_major>::type InMat;

  /**
   * MTL Type definition for Oriented Payoff Data type
   */
  typedef mtl::matrix<int, mtl::rectangle<>, mtl::compressed<>, 
    mtl::row_major>::type IMat;

  /**
   * Structure for Considered bids in the multiple associations case <p>    
   * int v;               // value under consideration. <br>
   * int js;              // Assoc Matrix col.  <br>
   * int grp;             // column group ID.  <br>
   */
  typedef struct PreBidClassMult {
    int v;               // value under consideration
    int js;              // Assoc Matrix col
    int grp;             // column group ID
  } PreBidClassMult;    

  /**
   * Structure for Considered bids <p>
   * int v;               // value under consideration. <br>
   * int js;              // Assoc Matrix col. <br>
   */
  typedef struct PreBidClass {
    int v;               // value under consideration
    int js;              // Assoc Matrix col
  } PreBidClass;    


  // ------------------------------------------------------------------
  //
  //	Action Methods
  //
  // ------------------------------------------------------------------


 public:

  /**
   * This routine is called by the user to generate associations,
   * which are placed into the AuctAssoc object, given AuctPay,
   * AuctShape and AuctParm objects. <p>
   *
   * It is possible to initialize AuctAssoc with some associations, if
   * the input AuctAssoc structure satisfies e-CS.  An empty
   * association structure trivially satisfies this. <p>
   *
   * Currently, the top-level auction simply calls a Forward Auction
   * routine.  Future versions might require a more sophisticated 
   * top-level auction with the ability to switch among high level 
   * auction strategies. <p>
   *
   * @param Pay Payoff Matrix
   * @param Sh Problem "Shape" which includes association multiplicities
   * @param S Association Matrix (modified to contain auction results)
   * @param Prms Algorithm and Run-time parameters
   * @return AuctMetric result from auctionF pass-through.
   */
  AuctMetric auction (AuctPay& Pay, AuctShape& Sh, AuctAssoc& S, 
		      AuctParm& Prms) {
    return auctionF (Pay, Sh, S, Prms);
  }

  /**
   * The total payoff for a specified assignment, using internal payoff.
   * Note that the payoff in Pay is scaled, cast to ints, etc.
   * @param Pay Payoff Matrix
   * @param Sh Problem "Shape" which includes association multiplicities
   * @param S Association Matrix (where auction results are placed)
   * @return Payoff from an assignment in "internal" integer Payoff units.
   */
  int totalpay (AuctPay& Pay, AuctShape& Sh, AuctAssoc& S);

  /**
   * The total payoff for a specified assignment, using "original" payoff.
   * @param Val a Vector of Vectors respresenting the original payoff matrix
   * @param Sh Problem "Shape" which includes association multiplicities
   * @param S Association Matrix (modified to contain auction results)
   * @return Payoff from an assignment in "original" Payoff units (same as Val).
   */
  template<class T>
    T totalpay (std::vector<std::vector<T> > Val, AuctShape& Sh, AuctAssoc& S) {
    int col, fullrow, row;
    T total = 0;
    for (fullrow=0; fullrow<Sh.nfullrows(); fullrow++) {
      row = Sh.get_rowmap(fullrow);
      col = Sh.get_colmap(S.col(fullrow));
      total += (col == -1) ? 0 : Val[row][col];
    }
    return total;
  }
      
 private:

  /**
   * Forward auction with (optional) epsilon-scaling and multi associations..
   * This is an auction manager for any algorithm which has a bidding 
   * step which is interated towards convergence.  It assumes that whatever
   * bidding step is implemented preserves epsilon-Complementarity Slackness.<p>
   * The cyclical iteration has two special features: <br>
   * Epsilon-scaling.  The bidding cycle can be run to convergence several 
   * times, in a sequence of decreasing values of epsilon. <br>
   * Greedy fill-in.  After a specified number of cycles, the bidding is 
   * terminated, and the remaining unassociated entries are filled in using a
   * greedy assignment algorithm.  This can be used to balance optimality with 
   * run-time.
   * @param Pay Payoff Matrix
   * @param Sh Problem "Shape" which includes association multiplicities
   * @param S Association Matrix (where auction results are placed)
   * @param Prms Algorithm and Run-time parameters
   * @return AuctMetric result.
   */
  AuctMetric auctionF (AuctPay& Pay, AuctShape& Sh, AuctAssoc& S, 
		      AuctParm& Prms);

  /**
   * Transpose an InMat Matrix (external storage) into an IMat Matrix
   * (internal storage)
   */
  void ctransp(InMat&, IMat&);

  /**
   * Check that the associations in S, for the problem defined by the
   * payoff, A, Shape, Sh, and Parameters, Prms, satisfy e-CS, a
   * technical condition required for optimality.  Used for debugging. <p>
   * @param A Payoff Matrix
   * @param Sh Problem "Shape" which includes association multiplicities
   * @param S Association Matrix
   * @param Prms Algorithm and Run-time parameters
   */
  void checkecs (IMat& A, AuctShape& Sh, AuctAssoc& S, AuctParm& Prms);

  /**
   * A greedy association to fill in the remaining, unassigned rows
   * of S in order of row index.
   * @param A Payoff Matrix
   * @param Sh Problem "Shape" which includes association multiplicities
   * @param S Association Matrix
   * @param Prms Algorithm and Run-time parameters
   */
  void greedy_fill (IMat& A, AuctShape& Sh, AuctAssoc& S, AuctParm& Prms);

  /**
   * SM auction "cycle" accomodates a (S)parse payoff with (M)ultiple 
   * associations.
   * @param A Payoff Matrix
   * @param Sh Problem "Shape" which includes association multiplicities
   * @param S Association Matrix
   * @param Prms Algorithm and Run-time parameters
   */
  void auct_cycle_SM (IMat& A, AuctShape& Sh, AuctAssoc& S, AuctParm& Prms);

  /**
   * SP1 auction "cycle" accomodates a (S)parse payoff with (P)rimitive, 
   * that is, single, associations.
   * @param A Payoff Matrix
   * @param Sh Problem "Shape" which includes association multiplicities
   * @param S Association Matrix
   * @param Prms Algorithm and Run-time parameters
   */
  void auct_cycle_SP1 (IMat& A, AuctShape& Sh, AuctAssoc& S, AuctParm& Prms);

  /**
   * SP2 auction "cycle" accomodates a (S)parse payoff with (P)rimitive, 
   * that is, single, associations, but it computes both profits and prices 
   * (suitable for reverse auction).
   * @param A Payoff Matrix
   * @param Sh Problem "Shape" which includes association multiplicities
   * @param S Association Matrix
   * @param Prms Algorithm and Run-time parameters
   */
  void auct_cycle_SP2 (IMat& A, AuctShape& Sh, AuctAssoc& S, AuctParm& Prms);

};

#endif
