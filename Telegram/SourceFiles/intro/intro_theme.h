/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include "intro/intro_step.h"

namespace Ui {
class LinkButton;
} // namespace Ui

namespace Intro {
namespace details {

class ThemeWidget : public Step {
public:
	ThemeWidget(
		QWidget *parent,
		not_null<Main::Account*> account,
		not_null<Data*> data);

	void submit() override;
	rpl::producer<QString> nextButtonText() const override;
	void activate() override;
	void setInnerFocus() override;

protected:
	void resizeEvent(QResizeEvent *e) override;

private:
	void selectLight();
	void selectDark();

	Ui::LinkButton *_light = nullptr;
	Ui::LinkButton *_dark = nullptr;
	bool _darkSelected = false;

};

} // namespace details
} // namespace Intro
