
#include"stdafx.h"
#define Mypi 3.1415926535
#include<iostream>
#include<vector>
#include<string>
#include<GL\glut.h>
#include<math.h>

int getNumberFromInputThread();//проверка на ввод с клавиатуры

int g_mode = 0;//идентификатор рисунка
//**********************************************************************************************
class graphics//класс графики 
{
public:
	graphics(float, float, unsigned int);
	~graphics() {};
	static graphics* GetInstance();
	void InitializeWindow(int&, char**);
	void BaseInitialize();

	void ReshapeWindow();
	void KeyboardFunc();
	void SpecialFunc();

	static void ChangeSizeWindow(int, int);
	static void processNormalKeys(unsigned char, int, int);
	static void processSpecialKeys(int, int, int);

	void MainLoop();

protected:
	static float width;//ширина окна
	static float height;//высота окна
	static unsigned int depth;//глубина рекурсии
};

//такая инициализация необходима во избежание ошибок
float graphics::width = 0;
float graphics::height = 0;
unsigned int graphics::depth = 0;
//type class_name::static_variable = value  --> способ инициализации статических переменных
//**********************************************************************************************

//класс - "Ковер Серпинского"
class Serpinsky_Carpet :public graphics
{
public:
	Serpinsky_Carpet(float width, float height, unsigned int depth) : graphics(width, height, depth) {};//конструктор с параметрами
	static Serpinsky_Carpet* GetInstance();
	void Drawing_Serpinsky_Carpet(float, float, float,unsigned int);
	void Paint_Square(float, float, float);
	void DisplayFunc();
	static void Draw();
};
//**********************************************************************************************

//класс - "Окружности"(рисование 4(6) окружностей вокруг одной)
class Circles : public graphics
{
private:
	float radius;
	unsigned int number_of_circle;
public:
	Circles(float width, float height, unsigned int depth, float radius);
	static Circles* GetInstance();
	void Drawing_pattern(float, float, float, unsigned int, unsigned int);//рисование фигуры
	void Paint_Circle(float, float, float);//рисование окружности
	void DisplayFunc();
	static void Draw();
};
//**********************************************************************************************
//класс - "Губка Серпинского"
class Serpinsky_Sponge : public graphics
{
private:
	float Side_Size;
	int Angle;
	std::vector<std::vector<float>> arr;//массив вершин(точек) и сторон кубиков, которые НЕ надо рисовать
public:
	Serpinsky_Sponge(float width, float height, unsigned int depth, float Side_Size, int Angle);
	~Serpinsky_Sponge() {};
	static Serpinsky_Sponge* GetInstance();
	static void timer(int);
	void Sponge_Painting(float x, float y, float z,//координаты левого нижнего угла прямоугольника
		float side, unsigned int   depth_recursion);
	void Draw_Cube(float x, float y, float z, float side);
	void BildLists();
	static void Draw();
	void DisplayFunc();

	bool Object_exists;//флаг, определяющий, был ли создан объект в ходе работы программы
	bool Object_active;//флаг, определяющий, рисуется ли губка Серпинского
};
//**********************************************************************************************

//класс - "Звезда Дюрера"
class Durer_Star : public graphics
{
private:
	float radius;
public:
	Durer_Star(float width, float height, unsigned int deep, float radius);
	~Durer_Star() {};
	static Durer_Star* GetInstance();
	void Draw_Star(float, float, float, float, unsigned int); //рисование фигуры
	static void Draw();
	void DisplayFunc();
};
//**********************************************************************************************

//определение класс graphics
graphics::graphics(float width, float height, unsigned int depth)
{
	this->width = width;
	this->height = height;
	this->depth = depth;
}

graphics * graphics::GetInstance()//функция, возвращающая указатель на статичный экземпляр класса
{
	static graphics Object(500, 500, 3);//статичная переменная- одна для всех экземпляров класса
	return &Object;
}

void graphics::InitializeWindow(int& argc, char** argv)//функция по созданию окна
{
	//Инициализация окна
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);//позиция при создании
	glutInitWindowSize(this->width, this->height);
	glutCreateWindow("Fractals");
}

