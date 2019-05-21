************************************************************************
* Library Name: LeoBarlette
* Cell Name:    NAND2
* View Name:    schematic
************************************************************************

.SUBCKT NAND2 GND A B C OUT VDD
MM0 VDD A OUT VDD pmos_rvt w=81.0n l=20n nfin=3
MM1 VDD B OUT VDD pmos_rvt w=81.0n l=20n nfin=3
MM2 VDD C OUT VDD pmos_rvt w=81.0n l=20n nfin=3
MM3 OUT A net1 GND nmos_rvt w=81.0n l=20n nfin=3
MM4 net1 B net2 GND nmos_rvt w=81.0n l=20n nfin=3
MM5 net2 C GND GND nmos_rvt w=81.0n l=20n nfin=3
.ENDS

