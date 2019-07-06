#include "app.h"

class renderer :public application
{
public:
	void render(double currentTime)
	{
		static const GLfloat red[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, red);
	}
};

DECLARE_MAIN(renderer);