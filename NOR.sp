************************************************************************
* Library Name: LeoBarlette
* Cell Name:    NOR2
* View Name:    schematic
************************************************************************

.SUBCKT NOR2 GND A B OUT VDD
MM0 VDD A net1 VDD pmos_rvt w=81.0n l=20n nfin=3
MM1 net1 B OUT VDD pmos_rvt w=81.0n l=20n nfin=3
MM2 OUT A GND GND nmos_rvt w=81.0n l=20n nfin=3
MM3 OUT B GND GND nmos_rvt w=81.0n l=20n nfin=3
.ENDS

