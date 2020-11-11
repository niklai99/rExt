/*
** Created by Simone Cigagna
**
** This class is for linear fits only.
** See constructors for a detailed explenation on how to use this.
**
*/

#ifndef Fit_h
#define Fit_h

#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class fit{
    public:
        /* CONSTRUCTORS */
        fit(const double* x_, const double* y_, const double *err_y_, const int n);
        fit(const double* x_, const double* y_, const int n);
        fit(const double* x_, const double* y_, const double* err_y_, const int n, const char printRes);
        fit(const string fname, const int numCol);
        /* GET */
        const double* getX() const {return x;};
        const double* getY() const {return y;};
        const double* getErrY() const {return err_y;};
        double getPar_a() const {return par_a;};
        double getPar_b() const {return par_b;};
        // missing stuff
        // missing destructor

    private:
        /* VARIABLES */
        //stuff to make the code shorter
        double sumX, sumY, sumXsq, sumYsq, sumXY;
        //fit results
        double delta, par_a, par_b, err_a, err_b;
        double r, err_post, tStud, chisq;
        //data
        double *x,*y;
        double *err_y;
        int n;

        /* FUNCTIONS */
        //set fit results
        void setSums();
        void setDelta();
        void setPar_a(); void setPar_b(); void setErrParA(); void setErrParB();
        void setR(); void setErrPost(); void setTstud();
        void setChisq();
        double getGeneralSetAverage(double* data);
        double getGeneralSetError(double* data);
        //print results
        void printOnFile();
        void printOnScreen();
};

#endif
