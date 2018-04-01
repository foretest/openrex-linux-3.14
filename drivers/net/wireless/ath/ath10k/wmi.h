/*
 * Copyright (c) 2005-2011 Atheros Communications Inc.
 * Copyright (c) 2011-2013 Qualcomm Atheros, Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _WMI_H_
#define _WMI_H_

#include <linux/types.h>
#include <net/mac80211.h>

/*
 * This file specifies the WMI interface for the Unified Software
 * Architecture.
 *
 * It includes definitions of all the commands and events. Commands are
 * messages from the host to the target. Events and Replies are messages
 * from the target to the host.
 *
 * Ownership of correctness in regards to WMI commands belongs to the host
 * driver and the target is not required to validate parameters for value,
 * proper range, or any other checking.
 *
 * Guidelines for extending this interface are below.
 *
 * 1. Add new WMI commands ONLY within the specified range - 0x9000 - 0x9fff
 *
 * 2. Use ONLY u32 type for defining member variables within WMI
 *    command/event structures. Do not use u8, u16, bool or
 *    enum types within these structures.
 *
 * 3. DO NOT define bit fields within structures. Implement bit fields
 *    using masks if necessary. Do not use the programming language's bit
 *    field definition.
 *
 * 4. Define macros for encode/decode of u8, u16 fields within
 *    the u32 variables. Use these macros for set/get of these fields.
 *    Try to use this to optimize the structure without bloating it with
 *    u32 variables for every lower sized field.
 *
 * 5. Do not use PACK/UNPACK attributes for the structures as each member
 *    variable is already 4-byte aligned by virtue of being a u32
 *    type.
 *
 * 6. Comment each parameter part of the WMI command/event structure by
 *    using the 2 stars at the begining of C comment instead of one star to
 *    enable HTML document generation using Doxygen.
 *
 */

/* Control Path */
struct wmi_cmd_hdr {
	__le32 cmd_id;
} __packed;

#define WMI_CMD_HDR_CMD_ID_MASK   0x00FFFFFF
#define WMI_CMD_HDR_CMD_ID_LSB    0
#define WMI_CMD_HDR_PLT_PRIV_MASK 0xFF000000
#define WMI_CMD_HDR_PLT_PRIV_LSB  24

#define HTC_PROTOCOL_VERSION    0x0002
#define WMI_PROTOCOL_VERSION    0x0002

enum wmi_service_id {
	WMI_SERVICE_BEACON_OFFLOAD = 0,   /* beacon offload */
	WMI_SERVICE_SCAN_OFFLOAD,	  /* scan offload */
	WMI_SERVICE_ROAM_OFFLOAD,	  /* roam offload */
	WMI_SERVICE_BCN_MISS_OFFLOAD,     /* beacon miss offload */
	WMI_SERVICE_STA_PWRSAVE,	  /* fake sleep + basic power save */
	WMI_SERVICE_STA_ADVANCED_PWRSAVE, /* uapsd, pspoll, force sleep */
	WMI_SERVICE_AP_UAPSD,		  /* uapsd on AP */
	WMI_SERVICE_AP_DFS,		  /* DFS on AP */
	WMI_SERVICE_11AC,		  /* supports 11ac */
	WMI_SERVICE_BLOCKACK,	/* Supports triggering ADDBA/DELBA from host*/
	WMI_SERVICE_PHYERR,		  /* PHY error */
	WMI_SERVICE_BCN_FILTER,		  /* Beacon filter support */
	WMI_SERVICE_RTT,		  /* RTT (round trip time) support */
	WMI_SERVICE_RATECTRL,		  /* Rate-control */
	WMI_SERVICE_WOW,		  /* WOW Support */
	WMI_SERVICE_RATECTRL_CACHE,       /* Rate-control caching */
	WMI_SERVICE_IRAM_TIDS,            /* TIDs in IRAM */
	WMI_SERVICE_ARPNS_OFFLOAD,	  /* ARP NS Offload support */
	WMI_SERVICE_NLO,		  /* Network list offload service */
	WMI_SERVICE_GTK_OFFLOAD,	  /* GTK offload */
	WMI_SERVICE_SCAN_SCH,		  /* Scan Scheduler Service */
	WMI_SERVICE_CSA_OFFLOAD,	  /* CSA offload service */
	WMI_SERVICE_CHATTER,		  /* Chatter service */
	WMI_SERVICE_COEX_FREQAVOID,	  /* FW report freq range to avoid */
	WMI_SERVICE_PACKET_POWER_SAVE,	  /* packet power save service */
	WMI_SERVICE_FORCE_FW_HANG,        /* To test fw recovery mechanism */
	WMI_SERVICE_GPIO,                 /* GPIO service */
	WMI_SERVICE_STA_DTIM_PS_MODULATED_DTIM, /* Modulated DTIM support */
	WMI_STA_UAPSD_BASIC_AUTO_TRIG,    /* UAPSD AC Trigger Generation  */
	WMI_STA_UAPSD_VAR_AUTO_TRIG,      /* -do- */
	WMI_SERVICE_STA_KEEP_ALIVE,       /* STA keep alive mechanism support */
	WMI_SERVICE_TX_ENCAP,             /* Packet type for TX encapsulation */

	WMI_SERVICE_LAST,
	WMI_MAX_SERVICE = 64		  /* max service */
};

static inline char *wmi_service_name(int service_id)
{
	switch (service_id) {
	case WMI_SERVICE_BEACON_OFFLOAD:
		return "BEACON_OFFLOAD";
	case WMI_SERVICE_SCAN_OFFLOAD:
		return "SCAN_OFFLOAD";
	case WMI_SERVICE_ROAM_OFFLOAD:
		return "ROAM_OFFLOAD";
	case WMI_SERVICE_BCN_MISS_OFFLOAD:
		return "BCN_MISS_OFFLOAD";
	case WMI_SERVICE_STA_PWRSAVE:
		return "STA_PWRSAVE";
	case WMI_SERVICE_STA_ADVANCED_PWRSAVE:
		return "STA_ADVANCED_PWRSAVE";
	case WMI_SERVICE_AP_UAPSD:
		return "AP_UAPSD";
	case WMI_SERVICE_AP_DFS:
		return "AP_DFS";
	case WMI_SERVICE_11AC:
		return "11AC";
	case WMI_SERVICE_BLOCKACK:
		return "BLOCKACK";
	case WMI_SERVICE_PHYERR:
		return "PHYERR";
	case WMI_SERVICE_BCN_FILTER:
		return "BCN_FILTER";
	case WMI_SERVICE_RTT:
		return "RTT";
	case WMI_SERVICE_RATECTRL:
		return "RATECTRL";
	case WMI_SERVICE_WOW:
		return "WOW";
	case WMI_SERVICE_RATECTRL_CACHE:
		return "RATECTRL CACHE";
	case WMI_SERVICE_IRAM_TIDS:
		return "IRAM TIDS";
	case WMI_SERVICE_ARPNS_OFFLOAD:
		return "ARPNS_OFFLOAD";
	case WMI_SERVICE_NLO:
		return "NLO";
	case WMI_SERVICE_GTK_OFFLOAD:
		return "GTK_OFFLOAD";
	case WMI_SERVICE_SCAN_SCH:
		return "SCAN_SCH";
	case WMI_SERVICE_CSA_OFFLOAD:
		return "CSA_OFFLOAD";
	case WMI_SERVICE_CHATTER:
		return "CHATTER";
	case WMI_SERVICE_COEX_FREQAVOID:
		return "COEX_FREQAVOID";
	case WMI_SERVICE_PACKET_POWER_SAVE:
		return "PACKET_POWER_SAVE";
	case WMI_SERVICE_FORCE_FW_HANG:
		return "FORCE FW HANG";
	case WMI_SERVICE_GPIO:
		return "GPIO";
	case WMI_SERVICE_STA_DTIM_PS_MODULATED_DTIM:
		return "MODULATED DTIM";
	case WMI_STA_UAPSD_BASIC_AUTO_TRIG:
		return "BASIC UAPSD";
	case WMI_STA_UAPSD_VAR_AUTO_TRIG:
		return "VAR UAPSD";
	case WMI_SERVICE_STA_KEEP_ALIVE:
		return "STA KEEP ALIVE";
	case WMI_SERVICE_TX_ENCAP:
		return "TX ENCAP";
	default:
		return "UNKNOWN SERVICE\n";
	}
}


#define WMI_SERVICE_BM_SIZE \
	((WMI_MAX_SERVICE + sizeof(u32) - 1)/sizeof(u32))

/* 2 word representation of MAC addr */
struct wmi_mac_addr {
	union {
		u8 addr[6];
		struct {
			u32 word0;
			u32 word1;
		} __packed;
	} __packed;
} __packed;

/* macro to convert MAC address from WMI word format to char array */
#define WMI_MAC_ADDR_TO_CHAR_ARRAY(pwmi_mac_addr, c_macaddr) do { \
	(c_macaddr)[0] =  ((pwmi_mac_addr)->word0) & 0xff; \
	(c_macaddr)[1] = (((pwmi_mac_addr)->word0) >> 8) & 0xff; \
	(c_macaddr)[2] = (((pwmi_mac_addr)->word0) >> 16) & 0xff; \
	(c_macaddr)[3] = (((pwmi_mac_addr)->word0) >> 24) & 0xff; \
	(c_macaddr)[4] =  ((pwmi_mac_addr)->word1) & 0xff; \
	(c_macaddr)[5] = (((pwmi_mac_addr)->word1) >> 8) & 0xff; \
	} while (0)

struct wmi_cmd_map {
	u32 init_cmdid;
	u32 start_scan_cmdid;
	u32 stop_scan_cmdid;
	u32 scan_chan_list_cmdid;
	u32 scan_sch_prio_tbl_cmdid;
	u32 pdev_set_regdomain_cmdid;
	u32 pdev_set_channel_cmdid;
	u32 pdev_set_param_cmdid;
	u32 pdev_pktlog_enable_cmdid;
	u32 pdev_pktlog_disable_cmdid;
	u32 pdev_set_wmm_params_cmdid;
	u32 pdev_set_ht_cap_ie_cmdid;
	u32 pdev_set_vht_cap_ie_cmdid;
	u32 pdev_set_dscp_tid_map_cmdid;
	u32 pdev_set_quiet_mode_cmdid;
	u32 pdev_green_ap_ps_enable_cmdid;
	u32 pdev_get_tpc_config_cmdid;
	u32 pdev_set_base_macaddr_cmdid;
	u32 vdev_create_cmdid;
	u32 vdev_delete_cmdid;
	u32 vdev_start_request_cmdid;
	u32 vdev_restart_request_cmdid;
	u32 vdev_up_cmdid;
	u32 vdev_stop_cmdid;
	u32 vdev_down_cmdid;
	u32 vdev_set_param_cmdid;
	u32 vdev_install_key_cmdid;
	u32 peer_create_cmdid;
	u32 peer_delete_cmdid;
	u32 peer_flush_tids_cmdid;
	u32 peer_set_param_cmdid;
	u32 peer_assoc_cmdid;
	u32 peer_add_wds_entry_cmdid;
	u32 peer_remove_wds_entry_cmdid;
	u32 peer_mcast_group_cmdid;
	u32 bcn_tx_cmdid;
	u32 pdev_send_bcn_cmdid;
	u32 bcn_tmpl_cmdid;
	u32 bcn_filter_rx_cmdid;
	u32 prb_req_filter_rx_cmdid;
	u32 mgmt_tx_cmdid;
	u32 prb_tmpl_cmdid;
	u32 addba_clear_resp_cmdid;
	u32 addba_send_cmdid;
	u32 addba_status_cmdid;
	u32 delba_send_cmdid;
	u32 addba_set_resp_cmdid;
	u32 send_singleamsdu_cmdid;
	u32 sta_powersave_mode_cmdid;
	u32 sta_powersave_param_cmdid;
	u32 sta_mimo_ps_mode_cmdid;
	u32 pdev_dfs_enable_cmdid;
	u32 pdev_dfs_disable_cmdid;
	u32 roam_scan_mode;
	u32 roam_scan_rssi_threshold;
	u32 roam_scan_period;
	u32 roam_scan_rssi_change_threshold;
	u32 roam_ap_profile;
	u32 ofl_scan_add_ap_profile;
	u32 ofl_scan_remove_ap_profile;
	u32 ofl_scan_period;
	u32 p2p_dev_set_device_info;
	u32 p2p_dev_set_discoverability;
	u32 p2p_go_set_beacon_ie;
	u32 p2p_go_set_probe_resp_ie;
	u32 p2p_set_vendor_ie_data_cmdid;
	u32 ap_ps_peer_param_cmdid;
	u32 ap_ps_peer_uapsd_coex_cmdid;
	u32 peer_rate_retry_sched_cmdid;
	u32 wlan_profile_trigger_cmdid;
	u32 wlan_profile_set_hist_intvl_cmdid;
	u32 wlan_profile_get_profile_data_cmdid;
	u32 wlan_profile_enable_profile_id_cmdid;
	u32 wlan_profile_list_profile_id_cmdid;
	u32 pdev_suspend_cmdid;
	u32 pdev_resume_cmdid;
	u32 add_bcn_filter_cmdid;
	u32 rmv_bcn_filter_cmdid;
	u32 wow_add_wake_pattern_cmdid;
	u32 wow_del_wake_pattern_cmdid;
	u32 wow_enable_disable_wake_event_cmdid;
	u32 wow_enable_cmdid;
	u32 wow_hostwakeup_from_sleep_cmdid;
	u32 rtt_measreq_cmdid;
	u32 rtt_tsf_cmdid;
	u32 vdev_spectral_scan_configure_cmdid;
	u32 vdev_spectral_scan_enable_cmdid;
	u32 request_stats_cmdid;
	u32 set_arp_ns_offload_cmdid;
	u32 network_list_offload_config_cmdid;
	u32 gtk_offload_cmdid;
	u32 csa_offload_enable_cmdid;
	u32 csa_offload_chanswitch_cmdid;
	u32 chatter_set_mode_cmdid;
	u32 peer_tid_addba_cmdid;
	u32 peer_tid_delba_cmdid;
	u32 sta_dtim_ps_method_cmdid;
	u32 sta_uapsd_auto_trig_cmdid;
	u32 sta_keepalive_cmd;
	u32 echo_cmdid;
	u32 pdev_utf_cmdid;
	u32 dbglog_cfg_cmdid;
	u32 pdev_qvit_cmdid;
	u32 pdev_ftm_intg_cmdid;
	u32 vdev_set_keepalive_cmdid;
	u32 vdev_get_keepalive_cmdid;
	u32 force_fw_hang_cmdid;
	u32 gpio_config_cmdid;
	u32 gpio_output_cmdid;
};

/*
 * wmi command groups.
 */
enum wmi_cmd_group {
	/* 0 to 2 are reserved */
	WMI_GRP_START = 0x3,
	WMI_GRP_SCAN = WMI_GRP_START,
	WMI_GRP_PDEV,
	WMI_GRP_VDEV,
	WMI_GRP_PEER,
	WMI_GRP_MGMT,
	WMI_GRP_BA_NEG,
	WMI_GRP_STA_PS,
	WMI_GRP_DFS,
	WMI_GRP_ROAM,
	WMI_GRP_OFL_SCAN,
	WMI_GRP_P2P,
	WMI_GRP_AP_PS,
	WMI_GRP_RATE_CTRL,
	WMI_GRP_PROFILE,
	WMI_GRP_SUSPEND,
	WMI_GRP_BCN_FILTER,
	WMI_GRP_WOW,
	WMI_GRP_RTT,
	WMI_GRP_SPECTRAL,
	WMI_GRP_STATS,
	WMI_GRP_ARP_NS_OFL,
	WMI_GRP_NLO_OFL,
	WMI_GRP_GTK_OFL,
	WMI_GRP_CSA_OFL,
	WMI_GRP_CHATTER,
	WMI_GRP_TID_ADDBA,
	WMI_GRP_MISC,
	WMI_GRP_GPIO,
};

#define WMI_CMD_GRP(grp_id) (((grp_id) << 12) | 0x1)
#define WMI_EVT_GRP_START_ID(grp_id) (((grp_id) << 12) | 0x1)

#define WMI_CMD_UNSUPPORTED 0

/* Command IDs and command events for MAIN FW. */
enum wmi_cmd_id {
	WMI_INIT_CMDID = 0x1,

	/* Scan specific commands */
	WMI_START_SCAN_CMDID = WMI_CMD_GRP(WMI_GRP_SCAN),
	WMI_STOP_SCAN_CMDID,
	WMI_SCAN_CHAN_LIST_CMDID,
	WMI_SCAN_SCH_PRIO_TBL_CMDID,

	/* PDEV (physical device) specific commands */
	WMI_PDEV_SET_REGDOMAIN_CMDID = WMI_CMD_GRP(WMI_GRP_PDEV),
	WMI_PDEV_SET_CHANNEL_CMDID,
	WMI_PDEV_SET_PARAM_CMDID,
	WMI_PDEV_PKTLOG_ENABLE_CMDID,
	WMI_PDEV_PKTLOG_DISABLE_CMDID,
	WMI_PDEV_SET_WMM_PARAMS_CMDID,
	WMI_PDEV_SET_HT_CAP_IE_CMDID,
	WMI_PDEV_SET_VHT_CAP_IE_CMDID,
	WMI_PDEV_SET_DSCP_TID_MAP_CMDID,
	WMI_PDEV_SET_QUIET_MODE_CMDID,
	WMI_PDEV_GREEN_AP_PS_ENABLE_CMDID,
	WMI_PDEV_GET_TPC_CONFIG_CMDID,
	WMI_PDEV_SET_BASE_MACADDR_CMDID,

	/* VDEV (virtual device) specific commands */
	WMI_VDEV_CREATE_CMDID = WMI_CMD_GRP(WMI_GRP_VDEV),
	WMI_VDEV_DELETE_CMDID,
	WMI_VDEV_START_REQUEST_CMDID,
	WMI_VDEV_RESTART_REQUEST_CMDID,
	WMI_VDEV_UP_CMDID,
	WMI_VDEV_STOP_CMDID,
	WMI_VDEV_DOWN_CMDID,
	WMI_VDEV_SET_PARAM_CMDID,
	WMI_VDEV_INSTALL_KEY_CMDID,

	/* peer specific commands */
	WMI_PEER_CREATE_CMDID = WMI_CMD_GRP(WMI_GRP_PEER),
	WMI_PEER_DELETE_CMDID,
	WMI_PEER_FLUSH_TIDS_CMDID,
	WMI_PEER_SET_PARAM_CMDID,
	WMI_PEER_ASSOC_CMDID,
	WMI_PEER_ADD_WDS_ENTRY_CMDID,
	WMI_PEER_REMOVE_WDS_ENTRY_CMDID,
	WMI_PEER_MCAST_GROUP_CMDID,

	/* beacon/management specific commands */
	WMI_BCN_TX_CMDID = WMI_CMD_GRP(WMI_GRP_MGMT),
	WMI_PDEV_SEND_BCN_CMDID,
	WMI_BCN_TMPL_CMDID,
	WMI_BCN_FILTER_RX_CMDID,
	WMI_PRB_REQ_FILTER_RX_CMDID,
	WMI_MGMT_TX_CMDID,
	WMI_PRB_TMPL_CMDID,

	/* commands to directly control BA negotiation directly from host. */
	WMI_ADDBA_CLEAR_RESP_CMDID = WMI_CMD_GRP(WMI_GRP_BA_NEG),
	WMI_ADDBA_SEND_CMDID,
	WMI_ADDBA_STATUS_CMDID,
	WMI_DELBA_SEND_CMDID,
	WMI_ADDBA_SET_RESP_CMDID,
	WMI_SEND_SINGLEAMSDU_CMDID,

	/* Station power save specific config */
	WMI_STA_POWERSAVE_MODE_CMDID = WMI_CMD_GRP(WMI_GRP_STA_PS),
	WMI_STA_POWERSAVE_PARAM_CMDID,
	WMI_STA_MIMO_PS_MODE_CMDID,

	/** DFS-specific commands */
	WMI_PDEV_DFS_ENABLE_CMDID = WMI_CMD_GRP(WMI_GRP_DFS),
	WMI_PDEV_DFS_DISABLE_CMDID,

	/* Roaming specific  commands */
	WMI_ROAM_SCAN_MODE = WMI_CMD_GRP(WMI_GRP_ROAM),
	WMI_ROAM_SCAN_RSSI_THRESHOLD,
	WMI_ROAM_SCAN_PERIOD,
	WMI_ROAM_SCAN_RSSI_CHANGE_THRESHOLD,
	WMI_ROAM_AP_PROFILE,

	/* offload scan specific commands */
	WMI_OFL_SCAN_ADD_AP_PROFILE = WMI_CMD_GRP(WMI_GRP_OFL_SCAN),
	WMI_OFL_SCAN_REMOVE_AP_PROFILE,
	WMI_OFL_SCAN_PERIOD,

	/* P2P specific commands */
	WMI_P2P_DEV_SET_DEVICE_INFO = WMI_CMD_GRP(WMI_GRP_P2P),
	WMI_P2P_DEV_SET_DISCOVERABILITY,
	WMI_P2P_GO_SET_BEACON_IE,
	WMI_P2P_GO_SET_PROBE_RESP_IE,
	WMI_P2P_SET_VENDOR_IE_DATA_CMDID,

	/* AP power save specific config */
	WMI_AP_PS_PEER_PARAM_CMDID = WMI_CMD_GRP(WMI_GRP_AP_PS),
	WMI_AP_PS_PEER_UAPSD_COEX_CMDID,

