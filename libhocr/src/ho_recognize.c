/***************************************************************************
 *            ho_recognize.c
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
#include "ho_segment.h"
#include "ho_font.h"

#include "ho_recognize.h"

int
ho_recognize_array_in_size ()
{
  return HO_ARRAY_IN_SIZE;
}

int
ho_recognize_array_out_size ()
{
  return HO_ARRAY_OUT_SIZE;
}

int
ho_recognize_hbar_up (const ho_bitmap * m_text, const ho_bitmap * m_mask,
		      double *height, double *start, double *end,
		      double *dist)
{
  ho_bitmap *m_hbars = NULL;
  int x, y, y_start, y_end, line_start, line_height;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the hbar bitmap */
  m_hbars = ho_font_hbars (m_text, m_mask);
  if (!m_hbars)
    return TRUE;

  /* get start and end of horizontal top bar */
  y = line_start - line_height / 2;
  if (y < 0)
    y = 0;
  for (;
       y < line_start + line_height / 2 && !ho_bitmap_get (m_hbars, x, y);
       y++);
  y_start = y;
  for (; y < line_start + line_height / 2 && ho_bitmap_get (m_hbars, x, y);
       y++);
  y_end = y;

  ho_bitmap_free (m_hbars);

  /* check for no top bar */
  if (y_start == y_end)
    {
      *height = 0.0;
      *start = 0.0;
      *end = 0.0;

      return FALSE;
    }

  /* if start is far from line top check artefacts */
  if (y_start > line_start + line_height / 12)
    {
      int new_sum = 0;
      int sum = new_sum;

      for (;
	   y_start > line_start && (new_sum >= sum
				    || new_sum > m_text->width / 2);
	   y_start--)
	for (sum = new_sum, new_sum = 0, x = 0; x < m_text->width; x++)
	  new_sum += ho_bitmap_get (m_text, x, y_start);
    }

  /* set height */
  *height = (double) (y_end - y_start) / (double) line_height;

  /* set distance from top */
  *dist = (double) (line_start - y_start) / (double) line_height;

  /* get x start and end of bar */
  {
    int sum = 0;
    for (x = 0; x < m_text->width && sum == 0; x++)
      for (sum = 0, y = y_start; y < y_end; y++)
	sum += ho_bitmap_get (m_text, x, y);

    *start = (double) x / (double) (m_text->width);

    for (; x < m_text->width && sum > 0; x++)
      for (sum = 0, y = y_start; y < y_end; y++)
	sum += ho_bitmap_get (m_text, x, y);

    *end = (double) x / (double) (m_text->width);
  }

  return FALSE;
}

int
ho_recognize_hbar_down (const ho_bitmap * m_text,
			const ho_bitmap * m_mask, double *height,
			double *start, double *end, double *dist)
{
  ho_bitmap *m_hbars = NULL;
  int x, y, y_start, y_end, line_start, line_height;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the hbar bitmap */
  m_hbars = ho_font_hbars (m_text, m_mask);
  if (!m_hbars)
    return TRUE;

  /* get start and end of horizontal top bar */
  y = line_start + 3 * line_height / 2;
  if (y > m_mask->height)
    y = m_mask->height;
  for (;
       y > line_start + line_height / 2 && !ho_bitmap_get (m_hbars, x, y);
       y--);
  y_end = y;
  for (; y > line_start + line_height / 2 && ho_bitmap_get (m_hbars, x, y);
       y--);
  y_start = y;

  ho_bitmap_free (m_hbars);

  /* check for no top bar */
  if (y_start == y_end)
    {
      *height = 0.0;
      *start = 0.0;
      *end = 0.0;

      return FALSE;
    }

  /* if start is far from line top check artefacts */
  if (y_end < line_start + 11 * line_height / 12)
    {
      int new_sum = 0;
      int sum = new_sum;

      for (;
	   y_end < line_start + line_height && (new_sum >= sum
						|| new_sum >
						m_text->width / 2); y_end++)
	for (sum = new_sum, new_sum = 0, x = 0; x < m_text->width; x++)
	  new_sum += ho_bitmap_get (m_text, x, y_start);
    }

  /* set height */
  *height = (double) (y_end - y_start) / (double) line_height;

  /* set distance from bottom */
  *dist = (double) (line_start + line_height - y_end) / (double) line_height;

  /* get x start and end of bar */
  {
    int sum = 0;
    for (x = 0; x < m_text->width && sum == 0; x++)
      for (sum = 0, y = y_start; y < y_end; y++)
	sum += ho_bitmap_get (m_text, x, y);

    *start = (double) x / (double) (m_text->width);

    for (; x < m_text->width && sum > 0; x++)
      for (sum = 0, y = y_start; y < y_end; y++)
	sum += ho_bitmap_get (m_text, x, y);

    *end = (double) x / (double) (m_text->width);
  }

  return FALSE;
}

