/*
 * Данная программа представляет собой простой пример поиска простых геометрических форм на изображении
 * Пешкичев Роман. 2016 год
 */
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <cmath>
#include <iostream>

using namespace cv;
using namespace std;

/*
 * Массивы для хранения изображений
 */
Mat src;
Mat gray;
Mat canny_output;

/*
 * Вектора для хранения контуров и их иерархии
 */
vector<vector<Point> > contours;
vector<Point> approx;

/*
* Функция отрисовывает строку @label внутри контура @contour на изображении @im
*/
void setLabel(Mat& im, const string label, vector<Point>& contour)
{
	int fontface = FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	Size text = getTextSize(label, fontface, scale, thickness, &baseline);
	Rect r = boundingRect(contour);

	Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	rectangle(im, pt + Point(0, baseline), pt + Point(text.width, -text.height), CV_RGB(255, 255, 255), CV_FILLED);
	putText(im, label, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
}


int main(int argc, char **argv)
{
	/*
	* Создать окно для отображения изображения
	*/
	namedWindow("Result", CV_WINDOW_NORMAL);
	
	/*
	* считать изображение из файла, путь к которому был передан в качестве агрумента командной строки
	*/	
	src = imread(argv[1]);
	if (src.empty())
	{
		printf("Can't open file %s .\n",argv[1]);
		return -1;
	}

	/*
	 * Конвертировать изображение в полутоновое
	 */
	cvtColor(src, gray, CV_BGR2GRAY);
	
	/*
	 * Сгладить изображение простым усреднением.
	 * Значение яркости пикселя к которому будет привязано скользящее окно
	 * станет равно сумме всех элементов внутри скользящего окна, поделенной на их(элементов) количество
	 */
	blur( gray, gray, Size(3,3) );
	
	/*
	 * Использовать оператор Canny для преобразования изображения @gray к черно-белому @canny_output,
	 * на котором останутся лишь конуры объектов
	 * Третий и четвертый параметры используются для удаления слабых границ. При значениях 0,255 не 
	 * имеет эффекта. Подробнее https://ru.wikipedia.org/wiki/Оператор_Кэнни
	 * Пятый параметр - размер опрератора Собеля, используемого для вычисления градиента (1, 3, 5 или 7)
	 */
	Canny(gray, canny_output, 0, 255, 5);
	
	/*
	 * Привести контуры к векторному виду и аппроксимировать
	 * CV_CHAIN_APPROX_NONE - не производить никакой аппроксимации
	 * CV_RETR_TREE - способ представления иерархии контуров hierarchy. CV_RETR_TREE - полная иерархия
	 * Point(0, 0) - сдвиг точек контуров относительно изображения из которого было вырезано, то на котором мы ищем контуры(здесь не используется, поэтому 0)
	 * canny_output - чернобелое изображение(выход алгоритма Canny)
	 * contours и hierarchy - структуры для хранения контуров и их иерархии соответсвенно
	 */
	findContours(canny_output, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	
	/*
	 * Пройти по всем найденным контурам и нарисовать их на изображении
	 */
	for (unsigned i = 0; i < contours.size(); i++)
	{
		/*
		 * Вычислить площадь контура, периметр и компактность
		 * Компактность - это отношение площади к квадрату периметра
		 */
		double area = fabs(contourArea(contours[i]));
		double perim = arcLength(Mat(contours[i]), true);
		double compact = area / (perim * perim);
		
		/*
		 * Эта функция производит аппроксимацию точек контура, преобразуя их в сегменты(отрезки)
		 * и оставляя только их конечные точки.
		 * Функция строит отрезок между первой и последней точками контура и находит точку контура наиболее отклоняющуюяся
		 * от данного отрезка. Если расстояние между отрезком и точкой меньше третьего параметра(3.5% от периметра)
		 * то все точки контура удаляюся, остаются лишь конечные точки отрезка. Если же расстояние больше
		 * третьего параметра, то создаются два отрезка, разделеннных этой точкой и функция вызывается для каждого из
		 * этих отрезков и т.д.
		 * Последний параметр говорит, что мы имеем замкнутый контур, где первая и последняя точки совпадают 
		 */
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.035, true);
		
		/*
		 * После аппроксимации для треугольков должно получится 3 точки в контуре,
		 * для четырехугольников 4 и т.д.
		 * 0.0625 - компактность квадрата
		 * 0.079577 - компактность круга
		 */
		if(approx.size()==3)
		{
			setLabel(src, "TRI", contours[i]);
		}
		else if( compact > 0.06 && compact< 0.065 && approx.size()==4 ) ////+-4% 0.0625
		{
			setLabel(src, "SQARE", contours[i]);
		}
		else if ( compact > 0.07 && compact< 0.087 && approx.size() >4 ) //+-10% 0.079577
		{
			setLabel(src, "CIR", contours[i]);
		}
		else if(approx.size()==6)
		{
			setLabel(src, "HEXA", contours[i]);
		}
	}
	
	/*
	* отобразить результат и ждать нажатия любой клавиши на клавиатуре
	* 
	*/
	imshow("Result", src);
	waitKey(0);
	return 0;
}
