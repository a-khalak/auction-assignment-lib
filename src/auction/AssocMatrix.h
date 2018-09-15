
#ifndef AssocMatrix_H
#define AssocMatrix_H

class AssocMatrix {
  
  // ------------------------------------------------------------------
  //
  //	Constructors, Destructors, and Operators.
  //
  // ------------------------------------------------------------------

 public:

  /** 
   * Constructor for symmetric association matrix.
   * @param N Number of rows/columns in Association Matrix
   */
  AssocMatrix(int N);

  /** 
   * Constructor for asymmetric association matrix.
   * @param N Number of rows in Association Matrix
   * @param M Number of columns in Association Matrix
   */
  AssocMatrix(int N, int M);

  /** Association Matrix Destructor */
  virtual ~AssocMatrix();

  // ------------------------------------------------------------------
  //
  //	Accessor Methods
  //
  // ------------------------------------------------------------------

 public:

  /**
   * Accesses rows
   * @return Number of rows in AssocMatrix
   */
  int nrows() {return Nrows;};

  /**
   * Accesses columns. 
   * @return Number of columns in AssocMatrix
   */
  int ncols() {return Ncols;};

  /**
   * Accesses associations 
   * @return Number of filled entries in AssocMatrix
   */
  int nassoc() {return Nnz;};

  /**
   * Lookup rows from columns (vector)
   * @param column_by_row Pointer to array of column indicies, for each row
   */
  void get_columns(int* column_by_row);

  /**
   * Lookup columns from rows (vector)
   * @param row_by_column Pointer to array of row indicies, for each column
   */
  void get_rows(int* row_by_column);
  
  /**
   * Lookup rows from columns (scalar)
   * @param col Column to look up
   * @return Row corresponding to input column, or -1 if unassigned 
   */
  inline int row (int col) {
    return ((col >= 0) && (col < Ncols)) ? rowassign[col] : -1; };

  /**
   * Lookup columns from rows (scalar)
   * @param row Row to look up
   * @return Column corresponding to input row, or -1 if unassigned 
   */
  inline int col (int row) {
    return ((row >= 0) && (row < Nrows)) ? colassign[row] : -1; };
  
  // ------------------------------------------------------------------
  //
  //	Action Methods
  //
  // ------------------------------------------------------------------

 public:

  /**
   * Add an element to the association matrix. 
   * Existing entries w/ the same row or the same column are cleared,
   * if necessary.
   * @param row Row to add
   * @param col Column to add
   * @return A flag which is 0 if (row,col) pair is feasible, -1 otherwise
   */
  int add (int row, int col);

  /**
   * Get value.
   * Lookup the value of a (row, col) pair.
   * @param r Row to look up.
   * @param c Column to look up.
   * @return Value of matrix: 1 if there is an assignment, 0 otherwise
   */
  inline int val (int r, int c) {
    return ((row(c) == r) && (col(r) == c)) ? 1 : 0; };

  /**
   * Get the total number of associated pairs in a subblock. 
   * The subblock represents the entries within a given range of rows, and
   * within a given range of columns.
   * @param startrow Beginning row of the subblock
   * @param endrow Ending row of the subblock
   * @param startcol Beginning column of the subblock
   * @param endcol Ending column of the subblock
   * @return Sum of associations within this subblock
   */
  int blockval (int startrow, int endrow, int startcol, int endcol);

  /** Transpose the matrix */
  void transpose();

  /** Unassociate all the elements.*/
  void blank();

 protected:

  /**
   * Delete a row and column from the matrix.  
   * We exploit that the association matrix is stored as two vectors (one
   * of column indicies, and one of row indicies).  Instead of clearing 
   * N+M entries (i.e. a linear time operation), this special data structure
   * allows the deletion to be performed in constant time.  There are five
   * special cases which must be considered.  In case either the row or
   * column are already assigned, the corresponding assignment must be looked
   * up and cleared: <p>
   * First, both row and column indices may be completely unassigned <br>
   * Second, there is an entry exactly for the row and col specified <br>
   * Third, the row has a different entry, but column is unassigned <br> 
   * Fourth, the column has a different entry, but row is unassigned <br> 
   * Fifth,  the row and column both have entries different from that provided
   * @param in_row Row to clear of associations
   * @param in_col Column to clear of associations
   */
  void clear_pair (int in_row, int in_col);


//
// ------------------------------------------------------------------
//
//    Data Members.
//
// ------------------------------------------------------------------
//

 protected:
  int Nrows; //!< Number of Rows in the AssocMatrix
  int Ncols; //!< Number of Columns in the AssocMatrix
  int Nnz;   //!< Number of Non-zeros in the AssocMatrix


 private:
  int *rowassign; //!< stores the column indicies of each row
  int *colassign; //!< stores the row indicies of each column  
    
}; 


#endif