void graphics::BaseInitialize()//функция базовой инициализации 
{
	glClearColor(1.0, 1.0, 1.0, 1.0);//цвет фона
	glMatrixMode(GL_PROJECTION);//матрица проекций
	glLoadIdentity();//сбрасываем параметры и получается единичная матрица
	glOrtho(0, width, 0, height, -1.0, 1.0);//оси координат
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//перерисовка изображения
void graphics::ReshapeWindow()//функция регистрирует функцию обработки изменений размеров окна
{
	glutReshapeFunc(graphics::ChangeSizeWindow);
}

void graphics::ChangeSizeWindow(int width, int height)//функция вызывается при изменении размеров окна
{
	graphics::GetInstance()->width = static_cast<float>(width);
	graphics::GetInstance()->height = static_cast<float>(height);

	glViewport(0, 0, width, height);//перестраивается вывод изображения в окно с другими размерами

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//обработка клавиатуры
void graphics::KeyboardFunc()//функция регистрирует функцию обработки нажатия обычных клавиш
{
	glutKeyboardFunc(graphics::processNormalKeys);
}

void  graphics::processNormalKeys(unsigned char key, int x, int y)//функция отвечает за обработку обычных клавиш
{
	switch (key)
	{
	case 27:
		exit(0);
	default:
		break;
	}
}

void graphics::SpecialFunc()//функция регистрирует функцию обработки нажатия специальных клавиш
{
	glutSpecialFunc(graphics::processSpecialKeys);
}

void graphics::processSpecialKeys(int key, int x, int y)//функция обработки нажатия специальных клавиш
{
	switch (key)
	{
	case GLUT_KEY_UP:
		switch (g_mode)
		{
		case 1://был нарисован ковер Серпинского
			Serpinsky_Carpet::depth++;
			Serpinsky_Carpet::Draw();
			break;

		case 2://были нарисованы круги
			Circles::depth++;
			Circles::Draw();
			break;
		case 3://была нарисована губка Серпинского
			if (Serpinsky_Sponge::depth < 3)
				Serpinsky_Sponge::depth++;
			Serpinsky_Sponge::Draw();
			break;
		case 4://была нарисована звезда Дюрера
			Durer_Star::depth++;
			Durer_Star::Draw();
			break;
		default:
			break;
		}

		break;
	case GLUT_KEY_DOWN:
		if (graphics::depth > 1)
		{
			switch (g_mode)
			{
			case 1://был нарисован ковер Серпинского
				Serpinsky_Carpet::depth--;
				Serpinsky_Carpet::Draw();
				break;

			case 2://были нарисованы круги
				Circles::depth--;
				Circles::Draw();
				break;
			case 3://была нарисована губка Серпинского
				Serpinsky_Sponge::depth--;
				Serpinsky_Sponge::Draw();
				break;
			case 4://была нарисована звезда Дюрера
				Durer_Star::depth--;
				Durer_Star::Draw();
				break;
			default:
				break;
			}
		}
		break;
	case GLUT_KEY_LEFT:
		switch (g_mode)
		{
		case 1://был нарисован ковер Серпинского
			g_mode = 4;
			glutSetWindowTitle("Durer star");
			Durer_Star::Draw();
			break;
		case 2://были нарисованы окружности
			g_mode--;
			glutSetWindowTitle("Serpinsky carpet");
			Serpinsky_Carpet::Draw();
			break;
		case 3://была нарисована губка Серпинского
			g_mode--;
			glutSetWindowTitle("Circles");
			Serpinsky_Sponge::GetInstance()->Object_active = false;//нужно для отключения таймера
			Circles::GetInstance()->BaseInitialize();
			Circles::Draw();
			break;
		case 4://была нарисована звезда Дюрера
			g_mode--;
			glutSetWindowTitle("Serpinsky sponge");
			Serpinsky_Sponge::GetInstance()->Object_active = true;//флаг для работы таймера

			if (!Serpinsky_Sponge::GetInstance()->Object_exists)//если объект не был создан
			{
				Serpinsky_Sponge::GetInstance()->BildLists();
				Serpinsky_Sponge::GetInstance()->Object_exists = true;
			}
			if (graphics::depth > 3)
				graphics::depth = 3;
			glutTimerFunc(30, Serpinsky_Sponge::timer, 0);
			Serpinsky_Sponge::Draw();
			break;

		default:
			break;
		}
		break;

	case GLUT_KEY_RIGHT:

		switch (g_mode)
		{
		case 1://был нарисован ковер Серпинского
			g_mode++;
			glutSetWindowTitle("Circles");
			Circles::Draw();
			break;
		case 2://были нарисованы окружности
			g_mode++;
			glutSetWindowTitle("Serpinsky sponge");
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			if (graphics::depth > 3)
				graphics::depth = 3;

			if (!Serpinsky_Sponge::GetInstance()->Object_exists)//если объект не был создан
			{
				Serpinsky_Sponge::GetInstance()->BildLists();
				Serpinsky_Sponge::GetInstance()->Object_exists = true;
			}
			Serpinsky_Sponge::GetInstance()->Object_active = true;
			glutTimerFunc(30, Serpinsky_Sponge::timer, 0);
			Serpinsky_Sponge::Draw();

			break;

		case 3://была нарисована губка Серпинского
			g_mode++;
			glutSetWindowTitle("Durer Star");
			Serpinsky_Sponge::GetInstance()->Object_active = false;//нужно для отключения таймера
			Durer_Star::GetInstance()->BaseInitialize();
			Durer_Star::Draw();
			break;
		case 4:
			g_mode = 1;
			glutSetWindowTitle("Serpinsky carpet");
			Serpinsky_Carpet::Draw();
			break;
		default:
			break;
		}

		break;
	default:
		break;
	}
}

void graphics::MainLoop()
{
	glutMainLoop();
}
//**********************************************************************************************

//определение клааса - "Ковер Серпинского"
Serpinsky_Carpet* Serpinsky_Carpet::GetInstance()
{
	static Serpinsky_Carpet Object = Serpinsky_Carpet(500, 500, 3);
	return &Object;
}

void Serpinsky_Carpet::DisplayFunc()//функция регистрирует функцию рисования
{
	glutDisplayFunc(Serpinsky_Carpet::Draw);
}

void Serpinsky_Carpet::Draw()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	Serpinsky_Carpet::GetInstance()->Drawing_Serpinsky_Carpet(0, 0, Serpinsky_Carpet::GetInstance()->width, Serpinsky_Carpet::GetInstance()->depth);
	glutSwapBuffers();
}
void Serpinsky_Carpet::Drawing_Serpinsky_Carpet(float x, float y,//координаты левого нижнего угла прямоугольника
	float side, unsigned int depth_recursion)//функция построения фигуры "Ковер Серпинского"
{
	if (!depth_recursion)
		return;

		this->Paint_Square(x + side / 3.0, y + side / 3.0, side / 3.0);

		if (depth_recursion == 1)
			return;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				this->Drawing_Serpinsky_Carpet(x + j*side / 3.0, y + i*side / 3.0, side / 3.0, depth_recursion - 1);
			}
		}
}

