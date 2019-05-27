************************************************************************
* Library Name: LeoBarlette
* Cell Name:    NOR2
* View Name:    schematic
************************************************************************

.SUBCKT 1S2P GND A B C OUT VDD
MM0 VDD A net1 VDD pmos_rvt w=81.0n l=20n nfin=3
MM1 VDD B net1 VDD pmos_rvt w=81.0n l=20n nfin=3
MM2 net1 C OUT VDD pmos_rvt w=81.0n l=20n nfin=3
MM3 OUT A net2 GND nmos_rvt w=81.0n l=20n nfin=3
MM4 net2 B GND GND nmos_rvt w=81.0n l=20n nfin=3
MM5 OUT C GND GND nmos_rvt w=81.0n l=20n nfin=3
.ENDS

