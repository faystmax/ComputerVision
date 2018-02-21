#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <omp.h>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

#if defined(_OPENMP)
    std::cout<<"Compiled by an OpenMP-compliant implementation!"<<std::endl;
#endif

    return a.exec();
}
