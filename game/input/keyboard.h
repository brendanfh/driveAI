#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#define MAX_KEY_PRESSES 32

class KeyboardManager {
private:
	int keys[MAX_KEY_PRESSES];
	int keys_pressed;

public:	
	KeyboardManager();
	~KeyboardManager();

	auto PressKey(int key) -> void;
	auto ReleaseKey(int key) -> void;
	auto Clear() -> void;

	[[nodiscard]] auto IsDown(/*SDL_KeyCode*/ int key) const -> bool;
};

#endif