void Serpinsky_Carpet::Paint_Square(float x, float y, float side)
{
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(x, y, 0);
	glVertex3f(x, y + side, 0);
	glVertex3f(x + side, y, 0);
	glVertex3f(x + side, y + side, 0);
	glEnd();
}
//**********************************************************************************************
//определение класса - "Окружности типа 4 и типа 6"
Circles::Circles(float width, float height, unsigned int depth, float radius) : graphics(width, height, depth)
{
	this->radius = radius;
	system("cls");
	std::cout << " Enter a figure number:\n-----------------------\n 1)Figure with 4 circles around one;\n 2)Figure with 6 circles around one;" << std::endl;
	int ch;
	ch = getNumberFromInputThread();
	while (ch != 1 && ch != 2)
	{
		system("cls");
		std::cout << " Enter a figure number:\n-----------------------\n 1)Figure with 4 circles around one;\n 2)Figure with 6 circles around one;" << std::endl;
		ch = getNumberFromInputThread();
	}

	ch == 1 ? this->number_of_circle = 4 : this->number_of_circle = 6;

}

Circles* Circles::GetInstance()
{
	static Circles Object = Circles(500, 500, 3, 80);
	return &Object;
}
void Circles::DisplayFunc()
{
	glutDisplayFunc(Circles::Draw);
}

void Circles::Draw()
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	Circles::GetInstance()->Drawing_pattern(Circles::GetInstance()->width / 2.0, Circles::GetInstance()->height / 2.0, Circles::GetInstance()->radius, Circles::GetInstance()->depth, Circles::GetInstance()->number_of_circle);
	glutSwapBuffers();
}

