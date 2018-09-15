

#ifndef AuctParm_H
#define AuctParm_H

class AuctParm {

  // ------------------------------------------------------------------
  //
  //	Constructors, Destructors, and Operators.
  //
  // ------------------------------------------------------------------


 public:
  
  /** 
   * Constructor.  Initialize to default values.
   */
  AuctParm () {
    maxcycles = 1000;
    epsilon = 1;
    resolution = 1;
    MAXINT = 1000000000;
    epsbegin = 5;
    epsfactor   = 5;
    dcare    = 0;
    escale_flag = epsrel = true;
    status_msg_flag = debug_flag = false;
  }

  /** 
   * Destructor. 
   */
  ~AuctParm () { }

  // ------------------------------------------------------------------
  //
  //	Accessor Methods
  //
  // ------------------------------------------------------------------

 public:

  /**
   * Accesses maximum number of auction cycles.
   * @return Max number of Auction cycles
   */
  int get_maxcycles() { return maxcycles; }

  /**
   * Accesses epsilon, the bidding increment.
   * @return Epsilon the bidding increment.
   */
  int get_epsilon() { return epsilon; }

  /**
   * Accesses starting value for epsilon.
   * @return Starting value for epsilon.
   */
  double get_epsstart() { return epsbegin; }

  /**
   * Accesses reduction factor for epsilon
   * @return reduction factor for epsilon
   */
  int get_epsfac  () { return epsfactor; }

  /**
   * Accesses discretization resolution of payoff matrix
   * @return discretization resolution of payoff matrix
   */
  double get_res() { return resolution; }

  /**
   * Accesses don't care value in payoff matrix
   * @return Value in payoff matrix to ignore in sparsification
   */
  int get_dontcare() {return dcare; }

  /**
   * Accesses Maximum possible payoff value (usu. set by largest integer)
   * @return  Maximum possible payoff value (usu. set by largest integer)
   */
  int get_MAXINT() {return MAXINT; }

  /**
   * Accesses Flag for turning run-time messages on
   * @return Flag for turning run-time messages on
   */
  bool MESSG() {return status_msg_flag; }

  /**
   * Accesses Flag for turning debugging messages on
   * @return Flag for turning debugging messages on
   */
  bool DEBUG() {return debug_flag; }

  /**
   * Accesses  Flag for turning epsilon-scaling feature on
   * @return  Flag for turning epsilon-scaling feature on
   */
  bool EScale() {return escale_flag; }

  /**
   * Accesses  Flag indicating scale of epsilon
   * @return True if epsilon=1 is optimal, false if epsilon in Payoff units.
   */
  bool epsrelative() {return epsrel; }
  
  /**
   * Sets maximum number of auction cycles.
   * @param n Max number of Auction cycles
   */
  void set_maxcycles (int n) { maxcycles = n; }

  /**
   * Sets  epsilon, the bidding increment.
   * @param n epsilon, the bidding increment.
   */
  void set_epsilon   (int n) { epsilon   = n; }

  /**
   * Sets starting value for epsilon.
   * @param x Starting value for epsilon.
   */
  void set_epsstart(double x) { epsbegin   = x; }

  /**
   * Sets reduction factor for epsilon
   * @param n reduction factor for epsilon
   */
  void set_epsfac  (int n) { epsfactor   = n; }

  /**
   * Sets discretization resolution of payoff matrix
   * @param x discretization resolution of payoff matrix
   */
  void set_res (double x) { resolution = x;}

  /**
   * Sets don't care value in payoff matrix
   * @param x Value in payoff matrix to ignore in sparsification
   */
  void set_dontcare (int x) { dcare = x; }

  /**
   * Sets Maximum possible payoff value (usu. set by largest integer)
   * @param n Maximum possible payoff value (usu. set by largest integer)
   */
  void set_MAXINT (int n) { MAXINT = n;}

  /**
   * Sets Flag for turning run-time messages on
   * @param flag True if messages are to be printed, false otherwise
   */
  void set_MESSG (bool flag) { status_msg_flag = flag;}

  /**
   * Sets Flag for turning debugging messages on
   * @param flag True if debugging messages are to be printed, false otherwise
   */
  void set_DEBUG (bool flag) { debug_flag = flag;}

  /**
   * Sets Flag for turning epsilon-scaling feature on
   * @param flag True to turn epsilon-scaling feature on
   */
  void set_EScale (bool flag) { escale_flag = flag;
                                if (!flag) {
                                    epsrel = false;
                                    epsbegin = 1;
                                }
                              }
  /**
   * Sets Flag indicating scale of epsilon
   * @return flag True if epsilon=1 is optimal, false if epsilon in Payoff units.
   */
  void set_epsrelative (bool flag) { epsrel = flag; }

//
// ------------------------------------------------------------------
//
//    Data Members.
//
// ------------------------------------------------------------------
//

 private:

  int maxcycles; //!< Maximum number of auction cycles
  int epsilon;  //!< Bidding increment
  int dcare;  //!< Don't care value for sparsification
  int MAXINT;  //!< Maximum possible payoff value
  double resolution;  //!< Discretization resolution for payoff matrix
  double epsbegin;  //!< Beginning value of epsilon
  bool escale_flag;  //!< Flag to turn on epsilon-scaling
  bool debug_flag;  //!< Flag to turn on debugging messages
  bool status_msg_flag;  //!< Flag to turn on status messages
  bool epsrel;  //!< Flag to indicate a problem-relative epsilon (1=optimal)
  int  epsfactor;  //!< Reduction factor for epsilon-scaling
};

#endif
