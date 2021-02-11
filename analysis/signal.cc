// Int_t* make_pulse();
// void gen_signal(TH1D*, Int_t*);
// void add_phot(TH1D* , Double_t , Int_t*);


void make_pulse(Int_t *out_pulse){
	// TFile *input = new TFile("../../tl-data/root/data_20210202_145240.root");
	TFile *input = new TFile("../../tl-data/root/data_20210208_160910_wf.root");
	TTree *tree = (TTree*)input->Get("T");

	Int_t Pulse1[60];
	Int_t Qtot1 = 0;

	tree->SetBranchAddress("Pulse1", Pulse1);
	tree->SetBranchAddress("Qtot1", &Qtot1);


	// Long_t nEn = tree->GetEntries();

	tree->GetEntry(35257);

	// return Pulse1;

	for (Int_t i = 0; i < 60; ++i){
		out_pulse[i] = Pulse1[i];
	}

	delete input, tree;
}


void gen_signal(TH1D *h_sig_old, Int_t *Pulse1){
	// gROOT->Reset();


	// std::vector<std::vector<Int_t> > pulses;

	
	for (Int_t bin = 1; bin <= 60; ++bin){
		h_sig_old->SetBinContent(bin, Pulse1[bin-1]);

	}

	// pedestal correction
	Int_t nPed = 20;
	Double_t ped_before = 0;
	// for (Int_t bin = 1; bin <= nPed; ++bin){
	// 	ped_before += h_sig_old->GetBinContent(bin);
	// }
	// ped_before /= nPed;


	for (Int_t bin = 1; bin <= 60; ++bin){
		h_sig_old->SetBinContent(bin, h_sig_old->GetBinContent(bin) - ped_before);
	}
	


}

void add_phot(TH1D* sig_tot, Double_t start_time, Double_t amplitude,  Int_t* Pulse1){
	TH1D *this_phot = new TH1D("sig","sig", 60, 0 + (start_time - 85), 240 + (start_time - 85));
	gen_signal(this_phot, Pulse1);



	for (Int_t bin_tot = 1; bin_tot <= sig_tot->GetNbinsX(); ++bin_tot){
		Int_t bin_sig = this_phot->FindBin(sig_tot->GetBinLowEdge(bin_tot));
		if (bin_sig > 0 && bin_sig <= this_phot->GetNbinsX()) 
			sig_tot->SetBinContent(bin_tot, 
				sig_tot->GetBinContent(bin_tot) +
				this_phot->GetBinContent(bin_sig) *amplitude);
	}

	delete this_phot;

}


