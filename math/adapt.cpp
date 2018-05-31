/* This file is part of TUNNEX.

 TUNNEX is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 laster any later version.

 TUNNEX is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TUNNEX.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "math.h"
#include "adapt.h"
#include <limits>
#include <complex>

template <class T>
Adapt<T>::Adapt(double tol) : TOL(tol),terminate(true),out_of_tolerance(false)
{
    const double EPS=std::numeric_limits<double>::epsilon();
    if (TOL < 10.0*EPS)
           TOL=10.0*EPS;
}

template <class T>
double Adapt<T>::integrate(std::function<T(T)> func, const double a, const double b) {
    double m,h,fa,fb,i1,i2,is,erri1,erri2,r,y[13]; m=0.5*(a+b);
    h=0.5*(b-a);
    fa=y[0]=func(a);
    fb=y[12]=func(b);
    for (int i=1;i<12;i++)
        y[i]=func(m+x[i]*h);

     i2=(h/6.0)*(y[0]+y[12]+5.0*(y[4]+y[8]));
     i1=(h/1470.0)*(77.0*(y[0]+y[12])+432.0*(y[2]+y[10])+
        625.0*(y[4]+y[8])+672.0*y[6]);
     is=h*(0.0158271919734802*(y[0]+y[12])+0.0942738402188500*
        (y[1]+y[11])+0.155071987336585*(y[2]+y[10])+ 0.188821573960182*(y[3]+y[9])+0.199773405226859*
        (y[4]+y[8])+0.224926465333340*(y[5]+y[7])+
        0.242611071901408*y[6]); erri1=fabs(i1-is);

    erri2=fabs(i2-is);
    r=(erri2 != 0.0) ? erri1/erri2 : 1.0; toler=(r > 0.0 && r < 1.0) ? TOL/r : TOL;
    if (is == 0.0)
        is=b-a;

    is=fabs(is);
    return adaptlob(func,a,b,fa,fb,is);
}

template <class T>
double Adapt<T>::adaptlob(std::function<T(T)> func, const double a, const double b, const double fa,
const double fb, const double is)
{
    double m,h,mll,ml,mr,mrr,fmll,fml,fm,fmrr,fmr,i1,i2;
    m=0.5*(a+b);
    h=0.5*(b-a);
    mll=m-alpha*h;
    ml=m-beta*h;
    mr=m+beta*h;
    mrr=m+alpha*h;
    fmll=func(mll);
    fml=func(ml);
    fm=func(m);
    fmr=func(mr);
    fmrr=func(mrr);
    i2=h/6.0*(fa+fb+5.0*(fml+fmr)); i1=h/1470.0*(77.0*(fa+fb)+432.0*(fmll+fmrr)+625.0*(fml+fmr)+672.0*fm);
    if (fabs(i1-i2) <= toler*is || mll <= a || b <= mrr) {
        if ((mll <= a || b <= mrr) && terminate) {
           out_of_tolerance=true;
           terminate=false;
        }
        return i1;
    }
    else
        return adaptlob(func,a,mll,fa,fmll,is)+
               adaptlob(func,mll,ml,fmll,fml,is)+
               adaptlob(func,ml,m,fml,fm,is)+
               adaptlob(func,m,mr,fm,fmr,is)+
               adaptlob(func,mr,mrr,fmr,fmrr,is)+
               adaptlob(func,mrr,b,fmrr,fb,is);
}
template<class T>
const double Adapt<T>::alpha=sqrt(2.0/3.0);
template<class T>
const double Adapt<T>::beta=1.0/sqrt(5.0);
template<class T>
const double Adapt<T>::x1=0.942882415695480;
template<class T>
const double Adapt<T>::x2=0.641853342345781;
template<class T>
const double Adapt<T>::x3=0.236383199662150;
template<class T>
const double Adapt<T>::x[12]={0,-x1,-alpha,-x2,-beta,-x3,0.0,x3,beta,x2,alpha,x1};

template MATH_LIB_DECL Adapt<double>::Adapt(double tol);
template MATH_LIB_DECL double Adapt<double>::integrate(std::function<double(double)> func, const double a, const double b);
template MATH_LIB_DECL double Adapt<double>::adaptlob(std::function<double(double)> func, const double a, const double b, const double fa, const double fb, const double is);


