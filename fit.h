#ifndef Fit_h
#define Fit_h

#include <iostream>
#include <math.h>
#include <fstream>

using namespace std;

class fit{
public:
    fit(double* x_, double* y_, double *err_y_, int n);
    fit(double* x_, double* y_, int n);
    fit(double* x_, double* y_, double* err_y_, int n, char printRes);
    double* getX() const {return x;};
    double* getY() const {return y;};
    double* getErrY() const {return err_y;};
    double getPar_a() const {return par_a;};
    double getPar_b() const {return par_b;};
private:
    double sumX, sumY, sumXsq, sumYsq, sumXY;
    double delta, par_a, par_b, err_a, err_b;
    double *x,*y, *err_y;
    int n;
    double r, err_post, tStud;
    void setSums();
    void setDelta();
    void setPar_a(); void setPar_b(); void setErrParA(); void setErrParB();
    void setR(); void setErrPost(); void setTstud();
    double getGeneralSetAverage(double* data);
    double getGeneralSetError(double* data);
    void printOnFile();
    void printOnScreen();
};

#endif
