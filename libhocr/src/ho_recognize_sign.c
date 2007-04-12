/***************************************************************************
 *            ho_recognize_sign.c
 *
 *  Fri Aug 12 20:13:33 2005
 *  Copyright  2005-2007  Yaacov Zamir
 *  <kzamir@walla.co.il>
 ****************************************************************************/

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#ifndef TRUE
#define TRUE -1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL ((void*)0)
#endif

#include "ho_bitmap.h"
#include "ho_objmap.h"
#include "ho_bitmap_filter.h"
#include "ho_segment.h"
#include "ho_font.h"
#include "ho_recognize.h"

#include "ho_recognize_sign.h"

/* TODO: font parts, e.g top-bar, little nose to the left ... */



/* reading signs */

double
ho_recognize_sign_dot (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_comma (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_minus (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

/* hebrew fonts */

double
ho_recognize_sign_alef (const double *array_in)
{
  int i;

  static const double array_data[HO_ARRAY_IN_SIZE][2] = {
    {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0},
    {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0},
    {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0},
    {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0},
    {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0},
    {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0},
    {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0},
    {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0},
    {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0},
    {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0},
    {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0},
    {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0},
    {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0},
    {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}
  };

  for (i = 0; i < HO_ARRAY_IN_SIZE; i++)
    if (array_in[i] < array_data[i][0] || array_in[i] > array_data[i][1])
      return 0.0;

  return 1.0;
}

double
ho_recognize_sign_bet (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_gimal (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_dalet (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_hey (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_vav (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_zayin (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_het (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_tet (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_yod (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_caf (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_caf_sofit (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_lamed (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_mem (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_mem_sofit (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_nun (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_nun_sofit (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_samech (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_ayin (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_pey (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_pey_sofit (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_tzadi (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_tzadi_sofit (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_kof (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_resh (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_shin (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}

double
ho_recognize_sign_tav (const double *array_in)
{
  double return_value = 0.0;

  return return_value;
}
