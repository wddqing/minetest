/*
Minetest
Copyright (C) 2010-2016 celeron55, Perttu Ahola <celeron55@gmail.com>
Copyright (C) 2014-2016 nerzhul, Loic Blot <loic.blot@unix-experience.fr>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef REMOTEPLAYER_HEADER
#define REMOTEPLAYER_HEADER

#include "player.h"

class PlayerSAO;

enum RemotePlayerChatResult {
	RPLAYER_CHATRESULT_OK,
	RPLAYER_CHATRESULT_FLOODING,
	RPLAYER_CHATRESULT_KICK,
};
/*
	Player on the server
*/
class RemotePlayer : public Player
{
public:
	RemotePlayer(const char *name, IItemDefManager *idef);
	virtual ~RemotePlayer() {}

	void save(std::string savedir, IGameDef *gamedef);
	void deSerialize(std::istream &is, const std::string &playername);

	PlayerSAO *getPlayerSAO() { return m_sao; }
	void setPlayerSAO(PlayerSAO *sao) { m_sao = sao; }
	void setPosition(const v3f &position);

	const RemotePlayerChatResult canSendChatMessage();

	void setHotbarItemcount(s32 hotbar_itemcount)
	{
		hud_hotbar_itemcount = hotbar_itemcount;
	}

	s32 getHotbarItemcount() const { return hud_hotbar_itemcount; }

	void overrideDayNightRatio(bool do_override, float ratio)
	{
		m_day_night_ratio_do_override = do_override;
		m_day_night_ratio = ratio;
	}

	void getDayNightRatio(bool *do_override, float *ratio)
	{
		*do_override = m_day_night_ratio_do_override;
		*ratio = m_day_night_ratio;
	}

	// Use a function, if isDead can be defined by other conditions
	bool isDead() const { return hp == 0; }

	void setHotbarImage(const std::string &name)
	{
		hud_hotbar_image = name;
	}

	std::string getHotbarImage() const
	{
		return hud_hotbar_image;
	}

	void setHotbarSelectedImage(const std::string &name)
	{
		hud_hotbar_selected_image = name;
	}

	const std::string &getHotbarSelectedImage() const
	{
		return hud_hotbar_selected_image;
	}

	// Deprecated
	f32 getRadPitchDep() const { return -1.0 * m_pitch * core::DEGTORAD; }

	// Deprecated
	f32 getRadYawDep() const { return (m_yaw + 90.) * core::DEGTORAD; }

	void setSky(const video::SColor &bgcolor, const std::string &type,
				const std::vector<std::string> &params)
	{
		m_sky_bgcolor = bgcolor;
		m_sky_type = type;
		m_sky_params = params;
	}

	void getSky(video::SColor *bgcolor, std::string *type,
				std::vector<std::string> *params)
	{
		*bgcolor = m_sky_bgcolor;
		*type = m_sky_type;
		*params = m_sky_params;
	}

	bool checkModified() const { return m_dirty || inventory.checkModified(); }

	void setModified(const bool x)
	{
		m_dirty = x;
		if (!x)
			inventory.setModified(x);
	}

	virtual void setBreath(u16 breath)
	{
		if (breath != m_breath)
			m_dirty = true;
		Player::setBreath(breath);
	}

	virtual void setPitch(f32 pitch)
	{
		if (pitch != m_pitch)
			m_dirty = true;
		Player::setPitch(pitch);
	}

	virtual void setYaw(f32 yaw)
	{
		if (yaw != m_yaw)
			m_dirty = true;
		Player::setYaw(yaw);
	}

	u16 protocol_version;
private:
	/*
		serialize() writes a bunch of text that can contain
		any characters except a '\0', and such an ending that
		deSerialize stops reading exactly at the right point.
	*/
	void serialize(std::ostream &os);

	PlayerSAO *m_sao;
	bool m_dirty;

	static bool m_setting_cache_loaded;
	static float m_setting_chat_message_limit_per_10sec;
	static u16 m_setting_chat_message_limit_trigger_kick;

	u32 m_last_chat_message_sent;
	float m_chat_message_allowance;
	u16 m_message_rate_overhead;

	bool m_day_night_ratio_do_override;
	float m_day_night_ratio;
	std::string hud_hotbar_image;
	std::string hud_hotbar_selected_image;

	std::string m_sky_type;
	video::SColor m_sky_bgcolor;
	std::vector<std::string> m_sky_params;
};

#endif