int
ho_recognize_vbar_right (const ho_bitmap * m_text,
			 const ho_bitmap * m_mask, double *width,
			 double *start, double *end, double *dist)
{
  ho_bitmap *m_vbars = NULL;
  int x, y, x_start, x_end, line_start, line_height;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the vbar bitmap */
  m_vbars = ho_font_vbars (m_text, m_mask);
  if (!m_vbars)
    return TRUE;

  /* get start and end of vertical left bar */
  y = line_start + line_height / 2;
  for (x = m_text->width;
       x > m_text->width / 2 && !ho_bitmap_get (m_vbars, x, y); x--);
  x_end = x;
  for (; x > m_text->width / 2 && ho_bitmap_get (m_vbars, x, y); x--);
  x_start = x;

  ho_bitmap_free (m_vbars);

  /* check for no top bar */
  if (x_start == x_end)
    {
      *width = 0.0;
      *start = 0.0;
      *end = 0.0;

      return FALSE;
    }

  /* if start is far from line top check artefacts */
  if (x_end < 11 * m_text->width / 12)
    {
      int new_sum = 0;
      int sum = new_sum;

      for (;
	   x_end < m_text->width && (new_sum >= sum
				     || new_sum > line_height / 2); x_end++)
	for (sum = new_sum, new_sum = 0, y = line_start;
	     y < line_start + line_height; y++)
	  new_sum += ho_bitmap_get (m_text, x_end, y);
    }

  /* set width */
  *width = (double) (x_end - x_start) / (double) m_text->width;

  /* set distance from top */
  *dist = (double) (m_text->width - x_end) / (double) m_text->width;

  /* get y start and end of bar */
  {
    int sum = 0;
    for (y = line_start; y < line_start + line_height && sum == 0; y++)
      for (sum = 0, x = x_start; x < x_end; x++)
	sum += ho_bitmap_get (m_text, x, y);

    *start = (double) (y - line_start) / (double) line_height;

    for (; y < line_start + line_height && sum > 0; y++)
      for (sum = 0, x = x_start; x < x_end; x++)
	sum += ho_bitmap_get (m_text, x, y);

    *end = (double) (y - line_start) / (double) line_height;
  }

  return FALSE;
}

int
ho_recognize_vbar_left (const ho_bitmap * m_text,
			const ho_bitmap * m_mask, double *width,
			double *start, double *end, double *dist)
{
  ho_bitmap *m_vbars = NULL;
  int x, y, x_start, x_end, line_start, line_height;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the vbar bitmap */
  m_vbars = ho_font_vbars (m_text, m_mask);
  if (!m_vbars)
    return TRUE;

  /* get start and end of vertical left bar */
  y = line_start + line_height / 2;
  for (x = 0; x < m_text->width / 2 && !ho_bitmap_get (m_vbars, x, y); x++);
  x_start = x;
  for (; x < m_text->width / 2 && ho_bitmap_get (m_vbars, x, y); x++);
  x_end = x;

  ho_bitmap_free (m_vbars);

  /* check for no top bar */
  if (x_start == x_end)
    {
      *width = 0.0;
      *start = 0.0;
      *end = 0.0;

      return FALSE;
    }

  /* if start is far from line top check artefacts */
  if (x_start > m_text->width / 12)
    {
      int new_sum = 0;
      int sum = new_sum;

      for (;
	   x_start > 0 && (new_sum >= sum
			   || new_sum > line_height / 2); x_start--)
	for (sum = new_sum, new_sum = 0, y = line_start;
	     y < line_start + line_height; y++)
	  new_sum += ho_bitmap_get (m_text, x_start, y);
    }

  /* set width */
  *width = (double) (x_end - x_start) / (double) m_text->width;

  /* set distance from top */
  *dist = (double) (x_start) / (double) m_text->width;

  /* get y start and end of bar */
  {
    int sum = 0;
    for (y = line_start; y < line_start + line_height && sum == 0; y++)
      for (sum = 0, x = x_start; x < x_end; x++)
	sum += ho_bitmap_get (m_text, x, y);

    *start = (double) (y - line_start) / (double) line_height;

    for (; y < line_start + line_height && sum > 0; y++)
      for (sum = 0, x = x_start; x < x_end; x++)
	sum += ho_bitmap_get (m_text, x, y);

    *end = (double) (y - line_start) / (double) line_height;
  }

  return FALSE;
}

