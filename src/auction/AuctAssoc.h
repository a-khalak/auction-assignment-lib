
#ifndef AuctAssoc_H
#define AuctAssoc_H

#include "AssocMatrix.h"

class AuctAssoc : public AssocMatrix {


  // ------------------------------------------------------------------
  //
  //	Constructors, Destructors, and Operators.
  //
  // ------------------------------------------------------------------

 public:

  /** 
   * Constructor for symmetric association matrix
   * @param N size of association matrix
   */
  AuctAssoc(int N);
  
  /** 
   * Constructor for asymmetric association matrix
   * @param N number of rows in association matrix
   * @param M number of columns in association matrix
   */
  AuctAssoc(int N, int M);

  /** 
   * Constructor for symmetric association matrix
   * @param S Shape of auction problem.
   */
  AuctAssoc (AuctShape& S);

  /** 
   * Destructor. 
   */
  virtual ~AuctAssoc();

  // ------------------------------------------------------------------
  //
  //	Accessor Methods
  //
  // ------------------------------------------------------------------

 public:

  /** 
   * Access prices (scalar).
   * @param ind Column index of price to look up
   * @return Price of queried row
   */
  inline int      Price(int ind)            { return prices[ind]; }

  /** 
   * Set prices (scalar).
   * @param ind Column index of price to set
   * @param value Price for given row
   */
  inline void set_Price(int ind, int value) { 
    if (ind > -1) prices[ind] = value; 
  }

  /** 
   * Access profits (scalar).
   * @param ind Row index of profit to look up
   * @return Profit of queried column
   */
  inline int      Prof(int ind)             { return profits[ind]; }

  /** 
   * Set profits (scalar).
   * @param ind Row index of profit to set
   * @param value Profit of given column
   */
  inline void set_Prof(int ind, int value)  { 
    if (ind > -1) profits[ind] = value; 
  }

  /** 
   * Access prices (vector).
   * @param outprices Array in which to fill prices for each row
   */
  void get_prices(int* outprices) { std::copy(prices, prices+Ncols, outprices); }

  /** 
   * Set prices (vector).
   * @param inprices Array containing prices for each row
   */
  void set_prices(int* inprices) { std::copy(inprices, inprices+Ncols, prices); }

  /** 
   * Access profits (vector).
   * @param outprof Array in which to fill profits for each column
   */
   void get_profits(int* outprof) { std::copy(profits, profits+Nrows, outprof); }

  /** 
   * Set profits (vector).
   * @param inprof Array containing profits for each column
   */
  void set_profits(int* inprof) { std::copy(inprof, inprof+Nrows, profits); }


  // ------------------------------------------------------------------
  //
  //	Action Methods
  //
  // ------------------------------------------------------------------

 public:

  /** 
   * Consistency test w.r.t. a given shape. 
   * @param Sh Shape to test consistency with
   * @return True if given shape is consistent with this Association Matrix
   */
  bool is_consistent (AuctShape &Sh) {
    return (Sh.nfullrows() == Nrows && Sh.nfullcols() == Ncols);
  }

  /** 
   * Transpose Associations.  Overrides the AssocMatrix version. 
   */
  void transpose();

  /** 
   * compute minimum price. 
   * @return Minimum price for all rows
   */
  int min_price();

  /** 
   * Compute maximum price for specified rows.  Only rows which have an
   * associated column are considered.  The price returned is that of the
   * associated column.
   * @return Maximum price for rows in specified range
   * @param first First row in range to consider
   * @param last Last row in range to consider
   */
  int max_price_in_rowrange (int first, int last);

  /** 
   * Compute minimum price for specified rows. Only rows which have an
   * associated column are considered.  The price returned is that of the
   * associated column.
   * @return Minimum price for rows in specified range
   * @param first First row in range to consider
   * @param last Last row in range to consider
   */
  int min_price_in_rowrange (int first, int last);

  /** 
   * Compute maximum price for specified columns. 
   * @return Maximum price for rows in specified range
   * @param first First column in range to consider
   * @param last Last column in range to consider
   */
  int max_price_in_colrange (int first, int last);

  /** 
   * Compute minimum price for specified columns. 
   * @return Minimum price for columns in specified range
   * @param first First column in range to consider
   * @param last Last column in range to consider
   */
  int min_price_in_colrange (int first, int last);

  /** 
   * Compute the minimum associated price for a columns. 
   * @return Minimum price for columns which are associated w/ a row.
   */
  int get_minassoc();

  /** 
   * Compute the maximum unassociated price for a columns. 
   * @return Maximum price for a column which is not associated with any row.
   */
  int get_maxunass();

  /** 
   * Compute the minimum associated profit for a row. 
   * @return Minimum profit for a row which is associated w/ a column.
   */
  int get_minassoc_prof();

  /** 
   * Compute the maximum associated price for a column. 
   * @return Maximum profit for a row which is not associated with any column.
   */
  int get_maxunass_prof();

  /** 
   * Equalize the prices of unassigned entries; useful in checking e-CS.
   * @param Shape of payoff matrix.
   */
  void group_price_equalize(AuctShape& Sh);

  /** 
   * Enforce "hidden" bids, which arise from asymmtric problems.  
   * @param Sh Shape of Auction Problem
   * @param Param Auction Parameters
   */
  void hidden_bid(AuctShape& Sh, AuctParm& Param);

  /** 
   * Output associations (scalar). This is relevant for the case of 
   * multiple assignments per row/column.
   * @param Sh Shape of Auction Problem
   * @param i Row in Original Payoff matrix to consider
   * @param j Column in Original Payoff matrix to consider
   * @return Number of associations at (i,j) in AuctPay
   */
  int numAssoc (AuctShape& Sh, int i, int j);
 
  /** 
   * Output assignments (vector, AuctPay based).
   * Return the column index (of AuctPay) of each assignments per 
   * expanded row of the AssocMatrix.  Here, multiple row assignments are
   * treated as separate rows.
   * @param Sh Shape of Auction Problem
   * @param ovec Output vector in which to place column indicies
   */
  void getrowassign (AuctShape& Sh, std::vector<int>& ovec);

  /** 
   * Output assignments (vector, AuctAssoc based).
   * Return the (expanded) column index (of AuctAssoc) of each assignment 
   * per expanded row of the AuctAssoc matrix.  
   * @param Sh Shape of Auction Problem
   * @param ovec Output vector in which to place column indicies
   */
  void getrowassign_id (AuctShape& Sh, std::vector<int>& ovec);

//
// ------------------------------------------------------------------
//
//    Data Members.
//
// ------------------------------------------------------------------
//
  
 private:

  int *prices;  //!< array of prices for each row
  int *profits; //!< array of profits for each column
  
    
};

#endif
