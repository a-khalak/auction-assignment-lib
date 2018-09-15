
#include <vector>
#include <algorithm>
#include <functional>
#include "AuctParm.h"
#include "AuctShape.h"
#include "AuctAssoc.h"

AuctAssoc::AuctAssoc(int N) : AssocMatrix::AssocMatrix(N) {
  prices = new int[N];
  std::fill (prices, prices+N, 0);
  profits = new int[N];
  std::fill (profits, profits+N, 0);
}

AuctAssoc::AuctAssoc(int N, int M) : AssocMatrix::AssocMatrix(N,M) {
  prices = new int[Ncols];
  std::fill (prices, prices+Ncols, 0);
  profits = new int[Nrows];
  std::fill (profits, profits+Nrows, 0);
}

AuctAssoc::AuctAssoc (AuctShape& S) :
  AssocMatrix::AssocMatrix(S.nfullrows(), S.nfullcols()) {
  prices = new int[Ncols];
  std::fill (prices, prices+Ncols, 0);
  profits = new int[Nrows];
  std::fill (profits, profits+Nrows, 0);
}

AuctAssoc::~AuctAssoc() {
  delete [] prices;
  delete [] profits;
}


void AuctAssoc::transpose() {
  AssocMatrix::transpose();
  int* temp;
  temp = prices; prices = profits; profits = temp;
}

int AuctAssoc::min_price () {
  int *mpir;
  mpir = std::min_element (prices, prices + Ncols);
  return *mpir;
}

int AuctAssoc::max_price_in_colrange (int first, int last) {
  int *mpir;
  mpir = std::max_element(prices+first, prices+last);
  return *mpir;
}

int AuctAssoc::min_price_in_colrange (int first, int last) {
  int *mpir;
  mpir = std::min_element(prices+first, prices+last);
  return *mpir;
}

int AuctAssoc::max_price_in_rowrange (int first, int last) {
  int mpir=0, col;
  for (int j=first; j<last; j++) {
    col = AssocMatrix::col(j);
    if (col >= 0)
      if (prices[col] > mpir) mpir = prices[col];
  }
  return mpir;
}

int AuctAssoc::min_price_in_rowrange (int first, int last) {
  int mpir=0, col;
  bool start = true;
  for (int j=first; j<last; j++) {
    col = AssocMatrix::col(j);
    if (col >= 0)
      if (prices[col] > mpir || start) {
	start = false;
	mpir = prices[col];
      }
  }
  return mpir;
}

int AuctAssoc::get_minassoc() {
  bool start = true;
  int col, minassoc = 0;

  for (int i = 0; i<Nrows; i++) {
    col = AssocMatrix::col(i);
    if (col >= 0)
      if (start || prices[col] < minassoc) {
	start = false;
	minassoc = prices[col];
      }
  }
  return minassoc;
}

int AuctAssoc::get_maxunass() {
  bool start = true;
  int maxunass = 0;

  for (int i = 0; i<Ncols; i++)
    if (AssocMatrix::row(i) < 0)
      if (start || prices[i] > maxunass) {
	start = false;
	maxunass = prices[i];
      }
  return maxunass;
}

int AuctAssoc::get_minassoc_prof() {
  bool start = true;
  int minassoc = 0;

  for (int i = 0; i<Nrows; i++) {
    if (AssocMatrix::col(i) >= 0)
      if (start || profits[i] < minassoc) {
	start = false;
	minassoc = profits[i];
      }
  }
  return minassoc;
}

int AuctAssoc::get_maxunass_prof() {
  bool start = true;
  int maxunass = 0;

  for (int i = 0; i<Nrows; i++)
    if (AssocMatrix::col(i) < 0)
      if (start || profits[i] > maxunass) {
	start = false;
	maxunass = profits[i];
      }
  return maxunass;
}



template <class T>
class IndVal {
 public:
  T value;
  int index;
  IndVal (T v, int i) { value = v; index = i; }
  bool operator<(const IndVal &x) const {return (value < x.value);}
  bool operator>(const IndVal &x) const {return (value > x.value);}
};

void AuctAssoc::hidden_bid(AuctShape& Sh, AuctParm& Param) {
  std::vector <IndVal<int> > assocprice, unassocprice;
  int i, rowa, cola;
  int minassoc=Param.get_MAXINT(), maxunass=0;

  // separate Prices into Associated, and unassociated and sort.
  for (i = 0; i < Ncols; i++) {
    if (AssocMatrix::row(i)>=0) {
      assocprice.push_back(IndVal<int>(prices[i],i));
      minassoc = (minassoc < prices[i]) ? minassoc: prices[i];
    } else {
      unassocprice.push_back(IndVal<int>(prices[i],i));
      maxunass = (maxunass < prices[i]) ? prices[i] : maxunass;
    }
  }

  int nbids = Ncols - Nrows;
  nbids = std:: min (nbids, (int)assocprice.size());
  nbids = std:: min (nbids, (int)unassocprice.size());

  if ((nbids > 0) && (minassoc < maxunass)) {

    // The associated are in acending and unassociated in decending order.
    std::partial_sort(assocprice.begin(), assocprice.end(),
         assocprice.begin() + nbids, std::less<IndVal<int> > ());
    std::partial_sort(unassocprice.begin(), unassocprice.end(),
         unassocprice.begin() + nbids, std::greater<IndVal<int> > ());

    for(i=0; i < nbids && assocprice[i].value < unassocprice[i].value; i++) {
      cola = assocprice[i].index;
      rowa = AssocMatrix::row(cola);
      AssocMatrix::clear_pair(rowa, cola);
      minassoc = assocprice[i].value;
    }

    for (unsigned int j = abs(i); j < unassocprice.size(); j++) {
      prices[unassocprice[j].index] = minassoc;
    }

  } else {
    for (i=0; i<(int)unassocprice.size(); i++) {
      prices[unassocprice[i].index] = minassoc - 1;
    }
  }
}

// A method to reset the prices to the max within a row group
void AuctAssoc::group_price_equalize(AuctShape& Sh) {
  int n = Sh.nrows();
  int mpg, col;

  for (int i = 0; i<n; i++) {
    mpg = max_price_in_rowrange (Sh.get_rowptr(i), Sh.get_rowptr(i+1));
    for (int j = Sh.get_rowptr(i); j < Sh.get_rowptr(i+1); j++) {
      col = AssocMatrix::col(j);
      if (col >= 0) prices[col] = mpg;
    }
  }
}


// Get the number of associations associated with (i,j) of the AuctPay matrix
int AuctAssoc::numAssoc (AuctShape& Sh, int i, int j) {
  return AssocMatrix::blockval (Sh.get_rowptr(i), Sh.get_rowptr(i+1),
				Sh.get_colptr(j), Sh.get_colptr(j+1));
}


// Get the compressed column indicies associated with each expanded row
void AuctAssoc::getrowassign (AuctShape& Sh, std::vector<int>& ovec) {
    ovec.clear();
    for (int i = 0; i < Nrows; i++)
      ovec.push_back( Sh.get_colmap( col(i) ) );
  }

// Output assignments: rows of AssocMatrix --> columns of AssocMatrix
void AuctAssoc::getrowassign_id (AuctShape& Sh, std::vector<int>& ovec) {
  ovec.clear();
    for (int i = 0; i < Nrows; i++)
      ovec.push_back( col(i) );
}
