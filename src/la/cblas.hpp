#pragma once

#ifdef __USE_MKL
#include <mkl_cblas.h>
#include <mkl_lapacke.h>
#include <mkl.h>
#else
#include <cblas.h>
#include <lapacke.h>
#endif

#include <complex>
#include <Kokkos_Core.hpp>


#ifdef __USE_MKL
#define CPX MKL_Complex16
#else
#define CPX _Complex double
#endif

namespace nlcglib {
namespace cblas {


struct blas_base
{
  static const CBLAS_UPLO LOWER { CblasLower };
  static const CBLAS_UPLO UPPER { CblasUpper };
  static const CBLAS_TRANSPOSE H { CblasConjTrans };
  static const CBLAS_TRANSPOSE N { CblasNoTrans };
};


struct lapack_base
{
  static const char VECTOR{'V'};
  static const char NOVECTOR{'N'};
};

template<typename T>
struct zheevd
{
};

template<>
struct zheevd<std::complex<double>> : lapack_base
{
  inline int static call(CBLAS_ORDER order,
                         char jobz,
                         char uplo,
                         int n,
                         std::complex<double>* a,
                         const int lda,
                         double* w)
  {
    return LAPACKE_zheevd(order, jobz, uplo, n, reinterpret_cast<CPX *>(a), lda, w);
  }
};


template <>
struct zheevd<Kokkos::complex<double>> : lapack_base
{
  inline int static call(CBLAS_ORDER order,
                         char jobz,
                         char uplo,
                         int n,
                         Kokkos::complex<double> *a,
                         const int lda,
                         double *w)
  {
    return LAPACKE_zheevd(order, jobz, uplo, n, reinterpret_cast<CPX *>(a), lda, w);
  }
};


template <typename T>
struct potrf {};

template<>
struct potrf<std::complex<double>> : lapack_base
{
  inline int static call(CBLAS_ORDER order,
                         char uplo,
                         int n,
                         std::complex<double>* a,
                         int lda)
  {
    // std::cout << "LAPACK_COL_MAJOR: " << LAPACK_COL_MAJOR << "\n";
    // std::cout << "order: " << order << "\n";
    // std::cout << "uplo: " << uplo << "\n";
    // std::cout << "n: " << n << "\n";
    return LAPACKE_zpotrf(LAPACK_COL_MAJOR, uplo, n, reinterpret_cast<CPX *>(a), lda);
  }
};

template <>
struct potrf<Kokkos::complex<double>> : lapack_base
{
  inline int static call(CBLAS_ORDER order, char uplo, int n, Kokkos::complex<double> *a, int lda)
  {
    // std::cout << "LAPACK_COL_MAJOR: " << LAPACK_COL_MAJOR << "\n";
    // std::cout << "order: " << order << "\n";
    // std::cout << "uplo: " << uplo << "\n";
    // std::cout << "n: " << n << "\n";
    return LAPACKE_zpotrf(LAPACK_COL_MAJOR, uplo, n, reinterpret_cast<CPX *>(a), lda);
  }
};


template <typename T>
struct potrs {};

template<>
struct potrs<std::complex<double>> : lapack_base
{
  inline int static call(CBLAS_ORDER order,
                         char uplo,
                         int n,
                         int nrhs,
                         std::complex<double>* a,
                         int lda,
                         std::complex<double>* b,
                         int ldb)
  {
    return LAPACKE_zpotrs(order,
                          uplo,
                          n,
                          nrhs,
                          reinterpret_cast<const CPX *>(a),
                          lda,
                          reinterpret_cast<CPX *>(b),
                          ldb);
  }
};

template <>
struct potrs<Kokkos::complex<double>> : lapack_base
{
  inline int static call(CBLAS_ORDER order,
                         char uplo,
                         int n,
                         int nrhs,
                         Kokkos::complex<double> *a,
                         int lda,
                         Kokkos::complex<double> *b,
                         int ldb)
  {
    return LAPACKE_zpotrs(order,
                          uplo,
                          n,
                          nrhs,
                          reinterpret_cast<const CPX *>(a),
                          lda,
                          reinterpret_cast<CPX *>(b),
                          ldb);
  }
};


template <typename T>
struct gemm
{
};

template <>
struct gemm<std::complex<double>> : blas_base
{
  /// Hermitian transpose
  inline static void call(const CBLAS_ORDER Order,
                          const CBLAS_TRANSPOSE TransA,
                          const CBLAS_TRANSPOSE TransB,
                          const int M,
                          const int N,
                          const int K,
                          const std::complex<double> alpha,
                          const std::complex<double> *A,
                          const int lda,
                          const void *B,
                          const int ldb,
                          const std::complex<double> beta,
                          std::complex<double> *C,
                          const int ldc)
  {
    cblas_zgemm(Order,
                TransA,
                TransB,
                M,
                N,
                K,
                (void *)&alpha,
                (void *)A,
                lda,
                (void *)B,
                ldb,
                (void *)&beta,
                (void *)C,
                ldc);
  }
};


template <>
struct gemm<Kokkos::complex<double>> : blas_base
{
  /// Hermitian transpose
  inline static void call(const CBLAS_ORDER Order,
                          const CBLAS_TRANSPOSE TransA,
                          const CBLAS_TRANSPOSE TransB,
                          const int M,
                          const int N,
                          const int K,
                          const Kokkos::complex<double> alpha,
                          const Kokkos::complex<double> *A,
                          const int lda,
                          const void *B,
                          const int ldb,
                          const Kokkos::complex<double> beta,
                          Kokkos::complex<double> *C,
                          const int ldc)
  {
    cblas_zgemm(Order,
                TransA,
                TransB,
                M,
                N,
                K,
                (void *)&alpha,
                (void *)A,
                lda,
                (void *)B,
                ldb,
                (void *)&beta,
                (void *)C,
                ldc);
  }
};



template <>
struct gemm<double> : blas_base
{
  /// Hermitian transpose
  inline static void call(const CBLAS_ORDER Order,
                          const CBLAS_TRANSPOSE TransA,
                          const CBLAS_TRANSPOSE TransB,
                          const int M,
                          const int N,
                          const int K,
                          const double alpha,
                          const double *A,
                          const int lda,
                          const double *B,
                          const int ldb,
                          const double beta,
                          double *C,
                          const int ld)
  {
    cblas_dgemm(Order, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ld);
  }
};


template<class T>
struct geam {};


template<>
struct geam<Kokkos::complex<double>> : blas_base
{
  inline static void call(const CBLAS_ORDER Order,
                          const CBLAS_TRANSPOSE TransA,
                          const CBLAS_TRANSPOSE TransB,
                          const int M,
                          const int N,
                          Kokkos::complex<double> alpha,
                          const Kokkos::complex<double> *A,
                          const int lda,
                          const Kokkos::complex<double> beta,
                          const Kokkos::complex<double>* B,
                          const int ldb,
                          Kokkos::complex<double> *C,
                          const int ldc)
  {
#ifdef __USE_MKL
    mkl_zomatadd(Order,
                 TransA,
                 TransB,
                 m,
                 n,
                 alpha,
                 A,
                 lda,
                 beta,
                 B,
                 ldb,
                 C,
                 ldc);
#else
    auto cA = reinterpret_cast<const std::complex<double>*>(A);
    auto cB = reinterpret_cast<const std::complex<double>*>(B);
    auto cC = reinterpret_cast<std::complex<double>*>(C);

    std::complex<double> alpha_{alpha.real(), alpha.imag()};
    std::complex<double> beta_{beta.real(), beta.imag()};

    if (TransA == CBLAS_TRANSPOSE::CblasNoTrans && TransB == CBLAS_TRANSPOSE::CblasNoTrans && Order == CblasColMajor) {
#pragma omp parallel for
      for (auto j = 0ul; j < N; ++j) {
        for (auto i = 0ul; i < M; ++i) {
          cC[i + ldc * j] = alpha_ * (cA[i + lda * j]) + beta_ * cB[i + M * j];
        }
      }
    } else {
      throw std::runtime_error("cblas::geam: transpose args not implemented");
    }

#endif
  }

};

}  // namespace cblas
}  // namespace nlcglib
