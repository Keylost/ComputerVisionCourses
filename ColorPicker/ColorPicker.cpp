/*
 * Данная программа представляет собой инструмент для определения цвета и яркости точек на изображении
 * Пешкичев Роман. 2016 год
 */

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <stdint.h>

using namespace cv;
using namespace std;

/*
 * Массив для хранения изображения
 */
Mat image;

/*
 * Функция - обработчик событий мыши
 * @event - событие
 * @x & @y - координаты мыши на момент события
 * @flags - набор флагов
 * @orig - параметры переданные пользователем
 */
void onmouse(int event, int x, int y, int flags, void* orig)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
			Mat bgr; ((Mat *)orig)->copyTo(bgr);
			Mat hls; cvtColor(bgr,hls,CV_BGR2HLS);
			uint8_t *phls,*pbgr;
			phls = (uint8_t*)hls.ptr<uint8_t>(y,x);
			pbgr = (uint8_t*)bgr.ptr<uint8_t>(y,x);
			double gray = 0.2989 *pbgr[2] + 0.5870 * pbgr[1] + 0.1140 * pbgr[0];
			
			printf("---------\n");
			printf("B: %d, G: %d, R: %d\n",pbgr[0],pbgr[1],pbgr[2]);			
			printf("Gray scale value: %f\n",gray);
			printf("H: %d, L: %d, S: %d\n",phls[0],phls[1],phls[2]);
			printf("Position %dx%d\n",y,x);
			printf("---------\n");
			
			rectangle(bgr,Point(x-2,y-2),Point(x+2,y+2), Scalar(0,255,255));
			imshow("Image",bgr);
     }
}


int main(int argc, char** argv)
{
	/*
	* Создать окно для отображения изображения
	*/
	namedWindow("Image",WINDOW_NORMAL);
	
	/*
	* считать изображение из файла, путь к которому был передан в качестве агрумента командной строки
	*/
	image = imread(argv[1],1);
	if (image.empty())
	{
		printf("Can't open file %s .\n",argv[1]);
		return -1;
	}
	
	/*
	 * Назначить функцию обработчик событий мыши
	 */
    setMouseCallback("Image", onmouse, &image);
    
    /*
	* Отобразить изображение и ждать нажатия ESC на клавиатуре
	* 27 - ASCII код ESC
	*/
    imshow("Image",image);
	while(waitKey(0)!=27);
	return 0;
}