	/* Rate-control specific commands */
	WMI_PEER_RATE_RETRY_SCHED_CMDID =
	WMI_CMD_GRP(WMI_GRP_RATE_CTRL),

	/* WLAN Profiling commands. */
	WMI_WLAN_PROFILE_TRIGGER_CMDID = WMI_CMD_GRP(WMI_GRP_PROFILE),
	WMI_WLAN_PROFILE_SET_HIST_INTVL_CMDID,
	WMI_WLAN_PROFILE_GET_PROFILE_DATA_CMDID,
	WMI_WLAN_PROFILE_ENABLE_PROFILE_ID_CMDID,
	WMI_WLAN_PROFILE_LIST_PROFILE_ID_CMDID,

	/* Suspend resume command Ids */
	WMI_PDEV_SUSPEND_CMDID = WMI_CMD_GRP(WMI_GRP_SUSPEND),
	WMI_PDEV_RESUME_CMDID,

	/* Beacon filter commands */
	WMI_ADD_BCN_FILTER_CMDID = WMI_CMD_GRP(WMI_GRP_BCN_FILTER),
	WMI_RMV_BCN_FILTER_CMDID,

	/* WOW Specific WMI commands*/
	WMI_WOW_ADD_WAKE_PATTERN_CMDID = WMI_CMD_GRP(WMI_GRP_WOW),
	WMI_WOW_DEL_WAKE_PATTERN_CMDID,
	WMI_WOW_ENABLE_DISABLE_WAKE_EVENT_CMDID,
	WMI_WOW_ENABLE_CMDID,
	WMI_WOW_HOSTWAKEUP_FROM_SLEEP_CMDID,

	/* RTT measurement related cmd */
	WMI_RTT_MEASREQ_CMDID = WMI_CMD_GRP(WMI_GRP_RTT),
	WMI_RTT_TSF_CMDID,

	/* spectral scan commands */
	WMI_VDEV_SPECTRAL_SCAN_CONFIGURE_CMDID = WMI_CMD_GRP(WMI_GRP_SPECTRAL),
	WMI_VDEV_SPECTRAL_SCAN_ENABLE_CMDID,

	/* F/W stats */
	WMI_REQUEST_STATS_CMDID = WMI_CMD_GRP(WMI_GRP_STATS),

	/* ARP OFFLOAD REQUEST*/
	WMI_SET_ARP_NS_OFFLOAD_CMDID = WMI_CMD_GRP(WMI_GRP_ARP_NS_OFL),

	/* NS offload confid*/
	WMI_NETWORK_LIST_OFFLOAD_CONFIG_CMDID = WMI_CMD_GRP(WMI_GRP_NLO_OFL),

	/* GTK offload Specific WMI commands*/
	WMI_GTK_OFFLOAD_CMDID = WMI_CMD_GRP(WMI_GRP_GTK_OFL),

	/* CSA offload Specific WMI commands*/
	WMI_CSA_OFFLOAD_ENABLE_CMDID = WMI_CMD_GRP(WMI_GRP_CSA_OFL),
	WMI_CSA_OFFLOAD_CHANSWITCH_CMDID,

	/* Chatter commands*/
	WMI_CHATTER_SET_MODE_CMDID = WMI_CMD_GRP(WMI_GRP_CHATTER),

	/* addba specific commands */
	WMI_PEER_TID_ADDBA_CMDID = WMI_CMD_GRP(WMI_GRP_TID_ADDBA),
	WMI_PEER_TID_DELBA_CMDID,

	/* set station mimo powersave method */
	WMI_STA_DTIM_PS_METHOD_CMDID,
	/* Configure the Station UAPSD AC Auto Trigger Parameters */
	WMI_STA_UAPSD_AUTO_TRIG_CMDID,

	/* STA Keep alive parameter configuration,
	   Requires WMI_SERVICE_STA_KEEP_ALIVE */
	WMI_STA_KEEPALIVE_CMD,

	/* misc command group */
	WMI_ECHO_CMDID = WMI_CMD_GRP(WMI_GRP_MISC),
	WMI_PDEV_UTF_CMDID,
	WMI_DBGLOG_CFG_CMDID,
	WMI_PDEV_QVIT_CMDID,
	WMI_PDEV_FTM_INTG_CMDID,
	WMI_VDEV_SET_KEEPALIVE_CMDID,
	WMI_VDEV_GET_KEEPALIVE_CMDID,
	WMI_FORCE_FW_HANG_CMDID,

	/* GPIO Configuration */
	WMI_GPIO_CONFIG_CMDID = WMI_CMD_GRP(WMI_GRP_GPIO),
	WMI_GPIO_OUTPUT_CMDID,
};

enum wmi_event_id {
	WMI_SERVICE_READY_EVENTID = 0x1,
	WMI_READY_EVENTID,

	/* Scan specific events */
	WMI_SCAN_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_SCAN),

	/* PDEV specific events */
	WMI_PDEV_TPC_CONFIG_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_PDEV),
	WMI_CHAN_INFO_EVENTID,
	WMI_PHYERR_EVENTID,

	/* VDEV specific events */
	WMI_VDEV_START_RESP_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_VDEV),
	WMI_VDEV_STOPPED_EVENTID,
	WMI_VDEV_INSTALL_KEY_COMPLETE_EVENTID,

	/* peer specific events */
	WMI_PEER_STA_KICKOUT_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_PEER),

	/* beacon/mgmt specific events */
	WMI_MGMT_RX_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_MGMT),
	WMI_HOST_SWBA_EVENTID,
	WMI_TBTTOFFSET_UPDATE_EVENTID,

	/* ADDBA Related WMI Events*/
	WMI_TX_DELBA_COMPLETE_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_BA_NEG),
	WMI_TX_ADDBA_COMPLETE_EVENTID,

	/* Roam event to trigger roaming on host */
	WMI_ROAM_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_ROAM),
	WMI_PROFILE_MATCH,

	/* WoW */
	WMI_WOW_WAKEUP_HOST_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_WOW),

	/* RTT */
	WMI_RTT_MEASUREMENT_REPORT_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_RTT),
	WMI_TSF_MEASUREMENT_REPORT_EVENTID,
	WMI_RTT_ERROR_REPORT_EVENTID,

	/* GTK offload */
	WMI_GTK_OFFLOAD_STATUS_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_GTK_OFL),
	WMI_GTK_REKEY_FAIL_EVENTID,

	/* CSA IE received event */
	WMI_CSA_HANDLING_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_CSA_OFL),

	/* Misc events */
	WMI_ECHO_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_MISC),
	WMI_PDEV_UTF_EVENTID,
	WMI_DEBUG_MESG_EVENTID,
	WMI_UPDATE_STATS_EVENTID,
	WMI_DEBUG_PRINT_EVENTID,
	WMI_DCS_INTERFERENCE_EVENTID,
	WMI_PDEV_QVIT_EVENTID,
	WMI_WLAN_PROFILE_DATA_EVENTID,
	WMI_PDEV_FTM_INTG_EVENTID,
	WMI_WLAN_FREQ_AVOID_EVENTID,
	WMI_VDEV_GET_KEEPALIVE_EVENTID,

	/* GPIO Event */
	WMI_GPIO_INPUT_EVENTID = WMI_EVT_GRP_START_ID(WMI_GRP_GPIO),
};

/* Command IDs and command events for 10.X firmware */
enum wmi_10x_cmd_id {
	WMI_10X_START_CMDID = 0x9000,
	WMI_10X_END_CMDID = 0x9FFF,

	/* initialize the wlan sub system */
	WMI_10X_INIT_CMDID,

	/* Scan specific commands */

	WMI_10X_START_SCAN_CMDID = WMI_10X_START_CMDID,
	WMI_10X_STOP_SCAN_CMDID,
	WMI_10X_SCAN_CHAN_LIST_CMDID,
	WMI_10X_ECHO_CMDID,

	/* PDEV(physical device) specific commands */
	WMI_10X_PDEV_SET_REGDOMAIN_CMDID,
	WMI_10X_PDEV_SET_CHANNEL_CMDID,
	WMI_10X_PDEV_SET_PARAM_CMDID,
	WMI_10X_PDEV_PKTLOG_ENABLE_CMDID,
	WMI_10X_PDEV_PKTLOG_DISABLE_CMDID,
	WMI_10X_PDEV_SET_WMM_PARAMS_CMDID,
	WMI_10X_PDEV_SET_HT_CAP_IE_CMDID,
	WMI_10X_PDEV_SET_VHT_CAP_IE_CMDID,
	WMI_10X_PDEV_SET_BASE_MACADDR_CMDID,
	WMI_10X_PDEV_SET_DSCP_TID_MAP_CMDID,
	WMI_10X_PDEV_SET_QUIET_MODE_CMDID,
	WMI_10X_PDEV_GREEN_AP_PS_ENABLE_CMDID,
	WMI_10X_PDEV_GET_TPC_CONFIG_CMDID,

	/* VDEV(virtual device) specific commands */
	WMI_10X_VDEV_CREATE_CMDID,
	WMI_10X_VDEV_DELETE_CMDID,
	WMI_10X_VDEV_START_REQUEST_CMDID,
	WMI_10X_VDEV_RESTART_REQUEST_CMDID,
	WMI_10X_VDEV_UP_CMDID,
	WMI_10X_VDEV_STOP_CMDID,
	WMI_10X_VDEV_DOWN_CMDID,
	WMI_10X_VDEV_STANDBY_RESPONSE_CMDID,
	WMI_10X_VDEV_RESUME_RESPONSE_CMDID,
	WMI_10X_VDEV_SET_PARAM_CMDID,
	WMI_10X_VDEV_INSTALL_KEY_CMDID,

	/* peer specific commands */
	WMI_10X_PEER_CREATE_CMDID,
	WMI_10X_PEER_DELETE_CMDID,
	WMI_10X_PEER_FLUSH_TIDS_CMDID,
	WMI_10X_PEER_SET_PARAM_CMDID,
	WMI_10X_PEER_ASSOC_CMDID,
	WMI_10X_PEER_ADD_WDS_ENTRY_CMDID,
	WMI_10X_PEER_REMOVE_WDS_ENTRY_CMDID,
	WMI_10X_PEER_MCAST_GROUP_CMDID,

	/* beacon/management specific commands */

	WMI_10X_BCN_TX_CMDID,
	WMI_10X_BCN_PRB_TMPL_CMDID,
	WMI_10X_BCN_FILTER_RX_CMDID,
	WMI_10X_PRB_REQ_FILTER_RX_CMDID,
	WMI_10X_MGMT_TX_CMDID,

	/* commands to directly control ba negotiation directly from host. */
	WMI_10X_ADDBA_CLEAR_RESP_CMDID,
	WMI_10X_ADDBA_SEND_CMDID,
	WMI_10X_ADDBA_STATUS_CMDID,
	WMI_10X_DELBA_SEND_CMDID,
	WMI_10X_ADDBA_SET_RESP_CMDID,
	WMI_10X_SEND_SINGLEAMSDU_CMDID,

	/* Station power save specific config */
	WMI_10X_STA_POWERSAVE_MODE_CMDID,
	WMI_10X_STA_POWERSAVE_PARAM_CMDID,
	WMI_10X_STA_MIMO_PS_MODE_CMDID,

	/* set debug log config */
	WMI_10X_DBGLOG_CFG_CMDID,

	/* DFS-specific commands */
	WMI_10X_PDEV_DFS_ENABLE_CMDID,
	WMI_10X_PDEV_DFS_DISABLE_CMDID,

	/* QVIT specific command id */
	WMI_10X_PDEV_QVIT_CMDID,

	/* Offload Scan and Roaming related  commands */
	WMI_10X_ROAM_SCAN_MODE,
	WMI_10X_ROAM_SCAN_RSSI_THRESHOLD,
	WMI_10X_ROAM_SCAN_PERIOD,
	WMI_10X_ROAM_SCAN_RSSI_CHANGE_THRESHOLD,
	WMI_10X_ROAM_AP_PROFILE,
	WMI_10X_OFL_SCAN_ADD_AP_PROFILE,
	WMI_10X_OFL_SCAN_REMOVE_AP_PROFILE,
	WMI_10X_OFL_SCAN_PERIOD,

	/* P2P specific commands */
	WMI_10X_P2P_DEV_SET_DEVICE_INFO,
	WMI_10X_P2P_DEV_SET_DISCOVERABILITY,
	WMI_10X_P2P_GO_SET_BEACON_IE,
	WMI_10X_P2P_GO_SET_PROBE_RESP_IE,

	/* AP power save specific config */
	WMI_10X_AP_PS_PEER_PARAM_CMDID,
	WMI_10X_AP_PS_PEER_UAPSD_COEX_CMDID,

	/* Rate-control specific commands */
	WMI_10X_PEER_RATE_RETRY_SCHED_CMDID,

	/* WLAN Profiling commands. */
	WMI_10X_WLAN_PROFILE_TRIGGER_CMDID,
	WMI_10X_WLAN_PROFILE_SET_HIST_INTVL_CMDID,
	WMI_10X_WLAN_PROFILE_GET_PROFILE_DATA_CMDID,
	WMI_10X_WLAN_PROFILE_ENABLE_PROFILE_ID_CMDID,
	WMI_10X_WLAN_PROFILE_LIST_PROFILE_ID_CMDID,

	/* Suspend resume command Ids */
	WMI_10X_PDEV_SUSPEND_CMDID,
	WMI_10X_PDEV_RESUME_CMDID,

	/* Beacon filter commands */
	WMI_10X_ADD_BCN_FILTER_CMDID,
	WMI_10X_RMV_BCN_FILTER_CMDID,

	/* WOW Specific WMI commands*/
	WMI_10X_WOW_ADD_WAKE_PATTERN_CMDID,
	WMI_10X_WOW_DEL_WAKE_PATTERN_CMDID,
	WMI_10X_WOW_ENABLE_DISABLE_WAKE_EVENT_CMDID,
	WMI_10X_WOW_ENABLE_CMDID,
	WMI_10X_WOW_HOSTWAKEUP_FROM_SLEEP_CMDID,

	/* RTT measurement related cmd */
	WMI_10X_RTT_MEASREQ_CMDID,
	WMI_10X_RTT_TSF_CMDID,

	/* transmit beacon by value */
	WMI_10X_PDEV_SEND_BCN_CMDID,

	/* F/W stats */
	WMI_10X_VDEV_SPECTRAL_SCAN_CONFIGURE_CMDID,
	WMI_10X_VDEV_SPECTRAL_SCAN_ENABLE_CMDID,
	WMI_10X_REQUEST_STATS_CMDID,

	/* GPIO Configuration */
	WMI_10X_GPIO_CONFIG_CMDID,
	WMI_10X_GPIO_OUTPUT_CMDID,

	WMI_10X_PDEV_UTF_CMDID = WMI_10X_END_CMDID - 1,
};

enum wmi_10x_event_id {
	WMI_10X_SERVICE_READY_EVENTID = 0x8000,
	WMI_10X_READY_EVENTID,
	WMI_10X_START_EVENTID = 0x9000,
	WMI_10X_END_EVENTID = 0x9FFF,

	/* Scan specific events */
	WMI_10X_SCAN_EVENTID = WMI_10X_START_EVENTID,
	WMI_10X_ECHO_EVENTID,
	WMI_10X_DEBUG_MESG_EVENTID,
	WMI_10X_UPDATE_STATS_EVENTID,

	/* Instantaneous RSSI event */
	WMI_10X_INST_RSSI_STATS_EVENTID,

	/* VDEV specific events */
	WMI_10X_VDEV_START_RESP_EVENTID,
	WMI_10X_VDEV_STANDBY_REQ_EVENTID,
	WMI_10X_VDEV_RESUME_REQ_EVENTID,
	WMI_10X_VDEV_STOPPED_EVENTID,

	/* peer  specific events */
	WMI_10X_PEER_STA_KICKOUT_EVENTID,

	/* beacon/mgmt specific events */
	WMI_10X_HOST_SWBA_EVENTID,
	WMI_10X_TBTTOFFSET_UPDATE_EVENTID,
	WMI_10X_MGMT_RX_EVENTID,

	/* Channel stats event */
	WMI_10X_CHAN_INFO_EVENTID,

	/* PHY Error specific WMI event */
	WMI_10X_PHYERR_EVENTID,

	/* Roam event to trigger roaming on host */
	WMI_10X_ROAM_EVENTID,

	/* matching AP found from list of profiles */
	WMI_10X_PROFILE_MATCH,

	/* debug print message used for tracing FW code while debugging */
	WMI_10X_DEBUG_PRINT_EVENTID,
	/* VI spoecific event */
	WMI_10X_PDEV_QVIT_EVENTID,
	/* FW code profile data in response to profile request */
	WMI_10X_WLAN_PROFILE_DATA_EVENTID,

	/*RTT related event ID*/
	WMI_10X_RTT_MEASUREMENT_REPORT_EVENTID,
	WMI_10X_TSF_MEASUREMENT_REPORT_EVENTID,
	WMI_10X_RTT_ERROR_REPORT_EVENTID,

	WMI_10X_WOW_WAKEUP_HOST_EVENTID,
	WMI_10X_DCS_INTERFERENCE_EVENTID,

	/* TPC config for the current operating channel */
	WMI_10X_PDEV_TPC_CONFIG_EVENTID,

	WMI_10X_GPIO_INPUT_EVENTID,
	WMI_10X_PDEV_UTF_EVENTID = WMI_10X_END_EVENTID-1,
};

enum wmi_phy_mode {
	MODE_11A        = 0,   /* 11a Mode */
	MODE_11G        = 1,   /* 11b/g Mode */
	MODE_11B        = 2,   /* 11b Mode */
	MODE_11GONLY    = 3,   /* 11g only Mode */
	MODE_11NA_HT20   = 4,  /* 11a HT20 mode */
	MODE_11NG_HT20   = 5,  /* 11g HT20 mode */
	MODE_11NA_HT40   = 6,  /* 11a HT40 mode */
	MODE_11NG_HT40   = 7,  /* 11g HT40 mode */
	MODE_11AC_VHT20 = 8,
	MODE_11AC_VHT40 = 9,
	MODE_11AC_VHT80 = 10,
	/*    MODE_11AC_VHT160 = 11, */
	MODE_11AC_VHT20_2G = 11,
	MODE_11AC_VHT40_2G = 12,
	MODE_11AC_VHT80_2G = 13,
	MODE_UNKNOWN    = 14,
	MODE_MAX        = 14
};

static inline const char *ath10k_wmi_phymode_str(enum wmi_phy_mode mode)
{
	switch (mode) {
	case MODE_11A:
		return "11a";
	case MODE_11G:
		return "11g";
	case MODE_11B:
		return "11b";
	case MODE_11GONLY:
		return "11gonly";
	case MODE_11NA_HT20:
		return "11na-ht20";
	case MODE_11NG_HT20:
		return "11ng-ht20";
	case MODE_11NA_HT40:
		return "11na-ht40";
	case MODE_11NG_HT40:
		return "11ng-ht40";
	case MODE_11AC_VHT20:
		return "11ac-vht20";
	case MODE_11AC_VHT40:
		return "11ac-vht40";
	case MODE_11AC_VHT80:
		return "11ac-vht80";
	case MODE_11AC_VHT20_2G:
		return "11ac-vht20-2g";
	case MODE_11AC_VHT40_2G:
		return "11ac-vht40-2g";
	case MODE_11AC_VHT80_2G:
		return "11ac-vht80-2g";
	case MODE_UNKNOWN:
		/* skip */
		break;

		/* no default handler to allow compiler to check that the
		 * enum is fully handled */
	};

	return "<unknown>";
}

#define WMI_CHAN_LIST_TAG	0x1
#define WMI_SSID_LIST_TAG	0x2
#define WMI_BSSID_LIST_TAG	0x3
#define WMI_IE_TAG		0x4

struct wmi_channel {
	__le32 mhz;
	__le32 band_center_freq1;
	__le32 band_center_freq2; /* valid for 11ac, 80plus80 */
	union {
		__le32 flags; /* WMI_CHAN_FLAG_ */
		struct {
			u8 mode; /* only 6 LSBs */
		} __packed;
	} __packed;
	union {
		__le32 reginfo0;
		struct {
			/* note: power unit is 0.5 dBm */
			u8 min_power;
			u8 max_power;
			u8 reg_power;
			u8 reg_classid;
		} __packed;
	} __packed;
	union {
		__le32 reginfo1;
		struct {
			u8 antenna_max;
		} __packed;
	} __packed;
} __packed;

struct wmi_channel_arg {
	u32 freq;
	u32 band_center_freq1;
	bool passive;
	bool allow_ibss;
	bool allow_ht;
	bool allow_vht;
	bool ht40plus;
	bool chan_radar;
	/* note: power unit is 0.5 dBm */
	u32 min_power;
	u32 max_power;
	u32 max_reg_power;
	u32 max_antenna_gain;
	u32 reg_class_id;
	enum wmi_phy_mode mode;
};

enum wmi_channel_change_cause {
	WMI_CHANNEL_CHANGE_CAUSE_NONE = 0,
	WMI_CHANNEL_CHANGE_CAUSE_CSA,
};

#define WMI_CHAN_FLAG_HT40_PLUS      (1 << 6)
#define WMI_CHAN_FLAG_PASSIVE        (1 << 7)
#define WMI_CHAN_FLAG_ADHOC_ALLOWED  (1 << 8)
#define WMI_CHAN_FLAG_AP_DISABLED    (1 << 9)
#define WMI_CHAN_FLAG_DFS            (1 << 10)
#define WMI_CHAN_FLAG_ALLOW_HT       (1 << 11)
#define WMI_CHAN_FLAG_ALLOW_VHT      (1 << 12)

