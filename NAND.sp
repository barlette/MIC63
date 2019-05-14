************************************************************************
* Library Name: LeoBarlette
* Cell Name:    NAND2
* View Name:    schematic
************************************************************************

.SUBCKT NAND2 GND A B OUT VDD
MM0 VDD A OUT VDD pmos_rvt w=81.0n l=20n nfin=3
MM1 VDD B OUT VDD pmos_rvt w=81.0n l=20n nfin=3
MM2 OUT A net1 GND nmos_rvt w=81.0n l=20n nfin=3
MM3 net1 B GND GND nmos_rvt w=81.0n l=20n nfin=3
.ENDS

