#include "Montecarlo.h"

Montecarlo::Montecarlo(TF1* f, const vector<double> &x, const vector<double> &errX,
                   const vector<double> &errY, const int n):
    x_real(x), errX(errX), errY(errY), num_iter(n), f(f), num_data(x.size()), num_par(f->GetNpar())  {

    // prepare vectors
    param.resize(num_par); param_err.resize(num_par); param_rel_err.resize(num_par);

    // set teoretical y values
    setY_teo();
    // set (x,y) with gaussian noise 'num_iter' times
    setXY_gaus();
    // fit all data
    fitGeneratedData();
}

void Montecarlo::setY_teo(){
    for(double i: x_real)
        y_teo.push_back(f->Eval(i));
}
void Montecarlo::setXY_gaus(){
    TRandom *g = new TRandom();
    for(int j = 0; j < num_data; j++){
        for(int i = 0; i < num_iter; i++){
            y_toy[i].push_back(g->Gaus(y_teo[j], errY[j]));
            x_toy[i].push_back(g->Gaus(x_real[j], errX[j]));
        }
    }
    delete g;
}

void Montecarlo::fitGeneratedData(){
    for(int i = 0; i < num_iter; i++){
        TGraphErrors *gr1 = new TGraphErrors(num_data, &x_toy[i][0], &y_toy[i][0], &errX[0], &errY[0]);
        TFitResultPtr result = gr1->Fit(f->GetName(), "SQ");
        for(int j = 0; j < param.size(); j++){
            double par_temp = result->Parameter(j);
            double par_err_temp = result->ParError(j);
            param[j].push_back(par_temp);
            param_err[j].push_back(par_err_temp);
            param_rel_err[j].push_back(par_err_temp / abs(par_temp));
        }
        delete gr1;
    }
}

void Montecarlo::drawHist(const int par_n){
    histCanvas = new TCanvas ("canvas1", "canvas1", 1080, 720);
    histCanvas->Divide(2,0);
    histCanvas->cd(1);
    TH1D *hist = new TH1D("par", "Distribuzione del parametro ", 50, 9.984, 9.992);
    for(double i: param[par_n])
        hist->Fill(i);
    hist->Draw();
    hist->Fit("gaus","Q");
    gPad->Modified();
    hist->GetXaxis()->SetTickLength(0.02);
    hist->GetYaxis()->SetTickLength(0.02);
    gStyle->SetStripDecimals(kFALSE);
    histCanvas->cd(2);

    TH1D *hist1 = new TH1D("#sigma_{slope}/slope distr", "Distribuzione di #sigma_{slope}/slope; #sigma_{slope}/slope; counts", 50, 0.0072, 0.0076);
    for(double i: param_rel_err[par_n])
        hist1->Fill(i);
    hist1->Draw();
    hist1->Fit("gaus", "Q");
    gPad->Modified();
    hist1->GetXaxis()->SetTickLength(0.02);
    hist1->GetYaxis()->SetTickLength(0.02);
    hist1->GetXaxis()->SetMaxDigits(6);
}
