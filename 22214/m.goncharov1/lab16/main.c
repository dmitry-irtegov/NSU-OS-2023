#include<stdio.h>
#include<termios.h>
#include<unistd.h>

// канонический режим - символы накапливаются в буфере, пока не нажмут ентер.
// некаонический режим - буфера нет, ентер нажимать не надо. 
// Не рекомендуется использовать буферизированного ввода типа scanf, get

int main() {
	
	struct termios new_settings;
	struct termios stored_settings;

	tcgetattr(0, &stored_settings);
	new_settings = stored_settings;

	new_settings.c_lflag &= (~ICANON); // неканонический режим
	new_settings.c_cc[VTIME] = 0; // Сколько децисекнуд есть чтобы ввести некст символ
	new_settings.c_cc[VMIN] = 1; // Минимальное количество введенных символов

	tcsetattr(0, TCSANOW, &new_settings);

	printf("?\n");
	char x;
	read (0, &x, 1);
	
	tcsetattr(0, TCSANOW, &stored_settings);

	return 0;
}
