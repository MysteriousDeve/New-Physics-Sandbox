#pragma once

#include "../Include.h"
#include "InteractionHandler.h"

vector<InteractionHandler*> eventHandler;

void OnScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	for (auto& h : eventHandler)
	{
		if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) h->OnScroll(window, xoffset, yoffset);
	}
}