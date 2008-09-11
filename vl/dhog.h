/** @file     dhog.h
 ** @author   Andrea Vedaldi
 ** @brief    Dense Feature Transform (DHOG)
 **/

/* AUTORIGHTS
 Copyright 2007 (c) Andrea Vedaldi and Brian Fulkerson
 
 This file is part of VLFeat, available in the terms of the GNU
 General Public License version 2.
 */

#ifndef VL_DHOG_H 
#define VL_DHOG_H 

#include "generic.h"

/** @brief DHOG keypoint */
typedef struct VlDhogKeypoint_
{
  double x ; /**< x coordinate */
  double y ; /**< y coordinate */
  double s ; /**< scale */
} VlDhogKeypoint ;

/** @brief DHOG filter */
typedef struct VlDhogFilter_ 
{  
  int width ;            /**< image width */
  int height ;           /**< image height */
  int sampling_step ;    /**< downsampling step */
  int bin_size ;         /**< spatial bin size (in pixels) */
  int fast ;             /**< be fast but more approximated */

  int dwidth ;           /**< downsampled width */
  int dheight ;          /**< downsampled height */
  
  float **hist ;         /**< dense histograms */
  float *tmp ;           /**< temporary buffer */
  float *tmp2 ;          /**< temporary buffer */
  float *descr ;         /**< descriptors */
  VlDhogKeypoint *keys ; /**< keypoints */
  int nkeys ;            /**< number of keypoints */
}  VlDhogFilter ;

VL_EXPORT VlDhogFilter *vl_dhog_new (int width, int height, int sampling_step, int bin_size) ;
VL_EXPORT void vl_dhog_delete (VlDhogFilter *f) ;
VL_EXPORT void vl_dhog_process (VlDhogFilter *f, float const* im, vl_bool fast) ;

/** @name Retrieving data and parameters
 ** @{
 **/
VL_INLINE float          *vl_dhog_get_descriptors   (VlDhogFilter *f) ;
VL_INLINE int             vl_dhog_get_keypoint_num  (VlDhogFilter *f) ;
VL_INLINE VlDhogKeypoint *vl_dhog_get_keypoints     (VlDhogFilter *f) ;
VL_INLINE void            vl_dhog_transpose_descriptor (float* dst, float const* src) ;
/** @} */

/** ------------------------------------------------------------------
 ** @brief Get descriptors.
 ** @param f DHOG filter.
 ** @return descriptors.
 **/

float *
vl_dhog_get_descriptors (VlDhogFilter *f)
{
  return f->descr ;
}

/** ------------------------------------------------------------------
 ** @brief Get keypoints
 ** @param f DHOG filter.
 **/

VlDhogKeypoint *
vl_dhog_get_keypoints (VlDhogFilter *f)
{
  return f->keys ;
}

/** ------------------------------------------------------------------
 ** @brief Get number of keypoints
 ** @param f DHOG filter.
 **/

int
vl_dhog_get_keypoint_num (VlDhogFilter *f)
{
  return f->nkeys ;
}


/** ------------------------------------------------------------------
 ** @brief Transpose descriptor
 **
 ** @param dst destination buffer.
 ** @param src source buffer.
 **
 ** The function writes to @a dst the transpose of the SIFT descriptor
 ** @a src. Let <code>I</code> be an image. The transpose operator
 ** satisfies the equation
 ** <code>transpose(dhog(I,x,y)) = dhog(transpose(I),y,x)</code>
 **/

VL_INLINE void 
vl_dhog_transpose_descriptor (float* dst, float const* src) 
{
  int const BO = 8 ;  /* number of orientation bins */
  int const BP = 4 ;  /* number of spatial bins     */
  int i, j, t ;
 
  for (j = 0 ; j < BP ; ++j) {
    for (i = 0 ; i < BP ; ++i) {
      int o  = BO * i + BP*BO * j  ;
      int op = BO * j + BP*BO * i ;      
      for (t = 0 ; t < BO ; ++t) 
        dst [op+((BO/4+BO) -t)%BO] = src [o+t] ;
    }
  }
}

/*  VL_DHOG_H */
#endif
