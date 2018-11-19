// compile with:  g++ -o mezon mezon.cc `root-config --cflags --glibs` 

#include <cstdlib> 
#include <iostream> 
#include "TRandom.h"  
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TRandom3.h"
#include "TApplication.h"
#include "TMath.h"
#include <cmath>
#include "TFile.h"
#include "THStack.h"


using namespace std;


double fRand(double fMin, double fMax)
{
    double f = (double)rand() / (RAND_MAX+1.0);
    return fMin + f * (fMax - fMin);
}

Double_t myF(Double_t x)
{
  Double_t dm=1/(1.2*10);
  Double_t gs= 1/(0.9); 
  Double_t gl= 1/(0.5*1000);
  Double_t wynik;
 
  wynik=(1000000/4)*(TMath::Exp(-gs*x) + TMath::Exp(-gl*x) - 2*TMath::Exp(-(gs + gl)*x/2) * TMath::Cos(dm*x));

  return wynik;
}

Double_t myF2(Double_t x) 
{
  Double_t dm= 1/(1.2*10);
  Double_t gs= 1/(0.9);
  Double_t gl= 1/(0.5*1000);
  Double_t wynik;
 
  wynik=(1000000/4)*((TMath::Exp(-gs*x)) + TMath::Exp(-gl*x) + 2*TMath::Exp(-(gs + gl)*x/2) * TMath::Cos(dm*x));

  return wynik;

}

Double_t generator2(TRandom3& rand2)
{
 Double_t x= rand2.Uniform(0,4000);
 Double_t y= rand2.Uniform(0,1); 
 Double_t f = myF2(x);
 Double_t y2=y*1000000;
 
 if(y2<f) return x; 
 return -1; 
}



Double_t generator(TRandom3& rand)
{
 Double_t x= rand.Uniform(0,4000);
 Double_t y= rand.Uniform(0,0.25); 
 Double_t f = myF(x);
 Double_t y2=y*1000000;
 
 if(y2<f) return x; 
 return -1; 
}

void format_h(TH1F* h, int linecolor){
    h->SetLineWidth(3);
    h->SetLineColor(linecolor);
    }


int main(int argc, char **argv)
{

if(argc!=2){
    printf("unexpected number of arguments\n");
    return -1;
}
  
  TString home_path=Form(argv[1]);
  TApplication theApp("App",0,0);
  TFile outfile(home_path.Data(),"recreate");
  
  THStack *hs = new THStack("hs","");
  TH1 *h1 = new TH1F("h1","histogram decay K0bar",1000,0,4000);
  TH1 *h2 =	new TH1F("h2","histogram decay K0",1000,0,4000);
  
 
  TCanvas *cs = new TCanvas("cs","transparent pad",200,10,700,500);
  cs->cd();
  
  
  TRandom3 rand, rand2;	
  Double_t result=0, result2=1;
  int i=0, m=0;
  
  while(m<100000 || i<100000) {
  
   if(i<100000){
   result = generator(rand);
   printf("%f\n",result);
   
    if (result < 0){}else{
	  h1->Fill(result);
	  i++;
       }}

   if(m<100000){
	
   result2 = generator2(rand2);
   printf("%f\n",result2);
   
    if (result2 < 0){}else{
	  h2->Fill(result2);
	  m++;
       }}
	
	
	  
  }	    
  
     hs->Add(h1);
     hs->Add(h2);
     hs->Write();
     
   outfile.Write();
   outfile.Close();
   
  theApp.Run();
  
  return 0;
}



