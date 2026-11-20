/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include "base/basic_types.h"

class UserData;

namespace Ui {
class FlatLabel;
class PopupMenu;
} // namespace Ui

namespace Info {
namespace Profile {

[[nodiscard]] bool IsCollectiblePhone(not_null<UserData*> user);

void AddPhoneMenu(not_null<Ui::PopupMenu*> menu, not_null<UserData*> user);
void AddPhoneSpoilerMenu(
	not_null<Ui::PopupMenu*> menu,
	not_null<UserData*> user);

void TogglePhoneSpoiler(not_null<UserData*> user);
[[nodiscard]] bool CopyAndHidePhoneSpoiler(not_null<UserData*> user);
void SetupPhoneSpoilerClick(
	not_null<Ui::FlatLabel*> label,
	not_null<UserData*> user,
	Fn<void()> copied = {});

} // namespace Profile
} // namespace Info
