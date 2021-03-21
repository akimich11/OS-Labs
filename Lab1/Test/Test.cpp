#include "../Vector/Vector.h"

void printVector(Vector v) 
{
    cout << "(" << v.getX() << ", " << v.getY() << ")\n";
}

int main()
{
    Number n1 = 2, n2 = NumberConstructor(3), n3 = 4, n4 = 8;
    Vector v1(n1, n2), v2(n3, n4), v3 = v1 + v2;

    cout << "\nn1: " << n1 << "\nn2: " << n2 << "\n";
    cout << "\nv1: ";
    printVector(v1);
    cout << "v2: ";
    printVector(v2);

    cout << '\n';
    cout << "n1 + n2 = " << n1 + n2 << "\n";
    cout << "n1 - n2 = " << n1 - n2 << "\n";
    cout << "n1 * n2 = " << n1 * n2 << "\n";
    cout << "n1 / n2 = " << n1 / n2 << "\n\n";

    cout << "v1 radius = " << v1.getRadius() << "\n";
    cout << "v2 angle = " << v2.getAngle() << "\n";
    cout << "v1 + v2: ";
    printVector(v1 + v2);

    cout << "\nPress Enter...";
    cin.get();
}
