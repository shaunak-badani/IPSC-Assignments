#include <bits/stdc++.h>
#define ll long long int

using namespace std;

typedef struct matrix {
	ll val[100][100];
	ll m;
	ll n;

	// constructor
	matrix(ll m, ll n) {
		for(int i = 0 ; i < m ; i++) {
			for(int j = 0 ; j < n ; j++) {
				this->val[i][j] = 0;
			}
		}
		this->m = m;
		this->n = n;
	}

	void print() {
		for(int i = 0 ; i < this->m ; i++) {
			for(int j = 0 ; j < this->n ; j++) {
				cout << this->val[i][j] << " ";
			}
			cout << endl;
		}
		cout << "---" << endl;
	}

	bool in_row(int k) {
		return k < this->m;
	}

	bool in_column(int k) {
		return k < this->n;
	}

}mat;

mat dense_matrix_mult(mat B, mat C) {
	if(B.n != C.m){
		cout << "Matrix dimensions do not match. Returning first matrix called" << endl;
		return B;
	}
	mat A = matrix(B.m, C.n);
	for(int i = 0 ; i < B.m ; i++) {
		for(int j = 0 ; j < C.n ; j++) {
			for(int k = 0 ; k < B.n ; k++) 
				A.val[i][j] += B.val[i][k] * C.val[k][j];
		}
	}
	return A;
}



int main() {
	ll n, m, inp;

	// INPUT MATRIX B WITH DIMENSIONS
	cin >> m >> n;
	mat B = matrix(m, n);

	for(ll i = 0 ; i < m ; i++) {
		for(ll j = 0 ; j < n ; j++) {
			cin >> inp;
			B.val[i][j] = inp;
		}
	}

	// INPUT MATRIX C WITH DIMENSIONS

	cin >> m >> n;
	mat C = matrix(m, n);

	for(ll i = 0 ; i < m ; i++) {
		for(ll j = 0 ; j < n ; j++) {
			cin >> inp;
			C.val[i][j] = inp;
		}
	}

	mat A = dense_matrix_mult(B, C);
	A.print();
}