

#ifndef AuctShape_H
#define AuctShape_H

#include<vector>

class AuctShape {

  // ------------------------------------------------------------------
  //
  //	Constructors, Destructors, and Operators.
  //
  // ------------------------------------------------------------------

 public:

  /** 
   * Constructor with row size, column size, and number of non-zeros. 
   * @param nrows Number of Rows in the payoff matrix
   * @param mcols Number of Columns in the payoff matrix
   * @param num_non_zero Number of Non-zero elements in the payoff matrix
   */
  AuctShape (int nrows, int mcols, int num_non_zero) {
    n = nrows;
    m = mcols;
    Nnz = num_non_zero;
    rowmult = new int [n];
    colmult = new int [m];
    rowptr = new int[n+1];
    colptr = new int[m+1];
    int i;
    for (i = 0; i < n; i++) rowmult[i] = 1;
    for (i = 0; i < m; i++) colmult[i] = 1;
    computemaps();
  };


  /** 
   * Construct with a column-oriented vector of vectors.  This may be any 
   * data type which may be cast to integers.  The result will be discretized,
   * sparsified, and scaled.
   * @param Vpay An STL vector of vectors from which to extract the payoff. 
   */
  template <class T> 
  AuctShape (std::vector<std::vector<T> > &Vpay) {
    Nnz = 0; // needs to be set later, probably in the AssocPay constructor
    n = Vpay.size();
    m = 0;
	int i=0;
    for (i = 0; i < n; i++) {
      if (Vpay[i].size() > (unsigned int)m) m = Vpay[i].size();
    }

    rowmult = new int [n];
    colmult = new int [m];
    rowptr = new int[n+1];
    colptr = new int[m+1];
    for (i = 0; i < n; i++) rowmult[i] = 1;
    for (i = 0; i < m; i++) colmult[i] = 1;
    computemaps();
  };

  /**
   * Destructor
   */
  ~AuctShape () {
    delete [] rowmult;
    delete [] colmult;
    delete [] rowptr;
    delete [] colptr;
  };


  // ------------------------------------------------------------------
  //
  //	Accessor Methods
  //
  // ------------------------------------------------------------------

  // accessor methods.  The maps are recomputed automatically when
  // needed.  It can still screw this up, if the user sets rowmult
  // and then sets rowmap without a recompute in between.  However,
  // this is an odd usage which is probably not worth the overhead
  // needed to automatically account for it

  /**
   * Accessor for the number of rows.
   * @return Number of rows
   */
  int nrows() { return n;}

  /**
   * Accessor for the number of columns.
   * @return Number of columns.
   */
  int ncols() { return m;}

  /**
   * Accessor for the number of non-zero elements.
   * @return Number of non-zero elements.
   */
  int nnz() {return Nnz;}

  /**
   * Accessor for the number of potential row associations.
   * @return Number of potential row associations.
   */
  int nfullrows()        {ifcompute(); return rowptr[n];}

  /**
   * Accessor for the number of potential column associations.
   * @return Number of potential column associations.
   */
  int nfullcols()        {ifcompute(); return colptr[m];}

  /**
   * Accessor for the index of first corresponding entry in expanded row 
   * array for a given row.  
   * @param i The row (from the Payoff matrix) to look up
   * @return First corrsponding entry in the expanded row array
   */
  inline int get_rowptr(int i)  {
    return (i >= 0 && i < n+1) ? rowptr[i] : -1; }

  /**
   * Accessor for the index of first corresponding entry in expanded column 
   * array for a given column.  
   * @param i The column (from the Payoff matrix) to look up
   * @return First corrsponding entry in the expanded column array
   */
  inline int get_colptr(int i)  {			 
    return (i >= 0 && i < m+1) ? colptr[i] : -1; }

  /**
   * Accessor for the number of associations for a given row in Payoff matrix
   * @param i The row from the Payoff Matrix to look up
   * @return Number of associations to this row
   */
  inline int get_rowmult(int i) {	     		 
    return (i >= 0 && i < n) ? rowmult[i]: -1; }

  /**
   * Accessor for the number of associations for a given col in Payoff matrix
   * @param i The column from the Payoff Matrix to look up
   * @return Number of associations to this column
   */
  inline int get_colmult(int i) {	     		 
    return (i >= 0 && i < m) ? colmult[i]: -1; }

  /**
   * Accessor to the corresponding Payoff row for a given entry in 
   * expanded row array.
   * @param i Index to expanded row array (of row associations)
   * @return Corresponding row to Payoff matrix
   */
  inline int get_rowmap(int i)  {	     		 
    return (i >= 0 && i < rowptr[n]) ? rowmap[i] : -1; }

