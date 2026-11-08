/*
This file is part of Ghost Desktop,
an unofficial Telegram client based on Telegram Desktop.

Licensed under the GPLv3 with OpenSSL exception.
*/
#pragma once

namespace Ghost {

// Privacy switches of the Ghost mode. All of them only stop this client
// from reporting your own activity to the server; nothing else changes.
struct Settings {
	bool hideOnline = true; // Always report "offline".
	bool hideRead = true; // Do not send read receipts.
	bool hideTyping = true; // Do not send "typing..." and similar actions.
	bool hideStoryViews = true; // Do not mark stories as viewed.
};

[[nodiscard]] const Settings &Current();
void Update(Fn<void(Settings&)> change);

[[nodiscard]] rpl::producer<> Changes();

// Picks the Russian or English string by the current interface language.
[[nodiscard]] QString Tr(const char *en, const char *ru);

} // namespace Ghost
