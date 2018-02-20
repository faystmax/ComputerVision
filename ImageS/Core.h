#ifndef CORE_H
#define CORE_H


class Core {
public:
    Core();
    Core(int width, double core[][3]);
    Core(int width, int height, double** core);
    ~Core();

    int getHeight() const {return height;}
    void setHeight(int value) {height = value;}
    int getWidth() const {return width;}
    void setWidth(int value) {width = value;}
    double getCoreAt(int x, int y) const {return core[x][y];}

private:
    int width;
    int height;
    double** core;
};

#endif // CORE_H
