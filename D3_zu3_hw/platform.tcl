# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct D:\code\2Wproject\D3project\D3_zu3_hw\platform.tcl
# 
# OR launch xsct and run below command.
# source D:\code\2Wproject\D3project\D3_zu3_hw\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {D3_zu3_hw}\
-hw {D:\code\2Wproject\xsa\top03.xsa}\
-proc {psu_cortexa53_0} -os {freertos10_xilinx} -arch {64-bit} -fsbl-target {psu_cortexa53_0} -out {D:/code/2Wproject/D3project}

platform write
platform generate -domains 
platform active {D3_zu3_hw}
bsp reload
bsp setlib -name xilffs -ver 4.6
bsp setlib -name lwip211 -ver 1.7
bsp removelib -name lwip211
bsp setlib -name lwip211 -ver 1.7
bsp config api_mode "SOCKET_API"
bsp config lwip_tcp_keepalive "false"
bsp config no_sys_no_timers "true"
bsp config socket_mode_thread_prio "2"
bsp config igmp_options "true"
bsp config use_chmod "false"
bsp config use_lfn "0"
bsp config use_chmod "false"
bsp config use_lfn "1"
bsp config use_strfunc "11"
bsp write
bsp reload
catch {bsp regenerate}
bsp write
bsp write
platform generate
bsp reload
bsp config api_mode "SOCKET_API"
bsp config igmp_options "true"
bsp config dhcp_does_arp_check "true"
bsp config lwip_dhcp "true"
bsp write
bsp reload
catch {bsp regenerate}
bsp reload
platform active {D3_zu3_hw}
bsp reload
bsp config dhcp_does_arp_check "true"
bsp config lwip_dhcp "true"
bsp config pbuf_pool_size "1024"
bsp config mem_size "1310720"
bsp config memp_n_pbuf "256"
bsp config memp_n_sys_timeout "8"
bsp config memp_n_tcp_pcb "256"
bsp config memp_n_udp_pcb "4"
bsp config memp_n_tcp_seg "512"
bsp config memp_num_api_msg "16"
bsp config memp_num_netbuf "8"
bsp config memp_num_netconn "16"
bsp config memp_num_tcpip_msg "64"
bsp config memp_num_api_msg "16"
bsp config memp_n_udp_pcb "4"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_domain 
bsp config stream_buffer "true"
bsp config support_static_allocation "true"
bsp config max_priorities "31"
bsp config max_task_name_len "16"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_domain 
bsp config socket_mode_thread_prio "31"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_domain 
platform active {D3_zu3_hw}
domain active {zynqmp_fsbl}
bsp reload
bsp removelib -name xilpm
bsp removelib -name xilsecure
bsp setlib -name lwip211 -ver 1.7
bsp reload
bsp setlib -name lwip211 -ver 1.7
bsp removelib -name xilpm
bsp removelib -name xilsecure
bsp reload
domain active {freertos10_xilinx_domain}
bsp reload
domain active {zynqmp_fsbl}
bsp reload
domain active {freertos10_xilinx_domain}
bsp config mem_size "131072"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_domain 
platform generate -domains freertos10_xilinx_domain 
bsp setlib -name lwip211 -ver 1.6
bsp write
bsp reload
catch {bsp regenerate}
bsp write
platform generate -domains freertos10_xilinx_domain 
bsp reload
platform clean
platform generate
platform active {D3_zu3_hw}
bsp reload
bsp setlib -name lwip211 -ver 2.0
bsp write
bsp reload
catch {bsp regenerate}
bsp config api_mode "SOCKET_API"
bsp config socket_mode_thread_prio "31"
bsp config igmp_options "true"
bsp config dhcp_does_arp_check "true"
bsp config lwip_dhcp "true"
bsp write
bsp reload
platform active {D3_zu3_hw}
bsp reload
bsp config api_mode "SOCKET_API"
bsp config socket_mode_thread_prio "31"
bsp config igmp_options "true"
bsp config dhcp_does_arp_check "true"
bsp config lwip_dhcp "true"
bsp write
bsp reload
platform active {D3_zu3_hw}
bsp reload
bsp config api_mode "SOCKET_API"
bsp config socket_mode_thread_prio "31"
bsp config igmp_options "true"
bsp config ipv6_enable "false"
bsp config dhcp_does_arp_check "true"
bsp config lwip_dhcp "true"
bsp write
bsp config api_mode "RAW_API"
bsp config api_mode "SOCKET_API"
bsp reload
bsp config api_mode "SOCKET_API"
bsp write
bsp config api_mode "SOCKET_API"
bsp config lwip_tcp_keepalive "false"
bsp config igmp_options "false"
bsp write
bsp reload
catch {bsp regenerate}
bsp removelib -name lwip211
bsp write
bsp reload
catch {bsp regenerate}
bsp reload
platform clean
domain active {zynqmp_fsbl}
bsp reload
domain active {freertos10_xilinx_domain}
bsp config api_mode "SOCKET_API"
bsp config igmp_options "true"
bsp write
bsp reload
catch {bsp regenerate}
bsp reload
platform generate
bsp reload
platform generate -domains freertos10_xilinx_domain 
bsp config icmp_debug "true"
bsp config igmp_debug "true"
bsp config ip_debug "true"
bsp config lwip_debug "true"
bsp config netif_debug "false"
bsp config netif_debug "false"
bsp config sys_debug "false"
bsp config netif_debug "true"
bsp config pbuf_debug "true"
bsp config sys_debug "true"
bsp config tcp_debug "true"
bsp config udp_debug "true"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_domain 
bsp config icmp_debug "true"
bsp config icmp_debug "true"
bsp config icmp_debug "false"
bsp config igmp_debug "false"
bsp config ip_debug "false"
bsp config lwip_debug "false"
bsp config netif_debug "false"
bsp config pbuf_debug "false"
bsp config sys_debug "false"
bsp config tcp_debug "false"
bsp config udp_debug "false"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_domain 