int
ho_recognize_dbar (const ho_bitmap * m_text, const ho_bitmap * m_mask,
		   double *width, double *start, double *end)
{
  ho_bitmap *m_dbar = NULL;
  int x, y, x_start, x_end, line_start, line_height;
  int sum;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the vbar bitmap */
  m_dbar = ho_font_diagonal (m_text, m_mask);
  if (!m_dbar)
    return TRUE;

  /* get start and end of vertical left bar */
  sum = 0;
  for (x = 0; x < m_text->width && sum == 0; x++)
    for (sum = 0, y = 0; y < m_text->height; y++)
      sum += ho_bitmap_get (m_dbar, x, y);

  x_start = x;

  for (; x < m_text->width && sum > 0; x++)
    for (sum = 0, y = 0; y < m_text->height; y++)
      sum += ho_bitmap_get (m_dbar, x, y);

  x_end = x;

  ho_bitmap_free (m_dbar);

  /* check for no top bar */
  if (x_start == x_end)
    {
      *width = 0.0;
      *start = 0.0;
      *end = 0.0;

      return FALSE;
    }

  *width = (double) (x_end - x_start) / (double) (m_text->width);;
  *start = (double) x_start / (double) (m_text->width);
  *end = (double) x_end / (double) (m_text->width);

  return FALSE;
}

int
ho_recognize_dbar_left (const ho_bitmap * m_text, const ho_bitmap * m_mask,
			double *width, double *start, double *end)
{
  ho_bitmap *m_dbar = NULL;
  int x, y, x_start, x_end, line_start, line_height;
  int sum;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the vbar bitmap */
  m_dbar = ho_font_diagonal_left (m_text, m_mask);
  if (!m_dbar)
    return TRUE;

  /* get start and end of vertical left bar */
  sum = 0;
  for (x = 0; x < m_text->width && sum == 0; x++)
    for (sum = 0, y = 0; y < m_text->height; y++)
      sum += ho_bitmap_get (m_dbar, x, y);

  x_start = x;

  for (; x < m_text->width && sum > 0; x++)
    for (sum = 0, y = 0; y < m_text->height; y++)
      sum += ho_bitmap_get (m_dbar, x, y);

  x_end = x;

  ho_bitmap_free (m_dbar);

  /* check for no top bar */
  if (x_start == x_end)
    {
      *width = 0.0;
      *start = 0.0;
      *end = 0.0;

      return FALSE;
    }

  *width = (double) (x_end - x_start) / (double) (m_text->width);;
  *start = (double) x_start / (double) (m_text->width);
  *end = (double) x_end / (double) (m_text->width);

  return FALSE;
}

int
ho_recognize_edges_top (const ho_bitmap * m_text, const ho_bitmap * m_mask,
			int *num, int *left, int *middle, int *right)
{
  ho_bitmap *m_edges = NULL;
  int x, y, x_start, x_end, line_start, line_height;
  int sum;

  /* get the top edges */
  m_edges = ho_font_edges_top (m_text, m_mask);
  if (!m_edges)
    return TRUE;

  /* get lines */
  y = 1;
  x = 1;
  (*num) = (*left) = (*middle) = (*right) = 0;

  while (x < m_mask->width)
    {
      for (; x < m_mask->width && !ho_bitmap_get (m_edges, x, y); x++);
      if (x < m_mask->width)
	{
	  (*num)++;
	  if (x < m_mask->width / 3)
	    (*left)++;
	  else if (x < 2 * m_mask->width / 3)
	    (*middle)++;
	  else
	    (*right)++;
	}
      for (; x < m_mask->width && ho_bitmap_get (m_edges, x, y); x++);
    }

  return FALSE;
}