//функция построения фрактала "Окружности типа 4 и типа 6"
void Circles::Drawing_pattern(float x, float y, float radius, unsigned int depth_recursion, unsigned int number_of_circles)
{
	if (!depth_recursion)
		return;

	this->Paint_Circle(x, y, radius);//рисуем окружность
	switch (number_of_circles)
	{
	case 4:
		for (int i = 0; i < number_of_circles; i++)
			this->Drawing_pattern(x + radius*cos(static_cast<float>(i * 360) / number_of_circles *Mypi / 180), y - radius*sin(static_cast<float>(i * 360) / number_of_circles*Mypi / 180), radius / 2, depth_recursion - 1, number_of_circles);
		break;
	case 6:
		for (int i = 0; i < number_of_circles; i++)
			this->Drawing_pattern(x + 1.8 * radius*cos(static_cast<float>(i * 360) / number_of_circles *Mypi / 180), y - 1.8 * radius*sin(static_cast<float>(i * 360) / number_of_circles*Mypi / 180), radius / 3.3, depth_recursion - 1, number_of_circles);
		break;
	default:
		break;
	}
}

void Circles::Paint_Circle(float x, float y, float radius)
{
	glColor3f(0, 0.8, 0.8);
	glBegin(GL_POINTS);
	for (int i = 0; i < 360; i++)
	{
		glVertex2f(x + radius*cos(static_cast<float>(i)*3.14 / 180), y + radius*sin(static_cast<float>(i)*3.14 / 180));
	}
	glEnd();
}
//**********************************************************************************************
//определение клааса - "Губка Серпинского"
Serpinsky_Sponge::Serpinsky_Sponge(float width, float height, unsigned int depth, float Side_Size, int Angle) : graphics(width, height, depth)
{
	this->Side_Size = Side_Size;
	this->Angle = Angle;
	this->Object_exists = false;
	this->Object_active = true;
};

void Serpinsky_Sponge::DisplayFunc()
{
	glutDisplayFunc(Serpinsky_Sponge::Draw);
}

void Serpinsky_Sponge::Draw()
{

	glClearColor(0, 0, 0.15, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	float ambient[4] = { 0.6, 0.6, 0.6, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);//установка фоновой освещенности
	glEnable(GL_LIGHT0);

	float pos[] = { 5,1,5,1 };
	float dir[] = { 0,0,0, 1 };
	float specular[] = { 1,1,1,1 };

	glLightfv(GL_LIGHT0, GL_POSITION, pos);//установка камеры в определенную точку
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);//куда "смотрит" камера

	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);//отраженный свет(блик)
	glMaterialf(GL_FRONT, GL_SHININESS, 90.0);//степень отраженного света

	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);//

	glRotatef(Serpinsky_Sponge::GetInstance()->Angle, 1.0, 1.0, 1.0);
	glCallList(Serpinsky_Sponge::GetInstance()->depth);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_DEPTH_TEST);
	glutSwapBuffers();

}

//функция построения фрактала "Губка Серпинского"
void Serpinsky_Sponge::Sponge_Painting(float x, float y, float z,float side, unsigned int depth_recursion)
{

	if (!depth_recursion)
	{
		for (int i = 0; i < Serpinsky_Sponge::arr.size(); i++)
		{
			if (((x >= this->arr[i][0]) && (x < this->arr[i][0] + this->arr[i][3]) && (y >= this->arr[i][1]) && (y < this->arr[i][1] + this->arr[i][3])) ||
				((x >= this->arr[i][0]) && (x < this->arr[i][0] + this->arr[i][3]) && (z >= this->arr[i][2]) && (z < this->arr[i][2] + this->arr[i][3])) ||
				((z >= this->arr[i][2]) && (z < this->arr[i][2] + this->arr[i][3]) && (y >= this->arr[i][1]) && (y < this->arr[i][1] + this->arr[i][3])))
				return;
		}
		this->Draw_Cube(x, y, z, side);
		return;
	}

		std::vector<float> ex = std::vector<float>();
		ex.resize(4);
		ex[0] = x + side / 3.0;
		ex[1] = y + side / 3.0;
		ex[2] = z + side / 3.0;
		ex[3] = side / 3.0;
		this->arr.push_back(ex);

		for (int k = 0; k < 3; k++)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					this->Sponge_Painting(x + j * side / 3.0, y + i * side / 3.0, z + k * side / 3.0, side / 3.0, depth_recursion - 1);
				}
			}
		}

}

