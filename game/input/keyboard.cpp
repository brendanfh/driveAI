#include "keyboard.h"

#include <iostream>

KeyboardManager::KeyboardManager()
{
	for (int i = 0; i < MAX_KEY_PRESSES; i++) {
		this->keys[i] = 0;
	}

	this->keys_pressed = 0;
}

KeyboardManager::~KeyboardManager()
{
	for (int i = 0; i < MAX_KEY_PRESSES; i++) {
		this->keys[i] = 0;
	}
}

auto KeyboardManager::PressKey(int key) -> void
{
	if (this->keys_pressed >= MAX_KEY_PRESSES - 1) {
		throw "Too many key presses";
	}

	for (int i = 0; i < this->keys_pressed; i++) {
		if (this->keys[i] == key)
			return;
	}

	this->keys[this->keys_pressed] = key;
	this->keys_pressed++;
}

auto KeyboardManager::ReleaseKey(int key) -> void
{
	for (int i = 0; i < this->keys_pressed; i++) {
		if (this->keys[i] == key) {
			for (int j = i + 1; j < this->keys_pressed; j++) {
				this->keys[j - 1] = this->keys[j];
			}

			this->keys_pressed--;
			break;
		}
	}
}

auto KeyboardManager::Clear() -> void
{
	for (int i = 0; i < this->keys_pressed; i++) {
		this->keys[i] = 0;
	}

	this->keys_pressed = 0;
}

auto KeyboardManager::IsDown(/*SDL_KeyCode*/ int key) const -> bool
{
	for (int i = 0; i < this->keys_pressed; i++) {
		if (this->keys[i] == key)
			return true;
	}

	return false;
}