int
ho_recognize_notch_top (const ho_bitmap * m_text, const ho_bitmap * m_mask,
			int *num, int *left, int *middle, int *right)
{
  ho_bitmap *m_edges = NULL;
  int x, y, x_start, x_end, line_start, line_height;
  int sum;

  /* get the top edges */
  m_edges = ho_font_notch_top (m_text, m_mask);
  if (!m_edges)
    return TRUE;

  /* get lines */
  y = 1;
  x = 1;
  (*num) = (*left) = (*middle) = (*right) = 0;

  while (x < m_mask->width)
    {
      for (; x < m_mask->width && !ho_bitmap_get (m_edges, x, y); x++);
      if (x < m_mask->width)
	{
	  (*num)++;
	  if (x < m_mask->width / 3)
	    (*left)++;
	  else if (x < 2 * m_mask->width / 3)
	    (*middle)++;
	  else
	    (*right)++;
	}
      for (; x < m_mask->width && ho_bitmap_get (m_edges, x, y); x++);
    }

  return FALSE;
}

int
ho_recognize_edges_bottom (const ho_bitmap * m_text, const ho_bitmap * m_mask,
			   int *num, int *left, int *middle, int *right)
{
  ho_bitmap *m_edges = NULL;
  int x, y, x_start, x_end, line_start, line_height;
  int sum;

  /* get the top edges */
  m_edges = ho_font_edges_bottom (m_text, m_mask);
  if (!m_edges)
    return TRUE;

  /* get lines */
  y = m_edges->height - 2;
  x = 1;
  (*num) = (*left) = (*middle) = (*right) = 0;

  while (x < m_mask->width)
    {
      for (; x < m_mask->width && !ho_bitmap_get (m_edges, x, y); x++);
      if (x < m_mask->width)
	{
	  (*num)++;
	  if (x < m_mask->width / 3)
	    (*left)++;
	  else if (x < 2 * m_mask->width / 3)
	    (*middle)++;
	  else
	    (*right)++;
	}

      for (; x < m_mask->width && ho_bitmap_get (m_edges, x, y); x++);
    }

  return FALSE;
}

int
ho_recognize_notch_bottom (const ho_bitmap * m_text, const ho_bitmap * m_mask,
			   int *num, int *left, int *middle, int *right)
{
  ho_bitmap *m_edges = NULL;
  int x, y, x_start, x_end, line_start, line_height;
  int sum;

  /* get the top edges */
  m_edges = ho_font_notch_bottom (m_text, m_mask);
  if (!m_edges)
    return TRUE;

  /* get lines */
  y = m_edges->height - 2;
  x = 1;
  (*num) = (*left) = (*middle) = (*right) = 0;

  while (x < m_mask->width)
    {
      for (; x < m_mask->width && !ho_bitmap_get (m_edges, x, y); x++);
      if (x < m_mask->width)
	{
	  (*num)++;
	  if (x < m_mask->width / 3)
	    (*left)++;
	  else if (x < 2 * m_mask->width / 3)
	    (*middle)++;
	  else
	    (*right)++;
	}

      for (; x < m_mask->width && ho_bitmap_get (m_edges, x, y); x++);
    }

  return FALSE;
}

int
ho_recognize_edges_left (const ho_bitmap * m_text, const ho_bitmap * m_mask,
			 int *num, int *top, int *middle, int *bottom)
{
  ho_bitmap *m_edges = NULL;
  int x, y, x_start, x_end, line_start, line_height;
  int sum;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > line_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_height = y - line_start + 1;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the top edges */
  m_edges = ho_font_edges_left (m_text, m_mask);
  if (!m_edges)
    return TRUE;

  /* get lines */
  y = line_start;
  x = 1;
  (*num) = (*top) = (*middle) = (*bottom) = 0;

  while (y < line_start + line_height)
    {
      for (; y < line_start + line_height && !ho_bitmap_get (m_edges, x, y);
	   y++);
      if (y < line_start + line_height)
	{
	  (*num)++;
	  if (y < line_start + line_height / 3)
	    (*top)++;
	  else if (y < line_start + 2 * line_height / 3)
	    (*middle)++;
	  else
	    (*bottom)++;
	}

      for (; y < line_start + line_height && ho_bitmap_get (m_edges, x, y);
	   y++);
    }

  return FALSE;
}