/* Indicate reason for channel switch */
#define WMI_CHANNEL_CHANGE_CAUSE_CSA (1 << 13)

#define WMI_MAX_SPATIAL_STREAM   3

/* HT Capabilities*/
#define WMI_HT_CAP_ENABLED                0x0001   /* HT Enabled/ disabled */
#define WMI_HT_CAP_HT20_SGI       0x0002   /* Short Guard Interval with HT20 */
#define WMI_HT_CAP_DYNAMIC_SMPS           0x0004   /* Dynamic MIMO powersave */
#define WMI_HT_CAP_TX_STBC                0x0008   /* B3 TX STBC */
#define WMI_HT_CAP_TX_STBC_MASK_SHIFT     3
#define WMI_HT_CAP_RX_STBC                0x0030   /* B4-B5 RX STBC */
#define WMI_HT_CAP_RX_STBC_MASK_SHIFT     4
#define WMI_HT_CAP_LDPC                   0x0040   /* LDPC supported */
#define WMI_HT_CAP_L_SIG_TXOP_PROT        0x0080   /* L-SIG TXOP Protection */
#define WMI_HT_CAP_MPDU_DENSITY           0x0700   /* MPDU Density */
#define WMI_HT_CAP_MPDU_DENSITY_MASK_SHIFT 8
#define WMI_HT_CAP_HT40_SGI               0x0800

#define WMI_HT_CAP_DEFAULT_ALL (WMI_HT_CAP_ENABLED       | \
				WMI_HT_CAP_HT20_SGI      | \
				WMI_HT_CAP_HT40_SGI      | \
				WMI_HT_CAP_TX_STBC       | \
				WMI_HT_CAP_RX_STBC       | \
				WMI_HT_CAP_LDPC)


/*
 * WMI_VHT_CAP_* these maps to ieee 802.11ac vht capability information
 * field. The fields not defined here are not supported, or reserved.
 * Do not change these masks and if you have to add new one follow the
 * bitmask as specified by 802.11ac draft.
 */

#define WMI_VHT_CAP_MAX_MPDU_LEN_MASK            0x00000003
#define WMI_VHT_CAP_RX_LDPC                      0x00000010
#define WMI_VHT_CAP_SGI_80MHZ                    0x00000020
#define WMI_VHT_CAP_TX_STBC                      0x00000080
#define WMI_VHT_CAP_RX_STBC_MASK                 0x00000300
#define WMI_VHT_CAP_RX_STBC_MASK_SHIFT           8
#define WMI_VHT_CAP_MAX_AMPDU_LEN_EXP            0x03800000
#define WMI_VHT_CAP_MAX_AMPDU_LEN_EXP_SHIFT      23
#define WMI_VHT_CAP_RX_FIXED_ANT                 0x10000000
#define WMI_VHT_CAP_TX_FIXED_ANT                 0x20000000

/* The following also refer for max HT AMSDU */
#define WMI_VHT_CAP_MAX_MPDU_LEN_3839            0x00000000
#define WMI_VHT_CAP_MAX_MPDU_LEN_7935            0x00000001
#define WMI_VHT_CAP_MAX_MPDU_LEN_11454           0x00000002

#define WMI_VHT_CAP_DEFAULT_ALL (WMI_VHT_CAP_MAX_MPDU_LEN_11454  | \
				 WMI_VHT_CAP_RX_LDPC             | \
				 WMI_VHT_CAP_SGI_80MHZ           | \
				 WMI_VHT_CAP_TX_STBC             | \
				 WMI_VHT_CAP_RX_STBC_MASK        | \
				 WMI_VHT_CAP_MAX_AMPDU_LEN_EXP   | \
				 WMI_VHT_CAP_RX_FIXED_ANT        | \
				 WMI_VHT_CAP_TX_FIXED_ANT)

/*
 * Interested readers refer to Rx/Tx MCS Map definition as defined in
 * 802.11ac
 */
#define WMI_VHT_MAX_MCS_4_SS_MASK(r, ss)      ((3 & (r)) << (((ss) - 1) << 1))
#define WMI_VHT_MAX_SUPP_RATE_MASK           0x1fff0000
#define WMI_VHT_MAX_SUPP_RATE_MASK_SHIFT     16

enum {
	REGDMN_MODE_11A              = 0x00001, /* 11a channels */
	REGDMN_MODE_TURBO            = 0x00002, /* 11a turbo-only channels */
	REGDMN_MODE_11B              = 0x00004, /* 11b channels */
	REGDMN_MODE_PUREG            = 0x00008, /* 11g channels (OFDM only) */
	REGDMN_MODE_11G              = 0x00008, /* XXX historical */
	REGDMN_MODE_108G             = 0x00020, /* 11a+Turbo channels */
	REGDMN_MODE_108A             = 0x00040, /* 11g+Turbo channels */
	REGDMN_MODE_XR               = 0x00100, /* XR channels */
	REGDMN_MODE_11A_HALF_RATE    = 0x00200, /* 11A half rate channels */
	REGDMN_MODE_11A_QUARTER_RATE = 0x00400, /* 11A quarter rate channels */
	REGDMN_MODE_11NG_HT20        = 0x00800, /* 11N-G HT20 channels */
	REGDMN_MODE_11NA_HT20        = 0x01000, /* 11N-A HT20 channels */
	REGDMN_MODE_11NG_HT40PLUS    = 0x02000, /* 11N-G HT40 + channels */
	REGDMN_MODE_11NG_HT40MINUS   = 0x04000, /* 11N-G HT40 - channels */
	REGDMN_MODE_11NA_HT40PLUS    = 0x08000, /* 11N-A HT40 + channels */
	REGDMN_MODE_11NA_HT40MINUS   = 0x10000, /* 11N-A HT40 - channels */
	REGDMN_MODE_11AC_VHT20       = 0x20000, /* 5Ghz, VHT20 */
	REGDMN_MODE_11AC_VHT40PLUS   = 0x40000, /* 5Ghz, VHT40 + channels */
	REGDMN_MODE_11AC_VHT40MINUS  = 0x80000, /* 5Ghz  VHT40 - channels */
	REGDMN_MODE_11AC_VHT80       = 0x100000, /* 5Ghz, VHT80 channels */
	REGDMN_MODE_ALL              = 0xffffffff
};

#define REGDMN_CAP1_CHAN_HALF_RATE        0x00000001
#define REGDMN_CAP1_CHAN_QUARTER_RATE     0x00000002
#define REGDMN_CAP1_CHAN_HAL49GHZ         0x00000004

/* regulatory capabilities */
#define REGDMN_EEPROM_EEREGCAP_EN_FCC_MIDBAND   0x0040
#define REGDMN_EEPROM_EEREGCAP_EN_KK_U1_EVEN    0x0080
#define REGDMN_EEPROM_EEREGCAP_EN_KK_U2         0x0100
#define REGDMN_EEPROM_EEREGCAP_EN_KK_MIDBAND    0x0200
#define REGDMN_EEPROM_EEREGCAP_EN_KK_U1_ODD     0x0400
#define REGDMN_EEPROM_EEREGCAP_EN_KK_NEW_11A    0x0800

struct hal_reg_capabilities {
	/* regdomain value specified in EEPROM */
	__le32 eeprom_rd;
	/*regdomain */
	__le32 eeprom_rd_ext;
	/* CAP1 capabilities bit map. */
	__le32 regcap1;
	/* REGDMN EEPROM CAP. */
	__le32 regcap2;
	/* REGDMN MODE */
	__le32 wireless_modes;
	__le32 low_2ghz_chan;
	__le32 high_2ghz_chan;
	__le32 low_5ghz_chan;
	__le32 high_5ghz_chan;
} __packed;

enum wlan_mode_capability {
	WHAL_WLAN_11A_CAPABILITY   = 0x1,
	WHAL_WLAN_11G_CAPABILITY   = 0x2,
	WHAL_WLAN_11AG_CAPABILITY  = 0x3,
};

/* structure used by FW for requesting host memory */
struct wlan_host_mem_req {
	/* ID of the request */
	__le32 req_id;
	/* size of the  of each unit */
	__le32 unit_size;
	/* flags to  indicate that
	 * the number units is dependent
	 * on number of resources(num vdevs num peers .. etc)
	 */
	__le32 num_unit_info;
	/*
	 * actual number of units to allocate . if flags in the num_unit_info
	 * indicate that number of units is tied to number of a particular
	 * resource to allocate then  num_units filed is set to 0 and host
	 * will derive the number units from number of the resources it is
	 * requesting.
	 */
	__le32 num_units;
} __packed;

#define WMI_SERVICE_IS_ENABLED(wmi_svc_bmap, svc_id) \
	((((wmi_svc_bmap)[(svc_id)/(sizeof(u32))]) & \
	(1 << ((svc_id)%(sizeof(u32))))) != 0)

/*
 * The following struct holds optional payload for
 * wmi_service_ready_event,e.g., 11ac pass some of the
 * device capability to the host.
 */
struct wmi_service_ready_event {
	__le32 sw_version;
	__le32 sw_version_1;
	__le32 abi_version;
	/* WMI_PHY_CAPABILITY */
	__le32 phy_capability;
	/* Maximum number of frag table entries that SW will populate less 1 */
	__le32 max_frag_entry;
	__le32 wmi_service_bitmap[WMI_SERVICE_BM_SIZE];
	__le32 num_rf_chains;
	/*
	 * The following field is only valid for service type
	 * WMI_SERVICE_11AC
	 */
	__le32 ht_cap_info; /* WMI HT Capability */
	__le32 vht_cap_info; /* VHT capability info field of 802.11ac */
	__le32 vht_supp_mcs; /* VHT Supported MCS Set field Rx/Tx same */
	__le32 hw_min_tx_power;
	__le32 hw_max_tx_power;
	struct hal_reg_capabilities hal_reg_capabilities;
	__le32 sys_cap_info;
	__le32 min_pkt_size_enable; /* Enterprise mode short pkt enable */
	/*
	 * Max beacon and Probe Response IE offload size
	 * (includes optional P2P IEs)
	 */
	__le32 max_bcn_ie_size;
	/*
	 * request to host to allocate a chuck of memory and pss it down to FW
	 * via WM_INIT. FW uses this as FW extesnsion memory for saving its
	 * data structures. Only valid for low latency interfaces like PCIE
	 * where FW can access this memory directly (or) by DMA.
	 */
	__le32 num_mem_reqs;
	struct wlan_host_mem_req mem_reqs[1];
} __packed;

/* This is the definition from 10.X firmware branch */
struct wmi_service_ready_event_10x {
	__le32 sw_version;
	__le32 abi_version;

	/* WMI_PHY_CAPABILITY */
	__le32 phy_capability;

	/* Maximum number of frag table entries that SW will populate less 1 */
	__le32 max_frag_entry;
	__le32 wmi_service_bitmap[WMI_SERVICE_BM_SIZE];
	__le32 num_rf_chains;

	/*
	 * The following field is only valid for service type
	 * WMI_SERVICE_11AC
	 */
	__le32 ht_cap_info; /* WMI HT Capability */
	__le32 vht_cap_info; /* VHT capability info field of 802.11ac */
	__le32 vht_supp_mcs; /* VHT Supported MCS Set field Rx/Tx same */
	__le32 hw_min_tx_power;
	__le32 hw_max_tx_power;

	struct hal_reg_capabilities hal_reg_capabilities;

	__le32 sys_cap_info;
	__le32 min_pkt_size_enable; /* Enterprise mode short pkt enable */

	/*
	 * request to host to allocate a chuck of memory and pss it down to FW
	 * via WM_INIT. FW uses this as FW extesnsion memory for saving its
	 * data structures. Only valid for low latency interfaces like PCIE
	 * where FW can access this memory directly (or) by DMA.
	 */
	__le32 num_mem_reqs;

	struct wlan_host_mem_req mem_reqs[1];
} __packed;


#define WMI_SERVICE_READY_TIMEOUT_HZ (5*HZ)
#define WMI_UNIFIED_READY_TIMEOUT_HZ (5*HZ)

struct wmi_ready_event {
	__le32 sw_version;
	__le32 abi_version;
	struct wmi_mac_addr mac_addr;
	__le32 status;
} __packed;

struct wmi_resource_config {
	/* number of virtual devices (VAPs) to support */
	__le32 num_vdevs;

	/* number of peer nodes to support */
	__le32 num_peers;

	/*
	 * In offload mode target supports features like WOW, chatter and
	 * other protocol offloads. In order to support them some
	 * functionalities like reorder buffering, PN checking need to be
	 * done in target. This determines maximum number of peers suported
	 * by target in offload mode
	 */
	__le32 num_offload_peers;

	/* For target-based RX reordering */
	__le32 num_offload_reorder_bufs;

	/* number of keys per peer */
	__le32 num_peer_keys;

	/* total number of TX/RX data TIDs */
	__le32 num_tids;

	/*
	 * max skid for resolving hash collisions
	 *
	 *   The address search table is sparse, so that if two MAC addresses
	 *   result in the same hash value, the second of these conflicting
	 *   entries can slide to the next index in the address search table,
	 *   and use it, if it is unoccupied.  This ast_skid_limit parameter
	 *   specifies the upper bound on how many subsequent indices to search
	 *   over to find an unoccupied space.
	 */
	__le32 ast_skid_limit;

	/*
	 * the nominal chain mask for transmit
	 *
	 *   The chain mask may be modified dynamically, e.g. to operate AP
	 *   tx with a reduced number of chains if no clients are associated.
	 *   This configuration parameter specifies the nominal chain-mask that
	 *   should be used when not operating with a reduced set of tx chains.
	 */
	__le32 tx_chain_mask;

	/*
	 * the nominal chain mask for receive
	 *
	 *   The chain mask may be modified dynamically, e.g. for a client
	 *   to use a reduced number of chains for receive if the traffic to
	 *   the client is low enough that it doesn't require downlink MIMO
	 *   or antenna diversity.
	 *   This configuration parameter specifies the nominal chain-mask that
	 *   should be used when not operating with a reduced set of rx chains.
	 */
	__le32 rx_chain_mask;

	/*
	 * what rx reorder timeout (ms) to use for the AC
	 *
	 *   Each WMM access class (voice, video, best-effort, background) will
	 *   have its own timeout value to dictate how long to wait for missing
	 *   rx MPDUs to arrive before flushing subsequent MPDUs that have
	 *   already been received.
	 *   This parameter specifies the timeout in milliseconds for each
	 *   class.
	 */
	__le32 rx_timeout_pri_vi;
	__le32 rx_timeout_pri_vo;
	__le32 rx_timeout_pri_be;
	__le32 rx_timeout_pri_bk;

	/*
	 * what mode the rx should decap packets to
	 *
	 *   MAC can decap to RAW (no decap), native wifi or Ethernet types
	 *   THis setting also determines the default TX behavior, however TX
	 *   behavior can be modified on a per VAP basis during VAP init
	 */
	__le32 rx_decap_mode;

	/* what is the maximum scan requests than can be queued */
	__le32 scan_max_pending_reqs;

	/* maximum VDEV that could use BMISS offload */
	__le32 bmiss_offload_max_vdev;

	/* maximum VDEV that could use offload roaming */
	__le32 roam_offload_max_vdev;

	/* maximum AP profiles that would push to offload roaming */
	__le32 roam_offload_max_ap_profiles;

	/*
	 * how many groups to use for mcast->ucast conversion
	 *
	 *   The target's WAL maintains a table to hold information regarding
	 *   which peers belong to a given multicast group, so that if
	 *   multicast->unicast conversion is enabled, the target can convert
	 *   multicast tx frames to a series of unicast tx frames, to each
	 *   peer within the multicast group.
	     This num_mcast_groups configuration parameter tells the target how
	 *   many multicast groups to provide storage for within its multicast
	 *   group membership table.
	 */
	__le32 num_mcast_groups;

	/*
	 * size to alloc for the mcast membership table
	 *
	 *   This num_mcast_table_elems configuration parameter tells the
	 *   target how many peer elements it needs to provide storage for in
	 *   its multicast group membership table.
	 *   These multicast group membership table elements are shared by the
	 *   multicast groups stored within the table.
	 */
	__le32 num_mcast_table_elems;

	/*
	 * whether/how to do multicast->unicast conversion
	 *
	 *   This configuration parameter specifies whether the target should
	 *   perform multicast --> unicast conversion on transmit, and if so,
	 *   what to do if it finds no entries in its multicast group
	 *   membership table for the multicast IP address in the tx frame.
	 *   Configuration value:
	 *   0 -> Do not perform multicast to unicast conversion.
	 *   1 -> Convert multicast frames to unicast, if the IP multicast
	 *        address from the tx frame is found in the multicast group
	 *        membership table.  If the IP multicast address is not found,
	 *        drop the frame.
	 *   2 -> Convert multicast frames to unicast, if the IP multicast
	 *        address from the tx frame is found in the multicast group
	 *        membership table.  If the IP multicast address is not found,
	 *        transmit the frame as multicast.
	 */
	__le32 mcast2ucast_mode;

	/*
	 * how much memory to allocate for a tx PPDU dbg log
	 *
	 *   This parameter controls how much memory the target will allocate
	 *   to store a log of tx PPDU meta-information (how large the PPDU
	 *   was, when it was sent, whether it was successful, etc.)
	 */
	__le32 tx_dbg_log_size;

	/* how many AST entries to be allocated for WDS */
	__le32 num_wds_entries;

	/*
	 * MAC DMA burst size, e.g., For target PCI limit can be
	 * 0 -default, 1 256B
	 */
	__le32 dma_burst_size;

	/*
	 * Fixed delimiters to be inserted after every MPDU to
	 * account for interface latency to avoid underrun.
	 */
	__le32 mac_aggr_delim;

	/*
	 *   determine whether target is responsible for detecting duplicate
	 *   non-aggregate MPDU and timing out stale fragments.
	 *
	 *   A-MPDU reordering is always performed on the target.
	 *
	 *   0: target responsible for frag timeout and dup checking
	 *   1: host responsible for frag timeout and dup checking
	 */
	__le32 rx_skip_defrag_timeout_dup_detection_check;

	/*
	 * Configuration for VoW :
	 * No of Video Nodes to be supported
	 * and Max no of descriptors for each Video link (node).
	 */
	__le32 vow_config;

	/* maximum VDEV that could use GTK offload */
	__le32 gtk_offload_max_vdev;

	/* Number of msdu descriptors target should use */
	__le32 num_msdu_desc;

	/*
	 * Max. number of Tx fragments per MSDU
	 *  This parameter controls the max number of Tx fragments per MSDU.
	 *  This is sent by the target as part of the WMI_SERVICE_READY event
	 *  and is overriden by the OS shim as required.
	 */
	__le32 max_frag_entries;
} __packed;

struct wmi_resource_config_10x {
	/* number of virtual devices (VAPs) to support */
	__le32 num_vdevs;

	/* number of peer nodes to support */
	__le32 num_peers;

	/* number of keys per peer */
	__le32 num_peer_keys;

	/* total number of TX/RX data TIDs */
	__le32 num_tids;

	/*
	 * max skid for resolving hash collisions
	 *
	 *   The address search table is sparse, so that if two MAC addresses
	 *   result in the same hash value, the second of these conflicting
	 *   entries can slide to the next index in the address search table,
	 *   and use it, if it is unoccupied.  This ast_skid_limit parameter
	 *   specifies the upper bound on how many subsequent indices to search
	 *   over to find an unoccupied space.
	 */
	__le32 ast_skid_limit;

	/*
	 * the nominal chain mask for transmit
	 *
	 *   The chain mask may be modified dynamically, e.g. to operate AP
	 *   tx with a reduced number of chains if no clients are associated.
	 *   This configuration parameter specifies the nominal chain-mask that
	 *   should be used when not operating with a reduced set of tx chains.
	 */
	__le32 tx_chain_mask;

	/*
	 * the nominal chain mask for receive
	 *
	 *   The chain mask may be modified dynamically, e.g. for a client
	 *   to use a reduced number of chains for receive if the traffic to
	 *   the client is low enough that it doesn't require downlink MIMO
	 *   or antenna diversity.
	 *   This configuration parameter specifies the nominal chain-mask that
	 *   should be used when not operating with a reduced set of rx chains.
	 */
	__le32 rx_chain_mask;

	/*
	 * what rx reorder timeout (ms) to use for the AC
	 *
	 *   Each WMM access class (voice, video, best-effort, background) will
	 *   have its own timeout value to dictate how long to wait for missing
	 *   rx MPDUs to arrive before flushing subsequent MPDUs that have
	 *   already been received.
	 *   This parameter specifies the timeout in milliseconds for each
	 *   class.
	 */
	__le32 rx_timeout_pri_vi;
	__le32 rx_timeout_pri_vo;
	__le32 rx_timeout_pri_be;
	__le32 rx_timeout_pri_bk;

	/*
	 * what mode the rx should decap packets to
	 *
	 *   MAC can decap to RAW (no decap), native wifi or Ethernet types
	 *   THis setting also determines the default TX behavior, however TX
	 *   behavior can be modified on a per VAP basis during VAP init
	 */
	__le32 rx_decap_mode;

	/* what is the maximum scan requests than can be queued */
	__le32 scan_max_pending_reqs;

	/* maximum VDEV that could use BMISS offload */
	__le32 bmiss_offload_max_vdev;

	/* maximum VDEV that could use offload roaming */
	__le32 roam_offload_max_vdev;

	/* maximum AP profiles that would push to offload roaming */
	__le32 roam_offload_max_ap_profiles;

