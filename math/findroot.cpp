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
#include <math.h>
#include "findroot.h"
#include <exception>
#include <string>

struct myErr : public std::exception{myErr(const char* c) throw(); const char *msg; virtual const char *what() const throw();};

myErr::myErr (const char *c) throw() : msg(c){

}

const char* myErr::what() const throw(){
    return msg;
}

template<class T,class D> FindRoot<T,D>::FindRoot()
{
}

template<class T,class D> bool FindRoot<T,D>::zbrac(std::function<T(D)> func, double &x1, double &x2) throw() {
    const int NTRY=50;
    const double FACTOR=1.6;
    if(x1 == x2)
        throw myErr("Bad initial range in zbrac");
    double f1=func(x1);
    double f2=func(x2);
    for (int j=0; j < NTRY; j++){
        if (f1*f2 < 0.0)
            return true;
        if(fabs(f1) < fabs(f2))
            f1=func(x1 += FACTOR*(x1-x2));
        else
            f2=func(x2 += FACTOR*(x2-x1));
    }
    return false;
}

template<class T,class D> double FindRoot<T,D>::rtbis(std::function<T(D)> func, const double x1, const double x2, const double xacc) throw() {
    const int JMAX=50;
    double dx, xmid, rtb;
    double f=func(x1);
    double fmid=func(x2);
    if(f*fmid >= 0.0)
        throw myErr("Root must be bracketed for bisection in rtbis");
    rtb=f < 0.0 ? (dx=x2-x1,x1) : (dx=x1-x2,x2);
    for (int j=0; j < JMAX; j++){
        fmid=func(xmid=rtb+(dx*=0.5));
        if(fmid <= 0.0)
            rtb=xmid;
        if(fabs(dx) < xacc || fmid == 0.0)
            return rtb;
    }
    throw myErr("Too many bisections in rtbis");
}

template<class T,class D> double FindRoot<T,D>::rtflsp(std::function<T(D)> func, const double x1, const double x2, const double xacc) throw(){
    auto SWAP=[](double a, double b){double temp=a; a=b; b=temp;};
    const int MAXIT=30;
    double xl,xh,del;
    double fl=func(x1);
    double fh=func(x2);
    if( fl*fh > 0.0)
        throw myErr("Root must be bracketed in rtflsp");
    if(fl < 0.0){
        xl=x1;
        xh=x2;
    }
    else {
        xl=x2;
        xh=x1;
        SWAP(fl,fh);
    }
    double dx=xh-xl;
    for(int j=0; j < MAXIT; j++){
        double rtf=xl+dx*fl/(fl-fh);
        double f=func(rtf);
        if(f < 0.0){
            del=xl-rtf;
            xl=rtf;
            fl=f;
        }
        else {
            del=xh-rtf;
            xh=rtf;
            fh=f;
        }
        dx=xh-xl;
        if(fabs(del) < xacc || f == 0.0)
            return rtf;
    }
    throw myErr("Maximum number of iterations exceeded in rtflsp");
}


template class FindRoot<double,double>;
