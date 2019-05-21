************************************************************************
* Library Name: LeoBarlette
* Cell Name:    NOR2
* View Name:    schematic
************************************************************************

.SUBCKT H GND A B C D OUT VDD
MM0 VDD A net1 VDD pmos_rvt w=81.0n l=20n nfin=3
MM1 VDD B net1 VDD pmos_rvt w=81.0n l=20n nfin=3
MM2 net1 C OUT VDD pmos_rvt w=81.0n l=20n nfin=3
MM3 net1 D OUT VDD pmos_rvt w=81.0n l=20n nfin=3
MM4 GND A net2 GND nmos_rvt w=81.0n l=20n nfin=3
MM5 GND B net2 GND nmos_rvt w=81.0n l=20n nfin=3
MM6 net2 C OUT GND nmos_rvt w=81.0n l=20n nfin=3
MM7 net2 D OUT GND nmos_rvt w=81.0n l=20n nfin=3
.ENDS

