* test file for extracted cmos full adder layout with asap7 finfet tech
.include "ST2.pex.netlist"
.include "INV.pex.netlist"

.include "hspice/7nm_TT.pm"
.include "hspice/nonvar.pm"
*.OPTION POST = 2
.define gauss(nom, rvar, sig) (nom + (nom*rvar)/sig * sgauss(0))

.param VCC = 0.1

*.param p_phig = 4.8108
*.param n_phig = 4.3720

.param p_phig = gauss(4.8108, 0.01, 3)
.param n_phig = gauss(4.3720, 0.01, 3)

*1.5MHz
.param freq1 = 333.3333335n
.param freq2 = 'freq1 + 0.01n'
.param freq3 = '2*freq1'
.param freq4 = 'freq3 + 0.01n'
.param freq5 = '3*freq1'

VIN IN 0 PWL(0n 0 freq1 0 freq2 VCC freq3 VCC freq4 0 freq5 0)


VVDD0 VDD0 GND VCC
VVDD1 VDD1 GND VCC
VGND GND GND 0

XINV0 GND VDD0 IN0 OUT0  INV
XINV1 GND VDD0 OUT0 IN  INV
XINV GND VDD1 IN OUT ST2

*CAPACITANCIAS
CAPS1 OUT GND 1f


.tran 1p freq5 sweep Monte = 2000

	.measure tran vtplus  find v(IN) when v(OUT) = v(IN) CROSS=1
	.measure tran vtminus find v(IN) when v(OUT) = v(IN) CROSS=2

	.measure tran vhys	param = '(vtplus - vtminus)'
	.measure tran vmid	param = '(vtplus + vtminus)/2.0'

	.measure tran TP_LH_IN trig v(IN) val=VCC*0.5 rise=1 targ v(OUT) val=VCC*0.5 fall=1
	.measure tran TP_HL_IN trig v(IN) val=VCC*0.5 fall=1 targ v(OUT) val=VCC*0.5 rise=1

	.measure tran energy integ i(VVDD1) from=0n to=freq5	
.end
