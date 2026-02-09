/**
 * @file matrix.hpp
 * @brief 行列
 * @author m1ffyz
 * @date 2026-02-09
 * @see https://github.com/m1ffyz/library/blob/main/math/matrix.hpp
 */

template <typename T>
struct matrix {
    vector<vector<T>> A;

    matrix(int n, int m, T x = 0) : A(n, vector<T> (m, x)) {}

    matrix(vector<vector<T>> _A) : A(_A) {}

    matrix(int n) : A(n, vector<T> (n, 0)) {};

    int height() const {
        return A.size();
    }

    int width() const {
        if (A.empty()) {
            return 0;
        }
        return A[0].size();
    }

    inline const vector<T> &operator[](int k) const {
        return A[k];
    }

    inline vector<T> &operator[] (int k) {
        return A[k];
    }

    static matrix I(int n) {
        matrix mat(n);
        for (int i = 0; i < n; i ++) {
            mat[i][i] = 1;
        }
        return mat;
    }

    matrix &operator+=(const matrix &B) {
        int n = height(), m = width();
        assert(n == B.height() and m == B.width());
        for (int i = 0; i < n; i ++) {
            for (int j = 0; j < m; j ++) {
                (*this)[i][j] += B[i][j];
            }
        }
        return *this;
    }

    matrix &operator-=(const matrix &B) {
        int n = height(), m = width();
        assert(n == B.height() and m == B.width());
        for (int i = 0; i < n; i ++) {
            for (int j = 0; j < m; j ++) {
                (*this)[i][j] -= B[i][j];
            }
        }
        return *this;
    }

    matrix &operator*=(const matrix &B) {
        int n = height(), m = B.width(), p = width();
        assert(p == B.height());
        vector<vector<T>> C(n, vector<T> (m, 0));
        for (int i = 0; i < n; i ++) {
            for (int k = 0; k < p; k ++) {
                for (int j = 0; j < m; j ++) {
                    C[i][j] += (*this)[i][k] * B[k][j];
                }
            }
        }
        A.swap(C);
        return *this;
    }

    matrix pow(long long k) const {
        matrix res = matrix::I(height());
        matrix b = *this;
        while (k > 0) {
            if (k & 1) {
                res *= b;
            }
            b *= b;
            k >>= 1;
        }
        return res;
    }

    matrix operator+(const matrix &B) const {
        return (matrix(*this) += B);
    }

    matrix operator-(const matrix &B) const {
        return (matrix(*this) -= B);
    }

    matrix operator*(const matrix &B) const {
        return (matrix(*this) *= B);
    }
};
