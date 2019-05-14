************************************************************************
* auCdl Netlist:
* 
* Library Name:  LeoBarlette
* Top Cell Name: ST2
* View Name:     schematic
* Netlisted on:  Jan  4 23:42:07 2019
************************************************************************

*.EQUATION
*.SCALE METER
*.MEGA
.PARAM



************************************************************************
* Library Name: LeoBarlette
* Cell Name:    ST2
* View Name:    schematic
************************************************************************

.SUBCKT ST2 GND IN OUT VDD
*.PININFO IN:I OUT:O GND:B VDD:B
MM5 net7 IN GND GND nmos_rvt w=81.0n l=20n nfin=3
MM4 OUT IN net7 GND nmos_rvt w=81.0n l=20n nfin=3
MM0 VDD OUT net7 GND nmos_rvt w=81.0n l=20n nfin=3
MM3 GND OUT net9 VDD pmos_rvt w=81.0n l=20n nfin=3
MM2 OUT IN net9 VDD pmos_rvt w=81.0n l=20n nfin=3
MM1 net9 IN VDD VDD pmos_rvt w=81.0n l=20n nfin=3
.ENDS