	/*
	 * how many groups to use for mcast->ucast conversion
	 *
	 *   The target's WAL maintains a table to hold information regarding
	 *   which peers belong to a given multicast group, so that if
	 *   multicast->unicast conversion is enabled, the target can convert
	 *   multicast tx frames to a series of unicast tx frames, to each
	 *   peer within the multicast group.
	     This num_mcast_groups configuration parameter tells the target how
	 *   many multicast groups to provide storage for within its multicast
	 *   group membership table.
	 */
	__le32 num_mcast_groups;

	/*
	 * size to alloc for the mcast membership table
	 *
	 *   This num_mcast_table_elems configuration parameter tells the
	 *   target how many peer elements it needs to provide storage for in
	 *   its multicast group membership table.
	 *   These multicast group membership table elements are shared by the
	 *   multicast groups stored within the table.
	 */
	__le32 num_mcast_table_elems;

	/*
	 * whether/how to do multicast->unicast conversion
	 *
	 *   This configuration parameter specifies whether the target should
	 *   perform multicast --> unicast conversion on transmit, and if so,
	 *   what to do if it finds no entries in its multicast group
	 *   membership table for the multicast IP address in the tx frame.
	 *   Configuration value:
	 *   0 -> Do not perform multicast to unicast conversion.
	 *   1 -> Convert multicast frames to unicast, if the IP multicast
	 *        address from the tx frame is found in the multicast group
	 *        membership table.  If the IP multicast address is not found,
	 *        drop the frame.
	 *   2 -> Convert multicast frames to unicast, if the IP multicast
	 *        address from the tx frame is found in the multicast group
	 *        membership table.  If the IP multicast address is not found,
	 *        transmit the frame as multicast.
	 */
	__le32 mcast2ucast_mode;

	/*
	 * how much memory to allocate for a tx PPDU dbg log
	 *
	 *   This parameter controls how much memory the target will allocate
	 *   to store a log of tx PPDU meta-information (how large the PPDU
	 *   was, when it was sent, whether it was successful, etc.)
	 */
	__le32 tx_dbg_log_size;

	/* how many AST entries to be allocated for WDS */
	__le32 num_wds_entries;

	/*
	 * MAC DMA burst size, e.g., For target PCI limit can be
	 * 0 -default, 1 256B
	 */
	__le32 dma_burst_size;

	/*
	 * Fixed delimiters to be inserted after every MPDU to
	 * account for interface latency to avoid underrun.
	 */
	__le32 mac_aggr_delim;

	/*
	 *   determine whether target is responsible for detecting duplicate
	 *   non-aggregate MPDU and timing out stale fragments.
	 *
	 *   A-MPDU reordering is always performed on the target.
	 *
	 *   0: target responsible for frag timeout and dup checking
	 *   1: host responsible for frag timeout and dup checking
	 */
	__le32 rx_skip_defrag_timeout_dup_detection_check;

	/*
	 * Configuration for VoW :
	 * No of Video Nodes to be supported
	 * and Max no of descriptors for each Video link (node).
	 */
	__le32 vow_config;

	/* Number of msdu descriptors target should use */
	__le32 num_msdu_desc;

	/*
	 * Max. number of Tx fragments per MSDU
	 *  This parameter controls the max number of Tx fragments per MSDU.
	 *  This is sent by the target as part of the WMI_SERVICE_READY event
	 *  and is overriden by the OS shim as required.
	 */
	__le32 max_frag_entries;
} __packed;


#define NUM_UNITS_IS_NUM_VDEVS   0x1
#define NUM_UNITS_IS_NUM_PEERS   0x2

/* strucutre describing host memory chunk. */
struct host_memory_chunk {
	/* id of the request that is passed up in service ready */
	__le32 req_id;
	/* the physical address the memory chunk */
	__le32 ptr;
	/* size of the chunk */
	__le32 size;
} __packed;

struct wmi_init_cmd {
	struct wmi_resource_config resource_config;
	__le32 num_host_mem_chunks;

	/*
	 * variable number of host memory chunks.
	 * This should be the last element in the structure
	 */
	struct host_memory_chunk host_mem_chunks[1];
} __packed;

/* _10x stucture is from 10.X FW API */
struct wmi_init_cmd_10x {
	struct wmi_resource_config_10x resource_config;
	__le32 num_host_mem_chunks;

	/*
	 * variable number of host memory chunks.
	 * This should be the last element in the structure
	 */
	struct host_memory_chunk host_mem_chunks[1];
} __packed;

/* TLV for channel list */
struct wmi_chan_list {
	__le32 tag; /* WMI_CHAN_LIST_TAG */
	__le32 num_chan;
	__le32 channel_list[0];
} __packed;

struct wmi_bssid_list {
	__le32 tag; /* WMI_BSSID_LIST_TAG */
	__le32 num_bssid;
	struct wmi_mac_addr bssid_list[0];
} __packed;

struct wmi_ie_data {
	__le32 tag; /* WMI_IE_TAG */
	__le32 ie_len;
	u8 ie_data[0];
} __packed;

struct wmi_ssid {
	__le32 ssid_len;
	u8 ssid[32];
} __packed;

struct wmi_ssid_list {
	__le32 tag; /* WMI_SSID_LIST_TAG */
	__le32 num_ssids;
	struct wmi_ssid ssids[0];
} __packed;

/* prefix used by scan requestor ids on the host */
#define WMI_HOST_SCAN_REQUESTOR_ID_PREFIX 0xA000

/* prefix used by scan request ids generated on the host */
/* host cycles through the lower 12 bits to generate ids */
#define WMI_HOST_SCAN_REQ_ID_PREFIX 0xA000

#define WLAN_SCAN_PARAMS_MAX_SSID    16
#define WLAN_SCAN_PARAMS_MAX_BSSID   4
#define WLAN_SCAN_PARAMS_MAX_IE_LEN  256

/* Scan priority numbers must be sequential, starting with 0 */
enum wmi_scan_priority {
	WMI_SCAN_PRIORITY_VERY_LOW = 0,
	WMI_SCAN_PRIORITY_LOW,
	WMI_SCAN_PRIORITY_MEDIUM,
	WMI_SCAN_PRIORITY_HIGH,
	WMI_SCAN_PRIORITY_VERY_HIGH,
	WMI_SCAN_PRIORITY_COUNT   /* number of priorities supported */
};

struct wmi_start_scan_cmd {
	/* Scan ID */
	__le32 scan_id;
	/* Scan requestor ID */
	__le32 scan_req_id;
	/* VDEV id(interface) that is requesting scan */
	__le32 vdev_id;
	/* Scan Priority, input to scan scheduler */
	__le32 scan_priority;
	/* Scan events subscription */
	__le32 notify_scan_events;
	/* dwell time in msec on active channels */
	__le32 dwell_time_active;
	/* dwell time in msec on passive channels */
	__le32 dwell_time_passive;
	/*
	 * min time in msec on the BSS channel,only valid if atleast one
	 * VDEV is active
	 */
	__le32 min_rest_time;
	/*
	 * max rest time in msec on the BSS channel,only valid if at least
	 * one VDEV is active
	 */
	/*
	 * the scanner will rest on the bss channel at least min_rest_time
	 * after min_rest_time the scanner will start checking for tx/rx
	 * activity on all VDEVs. if there is no activity the scanner will
	 * switch to off channel. if there is activity the scanner will let
	 * the radio on the bss channel until max_rest_time expires.at
	 * max_rest_time scanner will switch to off channel irrespective of
	 * activity. activity is determined by the idle_time parameter.
	 */
	__le32 max_rest_time;
	/*
	 * time before sending next set of probe requests.
	 * The scanner keeps repeating probe requests transmission with
	 * period specified by repeat_probe_time.
	 * The number of probe requests specified depends on the ssid_list
	 * and bssid_list
	 */
	__le32 repeat_probe_time;
	/* time in msec between 2 consequetive probe requests with in a set. */
	__le32 probe_spacing_time;
	/*
	 * data inactivity time in msec on bss channel that will be used by
	 * scanner for measuring the inactivity.
	 */
	__le32 idle_time;
	/* maximum time in msec allowed for scan  */
	__le32 max_scan_time;
	/*
	 * delay in msec before sending first probe request after switching
	 * to a channel
	 */
	__le32 probe_delay;
	/* Scan control flags */
	__le32 scan_ctrl_flags;

	/* Burst duration time in msecs */
	__le32 burst_duration;
	/*
	 * TLV (tag length value )  paramerters follow the scan_cmd structure.
	 * TLV can contain channel list, bssid list, ssid list and
	 * ie. the TLV tags are defined above;
	 */
} __packed;

/* This is the definition from 10.X firmware branch */
struct wmi_start_scan_cmd_10x {
	/* Scan ID */
	__le32 scan_id;

	/* Scan requestor ID */
	__le32 scan_req_id;

	/* VDEV id(interface) that is requesting scan */
	__le32 vdev_id;

	/* Scan Priority, input to scan scheduler */
	__le32 scan_priority;

	/* Scan events subscription */
	__le32 notify_scan_events;

	/* dwell time in msec on active channels */
	__le32 dwell_time_active;

	/* dwell time in msec on passive channels */
	__le32 dwell_time_passive;

	/*
	 * min time in msec on the BSS channel,only valid if atleast one
	 * VDEV is active
	 */
	__le32 min_rest_time;

	/*
	 * max rest time in msec on the BSS channel,only valid if at least
	 * one VDEV is active
	 */
	/*
	 * the scanner will rest on the bss channel at least min_rest_time
	 * after min_rest_time the scanner will start checking for tx/rx
	 * activity on all VDEVs. if there is no activity the scanner will
	 * switch to off channel. if there is activity the scanner will let
	 * the radio on the bss channel until max_rest_time expires.at
	 * max_rest_time scanner will switch to off channel irrespective of
	 * activity. activity is determined by the idle_time parameter.
	 */
	__le32 max_rest_time;

	/*
	 * time before sending next set of probe requests.
	 * The scanner keeps repeating probe requests transmission with
	 * period specified by repeat_probe_time.
	 * The number of probe requests specified depends on the ssid_list
	 * and bssid_list
	 */
	__le32 repeat_probe_time;

	/* time in msec between 2 consequetive probe requests with in a set. */
	__le32 probe_spacing_time;

	/*
	 * data inactivity time in msec on bss channel that will be used by
	 * scanner for measuring the inactivity.
	 */
	__le32 idle_time;

	/* maximum time in msec allowed for scan  */
	__le32 max_scan_time;

	/*
	 * delay in msec before sending first probe request after switching
	 * to a channel
	 */
	__le32 probe_delay;

	/* Scan control flags */
	__le32 scan_ctrl_flags;

	/*
	 * TLV (tag length value )  paramerters follow the scan_cmd structure.
	 * TLV can contain channel list, bssid list, ssid list and
	 * ie. the TLV tags are defined above;
	 */
} __packed;


struct wmi_ssid_arg {
	int len;
	const u8 *ssid;
};

struct wmi_bssid_arg {
	const u8 *bssid;
};

struct wmi_start_scan_arg {
	u32 scan_id;
	u32 scan_req_id;
	u32 vdev_id;
	u32 scan_priority;
	u32 notify_scan_events;
	u32 dwell_time_active;
	u32 dwell_time_passive;
	u32 min_rest_time;
	u32 max_rest_time;
	u32 repeat_probe_time;
	u32 probe_spacing_time;
	u32 idle_time;
	u32 max_scan_time;
	u32 probe_delay;
	u32 scan_ctrl_flags;

	u32 ie_len;
	u32 n_channels;
	u32 n_ssids;
	u32 n_bssids;

	u8 ie[WLAN_SCAN_PARAMS_MAX_IE_LEN];
	u32 channels[64];
	struct wmi_ssid_arg ssids[WLAN_SCAN_PARAMS_MAX_SSID];
	struct wmi_bssid_arg bssids[WLAN_SCAN_PARAMS_MAX_BSSID];
};

/* scan control flags */

/* passively scan all channels including active channels */
#define WMI_SCAN_FLAG_PASSIVE        0x1
/* add wild card ssid probe request even though ssid_list is specified. */
#define WMI_SCAN_ADD_BCAST_PROBE_REQ 0x2
/* add cck rates to rates/xrate ie for the generated probe request */
#define WMI_SCAN_ADD_CCK_RATES 0x4
/* add ofdm rates to rates/xrate ie for the generated probe request */
#define WMI_SCAN_ADD_OFDM_RATES 0x8
/* To enable indication of Chan load and Noise floor to host */
#define WMI_SCAN_CHAN_STAT_EVENT 0x10
/* Filter Probe request frames  */
#define WMI_SCAN_FILTER_PROBE_REQ 0x20
/* When set, DFS channels will not be scanned */
#define WMI_SCAN_BYPASS_DFS_CHN 0x40
/* Different FW scan engine may choose to bail out on errors.
 * Allow the driver to have influence over that. */
#define WMI_SCAN_CONTINUE_ON_ERROR 0x80

/* WMI_SCAN_CLASS_MASK must be the same value as IEEE80211_SCAN_CLASS_MASK */
#define WMI_SCAN_CLASS_MASK 0xFF000000


enum wmi_stop_scan_type {
	WMI_SCAN_STOP_ONE	= 0x00000000, /* stop by scan_id */
	WMI_SCAN_STOP_VDEV_ALL	= 0x01000000, /* stop by vdev_id */
	WMI_SCAN_STOP_ALL	= 0x04000000, /* stop all scans */
};

struct wmi_stop_scan_cmd {
	__le32 scan_req_id;
	__le32 scan_id;
	__le32 req_type;
	__le32 vdev_id;
} __packed;

struct wmi_stop_scan_arg {
	u32 req_id;
	enum wmi_stop_scan_type req_type;
	union {
		u32 scan_id;
		u32 vdev_id;
	} u;
};

struct wmi_scan_chan_list_cmd {
	__le32 num_scan_chans;
	struct wmi_channel chan_info[0];
} __packed;

struct wmi_scan_chan_list_arg {
	u32 n_channels;
	struct wmi_channel_arg *channels;
};

enum wmi_bss_filter {
	WMI_BSS_FILTER_NONE = 0,        /* no beacons forwarded */
	WMI_BSS_FILTER_ALL,             /* all beacons forwarded */
	WMI_BSS_FILTER_PROFILE,         /* only beacons matching profile */
	WMI_BSS_FILTER_ALL_BUT_PROFILE, /* all but beacons matching profile */
	WMI_BSS_FILTER_CURRENT_BSS,     /* only beacons matching current BSS */
	WMI_BSS_FILTER_ALL_BUT_BSS,     /* all but beacons matching BSS */
	WMI_BSS_FILTER_PROBED_SSID,     /* beacons matching probed ssid */
	WMI_BSS_FILTER_LAST_BSS,        /* marker only */
};

enum wmi_scan_event_type {
	WMI_SCAN_EVENT_STARTED         = 0x1,
	WMI_SCAN_EVENT_COMPLETED       = 0x2,
	WMI_SCAN_EVENT_BSS_CHANNEL     = 0x4,
	WMI_SCAN_EVENT_FOREIGN_CHANNEL = 0x8,
	WMI_SCAN_EVENT_DEQUEUED        = 0x10,
	WMI_SCAN_EVENT_PREEMPTED       = 0x20, /* possibly by high-prio scan */
	WMI_SCAN_EVENT_START_FAILED    = 0x40,
	WMI_SCAN_EVENT_RESTARTED       = 0x80,
	WMI_SCAN_EVENT_MAX             = 0x8000
};

enum wmi_scan_completion_reason {
	WMI_SCAN_REASON_COMPLETED,
	WMI_SCAN_REASON_CANCELLED,
	WMI_SCAN_REASON_PREEMPTED,
	WMI_SCAN_REASON_TIMEDOUT,
	WMI_SCAN_REASON_MAX,
};

struct wmi_scan_event {
	__le32 event_type; /* %WMI_SCAN_EVENT_ */
	__le32 reason; /* %WMI_SCAN_REASON_ */
	__le32 channel_freq; /* only valid for WMI_SCAN_EVENT_FOREIGN_CHANNEL */
	__le32 scan_req_id;
	__le32 scan_id;
	__le32 vdev_id;
} __packed;

/*
 * This defines how much headroom is kept in the
 * receive frame between the descriptor and the
 * payload, in order for the WMI PHY error and
 * management handler to insert header contents.
 *
 * This is in bytes.
 */
#define WMI_MGMT_RX_HDR_HEADROOM    52

/*
 * This event will be used for sending scan results
 * as well as rx mgmt frames to the host. The rx buffer
 * will be sent as part of this WMI event. It would be a
 * good idea to pass all the fields in the RX status
 * descriptor up to the host.
 */
struct wmi_mgmt_rx_hdr_v1 {
	__le32 channel;
	__le32 snr;
	__le32 rate;
	__le32 phy_mode;
	__le32 buf_len;
	__le32 status; /* %WMI_RX_STATUS_ */
} __packed;

struct wmi_mgmt_rx_hdr_v2 {
	struct wmi_mgmt_rx_hdr_v1 v1;
	__le32 rssi_ctl[4];
} __packed;

struct wmi_mgmt_rx_event_v1 {
	struct wmi_mgmt_rx_hdr_v1 hdr;
	u8 buf[0];
} __packed;

struct wmi_mgmt_rx_event_v2 {
	struct wmi_mgmt_rx_hdr_v2 hdr;
	u8 buf[0];
} __packed;

#define WMI_RX_STATUS_OK			0x00
#define WMI_RX_STATUS_ERR_CRC			0x01
#define WMI_RX_STATUS_ERR_DECRYPT		0x08
#define WMI_RX_STATUS_ERR_MIC			0x10
#define WMI_RX_STATUS_ERR_KEY_CACHE_MISS	0x20

#define PHY_ERROR_SPECTRAL_SCAN		0x26
#define PHY_ERROR_FALSE_RADAR_EXT		0x24
#define PHY_ERROR_RADAR				0x05

struct wmi_single_phyerr_rx_hdr {
	/* TSF timestamp */
	__le32 tsf_timestamp;

	/*
	 * Current freq1, freq2
	 *
	 * [7:0]:    freq1[lo]
	 * [15:8] :   freq1[hi]
	 * [23:16]:   freq2[lo]
	 * [31:24]:   freq2[hi]
	 */
	__le16 freq1;
	__le16 freq2;

	/*
	 * Combined RSSI over all chains and channel width for this PHY error
	 *
	 * [7:0]: RSSI combined
	 * [15:8]: Channel width (MHz)
	 * [23:16]: PHY error code
	 * [24:16]: reserved (future use)
	 */
	u8 rssi_combined;
	u8 chan_width_mhz;
	u8 phy_err_code;
	u8 rsvd0;

	/*
	 * RSSI on chain 0 through 3
	 *
	 * This is formatted the same as the PPDU_START RX descriptor
	 * field:
	 *
	 * [7:0]:   pri20
	 * [15:8]:  sec20
	 * [23:16]: sec40
	 * [31:24]: sec80
	 */

	__le32 rssi_chain0;
	__le32 rssi_chain1;
	__le32 rssi_chain2;
	__le32 rssi_chain3;

	/*
	 * Last calibrated NF value for chain 0 through 3
	 *
	 * nf_list_1:
	 *
	 * + [15:0] - chain 0
	 * + [31:16] - chain 1
	 *
	 * nf_list_2:
	 *
	 * + [15:0] - chain 2
	 * + [31:16] - chain 3
	 */
	__le32 nf_list_1;
	__le32 nf_list_2;


	/* Length of the frame */
	__le32 buf_len;
} __packed;

struct wmi_single_phyerr_rx_event {
	/* Phy error event header */
	struct wmi_single_phyerr_rx_hdr hdr;
	/* frame buffer */
	u8 bufp[0];
} __packed;

struct wmi_comb_phyerr_rx_hdr {
	/* Phy error phy error count */
	__le32 num_phyerr_events;
	__le32 tsf_l32;
	__le32 tsf_u32;
} __packed;

struct wmi_comb_phyerr_rx_event {
	/* Phy error phy error count */
	struct wmi_comb_phyerr_rx_hdr hdr;
	/*
	 * frame buffer - contains multiple payloads in the order:
	 *                    header - payload, header - payload...
	 *  (The header is of type: wmi_single_phyerr_rx_hdr)
	 */
	u8 bufp[0];
} __packed;

#define PHYERR_TLV_SIG				0xBB
#define PHYERR_TLV_TAG_SEARCH_FFT_REPORT	0xFB
#define PHYERR_TLV_TAG_RADAR_PULSE_SUMMARY	0xF8

struct phyerr_radar_report {
	__le32 reg0; /* RADAR_REPORT_REG0_* */
	__le32 reg1; /* REDAR_REPORT_REG1_* */
} __packed;

#define RADAR_REPORT_REG0_PULSE_IS_CHIRP_MASK		0x80000000
#define RADAR_REPORT_REG0_PULSE_IS_CHIRP_LSB		31

#define RADAR_REPORT_REG0_PULSE_IS_MAX_WIDTH_MASK	0x40000000
#define RADAR_REPORT_REG0_PULSE_IS_MAX_WIDTH_LSB	30

#define RADAR_REPORT_REG0_AGC_TOTAL_GAIN_MASK		0x3FF00000
#define RADAR_REPORT_REG0_AGC_TOTAL_GAIN_LSB		20

#define RADAR_REPORT_REG0_PULSE_DELTA_DIFF_MASK		0x000F0000
#define RADAR_REPORT_REG0_PULSE_DELTA_DIFF_LSB		16

#define RADAR_REPORT_REG0_PULSE_DELTA_PEAK_MASK		0x0000FC00
#define RADAR_REPORT_REG0_PULSE_DELTA_PEAK_LSB		10

