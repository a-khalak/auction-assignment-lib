//
// Copyright 1997, 1998, 1999 University of Notre Dame.
// Authors: Andrew Lumsdaine, Jeremy G. Siek, Lie-Quan Lee
//
// This file is part of the Matrix Template Library
//
// You should have received a copy of the License Agreement for the
// Matrix Template Library along with the software;  see the
// file LICENSE.  If not, contact Office of Research, University of Notre
// Dame, Notre Dame, IN  46556.
//
// Permission to modify the code and to distribute modified code is
// granted, provided the text of this NOTICE is retained, a notice that
// the code was modified is included with the above COPYRIGHT NOTICE and
// with the COPYRIGHT NOTICE in the LICENSE file, and that the LICENSE
// file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
//

#ifndef LAPACK_INTERFACE_H
#define LAPACK_INTERFACE_H

#include "mtl/mtl_complex.h"



extern "C" {

  void dgecon_ (const char & norm, const int & n, const double da[],
		const int & lda, const double & danorm,
		double & drcond, double dwork[],
		int iwork2[], int & info);

  void sgecon_ (const char & norm, const int & n,
		const float sa[], const int & lda,
		const float & sanorm, float & srcond,
		float swork[], int iwork2[],
		int & info);

  void zgecon_ (const char & norm, const int & n,
		const std::complex<double> za[], const int & lda,
		const double & danorm, double & drcond,
		std::complex<double> zwork[], double dwork2[],
		int & info);

  void cgecon_ (const char& norm, const int& n,
		const std::complex<float> sa[], const int& lda,
		const float & danorm, float& srcond,
		std::complex<float> cwork[],	float swork2[],
		int& info);


  void dgeev_(const char& jobvl, const char& jobvr,
	      const int& n, double da[],
	      const int& lda, double dwr[],
	      double dwi[], double dvl[],
	      const int& ldvl, double dvr[],
	      const int& ldvr, double dwork[],
	      const int& ldwork, int& info);

  void sgeev_(const char& jobvl, const char& jobvr,
	      const int&  n, float sa[],
	      const int& lda, float swr[],
	      float swi[], float svl[],
	      const int& ldvl, float svr[],
	      const int& ldvr, float swork[],
	      const int& ldwork, int& info);

  void zgeev_(const char& jobvl, const char& jobvr,
	      const int&  n, std::complex<double> za[],
	      const int& lda, std::complex<double> zw[],
	      std::complex<double> zvl[], const int& ldvl,
	      std::complex<double> zvr[], const int& ldvr,
	      std::complex<double> zwork[], const int& ldwork,
	      double dwork2[], int& info);

  void cgeev_(const char& jobvl, const char& jobvr,
	      const int&  n, std::complex<float> ca[],
	      const int& lda, std::complex<float> cw[],
	      std::complex<float> cvl[], const int& ldvl,
	      std::complex<float> cvr[], const int& ldvr,
	      std::complex<float> cwork[], const int& ldwork,
	      float swork2[], int& info);

  void dgeqpf_(const int & m, const int & n,
	       double da[], const int & lda,
	       int jpivot[], double dtau[],
	       double dwork[], int & info);

  void sgeqpf_(const int & m, const int & n,
	       float sa[], const int & lda,
	       int jpivot[], float stau[],
	       float swork[], int & info);

  void zgeqpf_(const int & m,
	       const int & n,
	       std::complex<double> za[],
	       const int & lda,
	       int jpivot[],
	       std::complex<double> ztau[],
	       std::complex<double> zwork[],
	       double dwork2[],
	       int & info);

  void cgeqpf_(const int & m,
	       const int & n,
	       std::complex<float> ca[],
	       const int & lda,
	       int jpivot[],
	       std::complex<float> ctau[],
	       std::complex<float> cwork[],
	       float swork2[],
	       int& info);

  void dgeqrf_(const int & m,
	       const int & n,
	       double da[],
	       const int & lda,
	       double dtau[],
	       double dwork[],
	       const int& ldwork,
	       int& info);

  void sgeqrf_(const int & m,
	       const int & n,
	       float sa[],
	       const int & lda,
	       float stau[],
	       float swork[],
	       const int& ldwork,
	       int& info);

  void zgeqrf_(const int & m,
	       const int & n,
	       std::complex<double> za[],
	       const int & lda,
	       std::complex<double> ztau[],
	       std::complex<double> zwork[],
	       const int& ldwork,
	       int & info);

  void cgeqrf_(const int & m,
	       const int & n,
	       std::complex<float> ca[],
	       const int & lda,
	       std::complex<float> ctau[],
	       std::complex<float> cwork[],
	       const int& ldwork,
	       int & info);

  void dgesv_(const int & n,
	      const int & nrhs,
	      double da[],
	      const int & lda,
	      int ipivot[],
	      double db[],
	      const int & ldb,
	      int & info);

  void sgesv_(const int & n,
	      const int & nrhs,
	      float sa[],
	      const int & lda,
	      int ipivot[],
	      float sb[],
	      const int & ldb,
	      int & info);

  void zgesv_(const int & n,
	      const int & nrhs,
	      std::complex<double> za[],
	      const int & lda,
	      int ipivot[],
	      std::complex<double> zb[],
	      const int & ldb,
	      int & info);

  void cgesv_(const int & n,
	      const int & nrhs,
	      std::complex<float> ca[],
	      const int & lda,
	      int ipivot[],
	      std::complex<float> cb[],
	      const int & ldb,
	      int & info);

 void dgetrf_ (const int& m,
	       const int& n,
	       double da[],
	       const int& lda,
	       int ipivot[],
	       int& info);

 void sgetrf_ (const int& m,
	       const int& n,
	       float sa[],
	       const int& lda,
	       int ipivot[],
	       int& info);

 void zgetrf_ (const int& m,
	       const int& n,
	       std::complex<double> za[],
	       const int& lda,
	       int ipivot[],
	       int& info);

 void cgetrf_ (const int& m,
	       const int& n,
	       std::complex<float> ca[],
	       const int& lda,
	       int ipivot[],
	       int& info);
   
   
 void dgetrs_ (const char& transa,
	       const int& n,
	       const int& nrhs,
	       const double da[],
	       const int& lda,
	       int ipivot[],
	       double db[],
	       const int& ldb,
	       int& info);
 
 void sgetrs_ (const char& transa,
	       const int& n,
	       const int& nrhs,
	       const float  sa[],
	       const int& lda,
	       int ipivot[],
	       float sb[],
	       const int& ldb,
	       int& info);

 void zgetrs_ (const char& transa,
	       const int& n,
	       const int& nrhs,
	       const std::complex<double> za[],
	       const int& lda,
	       int ipivot[],
	       std::complex<double> zb[],
	       const int& ldb,
	       int& info);

void cgetrs_ (const char& transa,
	      const int& n,
	      const int& nrhs,
	      const std::complex<float> ca[],
	      const int& lda,
	      int ipivot[],
	      std::complex<float> cb[],
	      const int& ldb,
	      int& info);

void dgeequ_ (const int& m, const int& n, const double da[], const int& lda,
	      double r[], double c[], double& rowcnd, double& colcnd,
	      double& amax, int& info);

void sgeequ_ (const int& m, const int& n, const float da[], const int& lda,
	      float r[], float c[], float& rowcnd, float& colcnd,
	      float& amax, int& info);

void cgeequ_ (const int& m, const int& n, const std::complex<float> da[],
	      const int& lda, std::complex<float> r[], std::complex<float> c[],
	      float& rowcnd, float& colcnd,
	      float& amax, int& info);

void zgeequ_ (const int& m, const int& n, const std::complex<double> da[],
	      const int& lda, std::complex<double> r[], std::complex<double> c[],
	      double& rowcnd, double& colcnd,
	      double& amax, int& info);



void dgelqf_ (const int& m, const int& n, double da[], const int& lda,
	      double dtau[], double work[], const int& ldwork, int& info);

void sgelqf_ (const int& m, const int& n, float da[], const int& lda,
	      float dtau[], float work[], const int& ldwork, int& info);

void cgelqf_ (const int& m, const int& n, std::complex<float> da[], const int& lda,
	      std::complex<float> dtau[], std::complex<float> work[], 
	      const int& ldwork, int& info);

void zgelqf_ (const int& m, const int& n, std::complex<double> da[], const int& lda,
	      std::complex<double> dtau[], std::complex<double> work[],
	      const int& ldwork, int& info);



void dorglq_(const int& m, const int& n, const int& k,
	     double da[], int& lda, double dtau[],
	     double dwork[], const int& ldwork, int& info);

void sorglq_(const int& m, const int& n, const int& k,
	     float da[], int& lda, float dtau[],
	     float dwork[], const int& ldwork, int& info);


void dorgqr_(const int& m, const int& n, const int& k,
	     double da[], const int& lda, double dtau[], 
	     double dwork[], const int& ldwork, int& info);

void sorgqr_(const int& m, const int& n, const int& k,
	     float da[], const int& lda, float dtau[], 
	     float dwork[], const int& ldwork, int& info);



 void dgeevx_ (const char & balanc, //IN
	       const char & jobvl,  //IN
	       const char & jobvr,  //IN
	       const char & sense,  //IN
	       const int & n,       //IN
	       double da[],         //IN
	       const int & lda,     //IN
	       double dwr[],        //OUT
	       double dwi[],        //OUT
	       double dvl[],        //OUT
	       const int & ldvl,    //IN
	       double dvr[],        //OUT
	       const int & ldvr,    //IN
	       int & ilo,           //OUT
	       int & ihi,           //OUT
	       double dscale[],     //OUT
	       double & dabnrm,     //OUT
	       double drcone[],     //OUT
	       double drconv[],     //OUT
	       double dwork[],      //WORKSPACE
	       const int & ldwork,  //IN
	       int iwork2[],        //WORKSPACE
	       int & info);     
 
 
 void dgemm_(const char *, const char*,
	     const int& cols, const int& rows, const int& mids, 
	     const double& a, const double* B, const int& ldb,
	     const double* A, const int& lda, const double& b, 
	     double* C, const int& ldc);  
 
 
}
 
#endif