 Int_t trigger(Int_t * sign)
{
 const Int_t m=2000;
 TH1I *ht1 = new TH1I("ht1","proZ3",m,0,m);
 TH1F *ht2 = new TH1F("ht2","proZ3",m,0,m);
   ht1->SetLineColor(7);
   ht2->SetLineColor(8);;

 Int_t spec[m],si[m+600];
 Int_t s1=0,s2=0;
 Int_t i,j;
  
for (j=0;j<m+300;j++)
 {
	if ((j>=300) && (j<m+300))
	  {
	  si[j]=sign[j-300];
	  ht1->SetBinContent(j-300,sign[j-300]);
	  }
		
	else 
	si[j]=0;
 }

for (i=0;i<m;i++)
 {
 s1=s1+si[i+100]-si[i];
 s2=s2+si[i+300]-si[i+200];
 spec[i]=s1-s2;
//cout <<"fff  "<<i<<"  "<<spec[i]<<"    "<<sign[j-300]<<"  "<< endl;
 ht2->SetBinContent(i,spec[i]);
}
return spec;
//h2->Draw();
//h1->Draw("same");
}