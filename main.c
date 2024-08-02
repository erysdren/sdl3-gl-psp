
/*
MIT License

Copyright (c) 2024 erysdren (it/she/they)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

typedef struct app {
	SDL_Window *window;
	SDL_GLContext context;
} app_t;

int SDL_AppInit(void **appstate, int argc, char **argv)
{
	app_t *a;

	/* init sdl */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD) != 0)
		return -1;

	/* allocate app state */
	a = SDL_malloc(sizeof(app_t));
	if (a == NULL)
		return -1;

	/* create window */
	a->window = SDL_CreateWindow("SDL App", 480, 272, SDL_WINDOW_OPENGL);
	if (a->window == NULL)
		return -1;

	/* init gl */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	a->context = SDL_GL_CreateContext(a->window);
	if (a->context == NULL)
		return -1;

	/* set swap interval */
	SDL_GL_SetSwapInterval(1);

	/* return app state */
	*appstate = (void *)a;

	return 0;
}

void SDL_AppQuit(void *appstate)
{
	app_t *a = (app_t *)appstate;
	SDL_DestroyWindow(a->window);
	SDL_GL_DestroyContext(a->context);
	SDL_free(a);
	SDL_Quit();
}

int SDL_AppIterate(void *appstate)
{
	int w, h;
	float aspect;
	app_t *a = (app_t *)appstate;

	/* get window size */
	SDL_GetWindowSizeInPixels(a->window, &w, &h);
	aspect = (float)w / (float)h;

	/* setup viewport */
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-aspect, aspect, -1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* clear screen */
	glClearColor(1, 1, 1, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	/* draw a rectangle */
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
		glVertex2f(-0.5, 0.5);
		glVertex2f(0.5, 0.5);
		glVertex2f(0.5, -0.5);
		glVertex2f(-0.5, -0.5);
	glEnd();

	/* swap buffers */
	SDL_GL_SwapWindow(a->window);
}

int SDL_AppEvent(void *appstate, const SDL_Event *event)
{
	app_t *a = (app_t *)appstate;

	switch (event->type)
	{
		case SDL_EVENT_QUIT:
			return 1;

		case SDL_EVENT_GAMEPAD_ADDED:
			SDL_OpenGamepad(event->cdevice.which);
			break;

		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
			if (event->gbutton.button == SDL_GAMEPAD_BUTTON_START)
				return 1;
			break;
	}

	return 0;
}