#define RADAR_REPORT_REG0_PULSE_SIDX_MASK		0x000003FF
#define RADAR_REPORT_REG0_PULSE_SIDX_LSB		0

#define RADAR_REPORT_REG1_PULSE_SRCH_FFT_VALID_MASK	0x80000000
#define RADAR_REPORT_REG1_PULSE_SRCH_FFT_VALID_LSB	31

#define RADAR_REPORT_REG1_PULSE_AGC_MB_GAIN_MASK	0x7F000000
#define RADAR_REPORT_REG1_PULSE_AGC_MB_GAIN_LSB		24

#define RADAR_REPORT_REG1_PULSE_SUBCHAN_MASK_MASK	0x00FF0000
#define RADAR_REPORT_REG1_PULSE_SUBCHAN_MASK_LSB	16

#define RADAR_REPORT_REG1_PULSE_TSF_OFFSET_MASK		0x0000FF00
#define RADAR_REPORT_REG1_PULSE_TSF_OFFSET_LSB		8

#define RADAR_REPORT_REG1_PULSE_DUR_MASK		0x000000FF
#define RADAR_REPORT_REG1_PULSE_DUR_LSB			0

struct phyerr_fft_report {
	__le32 reg0; /* SEARCH_FFT_REPORT_REG0_ * */
	__le32 reg1; /* SEARCH_FFT_REPORT_REG1_ * */
} __packed;

#define SEARCH_FFT_REPORT_REG0_TOTAL_GAIN_DB_MASK	0xFF800000
#define SEARCH_FFT_REPORT_REG0_TOTAL_GAIN_DB_LSB	23

#define SEARCH_FFT_REPORT_REG0_BASE_PWR_DB_MASK		0x007FC000
#define SEARCH_FFT_REPORT_REG0_BASE_PWR_DB_LSB		14

#define SEARCH_FFT_REPORT_REG0_FFT_CHN_IDX_MASK		0x00003000
#define SEARCH_FFT_REPORT_REG0_FFT_CHN_IDX_LSB		12

#define SEARCH_FFT_REPORT_REG0_PEAK_SIDX_MASK		0x00000FFF
#define SEARCH_FFT_REPORT_REG0_PEAK_SIDX_LSB		0

#define SEARCH_FFT_REPORT_REG1_RELPWR_DB_MASK		0xFC000000
#define SEARCH_FFT_REPORT_REG1_RELPWR_DB_LSB		26

#define SEARCH_FFT_REPORT_REG1_AVGPWR_DB_MASK		0x03FC0000
#define SEARCH_FFT_REPORT_REG1_AVGPWR_DB_LSB		18

#define SEARCH_FFT_REPORT_REG1_PEAK_MAG_MASK		0x0003FF00
#define SEARCH_FFT_REPORT_REG1_PEAK_MAG_LSB		8

#define SEARCH_FFT_REPORT_REG1_NUM_STR_BINS_IB_MASK	0x000000FF
#define SEARCH_FFT_REPORT_REG1_NUM_STR_BINS_IB_LSB	0


struct phyerr_tlv {
	__le16 len;
	u8 tag;
	u8 sig;
} __packed;

#define DFS_RSSI_POSSIBLY_FALSE			50
#define DFS_PEAK_MAG_THOLD_POSSIBLY_FALSE	40

struct wmi_mgmt_tx_hdr {
	__le32 vdev_id;
	struct wmi_mac_addr peer_macaddr;
	__le32 tx_rate;
	__le32 tx_power;
	__le32 buf_len;
} __packed;

struct wmi_mgmt_tx_cmd {
	struct wmi_mgmt_tx_hdr hdr;
	u8 buf[0];
} __packed;

struct wmi_echo_event {
	__le32 value;
} __packed;

struct wmi_echo_cmd {
	__le32 value;
} __packed;


struct wmi_pdev_set_regdomain_cmd {
	__le32 reg_domain;
	__le32 reg_domain_2G;
	__le32 reg_domain_5G;
	__le32 conformance_test_limit_2G;
	__le32 conformance_test_limit_5G;
} __packed;

/* Command to set/unset chip in quiet mode */
struct wmi_pdev_set_quiet_cmd {
	/* period in TUs */
	__le32 period;

	/* duration in TUs */
	__le32 duration;

	/* offset in TUs */
	__le32 next_start;

	/* enable/disable */
	__le32 enabled;
} __packed;


/*
 * 802.11g protection mode.
 */
enum ath10k_protmode {
	ATH10K_PROT_NONE     = 0,    /* no protection */
	ATH10K_PROT_CTSONLY  = 1,    /* CTS to self */
	ATH10K_PROT_RTSCTS   = 2,    /* RTS-CTS */
};

enum wmi_beacon_gen_mode {
	WMI_BEACON_STAGGERED_MODE = 0,
	WMI_BEACON_BURST_MODE = 1
};

enum wmi_csa_event_ies_present_flag {
	WMI_CSA_IE_PRESENT = 0x00000001,
	WMI_XCSA_IE_PRESENT = 0x00000002,
	WMI_WBW_IE_PRESENT = 0x00000004,
	WMI_CSWARP_IE_PRESENT = 0x00000008,
};

/* wmi CSA receive event from beacon frame */
struct wmi_csa_event {
	__le32 i_fc_dur;
	/* Bit 0-15: FC */
	/* Bit 16-31: DUR */
	struct wmi_mac_addr i_addr1;
	struct wmi_mac_addr i_addr2;
	__le32 csa_ie[2];
	__le32 xcsa_ie[2];
	__le32 wb_ie[2];
	__le32 cswarp_ie;
	__le32 ies_present_flag; /* wmi_csa_event_ies_present_flag */
} __packed;

/* the definition of different PDEV parameters */
#define PDEV_DEFAULT_STATS_UPDATE_PERIOD    500
#define VDEV_DEFAULT_STATS_UPDATE_PERIOD    500
#define PEER_DEFAULT_STATS_UPDATE_PERIOD    500

struct wmi_pdev_param_map {
	u32 tx_chain_mask;
	u32 rx_chain_mask;
	u32 txpower_limit2g;
	u32 txpower_limit5g;
	u32 txpower_scale;
	u32 beacon_gen_mode;
	u32 beacon_tx_mode;
	u32 resmgr_offchan_mode;
	u32 protection_mode;
	u32 dynamic_bw;
	u32 non_agg_sw_retry_th;
	u32 agg_sw_retry_th;
	u32 sta_kickout_th;
	u32 ac_aggrsize_scaling;
	u32 ltr_enable;
	u32 ltr_ac_latency_be;
	u32 ltr_ac_latency_bk;
	u32 ltr_ac_latency_vi;
	u32 ltr_ac_latency_vo;
	u32 ltr_ac_latency_timeout;
	u32 ltr_sleep_override;
	u32 ltr_rx_override;
	u32 ltr_tx_activity_timeout;
	u32 l1ss_enable;
	u32 dsleep_enable;
	u32 pcielp_txbuf_flush;
	u32 pcielp_txbuf_watermark;
	u32 pcielp_txbuf_tmo_en;
	u32 pcielp_txbuf_tmo_value;
	u32 pdev_stats_update_period;
	u32 vdev_stats_update_period;
	u32 peer_stats_update_period;
	u32 bcnflt_stats_update_period;
	u32 pmf_qos;
	u32 arp_ac_override;
	u32 arpdhcp_ac_override;
	u32 dcs;
	u32 ani_enable;
	u32 ani_poll_period;
	u32 ani_listen_period;
	u32 ani_ofdm_level;
	u32 ani_cck_level;
	u32 dyntxchain;
	u32 proxy_sta;
	u32 idle_ps_config;
	u32 power_gating_sleep;
	u32 fast_channel_reset;
	u32 burst_dur;
	u32 burst_enable;
};

#define WMI_PDEV_PARAM_UNSUPPORTED 0

enum wmi_pdev_param {
	/* TX chian mask */
	WMI_PDEV_PARAM_TX_CHAIN_MASK = 0x1,
	/* RX chian mask */
	WMI_PDEV_PARAM_RX_CHAIN_MASK,
	/* TX power limit for 2G Radio */
	WMI_PDEV_PARAM_TXPOWER_LIMIT2G,
	/* TX power limit for 5G Radio */
	WMI_PDEV_PARAM_TXPOWER_LIMIT5G,
	/* TX power scale */
	WMI_PDEV_PARAM_TXPOWER_SCALE,
	/* Beacon generation mode . 0: host, 1: target   */
	WMI_PDEV_PARAM_BEACON_GEN_MODE,
	/* Beacon generation mode . 0: staggered 1: bursted   */
	WMI_PDEV_PARAM_BEACON_TX_MODE,
	/*
	 * Resource manager off chan mode .
	 * 0: turn off off chan mode. 1: turn on offchan mode
	 */
	WMI_PDEV_PARAM_RESMGR_OFFCHAN_MODE,
	/*
	 * Protection mode:
	 * 0: no protection 1:use CTS-to-self 2: use RTS/CTS
	 */
	WMI_PDEV_PARAM_PROTECTION_MODE,
	/*
	 * Dynamic bandwidth - 0: disable, 1: enable
	 *
	 * When enabled HW rate control tries different bandwidths when
	 * retransmitting frames.
	 */
	WMI_PDEV_PARAM_DYNAMIC_BW,
	/* Non aggregrate/ 11g sw retry threshold.0-disable */
	WMI_PDEV_PARAM_NON_AGG_SW_RETRY_TH,
	/* aggregrate sw retry threshold. 0-disable*/
	WMI_PDEV_PARAM_AGG_SW_RETRY_TH,
	/* Station kickout threshold (non of consecutive failures).0-disable */
	WMI_PDEV_PARAM_STA_KICKOUT_TH,
	/* Aggerate size scaling configuration per AC */
	WMI_PDEV_PARAM_AC_AGGRSIZE_SCALING,
	/* LTR enable */
	WMI_PDEV_PARAM_LTR_ENABLE,
	/* LTR latency for BE, in us */
	WMI_PDEV_PARAM_LTR_AC_LATENCY_BE,
	/* LTR latency for BK, in us */
	WMI_PDEV_PARAM_LTR_AC_LATENCY_BK,
	/* LTR latency for VI, in us */
	WMI_PDEV_PARAM_LTR_AC_LATENCY_VI,
	/* LTR latency for VO, in us  */
	WMI_PDEV_PARAM_LTR_AC_LATENCY_VO,
	/* LTR AC latency timeout, in ms */
	WMI_PDEV_PARAM_LTR_AC_LATENCY_TIMEOUT,
	/* LTR platform latency override, in us */
	WMI_PDEV_PARAM_LTR_SLEEP_OVERRIDE,
	/* LTR-RX override, in us */
	WMI_PDEV_PARAM_LTR_RX_OVERRIDE,
	/* Tx activity timeout for LTR, in us */
	WMI_PDEV_PARAM_LTR_TX_ACTIVITY_TIMEOUT,
	/* L1SS state machine enable */
	WMI_PDEV_PARAM_L1SS_ENABLE,
	/* Deep sleep state machine enable */
	WMI_PDEV_PARAM_DSLEEP_ENABLE,
	/* RX buffering flush enable */
	WMI_PDEV_PARAM_PCIELP_TXBUF_FLUSH,
	/* RX buffering matermark */
	WMI_PDEV_PARAM_PCIELP_TXBUF_WATERMARK,
	/* RX buffering timeout enable */
	WMI_PDEV_PARAM_PCIELP_TXBUF_TMO_EN,
	/* RX buffering timeout value */
	WMI_PDEV_PARAM_PCIELP_TXBUF_TMO_VALUE,
	/* pdev level stats update period in ms */
	WMI_PDEV_PARAM_PDEV_STATS_UPDATE_PERIOD,
	/* vdev level stats update period in ms */
	WMI_PDEV_PARAM_VDEV_STATS_UPDATE_PERIOD,
	/* peer level stats update period in ms */
	WMI_PDEV_PARAM_PEER_STATS_UPDATE_PERIOD,
	/* beacon filter status update period */
	WMI_PDEV_PARAM_BCNFLT_STATS_UPDATE_PERIOD,
	/* QOS Mgmt frame protection MFP/PMF 0: disable, 1: enable */
	WMI_PDEV_PARAM_PMF_QOS,
	/* Access category on which ARP frames are sent */
	WMI_PDEV_PARAM_ARP_AC_OVERRIDE,
	/* DCS configuration */
	WMI_PDEV_PARAM_DCS,
	/* Enable/Disable ANI on target */
	WMI_PDEV_PARAM_ANI_ENABLE,
	/* configure the ANI polling period */
	WMI_PDEV_PARAM_ANI_POLL_PERIOD,
	/* configure the ANI listening period */
	WMI_PDEV_PARAM_ANI_LISTEN_PERIOD,
	/* configure OFDM immunity level */
	WMI_PDEV_PARAM_ANI_OFDM_LEVEL,
	/* configure CCK immunity level */
	WMI_PDEV_PARAM_ANI_CCK_LEVEL,
	/* Enable/Disable CDD for 1x1 STAs in rate control module */
	WMI_PDEV_PARAM_DYNTXCHAIN,
	/* Enable/Disable proxy STA */
	WMI_PDEV_PARAM_PROXY_STA,
	/* Enable/Disable low power state when all VDEVs are inactive/idle. */
	WMI_PDEV_PARAM_IDLE_PS_CONFIG,
	/* Enable/Disable power gating sleep */
	WMI_PDEV_PARAM_POWER_GATING_SLEEP,
};

enum wmi_10x_pdev_param {
	/* TX chian mask */
	WMI_10X_PDEV_PARAM_TX_CHAIN_MASK = 0x1,
	/* RX chian mask */
	WMI_10X_PDEV_PARAM_RX_CHAIN_MASK,
	/* TX power limit for 2G Radio */
	WMI_10X_PDEV_PARAM_TXPOWER_LIMIT2G,
	/* TX power limit for 5G Radio */
	WMI_10X_PDEV_PARAM_TXPOWER_LIMIT5G,
	/* TX power scale */
	WMI_10X_PDEV_PARAM_TXPOWER_SCALE,
	/* Beacon generation mode . 0: host, 1: target   */
	WMI_10X_PDEV_PARAM_BEACON_GEN_MODE,
	/* Beacon generation mode . 0: staggered 1: bursted   */
	WMI_10X_PDEV_PARAM_BEACON_TX_MODE,
	/*
	 * Resource manager off chan mode .
	 * 0: turn off off chan mode. 1: turn on offchan mode
	 */
	WMI_10X_PDEV_PARAM_RESMGR_OFFCHAN_MODE,
	/*
	 * Protection mode:
	 * 0: no protection 1:use CTS-to-self 2: use RTS/CTS
	 */
	WMI_10X_PDEV_PARAM_PROTECTION_MODE,
	/* Dynamic bandwidth 0: disable 1: enable */
	WMI_10X_PDEV_PARAM_DYNAMIC_BW,
	/* Non aggregrate/ 11g sw retry threshold.0-disable */
	WMI_10X_PDEV_PARAM_NON_AGG_SW_RETRY_TH,
	/* aggregrate sw retry threshold. 0-disable*/
	WMI_10X_PDEV_PARAM_AGG_SW_RETRY_TH,
	/* Station kickout threshold (non of consecutive failures).0-disable */
	WMI_10X_PDEV_PARAM_STA_KICKOUT_TH,
	/* Aggerate size scaling configuration per AC */
	WMI_10X_PDEV_PARAM_AC_AGGRSIZE_SCALING,
	/* LTR enable */
	WMI_10X_PDEV_PARAM_LTR_ENABLE,
	/* LTR latency for BE, in us */
	WMI_10X_PDEV_PARAM_LTR_AC_LATENCY_BE,
	/* LTR latency for BK, in us */
	WMI_10X_PDEV_PARAM_LTR_AC_LATENCY_BK,
	/* LTR latency for VI, in us */
	WMI_10X_PDEV_PARAM_LTR_AC_LATENCY_VI,
	/* LTR latency for VO, in us  */
	WMI_10X_PDEV_PARAM_LTR_AC_LATENCY_VO,
	/* LTR AC latency timeout, in ms */
	WMI_10X_PDEV_PARAM_LTR_AC_LATENCY_TIMEOUT,
	/* LTR platform latency override, in us */
	WMI_10X_PDEV_PARAM_LTR_SLEEP_OVERRIDE,
	/* LTR-RX override, in us */
	WMI_10X_PDEV_PARAM_LTR_RX_OVERRIDE,
	/* Tx activity timeout for LTR, in us */
	WMI_10X_PDEV_PARAM_LTR_TX_ACTIVITY_TIMEOUT,
	/* L1SS state machine enable */
	WMI_10X_PDEV_PARAM_L1SS_ENABLE,
	/* Deep sleep state machine enable */
	WMI_10X_PDEV_PARAM_DSLEEP_ENABLE,
	/* pdev level stats update period in ms */
	WMI_10X_PDEV_PARAM_PDEV_STATS_UPDATE_PERIOD,
	/* vdev level stats update period in ms */
	WMI_10X_PDEV_PARAM_VDEV_STATS_UPDATE_PERIOD,
	/* peer level stats update period in ms */
	WMI_10X_PDEV_PARAM_PEER_STATS_UPDATE_PERIOD,
	/* beacon filter status update period */
	WMI_10X_PDEV_PARAM_BCNFLT_STATS_UPDATE_PERIOD,
	/* QOS Mgmt frame protection MFP/PMF 0: disable, 1: enable */
	WMI_10X_PDEV_PARAM_PMF_QOS,
	/* Access category on which ARP and DHCP frames are sent */
	WMI_10X_PDEV_PARAM_ARPDHCP_AC_OVERRIDE,
	/* DCS configuration */
	WMI_10X_PDEV_PARAM_DCS,
	/* Enable/Disable ANI on target */
	WMI_10X_PDEV_PARAM_ANI_ENABLE,
	/* configure the ANI polling period */
	WMI_10X_PDEV_PARAM_ANI_POLL_PERIOD,
	/* configure the ANI listening period */
	WMI_10X_PDEV_PARAM_ANI_LISTEN_PERIOD,
	/* configure OFDM immunity level */
	WMI_10X_PDEV_PARAM_ANI_OFDM_LEVEL,
	/* configure CCK immunity level */
	WMI_10X_PDEV_PARAM_ANI_CCK_LEVEL,
	/* Enable/Disable CDD for 1x1 STAs in rate control module */
	WMI_10X_PDEV_PARAM_DYNTXCHAIN,
	/* Enable/Disable Fast channel reset*/
	WMI_10X_PDEV_PARAM_FAST_CHANNEL_RESET,
	/* Set Bursting DUR */
	WMI_10X_PDEV_PARAM_BURST_DUR,
	/* Set Bursting Enable*/
	WMI_10X_PDEV_PARAM_BURST_ENABLE,
};

struct wmi_pdev_set_param_cmd {
	__le32 param_id;
	__le32 param_value;
} __packed;

struct wmi_pdev_get_tpc_config_cmd {
	/* parameter   */
	__le32 param;
} __packed;

#define WMI_TPC_RATE_MAX		160
#define WMI_TPC_TX_N_CHAIN		4

enum wmi_tpc_config_event_flag {
	WMI_TPC_CONFIG_EVENT_FLAG_TABLE_CDD	= 0x1,
	WMI_TPC_CONFIG_EVENT_FLAG_TABLE_STBC	= 0x2,
	WMI_TPC_CONFIG_EVENT_FLAG_TABLE_TXBF	= 0x4,
};

struct wmi_pdev_tpc_config_event {
	__le32 reg_domain;
	__le32 chan_freq;
	__le32 phy_mode;
	__le32 twice_antenna_reduction;
	__le32 twice_max_rd_power;
	s32 twice_antenna_gain;
	__le32 power_limit;
	__le32 rate_max;
	__le32 num_tx_chain;
	__le32 ctl;
	__le32 flags;
	s8 max_reg_allow_pow[WMI_TPC_TX_N_CHAIN];
	s8 max_reg_allow_pow_agcdd[WMI_TPC_TX_N_CHAIN][WMI_TPC_TX_N_CHAIN];
	s8 max_reg_allow_pow_agstbc[WMI_TPC_TX_N_CHAIN][WMI_TPC_TX_N_CHAIN];
	s8 max_reg_allow_pow_agtxbf[WMI_TPC_TX_N_CHAIN][WMI_TPC_TX_N_CHAIN];
	u8 rates_array[WMI_TPC_RATE_MAX];
} __packed;

/* Transmit power scale factor. */
enum wmi_tp_scale {
	WMI_TP_SCALE_MAX    = 0,	/* no scaling (default) */
	WMI_TP_SCALE_50     = 1,	/* 50% of max (-3 dBm) */
	WMI_TP_SCALE_25     = 2,	/* 25% of max (-6 dBm) */
	WMI_TP_SCALE_12     = 3,	/* 12% of max (-9 dBm) */
	WMI_TP_SCALE_MIN    = 4,	/* min, but still on   */
	WMI_TP_SCALE_SIZE   = 5,	/* max num of enum     */
};

struct wmi_set_channel_cmd {
	/* channel (only frequency and mode info are used) */
	struct wmi_channel chan;
} __packed;

struct wmi_pdev_chanlist_update_event {
	/* number of channels */
	__le32 num_chan;
	/* array of channels */
	struct wmi_channel channel_list[1];
} __packed;

#define WMI_MAX_DEBUG_MESG (sizeof(u32) * 32)

struct wmi_debug_mesg_event {
	/* message buffer, NULL terminated */
	char bufp[WMI_MAX_DEBUG_MESG];
} __packed;