int
ho_recognize_notch_left (const ho_bitmap * m_text, const ho_bitmap * m_mask,
			 int *num, int *top, int *middle, int *bottom)
{
  ho_bitmap *m_edges = NULL;
  int x, y, x_start, x_end, line_start, line_height;
  int sum;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > line_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_height = y - line_start + 1;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the top edges */
  m_edges = ho_font_notch_left (m_text, m_mask);
  if (!m_edges)
    return TRUE;

  /* get lines */
  y = line_start;
  x = 1;
  (*num) = (*top) = (*middle) = (*bottom) = 0;

  while (y < line_start + line_height)
    {
      for (; y < line_start + line_height && !ho_bitmap_get (m_edges, x, y);
	   y++);
      if (y < line_start + line_height)
	{
	  (*num)++;
	  if (y < line_start + line_height / 3)
	    (*top)++;
	  else if (y < line_start + 2 * line_height / 3)
	    (*middle)++;
	  else
	    (*bottom)++;
	}

      for (; y < line_start + line_height && ho_bitmap_get (m_edges, x, y);
	   y++);
    }

  return FALSE;
}

int
ho_recognize_edges_right (const ho_bitmap * m_text, const ho_bitmap * m_mask,
			  int *num, int *top, int *middle, int *bottom)
{
  ho_bitmap *m_edges = NULL;
  int x, y, x_start, x_end, line_start, line_height;
  int sum;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > line_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_height = y - line_start + 1;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the top edges */
  m_edges = ho_font_edges_right (m_text, m_mask);
  if (!m_edges)
    return TRUE;

  /* get lines */
  y = line_start;
  x = m_mask->width - 2;
  (*num) = (*top) = (*middle) = (*bottom) = 0;

  while (y < line_start + line_height)
    {
      for (; y < line_start + line_height && !ho_bitmap_get (m_edges, x, y);
	   y++);
      if (y < line_start + line_height)
	{
	  (*num)++;
	  if (y < line_start + line_height / 3)
	    (*top)++;
	  else if (y < line_start + 2 * line_height / 3)
	    (*middle)++;
	  else
	    (*bottom)++;
	}

      for (; y < line_start + line_height && ho_bitmap_get (m_edges, x, y);
	   y++);
    }

  return FALSE;
}

int
ho_recognize_notch_right (const ho_bitmap * m_text, const ho_bitmap * m_mask,
			  int *num, int *top, int *middle, int *bottom)
{
  ho_bitmap *m_edges = NULL;
  int x, y, x_start, x_end, line_start, line_height;
  int sum;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > line_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  line_height = y - line_start + 1;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get the top edges */
  m_edges = ho_font_notch_right (m_text, m_mask);
  if (!m_edges)
    return TRUE;

  /* get lines */
  y = line_start;
  x = m_mask->width - 2;
  (*num) = (*top) = (*middle) = (*bottom) = 0;

  while (y < line_start + line_height)
    {
      for (; y < line_start + line_height && !ho_bitmap_get (m_edges, x, y);
	   y++);
      if (y < line_start + line_height)
	{
	  (*num)++;
	  if (y < line_start + line_height / 3)
	    (*top)++;
	  else if (y < line_start + 2 * line_height / 3)
	    (*middle)++;
	  else
	    (*bottom)++;
	}

      for (; y < line_start + line_height && ho_bitmap_get (m_edges, x, y);
	   y++);
    }

  return FALSE;
}

