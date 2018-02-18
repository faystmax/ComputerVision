#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <omp.h>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

#if defined(_OPENMP)
    std::cout<<"Compiled by an OpenMP-compliant implementation!"<<std::endl;
#endif

    MainWindow w;
    w.show();

    return a.exec();
}
