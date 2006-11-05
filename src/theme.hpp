/* $Id$ */
/*
   Copyright (C) 2003-5 by David White <davidnwhite@verizon.net>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef THEME_HPP_INCLUDED
#define THEME_HPP_INCLUDED

#include <map>
#include <string>
#include <vector>

class config;

#include "SDL.h"

typedef struct { size_t x1,y1,x2,y2; } _rect;

class theme
{

	class object
	{
	public:
		object();
		object(const config& cfg);

		SDL_Rect& location(const SDL_Rect& screen) const;
		const SDL_Rect& get_location(void) const;
		const std::string& get_id() const;

		//this supports relocating of theme elements ingame.
		//It is needed for [change] tags in theme WML.
		void modify_location(const _rect rect);
		void modify_location(std::string rect_str, SDL_Rect rect_ref);

		//all on-screen objects have 'anchoring' in the x and y dimensions
		//'fixed' means that they have fixed co-ordinates and don't move
		//'top anchored' means they are anchored to the top (or left) side of
		//the screen - the top (or left) edge stays a constant distance from
		//the top of the screen
		//'bottom anchored' is the inverse of top anchored
		//'proportional' means the location and dimensions change proportionally
		//to the screen size
		enum ANCHORING { FIXED, TOP_ANCHORED, PROPORTIONAL, BOTTOM_ANCHORED };

	private:
		bool location_modified_;
		std::string id_;
		SDL_Rect loc_;
		mutable SDL_Rect relative_loc_;
		mutable SDL_Rect last_screen_;

		ANCHORING xanchor_, yanchor_;

		static ANCHORING read_anchor(const std::string& str);
	};

public:

	class label : public object
	{
	public:
		label();
		explicit label(const config& cfg);

		using object::location;

		const std::string& text() const;
		const std::string& icon() const;

		bool empty() const;

		size_t font_size() const;
		Uint32 font_rgb() const;
	        bool font_rgb_set() const;
	private:
		std::string text_, icon_;
		size_t font_;
	        bool font_rgb_set_;
		Uint32 font_rgb_;
	};

	class status_item : public object
	{
	public:

		explicit status_item(const config& cfg);

		using object::location;

		const std::string& prefix() const;
		const std::string& postfix() const;

		//if the item has a label associated with it, show where the label is
		const label* get_label() const;

		size_t font_size() const;
                Uint32 font_rgb() const;
	        bool font_rgb_set() const;

	private:
		std::string prefix_, postfix_;
		label label_;
		size_t font_;
	        bool font_rgb_set_;
		Uint32 font_rgb_;
	};

	class panel : public object
	{
	public:
		explicit panel(const config& cfg);

		using object::location;

		const std::string& image() const;

	private:
		std::string image_;
	};

	class menu : public object
	{
	public:
		menu();
		explicit menu(const config& cfg);

		using object::location;

		bool is_context() const;

		const std::string& title() const;

		const std::string& tooltip() const;

		const std::string& type() const;

		const std::string& image() const;

		const std::vector<std::string>& items() const;
	private:
		bool context_;
		std::string title_, tooltip_, image_, type_;
		std::vector<std::string> items_;
	};

	explicit theme(const config& cfg, const SDL_Rect& screen);
	bool set_resolution(const SDL_Rect& screen);
	void modify(const config* cfg);

	const std::vector<panel>& panels() const;
	const std::vector<label>& labels() const;
	const std::vector<menu>& menus() const;

	const menu* context_menu() const;

	const status_item* get_status_item(const std::string& item) const;

	const SDL_Rect& main_map_location(const SDL_Rect& screen) const;
	const SDL_Rect& mini_map_location(const SDL_Rect& screen) const;
	const SDL_Rect& unit_image_location(const SDL_Rect& screen) const;

    static void set_known_themes(const config* cfg);
    static std::vector<std::string> get_known_themes();

private:
	theme::object& find_element(std::string id);
	theme::object& add_object(const config& cfg);
	void remove_object(std::string id);
	void set_object_location(theme::object& element, std::string rect_str, std::string ref_id);

	static std::map<std::string, config> known_themes;
	std::string cur_theme;
	config cfg_;
	std::vector<panel> panels_;
	std::vector<label> labels_;
	std::vector<menu> menus_;

	menu context_;

	std::map<std::string,status_item> status_;

	object main_map_, mini_map_, unit_image_;
};

#endif
