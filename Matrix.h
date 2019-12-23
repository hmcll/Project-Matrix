#pragma once
#include <vector>
#include <initializer_list>
class Matrix {
	friend class AccessDummy;
	std::vector<double> data;
	size_t dimx, dimy;
	Matrix(const size_t _dimx, const size_t _dimy) :
		data(), dimx(_dimx), dimy(_dimy)
	{}

	Matrix(const std::vector<double>& _data, const size_t _dimx, const size_t _dimy) :
		data(_data), dimx(_dimx), dimy(_dimy)
	{}

	const size_t ptoi(size_t x, size_t y) const {
		return y * dimx + x;
	};

public:
	template <typename... T>
	static Matrix mat2(T... t) {
		static_assert (sizeof...(t) == 4, "Invalid Argument Number");
		std::vector<double> dummy;
		(dummy.push_back(std::forward<T>(t)), ...);
		return Matrix(dummy, 2, 2);
	}

	template <typename... T>
	static Matrix&& mat3(T... t) {
		static_assert(sizeof...(t) == 9, "Invalid Argument Number");
		std::vector<double> dummy;
		(dummy.push_back(std::forward<T>(t)), ...);
		return std::move(Matrix(dummy, 3, 3));
	}

	template <typename... T>
	static Matrix&& mat4(T... t) {
		static_assert(sizeof...(t) == 16, "Invalid Argument Number");
		std::vector<double> dummy;
		(dummy.push_back(std::forward<T>(t)), ...);
		return std::move(Matrix(dummy, 4, 4));
	}
	template <typename... T>
	static Matrix vec2(T... t) {
		static_assert (sizeof...(t) == 2, "Invalid Argument Number");
		std::vector<double> dummy;
		(dummy.push_back(std::forward<T>(t)), ...);
		return Matrix(dummy, 1, 2);
	}

	template <typename... T>
	static Matrix&& vec3(T... t) {
		static_assert(sizeof...(t) == 3, "Invalid Argument Number");
		std::vector<double> dummy;
		(dummy.push_back(std::forward<T>(t)), ...);
		return Matrix(dummy, 1, 3);
	}

	template <typename... T>
	static Matrix&& vec4(T... t) {
		static_assert(sizeof...(t) == 4, "Invalid Argument Number");
		std::vector<double> dummy;
		(dummy.push_back(std::forward<T>(t)), ...);
		return Matrix(dummy, 1, 4);
	}

	const std::vector<double>& rawData() { return data; }

	class AccessDummy {
		friend class Matrix;
		size_t x;
		Matrix* m;
		const Matrix* cm;
		AccessDummy(size_t _x, Matrix& _m) :x(_x), m(&_m), cm(nullptr) {}
		AccessDummy(size_t _x, const Matrix& _m) :x(_x), m(nullptr), cm(&_m) {}
	public:
		double& operator[](size_t y) {
			if (y >= m->dimy)throw std::out_of_range("");
			return m->data[m->ptoi(x, y)];
		}
		const double& operator[](size_t y) const {
			if (y >= cm->dimy)throw std::out_of_range("");
			return cm->data[cm->ptoi(x, y)];
		}
	};

	AccessDummy operator[](size_t x) {
		if (x >= dimx)throw std::out_of_range("");
		return AccessDummy(x, *this);
	}

	const AccessDummy operator[](size_t x) const {
		if (x >= dimx)throw std::out_of_range("");
		return AccessDummy(x, *this);
	}

	Matrix operator*(const Matrix& oth) {
		if (oth.dimy != dimx)throw std::invalid_argument("");
		Matrix ret = Matrix(oth.dimx, dimy);
		for (int x = 0; x < oth.dimx; x++)
			for (int y = 0; y < dimy; y++) {
				double& target = ret[x][y];
				for (int k = 0; k < dimx; k++)
					target += (*this)[k][y] * oth[x][k];
			}
		return ret;
	}

	Matrix operator*(const int& oth) {
		Matrix ret = Matrix(dimx, dimy);
		for (int x = 0; x < dimx; x++)
			for (int y = 0; y < dimy; y++) {
				ret[x][y] = (*this)[x][y] * oth;
			}
		return ret;
	}
	friend Matrix operator*(const int& n, const Matrix& oth);

	Matrix operator+(const Matrix& oth) const {
		if (oth.dimx != dimx && oth.dimy != dimy)throw std::invalid_argument("");
		Matrix ret = Matrix(oth.dimx, dimy);
		for (int x = 0; x < oth.dimx; x++)
			for (int y = 0; y < dimy; y++)
				ret[x][y] = (*this)[x][y] + oth[x][y];
		return ret;
	}

	Matrix operator-(const Matrix& oth) const {
		if (oth.dimx != dimx && oth.dimy != dimy)throw std::invalid_argument("");
		Matrix ret = Matrix(oth.dimx, dimy);
		for (int x = 0; x < oth.dimx; x++)
			for (int y = 0; y < dimy; y++)
				ret[x][y] = (*this)[x][y] - oth[x][y];
		return ret;
	}

	Matrix& operator+=(const Matrix& oth) {
		if (oth.dimx != dimx && oth.dimy != dimy)throw std::invalid_argument("");
		for (int x = 0; x < oth.dimx; x++)
			for (int y = 0; y < dimy; y++)
				(*this)[x][y] += oth[x][y];
		return *this;
	}

	Matrix& operator-=(const Matrix& oth) {
		if (oth.dimx != dimx && oth.dimy != dimy)throw std::invalid_argument("");
		Matrix ret = Matrix(oth.dimx, dimy);
		for (int x = 0; x < oth.dimx; x++)
			for (int y = 0; y < dimy; y++)
				(*this)[x][y] -= oth[x][y];
		return *this;
	}

};

Matrix operator*(const int& n, const Matrix& oth) {
	Matrix ret = Matrix(oth.dimx, oth.dimy);
	for (int x = 0; x < oth.dimx; x++)
		for (int y = 0; y < oth.dimy; y++) {
			ret[x][y] = oth[x][y] * n;
		}
	return ret;
}
