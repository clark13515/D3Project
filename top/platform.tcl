# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct D:\code\2Wproject\D3project\top\platform.tcl
# 
# OR launch xsct and run below command.
# source D:\code\2Wproject\D3project\top\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {top}\
-hw {D:\code\2Wproject\zu_prj_v00\zu_prj_v00\top.xsa}\
-arch {64-bit} -fsbl-target {psu_cortexa53_0} -out {D:/code/2Wproject/D3project}

platform write
domain create -name {freertos10_xilinx_psu_cortexa53_0} -display-name {freertos10_xilinx_psu_cortexa53_0} -os {freertos10_xilinx} -proc {psu_cortexa53_0} -runtime {cpp} -arch {64-bit} -support-app {freertos_lwip_tcp_perf_server}
platform generate -domains 
platform active {top}
domain active {zynqmp_fsbl}
domain active {zynqmp_pmufw}
domain active {freertos10_xilinx_psu_cortexa53_0}
platform generate -quick
platform generate
platform clean
platform generate
domain active {zynqmp_fsbl}
domain active {freertos10_xilinx_psu_cortexa53_0}
bsp reload
bsp reload
bsp reload
bsp setlib -name xilffs -ver 4.6
bsp removelib -name xilffs
bsp setlib -name xilffs -ver 4.6
bsp write
bsp reload
catch {bsp regenerate}
bsp config use_lfn "1"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_psu_cortexa53_0 
platform active {top}
bsp reload
bsp config igmp_options "true"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_psu_cortexa53_0 
bsp config no_sys_no_timers "true"
bsp config socket_mode_thread_prio "31"
bsp config memp_n_pbuf "1024"
bsp config pbuf_pool_size "16384"
bsp config max_task_name_len "16"
bsp config total_heap_size "262140"
bsp config stream_buffer "true"
bsp config support_static_allocation "true"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_psu_cortexa53_0 
platform clean
platform generate
platform clean
platform generate
bsp write
platform generate -domains 
bsp reload
platform clean
bsp config support_static_allocation "true"
bsp write
platform generate
platform generate -domains freertos10_xilinx_psu_cortexa53_0 
platform clean
platform generate
platform clean
platform generate
bsp reload
platform clean
platform generate
domain active {zynqmp_fsbl}
bsp reload
platform write
platform generate -domains freertos10_xilinx_psu_cortexa53_0 
platform generate -domains 
platform active {top}
platform config -updatehw {D:/code/2Wproject/xsa/top_v02.xsa}
domain active {freertos10_xilinx_psu_cortexa53_0}
bsp reload
bsp setlib -name lwip211 -ver 1.6
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_psu_cortexa53_0 
bsp reload
bsp write
platform clean
platform generate
bsp reload
bsp reload
bsp reload
bsp reload
bsp reload
platform clean
platform generate
platform active {top}
bsp reload
platform generate -domains 
platform clean
platform generate
platform active {top}
bsp config use_strfunc "2"
bsp config enable_multi_partition "true"
bsp config fs_interface "1"
bsp config use_chmod "false"
bsp config use_lfn "1"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_psu_cortexa53_0 
bsp reload
platform clean
platform generate
platform clean
platform generate
domain create -name {teststandalonedomin} -os {standalone} -proc {psu_cortexa53_0} -arch {64-bit} -display-name {teststandalonedomin} -desc {} -runtime {cpp}
platform write
domain -report -json
bsp reload
bsp setlib -name xilffs -ver 4.6
bsp config enable_multi_partition "true"
bsp config use_lfn "1"
bsp config use_strfunc "2"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains 
platform clean
platform generate
domain remove teststandalonedomin
platform active {top}
domain create -name {test} -os {standalone} -proc {psu_cortexa53_1} -arch {64-bit} -display-name {test} -desc {} -runtime {cpp}
platform generate -domains 
platform write
domain -report -json
bsp reload
bsp setlib -name xilffs -ver 4.6
bsp config use_strfunc "2"
bsp config use_lfn "1"
bsp config enable_multi_partition "true"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains test 
platform active {top}
domain active {freertos10_xilinx_psu_cortexa53_0}
bsp reload
bsp reload
bsp reload
platform active {top}
domain active {freertos10_xilinx_psu_cortexa53_0}
bsp reload
bsp reload
bsp removelib -name xilffs
bsp setlib -name xilffs -ver 4.6
bsp reload
bsp reload
bsp config use_mkfs "true"
bsp reload
platform generate
platform generate -domains freertos10_xilinx_psu_cortexa53_0 
bsp reload
bsp reload
platform generate -domains 
bsp reload
bsp setlib -name lwip211 -ver 1.7
bsp setlib -name lwip211 -ver 1.6
bsp reload
domain active {test}
bsp setlib -name lwip211 -ver 1.7
bsp setlib -name lwip211 -ver 1.6
bsp reload
platform active {top}
bsp reload
bsp config PSU_TTC0_Select "false"
bsp config PSU_TTC1_Select "true"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_psu_cortexa53_0 
platform generate
bsp reload
platform generate -domains freertos10_xilinx_psu_cortexa53_0 
platform generate -domains freertos10_xilinx_psu_cortexa53_0 
platform active {top}
platform config -updatehw {D:/code/2Wproject/xsa/top03.xsa}
platform generate -domains 
domain active {zynqmp_fsbl}
bsp reload
bsp reload
platform active {top}
bsp setlib -name lwip211 -ver 1.6
bsp setlib -name lwip211 -ver 1.6
bsp removelib -name lwip211
bsp setlib -name lwip211 -ver 1.6
bsp removelib -name lwip211
bsp setlib -name lwip211 -ver 1.6
bsp removelib -name lwip211
bsp setlib -name lwip211 -ver 1.6
bsp removelib -name lwip211
bsp setlib -name lwip211 -ver 1.6
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains zynqmp_fsbl 
platform active {top}
domain active {freertos10_xilinx_psu_cortexa53_0}
bsp reload
bsp reload
platform active {top}
platform generate -domains 
domain active {freertos10_xilinx_psu_cortexa53_0}
domain active {zynqmp_fsbl}
bsp reload
bsp reload
domain active {freertos10_xilinx_psu_cortexa53_0}
bsp reload
platform generate -domains freertos10_xilinx_psu_cortexa53_0 
platform active {top}
domain active {zynqmp_fsbl}
bsp reload
bsp reload
platform generate -domains 
platform generate -domains freertos10_xilinx_psu_cortexa53_0 
platform generate -domains freertos10_xilinx_psu_cortexa53_0,zynqmp_fsbl 
platform clean
platform generate
platform clean
platform generate
domain active {freertos10_xilinx_psu_cortexa53_0}
bsp reload
bsp reload
platform active {top}
platform generate -domains freertos10_xilinx_psu_cortexa53_0,test,zynqmp_fsbl,zynqmp_pmufw 
platform generate -domains freertos10_xilinx_psu_cortexa53_0 
platform active {top}
domain active {freertos10_xilinx_psu_cortexa53_0}
bsp reload
bsp config icmp_debug "true"
bsp config ip_debug "false"
bsp config udp_debug "true"
bsp config tcp_debug "true"
bsp config sys_debug "true"
bsp config pbuf_debug "true"
bsp config netif_debug "true"
bsp config lwip_debug "true"
bsp config ip_debug "true"
bsp config igmp_debug "true"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_psu_cortexa53_0 
bsp setlib -name lwip211 -ver 1.7
bsp write
bsp reload
catch {bsp regenerate}
bsp config api_mode "SOCKET_API"
bsp config lwip_tcp_keepalive "false"
bsp config socket_mode_thread_prio "2"
bsp config use_axieth_on_zynq "1"
bsp config use_emaclite_on_zynq "1"
bsp config igmp_options "false"
bsp config ipv6_enable "false"
bsp write
bsp reload
catch {bsp regenerate}
bsp config api_mode "SOCKET_API"
bsp write
domain active {zynqmp_fsbl}
bsp reload
domain active {freertos10_xilinx_psu_cortexa53_0}
bsp config api_mode "SOCKET_API"
bsp write
bsp reload
platform active {top}
bsp reload
bsp config api_mode "SOCKET_API"
bsp write
bsp reload
platform clean
bsp config api_mode "SOCKET_API"
bsp write
bsp config api_mode "RAW_API"
bsp config api_mode "SOCKET_API"
bsp config api_mode "SOCKET_API"
bsp write
bsp setlib -name lwip211 -ver 1.6
bsp config api_mode "SOCKET_API"
bsp write
bsp reload
catch {bsp regenerate}
bsp setlib -name lwip211 -ver 1.6
bsp config api_mode "SOCKET_API"
bsp write
bsp reload
catch {bsp regenerate}
platform active {top}
bsp reload
bsp config api_mode "SOCKET_API"
bsp config dhcp_does_arp_check "true"
bsp config lwip_dhcp "true"
bsp config igmp_options "false"
bsp config igmp_options "true"
bsp config socket_mode_thread_prio "31"
bsp write
bsp reload
catch {bsp regenerate}
