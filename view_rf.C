{
   char filename[14];
   printf("%s","filename:             ");
   scanf("%s",filename);
   TFile *f = new TFile(filename,"READ");
   TCanvas *c2 = new TCanvas("c2", "HpXe",0,0,681,696);
   c2->Range(500,101.956,1700,1898.04);
   c2->SetFillColor(19);
   c2->SetBorderSize(2);
   c2->SetGridx();
   c2->SetGridy();
   c2->SetLeftMargin(0.11476);
   c2->SetRightMargin(0.0866133);
   c2->SetFrameFillColor(10);

}