int
ho_recognize_second_object (const ho_bitmap * m_text,
			    const ho_bitmap * m_mask,
			    double *has_second_objects,
			    double *height, double *width,
			    double *width_by_height, double *y_start,
			    double *y_end, double *x_start, double *x_end)
{
  ho_bitmap *m_second_obj = NULL;
  int x, y, line_start, line_height;
  int y_font_start, font_height;
  int x_font_start, font_width;
  int sum;

  /* init output */
  *has_second_objects = 0.0;
  *height = 0.0;
  *width = 0.0;
  *width_by_height = 0.0;

  *y_start = 0.0;
  *y_end = 0.0;

  *x_start = 0.0;
  *x_end = 0.0;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  m_second_obj = ho_font_second_object (m_text, m_mask);
  if (!m_second_obj)
    {
      return FALSE;
    }

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  y_font_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > y_font_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_height = y - y_font_start + 1;

  sum = 0;
  for (x = 0; x < m_mask->width && sum == 0; x++)
    for (sum = 0, y = 0; y < m_mask->height; y++)
      sum += ho_bitmap_get (m_text, x, y);
  x_font_start = x - 1;
  sum = 0;
  for (x = m_mask->width; x > x_font_start && sum == 0; x--)
    for (sum = 0, y = 0; y < m_mask->height; y++)
      sum += ho_bitmap_get (m_text, x, y);
  font_width = x - x_font_start + 1;

  *has_second_objects = 1.0;
  *height = (double) font_height / (double) line_height;
  *width = (double) font_width / (double) (m_mask->width);
  *width_by_height = (double) font_width / (double) font_height;

  *y_start = (double) (line_start - y_font_start) / (double) line_height;
  *y_end =
    (double) ((line_start + line_height) -
	      (y_font_start + font_height)) / (double) line_height;

  *x_start = (double) (x_font_start) / (double) (m_mask->width);
  *x_end = (double) (x_font_start + font_width) / (double) (m_mask->width);

  return FALSE;
}

int
ho_recognize_dimentions (const ho_bitmap * m_text, const ho_bitmap * m_mask,
			 double *height, double *width,
			 double *width_by_height, double *start, double *end)
{
  int x, y, x_start, x_end, line_start, line_height;
  int font_start, font_height;
  int sum;

  /* get line start and end */
  x = m_mask->width / 2;
  for (y = 0; y < m_mask->height && !ho_bitmap_get (m_mask, x, y); y++);
  line_start = y - 1;
  for (; y < m_mask->height && ho_bitmap_get (m_mask, x, y); y++);
  line_height = y - line_start;

  if (!line_height || m_mask->width < 2)
    return TRUE;

  /* get font start and end */
  sum = 0;
  for (y = 0; y < m_mask->height && sum == 0; y++)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_start = y - 1;
  sum = 0;
  for (y = m_mask->height - 1; y > font_start && sum == 0; y--)
    for (sum = 0, x = 0; x < m_mask->width; x++)
      sum += ho_bitmap_get (m_text, x, y);
  font_height = y - font_start + 1;
  if (!font_height)
    {
      printf ("NO HEIGHT");
      exit (0);
    }

  *height = (double) font_height / (double) line_height;
  *width = (double) (m_mask->width) / (double) line_height;
  *width_by_height = (double) (m_mask->width) / (double) font_height;
  *start = (double) (line_start - font_start) / (double) line_height;
  *end =
    (double) ((line_start + line_height) -
	      (font_start + font_height)) / (double) line_height;

  return FALSE;
}

