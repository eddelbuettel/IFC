#include <Rcpp.h>
using namespace Rcpp;

//' @title Matrix Cropping
//' @name cpp_crop
//' @description
//' Crops mat according to new_height and new_width parameters.
//' @param mat a numeric matrix.
//' @param new_height an unsigned integer, giving the new height of returned mat. Default is 0 for no change.
//' @param new_width an unsigned integer, giving the new width of returned mat. Default is 0 for no change.
//' @return a cropped matrix.
//' @keywords internal
////' @export
// [[Rcpp::export]]
NumericMatrix cpp_crop ( NumericMatrix mat, // remove const to allow sub
                         const R_len_t new_height = 0,
                         const R_len_t new_width = 0) {
  
  R_len_t img_c, img_r;
  img_c = mat.ncol();
  img_r = mat.nrow();
  // new dimensions are larger than original dimensions, no need to crop
  if((img_c <= new_width) && (img_r <= new_height)) return mat;
  // new width is larger than original width and new_height is 0, no need to crop
  if((img_c <= new_width) && (new_height == 0)) return mat;
  // new height is larger than original height and new_width is 0, no need to crop
  if((img_r <= new_height) && (new_width == 0)) return mat;
  
  R_len_t ori_c, ori_r, crop_c, crop_r;
  // height resizing
  if((new_height > 0) && (new_height < img_r)) {
    // compute same amount of rows to remove on top-bottom
    ori_r = (img_r - new_height) >> 1;
    crop_r = new_height;
  } else { // no height resizing
    ori_r = 0;
    crop_r = img_r;
  }
  // width resizing
  if((new_width > 0) && (new_width < img_c)) {
    // compute same amount of cols to remove on right-left
    ori_c = (img_c - new_width) >> 1;
    crop_c = new_width;
  } else { // no width resizing
    ori_c = 0;
    crop_c = img_c;
  }
  return mat( Range(ori_r,crop_r + ori_r - 1) , Range(ori_c,crop_c + ori_c - 1) );
}

// function to expand matrix without adding noise
NumericMatrix cpp_expand_no_noise ( const NumericMatrix mat,
                                    const R_len_t new_height = 0,
                                    const R_len_t new_width = 0,
                                    const double bg = 0.0) {
  R_len_t img_r, img_c;
  img_r = mat.nrow();
  img_c = mat.ncol();
  // new dimensions are smaller than original dimensions, no need to expand
  if((img_r >= new_height) && (img_c >= new_width)) return mat;
  
  R_len_t i_row, i_col, ori_r, ori_c, i_out, fin_height, fin_width;
  // final height: either original or desired height if larger
  fin_height = img_r > new_height ? img_r : new_height;
  // final width: either original or desired width if larger
  fin_width = img_c > new_width ? img_c : new_width;
  // compute padding
  ori_r = (fin_height-img_r) >> 1;
  ori_c = (fin_width-img_c) >> 1;
  
  // create output matrix with expanded dimension and fill with bg
  NumericMatrix out(fin_height, fin_width);
  out.fill(bg);
  
  // write mat into center of output matrix
  for(i_col = 0; i_col < img_c; i_col++) {
    i_out = (i_col + ori_c) * fin_height + ori_r;
    for(i_row = 0; i_row < img_r; i_row++) {
      out[i_out++] = mat(i_row, i_col);
    }
  }
  return out;
}

// function to expand matrix with new rows adding noisy padding bg
NumericMatrix cpp_expand_row ( const NumericMatrix mat,
                               const R_len_t new_height = 0,
                               const double bg = 0.0, const double sd = 0.0) {
  R_len_t img_r = mat.nrow();
  // new dimensions are smaller than original dimensions, no need to expand
  if(img_r >= new_height) return mat;
  
  R_len_t img_c, ori_r, i;
  img_c = mat.ncol();
  // compute row padding
  ori_r = (new_height-img_r) >> 1;
  
  // create output matrix with expanded rows
  NumericMatrix out(new_height, img_c);
  
  // write top padding
  for(i = 0; i < ori_r; i++) out(i, _) = rnorm(img_c, bg, sd);
  // write mat into center of output matrix
  for(; i < (ori_r + img_r); i++) out(i, _) = mat(i - ori_r, _);
  // write bottom padding
  for(; i < new_height; i++) out(i, _) = rnorm(img_c, bg, sd); 
  return out;
}

// function to expand matrix with new columns adding noisy padding bg
NumericMatrix cpp_expand_col ( const NumericMatrix mat,
                               const R_len_t new_width = 0,
                               const double bg = 0.0, const double sd = 0.0) {
  R_len_t img_c = mat.ncol();
  // new dimensions are smaller than original dimensions, no need to expand
  if(img_c >= new_width) return mat;
  
  R_len_t img_r, ori_c, i;
  img_r = mat.nrow();
  // compute row padding
  ori_c = (new_width-img_c) >> 1;
  
  // create output matrix with expanded rows
  NumericMatrix out(img_r, new_width);
  
  // write left padding
  for(i = 0; i < ori_c; i++) out(_, i) = rnorm(img_r, bg, sd);
  // write mat into center of output matrix
  for(; i < (ori_c + img_c); i++) out(_, i) = mat(_, i - ori_c);
  // write right padding
  for(; i < new_width; i++) out(_, i) = rnorm(img_r, bg, sd);
  
  return out;
}

// function to expand matrix with padding noisy bg
NumericMatrix cpp_expand_w_noise ( const NumericMatrix mat, 
                                   const R_len_t new_height = 0,
                                   const R_len_t new_width = 0,
                                   const double bg = 0.0, const double sd = 0.0) {
  NumericMatrix M0 = cpp_expand_col(mat, new_width, bg, sd);
  return cpp_expand_row(M0, new_height, bg, sd);
}

//' @title Matrix Resizing
//' @name cpp_resize
//' @description
//' Resizes mat according to new_height and new_width parameters.
//' @param mat a numeric matrix.
//' @param new_height an unsigned integer, giving the new height of returned mat. Default is 0 for no change.
//' @param new_width an unsigned integer, giving the new width of returned mat. Default is 0 for no change.
//' @param add_noise logical, if true adds normal noise when at least one new dimension is larger than original mat dimensions 
//' Rcpp::rnorm() function is used. Default is true.
//' @param bg double, mean value of the background added if add_noise is true. Default is 0.
//' @param sd double, standard deviation of the background added if add_noise is true. Default is 0.
//' @return a resized matrix with padding background if new_height or new_width is larger than original mat dimensions.
//' @keywords internal
////' @export
// [[Rcpp::export]]
NumericMatrix cpp_resize (const NumericMatrix mat, 
                          const R_len_t new_height = 0, 
                          const R_len_t new_width = 0,
                          const bool add_noise = true, 
                          const double bg = 0.0, const double sd = 0.0) {
  NumericMatrix crop = cpp_crop(mat, new_height, new_width);
  NumericMatrix out;
  if(add_noise) {
    out = cpp_expand_w_noise(crop, new_height, new_width, bg, sd);
  } else {
    out = cpp_expand_no_noise(crop, new_height, new_width, bg);
  }
  if(mat.hasAttribute("mask")) out.attr("mask") = mat.attr("mask");
  return out;
}