  /**
   * Accessor to the corresponding Payoff column for a given entry in 
   * expanded column array.
   * @param i Index to expanded column array (of column associations)
   * @return Corresponding column to Payoff matrix
   */
  inline int get_colmap(int i)  {	     		 
    return (i >= 0 && i < colptr[m]) ? colmap[i] : -1; }


  /**
   * Set the number of non-zero entries
   * @param nnz number of non zeros
   */
  void set_nnz(int nnz) {
    Nnz = nnz;
  }

  /**
   * Set a payoff row to have a given number of associations.
   * @param i Row in payoff matrix
   * @param mlt Number of associations to this row.
   */
  void set_rowmult(int i, int mlt=1) {
    rowmult[i] = mlt;
    compute_flag = 1;
  }

  /**
   * Set a payoff column to have a given number of associations.
   * @param i Column in payoff matrix
   * @param mlt Number of associations to this column.
   */
  void set_colmult(int i, int mlt=1) {
    colmult[i] = mlt;
    compute_flag = 1;
  }

  /**
   * Set an index in expanded row array to correspond to a specific row.
   * @param i Element in row association array
   * @param mlt Number of associations to this row.
   */
  void set_rowmap(int i, int mp) {
    rowmap[i] = mp;
    compute_flag = 2;
  }

  /**
   * Set an index in expanded row array to correspond to a specific row.
   * @param i Element in column association array.
   * @param mlt Number of associations to this column.
   */
  void set_colmap(int i, int mp) {
    colmap[i] = mp;
    compute_flag = 2;
  }

  // ------------------------------------------------------------------
  //
  //	Accessor Methods
  //
  // ------------------------------------------------------------------

  /**
   * Reset all the rows to have a single association.
   */
  void blank_rowmult() {
    int i;
    compute_flag = 1;
    for (i = 0; i < n; i++) rowmult[i] = 1;
  }

  /**
   * Reset all the columns to have a single association.
   */
  void blank_colmult() {
    int i;
    compute_flag = 1;
    for (i = 0; i < m; i++) colmult[i] = 1;
  }

    
  /**
   * Recompute indicies based on mappings of row associations to rows
   */
  void computemaps() {
    rowmap.clear(); colmap.clear();
    rowptr[0] = 0; colptr[0] = 0;
    int i=0,j=0;
    for (i=0; i<n; i++) {
      for (j=0; j<rowmult[i]; j++) rowmap.push_back(i);
      rowptr[i+1] = rowmap.size();
    }
    for (i=0; i<m; i++) {
      for (j=0; j<colmult[i]; j++) colmap.push_back(i);
      colptr[i+1] = colmap.size();
    }
    compute_flag = 0;
  }

  /**
   * Recompute indicies based on number of row associations for each
   * row.  This is expected to be the more common way to go.
   */
  void computemult() {
    int i;
    for (i = 0; i < n; i++) rowmult[i] = 0;
    for (i = 0; i < m; i++) colmult[i] = 0;
    for (i = 0; i < (int)rowmap.size(); i++) rowmult[rowmap[i]]++;
    for (i = 0; i < (int)colmap.size(); i++) colmult[colmap[i]]++;
    rowptr[0] = 0; colptr[0] = 0;
    for (i = 0; i < n; i++) rowptr[i+1] = rowptr[i] + rowmult[i];  
    for (i = 0; i < m; i++) colptr[i+1] = colptr[i] + colmult[i];  
    compute_flag = 0;
  }
  
  /** 
   * Automatic computation, in the sense to preserve the last change
   */
  inline void ifcompute() {
    if (compute_flag == 1) computemaps();
    else if (compute_flag == 2) computemult();
  }

  /** 
   * Transpose. 
   */
  void transpose() {
    int* temp;    

    std::swap (n,m);
    temp = rowmult;  rowmult = colmult;  colmult = temp;
    temp = rowptr;  rowptr = colptr;  colptr = temp;
    rowmap.swap(colmap);
  }

//
// ------------------------------------------------------------------
//
//    Data Members.
//
// ------------------------------------------------------------------
//


 protected:
  int n;  //!< Number of rows in payoff
  int m;  //!< Number of columns in payoff
  int Nnz; //!< Number of non-zero entries
  int compute_flag; //!< 1 to recompute based on maps, 2 based on mults.
  std::vector<int> rowmap; //!< Mapping from row associations to rows
  std::vector<int> colmap; //!< Mapping from column associations to columns

 private:
  int *rowmult; //!< Array of row multiples (i.e. number of associations)
  int *colmult; //!< Array of col multiples (i.e. number of associations)
  int *rowptr;  //!< Array of row pointers, index to first association given row
  int *colptr;  //!< Array of col multiples, index to first association given col

};

#endif
