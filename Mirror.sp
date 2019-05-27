************************************************************************
* Library Name: LeoBarlette
* Cell Name:    NOR2
* View Name:    schematic
************************************************************************

.SUBCKT 1S2P GND A B CI S CO VDD
MM0 VDD A net1 VDD pmos_rvt w=81.0n l=20n nfin=3
MM1 VDD B net1 VDD pmos_rvt w=81.0n l=20n nfin=3
MM2 net1 CI CO VDD pmos_rvt w=81.0n l=20n nfin=3
MM3 VDD B net2 VDD pmos_rvt w=81.0n l=20n nfin=3
MM4 net2 A CO VDD pmos_rvt w=81.0n l=20n nfin=3
MM5 VDD A net3 VDD pmos_rvt w=81.0n l=20n nfin=3
MM6 VDD B net3 VDD pmos_rvt w=81.0n l=20n nfin=3
MM7 VDD CI net3 VDD pmos_rvt w=81.0n l=20n nfin=3
MM8 net3 CO S VDD pmos_rvt w=81.0n l=20n nfin=3
MM9 VDD A net4 VDD pmos_rvt w=81.0n l=20n nfin=3
MM10 net4 B net5 VDD pmos_rvt w=81.0n l=20n nfin=3
MM11 net5 CI S VDD pmos_rvt w=81.0n l=20n nfin=3
MM12 GND A net6 GND nmos_rvt w=81.0n l=20n nfin=3
MM13 GND B net6 GND nmos_rvt w=81.0n l=20n nfin=3
MM14 net6 CI CO GND nmos_rvt w=81.0n l=20n nfin=3
MM15 GND B net7 GND nmos_rvt w=81.0n l=20n nfin=3
MM16 net7 A CO GND nmos_rvt w=81.0n l=20n nfin=3
MM17 GND A net8 GND nmos_rvt w=81.0n l=20n nfin=3
MM18 GND B net8 GND nmos_rvt w=81.0n l=20n nfin=3
MM19 GND CI ne8 GND nmos_rvt w=81.0n l=20n nfin=3
MM20 net8 CO S GND nmos_rvt w=81.0n l=20n nfin=3
MM21 GND A net9 GND nmos_rvt w=81.0n l=20n nfin=3
MM22 net9 B net10 GND nmos_rvt w=81.0n l=20n nfin=3
MM23 net10 CI S GND nmos_rvt w=81.0n l=20n nfin=3
.ENDS

