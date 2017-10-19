# REAX potential for Ni system
# .....

dimension      3
boundary       p p p
units		real

atom_style	charge
read_data	data.ni

pair_style	reax/c lmp_control
pair_coeff	* * ffield.reax.ni Ni B N

neighbor	2 bin
neigh_modify	every 10 delay 0 check no

fix		1 all nve
fix             2 all qeq/reax 1 0.0 10.0 1e-6 param.qeq
fix             3 all temp/berendsen 1500.0 1500.0 100.0

timestep	0.25

#dump		1 all atom 30 dump.reax.ab
dump            1 all xyz 1000 dump.reax.xyz

run		1000000