int
ho_recognize_create_array_in (const ho_bitmap * m_text,
			      const ho_bitmap * m_mask, double *array_in)
{
  ho_bitmap *m_holes = NULL;

  double height, width, start, end, dist;
  double width_by_height;
  double has_second_objects, y_start, y_end, x_start, x_end;
  int num, top, middle, bottom, left, right;
  int nobjects, nholes;

  /* fill the bars part of array */
  ho_recognize_hbar_up (m_text, m_mask, &height, &start, &end, &dist);
  array_in[0] = height;
  array_in[1] = start;
  array_in[2] = end;
  array_in[3] = dist / 2.0 + 0.5;

  ho_recognize_hbar_down (m_text, m_mask, &height, &start, &end, &dist);
  array_in[4] = height;
  array_in[5] = start;
  array_in[6] = end;
  array_in[7] = dist / 2.0 + 0.5;

  ho_recognize_vbar_left (m_text, m_mask, &width, &start, &end, &dist);
  array_in[8] = width;
  array_in[9] = start;
  array_in[10] = end;
  array_in[11] = dist / 2.0 + 0.5;

  ho_recognize_vbar_right (m_text, m_mask, &width, &start, &end, &dist);
  array_in[12] = width;
  array_in[13] = start;
  array_in[14] = end;
  array_in[15] = dist / 2.0 + 0.5;

  /* fill the diagonals part of array */
  ho_recognize_dbar (m_text, m_mask, &width, &start, &end);
  array_in[16] = width;
  array_in[17] = start;
  array_in[18] = end;

  ho_recognize_dbar_left (m_text, m_mask, &width, &start, &end);
  array_in[19] = width;
  array_in[20] = start;
  array_in[21] = end;

  /* fill the egdes part of array */
  ho_recognize_edges_top (m_text, m_mask, &num, &left, &middle, &right);
  array_in[22] = (num < 3) ? 0.0 : 1.0;
  array_in[23] = (left < 1) ? 0.0 : 1.0;
  array_in[24] = (middle < 1) ? 0.0 : 1.0;
  array_in[25] = (right < 1) ? 0.0 : 1.0;

  ho_recognize_edges_bottom (m_text, m_mask, &num, &left, &middle, &right);
  array_in[26] = (num < 3) ? 0.0 : 1.0;
  array_in[27] = (left < 1) ? 0.0 : 1.0;
  array_in[28] = (middle < 1) ? 0.0 : 1.0;
  array_in[29] = (right < 1) ? 0.0 : 1.0;

  ho_recognize_edges_left (m_text, m_mask, &num, &top, &middle, &bottom);
  array_in[30] = (num < 3) ? 0.0 : 1.0;
  array_in[31] = (top < 1) ? 0.0 : 1.0;
  array_in[32] = (middle < 1) ? 0.0 : 1.0;
  array_in[33] = (bottom < 1) ? 0.0 : 1.0;

  ho_recognize_edges_right (m_text, m_mask, &num, &top, &middle, &bottom);
  array_in[34] = (num < 3) ? 0.0 : 1.0;
  array_in[35] = (top < 1) ? 0.0 : 1.0;
  array_in[36] = (middle < 1) ? 0.0 : 1.0;
  array_in[37] = (bottom < 1) ? 0.0 : 1.0;

  /* fill the notchs part of array */
  ho_recognize_notch_top (m_text, m_mask, &num, &left, &middle, &right);
  array_in[38] = (num < 2) ? 0.0 : 1.0;
  array_in[39] = (left < 1) ? 0.0 : 1.0;
  array_in[40] = (middle < 1) ? 0.0 : 1.0;
  array_in[41] = (right < 1) ? 0.0 : 1.0;

  ho_recognize_notch_bottom (m_text, m_mask, &num, &left, &middle, &right);
  array_in[42] = (num < 2) ? 0.0 : 1.0;
  array_in[43] = (left < 1) ? 0.0 : 1.0;
  array_in[44] = (middle < 1) ? 0.0 : 1.0;
  array_in[45] = (right < 1) ? 0.0 : 1.0;

  ho_recognize_notch_left (m_text, m_mask, &num, &top, &middle, &bottom);
  array_in[46] = (num < 2) ? 0.0 : 1.0;
  array_in[47] = (top < 1) ? 0.0 : 1.0;
  array_in[48] = (middle < 1) ? 0.0 : 1.0;
  array_in[49] = (bottom < 1) ? 0.0 : 1.0;

  ho_recognize_notch_right (m_text, m_mask, &num, &top, &middle, &bottom);
  array_in[50] = (num < 2) ? 0.0 : 1.0;
  array_in[51] = (top < 1) ? 0.0 : 1.0;
  array_in[52] = (middle < 1) ? 0.0 : 1.0;
  array_in[53] = (bottom < 1) ? 0.0 : 1.0;

  /* fill the dimentions part */
  ho_recognize_dimentions (m_text, m_mask, &height, &width, &width_by_height,
			   &start, &end);
  array_in[54] = height / 2.0;
  if (array_in[54] < 0.0)
    array_in[54] = 0.0;
  else if (array_in[54] > 1.0)
    array_in[54] = 1.0;

  array_in[55] = width / 2.0;
  if (array_in[55] < 0.0)
    array_in[55] = 0.0;
  else if (array_in[55] > 1.0)
    array_in[55] = 1.0;

  array_in[56] = width_by_height / 2.0;
  if (array_in[56] < 0.0)
    array_in[56] = 0.0;
  else if (array_in[56] > 1.0)
    array_in[56] = 1.0;

  array_in[57] = 0.5 + start / 2.0;
  if (array_in[57] < 0.0)
    array_in[57] = 0.0;
  else if (array_in[57] > 1.0)
    array_in[57] = 1.0;

  array_in[58] = 0.5 + end / 2.0;
  if (array_in[58] < 0.0)
    array_in[58] = 0.0;
  else if (array_in[58] > 1.0)
    array_in[58] = 1.0;

  /* fill the objects and holes part */
  nobjects = ho_bitmap_filter_count_objects (m_text);
  array_in[59] = (nobjects > 1) ? 1.0 : 0.0;

  m_holes = ho_font_holes (m_text, m_mask);
  if (m_holes)
    {
      nholes = ho_bitmap_filter_count_objects (m_holes);
      array_in[60] = (nholes > 0) ? 1.0 : 0.0;
      array_in[61] = (nholes > 1) ? 1.0 : 0.0;

      ho_bitmap_free (m_holes);
    }
  else
    {
      array_in[60] = 0.0;
      array_in[61] = 0.0;
    }

  /* fill the second object part */
  ho_recognize_second_object (m_text,
			      m_mask,
			      &has_second_objects,
			      &height, &width,
			      &width_by_height, &y_start,
			      &y_end, &x_start, &x_end);

  array_in[62] = has_second_objects;
  if (array_in[62] < 0.0)
    array_in[62] = 0.0;
  else if (array_in[62] > 1.0)
    array_in[62] = 1.0;

  array_in[63] = height;
  if (array_in[63] < 0.0)
    array_in[63] = 0.0;
  else if (array_in[63] > 1.0)
    array_in[63] = 1.0;

  array_in[64] = width;
  if (array_in[64] < 0.0)
    array_in[64] = 0.0;
  else if (array_in[64] > 1.0)
    array_in[64] = 1.0;

  array_in[65] = width_by_height;
  if (array_in[65] < 0.0)
    array_in[65] = 0.0;
  else if (array_in[65] > 1.0)
    array_in[65] = 1.0;

  array_in[66] = y_start;
  if (array_in[66] < 0.0)
    array_in[66] = 0.0;
  else if (array_in[66] > 1.0)
    array_in[66] = 1.0;

  array_in[67] = y_end;
  if (array_in[67] < 0.0)
    array_in[67] = 0.0;
  else if (array_in[67] > 1.0)
    array_in[67] = 1.0;

  array_in[68] = x_start;
  if (array_in[68] < 0.0)
    array_in[68] = 0.0;
  else if (array_in[68] > 1.0)
    array_in[68] = 1.0;

  array_in[69] = x_end;
  if (array_in[69] < 0.0)
    array_in[69] = 0.0;
  else if (array_in[69] > 1.0)
    array_in[69] = 1.0;

  return FALSE;
}

