#include <iostream>
#include "BMP.h"

void main()
{
	ifstream in("in.txt");
	char st1[200000]="";
	in.getline(st1, 200000);
	setlocale(LC_ALL, "Russian");
	coder("test1.bmp", "outputtt.bmp", st1);//имя картинки входной, имя картинки выходной, сообщение
	decoder("test1.bmp", "outputtt.bmp");//имя картинки входной, имя картинки выходной
	system("pause");
	in.close();
	// return 0;
}

void readParams(ifstream &os) {
	os.read(reinterpret_cast<char*>(settings.signature), sizeof(settings.signature));
	os.read(reinterpret_cast<char*>(&settings.fileSize), sizeof(settings.fileSize));
	os.read(reinterpret_cast<char*>(&settings.reserved), sizeof(settings.reserved));
	os.read(reinterpret_cast<char*>(&settings.offset), sizeof(settings.offset));
	os.read(reinterpret_cast<char*>(&settings.headerSize), sizeof(settings.headerSize));
	os.read(reinterpret_cast<char*>(settings.dimensions), sizeof(settings.dimensions));
	settings.imgHeight = settings.dimensions[1];
	settings.imgWidth = settings.dimensions[0];
	os.read(reinterpret_cast<char*>(&settings.colorPlanes), sizeof(settings.colorPlanes));
	os.read(reinterpret_cast<char*>(&settings.bpp), sizeof(settings.bpp));
	os.read(reinterpret_cast<char*>(&settings.compression), sizeof(settings.compression));
	os.read(reinterpret_cast<char*>(&settings.imgSize), sizeof(settings.imgSize));
	os.read(reinterpret_cast<char*>(settings.resolution), sizeof(settings.resolution));
	os.read(reinterpret_cast<char*>(&settings.pltColors), sizeof(settings.pltColors));
	os.read(reinterpret_cast<char*>(&settings.impColors), sizeof(settings.impColors));
	/*вывод информации об изображении
	cout << "signature =" << settings.signature << endl;
	cout << "fileSize =" << settings.fileSize << endl;
	cout << "reserved =" << settings.reserved << endl;
	cout << "headerSize =" << settings.headerSize << endl;
	cout << "dimensions =" << settings.dimensions[0] << " " << settings.dimensions[1] << endl;
	cout << "colorPlanes =" << settings.colorPlanes << endl;
	cout << "bpp =" << settings.bpp << endl;
	cout << "compression =" << settings.compression << endl;
	cout << "imgSize =" << settings.imgSize << endl;
	cout << "resolution =" << settings.resolution[0] << " " << settings.resolution[1] << endl;
	cout << "pltColors =" << settings.pltColors << endl;
	cout << "impColors =" << settings.impColors << endl;
	*/
	settings.lineImgSize = settings.imgHeight * settings.imgWidth * color;
}

void readIMG(ifstream &os) {
	//считывание картинки и перенос в 1-мерный массив 
	IMG = new unsigned char[settings.lineImgSize];
	char r = 0, g = 0, b = 0, x = 0;
	for (int i = 0; i < settings.lineImgSize; i++) {
		IMG[i] = 0;
	}
	for (int i = 0; i < settings.lineImgSize; i += color) {
		os >> b;
		os >> g;
		os >> r;
		if (color == 4)
			os >> x;
		IMG[i] = b;
		IMG[i + 1] = g;
		IMG[i + 2] = r;
		if (color == 4)
			IMG[i + 3] = x;

	}
}

void writeParams(ofstream &os) {
	os.write(reinterpret_cast<char*>(settings.signature), sizeof(settings.signature));
	os.write(reinterpret_cast<char*>(&settings.fileSize), sizeof(settings.fileSize));
	os.write(reinterpret_cast<char*>(&settings.reserved), sizeof(settings.reserved));
	os.write(reinterpret_cast<char*>(&settings.offset), sizeof(settings.offset));
	os.write(reinterpret_cast<char*>(&settings.headerSize), sizeof(settings.headerSize));
	os.write(reinterpret_cast<char*>(settings.dimensions), sizeof(settings.dimensions));
	os.write(reinterpret_cast<char*>(&settings.colorPlanes), sizeof(settings.colorPlanes));
	os.write(reinterpret_cast<char*>(&settings.bpp), sizeof(settings.bpp));
	os.write(reinterpret_cast<char*>(&settings.compression), sizeof(settings.compression));
	os.write(reinterpret_cast<char*>(&settings.imgSize), sizeof(settings.imgSize));
	os.write(reinterpret_cast<char*>(settings.resolution), sizeof(settings.resolution));
	os.write(reinterpret_cast<char*>(&settings.pltColors), sizeof(settings.pltColors));
	os.write(reinterpret_cast<char*>(&settings.impColors), sizeof(settings.impColors));
}

void writeIMG(ofstream &os) {
	//запись картинки в файл
	for (int i = 0; i < settings.lineImgSize; i += color) {
		os << IMG[i];
		os << IMG[i + 1];
		os << IMG[i + 2];
		if (color == 4)
			os << IMG[i + 3];
	}
}

void coder(const char* baseImgName, const char* newImgName, const char* letter) {
	//чтение картинки исходной
	std::ifstream in(baseImgName, std::ios::binary);
	readParams(in); //cчитывание параметров
	readIMG(in); //считывание картинки
	in.close();
	//кодирование сообщения
	int i = 0;
	int j = 0;
	while (*(letter + i) != '\0') {
		char ch = *(letter + i);
		unsigned char ab = (ch & 0b11100000) >> 5;
		unsigned char ag = (ch & 0b00011100) >> 2;
		unsigned char ar = (ch & 0b00000011);
		IMG[j] ^= ab;
		IMG[++j] ^= ag;
		IMG[++j] ^= ar;
		i += 1;
		j += 1;
	}
	//запись новой картинки
	std::ofstream out(newImgName, std::ios::binary);
	writeParams(out); //запись параметров
	writeIMG(out); //запись картинки
	out.close();
}

void decoder(const char* baseImgName, const char* newImgName) {
	setlocale(LC_ALL, "Russian");
	unsigned char *orig;
	//чтение картинки исходной
	std::ifstream inOrig(baseImgName, std::ios::binary);
	readParams(inOrig); //cчитывание параметров
	readIMG(inOrig); //считывание картинки
	inOrig.close();
	//копирование картинки
	orig = new unsigned char[settings.lineImgSize];
	for (int i = 0; i < settings.lineImgSize; i++) {
		orig[i] = IMG[i];
	}
	//открытие закодированной картинки
	std::ifstream in(newImgName, std::ios::binary);
	readParams(in); //cчитывание параметров
	readIMG(in); //считывание картинки
	in.close();
	ofstream out("out.txt");
	char st2[200000] = "";
	char *p = st2;
	//раскодировка картинки
	for (int i = 0; i < settings.lineImgSize; i += color) {
		unsigned char ab = (orig[i] ^ IMG[i]) << 5;
		unsigned char ag = (orig[i + 1] ^ IMG[i + 1]) << 2;
		unsigned char ar = (orig[i + 2] ^ IMG[i + 2]);
		*p = ab + ag + ar;
		out << *p;
	}
	out.close();
}
