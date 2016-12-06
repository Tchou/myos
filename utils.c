static unsigned char STR_BUFF[12] = "...........\0";

char * itoa(unsigned int c) {
	unsigned int i = 10;
	if (c == 0) {
		STR_BUFF[i] = '0';
		i--;
	} else {
		while (c != 0 && i >= 0) {
			STR_BUFF[i] = ((unsigned char)(c % 10) + '0');
			i--;
			c = c / 10;
		}
	}
	return STR_BUFF+i+1;
}
