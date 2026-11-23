/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "settings/settings_fork.h"

#include "base/options.h"
#include "lang/lang_keys.h"
#include "ui/power_saving.h"
#include "ui/vertical_list.h"
#include "ui/widgets/buttons.h"
#include "styles/style_layers.h"
#include "styles/style_settings.h"

#include <cstring>

namespace Settings {
namespace {

constexpr auto kCompactList = "plymegram-compact-list";
constexpr auto kNoAutoDownload = "plymegram-no-autodownload";
constexpr auto kStaticEmoji = "plymegram-static-emoji";
constexpr auto kShowIds = "plymegram-show-ids";

void AddToggle(
		not_null<Ui::VerticalLayout*> container,
		rpl::producer<QString> text,
		const char *option) {
	const auto &options = base::options::lookup<bool>(option);
	const auto button = container->add(object_ptr<Ui::SettingsButton>(
		container,
		std::move(text),
		st::settingsButtonNoIcon));
	button->toggleOn(rpl::single(options.value()));
	button->toggledValue() | rpl::on_next([option](bool value) {
		base::options::lookup<bool>(option).set(value);
		if (std::strcmp(option, kStaticEmoji) == 0) {
			PowerSaving::Set(value
				? (PowerSaving::Current() | PowerSaving::kEmojiChat | PowerSaving::kEmojiReactions | PowerSaving::kStickersChat)
				: (PowerSaving::Current() & ~(PowerSaving::kEmojiChat | PowerSaving::kEmojiReactions | PowerSaving::kStickersChat)));
		}
	}, button->lifetime());
}

} // namespace

void ForkFeaturesBox(not_null<Ui::GenericBox*> box) {
	box->setStyle(st::layerBox);
	box->setTitle(tr::lng_settings_fork_title());
	box->setWidth(st::boxWideWidth);
	const auto container = box->verticalLayout();
	Ui::AddSubsectionTitle(container, tr::lng_settings_fork_about(), st::powerSavingSubtitlePadding);
	AddToggle(container, tr::lng_settings_fork_compact(), kCompactList);
	AddToggle(container, tr::lng_settings_fork_no_autodl(), kNoAutoDownload);
	AddToggle(container, tr::lng_settings_fork_static_emoji(), kStaticEmoji);
	AddToggle(container, tr::lng_settings_fork_show_ids(), kShowIds);
}

} // namespace Settings
