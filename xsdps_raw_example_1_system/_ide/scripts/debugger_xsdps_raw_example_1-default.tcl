# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: D:\code\2Wproject\D3project\xsdps_raw_example_1_system\_ide\scripts\debugger_xsdps_raw_example_1-default.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source D:\code\2Wproject\D3project\xsdps_raw_example_1_system\_ide\scripts\debugger_xsdps_raw_example_1-default.tcl
# 
connect -url tcp:127.0.0.1:3121
source D:/Xilinx/Vitis/2021.2/scripts/vitis/util/zynqmp_utils.tcl
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent JTAG-HS1 210249858006" && level==0 && jtag_device_ctx=="jsn-JTAG-HS1-210249858006-14710093-0"}
fpga -file D:/code/2Wproject/D3project/xsdps_raw_example_1/_ide/bitstream/top.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw D:/code/2Wproject/D3project/top/export/top/hw/top03.xsa -mem-ranges [list {0x80000000 0xbfffffff} {0x400000000 0x5ffffffff} {0x1000000000 0x7fffffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
set mode [expr [mrd -value 0xFF5E0200] & 0xf]
targets -set -nocase -filter {name =~ "*A53*#0"}
rst -processor
dow D:/code/2Wproject/D3project/top/export/top/sw/top/boot/fsbl.elf
set bp_25_25_fsbl_bp [bpadd -addr &XFsbl_Exit]
con -block -timeout 60
bpremove $bp_25_25_fsbl_bp
targets -set -nocase -filter {name =~ "*A53*#0"}
rst -processor
dow D:/code/2Wproject/D3project/xsdps_raw_example_1/Debug/xsdps_raw_example_1.elf
configparams force-mem-access 0
bpadd -addr &main
