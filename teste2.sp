************************************************************************
* Library Name: LeoBarlette
* Cell Name:    NOR2
* View Name:    schematic
************************************************************************

.SUBCKT 1S2P GND A B C D OUT VDD
MM0 VDD C net1 VDD pmos_rvt w=81.0n l=20n nfin=3
MM1 net1 A OUT VDD pmos_rvt w=81.0n l=20n nfin=3
MM2 net1 B OUT VDD pmos_rvt w=81.0n l=20n nfin=3
MM3 VDD D OUT VDD pmos_rvt w=81.0n l=20n nfin=3
MM4 OUT D net2 GND nmos_rvt w=81.0n l=20n nfin=3
MM5 net2 A net3 GND nmos_rvt w=81.0n l=20n nfin=3
MM6 net3 B GND GND nmos_rvt w=81.0n l=20n nfin=3
MM7 net2 C GND GND nmos_rvt w=81.0n l=20n nfin=3
.ENDS

