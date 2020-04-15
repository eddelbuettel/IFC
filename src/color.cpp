#include <Rcpp.h>
using namespace Rcpp;

//' @title HSV to RGB Conversion
//' @name cpp_HSV2RGB
//' @description
//' Converts a color from HSV [0,1] space to RGB [0,1] space.
//' @param *r pointer to r
//' @param *g pointer to g
//' @param *b pointer to b
//' @param h double, [0,1], hue. Default is 0.0
//' @param s double, [0,1], saturation. Default is 0.0
//' @param v double, [0,1], value. Default is 0.0
//' @source see hsv source code from \pkg{grDevices}\cr
//' Based on HSV_TO_RGB from Foley and Van Dam First Ed. Page 616\cr
//' See Alvy Ray Smith, Color Gamut Transform Pairs, SIGGRAPH '78
//' @keywords internal
////' @export
void cpp_HSV2RGB(double *r, double *g, double *b,
                 const double h = 0.0, const double s = 0.0, const double v = 0.0) {
  if (h < 0 || h > 1 || s < 0 || s > 1 || v < 0 || v > 1) Rcpp::stop("invalid hsv color");
  double f, p, q, t;
  int i;
  
  f = modf(h * 6.0, &t);
  i = ((int) t) % 6;
  p = v * (1 - s);
  q = v * (1 - s * f);
  t = v * (1 - (s * (1 - f)));
  
  switch (i) {
  case 0:	*r = v;		*g = t;		*b = p;	break;
  case 1:	*r = q;		*g = v;		*b = p;	break;
  case 2:	*r = p;		*g = v;		*b = t;	break;
  case 3:	*r = p;		*g = q;		*b = v; break;
  case 4:	*r = t;		*g = p;		*b = v; break;
  case 5:	*r = v;		*g = p;		*b = q;	break;
  default:
    Rcpp::stop("bad hsv to rgb color conversion");
  }
}

//' @title Matrix HSV to RGB Conversion
//' @name cpp_M_HSV2RGB
//' @description
//' Converts grayscale [0,1] mat to 3D rgb array according to hsv space.
//' hue and saturation determines tint whereas v is given by each element of mat
//' @param mat NumericMatrix, [0,1].
//' @param h double, [0,1], hue. Default is 0.0
//' @param s double, [0,1], saturation. Default is 0.0
//' @return a NumericVector with 3 dimensions attribute i.e. a 3D array
//' - 1st Dim is matrix rows count,
//' - 2nd Dim is matrix cols count,
//' - 3rd Dim is RGB
//' @keywords internal
////' @export
// [[Rcpp::export]]
NumericVector cpp_M_HSV2RGB (const NumericMatrix mat, const double h = 0.0, const double s = 0.0) {
  R_len_t mat_r = mat.nrow();
  R_len_t mat_c = mat.ncol();
  R_len_t D = mat_r * mat_c;
  double r = 0.0, g = 0.0, b = 0.0;
  NumericMatrix R(mat_r, mat_c);
  NumericMatrix G(mat_r, mat_c);
  NumericMatrix B(mat_r, mat_c);
  
  for(R_len_t i_row = 0; i_row < mat_r; i_row++) {
    for(R_len_t i_col = 0; i_col < mat_c; i_col++) {
      cpp_HSV2RGB(&r, &g, &b, h, s, mat(i_row, i_col));
      R(i_row, i_col) = r;
      G(i_row, i_col) = g;
      B(i_row, i_col) = b;
    }
  }
  
  NumericVector out(D * 3);
  std::copy(R.begin(), R.end(), out.begin());
  std::copy(G.begin(), G.end(), out.begin() + D);
  std::copy(B.begin(), B.end(), out.begin() + 2 * D);
  out.attr("dim") = Rcpp::Dimension(mat_r, mat_c, 3);
  if(mat.hasAttribute("mask")) out.attr("mask") = mat.attr("mask");
  return out;
}
