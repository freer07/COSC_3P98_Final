// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

	/*glutInitWindowSize(global.w, global.h);
	glutCreateWindow("SIMPLE DISPLAY");
	glShadeModel(GL_SMOOTH);
	glutDisplayFunc(display_image);
	glutKeyboardFunc(keyboard);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, global.w, 0, global.h, 0, 1);

	init_menu();
	show_keys();*/

	glutMainLoop();

	return 0;
}
