#define A_STD

#include <algorithm>
#include "AssocMatrix.h"

// constructors for symmetric association
AssocMatrix::AssocMatrix(int N) {
  Nrows = N;
  Ncols = N;
  Nnz = 0;
  rowassign = new int[N];
  colassign = new int[N];
  A_STD fill (rowassign, rowassign+N, -1);
  A_STD fill (colassign, colassign+N, -1);
}

AssocMatrix::AssocMatrix(int N, int M) {
  Nrows = N;
  Ncols = M;
  Nnz = 0;
  rowassign = new int[Ncols];
  colassign = new int[Nrows];
  A_STD fill (rowassign, rowassign+Ncols, -1);
  A_STD fill (colassign, colassign+Nrows, -1);
}

// destructor
AssocMatrix::~AssocMatrix() {
  delete [] colassign;
  delete [] rowassign;
}

// look up lists of columns association with each row
void AssocMatrix::get_columns(int *column_by_row) {
  A_STD copy(colassign, colassign+Nrows, column_by_row);
}

// look up lists of rows associated w/ each column
void AssocMatrix::get_rows(int *row_by_column) {
  A_STD copy(rowassign, rowassign+Ncols, row_by_column);
}

// Add an entry to the assocmatrix, clearing other entries if necessary
int AssocMatrix::add (int row, int col) {
    if ((row >= 0) && (row < Nrows) && (col >= 0) && (col < Ncols)){
      clear_pair (row, col);
      colassign[row] = col;
      rowassign[col] = row;
      Nnz++;
      return 0;
    } else return -1;
  };


// access number of associations in a subblock
int AssocMatrix::blockval (int startrow, int endrow,
			    int startcol, int endcol) {
  int val=0, i;

  if (endrow<startrow || endcol<startcol) return 0;
  for (i = startrow; i<endrow; i++)
    val += (col(i)>=startcol && col(i)<endcol) ? 1 : 0;
  return val;
}


void AssocMatrix::transpose() {
  int temp1  = Ncols;
  Ncols      = Nrows;
  Nrows      = temp1;

  int* temp2 = rowassign;
  rowassign  = colassign;
  colassign  = temp2;
};

void AssocMatrix::blank() {
  int j;
    for (j=0; j<Ncols; j++) rowassign[j] = -1;
    for (j=0; j<Nrows; j++) colassign[j] = -1;
    Nnz = 0;
};

void AssocMatrix::clear_pair (int in_row, int in_col) {
  int tmp_col, tmp_row, o_row, o_col;
  tmp_row = rowassign[in_col];
  tmp_col = colassign[in_row];

  if ((tmp_row == -1) && (tmp_col == -1)) {                   // Case 1
  } else if ((tmp_row == in_row) && (tmp_col == in_col)) {    // Case 2
    rowassign[in_col]  = -1;
    colassign[in_row]  = -1;
    Nnz -= 1;
    } else if ((tmp_row == -1) && (tmp_col != -1)) {            // Case 3
      o_row = rowassign[tmp_col];
      rowassign[tmp_col] = -1;
      colassign[o_row]   = -1;
      Nnz -= 1;
    } else if ((tmp_row != -1) && (tmp_col == -1)) {            // Case 4
      o_col = colassign[tmp_row];
      rowassign[o_col]   = -1;
      colassign[tmp_row] = -1;
      Nnz -= 1;
    } else if ((tmp_row != -1) && (tmp_col != -1)){            // Case 5
      rowassign[in_row]  = -1;
      colassign[in_col]  = -1;
      rowassign[tmp_row] = -1;
      colassign[tmp_col] = -1;
      Nnz -= 2;
    }
}; // end clear pair
