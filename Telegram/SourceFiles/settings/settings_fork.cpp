/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "settings/settings_fork.h"

#include "base/options.h"
#include "data/data_auto_download.h"
#include "data/data_session.h"
#include "lang/lang_keys.h"
#include "main/main_session.h"
#include "main/main_session_settings.h"
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
constexpr auto kShowIds = "show-peer-id-below-about";

base::options::toggle CompactListOption({
	.id = kCompactList,
	.name = "Compact chat list",
	.description = "Use compact chat list rows.",
});

base::options::toggle NoAutoDownloadOption({
	.id = kNoAutoDownload,
	.name = "Disable media auto-download",
	.description = "Disable automatic media downloads.",
});

base::options::toggle StaticEmojiOption({
	.id = kStaticEmoji,
	.name = "Static emoji and stickers",
	.description = "Disable animated emoji and stickers.",
});

void AddToggle(
		not_null<Ui::VerticalLayout*> container,
		rpl::producer<QString> text,
		const char *option,
		Fn<void(bool)> apply = nullptr) {
	const auto &storedOption = base::options::lookup<bool>(option);
	const auto button = container->add(object_ptr<Ui::SettingsButton>(
		container,
		std::move(text),
		st::settingsButtonNoIcon));
	button->toggleOn(rpl::single(storedOption.value()));
	button->toggledValue() | rpl::on_next([option, apply](bool value) {
		base::options::lookup<bool>(option).set(value);
		if (apply) {
			apply(value);
		}
	}, button->lifetime());
}

} // namespace

void ForkFeaturesBox(
		not_null<Ui::GenericBox*> box,
		not_null<Main::Session*> session) {
	box->setStyle(st::layerBox);
	box->setTitle(tr::lng_settings_fork_title());
	box->setWidth(st::boxWideWidth);
	const auto container = box->verticalLayout();
	Ui::AddSubsectionTitle(container, tr::lng_settings_fork_about(), st::powerSavingSubtitlePadding);
	AddToggle(container, tr::lng_settings_fork_compact(), kCompactList);
	AddToggle(container, tr::lng_settings_fork_no_autodl(), kNoAutoDownload, [=](bool value) {
		auto &data = session->settings().autoDownload();
		for (const auto source : {
			Data::AutoDownload::Source::User,
			Data::AutoDownload::Source::Group,
			Data::AutoDownload::Source::Channel,
		}) {
			if (value) {
				SetDisabledForSource(data, source);
			} else {
				SetDefaultsForSource(data, source);
			}
		}
		session->data().photoLoadSettingsChanged();
		session->data().documentLoadSettingsChanged();
		session->saveSettingsDelayed();
	});
	AddToggle(container, tr::lng_settings_fork_static_emoji(), kStaticEmoji, [](bool value) {
		const auto flags = PowerSaving::kEmojiChat
			| PowerSaving::kEmojiReactions
			| PowerSaving::kStickersChat;
		PowerSaving::Set(value
			? (PowerSaving::Current() | flags)
			: (PowerSaving::Current() & ~flags));
	});
	AddToggle(container, tr::lng_settings_fork_show_ids(), kShowIds);
	Ui::AddDividerText(container, tr::lng_settings_fork_privacy());
	AddToggle(container, tr::lng_settings_fork_no_read(), "plymegram-no-read-metrics");
	AddToggle(container, tr::lng_settings_fork_no_promo(), "plymegram-no-promo-telemetry");
	AddToggle(container, tr::lng_settings_fork_no_sponsored(), "plymegram-no-sponsored-view");
	AddToggle(container, tr::lng_settings_fork_no_music(), "plymegram-no-music-listen");
}

} // namespace Settings
