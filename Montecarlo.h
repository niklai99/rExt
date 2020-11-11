#ifndef Montecarlo_h
#define Montecarlo_h

#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include "TGraphErrors.h"
#include <TCanvas.h>
#include <TF1.h>
#include <TH1D.h>
#include <TFitResult.h>
#include <TRandom.h>
#include <TStyle.h>

class Montecarlo{
public:
    Montecarlo(TF1* f, const vector<double> &x, const vector<double> &errX,
               const vector<double> &errY, const int n);
        void drawHist(const int par_n);
        void drawHist(const int par_n, const string par_name);
        void printHist(const int par_n);
private:
    const vector<double> x_real, errX, errY;
    const TF1 *f;
    const int num_iter, num_data, num_par;
    vector<vector<double>> x_toy, y_toy;
    vector<double> y_teo;
    vector<vector<double>> param, param_err, param_rel_err;
    TCanvas *histCanvas;

    void setY_teo();
    void setXY_gaus();
    void fitGeneratedData();
};

#endif
