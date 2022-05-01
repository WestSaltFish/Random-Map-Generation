---
wordpress_id: 195
title: Increase Internet connection speed in Windows XP
wordpress_url: http://anilwadghule.com/blog/?p=195
layout: post
published: false
---
This tip is designed for increased BROADBAND speed in Windows XP, but it should work for 56k modems too.<br /><br />QoS Packet Scheduler is a method of network bandwidth management that can monitor the importance of data packets and depending upon the priority of the packet, give it higher or lower priority or bandwidth levels. It's not very useful unless you're using apps which are QoS-aware or running a server, so you can gain some network overhead back by turning it off.<br /><strong>Note:</strong> This following tip will not work on XP Home Edition.<br /><br />1. Make sure you're logged on as actually "Administrator". Do not log on with any account that just has administrator privileges.<br />2. Start &gt; Run &gt; type gpedit.msc (not available in home version).<br />3. Expand the Local Computer Policy branch.<br />4. Expand the Administrative Templates branch.<br />5. Expand the Network branch.<br />6. Highlight the "QoS Packet Scheduler" in left window.<br />7. In right window double click the "limit reservable bandwidth" setting.<br />8. On setting tab check the ENABLED item.<br />9. Where it says "Bandwidth limit %" change it to read 0 (ZERO).<br />10. Close gpedit.msc.<br /><br />Effect is immediate on some systems, some need to re-boot.<br /><br />Thanks Santosh for the tip
