#include <Rcpp.h>
using namespace Rcpp;

//' @title A Gibbs sampler using Rcpp
//' @description A Gibbs sampler using Rcpp
//' @param N the number of samples
//' @param thin the number of between-sample random numbers
//' @return a random sample of size 
//' @examples
//' \dontrun{
//' rnC <- gibbsC(100,10)
//' par(mfrow=c(2,1));
//' plot(rnC[,1],type='l')
//' plot(rnC[,2],type='l')
//' }
//' @export
// [[Rcpp::export]]
NumericMatrix gibbsC(int N, int thin) {
  NumericMatrix mat(N, 2);
  double x = 0, y = 0;
  for(int i = 0; i < N; i++) {
    for(int j = 0; j < thin; j++) {
      x = rgamma(1, 3, 1 / (y * y + 4))[0];
      y = rnorm(1, 1 / (x + 1), 1 / sqrt(2 * (x + 1)))[0];
    }
    mat(i, 0) = x;
    mat(i, 1) = y;
  }
  return(mat);
}

double vacc3a(double age, bool female, bool ily){
  double p = 0.25 + 0.3 * 1 / (1 - exp(0.04 * age)) + 0.1 * ily;
  p = p * (female ? 1.25 : 0.75);
  p = std::max(p, 0.0);
  p = std::min(p, 1.0);
  return p;
}


// [[Rcpp::export]]

NumericMatrix cppgibbs(int N,float mu1,float mu2,float sigma1,float sigma2,float rho){
  NumericMatrix mat(N,2);
  float m1,s1,m2,s2;
  float x2;
  float x1;
  int i;
  mat(0,0)=mu1;
  mat(0,1)=mu2;
  s1 = sqrt(1-pow(rho,2))*sigma1;
  s2 = sqrt(1-pow(rho,2))*sigma2;
  for(i=1;i<N;i++){
    x2 = mat(i-1,1);
    m1 = mu1 + rho * (x2 - mu2)*sigma1/sigma2;
    mat(i,0) = ::Rf_rnorm(m1,s1);
    x1 = mat(i,0);
    m2 = mu2 + rho * (x1 - mu1)*sigma2/sigma1;
    mat(i,1) = ::Rf_rnorm(m2, s2);
  }
  return mat;
}





