#include <iostream>
#include <Eigen/Dense>

using namespace std;
// 使用Eigen命名空间
using namespace Eigen;

int main() {
    // 以Xd方式声明一个3x3的矩阵
    MatrixXd mat(3, 3);
    mat(0, 0) = 1.3;

    mat.isApprox(mat);
    cout << (mat - mat).cwiseAbs2().sum() << endl;
    cout << numext::mini(mat.cwiseAbs2().sum(), mat.cwiseAbs2().sum()) << endl;
    // 将矩阵(0,0)位置元素赋为1.3
    
    cout << mat << endl;
    return 0;
}