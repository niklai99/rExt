#include "fit.h"


fit::fit(const double* x_, const double* y_, const double *err_y_, const int n):
    x(x_), y(y_), err_y(err_y_), n(n) {
    /*arguments:
     * - array with x data
     * - array with y data
     * - array with y errors
     * - number of elements in each array
     * NOTE: results will NOT be printed*/
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

fit::fit(const double* x_, const double* y_, const double *err_y, const int n, const char printRes):
    fit(x_, y_, err_y, n){
    /*arguments:
     * - array with x data
     * - array with y data
     * - array with y errorsi
     * - number of elements in each array
     * - char 'F' or 'S' for printing the results on a 'F'ile or on the 'S'creen
     * NOTE: printing on screen will also produce a Graph and will therefore need
     * to be launched from Root*/
    if(printRes == 'F')
        printOnFile();
    else if(printRes == 'S')
        printOnScreen();
}

fit::fit(const double *x_, const double* y_, const int n):
    x(x_), y(y_), n(n){
    /*arguments:
     * - array with x data
     * - array with y data
     * - number of elements in each array
     * NOTE: this fit is NOT weighted and will NOT print result*/
    err_y = new double[n];
    for(int i = 0; i < n; i++){
       err_y[i] = 1.;
    }
    setSums();
    setDelta();
    setPar_a();
    setErrPost();
    setR();
    setTstud();
}

fit::fit(const string fname, const int numCol){
    /*arguments:
     * - file name
     * - number of columns stored in the file:
     *   accepted values are either 3 (x, y, errY) or
     *   4 (x, y , errX, errY)*/
    ifstream f(fname);
    double i = 0;
    vector<double> x_, y_, errX_, errY_;
    while(f>>i){
        x_.push_back(i);
        f >> i;
        y_.push_back(i);
        f >> i;
        if(numCol == 4){
            errX_.push_back(i);
            f >> i;
        }
        else if(numCol == 3)
            errX_.push_back(0);
        errY_.push_back(i);
    }
    fit(&x_[0], &y_[0], &errY_[0], x_.size());
    //TODO: add second fit with xerrors projected onto the y
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
    cout << "ChiSqu\t" << chisq << endl;
}

void fit::printOnFile(){
    ofstream f("fitResult.txt");
    f << "Num of entries: " << n << endl;
    f << "Par a\t" << par_a << "\t +/-" << err_a << endl;
    f << "Par b\t" << par_b << "\t +/-" << err_b << endl;
    f << "sigmaP\t" << err_post << endl;
    f << "correl\t" << r << endl;
    f << "t-Stud\t" << tStud << endl;
    f << "ChiSqu\t" << chisq << endl;
}

void fit::setChisq(){
    double temp = 0;
    for(int i = 0; i < n; i++)
        temp += pow(y[i] - par_b*x[i] - par_a,2)/pow(err_y[i],2);
    chisq = temp;
}
