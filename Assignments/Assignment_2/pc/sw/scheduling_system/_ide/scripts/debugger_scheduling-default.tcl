# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: D:\Vivado_Projects\Assignment_3\pc\sw\scheduling_system\_ide\scripts\debugger_scheduling-default.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source D:\Vivado_Projects\Assignment_3\pc\sw\scheduling_system\_ide\scripts\debugger_scheduling-default.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -filter {jtag_cable_name =~ "Digilent Zed 210248AC8896" && level==0 && jtag_device_ctx=="jsn-Zed-210248AC8896-23727093-0"}
fpga -file D:/Vivado_Projects/Assignment_3/pc/sw/scheduling/_ide/bitstream/design_1_wrapper.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw D:/Vivado_Projects/Assignment_3/pc/sw/design_1_wrapper/export/design_1_wrapper/hw/design_1_wrapper.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
stop
source D:/Vivado_Projects/Assignment_3/pc/sw/scheduling/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "*A9*#0"}
rst -processor
targets -set -nocase -filter {name =~ "*A9*#0"}
dow D:/Vivado_Projects/Assignment_3/pc/sw/scheduling/Debug/scheduling.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "*A9*#0"}
con
