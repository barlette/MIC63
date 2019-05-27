************************************************************************
* Library Name: LeoBarlette
* Cell Name:    NOR2
* View Name:    schematic
************************************************************************

.SUBCKT 1S2P GND A B C D OUT VDD
MM0 VDD B net1 VDD pmos_rvt w=81.0n l=20n nfin=3
MM1 VDD C net2 VDD pmos_rvt w=81.0n l=20n nfin=3
MM2 net2 D net1 VDD pmos_rvt w=81.0n l=20n nfin=3
MM3 net1 A OUT VDD pmos_rvt w=81.0n l=20n nfin=3
MM4 OUT A GND GND nmos_rvt w=81.0n l=20n nfin=3
MM5 OUT B net3 GND nmos_rvt w=81.0n l=20n nfin=3
MM6 net3 C GND GND nmos_rvt w=81.0n l=20n nfin=3
MM7 net3 D GND GND nmos_rvt w=81.0n l=20n nfin=3
.ENDS