void  Serpinsky_Sponge::Draw_Cube(float x, float y, float z, float side)//функция рисования куба по центру координат и стороне
{
	//передняя грань
	glColor3f(0, 0.1, 0.9);
	glBegin(GL_QUADS);
	glVertex3f(x, y, z);
	glVertex3f(x, y + side, z);
	glVertex3f(x + side, y + side, z);
	glVertex3f(x + side, y, z);
	glEnd();

	//задняя грань
	glColor3f(0, 0.1, 0.9);
	glBegin(GL_QUADS);
	glVertex3f(x, y, z + side);
	glVertex3f(x, y + side, z + side);
	glVertex3f(x + side, y + side, z + side);
	glVertex3f(x + side, y, z + side);
	glEnd();

	//правая грань
	glColor3f(0.1, 0.5, 0.8);
	glBegin(GL_QUADS);
	glVertex3f(x + side, y, z);
	glVertex3f(x + side, y + side, z);
	glVertex3f(x + side, y + side, z + side);
	glVertex3f(x + side, y, z + side);
	glEnd();

	//левая грань
	glColor3f(0.1, 0.5, 0.8);
	glBegin(GL_QUADS);
	glVertex3f(x, y, z);
	glVertex3f(x, y + side, z);
	glVertex3f(x, y + side, z + side);
	glVertex3f(x, y, z + side);
	glEnd();

	//верхняя грань
	glColor3f(0, 0.7, 1);
	glBegin(GL_QUADS);
	glVertex3f(x, y + side, z);
	glVertex3f(x, y + side, z + side);
	glVertex3f(x + side, y + side, z + side);
	glVertex3f(x + side, y + side, z);
	glEnd();

	//нижняя грань
	glColor3f(0, 0.7, 1);
	glBegin(GL_QUADS);
	glVertex3f(x, y, z);
	glVertex3f(x, y, z + side);
	glVertex3f(x + side, y, z + side);
	glVertex3f(x + side, y, z);
	glEnd();
}

Serpinsky_Sponge* Serpinsky_Sponge::GetInstance()
{
	static Serpinsky_Sponge Object = Serpinsky_Sponge(500, 500, 1, 0.6, 0);
	return &Object;
}

void Serpinsky_Sponge::timer(int value)
{
	if (Serpinsky_Sponge::GetInstance()->Object_active)
	{
		Serpinsky_Sponge::GetInstance()->Angle++;
		Serpinsky_Sponge::Draw();
		glutTimerFunc(30, Serpinsky_Sponge::timer, 0);
	}
}

void Serpinsky_Sponge::BildLists()
{
	glNewList(0, GL_COMPILE);
	this->Sponge_Painting(-0.2, -0.2, -0.2, this->Side_Size, 0);
	glEndList();

	glNewList(1, GL_COMPILE);
	this->Sponge_Painting(-0.2, -0.2, -0.2, this->Side_Size, 1);
	glEndList();

	glNewList(2, GL_COMPILE);
	this->Sponge_Painting(-0.2, -0.2, -0.2, this->Side_Size, 2);
	glEndList();

	glNewList(3, GL_COMPILE);
	this->Sponge_Painting(-0.2, -0.2, -0.2, this->Side_Size, 3);
	glEndList();
}
//**********************************************************************************************
//определение класса - "Звезда Дюрера"
Durer_Star::Durer_Star(float width, float height, unsigned int deep, float radius) : graphics(width, height, deep)
{
	this->radius = radius;
};

Durer_Star* Durer_Star::GetInstance()
{
	static Durer_Star Object(500, 500, 7, 100);
	return &Object;
}
void Durer_Star::DisplayFunc()
{
	glutDisplayFunc(Durer_Star::Draw);
}
void Durer_Star::Draw()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.2f, 0.5f);// Задание цвета для рисования

	glBegin(GL_LINES);// Начало рисования.Вершины 2n и 2n + 1 будут обьединены в линию
					  /* Вызов функции, рисующей фрактал. В функции передаются вершины в буфер */
	Durer_Star::GetInstance()->Draw_Star(0.55*Durer_Star::GetInstance()->width, 0.5*Durer_Star::GetInstance()->height, Durer_Star::GetInstance()->radius, 0, Durer_Star::GetInstance()->depth);
	glEnd();

	glutSwapBuffers();
}

