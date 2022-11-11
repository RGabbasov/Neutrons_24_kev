{
   char filename[14],treename[7],entry[1];
   printf("%s","input filename:        ");
   scanf("%s",filename);
   printf("%s","Set entry:        ");
    scanf("%s",entry);
   Int_t ent=atoi(entry);
   TFile *f = new TFile(filename,"READ");
   TTree *t1= (TTree*)f->Get("T");
   TBranch *b1=t1->GetBranch("neutron");
   TBranch *b2=t1->GetBranch("signal");
   TBranch *b3=t1->GetBranch("ev");
const Int_t m=2000;
 typedef struct {
Int_t numberph;
Int_t sig0[m];
Int_t sig1[m];
Int_t sig2[m];
Int_t sig3[m];
Int_t sig4[m];
Int_t sig5[m];
Int_t sig6[m];
} Signal_st;

typedef struct {
Double_t energy_i;
Double_t energy_f;
} Neutron_inf;

   TCanvas *c2 = new TCanvas("c2", "HpXe",0,0,681,696);
   c2->Range(500,101.956,1700,1898.04);
   c2->SetFillColor(19);
   c2->SetBorderSize(2);
   c2->SetGridx();
   c2->SetGridy();
   c2->SetLeftMargin(0.11476);
   c2->SetRightMargin(0.0866133);
   c2->SetFrameFillColor(10);

   TH1I *h10 = new TH1I("h10","proZ3",m,0,m);
   TH1I *h11 = new TH1I("h11","proZ3",m,0,m);
   TH1I *h12 = new TH1I("h12","proZ3",m,0,m);
   TH1I *h13 = new TH1I("h13","proZ3",m,0,m);
   TH1I *h14 = new TH1I("h14","proZ3",m,0,m);
   TH1I *h15 = new TH1I("h15","proZ3",m,0,m);
   TH1I *h16 = new TH1I("h16","proZ3",m,0,m);
  TH1F *h20 = new TH1F("h20","proZ3",m,0,m);
  TH1F *h21 = new TH1F("h21","proZ3",m,0,m);
  TH1F *h22 = new TH1F("h22","proZ3",m,0,m);
  TH1F *h23 = new TH1F("h23","proZ3",m,0,m);
  TH1F *h24 = new TH1F("h24","proZ3",m,0,m);
  TH1F *h25 = new TH1F("h25","proZ3",m,0,m);
  TH1F *h26 = new TH1F("h26","proZ3",m,0,m);

   h10->SetLineColor(7);
   h11->SetLineColor(8);
   h12->SetLineColor(2);
   h13->SetLineColor(3);
   h14->SetLineColor(4);
   h15->SetLineColor(13);
   h16->SetLineColor(14);
   
  Signal_st sig_st;
  Neutron_inf neut_st;
 Int_t sp0[m],sp1[m],sp2[m],sp3[m],sp4[m],sp5[m],sp6[m];
 //Int_t* tr1,tr2,tr3,tr4,tr5,tr6;
 Int_t s1=0,s2=0;

  Int_t i,j,k,event;
   b1->SetAddress(&neut_st);
   b2->SetAddress(&sig_st);
   b3->SetAddress(&event);
cout <<"eentry   "<<ent<<endl;
b2->GetEntry(ent);

for( i=0;i<m;i++)
 {
  sp0[i]=sig_st.sig0[i];

  sp1[i]=sig_st.sig1[i];
  sp2[i]=sig_st.sig2[i];
  sp3[i]=sig_st.sig3[i];
  sp4[i]=sig_st.sig4[i];
  sp5[i]=sig_st.sig5[i];
  sp6[i]=sig_st.sig6[i];
  h10->SetBinContent(i,sig_st.sig0[i]);
  h11->SetBinContent(i,sig_st.sig1[i]);
  h12->SetBinContent(i,sig_st.sig2[i]);
  h13->SetBinContent(i,sig_st.sig3[i]);
  h14->SetBinContent(i,sig_st.sig4[i]);
  h15->SetBinContent(i,sig_st.sig5[i]);
  h16->SetBinContent(i,sig_st.sig6[i]);
 }
Int_t* tr0=trigger(sp1);
//Int_t* tr1=trigger(sp1);
//Int_t* tr2=trigger(sp2);
//Int_t* tr3=trigger(sp3);
//Int_t* tr4=trigger(sp4);
//Int_t* tr5=trigger(sp5);
//Int_t* tr6=trigger(sp6);
for (j=0;j<m;j++)
 {
  h20->SetBinContent(j,tr0[j]);
  //h21->SetBinContent(j,tr1[j]);
  //h22->SetBinContent(j,tr2[j]);
  //h23->SetBinContent(j,tr3[j]);
 // h24->SetBinContent(j,tr4[j]);
 // h25->SetBinContent(j,tr5[j]);
 // h26->SetBinContent(j,tr6[j]);
//cout <<"ggg   "<<i<<"   "<<sp0[i]<<endl;
  }


h10->Draw();
h11->Draw("Same");
h12->Draw("Same");
h13->Draw("Same");
h14->Draw("Same");
h15->Draw("Same");
h16->Draw("Same");
//f->Close();
//delete f;
}