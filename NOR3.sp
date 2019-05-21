************************************************************************
* Library Name: LeoBarlette
* Cell Name:    NOR2
* View Name:    schematic
************************************************************************

.SUBCKT NOR2 GND A B C OUT VDD
MM0 VDD A net1 VDD pmos_rvt w=81.0n l=20n nfin=3
MM1 net1 B net2 VDD pmos_rvt w=81.0n l=20n nfin=3
MM2 net2 C OUT VDD pmos_rvt w=81.0n l=20n nfin=3
MM3 OUT A GND GND nmos_rvt w=81.0n l=20n nfin=3
MM4 OUT B GND GND nmos_rvt w=81.0n l=20n nfin=3
MM5 OUT C GND GND nmos_rvt w=81.0n l=20n nfin=3
.ENDS

