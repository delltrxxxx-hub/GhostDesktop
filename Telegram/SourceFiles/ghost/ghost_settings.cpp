/*
This file is part of Ghost Desktop,
an unofficial Telegram client based on Telegram Desktop.

Licensed under the GPLv3 with OpenSSL exception.
*/
#include "ghost/ghost_settings.h"

#include "lang/lang_keys.h"
#include "settings.h"

#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

namespace Ghost {
namespace {

[[nodiscard]] QString FilePath() {
	return cWorkingDir() + u"tdata/ghost_settings.json"_q;
}

struct State {
	Settings settings;
	rpl::event_stream<> changes;
	bool loaded = false;
};

[[nodiscard]] State &Instance() {
	static auto result = State();
	return result;
}

void Load(State &state) {
	state.loaded = true;
	auto file = QFile(FilePath());
	if (!file.open(QIODevice::ReadOnly)) {
		return;
	}
	const auto object = QJsonDocument::fromJson(file.readAll()).object();
	auto &s = state.settings;
	s.hideOnline = object.value(u"hideOnline"_q).toBool(s.hideOnline);
	s.hideRead = object.value(u"hideRead"_q).toBool(s.hideRead);
	s.hideTyping = object.value(u"hideTyping"_q).toBool(s.hideTyping);
	s.hideStoryViews = object.value(u"hideStoryViews"_q).toBool(
		s.hideStoryViews);
}

void Save(const Settings &s) {
	auto object = QJsonObject();
	object.insert(u"hideOnline"_q, s.hideOnline);
	object.insert(u"hideRead"_q, s.hideRead);
	object.insert(u"hideTyping"_q, s.hideTyping);
	object.insert(u"hideStoryViews"_q, s.hideStoryViews);
	auto file = QFile(FilePath());
	if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		file.write(QJsonDocument(object).toJson(QJsonDocument::Compact));
	}
}

} // namespace

const Settings &Current() {
	auto &state = Instance();
	if (!state.loaded) {
		Load(state);
	}
	return state.settings;
}

void Update(Fn<void(Settings&)> change) {
	auto &state = Instance();
	if (!state.loaded) {
		Load(state);
	}
	change(state.settings);
	Save(state.settings);
	state.changes.fire({});
}

rpl::producer<> Changes() {
	return Instance().changes.events();
}

QString Tr(const char *en, const char *ru) {
	return Lang::Id().startsWith(u"ru"_q)
		? QString::fromUtf8(ru)
		: QString::fromUtf8(en);
}

} // namespace Ghost