enum {
	/* P2P device */
	VDEV_SUBTYPE_P2PDEV = 0,
	/* P2P client */
	VDEV_SUBTYPE_P2PCLI,
	/* P2P GO */
	VDEV_SUBTYPE_P2PGO,
	/* BT3.0 HS */
	VDEV_SUBTYPE_BT,
};

struct wmi_pdev_set_channel_cmd {
	/* idnore power , only use flags , mode and freq */
	struct wmi_channel chan;
} __packed;

/* Customize the DSCP (bit) to TID (0-7) mapping for QOS */
#define WMI_DSCP_MAP_MAX    (64)
struct wmi_pdev_set_dscp_tid_map_cmd {
	/* map indicating DSCP to TID conversion */
	__le32 dscp_to_tid_map[WMI_DSCP_MAP_MAX];
} __packed;

enum mcast_bcast_rate_id {
	WMI_SET_MCAST_RATE,
	WMI_SET_BCAST_RATE
};

struct mcast_bcast_rate {
	enum mcast_bcast_rate_id rate_id;
	__le32 rate;
} __packed;

struct wmi_wmm_params {
	__le32 cwmin;
	__le32 cwmax;
	__le32 aifs;
	__le32 txop;
	__le32 acm;
	__le32 no_ack;
} __packed;

struct wmi_pdev_set_wmm_params {
	struct wmi_wmm_params ac_be;
	struct wmi_wmm_params ac_bk;
	struct wmi_wmm_params ac_vi;
	struct wmi_wmm_params ac_vo;
} __packed;

struct wmi_wmm_params_arg {
	u32 cwmin;
	u32 cwmax;
	u32 aifs;
	u32 txop;
	u32 acm;
	u32 no_ack;
};

struct wmi_pdev_set_wmm_params_arg {
	struct wmi_wmm_params_arg ac_be;
	struct wmi_wmm_params_arg ac_bk;
	struct wmi_wmm_params_arg ac_vi;
	struct wmi_wmm_params_arg ac_vo;
};

struct wal_dbg_tx_stats {
	/* Num HTT cookies queued to dispatch list */
	__le32 comp_queued;

	/* Num HTT cookies dispatched */
	__le32 comp_delivered;

	/* Num MSDU queued to WAL */
	__le32 msdu_enqued;

	/* Num MPDU queue to WAL */
	__le32 mpdu_enqued;

	/* Num MSDUs dropped by WMM limit */
	__le32 wmm_drop;

	/* Num Local frames queued */
	__le32 local_enqued;

	/* Num Local frames done */
	__le32 local_freed;

	/* Num queued to HW */
	__le32 hw_queued;

	/* Num PPDU reaped from HW */
	__le32 hw_reaped;

	/* Num underruns */
	__le32 underrun;

	/* Num PPDUs cleaned up in TX abort */
	__le32 tx_abort;

	/* Num MPDUs requed by SW */
	__le32 mpdus_requed;

	/* excessive retries */
	__le32 tx_ko;

	/* data hw rate code */
	__le32 data_rc;

	/* Scheduler self triggers */
	__le32 self_triggers;

	/* frames dropped due to excessive sw retries */
	__le32 sw_retry_failure;

	/* illegal rate phy errors  */
	__le32 illgl_rate_phy_err;

	/* wal pdev continous xretry */
	__le32 pdev_cont_xretry;

	/* wal pdev continous xretry */
	__le32 pdev_tx_timeout;

	/* wal pdev resets  */
	__le32 pdev_resets;

	__le32 phy_underrun;

	/* MPDU is more than txop limit */
	__le32 txop_ovf;
} __packed;

struct wal_dbg_rx_stats {
	/* Cnts any change in ring routing mid-ppdu */
	__le32 mid_ppdu_route_change;

	/* Total number of statuses processed */
	__le32 status_rcvd;

	/* Extra frags on rings 0-3 */
	__le32 r0_frags;
	__le32 r1_frags;
	__le32 r2_frags;
	__le32 r3_frags;

	/* MSDUs / MPDUs delivered to HTT */
	__le32 htt_msdus;
	__le32 htt_mpdus;

	/* MSDUs / MPDUs delivered to local stack */
	__le32 loc_msdus;
	__le32 loc_mpdus;

	/* AMSDUs that have more MSDUs than the status ring size */
	__le32 oversize_amsdu;

	/* Number of PHY errors */
	__le32 phy_errs;

	/* Number of PHY errors drops */
	__le32 phy_err_drop;

	/* Number of mpdu errors - FCS, MIC, ENC etc. */
	__le32 mpdu_errs;
} __packed;

struct wal_dbg_peer_stats {
	/* REMOVE THIS ONCE REAL PEER STAT COUNTERS ARE ADDED */
	__le32 dummy;
} __packed;

struct wal_dbg_stats {
	struct wal_dbg_tx_stats tx;
	struct wal_dbg_rx_stats rx;
	struct wal_dbg_peer_stats peer;
} __packed;

enum wmi_stats_id {
	WMI_REQUEST_PEER_STAT	= 0x01,
	WMI_REQUEST_AP_STAT	= 0x02
};

struct wmi_request_stats_cmd {
	__le32 stats_id;

	/*
	 * Space to add parameters like
	 * peer mac addr
	 */
} __packed;

/* Suspend option */
enum {
	/* suspend */
	WMI_PDEV_SUSPEND,

	/* suspend and disable all interrupts */
	WMI_PDEV_SUSPEND_AND_DISABLE_INTR,
};

struct wmi_pdev_suspend_cmd {
	/* suspend option sent to target */
	__le32 suspend_opt;
} __packed;

struct wmi_stats_event {
	__le32 stats_id; /* %WMI_REQUEST_ */
	/*
	 * number of pdev stats event structures
	 * (wmi_pdev_stats) 0 or 1
	 */
	__le32 num_pdev_stats;
	/*
	 * number of vdev stats event structures
	 * (wmi_vdev_stats) 0 or max vdevs
	 */
	__le32 num_vdev_stats;
	/*
	 * number of peer stats event structures
	 * (wmi_peer_stats) 0 or max peers
	 */
	__le32 num_peer_stats;
	__le32 num_bcnflt_stats;
	/*
	 * followed by
	 *   num_pdev_stats * size of(struct wmi_pdev_stats)
	 *   num_vdev_stats * size of(struct wmi_vdev_stats)
	 *   num_peer_stats * size of(struct wmi_peer_stats)
	 *
	 *  By having a zero sized array, the pointer to data area
	 *  becomes available without increasing the struct size
	 */
	u8 data[0];
} __packed;

/*
 * PDEV statistics
 * TODO: add all PDEV stats here
 */
struct wmi_pdev_stats {
	__le32 chan_nf;        /* Channel noise floor */
	__le32 tx_frame_count; /* TX frame count */
	__le32 rx_frame_count; /* RX frame count */
	__le32 rx_clear_count; /* rx clear count */
	__le32 cycle_count;    /* cycle count */
	__le32 phy_err_count;  /* Phy error count */
	__le32 chan_tx_pwr;    /* channel tx power */
	struct wal_dbg_stats wal; /* WAL dbg stats */
} __packed;

/*
 * VDEV statistics
 * TODO: add all VDEV stats here
 */
struct wmi_vdev_stats {
	__le32 vdev_id;
} __packed;

/*
 * peer statistics.
 * TODO: add more stats
 */
struct wmi_peer_stats {
	struct wmi_mac_addr peer_macaddr;
	__le32 peer_rssi;
	__le32 peer_tx_rate;
} __packed;

struct wmi_vdev_create_cmd {
	__le32 vdev_id;
	__le32 vdev_type;
	__le32 vdev_subtype;
	struct wmi_mac_addr vdev_macaddr;
} __packed;

enum wmi_vdev_type {
	WMI_VDEV_TYPE_AP      = 1,
	WMI_VDEV_TYPE_STA     = 2,
	WMI_VDEV_TYPE_IBSS    = 3,
	WMI_VDEV_TYPE_MONITOR = 4,
};

enum wmi_vdev_subtype {
	WMI_VDEV_SUBTYPE_NONE       = 0,
	WMI_VDEV_SUBTYPE_P2P_DEVICE = 1,
	WMI_VDEV_SUBTYPE_P2P_CLIENT = 2,
	WMI_VDEV_SUBTYPE_P2P_GO     = 3,
};

/* values for vdev_subtype */

/* values for vdev_start_request flags */
/*
 * Indicates that AP VDEV uses hidden ssid. only valid for
 *  AP/GO */
#define WMI_VDEV_START_HIDDEN_SSID  (1<<0)
/*
 * Indicates if robust management frame/management frame
 *  protection is enabled. For GO/AP vdevs, it indicates that
 *  it may support station/client associations with RMF enabled.
 *  For STA/client vdevs, it indicates that sta will
 *  associate with AP with RMF enabled. */
#define WMI_VDEV_START_PMF_ENABLED  (1<<1)

struct wmi_p2p_noa_descriptor {
	__le32 type_count; /* 255: continuous schedule, 0: reserved */
	__le32 duration;  /* Absent period duration in micro seconds */
	__le32 interval;   /* Absent period interval in micro seconds */
	__le32 start_time; /* 32 bit tsf time when in starts */
} __packed;

struct wmi_vdev_start_request_cmd {
	/* WMI channel */
	struct wmi_channel chan;
	/* unique id identifying the VDEV, generated by the caller */
	__le32 vdev_id;
	/* requestor id identifying the caller module */
	__le32 requestor_id;
	/* beacon interval from received beacon */
	__le32 beacon_interval;
	/* DTIM Period from the received beacon */
	__le32 dtim_period;
	/* Flags */
	__le32 flags;
	/* ssid field. Only valid for AP/GO/IBSS/BTAmp VDEV type. */
	struct wmi_ssid ssid;
	/* beacon/probe reponse xmit rate. Applicable for SoftAP. */
	__le32 bcn_tx_rate;
	/* beacon/probe reponse xmit power. Applicable for SoftAP. */
	__le32 bcn_tx_power;
	/* number of p2p NOA descriptor(s) from scan entry */
	__le32 num_noa_descriptors;
	/*
	 * Disable H/W ack. This used by WMI_VDEV_RESTART_REQUEST_CMDID.
	 * During CAC, Our HW shouldn't ack ditected frames
	 */
	__le32 disable_hw_ack;
	/* actual p2p NOA descriptor from scan entry */
	struct wmi_p2p_noa_descriptor noa_descriptors[2];
} __packed;

struct wmi_vdev_restart_request_cmd {
	struct wmi_vdev_start_request_cmd vdev_start_request_cmd;
} __packed;

struct wmi_vdev_start_request_arg {
	u32 vdev_id;
	struct wmi_channel_arg channel;
	u32 bcn_intval;
	u32 dtim_period;
	u8 *ssid;
	u32 ssid_len;
	u32 bcn_tx_rate;
	u32 bcn_tx_power;
	bool disable_hw_ack;
	bool hidden_ssid;
	bool pmf_enabled;
};

struct wmi_vdev_delete_cmd {
	/* unique id identifying the VDEV, generated by the caller */
	__le32 vdev_id;
} __packed;

struct wmi_vdev_up_cmd {
	__le32 vdev_id;
	__le32 vdev_assoc_id;
	struct wmi_mac_addr vdev_bssid;
} __packed;

struct wmi_vdev_stop_cmd {
	__le32 vdev_id;
} __packed;

struct wmi_vdev_down_cmd {
	__le32 vdev_id;
} __packed;

struct wmi_vdev_standby_response_cmd {
	/* unique id identifying the VDEV, generated by the caller */
	__le32 vdev_id;
} __packed;

struct wmi_vdev_resume_response_cmd {
	/* unique id identifying the VDEV, generated by the caller */
	__le32 vdev_id;
} __packed;

struct wmi_vdev_set_param_cmd {
	__le32 vdev_id;
	__le32 param_id;
	__le32 param_value;
} __packed;

#define WMI_MAX_KEY_INDEX   3
#define WMI_MAX_KEY_LEN     32

#define WMI_KEY_PAIRWISE 0x00
#define WMI_KEY_GROUP    0x01
#define WMI_KEY_TX_USAGE 0x02 /* default tx key - static wep */

struct wmi_key_seq_counter {
	__le32 key_seq_counter_l;
	__le32 key_seq_counter_h;
} __packed;

#define WMI_CIPHER_NONE     0x0 /* clear key */
#define WMI_CIPHER_WEP      0x1
#define WMI_CIPHER_TKIP     0x2
#define WMI_CIPHER_AES_OCB  0x3
#define WMI_CIPHER_AES_CCM  0x4
#define WMI_CIPHER_WAPI     0x5
#define WMI_CIPHER_CKIP     0x6
#define WMI_CIPHER_AES_CMAC 0x7

struct wmi_vdev_install_key_cmd {
	__le32 vdev_id;
	struct wmi_mac_addr peer_macaddr;
	__le32 key_idx;
	__le32 key_flags;
	__le32 key_cipher; /* %WMI_CIPHER_ */
	struct wmi_key_seq_counter key_rsc_counter;
	struct wmi_key_seq_counter key_global_rsc_counter;
	struct wmi_key_seq_counter key_tsc_counter;
	u8 wpi_key_rsc_counter[16];
	u8 wpi_key_tsc_counter[16];
	__le32 key_len;
	__le32 key_txmic_len;
	__le32 key_rxmic_len;

	/* contains key followed by tx mic followed by rx mic */
	u8 key_data[0];
} __packed;

struct wmi_vdev_install_key_arg {
	u32 vdev_id;
	const u8 *macaddr;
	u32 key_idx;
	u32 key_flags;
	u32 key_cipher;
	u32 key_len;
	u32 key_txmic_len;
	u32 key_rxmic_len;
	const void *key_data;
};

/*
 * vdev fixed rate format:
 * - preamble - b7:b6 - see WMI_RATE_PREMABLE_
 * - nss      - b5:b4 - ss number (0 mean 1ss)
 * - rate_mcs - b3:b0 - as below
 *    CCK:  0 - 11Mbps, 1 - 5,5Mbps, 2 - 2Mbps, 3 - 1Mbps,
 *          4 - 11Mbps (s), 5 - 5,5Mbps (s), 6 - 2Mbps (s)
 *    OFDM: 0 - 48Mbps, 1 - 24Mbps, 2 - 12Mbps, 3 - 6Mbps,
 *          4 - 54Mbps, 5 - 36Mbps, 6 - 18Mbps, 7 - 9Mbps
 *    HT/VHT: MCS index
 */

/* Preamble types to be used with VDEV fixed rate configuration */
enum wmi_rate_preamble {
	WMI_RATE_PREAMBLE_OFDM,
	WMI_RATE_PREAMBLE_CCK,
	WMI_RATE_PREAMBLE_HT,
	WMI_RATE_PREAMBLE_VHT,
};

/* Value to disable fixed rate setting */
#define WMI_FIXED_RATE_NONE    (0xff)

struct wmi_vdev_param_map {
	u32 rts_threshold;
	u32 fragmentation_threshold;
	u32 beacon_interval;
	u32 listen_interval;
	u32 multicast_rate;
	u32 mgmt_tx_rate;
	u32 slot_time;
	u32 preamble;
	u32 swba_time;
	u32 wmi_vdev_stats_update_period;
	u32 wmi_vdev_pwrsave_ageout_time;
	u32 wmi_vdev_host_swba_interval;
	u32 dtim_period;
	u32 wmi_vdev_oc_scheduler_air_time_limit;
	u32 wds;
	u32 atim_window;
	u32 bmiss_count_max;
	u32 bmiss_first_bcnt;
	u32 bmiss_final_bcnt;
	u32 feature_wmm;
	u32 chwidth;
	u32 chextoffset;
	u32 disable_htprotection;
	u32 sta_quickkickout;
	u32 mgmt_rate;
	u32 protection_mode;
	u32 fixed_rate;
	u32 sgi;
	u32 ldpc;
	u32 tx_stbc;
	u32 rx_stbc;
	u32 intra_bss_fwd;
	u32 def_keyid;
	u32 nss;
	u32 bcast_data_rate;
	u32 mcast_data_rate;
	u32 mcast_indicate;
	u32 dhcp_indicate;
	u32 unknown_dest_indicate;
	u32 ap_keepalive_min_idle_inactive_time_secs;
	u32 ap_keepalive_max_idle_inactive_time_secs;
	u32 ap_keepalive_max_unresponsive_time_secs;
	u32 ap_enable_nawds;
	u32 mcast2ucast_set;
	u32 enable_rtscts;
	u32 txbf;
	u32 packet_powersave;
	u32 drop_unencry;
	u32 tx_encap_type;
	u32 ap_detect_out_of_sync_sleeping_sta_time_secs;
};

#define WMI_VDEV_PARAM_UNSUPPORTED 0

/* the definition of different VDEV parameters */
enum wmi_vdev_param {
	/* RTS Threshold */
	WMI_VDEV_PARAM_RTS_THRESHOLD = 0x1,
	/* Fragmentation threshold */
	WMI_VDEV_PARAM_FRAGMENTATION_THRESHOLD,
	/* beacon interval in TUs */
	WMI_VDEV_PARAM_BEACON_INTERVAL,
	/* Listen interval in TUs */
	WMI_VDEV_PARAM_LISTEN_INTERVAL,
	/* muticast rate in Mbps */
	WMI_VDEV_PARAM_MULTICAST_RATE,
	/* management frame rate in Mbps */
	WMI_VDEV_PARAM_MGMT_TX_RATE,
	/* slot time (long vs short) */
	WMI_VDEV_PARAM_SLOT_TIME,
	/* preamble (long vs short) */
	WMI_VDEV_PARAM_PREAMBLE,
	/* SWBA time (time before tbtt in msec) */
	WMI_VDEV_PARAM_SWBA_TIME,
	/* time period for updating VDEV stats */
	WMI_VDEV_STATS_UPDATE_PERIOD,
	/* age out time in msec for frames queued for station in power save */
	WMI_VDEV_PWRSAVE_AGEOUT_TIME,
	/*
	 * Host SWBA interval (time in msec before tbtt for SWBA event
	 * generation).
	 */
	WMI_VDEV_HOST_SWBA_INTERVAL,
	/* DTIM period (specified in units of num beacon intervals) */
	WMI_VDEV_PARAM_DTIM_PERIOD,
	/*
	 * scheduler air time limit for this VDEV. used by off chan
	 * scheduler.
	 */
	WMI_VDEV_OC_SCHEDULER_AIR_TIME_LIMIT,
	/* enable/dsiable WDS for this VDEV  */
	WMI_VDEV_PARAM_WDS,
	/* ATIM Window */
	WMI_VDEV_PARAM_ATIM_WINDOW,
	/* BMISS max */
	WMI_VDEV_PARAM_BMISS_COUNT_MAX,
	/* BMISS first time */
	WMI_VDEV_PARAM_BMISS_FIRST_BCNT,
	/* BMISS final time */
	WMI_VDEV_PARAM_BMISS_FINAL_BCNT,
	/* WMM enables/disabled */
	WMI_VDEV_PARAM_FEATURE_WMM,
	/* Channel width */
	WMI_VDEV_PARAM_CHWIDTH,
	/* Channel Offset */
	WMI_VDEV_PARAM_CHEXTOFFSET,
	/* Disable HT Protection */
	WMI_VDEV_PARAM_DISABLE_HTPROTECTION,
	/* Quick STA Kickout */
	WMI_VDEV_PARAM_STA_QUICKKICKOUT,
	/* Rate to be used with Management frames */
	WMI_VDEV_PARAM_MGMT_RATE,
	/* Protection Mode */
	WMI_VDEV_PARAM_PROTECTION_MODE,
	/* Fixed rate setting */
	WMI_VDEV_PARAM_FIXED_RATE,
	/* Short GI Enable/Disable */
	WMI_VDEV_PARAM_SGI,
	/* Enable LDPC */
	WMI_VDEV_PARAM_LDPC,
	/* Enable Tx STBC */
	WMI_VDEV_PARAM_TX_STBC,
	/* Enable Rx STBC */
	WMI_VDEV_PARAM_RX_STBC,
	/* Intra BSS forwarding  */
	WMI_VDEV_PARAM_INTRA_BSS_FWD,
	/* Setting Default xmit key for Vdev */
	WMI_VDEV_PARAM_DEF_KEYID,
	/* NSS width */
	WMI_VDEV_PARAM_NSS,
	/* Set the custom rate for the broadcast data frames */
	WMI_VDEV_PARAM_BCAST_DATA_RATE,
	/* Set the custom rate (rate-code) for multicast data frames */
	WMI_VDEV_PARAM_MCAST_DATA_RATE,
	/* Tx multicast packet indicate Enable/Disable */
	WMI_VDEV_PARAM_MCAST_INDICATE,
	/* Tx DHCP packet indicate Enable/Disable */
	WMI_VDEV_PARAM_DHCP_INDICATE,
	/* Enable host inspection of Tx unicast packet to unknown destination */
	WMI_VDEV_PARAM_UNKNOWN_DEST_INDICATE,

	/* The minimum amount of time AP begins to consider STA inactive */
	WMI_VDEV_PARAM_AP_KEEPALIVE_MIN_IDLE_INACTIVE_TIME_SECS,

	/*
	 * An associated STA is considered inactive when there is no recent
	 * TX/RX activity and no downlink frames are buffered for it. Once a
	 * STA exceeds the maximum idle inactive time, the AP will send an
	 * 802.11 data-null as a keep alive to verify the STA is still
	 * associated. If the STA does ACK the data-null, or if the data-null
	 * is buffered and the STA does not retrieve it, the STA will be
	 * considered unresponsive
	 * (see WMI_VDEV_AP_KEEPALIVE_MAX_UNRESPONSIVE_TIME_SECS).
	 */
	WMI_VDEV_PARAM_AP_KEEPALIVE_MAX_IDLE_INACTIVE_TIME_SECS,