void sig_ana(TH1D *sig_tot, Double_t results[5]){
	Double_t threshold = 1000.;
	Int_t Ntot = 50*4;
	Int_t Ntail = 40*4;
	Double_t sig_i, sig_i1;
	Double_t t_i, t_i1;
	Double_t t_mu = -1.;
	Double_t t_e = -1.;

	// sig_tot->Draw();
	// return;


	for (Int_t bin = 1; bin <= sig_tot->GetNbinsX(); ++bin){
		sig_i = sig_tot->GetBinContent(bin);
		sig_i1 = sig_tot->GetBinContent(bin+1);
		t_i = sig_tot->GetBinLowEdge(bin);
		t_i1 = sig_tot->GetBinLowEdge(bin+1);


		if (threshold >= sig_i && threshold < sig_i1) {
			t_mu = t_i + (threshold - sig_i)*(t_i1 - t_i) / (sig_i1 - sig_i);
			break;			
		}
	}

	for (Int_t bin = sig_tot->FindBin(t_mu + 280.); bin <= sig_tot->GetNbinsX(); ++bin){
		sig_i = sig_tot->GetBinContent(bin);
		sig_i1 = sig_tot->GetBinContent(bin+1);
		t_i = sig_tot->GetBinCenter(bin);
		t_i1 = sig_tot->GetBinCenter(bin+1);


		if (threshold >= sig_i &&  threshold < sig_i1) {
			t_e = t_i + (threshold - sig_i)*(t_i1 - t_i) / (sig_i1 - sig_i);
			
			break;			
		}
	}

	std::cout << t_mu << " " << t_e << std::endl;

	TAxis *sig_axis = sig_tot->GetXaxis();

	Double_t xmin_tot = t_mu;
	Double_t xmax_tot = t_mu + Ntot;
	Int_t bmin_tot = sig_axis->FindBin(xmin_tot);
	Int_t bmax_tot = sig_axis->FindBin(xmax_tot);

	Double_t xmin_tail = t_mu + (Ntot - Ntail);
	Double_t xmax_tail = t_mu + Ntail;
	Int_t bmin_tail = sig_axis->FindBin(xmin_tail);
	Int_t bmax_tail = sig_axis->FindBin(xmax_tail);

	results[0] = sig_tot->Integral(bmin_tot, bmax_tot);
	results[0] -= sig_tot->GetBinContent(bmin_tot)*
		(xmin_tot-sig_axis->GetBinLowEdge(bmin_tot))
		/ sig_axis->GetBinWidth(bmin_tot);
	results[0] -= sig_tot->GetBinContent(bmax_tot)*
		(sig_axis->GetBinUpEdge(bmax_tot)-xmax_tot)/
		sig_axis->GetBinWidth(bmax_tot);


	results[1] = sig_tot->Integral(bmin_tail, bmax_tail);
	results[1] -= sig_tot->GetBinContent(bmin_tail)*
		(xmin_tail-sig_axis->GetBinLowEdge(bmin_tail))
		/ sig_axis->GetBinWidth(bmin_tail);
	results[1] -= sig_tot->GetBinContent(bmax_tail)*
		(sig_axis->GetBinUpEdge(bmax_tail)-xmax_tail)/
		sig_axis->GetBinWidth(bmax_tail);

	xmin_tot = t_e;
	xmax_tot = t_e + Ntot;
	bmin_tot = sig_axis->FindBin(xmin_tot);
	bmax_tot = sig_axis->FindBin(xmax_tot);

	xmin_tail = t_e + (Ntot - Ntail);
	xmax_tail = t_e + Ntail;
	bmin_tail = sig_axis->FindBin(xmin_tail);
	bmax_tail = sig_axis->FindBin(xmax_tail);

	results[2] = sig_tot->Integral(bmin_tot, bmax_tot);
	results[2] -= sig_tot->GetBinContent(bmin_tot)*
		(xmin_tot-sig_axis->GetBinLowEdge(bmin_tot))
		/ sig_axis->GetBinWidth(bmin_tot);
	results[2] -= sig_tot->GetBinContent(bmax_tot)*
		(sig_axis->GetBinUpEdge(bmax_tot)-xmax_tot)/
		sig_axis->GetBinWidth(bmax_tot);


	results[3] = sig_tot->Integral(bmin_tail, bmax_tail);
	results[3] -= sig_tot->GetBinContent(bmin_tail)*
		(xmin_tail-sig_axis->GetBinLowEdge(bmin_tail))
		/ sig_axis->GetBinWidth(bmin_tail);
	results[3] -= sig_tot->GetBinContent(bmax_tail)*
		(sig_axis->GetBinUpEdge(bmax_tail)-xmax_tail)/
		sig_axis->GetBinWidth(bmax_tail);

	if (t_e < 0 || t_mu < 0){
		results[2] = -1.;
		results[3] = -1.;
		results[4] = -1.;

	} else
	results[4] = t_e - t_mu;

}

