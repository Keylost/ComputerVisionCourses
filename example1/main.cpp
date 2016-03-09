/*
 * Данная программа представляет собой пример конвертирования цветного BGR изображения в серую шкалу
 * Пешкичев Роман. 2016 год
 */
 
#include <opencv2/opencv.hpp>
#include <stdint.h>

using namespace cv;

/*
 * Массивы для хранения изображений
 */
Mat original;
Mat result_simple;
Mat result_hdtv;
Mat result_ntsc;

/*
 * Каждому элементу этого перечисления соответствуют свои коеффициенты перевода bgr 
 * в grayscale() . см функцию convert_simple(...)
 */
enum convert_type
{
	SIMPLE,
	NTSC,
	HDTV
};

/*
 * Функция переводит цветное bgr изображение в чернобелую шкалу
 * @original - bgr изображение
 * @result - изображение куда будет помещен результат
 * @ctypy - способ преобразования(см. перечисление convert_type выше)
 */
void convert_simple(Mat &original, Mat &result,convert_type ctype)
{
	/*
	 * создать пустое однокональное результирующее изображение того же разрешения, что
	 * и оригинал
	 */
	result = Mat(original.rows,original.cols,CV_8UC1);
	
	/*
	 * указатели на начало строки в массиве хранящем изображение
	 */
	uint8_t *original_row;
	uint8_t *result_row;
	
	/*
	 * Обход массива точек изображения
	 * Цикл по строкам
	 */
	for(int i=0;i<original.rows;i++)
	{
		
		original_row = (uint8_t*)original.ptr<uint8_t>(i);
		result_row = (uint8_t*)result.ptr<uint8_t>(i);
		/*
		 * Цикл по столбцам
		 */
		for(int j=0;j<original.cols;j++)
		{
			int b = original_row[j*3+0];
			int g = original_row[j*3+1];
			int r = original_row[j*3+2];
			
			switch(ctype)
			{
				case SIMPLE:
					//Простое усреднение
					result_row[j] = (b+g+r)/3;
					break;
				case NTSC:
					//данные коеффициенты используются в PAL/NTSC
					result_row[j] = (uint8_t)(b*0.114 + g*0.587 + r*0.299);
					break;
				case HDTV:
					//стандарт ITU-R BT.709 испоользуемый в HDTV
					result_row[j] = (uint8_t)(b*0.0722 + g*0.7152 + r*0.2126);
					break;
				default: break;
			}
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
	namedWindow("Result_SIMPLE"); 
	namedWindow("Result_NTSC"); 
	namedWindow("Result_HDTV"); 
	
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
	* вызвать функции конвертирования цветного изображения в серую шкалу
	*/
	convert_simple(original,result_hdtv,HDTV);
	convert_simple(original,result_ntsc,NTSC);
	convert_simple(original,result_simple,SIMPLE);
	
	/*
	* отобразить результат и ждать нажатия любой клавиши на клавиатуре
	* 
	*/
	imshow("Original",original);
	imshow("Result_SIMPLE",result_simple);
	imshow("Result_NTSC",result_ntsc);
	imshow("Result_HDTV",result_hdtv);
	waitKey(0);
	
	return 0;
}