void Durer_Star::Draw_Star(float x, float y, float radius, float angle, unsigned int deep)
{

	/* Для уменьшения кода в строках вводятся эти переменные
	pi5 равна Пи, делённой на 5
	h равна расстоянию от центра большего пятиугольника к центрам меньших
	*/
	double pi5 = 3.141596 / 5;
	double h = 2 * radius*cos(pi5);

	for (int i = 0; i<5; i++) {
		/*
		Вычисление некоторых параметров для рисования их на экране
		ang2 Поворот относительно центрального пятиугольника на 360/5 градусов
		x2 центр меньшего пятиугольника
		y2 центр меньшего пятиугольника
		ang3 угол, на который нужно повернуть вершину изначально
		rad расстояние до каждого из углов меньшего пятиугольника от его центра
		*/
		double ang2 = angle + pi5*i * 2,
			x2 = x - h*cos(ang2),
			y2 = y - h*sin(ang2),
			rad_2 = radius / (2 * cos(pi5) + 1),
			ang3 = angle + 3.141596 + (2 * i + 1)*pi5;
		for (int j = 0; j<4; j++) {
			/* В буфер записываются вершины. Эти две вершины будут соединены в прямые между собой*/
			glVertex2f(x + rad_2*cos(ang3 + j*pi5 * 2), y + rad_2*sin(ang3 + j*pi5 * 2));
			glVertex2f(x + rad_2*cos(ang3 + (j + 1)*pi5 * 2), y + rad_2*sin(ang3 + (j + 1)*pi5 * 2));
		}
		/* Условие выхода из рекурсии. Из максимальной глубины рекурсии не строятся меньшие пятиугольники*/
		if (deep>0)
			this->Draw_Star(x2, y2, radius / (2 * cos(pi5) + 1), angle + 3.141596 + (2 * i + 1)*pi5, deep - 1);
	}
}
//**********************************************************************************************

int main(int argc, char**argv)
{
	//создание окна
	graphics::GetInstance()->InitializeWindow(argc, argv);

	//регистрация функций
	std::cout << " Enter a figure number:\n-----------------------\n 1)Serpinsky carpet;\n 2)Circles;\n 3)Serpinsky sponge;\n 4)Durer Star; " << std::endl;
	int ch = getNumberFromInputThread();
	while (ch<1 || ch>4)
	{
		system("cls");
		std::cout << " Enter a figure number\n-----------------------\n 1)Serpinsky carpet;\n 2)Circles;\n 3)Serpinsky sponge;\n 4)Durer Star; " << std::endl;
		ch = getNumberFromInputThread();
	}
	switch (ch)
	{
	case 1:
		g_mode = 1;
		glutSetWindowTitle("Serpinsky carpet");
		Serpinsky_Carpet::GetInstance()->BaseInitialize();
		Serpinsky_Carpet::GetInstance()->DisplayFunc();
		break;
	case 2:
		g_mode = 2;
		glutSetWindowTitle("Circles");
		Circles::GetInstance()->BaseInitialize();
		Circles::GetInstance()->DisplayFunc();
		break;

	case 3:
		g_mode = 3;
		glutSetWindowTitle("Serpinsky sponge");
		Serpinsky_Sponge::GetInstance()->Object_exists = true;
		Serpinsky_Sponge::GetInstance()->BildLists();
		Serpinsky_Sponge::GetInstance()->DisplayFunc();
		glutTimerFunc(30, Serpinsky_Sponge::timer, 0);//время в миллисекундах 
		break;
	case 4:
		g_mode = 4;
		glutSetWindowTitle("Durer star");
		Durer_Star::GetInstance()->BaseInitialize();
		Durer_Star::GetInstance()->DisplayFunc();
		break;
	default:
		break;
	}

	graphics::GetInstance()->ReshapeWindow();//обработка перерисовки
	graphics::GetInstance()->KeyboardFunc();//обработка нормальных клавиш
	graphics::GetInstance()->SpecialFunc();//обработка специальных клавиш

	graphics::GetInstance()->MainLoop();//вход в главный цикл
	return 0;
}

// Функция проверки ввода данных
// Возвращает проверенное на правильность ввода число
int getNumberFromInputThread()
{
	int n;
	std::string c;
	//проверка ввода 
	bool t = true;
	while (t)
	{
		std::cin >> c;
		t = false;
		n = 0;
		for (int i = 0; i < c.length(); i++)
		{

			if (((c[i] > '0') && (c[i] <= '9') && (i == 0)) || ((c[i] >= '0') && (c[i] <= '9') && (i != 0)))
			{
				n += (c[i] - '0') * pow(10, c.length() - i - 1);
			}
			else
			{
				std::cout << "it's not a number or it's irrational number" << std::endl;
				std::cout << "please, enter again: ";
				t = true;
				break;
			}
		}
	}
	return n;
}