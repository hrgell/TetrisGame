#include "stdafx.h"
#include "AllegroResources.h"

using namespace std;

AllegroResources::AllegroResources(int afps)
	:
	developing(false),
	paused(false),
	monitor(),
	scope_base(
		[]() { return al_init(); },
		al_uninstall_system,
		"Failed to initialize Allegro."),
	width(300),
	height(200),
	display(nullptr),
	scope_display(
		[&]() { return ((display = al_create_display(width, height)) != nullptr); },
		[&]() { al_destroy_display(display); },
		"Failed to initialize the allegro display."),
	scope_font(
		al_init_font_addon,
		al_shutdown_font_addon,
		"Failed to initialize the allegro font addon."),
	scope_ttffont(
		al_init_ttf_addon,
		al_shutdown_ttf_addon,
		"Failed to initialize the allegro true type font addon."),
	fonts(),
	scope_primitives(
		al_init_primitives_addon,
		al_shutdown_primitives_addon,
		"Failed to initialize the allegro primitives addon."),
	scope_keyboard(
		al_install_keyboard,
		al_uninstall_keyboard,
		"Failed to initialize the allegro keyboard."),
	scope_mouse(
		al_install_mouse,
		al_uninstall_mouse,
		"Failed to initialize the allegro mouse."),
	scope_audio(
		al_install_audio,
		al_uninstall_audio,
		"Failed to initialize the allegro audio."),
	scope_acodec(
		al_init_acodec_addon,
		// []() { return; }, // TODO how to free the audio codec
		"Failed to initialize the allegro acodec addon."),
	scope_image(
		al_init_image_addon,
		al_shutdown_image_addon,
		"Failed to initialize the allegro image addon."),
	fps(afps < 1 ? 1 : afps),
	timer(nullptr),
	scope_timer(
		[&]() { return ((timer = al_create_timer(1.0 / fps)) != nullptr); },
		[&]() { al_destroy_timer(timer); },
		"Failed to create allegro timer."),
	event_queue(nullptr),
	scope_event_queue(
		[&]() { return ((event_queue = al_create_event_queue()) != nullptr); },
		[&]() { al_destroy_event_queue(event_queue); },
		"Failed to create allegro event queue."),
	scope_event_kbd(
		[&]() { al_register_event_source(event_queue, al_get_keyboard_event_source()); },
		[&]() { al_unregister_event_source(event_queue, al_get_keyboard_event_source()); }),


	scope_event_mouse(
		[&]() { al_register_event_source(event_queue, al_get_mouse_event_source()); },
		[&]() { al_unregister_event_source(event_queue, al_get_mouse_event_source()); }),


	scope_event_timer(
		[&]() { al_register_event_source(event_queue, al_get_timer_event_source(timer)); },
		[&]() { al_unregister_event_source(event_queue, al_get_timer_event_source(timer)); }),
	scope_event_display(
		[&]() { al_register_event_source(event_queue, al_get_display_event_source(display)); },
		[&]() { al_unregister_event_source(event_queue, al_get_display_event_source(display)); }),
	audio(),
	sprite1("C:\\Users\\Anonymous User\\Desktop\\sprite1.png") // TODO Include the png as a resource in the binary, and extract it to a known path.
{
}

AllegroResources::~AllegroResources() {
}