	/*
	 * An associated STA is considered unresponsive if there is no recent
	 * TX/RX activity and downlink frames are buffered for it. Once a STA
	 * exceeds the maximum unresponsive time, the AP will send a
	 * WMI_STA_KICKOUT event to the host so the STA can be deleted. */
	WMI_VDEV_PARAM_AP_KEEPALIVE_MAX_UNRESPONSIVE_TIME_SECS,

	/* Enable NAWDS : MCAST INSPECT Enable, NAWDS Flag set */
	WMI_VDEV_PARAM_AP_ENABLE_NAWDS,
	/* Enable/Disable RTS-CTS */
	WMI_VDEV_PARAM_ENABLE_RTSCTS,
	/* Enable TXBFee/er */
	WMI_VDEV_PARAM_TXBF,

	/* Set packet power save */
	WMI_VDEV_PARAM_PACKET_POWERSAVE,

	/*
	 * Drops un-encrypted packets if eceived in an encrypted connection
	 * otherwise forwards to host.
	 */
	WMI_VDEV_PARAM_DROP_UNENCRY,

	/*
	 * Set the encapsulation type for frames.
	 */
	WMI_VDEV_PARAM_TX_ENCAP_TYPE,
};

/* the definition of different VDEV parameters */
enum wmi_10x_vdev_param {
	/* RTS Threshold */
	WMI_10X_VDEV_PARAM_RTS_THRESHOLD = 0x1,
	/* Fragmentation threshold */
	WMI_10X_VDEV_PARAM_FRAGMENTATION_THRESHOLD,
	/* beacon interval in TUs */
	WMI_10X_VDEV_PARAM_BEACON_INTERVAL,
	/* Listen interval in TUs */
	WMI_10X_VDEV_PARAM_LISTEN_INTERVAL,
	/* muticast rate in Mbps */
	WMI_10X_VDEV_PARAM_MULTICAST_RATE,
	/* management frame rate in Mbps */
	WMI_10X_VDEV_PARAM_MGMT_TX_RATE,
	/* slot time (long vs short) */
	WMI_10X_VDEV_PARAM_SLOT_TIME,
	/* preamble (long vs short) */
	WMI_10X_VDEV_PARAM_PREAMBLE,
	/* SWBA time (time before tbtt in msec) */
	WMI_10X_VDEV_PARAM_SWBA_TIME,
	/* time period for updating VDEV stats */
	WMI_10X_VDEV_STATS_UPDATE_PERIOD,
	/* age out time in msec for frames queued for station in power save */
	WMI_10X_VDEV_PWRSAVE_AGEOUT_TIME,
	/*
	 * Host SWBA interval (time in msec before tbtt for SWBA event
	 * generation).
	 */
	WMI_10X_VDEV_HOST_SWBA_INTERVAL,
	/* DTIM period (specified in units of num beacon intervals) */
	WMI_10X_VDEV_PARAM_DTIM_PERIOD,
	/*
	 * scheduler air time limit for this VDEV. used by off chan
	 * scheduler.
	 */
	WMI_10X_VDEV_OC_SCHEDULER_AIR_TIME_LIMIT,
	/* enable/dsiable WDS for this VDEV  */
	WMI_10X_VDEV_PARAM_WDS,
	/* ATIM Window */
	WMI_10X_VDEV_PARAM_ATIM_WINDOW,
	/* BMISS max */
	WMI_10X_VDEV_PARAM_BMISS_COUNT_MAX,
	/* WMM enables/disabled */
	WMI_10X_VDEV_PARAM_FEATURE_WMM,
	/* Channel width */
	WMI_10X_VDEV_PARAM_CHWIDTH,
	/* Channel Offset */
	WMI_10X_VDEV_PARAM_CHEXTOFFSET,
	/* Disable HT Protection */
	WMI_10X_VDEV_PARAM_DISABLE_HTPROTECTION,
	/* Quick STA Kickout */
	WMI_10X_VDEV_PARAM_STA_QUICKKICKOUT,
	/* Rate to be used with Management frames */
	WMI_10X_VDEV_PARAM_MGMT_RATE,
	/* Protection Mode */
	WMI_10X_VDEV_PARAM_PROTECTION_MODE,
	/* Fixed rate setting */
	WMI_10X_VDEV_PARAM_FIXED_RATE,
	/* Short GI Enable/Disable */
	WMI_10X_VDEV_PARAM_SGI,
	/* Enable LDPC */
	WMI_10X_VDEV_PARAM_LDPC,
	/* Enable Tx STBC */
	WMI_10X_VDEV_PARAM_TX_STBC,
	/* Enable Rx STBC */
	WMI_10X_VDEV_PARAM_RX_STBC,
	/* Intra BSS forwarding  */
	WMI_10X_VDEV_PARAM_INTRA_BSS_FWD,
	/* Setting Default xmit key for Vdev */
	WMI_10X_VDEV_PARAM_DEF_KEYID,
	/* NSS width */
	WMI_10X_VDEV_PARAM_NSS,
	/* Set the custom rate for the broadcast data frames */
	WMI_10X_VDEV_PARAM_BCAST_DATA_RATE,
	/* Set the custom rate (rate-code) for multicast data frames */
	WMI_10X_VDEV_PARAM_MCAST_DATA_RATE,
	/* Tx multicast packet indicate Enable/Disable */
	WMI_10X_VDEV_PARAM_MCAST_INDICATE,
	/* Tx DHCP packet indicate Enable/Disable */
	WMI_10X_VDEV_PARAM_DHCP_INDICATE,
	/* Enable host inspection of Tx unicast packet to unknown destination */
	WMI_10X_VDEV_PARAM_UNKNOWN_DEST_INDICATE,

	/* The minimum amount of time AP begins to consider STA inactive */
	WMI_10X_VDEV_PARAM_AP_KEEPALIVE_MIN_IDLE_INACTIVE_TIME_SECS,

	/*
	 * An associated STA is considered inactive when there is no recent
	 * TX/RX activity and no downlink frames are buffered for it. Once a
	 * STA exceeds the maximum idle inactive time, the AP will send an
	 * 802.11 data-null as a keep alive to verify the STA is still
	 * associated. If the STA does ACK the data-null, or if the data-null
	 * is buffered and the STA does not retrieve it, the STA will be
	 * considered unresponsive
	 * (see WMI_10X_VDEV_AP_KEEPALIVE_MAX_UNRESPONSIVE_TIME_SECS).
	 */
	WMI_10X_VDEV_PARAM_AP_KEEPALIVE_MAX_IDLE_INACTIVE_TIME_SECS,

	/*
	 * An associated STA is considered unresponsive if there is no recent
	 * TX/RX activity and downlink frames are buffered for it. Once a STA
	 * exceeds the maximum unresponsive time, the AP will send a
	 * WMI_10X_STA_KICKOUT event to the host so the STA can be deleted. */
	WMI_10X_VDEV_PARAM_AP_KEEPALIVE_MAX_UNRESPONSIVE_TIME_SECS,

	/* Enable NAWDS : MCAST INSPECT Enable, NAWDS Flag set */
	WMI_10X_VDEV_PARAM_AP_ENABLE_NAWDS,

	WMI_10X_VDEV_PARAM_MCAST2UCAST_SET,
	/* Enable/Disable RTS-CTS */
	WMI_10X_VDEV_PARAM_ENABLE_RTSCTS,

	WMI_10X_VDEV_PARAM_AP_DETECT_OUT_OF_SYNC_SLEEPING_STA_TIME_SECS,
};

/* slot time long */
#define WMI_VDEV_SLOT_TIME_LONG		0x1
/* slot time short */
#define WMI_VDEV_SLOT_TIME_SHORT	0x2
/* preablbe long */
#define WMI_VDEV_PREAMBLE_LONG		0x1
/* preablbe short */
#define WMI_VDEV_PREAMBLE_SHORT		0x2

enum wmi_start_event_param {
	WMI_VDEV_RESP_START_EVENT = 0,
	WMI_VDEV_RESP_RESTART_EVENT,
};

struct wmi_vdev_start_response_event {
	__le32 vdev_id;
	__le32 req_id;
	__le32 resp_type; /* %WMI_VDEV_RESP_ */
	__le32 status;
} __packed;

struct wmi_vdev_standby_req_event {
	/* unique id identifying the VDEV, generated by the caller */
	__le32 vdev_id;
} __packed;

struct wmi_vdev_resume_req_event {
	/* unique id identifying the VDEV, generated by the caller */
	__le32 vdev_id;
} __packed;

struct wmi_vdev_stopped_event {
	/* unique id identifying the VDEV, generated by the caller */
	__le32 vdev_id;
} __packed;

/*
 * common structure used for simple events
 * (stopped, resume_req, standby response)
 */
struct wmi_vdev_simple_event {
	/* unique id identifying the VDEV, generated by the caller */
	__le32 vdev_id;
} __packed;

/* VDEV start response status codes */
/* VDEV succesfully started */
#define WMI_INIFIED_VDEV_START_RESPONSE_STATUS_SUCCESS	0x0

/* requested VDEV not found */
#define WMI_INIFIED_VDEV_START_RESPONSE_INVALID_VDEVID	0x1

/* unsupported VDEV combination */
#define WMI_INIFIED_VDEV_START_RESPONSE_NOT_SUPPORTED	0x2

/* Beacon processing related command and event structures */
struct wmi_bcn_tx_hdr {
	__le32 vdev_id;
	__le32 tx_rate;
	__le32 tx_power;
	__le32 bcn_len;
} __packed;

struct wmi_bcn_tx_cmd {
	struct wmi_bcn_tx_hdr hdr;
	u8 *bcn[0];
} __packed;

struct wmi_bcn_tx_arg {
	u32 vdev_id;
	u32 tx_rate;
	u32 tx_power;
	u32 bcn_len;
	const void *bcn;
};

/* Beacon filter */
#define WMI_BCN_FILTER_ALL   0 /* Filter all beacons */
#define WMI_BCN_FILTER_NONE  1 /* Pass all beacons */
#define WMI_BCN_FILTER_RSSI  2 /* Pass Beacons RSSI >= RSSI threshold */
#define WMI_BCN_FILTER_BSSID 3 /* Pass Beacons with matching BSSID */
#define WMI_BCN_FILTER_SSID  4 /* Pass Beacons with matching SSID */

struct wmi_bcn_filter_rx_cmd {
	/* Filter ID */
	__le32 bcn_filter_id;
	/* Filter type - wmi_bcn_filter */
	__le32 bcn_filter;
	/* Buffer len */
	__le32 bcn_filter_len;
	/* Filter info (threshold, BSSID, RSSI) */
	u8 *bcn_filter_buf;
} __packed;

/* Capabilities and IEs to be passed to firmware */
struct wmi_bcn_prb_info {
	/* Capabilities */
	__le32 caps;
	/* ERP info */
	__le32 erp;
	/* Advanced capabilities */
	/* HT capabilities */
	/* HT Info */
	/* ibss_dfs */
	/* wpa Info */
	/* rsn Info */
	/* rrm info */
	/* ath_ext */
	/* app IE */
} __packed;

struct wmi_bcn_tmpl_cmd {
	/* unique id identifying the VDEV, generated by the caller */
	__le32 vdev_id;
	/* TIM IE offset from the beginning of the template. */
	__le32 tim_ie_offset;
	/* beacon probe capabilities and IEs */
	struct wmi_bcn_prb_info bcn_prb_info;
	/* beacon buffer length */
	__le32 buf_len;
	/* variable length data */
	u8 data[1];
} __packed;

struct wmi_prb_tmpl_cmd {
	/* unique id identifying the VDEV, generated by the caller */
	__le32 vdev_id;
	/* beacon probe capabilities and IEs */
	struct wmi_bcn_prb_info bcn_prb_info;
	/* beacon buffer length */
	__le32 buf_len;
	/* Variable length data */
	u8 data[1];
} __packed;

enum wmi_sta_ps_mode {
	/* enable power save for the given STA VDEV */
	WMI_STA_PS_MODE_DISABLED = 0,
	/* disable power save  for a given STA VDEV */
	WMI_STA_PS_MODE_ENABLED = 1,
};

struct wmi_sta_powersave_mode_cmd {
	/* unique id identifying the VDEV, generated by the caller */
	__le32 vdev_id;

	/*
	 * Power save mode
	 * (see enum wmi_sta_ps_mode)
	 */
	__le32 sta_ps_mode;
} __packed;

enum wmi_csa_offload_en {
	WMI_CSA_OFFLOAD_DISABLE = 0,
	WMI_CSA_OFFLOAD_ENABLE = 1,
};

struct wmi_csa_offload_enable_cmd {
	__le32 vdev_id;
	__le32 csa_offload_enable;
} __packed;

struct wmi_csa_offload_chanswitch_cmd {
	__le32 vdev_id;
	struct wmi_channel chan;
} __packed;

/*
 * This parameter controls the policy for retrieving frames from AP while the
 * STA is in sleep state.
 *
 * Only takes affect if the sta_ps_mode is enabled
 */
enum wmi_sta_ps_param_rx_wake_policy {
	/*
	 * Wake up when ever there is an  RX activity on the VDEV. In this mode
	 * the Power save SM(state machine) will come out of sleep by either
	 * sending null frame (or) a data frame (with PS==0) in response to TIM
	 * bit set in the received beacon frame from AP.
	 */
	WMI_STA_PS_RX_WAKE_POLICY_WAKE = 0,

	/*
	 * Here the power save state machine will not wakeup in response to TIM
	 * bit, instead it will send a PSPOLL (or) UASPD trigger based on UAPSD
	 * configuration setup by WMISET_PS_SET_UAPSD  WMI command.  When all
	 * access categories are delivery-enabled, the station will send a
	 * UAPSD trigger frame, otherwise it will send a PS-Poll.
	 */
	WMI_STA_PS_RX_WAKE_POLICY_POLL_UAPSD = 1,
};

/*
 * Number of tx frames/beacon  that cause the power save SM to wake up.
 *
 * Value 1 causes the SM to wake up for every TX. Value 0 has a special
 * meaning, It will cause the SM to never wake up. This is useful if you want
 * to keep the system to sleep all the time for some kind of test mode . host
 * can change this parameter any time.  It will affect at the next tx frame.
 */
enum wmi_sta_ps_param_tx_wake_threshold {
	WMI_STA_PS_TX_WAKE_THRESHOLD_NEVER = 0,
	WMI_STA_PS_TX_WAKE_THRESHOLD_ALWAYS = 1,

	/*
	 * Values greater than one indicate that many TX attempts per beacon
	 * interval before the STA will wake up
	 */
};

/*
 * The maximum number of PS-Poll frames the FW will send in response to
 * traffic advertised in TIM before waking up (by sending a null frame with PS
 * = 0). Value 0 has a special meaning: there is no maximum count and the FW
 * will send as many PS-Poll as are necessary to retrieve buffered BU. This
 * parameter is used when the RX wake policy is
 * WMI_STA_PS_RX_WAKE_POLICY_POLL_UAPSD and ignored when the RX wake
 * policy is WMI_STA_PS_RX_WAKE_POLICY_WAKE.
 */
enum wmi_sta_ps_param_pspoll_count {
	WMI_STA_PS_PSPOLL_COUNT_NO_MAX = 0,
	/*
	 * Values greater than 0 indicate the maximum numer of PS-Poll frames
	 * FW will send before waking up.
	 */
};

/*
 * This will include the delivery and trigger enabled state for every AC.
 * This is the negotiated state with AP. The host MLME needs to set this based
 * on AP capability and the state Set in the association request by the
 * station MLME.Lower 8 bits of the value specify the UAPSD configuration.
 */
#define WMI_UAPSD_AC_TYPE_DELI 0
#define WMI_UAPSD_AC_TYPE_TRIG 1

#define WMI_UAPSD_AC_BIT_MASK(ac, type) \
	((type ==  WMI_UAPSD_AC_TYPE_DELI) ? (1<<(ac<<1)) : (1<<((ac<<1)+1)))

enum wmi_sta_ps_param_uapsd {
	WMI_STA_PS_UAPSD_AC0_DELIVERY_EN = (1 << 0),
	WMI_STA_PS_UAPSD_AC0_TRIGGER_EN  = (1 << 1),
	WMI_STA_PS_UAPSD_AC1_DELIVERY_EN = (1 << 2),
	WMI_STA_PS_UAPSD_AC1_TRIGGER_EN  = (1 << 3),
	WMI_STA_PS_UAPSD_AC2_DELIVERY_EN = (1 << 4),
	WMI_STA_PS_UAPSD_AC2_TRIGGER_EN  = (1 << 5),
	WMI_STA_PS_UAPSD_AC3_DELIVERY_EN = (1 << 6),
	WMI_STA_PS_UAPSD_AC3_TRIGGER_EN  = (1 << 7),
};

enum wmi_sta_powersave_param {
	/*
	 * Controls how frames are retrievd from AP while STA is sleeping
	 *
	 * (see enum wmi_sta_ps_param_rx_wake_policy)
	 */
	WMI_STA_PS_PARAM_RX_WAKE_POLICY = 0,

	/*
	 * The STA will go active after this many TX
	 *
	 * (see enum wmi_sta_ps_param_tx_wake_threshold)
	 */
	WMI_STA_PS_PARAM_TX_WAKE_THRESHOLD = 1,

	/*
	 * Number of PS-Poll to send before STA wakes up
	 *
	 * (see enum wmi_sta_ps_param_pspoll_count)
	 *
	 */
	WMI_STA_PS_PARAM_PSPOLL_COUNT = 2,

	/*
	 * TX/RX inactivity time in msec before going to sleep.
	 *
	 * The power save SM will monitor tx/rx activity on the VDEV, if no
	 * activity for the specified msec of the parameter the Power save
	 * SM will go to sleep.
	 */
	WMI_STA_PS_PARAM_INACTIVITY_TIME = 3,

	/*
	 * Set uapsd configuration.
	 *
	 * (see enum wmi_sta_ps_param_uapsd)
	 */
	WMI_STA_PS_PARAM_UAPSD = 4,
};

struct wmi_sta_powersave_param_cmd {
	__le32 vdev_id;
	__le32 param_id; /* %WMI_STA_PS_PARAM_ */
	__le32 param_value;
} __packed;

/* No MIMO power save */
#define WMI_STA_MIMO_PS_MODE_DISABLE
/* mimo powersave mode static*/
#define WMI_STA_MIMO_PS_MODE_STATIC
/* mimo powersave mode dynamic */
#define WMI_STA_MIMO_PS_MODE_DYNAMIC

struct wmi_sta_mimo_ps_mode_cmd {
	/* unique id identifying the VDEV, generated by the caller */
	__le32 vdev_id;
	/* mimo powersave mode as defined above */
	__le32 mimo_pwrsave_mode;
} __packed;

/* U-APSD configuration of peer station from (re)assoc request and TSPECs */
enum wmi_ap_ps_param_uapsd {
	WMI_AP_PS_UAPSD_AC0_DELIVERY_EN = (1 << 0),
	WMI_AP_PS_UAPSD_AC0_TRIGGER_EN  = (1 << 1),
	WMI_AP_PS_UAPSD_AC1_DELIVERY_EN = (1 << 2),
	WMI_AP_PS_UAPSD_AC1_TRIGGER_EN  = (1 << 3),
	WMI_AP_PS_UAPSD_AC2_DELIVERY_EN = (1 << 4),
	WMI_AP_PS_UAPSD_AC2_TRIGGER_EN  = (1 << 5),
	WMI_AP_PS_UAPSD_AC3_DELIVERY_EN = (1 << 6),
	WMI_AP_PS_UAPSD_AC3_TRIGGER_EN  = (1 << 7),
};

/* U-APSD maximum service period of peer station */
enum wmi_ap_ps_peer_param_max_sp {
	WMI_AP_PS_PEER_PARAM_MAX_SP_UNLIMITED = 0,
	WMI_AP_PS_PEER_PARAM_MAX_SP_2 = 1,
	WMI_AP_PS_PEER_PARAM_MAX_SP_4 = 2,
	WMI_AP_PS_PEER_PARAM_MAX_SP_6 = 3,
	MAX_WMI_AP_PS_PEER_PARAM_MAX_SP,
};

/*
 * AP power save parameter
 * Set a power save specific parameter for a peer station
 */
enum wmi_ap_ps_peer_param {
	/* Set uapsd configuration for a given peer.
	 *
	 * Include the delivery and trigger enabled state for every AC.
	 * The host  MLME needs to set this based on AP capability and stations
	 * request Set in the association request  received from the station.
	 *
	 * Lower 8 bits of the value specify the UAPSD configuration.
	 *
	 * (see enum wmi_ap_ps_param_uapsd)
	 * The default value is 0.
	 */
	WMI_AP_PS_PEER_PARAM_UAPSD = 0,

	/*
	 * Set the service period for a UAPSD capable station
	 *
	 * The service period from wme ie in the (re)assoc request frame.
	 *
	 * (see enum wmi_ap_ps_peer_param_max_sp)
	 */
	WMI_AP_PS_PEER_PARAM_MAX_SP = 1,

	/* Time in seconds for aging out buffered frames for STA in PS */
	WMI_AP_PS_PEER_PARAM_AGEOUT_TIME = 2,
};

struct wmi_ap_ps_peer_cmd {
	/* unique id identifying the VDEV, generated by the caller */
	__le32 vdev_id;

