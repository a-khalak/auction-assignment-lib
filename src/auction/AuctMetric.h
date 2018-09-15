

#ifndef AuctMetric_H
#define AuctMetric_H

class AuctMetric {
  friend class AuctAlg;

  // ------------------------------------------------------------------
  //
  //	Constructors, Destructors, and Operators.
  //
  // ------------------------------------------------------------------

 public:

  /** 
   * Constructor.  Initialize to default values.
   */
  AuctMetric () {
    numcycles = numassoc_byauction = numassoc_total = numassoc_max = 0;
    payoffdensity = 0;
    timedout = re_oriented = sparsification = row_mults = false;
    col_mults = e_scaling = false;
  }

  /** 
   * Destructor.
   */
  ~AuctMetric () { }


  // ------------------------------------------------------------------
  //
  //	Accessor Methods
  //
  // ------------------------------------------------------------------

 public:

  /**
   * Accesses number of auction cycles executed
   * @return Number of auction cycles executed.
   */
  int ncycles() {return numcycles; }

  /**
   * Accesses number of items associated by auction
   * @return number of items associated by auction
   */
  int nauction_assoc() {return numassoc_byauction; }

  /**
   * Accesses total number of items associated (by auction & greedy)
   * @return total number of items associated
   */
  int ntotal_assoc() {return numassoc_total; }

  /**
   * Accesses maximum number of concievable associations.
   * @return Maximum of concievable associations (min of rows/cols)
   */
  int nmaxassoc() {return numassoc_max; }

  /**
   * Accesses density of the sparse payoff matrix
   * @return Density of the payoff matrix (i.e. non-zeros / total )
   */
  double paydensity() {return payoffdensity; }  

  /**
   * Accesses boolean flag indicating a time-out condition
   * @return True if the maximum number of cycles was reached, forcing termination.
   */
  bool timed_out() {return timedout; }

  /**
   * Accesses boolean flag indicating whether a reorientation was needed
   * @return True if payoff matrix was transposed before auction.
   */
  bool reoriented() {return re_oriented; }

  /**
   * Accesses boolean flag indicating whether a sparsification was needed
   * @return True if a dense input matrix was sparsified.
   */
  bool sparsified() {return sparsification; }

  /**
   * Accesses boolean flag indicating whether there were multiple row assocs.
   * @return True if there were multiple row associations.
   */
  bool rowmults() {return row_mults; }

  /**
   * Accesses boolean flag indicating whether there were column associations.
   * @return True if there were multiple column associations.
   */
  bool colmults() {return col_mults; }

  /**
   * Accesses boolean flag indicating whether epsilon-scaling was used
   * @return True if epsilon scaling was used.
   */
  bool escaling() {return e_scaling; }

 private:
  
  /**
   * Sets number of auction cycles executed
   * @param val Number of auction cycles executed.
   */
  void set_ncycles(int val)        { numcycles               = val; }

  /**
   * Sets number of items associated by auction
   * @param val number of items associated by auction
   */
  void set_nauction_assoc(int val) { numassoc_byauction      = val; }

  /**
   * Sets total number of items associated (by auction & greedy)
   * @param val total number of items associated
   */ 
  void set_ntotal_assoc(int val)   { numassoc_total          = val; }

  /**
   * Sets maximum number of concievable associations.
   * @param val Maximum of concievable associations (min of rows/cols)
   */ 
  void set_nmaxassoc(int val)      { numassoc_max            = val; }

  /**
   * Sets density of the sparse payoff matrix
   * @param val Density of the payoff matrix (i.e. non-zeros / total )
   */
  void set_paydensity(double val)  { payoffdensity           = val; }  

  /**
   * Sets boolean flag indicating a time-out condition
   * @param flag True if the maximum number of cycles was reached.
   */
  void set_timed_out(bool flag)    { timedout                = flag; }

  /**
   * Sets boolean flag indicating whether a reorientation was needed
   * @param flag True if payoff matrix was transposed before auction.
   */
  void set_reoriented(bool flag)   { re_oriented             = flag; }

  /**
   * Sets boolean flag indicating whether a sparsification was needed
   * @param flag True if a dense input matrix was sparsified.
   */
  void set_sparsified(bool flag)   { sparsification          = flag; }

  /**
   * Sets boolean flag indicating whether there were multiple row assocs.
   * @param flag True if there were multiple row associations.
   */
  void set_rowmults(bool flag)     { row_mults               = flag; }

  /**
   * Sets boolean flag indicating whether there were column associations.
   * @param flag True if there were multiple column associations.
   */
  void set_colmults(bool flag)     { col_mults               = flag; }

  /**
   * Sets boolean flag indicating whether epsilon-scaling was used
   * @param flag True if epsilon scaling was used.
   */
  void set_escaling(bool flag)     { e_scaling               = flag; }

//
// ------------------------------------------------------------------
//
//    Action Members.
//
// ------------------------------------------------------------------
//


 public:

  /**
   * Computes a boolean flag indicating whether the result is optimal
   * @return True if the result is optimal
   */
  bool isoptimal() {return (numassoc_max == numassoc_byauction); }

  /**
   * Computes a boolean flag indicating whether the problem was feasible
   * @return True if the problem was feasible
   */
  bool isfeasible() {return (numassoc_max == numassoc_total); }


//
// ------------------------------------------------------------------
//
//    Data Members.
//
// ------------------------------------------------------------------
//

 private:
  int numcycles; //!< Number of cycles run in auction 
  int numassoc_byauction; //!< Number of objects associated in auction
  int numassoc_total; //!< Number of objects associated, total
  int numassoc_max; //!< Maximum concievable number of associations
  double payoffdensity; //!< Density of the payoff matrix
  bool timedout; //!< Did the auction time out?
  bool re_oriented; //!< Was the payoff matrix transposed before the auction?
  bool sparsification; //!< Was the payoff matrix sparsified from a dense input?
  bool row_mults; //!< Were there multiple row associations?
  bool col_mults; //!< Were there multiple column associations?
  bool e_scaling; //!< Was epsilon-scaling employed?

  
};

#endif
