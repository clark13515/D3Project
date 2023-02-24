# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct D:\code\2Wproject\D3project\top_1\platform.tcl
# 
# OR launch xsct and run below command.
# source D:\code\2Wproject\D3project\top_1\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {top_1}\
-hw {D:\code\2Wproject\zu_prj_v01\zu_prj_v01\top.xsa}\
-arch {64-bit} -fsbl-target {psu_cortexa53_0} -out {D:/code/2Wproject/D3project}

platform write
domain create -name {freertos10_xilinx_psu_cortexa53_0} -display-name {freertos10_xilinx_psu_cortexa53_0} -os {freertos10_xilinx} -proc {psu_cortexa53_0} -runtime {cpp} -arch {64-bit} -support-app {freertos_lwip_udp_perf_server}
platform generate -domains 
platform active {top_1}
domain active {zynqmp_fsbl}
domain active {zynqmp_pmufw}
domain active {freertos10_xilinx_psu_cortexa53_0}
platform generate -quick
platform generate
bsp reload
bsp reload
platform generate -domains freertos10_xilinx_psu_cortexa53_0 
platform generate -domains freertos10_xilinx_psu_cortexa53_0 
platform config -updatehw {D:/code/2Wproject/xsa/top_v02.xsa}
platform generate -domains 
bsp reload
bsp setlib -name lwip211 -ver 1.6
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_psu_cortexa53_0 
platform config -updatehw {D:/code/2Wproject/zu_sdk_v01/zu_sdk_v01/top/hw/top.xsa}
platform generate -domains 
platform config -updatehw {D:/code/2Wproject/xsa/top_v02.xsa}
platform generate -domains 
bsp reload
bsp reload
platform config -updatehw {D:/code/2Wproject/zu_sdk_v01/zu_sdk_v01/top/export/top/hw/top.xsa}
platform generate -domains 
platform generate -domains freertos10_xilinx_psu_cortexa53_0 
platform active {top_1}
bsp reload
platform generate -domains 
platform clean
platform generate
platform clean
platform generate
platform generate -domains 
platform generate -domains teststandalonedomin 
platform generate
platform active {top_1}
domain active {zynqmp_fsbl}
bsp reload
bsp reload
