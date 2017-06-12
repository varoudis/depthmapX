// genlib - a component of the depthmapX - spatial network analysis platform

// Paf Template Library --- a set of useful C++ templates
//
// Copyright (c) 1996-2011 Alasdair Turner (a.turner@ucl.ac.uk)
//
//-----------------------------------------------------------------------------
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
//  See the lgpl.txt file for details
//-----------------------------------------------------------------------------

// a collection of math functions

#ifndef __PAFMATH_H__
#define __PAFMATH_H__

#include <genlib/paftl.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

inline double sqr(double a)
{
   return (a*a);
}

inline int sgn(double a)
{
   return (a < 0) ? -1 : 1;
}

#ifndef M_ROOT_1_2
#define M_ROOT_1_2 0.70710678118654752440084436210485
#endif

#ifndef M_1_LN2
#define M_1_LN2 1.4426950408889634073599246810019
#endif

const unsigned int PAF_RAND_MAX = 0x0FFFFFFF;
void pafsrand(unsigned int seed, int set = 0);
unsigned int pafrand(int set = 0);

// a random number from 0 to 1
inline double prandom(int set = 0)
{
   return double(pafrand(set)) / double(PAF_RAND_MAX);
}

// a random number from 0 to just less than 1

inline double prandomr(int set = 0)
{
   return double(pafrand(set)) / double(PAF_RAND_MAX + 1);
}

// note, in order to stop confusing myself I have ln defined:
#define ln(X) log(X)

inline double log2(double a)
{
   return (ln(a) * M_1_LN2);
}

// Hillier Hanson dvalue
/*
inline double dvalue(double k)
{
   return 2.0 * (3.3231 * k * log10(k+2) - 2.5863 * k + 1.0) / ((k - 1.0) * (k - 2.0));
}
*/

// Hillier Hanson dvalue (from Kruger 1989 -- see Teklenburg et al)
inline double dvalue(double k)
{
   return 2.0 * (k * (log2((k+2.0)/3.0) - 1.0) + 1.0) / ((k - 1.0) * (k - 2.0));
}

// Hillier Hanson pvalue
inline double pvalue(double k)
{
   return 2.0 * (k - log2(k) - 1.0) / ((k - 1.0) * (k - 2.0));
}

// Teklenburg integration (correction 31.01.11 due to Ulrich Thaler
inline double teklinteg(double nodecount, double totaldepth)
{
   return ln(0.5 * (nodecount - 2.0)) / ln(double(totaldepth - nodecount + 1));
}

// Penn palmtree

inline double palmtree(double n, double r)
{
   if (n > r) {
      return r * (n - 0.5 * (r+1));
   }
   else {
      return 0.5 * n * (n - 1);
   }
}

double poisson(int x, double lambda);
double cumpoisson(int x, double lambda);
int invcumpoisson(double p, double lambda);

// linear regression
// T should be int, float or double
template <class T> struct LinReg
{
   double S_x;
   double S_y;
   double S_xx;
   double S_yy;
   double S_xy;
   double n;
   //
   bool cached;
   double ca;
   double cb;
   //
   LinReg()
   {
      S_x = 0; S_y = 0; S_xx = 0; S_yy = 0; S_xy = 0; n = 0; cached = false;
   }
   LinReg(const pvector<T>& X, const pvector<T>& Y)
   {
      S_x = 0; S_y = 0; S_xx = 0; S_yy = 0; S_xy = 0;
      n = __min(X.size(),Y.size());
      for (int i = 0; i < n; i++) {
         S_x += double(X[i]); S_y += double(Y[i]);
         S_xx += sqr(double(X[i])); S_yy += sqr(double(Y[i]));
         S_xy += double(X[i]) * double(Y[i]);
      }
      cached = false;
   }
   void clear()
   {
      S_x = 0; S_y = 0; S_xx = 0; S_yy = 0; S_xy = 0; n = 0;
      cached = false;
   }
   void add(T x, T y)
   {
      n++;
      S_x += double(x); S_y += double(y);
      S_xx += sqr(double(x)); S_yy += sqr(double(y));
      S_xy += double(x) * double(y);
      cached = false;
   }
   void makecache()
   {
      cb = (n*S_xy-S_x*S_y)/(n*S_xx-S_x*S_x); ca = (S_y - cb * S_x) / n; cached = true;
   }
   // Y = bX + a
   double a()
   { if (!cached) makecache(); return ca; }
   double b()
   { if (!cached) makecache(); return cb; }
   double r()
   { return (n*S_xy-S_x*S_y)/sqrt( (n*S_xx-sqr(S_x))*(n*S_yy-sqr(S_y)) ); }
   double mu_x()
   { return S_x / n; }
   double mu_y()
   { return S_y / n; }
   // note you will need to keep the vectors if you need
   // to calculate the residuals: e[i] = y[i] - b() * x[i] - a();
   double model(double x)
   { if (!cached) makecache(); return cb * x + ca; }
   double invmodel(double y)
   { if (!cached) makecache(); return (y -ca) / cb ; }
};

#endif
