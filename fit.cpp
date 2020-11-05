#include "fit.h"


fit::fit(double* x_, double* y_, double *err_y_, int n):
    x(x_), y(y_), err_y(err_y_), n(n) {
    setSums();
    setDelta();
    setPar_a();
    setPar_b();
    setErrParA();
    setErrParB();
    setErrPost();
    setR();
    setTstud();
}

fit::fit(double* x_, double* y_, double *err_y, int n, char printRes):
    fit(x_, y_, err_y, n){
    if(printRes == 'F')
        printOnFile();
    else if(printRes == 'S')
        printOnScreen();
}

fit::fit(double *x_, double* y_, int n){
    //todo
}
void fit::setSums(){
    for(int i = 0; i < n; i ++){
        sumX += x[i];
        sumY += y[i];
        sumXsq += pow(x[i],2);
        sumYsq += pow(y[i],2);
        sumXY += x[i]*y[i];
    }
}
void fit::setDelta(){
    delta = n*sumXsq - pow( sumX,2);
}

void fit::setPar_a(){
    par_a = ( sumXsq * sumY - sumX * sumXY )/delta;
}

void fit::setErrPost(){
    double temp_err = 0;
    for(int i = 0; i < n; i++)
        temp_err += pow((par_a + par_b*x[i] - y[i]), 2);
    err_post =  sqrt(temp_err / (n-2));
}

void fit::setPar_b(){
    par_b = ( sumXY * n - sumX*sumY )/ delta;
}

void fit::setErrParA(){
    err_a = err_post*sqrt(sumXsq/delta);
}

void fit::setErrParB(){
    err_b = err_post*sqrt(n/delta);
}

double fit::getGeneralSetAverage(double* data){
    double temp_av = 0;
    for(int i = 0; i < n; i++)
        temp_av += data[i];
    return temp_av / n;
}

double fit::getGeneralSetError(double* data){
    double temp_err = 0;
    double avg = getGeneralSetAverage(data);
    for(int i = 0; i < n; i++)
        temp_err += pow(data[i] - avg, 2);
    return sqrt(temp_err / n);
}

void fit::setR(){
    double* xy = new double[n];
    for(int i = 0; i < n; i++)
        xy[i] = x[i]*y[i];
    double avg_x = getGeneralSetAverage(x);
    double avg_y = getGeneralSetAverage(y);
    double avg_xy = getGeneralSetAverage(xy);
    double sigma_x = getGeneralSetError(x);
    double sigma_y = getGeneralSetError(y);
    r = ( avg_x * avg_y - avg_xy ) / (sigma_x*sigma_y);
    delete xy;
}

void fit::setTstud(){
    tStud = (r * sqrt(n-2)) / sqrt(1 - pow(r,2));
}

void fit::printOnScreen(){
    cout << "____________________" << endl;
    cout << "Printing fit results" << endl;
    cout << "Num of entries: " << n << endl;
    cout << "Par a\t" << par_a << "\t +/-" << err_a << endl;
    cout << "Par b\t" << par_b << "\t +/-" << err_b << endl;
    cout << "sigmaP\t" << err_post << endl;
    cout << "correl\t" << r << endl;
    cout << "t-Stud\t" << tStud << endl;
}

void fit::printOnFile(){
    //todo
}
