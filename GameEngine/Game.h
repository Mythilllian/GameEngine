#pragma once

#include "Window.h"
#include "Objects.h"
#include <windows.h>
#include <gdiplus.h>
#include <list>
#include <chrono>

class Game {
public:
	Color backgroundColor = Color(255, 255, 255);
	std::list<Object*> objects = {};
	Game(int width, int height, const char* name) {
		window = CreateCustomWindow(width, height, name, "Game");
	}

	void AddObject(Object* object) {
		objects.push_back(object);
	}

	void Start() {
		using clock = std::chrono::high_resolution_clock;

		auto lastTime = clock::now();
		while (true) {  // Main loop
			// Calculates deltaTime
			auto currentTime = clock::now();
			std::chrono::duration<float> deltaTime = (currentTime - lastTime)*500.0;
			lastTime = currentTime;

			// deltaTime is in seconds
			Update(deltaTime.count());
		}
	}
private:
	HWND window;

	void Update(float deltaTime)
	{
		HDC hdc = GetDC(window);

		//Sets background color
		RECT rc; GetClientRect(window, &rc);
		HBRUSH brush = CreateSolidBrush(RGB(backgroundColor.r, backgroundColor.g, backgroundColor.b));
		FillRect(hdc, &rc, brush);
		DeleteObject(brush);
		
		for (Object* object : objects) {
			object->Update(hdc,deltaTime);
		}
		ReleaseDC(window, hdc);
	}
};