#include<stdio.h>
#include<termios.h>
#include<unistd.h>

// ������������ ����� - ������� ������������� � ������, ���� �� ������ �����.
// ������������� ����� - ������ ���, ����� �������� �� ����. 
// �� ������������� ������������ ����������������� ����� ���� scanf, get

int main() {
	
	struct termios new_settings;
	struct termios stored_settings;

	tcgetattr(0, &stored_settings);
	new_settings = stored_settings;

	new_settings.c_lflag &= (~ICANON); // �������������� �����
	new_settings.c_cc[VTIME] = 0; // ������� ���������� ���� ����� ������ ����� ������
	new_settings.c_cc[VMIN] = 1; // ����������� ���������� ��������� ��������

	tcsetattr(0, TCSANOW, &new_settings);

	printf("?\n");
	char x;
	read (0, &x, 1);
	
	tcsetattr(0, TCSANOW, &stored_settings);

	return 0;
}
