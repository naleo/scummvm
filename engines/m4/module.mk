MODULE := engines/m4

MODULE_OBJS = \
	m4.o \
	console.o \
	metaengine.o \
	vars.o \
	adv_db_r/db_catalog.o \
	adv_r/adv_background.o \
	adv_r/adv_been.o \
	adv_r/adv_chk.o \
	adv_r/adv_control.o \
	adv_r/adv_file.o \
	adv_r/adv_hotspot.o \
	adv_r/adv_interface.o \
	adv_r/adv_inv.o \
	adv_r/adv_player.o \
	adv_r/adv_rails.o \
	adv_r/adv_scale.o \
	adv_r/adv_trigger.o \
	adv_r/adv_walk.o \
	adv_r/chunk_ops.o \
	adv_r/conv.o \
	adv_r/conv_io.o \
	adv_r/db_env.o \
	adv_r/other.o \
	core/cstring.o \
	core/errors.o \
	core/imath.o \
	core/mouse.o \
	core/param.o \
	core/rooms.o \
	core/term.o \
	dbg/dbg_wscript.o \
	fileio/extensions.o \
	fileio/fileio.o \
	fileio/fstream.o \
	fileio/info.o \
	fileio/sys_file.o \
	graphics/gr_buff.o \
	graphics/gr_draw.o \
	graphics/gr_color.o \
	graphics/gr_font.o \
	graphics/gr_line.o \
	graphics/gr_pal.o \
	graphics/gr_series.o \
	graphics/gr_sprite.o \
	graphics/gr_surface.o \
	graphics/krn_pal.o \
	graphics/rend.o \
	gui/gui_buffer.o \
	gui/gui_dialog.o \
	gui/gui_item.o \
	gui/gui_mouse.o \
	gui/gui_sys.o \
	gui/gui_vmng_core.o \
	gui/gui_vmng_rectangles.o \
	gui/gui_vmng_screen.o \
	gui/hotkeys.o \
	mem/mem.o \
	mem/memman.o \
	mem/reloc.o \
	mem/res.o \
	platform/draw.o \
	platform/events.o \
	platform/timer.o \
	platform/sound/digi.o \
	platform/sound/midi.o \
	platform/tile/tile_read.o \
	wscript/ws_cruncher.o \
	wscript/ws_hal.o \
	wscript/ws_load.o \
	wscript/ws_machine.o \
	wscript/ws_timer.o \
	wscript/wscript.o \
	wscript/wst_regs.o \
	burger/core/conv.o \
	burger/core/play_break.o \
	burger/core/release_trigger.o \
	burger/core/stream_break.o \
	burger/gui/game_menu.o \
	burger/gui/gui_cheapo.o \
	burger/gui/gui_gizmo.o \
	burger/gui/interface.o \
	burger/rooms/room.o \
	burger/rooms/section.o \
	burger/rooms/section1/section1.o \
	burger/rooms/section1/room101.o \
	burger/rooms/section1/room102.o \
	burger/rooms/section1/room103.o \
	burger/rooms/section1/room104.o \
	burger/rooms/section1/room105.o \
	burger/rooms/section1/room106.o \
	burger/rooms/section1/room120.o \
	burger/rooms/section1/room133_136.o \
	burger/rooms/section1/room134_135.o \
	burger/rooms/section1/room137.o \
	burger/rooms/section1/room138.o \
	burger/rooms/section1/room139_144.o \
	burger/rooms/section1/room140_141.o \
	burger/rooms/section1/room142.o \
	burger/rooms/section1/room143.o \
	burger/rooms/section1/room145.o \
	burger/rooms/section1/room170.o \
	burger/rooms/section1/room171.o \
	burger/rooms/section1/room172.o \
	burger/rooms/section1/room173.o \
	burger/rooms/section1/room174.o \
	burger/rooms/section1/room175.o \
	burger/rooms/section1/room176.o \
	burger/rooms/section2/section2.o \
	burger/rooms/section2/room204.o \
	burger/rooms/section2/room207.o \
	burger/rooms/section3/section3.o \
	burger/rooms/section3/section3_room.o \
	burger/rooms/section3/mine.o \
	burger/rooms/section3/room301.o \
	burger/rooms/section3/room302.o \
	burger/rooms/section3/room303.o \
	burger/rooms/section3/room304.o \
	burger/rooms/section3/room305.o \
	burger/rooms/section3/room306.o \
	burger/rooms/section3/room307.o \
	burger/rooms/section3/room310.o \
	burger/rooms/section4/section4.o \
	burger/rooms/section4/section4_room.o \
	burger/rooms/section4/room401.o \
	burger/rooms/section4/room402.o \
	burger/rooms/section4/room404.o \
	burger/rooms/section4/room405.o \
	burger/rooms/section4/room406.o \
	burger/rooms/section4/room407.o \
	burger/rooms/section5/section5.o \
	burger/rooms/section5/section5_room.o \
	burger/rooms/section5/room501.o \
	burger/rooms/section5/room502.o \
	burger/rooms/section5/room503.o \
	burger/rooms/section5/room504.o \
	burger/rooms/section5/room505.o \
	burger/rooms/section5/room506.o \
	burger/rooms/section5/room507.o \
	burger/rooms/section5/room508.o \
	burger/rooms/section5/room509.o \
	burger/rooms/section5/room510.o \
	burger/rooms/section5/room511.o \
	burger/rooms/section5/room512.o \
	burger/rooms/section5/room513.o \
	burger/rooms/section6/section6.o \
	burger/rooms/section6/section6_room.o \
	burger/rooms/section6/room601.o \
	burger/rooms/section6/room602.o \
	burger/rooms/section6/room603.o \
	burger/rooms/section6/room604.o \
	burger/rooms/section6/room605.o \
	burger/rooms/section6/room606.o \
	burger/rooms/section6/room608.o \
	burger/rooms/section6/room609.o \
	burger/rooms/section6/room610.o \
	burger/rooms/section7/section7.o \
	burger/rooms/section7/room701.o \
	burger/rooms/section7/room702.o \
	burger/rooms/section7/room706.o \
	burger/rooms/section8/section8.o \
	burger/rooms/section8/room801.o \
	burger/rooms/section8/room802.o \
	burger/rooms/section9/section9.o \
	burger/rooms/section9/menu_room.o \
	burger/rooms/section9/room901.o \
	burger/rooms/section9/room902.o \
	burger/rooms/section9/room903.o \
	burger/rooms/section9/room904.o \
	burger/rooms/section9/room951.o \
	burger/rooms/section9/room971.o \
	burger/burger.o \
	burger/flags.o \
	burger/hotkeys.o \
	burger/inventory.o \
	burger/other.o \
	burger/series_player.o \
	burger/vars.o \
	burger/walker.o \
	riddle/gui/interface.o \
	riddle/rooms/room.o \
	riddle/rooms/section.o \
	riddle/rooms/section2/section2.o \
	riddle/rooms/section2/room201.o \
	riddle/rooms/section2/room202.o \
	riddle/rooms/section2/room203.o \
	riddle/rooms/section2/room204.o \
	riddle/rooms/section2/room205.o \
	riddle/rooms/section2/room207.o \
	riddle/rooms/section3/section3.o \
	riddle/rooms/section3/room301.o \
	riddle/rooms/section3/room303.o \
	riddle/rooms/section3/room304.o \
	riddle/rooms/section3/room305.o \
	riddle/rooms/section3/room309.o \
	riddle/rooms/section3/room352.o \
	riddle/rooms/section3/room354.o \
	riddle/rooms/section4/section4.o \
	riddle/rooms/section4/room401.o \
	riddle/rooms/section4/room402.o \
	riddle/rooms/section4/room403.o \
	riddle/rooms/section4/room404.o \
	riddle/rooms/section4/room405.o \
	riddle/rooms/section4/room406.o \
	riddle/rooms/section4/room407.o \
	riddle/rooms/section4/room408.o \
	riddle/rooms/section4/room409.o \
	riddle/rooms/section4/room410.o \
	riddle/rooms/section4/room413.o \
	riddle/rooms/section4/room456.o \
	riddle/rooms/section4/room493.o \
	riddle/rooms/section4/room494.o \
	riddle/rooms/section4/room495.o \
	riddle/rooms/section5/section5.o \
	riddle/rooms/section5/room501.o \
	riddle/rooms/section5/room504.o \
	riddle/rooms/section5/room506.o \
	riddle/rooms/section5/room507.o \
	riddle/rooms/section5/room508.o \
	riddle/rooms/section5/room509.o \
	riddle/rooms/section5/room510.o \
	riddle/rooms/section6/section6.o \
	riddle/rooms/section6/room603.o \
	riddle/rooms/section6/room604.o \
	riddle/rooms/section6/room605.o \
	riddle/rooms/section6/room607.o \
	riddle/rooms/section6/room608.o \
	riddle/rooms/section6/room610.o \
	riddle/rooms/section6/room615.o \
	riddle/rooms/section6/room620.o \
	riddle/rooms/section6/room621.o \
	riddle/rooms/section6/room622.o \
	riddle/rooms/section6/room623.o \
	riddle/rooms/section6/room624.o \
	riddle/rooms/section6/room625.o \
	riddle/rooms/section6/room626.o \
	riddle/rooms/section6/room627.o \
	riddle/rooms/section6/room628.o \
	riddle/rooms/section6/room629.o \
	riddle/rooms/section6/room630.o \
	riddle/rooms/section6/room631.o \
	riddle/rooms/section6/room632.o \
	riddle/rooms/section6/room633.o \
	riddle/rooms/section6/room634.o \
	riddle/rooms/section6/room635.o \
	riddle/rooms/section6/room636.o \
	riddle/rooms/section6/room637.o \
	riddle/rooms/section6/room638.o \
	riddle/rooms/section6/room640.o \
	riddle/rooms/section6/room641.o \
	riddle/rooms/section6/room642.o \
	riddle/rooms/section6/room643.o \
	riddle/rooms/section6/room644.o \
	riddle/rooms/section6/room645.o \
	riddle/rooms/section6/room646.o \
	riddle/rooms/section6/room647.o \
	riddle/rooms/section6/room648.o \
	riddle/rooms/section6/room649.o \
	riddle/rooms/section6/room650.o \
	riddle/rooms/section6/room651.o \
	riddle/rooms/section7/section7.o \
	riddle/rooms/section7/room701.o \
	riddle/rooms/section7/room702.o \
	riddle/rooms/section7/room703.o \
	riddle/rooms/section7/room704.o \
	riddle/rooms/section7/room705.o \
	riddle/rooms/section7/room706.o \
	riddle/rooms/section7/room707.o \
	riddle/rooms/section7/room709.o \
	riddle/rooms/section7/room710.o \
	riddle/rooms/section7/room711.o \
	riddle/rooms/section8/section8.o \
	riddle/rooms/section8/room801.o \
	riddle/rooms/section8/room802.o \
	riddle/rooms/section8/room803.o \
	riddle/rooms/section8/room804.o \
	riddle/rooms/section8/room805.o \
	riddle/rooms/section8/room806.o \
	riddle/rooms/section8/room807.o \
	riddle/rooms/section8/room808.o \
	riddle/rooms/section8/room809.o \
	riddle/rooms/section8/room810.o \
	riddle/rooms/section8/room811.o \
	riddle/rooms/section8/room814.o \
	riddle/rooms/section8/room818.o \
	riddle/rooms/section8/room824.o \
	riddle/rooms/section8/room834.o \
	riddle/rooms/section8/room844.o \
	riddle/rooms/section8/room850.o \
	riddle/rooms/section8/room860.o \
	riddle/rooms/section8/room861.o \
	riddle/rooms/section9/section9.o \
	riddle/rooms/section9/section9_room.o \
	riddle/rooms/section9/room901.o \
	riddle/rooms/section9/room902.o \
	riddle/rooms/section9/room903.o \
	riddle/rooms/section9/room905.o \
	riddle/rooms/section9/room907.o \
	riddle/rooms/section9/room908.o \
	riddle/rooms/section9/room917.o \
	riddle/rooms/section9/room918.o \
	riddle/rooms/section9/room996.o \
	riddle/riddle.o \
	riddle/flags.o \
	riddle/hotkeys.o \
	riddle/inventory.o \
	riddle/vars.o \
	riddle/walker.o


# This module can be built as a plugin
ifeq ($(ENABLE_M4), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
