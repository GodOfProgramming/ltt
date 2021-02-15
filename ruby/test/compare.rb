#!/usr/bin/env ruby

a = [
  :test_direct_default,
  :test_direct_upgrade,
  :test_network_next,
  :test_fallback_to_direct,
  :test_disable_network_next_on_server,
  :test_disable_network_next_on_client,
  :test_server_under_load,
  :test_reconnect_direct,
  :test_reconnect_next,
  :test_connect_to_another_server_direct,
  :test_connect_to_another_server_next,
  :test_route_switching,
  :test_on_off,
  :test_on_on_off,
  :test_multipath,
  :test_multipath_next_packet_loss,
  :test_multipath_fallback_to_direct,
  :test_uncommitted,
  :test_uncommitted_to_committed,
  :test_user_flags,
  :test_packet_loss_direct,
  :test_packet_loss_next,
  :test_idempotent
]

b = [
  :test_direct_raw,
  :test_direct_upgraded,
  :test_network_next_route,
  :test_fallback_to_direct_backend,
  :test_fallback_to_direct_client_side,
  :test_fallback_to_direct_server_restart,
  :test_disable_on_server,
  :test_disable_on_client,
  :test_route_switching,
  :test_on_off,
  :test_on_on_off,
  :test_reconnect_direct,
  :test_reconnect_direct_no_upgrade,
  :test_reconnect_next,
  :test_connect_to_another_server_direct,
  :test_connect_to_another_server_next,
  :test_multipath,
  :test_multipath_next_packet_loss,
  :test_multipath_fallback_to_direct,
  :test_uncommitted,
  :test_uncommitted_to_committed,
  :test_user_flags,
  :test_packet_loss_direct,
  :test_packet_loss_next,
  :test_server_under_load,
  :test_session_update_retry,
  :test_bandwidth_over_limit,
  :test_packet_loss,
  :test_bandwidth,
  :test_jitter,
  :test_tags,
  :test_direct_stats,
  :test_next_stats,
  :test_report_session,
  :test_client_ping_timed_out
]

for item in a do
  if !b.include?(item)
    puts item
  end
end

