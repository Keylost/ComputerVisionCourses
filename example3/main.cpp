/*
 * Данная программа представляет собой пример получения негатива изображения
 * Пешкичев Роман. 2016 год
 */
 
#include <opencv2/opencv.hpp>
#include <stdint.h>

using namespace cv;

/*
 * Массивы для хранения изображений
 */
Mat original;
Mat result;

/*
 * Функция получает негатив изображения
 * @original - bgr изображение для которого необходимо получить негатив
 * @result - массив для хранения результата
 */
void convert(Mat &original, Mat &result)
{
	/*
	 * Создать изображение того же разрешения и глубины цвета, что и оригинал
	 */
	result = Mat(original.rows,original.cols,CV_8UC3);
	
	uint8_t *row;
	uint8_t *result_row;
	/*
	 * Пройти по всему изображению, вычетая текущее значение пикселя из его максимального значения
	 */
	for(int i=0;i<original.rows;i++)
	{
		row = (uint8_t*)original.ptr<uint8_t>(i);
		result_row = (uint8_t*)result.ptr<uint8_t>(i);
		for(int j=0;j<original.cols;j++)
		{
			int b = j*3+0; int g= j*3+1; int r = j*3+2;
			result_row[b]  = 255 - row[b];
			result_row[g]  = 255 - row[g];
			result_row[r]  = 255 - row[r];
		}
	}
	
	return;
}

int main(int, char** argv)
{
	/*
	* Создать окна для отображения оригинального изображения
	* и результата обработки
	*/
	namedWindow("Original");
	namedWindow("Result",0); 

	/*
	* считать изображение из файла, путь к которому был передан в качестве агрумента командной строки
	*/	
	original = imread(argv[1],1);
	if (original.empty())
	{
		printf("Can't open file %s .\n",argv[1]);
		return -1;
	}
	
	/*
	* вызвать функцию получения негатива
	*/
	convert(original, result);
	//bitwise_not(original,result); //функция OpenCV которая сделает тоже самое
	
	/*
	* отобразить результат и ждать нажатия любой клавиши на клавиатуре
	* 
	*/
	imshow("Original",original);
	imshow("Result",result);
	waitKey(0);
	
	return 0;
}