void signal(){

	TFile *output_file = new TFile("output_minus_1000.root", "RECREATE");
	TTree *output_tree = new TTree("T", "T");

	// [0] - Qtot muon 
	// [1] - Qtail muon
	// [2] - Qtot electron
	// [3] - Qtail electron
	// [4] - Delta time
	Int_t Event_id = -1.;
	
	Double_t Qtot_mu = -1.;
	Double_t Qtail_mu = -1.;
	Double_t Qtot_e = -1.;
	Double_t Qtail_e = -1.;
	Double_t DeltaT = -1.;

	output_tree->Branch("Event_id", &Event_id, "Event_id/I");
	output_tree->Branch("Qtot_mu", &Qtot_mu, "Qtot_mu/D");
	output_tree->Branch("Qtail_mu", &Qtail_mu, "Qtail_mu/D");
	output_tree->Branch("Qtot_e", &Qtot_e, "Qtot_e/D");
	output_tree->Branch("Qtail_e", &Qtail_e, "Qtail_e/D");
	output_tree->Branch("DeltaT", &DeltaT, "DeltaT/D");

	TFile *input_sim = new TFile("data_minus.root");
	TTree *tree_sim = (TTree*)input_sim->Get("T");
	// tree_sim->Print();

	Double_t wlTable[14];
	Double_t pdeTable[14] = {13.2, 29.98, 34.5, 34.3, 33.3, 30.5, 27.7, 23.0, 18.0, 15.1, 9.0, 5.5, 3.6, 2.};

	for (int i = 0; i < 14; ++i)
	{
		wlTable[i] = 300. + i*25.;  
		pdeTable[i] /= 100.	;
	} 

	Float_t Wavelength[100000] = {0};
	Float_t Time[100000] = {0};
	Double_t pdeWal[100000] = {0};
	Int_t photonDetectorID[100000] = {0};
	
	TRandom3 *randGen = new TRandom3(0);


	Int_t nPhot = 0;
	Int_t muIsDecay = -1.;
	Int_t EventID = -1.;
	



	tree_sim->SetBranchAddress("photonWavelength", Wavelength);
	tree_sim->SetBranchAddress("photonTime", Time);
	tree_sim->SetBranchAddress("photonDetectorID", photonDetectorID);
	tree_sim->SetBranchAddress("nPhot", &nPhot);
	tree_sim->SetBranchAddress("muIsDecay", &muIsDecay);
	tree_sim->SetBranchAddress("EventID", &EventID);

	Int_t *Pulse1 = new Int_t[60];
	make_pulse(Pulse1);


	TH1D *out_sig;
	TH1D *out_time;


	Int_t nEn = tree_sim->GetEntries();
	for (Long_t j = 0; j < nEn; ++j) {
		out_sig = new TH1D("out_sig","out_sig", 10000, 0, 1e4);
		// out_time = new TH1D("out_time","out_time", 10000, 0, 1e4);
		// tree_sim->cd();
		tree_sim->GetEntry(j);
		if (!muIsDecay) continue;

		for (int i = 0; i < nPhot; ++i){
			if (photonDetectorID[i] != 1) continue;
			if (Time[i] > 0.) {
				for (int k = 0; k < 13; ++k) {
					if (Wavelength[i] >= wlTable[k] &&  Wavelength[i] < wlTable[k+1]) {
						Double_t a = wlTable[k]; Double_t fa = pdeTable[k];
						Double_t b = wlTable[k+1]; Double_t fb = pdeTable[k+1];
						Double_t c = Wavelength[i];
						pdeWal[i] = ( fb*(c-a) + fa*(b-c) ) / 25.;
						// std::cout << Wavelength[i] << "\t"<< pdeWal[i] << endl;
					}
				}
				Double_t dice = randGen->Uniform();
				// Double_t dice = 0;
				if (dice >= pdeWal[i]) continue; 
				// out_time->Fill(Time[i]);
				
				Double_t ampl = randGen->Gaus(250, 50);

				add_phot(out_sig, Time[i], ampl / 250. / 10., Pulse1);
				// delete entVect;
			}
		}

		// [0] - Qtot muon 
		// [1] - Qtail muon
		// [2] - Qtot electron
		// [3] - Qtail electron
		// [4] - Delta time

		Double_t results[5] = {-1.}; 
		sig_ana(out_sig, results);

		Event_id = EventID;
		Qtot_mu = results[0];
		Qtail_mu = results[1];
		Qtot_e = results[2];
		Qtail_e = results[3];
		DeltaT = results[4];

		// output_tree->cd();

		output_tree->Fill();
	}

	// out_sig->Draw();
	// out_time->SetLineColor(kRed);
	// out_time->Draw("SAME");


	// delete input_sim;
	// delete tree_sim;

	output_file->cd();
	output_tree->Write();
	output_file->Close();
	// return;
}

