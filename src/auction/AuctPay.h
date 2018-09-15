
#ifndef AuctPay_H
#define AuctPay_H

#include <vector>
#include <algorithm>

class AuctPay {
  friend class AuctAlg;
  

  // ------------------------------------------------------------------
  //
  //	Constructors, Destructors, and Operators.
  //
  // ------------------------------------------------------------------

 public:
 
  /**
   * Constructor: with AuctParm, AuctShape, and sparse vectors (int val).
   *
   * Make sure number of non-zeros is set in shape constructor first,
   * before using this method of instantiating an AuctPay object
   * @param Param Parameters for Auction
   * @param shape Auction "shape", should have number of non-zeros specified
   * @param ind STL vector of column indicies
   * @param vals STL vector of payoff matrix values corrsp. to ind
   * @param rowpt STL vector of indicies to vals vector for each row
   */
 template <class T>
  AuctPay (AuctParm& Param, AuctShape& shape, std::vector<int> ind,
	   std::vector<T> vals, std::vector<int> rowpt) {
    unsigned int nnz = (unsigned int) shape.nnz();
    double res = Param.get_res();
    sparsified = false;

    if (nnz != ind.size() | nnz != vals.size()) { 
      cerr << "Payoff constructor has inconsistent inputs!";
      exit(-1);
    }
    numrows = shape.nrows();
    num_non_zeros = nnz;
    values    = new int [shape.nnz()];
    col_index = new int [shape.nnz()];
    rowptr    = new int [shape.nrows()+1];

    unsigned int i;
    for (i = 0; i < nnz; i++) {
      values[i] = (int) (vals[i]/res);
    }
    copy (ind.begin(), ind.end(), col_index);
    copy (rowpt.begin(), rowpt.end(), rowptr);
  }

  /**
   * Constructor: with AssocShape and Dense vector of vectors of doubles.
   * Vpay is scaled, cast to integer, and sparsified.
   *
   * In this case, the Shape number of non-zeros is set by the constructor.
   * @param Param Parameters for Auction
   * @param shape Auction "shape", should have number of non-zeros specified
   * @param Vpay an STL vector of vectors specifying the payoff matrix.
   */
  template <class T>
  AuctPay (AuctParm& Param, AuctShape &shape, std::vector<std::vector<T> > &Vpay) {

    int nnz = 1;
    std::vector<int> vals, ind, rowpt;
    double res = Param.get_res();
    int i;
    T dont_care=(T)Param.get_dontcare();
    sparsified = true;

    // The Vpay input is unpacked as a (sparse, integer valued) row
    // oriented matrix.  Values equal to the dont_care value are
    // omitted from the matrix
    std::vector<std::vector<T> >::iterator vi;
    std::vector<T>::iterator vj;
    for (vi = Vpay.begin(); vi < Vpay.end(); vi++) {
      rowpt.push_back(nnz);
      for (i = 1, vj = (*vi).begin(); vj < (*vi).end(); i++, vj++) {
	if (*vj != dont_care) {
	  ++nnz;
	  ind.push_back(i);
	  vals.push_back((int)(*vj/res));
	}
      }    
    }
    rowpt.push_back(nnz);

    // Convert to flat arrays.  (needed for MTL constructor)
    shape.set_nnz(nnz);
    numrows = (int) shape.nrows();
    num_non_zeros = (int) nnz;
    values    = new int [shape.nnz()];
    col_index = new int [shape.nnz()];
    rowptr    = new int [shape.nrows()+1];
    std::copy (vals.begin(), vals.end(), values);
    std::copy (ind.begin(), ind.end(), col_index);
    std::copy (rowpt.begin(), rowpt.end(), rowptr);
  }    

  /**
   * Destructor
   */
  ~AuctPay() {
    delete [] values;
    delete [] col_index;
    delete [] rowptr;
  }

  // ------------------------------------------------------------------
  //
  //	Accessor Methods
  //
  // ------------------------------------------------------------------

 public:

  /**
   * get value at index i in the sparse payoff matrix
   * @param i Linear index into sparse payoff matrix
   * @return value at i
   */
  inline int get_value(int i) { return values[i]; }

  /**
   * get column index associated w/ linear index i in the sparse payoff matrix
   * @param i Linear index into sparse payoff matrix
   * @return Column index.
   */
  inline int get_col (int i) { return col_index[i]; }

  /**
   * get linear index associated w/ first non-zero entry on row i
   * @param i Row index into sparse payoff matrix
   * @return Linear index of 1st non-zero entry.
   */
  inline int get_rowptr (int i) { return rowptr[i]; }

  /**
   * get value at index i in the sparse payoff matrix
   * @param i Linear index into sparse payoff matrix
   * @param v value at i
   */
  template <class T>
  inline void set_value (int i, T v) {values[i] = (int)v; }

  /**
   * get column index associated w/ linear index i in the sparse payoff matrix
   * @param i Linear index into sparse payoff matrix
   * @param col Column index.
   */
  inline void set_col   (int i, int col) {col_index[i] = col; }

  /**
   * get linear index of first entry of row i in sparse payoff matrix
   * @param i Row index 
   * @param ptr Linear index of first non-zero value in row i
   */
  inline void set_rowpt (int i, int ptr) {rowptr[i] = ptr; }

  /** 
   * Accessor to flag indicating whether sparsification was necessary.
   * @return True if sparsification was done, false otherwise
   */
  bool dense_to_sparse() {return sparsified;}

 private:

  /** 
   * Accessor to internal value array
   * @return pointer to the value array
   */
  inline int* get_valpt() {return values;}

  /** 
   * Accessor to column indicies associated with each element
   * @return pointer to the column index array
   */
  inline int* get_indpt() {return col_index;}

  /** 
   * Accessor to internal array of indicies to first row element
   * @return pointer to the first row element index array
   */
  inline int* get_rowpt() {return rowptr;}


  // ------------------------------------------------------------------
  //
  //	Action Methods
  //
  // ------------------------------------------------------------------

 public:  

  /** 
   * Compute maximum payoff value in a row. 
   * @param i Row over which to take the maximum
   * @returns Maximum value 
   */
  int get_max_by_row (int i) {
    int rmax = 0;
    for (int j = rowptr[i]; j < rowptr[i+1]; j++)
      if (rmax < values[j]) rmax = values[j];
    return rmax;
  }
    
  /** 
   * Consistency Test.  Test if payoff matrix is consistent with a
   * given shape.
   * @param Sh Shape of the Auction problem
   * @return True if compatible, false if incompatible
   */
  bool is_consistent (AuctShape &Sh) {
    return (numrows == Sh.nrows() && num_non_zeros == Sh.nnz());     
  }


//
// ------------------------------------------------------------------
//
//    Data Members.
//
// ------------------------------------------------------------------
//

 private:

  int *values;     //!< Array of values in sparse matrix
  int *col_index;  //!< Array of columns (corrsp. to values) in sparse matrix
  int *rowptr;     //!< Array of pointers to values associated w/ each row
  int numrows;     //!< Total number of rows
  int num_non_zeros; //!< Number of non-zero entries
  bool sparsified; //!< Boolean flag indicating whether input matrix was dense.

};

#endif