int
ho_recognize_create_array_out (const double *array_in, double *array_out)
{
  int i;

  /* set array out */
  array_out[0] = 0.5;

  for (i = 1; i < HO_ARRAY_OUT_SIZE; i++)
    array_out[i] = ho_recognize_array (array_in, i);

  return FALSE;
}

char *
ho_recognize_array_out_to_font (const double *array_out)
{
  int i = 0;
  int max_i = 0;

  for (i = 1; i < HO_ARRAY_OUT_SIZE; i++)
    if (array_out[i] > array_out[max_i])
      max_i = i;

  return ho_sign_array[max_i];
}

char *
ho_recognize_font (const ho_bitmap * m_text, const ho_bitmap * m_mask)
{
  double array_in[HO_ARRAY_IN_SIZE];
  double array_out[HO_ARRAY_OUT_SIZE];
  char *font;
  int i;

  ho_recognize_create_array_in (m_text, m_mask, array_in);
  ho_recognize_create_array_out (array_in, array_out);
  font = ho_recognize_array_out_to_font (array_out);

  return font;
}

double
ho_recognize_array (const double *array_in, const int sign_index)
{
  int i;
  double return_value = 0.0;

  switch (sign_index)
    {
    case 1:			/* alef */
      return_value = 0.0;
      break;
    case 2:			/* bet */
      return_value = 0.0;;
      break;
    }

  return return_value;
}