	/* peer MAC address */
	struct wmi_mac_addr peer_macaddr;

	/* AP powersave param (see enum wmi_ap_ps_peer_param) */
	__le32 param_id;

	/* AP powersave param value */
	__le32 param_value;
} __packed;

/* 128 clients = 4 words */
#define WMI_TIM_BITMAP_ARRAY_SIZE 4

struct wmi_tim_info {
	__le32 tim_len;
	__le32 tim_mcast;
	__le32 tim_bitmap[WMI_TIM_BITMAP_ARRAY_SIZE];
	__le32 tim_changed;
	__le32 tim_num_ps_pending;
} __packed;

/* Maximum number of NOA Descriptors supported */
#define WMI_P2P_MAX_NOA_DESCRIPTORS 4
#define WMI_P2P_OPPPS_ENABLE_BIT	BIT(0)
#define WMI_P2P_OPPPS_CTWINDOW_OFFSET	1
#define WMI_P2P_NOA_CHANGED_BIT	BIT(0)

struct wmi_p2p_noa_info {
	/* Bit 0 - Flag to indicate an update in NOA schedule
	   Bits 7-1 - Reserved */
	u8 changed;
	/* NOA index */
	u8 index;
	/* Bit 0 - Opp PS state of the AP
	   Bits 1-7 - Ctwindow in TUs */
	u8 ctwindow_oppps;
	/* Number of NOA descriptors */
	u8 num_descriptors;

	struct wmi_p2p_noa_descriptor descriptors[WMI_P2P_MAX_NOA_DESCRIPTORS];
} __packed;

struct wmi_bcn_info {
	struct wmi_tim_info tim_info;
	struct wmi_p2p_noa_info p2p_noa_info;
} __packed;

struct wmi_host_swba_event {
	__le32 vdev_map;
	struct wmi_bcn_info bcn_info[1];
} __packed;

#define WMI_MAX_AP_VDEV 16

struct wmi_tbtt_offset_event {
	__le32 vdev_map;
	__le32 tbttoffset_list[WMI_MAX_AP_VDEV];
} __packed;


struct wmi_peer_create_cmd {
	__le32 vdev_id;
	struct wmi_mac_addr peer_macaddr;
} __packed;

struct wmi_peer_delete_cmd {
	__le32 vdev_id;
	struct wmi_mac_addr peer_macaddr;
} __packed;

struct wmi_peer_flush_tids_cmd {
	__le32 vdev_id;
	struct wmi_mac_addr peer_macaddr;
	__le32 peer_tid_bitmap;
} __packed;

struct wmi_fixed_rate {
	/*
	 * rate mode . 0: disable fixed rate (auto rate)
	 *   1: legacy (non 11n) rate  specified as ieee rate 2*Mbps
	 *   2: ht20 11n rate  specified as mcs index
	 *   3: ht40 11n rate  specified as mcs index
	 */
	__le32  rate_mode;
	/*
	 * 4 rate values for 4 rate series. series 0 is stored in byte 0 (LSB)
	 * and series 3 is stored at byte 3 (MSB)
	 */
	__le32  rate_series;
	/*
	 * 4 retry counts for 4 rate series. retry count for rate 0 is stored
	 * in byte 0 (LSB) and retry count for rate 3 is stored at byte 3
	 * (MSB)
	 */
	__le32  rate_retries;
} __packed;

struct wmi_peer_fixed_rate_cmd {
	/* unique id identifying the VDEV, generated by the caller */
	__le32 vdev_id;
	/* peer MAC address */
	struct wmi_mac_addr peer_macaddr;
	/* fixed rate */
	struct wmi_fixed_rate peer_fixed_rate;
} __packed;

#define WMI_MGMT_TID    17

struct wmi_addba_clear_resp_cmd {
	/* unique id identifying the VDEV, generated by the caller */
	__le32 vdev_id;
	/* peer MAC address */
	struct wmi_mac_addr peer_macaddr;
} __packed;

struct wmi_addba_send_cmd {
	/* unique id identifying the VDEV, generated by the caller */
	__le32 vdev_id;
	/* peer MAC address */
	struct wmi_mac_addr peer_macaddr;
	/* Tid number */
	__le32 tid;
	/* Buffer/Window size*/
	__le32 buffersize;
} __packed;

struct wmi_delba_send_cmd {
	/* unique id identifying the VDEV, generated by the caller */
	__le32 vdev_id;
	/* peer MAC address */
	struct wmi_mac_addr peer_macaddr;
	/* Tid number */
	__le32 tid;
	/* Is Initiator */
	__le32 initiator;
	/* Reason code */
	__le32 reasoncode;
} __packed;

struct wmi_addba_setresponse_cmd {
	/* unique id identifying the vdev, generated by the caller */
	__le32 vdev_id;
	/* peer mac address */
	struct wmi_mac_addr peer_macaddr;
	/* Tid number */
	__le32 tid;
	/* status code */
	__le32 statuscode;
} __packed;

struct wmi_send_singleamsdu_cmd {
	/* unique id identifying the vdev, generated by the caller */
	__le32 vdev_id;
	/* peer mac address */
	struct wmi_mac_addr peer_macaddr;
	/* Tid number */
	__le32 tid;
} __packed;

enum wmi_peer_smps_state {
	WMI_PEER_SMPS_PS_NONE = 0x0,
	WMI_PEER_SMPS_STATIC  = 0x1,
	WMI_PEER_SMPS_DYNAMIC = 0x2
};

enum wmi_peer_param {
	WMI_PEER_SMPS_STATE = 0x1, /* see %wmi_peer_smps_state */
	WMI_PEER_AMPDU      = 0x2,
	WMI_PEER_AUTHORIZE  = 0x3,
	WMI_PEER_CHAN_WIDTH = 0x4,
	WMI_PEER_NSS        = 0x5,
	WMI_PEER_USE_4ADDR  = 0x6
};

struct wmi_peer_set_param_cmd {
	__le32 vdev_id;
	struct wmi_mac_addr peer_macaddr;
	__le32 param_id;
	__le32 param_value;
} __packed;

#define MAX_SUPPORTED_RATES 128

struct wmi_rate_set {
	/* total number of rates */
	__le32 num_rates;
	/*
	 * rates (each 8bit value) packed into a 32 bit word.
	 * the rates are filled from least significant byte to most
	 * significant byte.
	 */
	__le32 rates[(MAX_SUPPORTED_RATES/4)+1];
} __packed;

struct wmi_rate_set_arg {
	unsigned int num_rates;
	u8 rates[MAX_SUPPORTED_RATES];
};

/*
 * NOTE: It would bea good idea to represent the Tx MCS
 * info in one word and Rx in another word. This is split
 * into multiple words for convenience
 */
struct wmi_vht_rate_set {
	__le32 rx_max_rate; /* Max Rx data rate */
	__le32 rx_mcs_set;  /* Negotiated RX VHT rates */
	__le32 tx_max_rate; /* Max Tx data rate */
	__le32 tx_mcs_set;  /* Negotiated TX VHT rates */
} __packed;

struct wmi_vht_rate_set_arg {
	u32 rx_max_rate;
	u32 rx_mcs_set;
	u32 tx_max_rate;
	u32 tx_mcs_set;
};

struct wmi_peer_set_rates_cmd {
	/* peer MAC address */
	struct wmi_mac_addr peer_macaddr;
	/* legacy rate set */
	struct wmi_rate_set peer_legacy_rates;
	/* ht rate set */
	struct wmi_rate_set peer_ht_rates;
} __packed;

struct wmi_peer_set_q_empty_callback_cmd {
	/* unique id identifying the VDEV, generated by the caller */
	__le32 vdev_id;
	/* peer MAC address */
	struct wmi_mac_addr peer_macaddr;
	__le32 callback_enable;
} __packed;

#define WMI_PEER_AUTH           0x00000001
#define WMI_PEER_QOS            0x00000002
#define WMI_PEER_NEED_PTK_4_WAY 0x00000004
#define WMI_PEER_NEED_GTK_2_WAY 0x00000010
#define WMI_PEER_APSD           0x00000800
#define WMI_PEER_HT             0x00001000
#define WMI_PEER_40MHZ          0x00002000
#define WMI_PEER_STBC           0x00008000
#define WMI_PEER_LDPC           0x00010000
#define WMI_PEER_DYN_MIMOPS     0x00020000
#define WMI_PEER_STATIC_MIMOPS  0x00040000
#define WMI_PEER_SPATIAL_MUX    0x00200000
#define WMI_PEER_VHT            0x02000000
#define WMI_PEER_80MHZ          0x04000000
#define WMI_PEER_PMF            0x08000000

/*
 * Peer rate capabilities.
 *
 * This is of interest to the ratecontrol
 * module which resides in the firmware. The bit definitions are
 * consistent with that defined in if_athrate.c.
 */
#define WMI_RC_DS_FLAG          0x01
#define WMI_RC_CW40_FLAG        0x02
#define WMI_RC_SGI_FLAG         0x04
#define WMI_RC_HT_FLAG          0x08
#define WMI_RC_RTSCTS_FLAG      0x10
#define WMI_RC_TX_STBC_FLAG     0x20
#define WMI_RC_RX_STBC_FLAG     0xC0
#define WMI_RC_RX_STBC_FLAG_S   6
#define WMI_RC_WEP_TKIP_FLAG    0x100
#define WMI_RC_TS_FLAG          0x200
#define WMI_RC_UAPSD_FLAG       0x400

/* Maximum listen interval supported by hw in units of beacon interval */
#define ATH10K_MAX_HW_LISTEN_INTERVAL 5

struct wmi_peer_assoc_complete_cmd {
	struct wmi_mac_addr peer_macaddr;
	__le32 vdev_id;
	__le32 peer_new_assoc; /* 1=assoc, 0=reassoc */
	__le32 peer_associd; /* 16 LSBs */
	__le32 peer_flags;
	__le32 peer_caps; /* 16 LSBs */
	__le32 peer_listen_intval;
	__le32 peer_ht_caps;
	__le32 peer_max_mpdu;
	__le32 peer_mpdu_density; /* 0..16 */
	__le32 peer_rate_caps;
	struct wmi_rate_set peer_legacy_rates;
	struct wmi_rate_set peer_ht_rates;
	__le32 peer_nss; /* num of spatial streams */
	__le32 peer_vht_caps;
	__le32 peer_phymode;
	struct wmi_vht_rate_set peer_vht_rates;
	/* HT Operation Element of the peer. Five bytes packed in 2
	 *  INT32 array and filled from lsb to msb. */
	__le32 peer_ht_info[2];
} __packed;

struct wmi_peer_assoc_complete_arg {
	u8 addr[ETH_ALEN];
	u32 vdev_id;
	bool peer_reassoc;
	u16 peer_aid;
	u32 peer_flags; /* see %WMI_PEER_ */
	u16 peer_caps;
	u32 peer_listen_intval;
	u32 peer_ht_caps;
	u32 peer_max_mpdu;
	u32 peer_mpdu_density; /* 0..16 */
	u32 peer_rate_caps; /* see %WMI_RC_ */
	struct wmi_rate_set_arg peer_legacy_rates;
	struct wmi_rate_set_arg peer_ht_rates;
	u32 peer_num_spatial_streams;
	u32 peer_vht_caps;
	enum wmi_phy_mode peer_phymode;
	struct wmi_vht_rate_set_arg peer_vht_rates;
};

struct wmi_peer_add_wds_entry_cmd {
	/* peer MAC address */
	struct wmi_mac_addr peer_macaddr;
	/* wds MAC addr */
	struct wmi_mac_addr wds_macaddr;
} __packed;

struct wmi_peer_remove_wds_entry_cmd {
	/* wds MAC addr */
	struct wmi_mac_addr wds_macaddr;
} __packed;

struct wmi_peer_q_empty_callback_event {
	/* peer MAC address */
	struct wmi_mac_addr peer_macaddr;
} __packed;

/*
 * Channel info WMI event
 */
struct wmi_chan_info_event {
	__le32 err_code;
	__le32 freq;
	__le32 cmd_flags;
	__le32 noise_floor;
	__le32 rx_clear_count;
	__le32 cycle_count;
} __packed;

#define WMI_CHAN_INFO_FLAG_COMPLETE BIT(0)

/* FIXME: empirically extrapolated */
#define WMI_CHAN_INFO_MSEC(x) ((x) / 76595)

/* Beacon filter wmi command info */
#define BCN_FLT_MAX_SUPPORTED_IES	256
#define BCN_FLT_MAX_ELEMS_IE_LIST	(BCN_FLT_MAX_SUPPORTED_IES / 32)

struct bss_bcn_stats {
	__le32 vdev_id;
	__le32 bss_bcnsdropped;
	__le32 bss_bcnsdelivered;
} __packed;

struct bcn_filter_stats {
	__le32 bcns_dropped;
	__le32 bcns_delivered;
	__le32 activefilters;
	struct bss_bcn_stats bss_stats;
} __packed;

struct wmi_add_bcn_filter_cmd {
	u32 vdev_id;
	u32 ie_map[BCN_FLT_MAX_ELEMS_IE_LIST];
} __packed;

enum wmi_sta_keepalive_method {
	WMI_STA_KEEPALIVE_METHOD_NULL_FRAME = 1,
	WMI_STA_KEEPALIVE_METHOD_UNSOLICITATED_ARP_RESPONSE = 2,
};

/* note: ip4 addresses are in network byte order, i.e. big endian */
struct wmi_sta_keepalive_arp_resp {
	__be32 src_ip4_addr;
	__be32 dest_ip4_addr;
	struct wmi_mac_addr dest_mac_addr;
} __packed;

struct wmi_sta_keepalive_cmd {
	__le32 vdev_id;
	__le32 enabled;
	__le32 method; /* WMI_STA_KEEPALIVE_METHOD_ */
	__le32 interval; /* in seconds */
	struct wmi_sta_keepalive_arp_resp arp_resp;
} __packed;

enum wmi_force_fw_hang_type {
	WMI_FORCE_FW_HANG_ASSERT = 1,
	WMI_FORCE_FW_HANG_NO_DETECT,
	WMI_FORCE_FW_HANG_CTRL_EP_FULL,
	WMI_FORCE_FW_HANG_EMPTY_POINT,
	WMI_FORCE_FW_HANG_STACK_OVERFLOW,
	WMI_FORCE_FW_HANG_INFINITE_LOOP,
};

#define WMI_FORCE_FW_HANG_RANDOM_TIME 0xFFFFFFFF

struct wmi_force_fw_hang_cmd {
	__le32 type;
	__le32 delay_ms;
} __packed;

enum ath10k_dbglog_level {
	ATH10K_DBGLOG_LEVEL_VERBOSE = 0,
	ATH10K_DBGLOG_LEVEL_INFO = 1,
	ATH10K_DBGLOG_LEVEL_WARN = 2,
	ATH10K_DBGLOG_LEVEL_ERR = 3,
};

/* VAP ids to enable dbglog */
#define ATH10K_DBGLOG_CFG_VAP_LOG_LSB		0
#define ATH10K_DBGLOG_CFG_VAP_LOG_MASK		0x0000ffff

/* to enable dbglog in the firmware */
#define ATH10K_DBGLOG_CFG_REPORTING_ENABLE_LSB	16
#define ATH10K_DBGLOG_CFG_REPORTING_ENABLE_MASK	0x00010000

/* timestamp resolution */
#define ATH10K_DBGLOG_CFG_RESOLUTION_LSB	17
#define ATH10K_DBGLOG_CFG_RESOLUTION_MASK	0x000E0000

/* number of queued messages before sending them to the host */
#define ATH10K_DBGLOG_CFG_REPORT_SIZE_LSB	20
#define ATH10K_DBGLOG_CFG_REPORT_SIZE_MASK	0x0ff00000

/*
 * Log levels to enable. This defines the minimum level to enable, this is
 * not a bitmask. See enum ath10k_dbglog_level for the values.
 */
#define ATH10K_DBGLOG_CFG_LOG_LVL_LSB		28
#define ATH10K_DBGLOG_CFG_LOG_LVL_MASK		0x70000000

/*
 * Note: this is a cleaned up version of a struct firmware uses. For
 * example, config_valid was hidden inside an array.
 */
struct wmi_dbglog_cfg_cmd {
	/* bitmask to hold mod id config*/
	__le32 module_enable;

	/* see ATH10K_DBGLOG_CFG_ */
	__le32 config_enable;

	/* mask of module id bits to be changed */
	__le32 module_valid;

	/* mask of config bits to be changed, see ATH10K_DBGLOG_CFG_ */
	__le32 config_valid;
} __packed;

#define ATH10K_RTS_MAX		2347
#define ATH10K_FRAGMT_THRESHOLD_MIN	540
#define ATH10K_FRAGMT_THRESHOLD_MAX	2346

#define WMI_MAX_EVENT 0x1000
/* Maximum number of pending TXed WMI packets */
#define WMI_SKB_HEADROOM sizeof(struct wmi_cmd_hdr)

/* By default disable power save for IBSS */
#define ATH10K_DEFAULT_ATIM 0

struct ath10k;
struct ath10k_vif;

int ath10k_wmi_attach(struct ath10k *ar);
void ath10k_wmi_detach(struct ath10k *ar);
int ath10k_wmi_wait_for_service_ready(struct ath10k *ar);
int ath10k_wmi_wait_for_unified_ready(struct ath10k *ar);

int ath10k_wmi_connect_htc_service(struct ath10k *ar);
int ath10k_wmi_pdev_set_channel(struct ath10k *ar,
				const struct wmi_channel_arg *);
int ath10k_wmi_pdev_suspend_target(struct ath10k *ar);
int ath10k_wmi_pdev_resume_target(struct ath10k *ar);
int ath10k_wmi_pdev_set_regdomain(struct ath10k *ar, u16 rd, u16 rd2g,
				  u16 rd5g, u16 ctl2g, u16 ctl5g);
int ath10k_wmi_pdev_set_param(struct ath10k *ar, u32 id, u32 value);
int ath10k_wmi_cmd_init(struct ath10k *ar);
int ath10k_wmi_start_scan(struct ath10k *ar, const struct wmi_start_scan_arg *);
void ath10k_wmi_start_scan_init(struct ath10k *ar, struct wmi_start_scan_arg *);
int ath10k_wmi_stop_scan(struct ath10k *ar,
			 const struct wmi_stop_scan_arg *arg);
int ath10k_wmi_vdev_create(struct ath10k *ar, u32 vdev_id,
			   enum wmi_vdev_type type,
			   enum wmi_vdev_subtype subtype,
			   const u8 macaddr[ETH_ALEN]);
int ath10k_wmi_vdev_delete(struct ath10k *ar, u32 vdev_id);
int ath10k_wmi_vdev_start(struct ath10k *ar,
			  const struct wmi_vdev_start_request_arg *);
int ath10k_wmi_vdev_restart(struct ath10k *ar,
			    const struct wmi_vdev_start_request_arg *);
int ath10k_wmi_vdev_stop(struct ath10k *ar, u32 vdev_id);
int ath10k_wmi_vdev_up(struct ath10k *ar, u32 vdev_id, u32 aid,
		       const u8 *bssid);
int ath10k_wmi_vdev_down(struct ath10k *ar, u32 vdev_id);
int ath10k_wmi_vdev_set_param(struct ath10k *ar, u32 vdev_id,
			      u32 param_id, u32 param_value);
int ath10k_wmi_vdev_install_key(struct ath10k *ar,
				const struct wmi_vdev_install_key_arg *arg);
int ath10k_wmi_peer_create(struct ath10k *ar, u32 vdev_id,
		    const u8 peer_addr[ETH_ALEN]);
int ath10k_wmi_peer_delete(struct ath10k *ar, u32 vdev_id,
		    const u8 peer_addr[ETH_ALEN]);
int ath10k_wmi_peer_flush(struct ath10k *ar, u32 vdev_id,
		   const u8 peer_addr[ETH_ALEN], u32 tid_bitmap);
int ath10k_wmi_peer_set_param(struct ath10k *ar, u32 vdev_id,
			      const u8 *peer_addr,
			      enum wmi_peer_param param_id, u32 param_value);
int ath10k_wmi_peer_assoc(struct ath10k *ar,
			  const struct wmi_peer_assoc_complete_arg *arg);
int ath10k_wmi_set_psmode(struct ath10k *ar, u32 vdev_id,
			  enum wmi_sta_ps_mode psmode);
int ath10k_wmi_set_sta_ps_param(struct ath10k *ar, u32 vdev_id,
				enum wmi_sta_powersave_param param_id,
				u32 value);
int ath10k_wmi_set_ap_ps_param(struct ath10k *ar, u32 vdev_id, const u8 *mac,
			       enum wmi_ap_ps_peer_param param_id, u32 value);
int ath10k_wmi_scan_chan_list(struct ath10k *ar,
			      const struct wmi_scan_chan_list_arg *arg);
int ath10k_wmi_beacon_send_nowait(struct ath10k *ar,
				  const struct wmi_bcn_tx_arg *arg);
int ath10k_wmi_pdev_set_wmm_params(struct ath10k *ar,
			const struct wmi_pdev_set_wmm_params_arg *arg);
int ath10k_wmi_request_stats(struct ath10k *ar, enum wmi_stats_id stats_id);
int ath10k_wmi_force_fw_hang(struct ath10k *ar,
			     enum wmi_force_fw_hang_type type, u32 delay_ms);
int ath10k_wmi_mgmt_tx(struct ath10k *ar, struct sk_buff *skb);
int ath10k_wmi_dbglog_cfg(struct ath10k *ar, u32 module_enable);

#endif /* _WMI_H_ */
