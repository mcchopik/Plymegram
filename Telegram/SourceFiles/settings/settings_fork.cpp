/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "settings/settings_fork.h"

#include "base/options.h"
#include "core/application.h"
#include "data/data_auto_download.h"
#include "data/data_session.h"
#include "info/profile/info_profile_actions.h"
#include "lang/lang_keys.h"
#include "main/main_session.h"
#include "main/main_session_settings.h"
#include "settings/settings_common.h"
#include "storage/localstorage.h"
#include "ui/boxes/choose_font_box.h"
#include "ui/power_saving.h"
#include "ui/style/style_core_font.h"
#include "ui/vertical_list.h"
#include "ui/widgets/buttons.h"
#include "styles/style_layers.h"
#include "styles/style_settings.h"

#include <cstring>

#include <QtGui/QColor>
#include <QtGui/QImage>

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
	AddToggle(container, tr::lng_settings_fork_show_dc(), Info::Profile::kOptionShowDcBelowAbout);

	const auto settings = &Core::App().settings();
	const auto family = container->lifetime().make_state<
		rpl::variable<QString>
	>(settings->customFontFamily());
	const auto fontLabel = family->value() | rpl::map([](const QString &value) {
		return value.isEmpty()
			? tr::lng_font_default(tr::now)
			: (value == style::SystemFontTag())
			? tr::lng_font_system(tr::now)
			: value;
	});
	const auto fontButton = AddButtonWithLabel(
		container,
		rpl::single(u"Custom font"_q),
		fontLabel,
		st::settingsButtonNoIcon);
	fontButton->setClickedCallback([=] {
		const auto save = [=](QString chosen) {
			*family = chosen;
			settings->setCustomFontFamily(chosen);
			Local::writeSettings();
			Core::Restart();
		};
		const auto generateBg = [] {
			auto image = QImage(
				QSize(st::boxWidth, st::boxWidth),
				QImage::Format_ARGB32_Premultiplied);
			image.fill(QColor(235, 235, 235));
			return image;
		};
		box->getDelegate()->show(Box(
			Ui::ChooseFontBox, generateBg, family->current(), save));
	});

	Ui::AddDividerText(container, tr::lng_settings_fork_privacy());
	AddToggle(container, tr::lng_settings_fork_no_read(), "plymegram-no-read-metrics");
	AddToggle(container, tr::lng_settings_fork_no_promo(), "plymegram-no-promo-telemetry");
	AddToggle(container, tr::lng_settings_fork_no_sponsored(), "plymegram-no-sponsored-view");
	AddToggle(container, tr::lng_settings_fork_no_music(), "plymegram-no-music-listen");
	AddToggle(container, tr::lng_settings_fork_hide_phone(), "plymegram-hide-phone", [=](bool value) {
		session->settings().setPhoneNumberHidden(value);
		session->saveSettingsDelayed();
	});
}

} // namespace Settings
