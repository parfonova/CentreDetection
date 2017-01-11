﻿/*img — изображение на котором будут нарисованы контуры
contour — указатель на первый контур
external_color — цвет внешних контуров
hole_color — цвет внутренних контуров(отверстие)
max_level — максимальный уровень для отображения контуров (0 — только данный контур, 1 — данный и все следующие на данном уровне, 2 — все следующие контуры и все контуры на следующем уровне и т.д. ) Если величина отрицательная, то будут нарисованы контуры на предыдущем уровне перед contour.
thickness — толщина линии для отображения контуров (если величина отрицательная, то область, ограниченная контуром заливается выбранным цветом )
line_type — тип линии
*/
//
// пример нахождения контуров с помощью функции cvFindContours()


#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>

IplImage* image = 0;
IplImage* gray = 0;
IplImage* bin = 0;
IplImage* dst = 0;

int main(int argc, char* argv[])
{
        // имя картинки задаётся первым параметром
        char* filename = argc >= 2 ? argv[1] : "Image0.jpg";
        // получаем картинку
        image = cvLoadImage(filename,1);

        printf("[i] image: %s\n", filename);
        assert( image != 0 );

        // создаём одноканальные картинки
        gray = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
        bin = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
        // клонируем
        dst = cvCloneImage(image);
        // окно для отображения картинки
        cvNamedWindow("original",CV_WINDOW_AUTOSIZE);
        cvNamedWindow("binary",CV_WINDOW_AUTOSIZE);
        cvNamedWindow("contours",CV_WINDOW_AUTOSIZE);

     
        // преобразуем в двоичное
        cvInRangeS(gray, cvScalar(40), cvScalar(150), bin); // atoi(argv[2])

        CvMemStorage* storage = cvCreateMemStorage(0);
        CvSeq* contours=0;

        // находим контуры
        int contoursCont = cvFindContours( bin, storage,&contours,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));

        // нарисуем контуры
        for(CvSeq* seq0 = contours;seq0!=0;seq0 = seq0->h_next){
                cvDrawContours(dst, seq0, CV_RGB(255,216,0), CV_RGB(0,0,250), 0, 1, 8); // рисуем контур
        }

        // показываем картинки
        cvShowImage("original",image);
        cvShowImage("binary", bin);
        cvShowImage("contours", dst);

        // ждём нажатия клавиши
        cvWaitKey(0);

        // освобождаем ресурсы
        cvReleaseImage(&image);
        cvReleaseImage(&gray);
        cvReleaseImage(&bin);
        cvReleaseImage(&dst);
        // удаляем окна
        cvDestroyAllWindows();
        return 0;
}