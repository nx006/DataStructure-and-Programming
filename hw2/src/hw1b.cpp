#include <iostream>
using namespace std;

template <typename T>
class CPoint {
    T x; T y;
public:
    T getXpos();
    T getYpos();
    CPoint(T x, T y);
    void Move(T x, T y);
    template<typename U>
    friend std::ostream& operator << (std::ostream& rst, CPoint& p);
};

template<typename T>
T CPoint<T>::getXpos()
{
    return this->x;
}

template<typename T>
T CPoint<T>::getYpos()
{
    return this->y;
}

template<typename T>
CPoint<T>::CPoint(T x, T y)
{
    this->x = x;
    this->y = y;
}

template <typename T>
void CPoint<T>::Move(T x, T y) {
    this->x = x;
    this->y = y;
}

template<typename T>
std::ostream& operator<<(std::ostream& rst, CPoint<T>& point)
{
    rst << "(" << point.getXpos() << ", " << point.getYpos() << ")" << endl;

    return rst;
}


int main() {
    CPoint<int> P1(1, 11);
    CPoint<double> P2(1.1, 2.2);
    P1.Move(8, 13);
    P2.Move(8.97, 20.39);
    cout << P1 << P2;
    return 0;
}