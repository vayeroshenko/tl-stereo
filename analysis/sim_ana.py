from ROOT import gStyle, gROOT, kRed
from ROOT import (TH1D, TFile, TTree, TCanvas, TLegend) 

# file_in = TFile("sim_20200206_1141/output.root")
file_in = TFile("output.root")
tree_in = file_in.Get("T")

h_qtot_mu = TH1D("h_qtot_mu","h_qtot_mu", 100, -1000 , 20000)
h_qtot_e = TH1D("h_qtot_e","h_qtot_e", 100, -1000, 20000)

tree_in.Draw("Qtot_mu >> h_qtot_mu", "Qtot_mu > 0 && Qtot_e > 0")
tree_in.Draw("Qtot_e >> h_qtot_e", "Qtot_mu > 0 && Qtot_e > 0")


l2 = TLegend(0.6, 0.7, 0.9, 0.9);
l2.AddEntry(h_qtot_mu, "Muon (prev)", "l")
l2.AddEntry(h_qtot_e, "Electron (next)", "l")

h_qtot_mu.SetLineColor(kRed)

gStyle.SetOptStat(0)

h_qtot_e.Draw();
h_qtot_mu.Draw("SAME");
l2.Draw()


