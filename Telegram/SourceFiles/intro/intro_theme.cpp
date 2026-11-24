/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "intro/intro_theme.h"

#include "intro/intro_qr.h"
#include "lang/lang_keys.h"
#include "ui/widgets/buttons.h"
#include "styles/style_intro.h"
#include "window/themes/window_theme.h"

namespace Intro {
namespace details {

ThemeWidget::ThemeWidget(
	QWidget *parent,
	not_null<Main::Account*> account,
	not_null<Data*> data)
: Step(parent, account, data, true) {
	setMouseTracking(true);
	setTitleText(rpl::single(u"Choose theme"_q));
	setDescriptionText(rpl::single(u"Pick how Plymegram looks. You can change this later in Settings."_q));

	_light = Ui::CreateChild<Ui::LinkButton>(this, u"Light"_q, st::introLink);
	_light->addClickHandler([this] { selectLight(); });

	_dark = Ui::CreateChild<Ui::LinkButton>(this, u"Dark"_q, st::introLink);
	_dark->addClickHandler([this] { selectDark(); });

	show();
}

void ThemeWidget::resizeEvent(QResizeEvent *e) {
	Step::resizeEvent(e);
	const auto x = contentLeft() + st::buttonRadius;
	const auto y = errorTop() + st::introLinkTop;
	_light->moveToLeft(x, y);
	_dark->moveToLeft(x + _light->width() + st::introLinkTop, y);
}

void ThemeWidget::selectLight() {
	_darkSelected = false;
	Window::Theme::SetNightModeValue(false);
}

void ThemeWidget::selectDark() {
	_darkSelected = true;
	Window::Theme::SetNightModeValue(true);
}

void ThemeWidget::submit() {
	if (_darkSelected) {
		Window::Theme::ToggleNightMode();
	} else {
		Window::Theme::SetNightModeValue(false);
	}
	Window::Theme::KeepApplied();
	goNext<QrWidget>();
}

rpl::producer<QString> ThemeWidget::nextButtonText() const {
	return tr::lng_start_msgs();
}

void ThemeWidget::activate() {
	Step::activate();
	setInnerFocus();
}

void ThemeWidget::setInnerFocus() {
}

} // namespace details
} // namespace Intro
