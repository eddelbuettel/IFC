/*
  This file is released under the GNU General Public License, Version 3, GPL-3  
  Copyright (C) 2020 Yohann Demont                                              
                                                                                
  It is part of IFC package, please cite:                                       
  -IFC: An R Package for Imaging Flow Cytometry                                 
  -YEAR: 2020                                                                   
  -COPYRIGHT HOLDERS: Yohann Demont, Gautier Stoll, Guido Kroemer,              
                      Jean-Pierre Marolleau, Loï�c Gaççon,                       
                      INSERM, UPD, CHU Amiens                                   
                                                                                
                                                                                
  DISCLAIMER:                                                                   
  -You are using this package on your own risk!                                 
  -We do not guarantee privacy nor confidentiality.                             
  -This program is distributed in the hope that it will be useful, but WITHOUT  
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or         
  FITNESS FOR A PARTICULAR PURPOSE. In no event shall the copyright holders or  
  contributors be liable for any direct, indirect, incidental, special,         
  exemplary, or consequential damages (including, but not limited to,           
  procurement of substitute goods or services; loss of use, data, or profits;   
  or business interruption) however caused and on any theory of liability,      
  whether in contract, strict liability, or tort (including negligence or       
  otherwise) arising in any way out of the use of this software, even if        
  advised of the possibility of such damage.                                    
                                                                                
  You should have received a copy of the GNU General Public License             
  along with IFC. If not, see <http://www.gnu.org/licenses/>.                   
*/

#ifndef IFC_TRANS_HPP
#define IFC_TRANS_HPP

#include <Rcpp.h>
Rcpp::NumericVector cpp_smoothLinLog (const Rcpp::NumericVector x, 
                                      const double hyper = 1000.0, 
                                      const double base = 10.0, 
                                      const double lin_comp = 2.302585);
Rcpp::NumericVector cpp_inv_smoothLinLog (const Rcpp::NumericVector x, 
                                          const double hyper = 1000.0, 
                                          const double base = 10.0, 
                                          const double lin_comp = 2.302585);
uint32_t cpp_int32_to_uint32 ( const int32_t x);
int32_t cpp_uint32_to_int32 ( const uint32_t x);
Rcpp::StringVector cpp_num_to_string(const Rcpp::NumericVector x, 
                                     const unsigned char precision = 16);

#endif
