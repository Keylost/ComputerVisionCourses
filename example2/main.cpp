/*
 * Данная программа представляет собой простой пример выделения контуров на изображении 
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

enum detect_type
{
	SIMPLE
};


/*
 * Функция для выделения контуров на изображении
 * @original - bgr изображение на котором необходимо найти контуры
 * @result - массив для хранения результата
 * @dtt - способ выделения контуров(см. перечисление detect_type выше)
 */
void detect_contours(Mat &original, Mat &result,detect_type dtt)
{
	/*
	 * Создать результирующее одноканальное изображение расширенное на два столба и две строки
	 * по сравнению с оригиналом заполненное черным цветом.
	 * Расширение необходимо для корректной обработки точек на краях иизображения
	 * (!!! обработку необходимо будет начать с первого пикселя, а не с нулевого)
	 */
	result = Mat(original.rows+2,original.cols+2,CV_8UC1,Scalar(0));
	
	/*
	 * Создать копию оригинального изображения и перевеси её в серую шкалу.
	 * Для выделения контуров информация о цвете не так важна как яркость, к тому же
	 * работа в серой шкале позволяет ускорить работу алгоритма
	 */
	Mat orig_copy;
	original.copyTo(orig_copy);
	cvtColor(orig_copy,orig_copy,CV_BGR2GRAY);
	
	
	uint8_t *original_row1,*original_row2;
	uint8_t *result_row;
	
	switch(dtt)
	{
		case SIMPLE:
		/*
		 * Вычетает из изображения тоже самое изображение со сдвигом в 1 пиксель
		 */
			for(int i=1;i<original.rows;i++)
			{
				original_row1 = (uint8_t*)orig_copy.ptr<uint8_t>(i);
				original_row2 = (uint8_t*)orig_copy.ptr<uint8_t>(i+1);
		
				result_row = (uint8_t*)result.ptr<uint8_t>(i);
				for(int j=1;j<original.cols;j++)
				{
					int lx = abs(original_row1[j]-original_row1[j+1]);
					int ly = abs(original_row1[j]-original_row2[j]);
					if(lx+ly>255) result_row[j] = 255;
					else result_row[j] = lx+ly;
				}
			}
			break;
		
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
	* вызвать функцию выделения контуров на изображении
	*/
	detect_contours(original, result,SIMPLE);
	
	/*
	* отобразить результат и ждать нажатия любой клавиши на клавиатуре
	* 
	*/
	imshow("Original",original);
	imshow("Result",result);
	waitKey(0);
	
	return 0;
}
